## **Real-time Systems – Lab 5**

### A bridge access controller and simulator

This assignment is about implementing a program for controlling car access to a narrow bridge by means of traffic lights. The lab will consist of two parts that work together: The AVR butterfly will be the traffic light controller and the PC will be the car simulator. The PC and the AVR butterfly will communicate by means of a serial cable. If this lab assignment is implemented correctly, your controller should be able to control the environment simulated by any other lab group, and vice versa!

The general problem setting is as follows:

-   There is a narrow bridge with only a single lane, on which traffic must be able to pass in both directions.
-   There are red and green traffic lights at each entry to the bridge, and there is buffer space for queuing on both sides.
-   There is a sensor at the entry to each buffer space, indicating the arrival of new cars.
-   There is another sensor at each side that indicates cars entering the bridge.
-   Rules posted at each side of the bridge stipulate that only one car may enter the bridge each time the light turns green.  **However, more than one car can be on the bridge at any given time (provided that they are moving in the same direction).**
-   We assume (for the sake of simplicity) that the buffer spaces can store an infinite number of cars.

See the following figure for schematic layout.

![lab5_bridge.png](https://ltu.instructure.com/courses/12170/files/1871725/preview)

## Traffic light controller

Your first task is to construct the software of the computer that controls the traffic lights. This controller should be implemented as a reactive object design on the AVR Butterfly using the  [TinyTimber (Links to an external site.)](http://www.sm.luth.se/csee/courses/d0003e/tinytimber_120116/TinyTimber.pdf)  kernel (source files:  [TinyTimber.h](https://ltu.instructure.com/courses/12170/files/1871697/download?wrap=1 "TinyTimber.h")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871697/download?wrap=1 "Preview the document")  and  [TinyTimber.c](https://ltu.instructure.com/courses/12170/files/1871696/download?wrap=1 "TinyTimber.c")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871696/download?wrap=1 "Preview the document")).

### Communication with simulated environment

Sensor input to the controller will be multiplexed all signals over a single channel: the receiver side of the Butterfly's Universal Synchronous and Asynchronous Serial Receiver and Transmitter (USART). A reception interrupt on the USART may mean any combination of 4 sensor signals, and we will use the received data value to identify individual sensors according to the following table:

-   Bit 0: Northbound car arrival sensor activated
-   Bit 1: Northbound bridge entry sensor activated
-   Bit 2: Southbound car arrival sensor activated
-   Bit 3: Southbound bridge entry sensor activated

Likewise, we will use the transmitter side of the USART for implementing the four output signals that control the status of each lamp of the traffic lights. The four signals will map onto the bits of the transmit data register as follows:

-   Bit 0: Northbound green light status
-   Bit 1: Northbound red light status
-   Bit 2: Southbound green light status
-   Bit 3: Southbound red light status

See the  [ATmega169 (Links to an external site.)](http://www.sm.luth.se/csee/courses/d0003e/labs/ATmega169.pdf)  manual pages 151 and onwards for detailed info about the USART. Note: The manual is written for an older version of the avr/io.c libraries and the register names have sometime changed slightly in order to be more "compatible" with larger AVR microcontrollers that include several USART ports. Many registers should include a '0' in the code you're going to write. Ex. UCSRB in the manual would be UCSR0B in the available avr/io C code libraries.

### Controller logic

The core task of the controller is to control the status of the four lamps on basis of just sensor inputs and the passage of time. The controller logic is subject to the following requirements:

-   It must be safe; i.e., cars going in opposite directions must never be allowed on the bridge at the same time.
-   It may not stop making progress. This means that the situation where the bridge is empty but queued cars are not allowed to enter the bridge must never arise.
-   It may not cause starvation. That is, the controller must make sure that northbound cars are not indefinitely delayed by a steady stream of southbound cars, and vice versa.
-   If there is a queue of cars from a particular direction, better bridge utilization will be achieved by letting more than one car pass from the same direction before allowing cars from the other direction. Delay between cars from the same direction should be 1 second.
-   For our particular scenario, we will assume that a reasonable time for passing the bridge once it is entered is 5 seconds.
-   It is, however,  _not_  required to deal with cars that stop on the bridge, make u-turns, ignore green lights, ignore red lights, ignore the one-car-per-green restriction, drive at extreme speeds (high and low), etc. These are not unrealistic assumptions – few real traffic light systems are designed to detect (say) cars that stop in the middle of a crossing due to engine failure.

### Controller user interface

The display of the AVR butterfly should print at least the following information:

-   The length of the queue of cars waiting to enter the bridge in northbound direction.
-   The length of the queue of cars waiting to enter the bridge in southbound direction.
-   The number of cars currently on the bridge.

This can be achieved by conceptually dividing the display in three parts, with two digits each.

## Simulated environment (cars)

The task of the simulated environment is to maintain a simple visualization of the bridge status, the traffic lights, and a dynamic set of cars that arrive at the sides of the bridge, wait in queues, and eventually cross the bridge. The simulator does not make any decisions, but only behaves according the instructions from the controller (to enter the bridge) and notifies the controller about actions (arrive and enter).

### Simulator user interface

The following information is the minimum to display in the user interface:

-   The length of the queue of cars waiting to enter the bridge in northbound direction.
-   The length of the queue of cars waiting to enter the bridge in southbound direction.
-   The traffic light status (best to display in four positions, for red-south, green-south, red-north, green-north).
-   The number of cars currently on the bridge.

At minimum this information can be displayed on a single line of text (well formatted and readable), which is repeatedly reprinted when something changes.

The input to the car simulator should be a simple one-character input, confirmed by the enter key. The following characters should be supported (at least):

-   “s”: enqueues a new car in southbound direction
-   “n”: enqueues a new car in northbound direction
-   “e”: exits the simulator

When cars are enqueued, the controller should be notified accordingly. We assume (again for the sake of simplicity) that cars always cross the bridge at maximum speed (and thus never stop or go back once they have started crossing). Cars that have crossed the bridge disappear from the simulation.

### Communication with controller

To connect to the controller application, the simulated environment should use serial port COM1 (also known as /dev/ttyS0), connected to the AVR Butterfly USART port via a null modem cable. That is, the environment simulator should  _receive_  lamp status updates according to the bit layout described above for the controller, while  _transmitting_  sensor signals that follow the layout expected by the controller. The following schematic diagram illustrates how the input and output signals of the controller and the simulator fit together.

Note: The cable used to connect the AVR to the serial port of the PC has only one valid way to be connected. It will not work correctly if reversed. It is recommended to test the cable in order to figure out how to connect it, as well as reading manuals to figure out where to connect it.

![lab5_communication.png](https://ltu.instructure.com/courses/12170/files/1871726/preview)

One important point to notice is that reception of lamp signal data represents a  _desired state of the lamps_, which means that it does not matter if the status of some lamp is set to on several times in a row – the lamp will simply stay on in this case. This should be contrasted to the sensor signals generated as input to the controller; here each active bit in each message stands for a new sensor activation, and thus a series of identical sensor messages means something different than a single message. This distinction is sometimes referred to as  _gated_  vs.  _edge-triggered_  logic in digital electronics.

## Implementation details

Each of your programs may, and should, treat the other as a black-box. Both programs receive data from the serial port on the PC (the simulator from /dev/ttyS0 and the AVR controller from the physical port via the USART on the butterfly card), process the received data and transmit a new output to the serial port for the other program to receive and process.

The controller will be a reactive objects design that runs on the AVR Butterfly on top of the TinyTimber kernel.

Observe that, since there is no sensor for cars leaving the bridge, the timing for when a car leaves the bridge needs to be maintained in both the simulator and the controller (which is perfectly realistic – a real traffic light also maintains this timing, or assumption of timing).

The simulator implementation on the PC host will use POSIX threads (pthreads) as its concurrency mechanism. In order to obtain better POSIX compatibility, and also to make the resulting program portable to non-Windows platforms, the simulator will be compiled and executed with the help of a terminal-based environment under  [Cygwin (Links to an external site.)](http://cygwin.com/)  – a Windows extension that provides a typical Unix programming environment including the gcc compiler, C libraries and includes files, and an assortment of Unix utility tools and command shells. It is also possible to compile and run the simulation on any linux-platform with the correct libraries and dependencies available.  
  
You will be required to use POSIX system calls in your C code. Manual pages for system calls and other library information are generally available via the man command on a Unix system, but under Cygwin the man information isn't always complete. For better Unix manual page coverage, see the online documentation resource  [UnixWare 7 (Links to an external site.)](http://uw714doc.sco.com/cgi-bin/manform).  
  
The design of the simulated environment can be done in numerous ways, using any number of threads. One particularly interesting design – not the least because of its sharp contrast to the reactive objects model – will use a dedicated thread for each simulated car, and literally use semaphore operations to stop and restart the car whenever the simulation so requires. Likewise, car speed can be controlled by means of delay statements. Anyway, whatever design you choose, make sure it is clear and comprehensible. It is especially important that the design is robust in the sense that it is able to handle all forms of input in any state.  
  
For communication with the AVR Butterfly, the serial port COM1 should be used. Under Cygwin, the COM1 port is accessed as an ordinary file named /dev/ttyS0. Note: If nothing is physically plugged into the port COM1, you will be able to write, but not read (since no data is sent). A serial port speed of 9600bps (bits per seconds) should be used with 8 bits data word, no parity bits and one stop bit (this is commonly denoted 9600N81).

Manual page  [termios (Links to an external site.)](http://uw714doc.sco.com/en/man/html.3C/termios.3C.html)  describes various system calls for controlling the characteristics of a serial port (referenced as a  _file descriptor_  obtained from previous  [open (Links to an external site.)](http://uw714doc.sco.com/en/man/html.2/open.2.html)). The keyboard and the terminal screen will automatically connect to your program as the stdin and stdout streams, respectively. See the manual page  [stdio (Links to an external site.)](http://uw714doc.sco.com/en/man/html.3S/Intro.3S.html)  for more information. If desirable, a more advanced output than the required text where text placement and appearance is can be controlled is to use  [ANSI terminal control sequences. (Links to an external site.)](http://www.termsys.demon.co.uk/vtansi.htm)If you're interested in digging yourself more deeply into terminal user interface implementation, ncurses is a very commonly used and powerful library.

### Serial port communication hints and guidelines

Consult the  [Asyncronous serial communication (Links to an external site.)](http://en.wikipedia.org/wiki/Asynchronous_serial_communication)  page for better understanding of how especially start/stop bits work over serial line.

In order to be able to properly test the serial communication, it is beneficial to not use your own programs at both ends to start withm, since it can be hard to track down in which program the problem is located. Start with implementing a program that sends or receives a single character (or sends back the received character, called “echoing”) on the AVR butterfly. Use a program like PuTTY to open a terminal to the serial port (with the correct speed and stop bit settings) to type single characters and se if they are received by the AVR (make sure “local echo” or similar is not turned on in PuTTY (it’s off by default)). This way, the AVR-side can be debugged separately from the the simulator.