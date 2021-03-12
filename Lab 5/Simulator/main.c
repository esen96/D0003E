#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */

void changeTrafficLights(uint8_t inSignal);
void *simulation(void *arg);
int initSerialPort(void);
void updateTrafficData(void);
void writeToPort(uint8_t data);
int blockedWait(void);
void *readFromPort(void *arg);
void sendNorthboundCar(void);
void sendSouthboundCar(void);
void *northBoundCar(void *arg);
void *southBoundCar(void *arg);

pthread_t portReadThread;

pthread_mutex_t trafficDataMutex;

int COM1;
int northQueue = 0; int southQueue = 0; int carsOnBridge = 0;
bool southGreen = false; bool northGreen = false; 




int main(void) {

	if (initSerialPort() != 0) {
		return -1;
	}

	pthread_create(&portReadThread, NULL, readFromPort, 0);


	updateTrafficData();

	blockedWait();
}

/* Prints current traffic data */
void updateTrafficData(void) {

	pthread_mutex_lock(&trafficDataMutex);

	printf("########################################## \n");
	printf("Cars in northern queue: %d \n", northQueue);
	printf("Cars in southern queue: %d \n", southQueue);
	if (northGreen) {
		printf("Traffic light status north: " GREEN "GREEN \n" RESET);
	}
	else {
		printf("Traffic light status north: " RED  "RED \n" RESET);
	}
	if (southGreen) {
		printf("Traffic light status south: " GREEN "GREEN \n" RESET);
	}
	else {
		printf("Traffic light status south: " RED  "RED \n" RESET);
	}
	printf("Cars on bridge: %d \n", carsOnBridge);
	printf("########################################## \n");

	pthread_mutex_unlock(&trafficDataMutex);
}

/* Blocked wait function to be run on main thread */
int blockedWait(void) {
	while (1) {
		fd_set rfds;					// make read set
		FD_ZERO(&rfds);					// empty set
		FD_SET(0, &rfds);				// include keyboard
		if (FD_ISSET(0, &rfds)) {
			char c = getchar();
			if (c == 'n') {
				northQueue++;
				writeToPort(0x1);
				updateTrafficData();
			}
			else if (c == 's') {
				southQueue++;
				writeToPort(0x4);
				updateTrafficData();
			}
			else if (c == 'e') {
				return 0;
			}
		}
	}
	return 0;
}

/* Sends a northbound car across the bridge */
void sendNorthboundCar(void) {
	northQueue--;
	carsOnBridge++;
	pthread_t car;
	pthread_create(&car, NULL, northBoundCar, NULL);
	writeToPort(0x2);
	updateTrafficData();
}

/* Sends a southbound car across the bridge */
void sendSouthboundCar(void) {
	southQueue--;
	carsOnBridge++;
	pthread_t car;
	pthread_create(&car, NULL, southBoundCar, NULL);
	writeToPort(0x8);
	updateTrafficData();
}

/* Thread pointer function emulating a northbound car crossing the bridge */
void *northBoundCar(void *arg) {
	sleep(5);
	carsOnBridge--;
	updateTrafficData();
	pthread_exit(NULL);
}

/* Thread pointer function emulating a southbound car crossing the bridge */
void *southBoundCar(void *arg) {
	sleep(5);
	carsOnBridge--;
	updateTrafficData();
	pthread_exit(NULL);
}

/* Function pointer for port reading thread */
void *readFromPort(void *arg) {
	while(1) {
		uint8_t Com1InSignal;
		if (read(COM1, &Com1InSignal, sizeof(Com1InSignal)) > 0) {
			changeTrafficLights(Com1InSignal);
		}
	}
}

/* Changes traffic lights according to incoming COM1 data */
void changeTrafficLights(uint8_t inSignal) {
	if (inSignal == 0x6) {
		southGreen = true;
		northGreen = false;
		sendSouthboundCar();
		
	}
	else if (inSignal == 0x9) {
		northGreen = true;
		southGreen = false;
		sendNorthboundCar();
	}
	else if (inSignal == 0xA) {
		northGreen = false;
		southGreen = false;
		updateTrafficData();
	}
}

/* Write output to COM1 serial port */
void writeToPort(uint8_t data) {
	if (write(COM1, &data, sizeof(data)) < 0) {
		printf("ERROR: Could not write to COM1 serial port!");
	}
}

/* Set serial port settings */
int initSerialPort(void) {
	COM1 = open("/dev/ttyS0", O_RDWR);
	if (COM1 < 0) {
		printf("ERROR: Cannot open COM1!");
		return -1;
	}
	struct termios config;
	if (tcgetattr(COM1, &config) < 0) {
		printf("ERROR: Cannot get termios attributes!");
		return -1;
	}

	if (cfsetispeed(&config, B9600) < 0 || cfsetospeed(&config, B9600) < 0) {
		printf("ERROR: Cannot configure COM1 bps!");
	}
	/* set 9600 baud rate, 8 bit word size, read enable, local line */
	config.c_cflag = B9600 | CS8 | CREAD | CLOCAL;

	/* block read until 1 character received to reduce CPU usage */
	config.c_cc[VMIN] = 1;

	if (tcsetattr(COM1, TCSANOW, &config) < 0) {
		printf("ERROR: Cannot set termios attributes!");
		return -1;
	}
	return 0;
}

