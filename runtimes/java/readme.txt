10:17 2007-04-12
To run make in this directory, you must first create a file called "setvariables.mk". See the Makefile for instructions.

13:47 2008-07-28
All classes that aren't extended should be final, as calling final methods isn't virtual, and should be faster.

11:07 2008-08-25
To use Static mode:
* define MOSYNC_STATIC_JAVA in config.h
* compile runtime
* in testPrograms:
**  set ELIM=java
**  use build.bat or bdir.bat
**  this will modify runtime to include the static program
* now you can run the runtime in the Java emulator.
