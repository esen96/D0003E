## Real-Time Systems – Lab 3

----------

## A re-implementation of lab 1 using threads

In this assignment we will explore the benefits of threaded concurrency by revisiting the problem studied in lab 1. That is, the task is to implement a program that (1) blinks an LCD segment of your choice once a second, (2) computes an ascending chain of prime numbers and displays these on the LCD, and (3) reacts to joystick button presses, all at the same time.  
  
To this end you will need to incorporate most of your code developed in lab 1. To make the assignment slightly more interesting, we will limit prime number printouts to LCD positions 0 and 1 (the left-most positions), and require the accumulated key press count to be displayed at LCD at positions 4 and 5 (the right-most positions). Function  printAt()  from lab 2 can preferably be used for this purpose (restored to its original definition, of course, involving no global variables). The concurrency support will be the multi-threading kernel you developed in the second lab assignment, which provides the ability to spawn new threads and to lock and unlock mutex variables.  
  
How to use these features for best results is a design issue, though, and we will actually explore three alternative solutions in this assignment. The task is to implement them all and then draw some conclusions. Note: Although we will play with both the tinythreads kernel and the tasks that use its services in this assignment, a clear separation between the kernel and its clients is still utterly important. This means that the kernel should be kept in the file  _tinythreads.c_, and that client code that use the kernel services reside in other files that only access the services exported in  _tinythreads.h_.

_Examination: each part should be demonstrated separately, so please copy the necessary files to separate folders._

### Alternative 1: busy-waiting + time-sharing between threads

Here you will rely on the time-sharing features of your final tinythreads kernel from lab 2 and re-use the code for the three different tasks you developed in parts 1-3 of lab 1. Spawn two of the tasks as separate threads and confirm that all three tasks appear to run concurrently. Compare this solution to the one you created in lab 1, part 4. Also experiment with different period settings for the kernel timer. What happens to the button response time when the timer period is high (1 second or above)? Why?

_Hint_: the counter TCNT1 is used by the kernel and triggers an interrupt each 0.05s. You cannot use the same counter for blinking, but you can add a variable to  _tinythreads.c_  that will be increased each time TCNT1 generates an interrupt, and you can modify the code in the  blink()  function to read and reset this variable (not directly but via auxiliary methods).

### Alternative 2: thread activation by unlocking a mutex

We will now abandon time-sharing in favor of interrupt-driven concurrency. You must therefore start by removing the time-sharing features from your kernel, which means removing the timer interrupt handler and all timer register manipulation from  _tinythreads.c_. It will also be beneficial to change the scheduling policy slightly, so that newly arrived tasks are given more attention than old ones. Do this by modifying function  enqueue()  to insert a new thread in the  first  position of the given queue instead of the last. Also, modify spawn so that the newly created thread is started immediately. (This can be done in several ways; make sure that the ready queue is handled correctly.)  
  
Now make the following changes to your client code:

1.  Let calculations of prime numbers to continue in the main thread and spawn the two other tasks into separate threads.
2.  For  **each**  of the busy-waiting tasks, create a  **separate**  global mutex variable and replace busy-waiting with a  lock  operation on this mutex. From the start, these mutexes must be locked.
3.  Configure Timer/Counter1 to generate an interrupt twice a second, and let the interrupt handler perform an  unlock  operation on the blinking task mutex. Make sure that the unlock function changes the running thread in order to ensure that the newly signaled thread is started (For a correctly implemented unlock() this should already be the case.). This "asymmetric" use of the mutex operations is more commonly associated with the  semaphore  datatype, but it works for our implementation of mutexes just as well. Also note that even though you are likely to re-use most of your previously developed code for accessing the timer, that code is now part of the client application, and not the kernel.
4.  Configure the joystick port to generate an interrupt when the joystick button is pressed in the downward direction, and create a handler for this interrupt that unlocks the button count mutex.

Verify that the concurrent application still works as expected. Can you think of a program setting (like the timer interval in alternative 1 above) that would make the blinking jitter and button count response times deteriorate?

_Note_. A thread will block when trying to lock a mutex if the mutex has already been locked, either by another thread or (as is the case here) by the thread itself. In the second case, the mutex is locked as  _a means of synchronization_, since the thread will continue execution once the mutex has been unlocked from another thread or (as is the case here) from an interrupt handler.

### Alternative 3: interrupt-driven concurrency

Going one step further in the event-driven direction, we will now replace the blocking event-loops of alternative 2 with threads that just react to one event and then terminate. Implement the following changes:

1.  Remove all loops as well as the mutexes and the calls to  lock  in your button count and blinking code from the previous solution. Package what remains as functions of the type expected by the  spawn  primitive. If your functions need to refer to the state of a previous reaction, use global variables.
2.  Modify the two interrupt handlers so that each call to  unlock  is replaced by a call to the  spawn  instead.

Verify that this formulation of the solution works just as well as before.  
  

----------

When all three alternative solutions have been implemented and verified, draw your own conclusions regarding the different programming styles exploited here and in lab assignment 1. Which one do you prefer? What pros and cons do you identify? Note that there are no absolute right or wrong answers here, just express your educated opinion.  
  
As a final exercise, try to remove the prime number computing task from one of the last two alternative solutions. What happens and why? Can you suggest a generically applicable fix to this problem?
