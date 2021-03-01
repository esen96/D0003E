## Real-Time Systems – Lab 4

----------

### Dual pulse generator with a GUI

  
In this assignment you will exercise programming according to the reactive objects model of  [TinyTimber](https://ltu.instructure.com/courses/12170/files/1871704/download?wrap=1 "TinyTimber.pdf")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871704/download?wrap=1 "Preview the document")  (start by reading the description in full), and in particular, gain acquaintance with event baselines and the  AFTER  primitive. The concrete task is to implement two separate  pulse generators  on the AVR Butterfly, and a Graphical User Interface (GUI) for controlling them using the on-board joystick and display.  
  
Our pulse generators will generate square waves; that is, periodic signals that alternate between low and high values with a 50% duty cycle. Output from the pulse generators could in principle be emitted through any of the six general I/O ports on the AVR microcontroller. However, most pins on these ports are actually already connected to various devices on the Butterfly board, so we will have to direct pulse generator output as follows:

-   Pulse generator 1 writes its output to bit 4 of port E, which is connected to pin 1 of socket J405.
-   Pulse generator 2 writes its output to bit 6 of port E, which is connected to pin 3 of socket J405.

See the  [AVR Butterfly User's Guide](https://ltu.instructure.com/courses/12170/files/1871715/download?wrap=1 "doc4271_butterfly.pdf")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871715/download?wrap=1 "Preview the document")  for information on the physical location of socket J405 and its pins.  
  
The period of each pulse generator should be individually controllable via a simple GUI on the LCD, operated by the joystick. The GUI should display two 2-digit numbers at positions 0 and 4 (counting from the right), standing for the current speed settings for pulse generators 1 and 2, respectively. The numbers should should adhere to the following rules:

-   Number should be frequency in Hz.
-   A value of 00 means a frequency of zero; i.e. a stopped pulse generator (emitting a constantly low output).

Two arbitrary chosen segments should be used to indicate which of the two pulse generators that is currently receiving user input. Moving the joystick to the right or to the left should switch between these states. Moving the joystick in the upward/downward direction should increase/decrease the setting for the pulse generator currently in focus. Pressing the joystick with a non-zero frequency should save the frequency and immediately set the frequency to zero. Pressing the joystick while zero should restore the setting to its previous saved value. If no save value exists, it can be assumed to be 0. Moreover, it should be possible to hold the joystick in the up or down position and thereby achieve the same effect as repeated input in the corresponding direction. Convenient values for the repetition period and a possible initial delay should be obtained by experimentation.

> _Important note:_ you may need to install handlers for more than one interrupt to receive all input from the joystick. Moreover, if a method  _m_  of some object A has been installed as an interrupt handler, then all methods of this object will be executed with interrupts disabled. Do not use SYNC() and AFTER() in any method of the object A; instead, make an asynchronous call using ASYNC() to some other object and process the interrupt there.

To implement the assignment you will need a copy of the TinyTimber kernel, which is split into a header file  [TinyTimber.h](https://ltu.instructure.com/courses/12170/files/1871718/download?wrap=1 "TinyTimber.h")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871718/download?wrap=1 "Preview the document")  and and implementation file  [TinyTimber.c](https://ltu.instructure.com/courses/12170/files/1871717/download?wrap=1 "TinyTimber.c")[![Preview the document](https://ltu.instructure.com/images/preview.png)](https://ltu.instructure.com/courses/12170/files/1871717/download?wrap=1 "Preview the document").  
  
The following generic stylistic guidelines are mandatory to your solution: each reactive object class definition  N  shall be confined to a designated pair of files called  N.h and  N.c. File  N.h shall contain a  _typedef_  for the class (i.e. struct)  N, an initialization macro named  initN(), and function headers for all methods supported by class  N. File  N.c shall contain the function definitions for the supported methods, and nothing else. The file  _main.c_  shall contain all object instances for the program as well as definitions of the top-level methods. Any remaining functionality (e.g. related to LCD handling) should be put in separate  _.h_  and  _.c_  files at your own discretion. No global variables whatsoever will be allowed in the program apart from the object instances defined in  _main.c_.  
  
In addition, the following constraints apply:

-   The design should contain a separate reactive object for each pulse generator and at least one additional reactive object that handles the GUI.
-   The two pulse generator objects should be instances of a single pulse generator class, which consequently must be parameterized with respect to the bit number of port E it wishes to control.
-   To avoid race conditions with two generators simultaneously reading and writing port E, a dedicated object should handle the actual writing of individual bits to this port.

It is advisable that the application be developed in steps, concentrating on one reactive object at a time. To verify that the pulse generator objects actually generate pulses of the correct frequency,  _connect an oscilloscope between ground (pin 4) and pin 1 or pin 3 of socket J405_. If your AVR does not have pins connected to socket, ask the lab supervisor to fix that for you. For an introduction to oscilloscopes, see  [this tutorial](https://ltu.instructure.com/courses/12170/files/1871727/download "Link").

### Examination

Demonstrate that the software works (using an oscilloscope) and answer the following questions (written answers are not necessary):

-   Explain the principles of implementing object orientation in C.
-   The TimyTimber kernel, if properly used, supports a  _reactive programming_ _model_  with  _object-level concurrency_. Explain these two terms.
-   When using the TinyTimber kernel, why is it important to invoke all object methods via kernel primitives (SYNC, ASYNC, AFTER, BEFORE) and not directly?
-   Explain the term  _race condition_.