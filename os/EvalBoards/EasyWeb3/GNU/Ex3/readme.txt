--------------------------------------------------------------------------------
 uC/OS-II Port for TI MSP430F1611 using GCC 3.2.3

 Base on the IAR port by: Jian Chen (yenger@hotmail.com) and
                          Jean Labrosse

 Version 2.77 of uC/OS-II was used to test the port.
--------------------------------------------------------------------------------

Notice: The attached files comprise only the MSP430 specific porting files.  You
        need to purchase the book in order to get the RTOS source code.  Refer
        to www.micrium.com for links to purchase the book.

This port uses the free GNU GCC crosscompiled for MSP430.  A variety of MSP430
variants are supported by the tools.  This port targets the MSP430F1611 which
includes 10k of on-chip SRAM.  There is no external bus, so this is all the 
RAM you get for application data and stacks.

You can download binaries and source distributions for the tools at SourceForge.
I used the 20040723 version of the Windows binaries which come in a
self extracting installer.  Everything works very well on Win2k.  A link to the
SourceForge project page is here: 

http://sourceforge.net/project/showfiles.php?group_id=42303

After downloading and installing the toolchain, you should be able to extract
the port files taking care to preserve the directory structure and run the
makefile to build the sample test application in the 'Ex1' directory.  If you
change the installation location, or the location of your uC/OS-II sources is
different from the prescribed location C:\Micrium\Software\uCOS-II\Source, then
you may have to adjust the paths in the makefile.

The port was tested using a MSP-FET430P140 development kit which includes a
small socketed development board and a JTAG emulation tool.  NOte that the board
ships with samples of the MSP430F149 as I recall, but I used a MSP430F1611 instead.
The two parts are pin compatible.  The only change
made to the board was to add a 3.6864MHz crystal to the XT2 pins.  This isn't
necessary to test the port as the onboard DCO can be used instead.  The system
tick is sourced from the TIMERA CCR0 interrupt at about 100 ticks/sec.  I didn't
check this too closely...you may want to check my clock division and CCR0 reload
value set in the ISR.  The compilers default linker script is used (since we don't
explicitly specify one in the makefile) which locates code and initialized data in
the flash and the .bss of course is in RAM.  If you really strip things down you can
change the make file to use ram.ld (included in the .zip file) to locate everything
in the 10K SRAM, but you really won't be able to do much practical.  This very simple
example blinks the LED on the FET430P140 at approximately 1Hz.

To program the microcontroller using the FET tool and JTAG you can simply use
the msp430-jtag program that comes with the GNU tools for MSP430.  You can use
msp430-objcpy to convert an .elf output file into a suitable hex file.  Once
you have a hex file you can invoke msp430-jtag at a command line similar to
this: msp430-jtag -epv uCOSTest.a43.  What this command does is bulk erases
the controller, programs uCOSTest.a43 into the flash, and then verifies the
code just written against the source file.  I have noticed intermittent 
results with the tool actually releasing the reset line, so you may want to
disconnect the tool an reset to watch your program run.  When the program is
controlled/debugged/run via gdb everything is fine with respect to reset.

To debug, you can use msp430-gdbproxy along with msp430-gdb.  The gdb-proxy
listens on a TCP socket and converts debugging commands from gdb into the
appropriate JTAG signals to drive the FET emulation tool. A simple deguggning
sequence goes like:

1.) Build you output file.  The compiler generates and .elf format executable.
2.) Start a DOS shell or a cygwin bash shell.
3.) In the shell enter: msp430-gdbproxy --port=2000 msp430
4.) Now in another DOS shell or cygwin bash shell enter: msp430-gdb -nw
5.) At the gdb prompt type: target remote localhost:2000
6.) At the gdb prompt type: monitor reset
7.) At the gdb prompt type: load uCOStest.elf (or the name of your .elf output)
8.) When it finishes loading, put a breakpoint at main by typing: br main
9.) Press c to continue execution of your program up to main
10.) You can continue to run your program by pressing 'c' again to continue.

You can also use Insight or another graphical front end to gdb, but the steps
above would be the minimum to get started.

Best regards,
Ryan A. Sherry
rsherry@rascontrols.com