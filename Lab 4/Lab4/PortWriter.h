#include "TinyTimber.h"

#ifndef PORT_H_
#define PORT_H_

typedef struct {
	Object super;
} PortWriter;

void deactivate(PortWriter *self, int arg);
void write(PortWriter *self, int arg);

#define initPortWriter() { initObject() }

#endif /* PORT_H_ */