## Real-Time Systems – Lab 1

----------

## C programming on bare metal

The purpose of this assignment is to exercise C programming immediately on the bare metal computer hardware, and to concretely illustrate some of the fundamental limitations of ordinary sequential programming. The tasks to be solved might therefore appear a bit artificial, but they have been chosen more because of their clarity than because of their realism.

Setup Atmel studio according to the  [Atmel studio configuration guide](https://ltu.instructure.com/courses/12170/pages/atmel-studio-configuration-guide "Atmel studio configuration guide").

The assignment should be carried out in C targeting the  [AVR Butterfly](https://ltu.instructure.com/courses/12170/files/1871715?wrap=1 "doc4271_butterfly.pdf")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871715?wrap=1 "Preview the document")  board. Note: in order to make the on-board processor run at its maximum speed, its CPU clock prescaler functionality must be disabled. This is achieved by writing the byte values 0x80 followed by 0x00 to the special register CLKPR when your program starts up. See the  [ATmega169 manual](https://ltu.instructure.com/courses/12170/files/1871713?wrap=1 "ATmega169.pdf")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871713?wrap=1 "Preview the document"), pages 29-31, for further info. Moreover, you won't need to worry about termination in any of these tasks. The intended behavior of the programs you will write is that they will continue to run until the Butterfly board is reset.

Note: to access the register names and bit names used in the manuals, start you program with an include-directive  #include <avr/io.h>.

_Examination: you will need to demonstrate each part separately, but you can only present your solutions when you've completed all the parts. Make sure to arrange your code such that all parts can easily be presented.  
_

Part 1

The first task is to establish some means of printing numeric output on the LCD display. To this end you should design a C function  writeChar(char ch, int pos), that activates display segments at character position  pos  such that a visible representation of the character  ch  is obtained. The function should simply do nothing if the  pos  parameter is outside the range supported by the display (that is,  0..5, where  0  is the left-most position). Furthermore, because this assignment isn't primarily about graphics design, you don't have to support the entire ASCII character range – it is perfectly OK just to print a blank representation of any character outside the numeric characters range ('0'..'9').

Important information regarding the LCD control registers and the segment layout can be found in the  [AVR LCD driver manual](https://ltu.instructure.com/courses/12170/files/1871714?wrap=1 "butterfly_lcd.pdf")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871714?wrap=1 "Preview the document"), section 3, and in the  [ATmega169 manual](https://ltu.instructure.com/courses/12170/files/1871713?wrap=1 "ATmega169.pdf")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871713?wrap=1 "Preview the document"), pages 209-224. For device initialization it is recommended that the following values are used: drive time 300 microseconds, contrast control voltage 3.35 V, external asynchronous clock source, 1/3 bias, 1/4 duty cycle, 25 segments enabled, prescaler setting N=16, clock divider setting D=8, LCD enabled, low power waveform, no frame interrupt, no blanking. It is part of the assignment to figure out the actual control register bit patterns that correspond to these settings.

When you have verified that individual digits can be properly displayed at the desired position, write a function  writeLong(long i)  that first converts the long integer  i  to a string of characters, then calls  writeChar  with a proper position value for each of these characters. In case the string representation of  i  requires more than six characters, the function should only display the six  least  significant digits.

Finally, demonstrate your LCD driver by implementing a program that continuously computes increasing prime numbers and prints them on the display. The prime numbers shall be computed in the simplest possible way: a  long  variable is incremented for each turn in a loop, and a helper function  is_prime(long i)  is called to determine whether the value is a prime number and thus to be printed. The initial value for this variable can be any value you find meaningful. Implementing  is_prime(i)  is tentatively done by computing  i % n  (i.e., the remainder from division  i/n) for all  2 <= n < i, and returning false (0) if any such expression is 0, true (1) otherwise.

Wrap up you solution in a function  primes()  that is called directly from  main()  after device initialization.

_Examination: we will ask you to run your program and check your code for correct CPU clock prescaler; LCD initialization; implementation of the functions._

Part 2

The second task is to write a program that makes a segment of the LCD display blink (i.e., turn on and off) with a steady frequency of 1 Hz. The segment chosen should be one of the special symbols that are not part of the six generic character display areas. Unfortunately, the documentation is a bit vague regarding which bits in the LCD control registers that connect to these symbols, thus some amount of experimentation will be required. However, the possibilities are quite limited: the only control register bits not already tied to the generic character segments are bits 1, 2, 5 and 6 of registers LCDDR0, LCDDR1 and LCDDR2; as well as bit 0 of registers LCDDR3, LCDDR8, LCDDR13 and LCDDR18.

For timing purposes, the 16-bit Timer/Counter1 unit should be used. Details of this device can be found in pages 95-123 of the  [ATmega169 manual](https://ltu.instructure.com/courses/12170/files/folder/documentation?preview=1346725); however, most of this information is beyond the scope of this assignment. All we will need is a continuously running timer that can be read at any time and doesn't wrap around too often. A suitable configuration is to let Timer/Counter1 use the 8 MHz system clock with a prescaling factor of 256, this will make the 16 bits in register TCNT1 take more than one second before wraparound. The only register whose default values need to be changed in order to achieve this is TCCR1B. You are not allowed to reset the timer register TCNT1, but should handle the wraparound in your solution. (It can be tricky to write TCNT1, since the register is two bytes wide, but it could be done). Note also that some special measures need to be taken to ensure proper operation when the timer wraps around to zero.

Concretely, this program should maintain an unsigned integer variable representing the next timer value to wait for, and use busy-waiting to stop execution until register TCNT1 has reached that value. Note that blinking the display requires updating in two phases, that preferably last half the desired period.

Wrap up you solution in a function  blink()  that is called directly from  main()  after device initialization.

_Examination: we will ask you to run the program and check your code, including timer configuration and implementation of the functions._

Part 3

The third part of this assignments is about dealing with external input. The task is to write a program that toggles between two LCD display segments (pick two that aren't used in any of the previous parts of the assignment) whenever the small on-board joystick is activated. The joystick is connected to certain bits of I/O ports B and E; here it is recommended that bit 7 of port B is used, which corresponds to downward movement of the joystick.

Details on how to operate the I/O ports is contained in pages 55-78 of the  [ATmega169 manual](https://ltu.instructure.com/courses/12170/files/1871713?wrap=1 "ATmega169.pdf")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871713?wrap=1 "Preview the document"); however, as with the section on 16-bit timers, most of this information is beyond the scope of the assignment. What is necessary to know is that the I/O ports can function as both inputs and outputs, and that the output register for port  x  (PORTx) controls pull-up resistors for the input register of port x (PINx) whenever port  x  is configured for input (the latter is controlled individually for each bit of port  x  by the settings in register DDRx). All ports are configured as inputs by default, but in order to obtain proper operation of the joystick switches, the pull-up resistors must be activated for the corresponding pins. For our purposes, this amounts to setting bit 7 high in register PORTB during program initialization (the other bits of PORTB must be preserved).

The program should be implemented using busy-waiting for changes on bit 7 in register PINB. Each pressing of the joystick should result in a single event, regardless of the duration (i.e. pressing the joystick down, holding it down for some time and then releasing it should be treated as a single input). Notice that the joystick switches are active low, which means that they will cause a 0 in the input bit position as long as the switch is pressed, and a 1 otherwise.

Wrap up you solution in a function  button()  that is called directly from  main()  after device initialization.

_Examination: we will ask you to run the program and test it by pressing the joystick down for short and long periods of time. We will also check your implementation of the functions._

Part 4

The final part of the assignment consists of putting all the previous parts together as one single application. This will most certainly require some modifications to your existing code; in fact, the purpose of this part is specifically to draw your attention to these modifications. To this end,  you will not be allowed to modify any of your existing functions, instead you should create new copies of any functions that need do be changed. Calculating a single prime number should not be interrupted, i.e. you may only check the timer and the input register between calculating prime numbers.

Try out your program for different start values for the prime number computations, for example 1, 5000, and 25000. How do these values affect the display of the 1 Hz blinking segment? How is the response time of the joystick affected? What would it take to ensure a stable blinking display and a responsive joystick, independent of the currently calculated prime number?

What can you say about the structure of your combined program? Was some clarity of your individual solutions lost in the adaption process? Why wasn't it possible to simply call the original functions one after another in the main program?

_Examination: we will ask you to run the program calculating prime numbers that are larger than 25000. You will then have to answer the questions above.
