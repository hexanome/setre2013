# makfile configuration
NAME            = bargraph_test
OBJECTS         = test.o bargraph.o platform.o
CPU             = msp430f149
INCLUDE			= ../
VPATH			= ../

CFLAGS          = -mmcu=${CPU} -O2 -Wall -g -I${INCLUDE}

#switch the compiler (for the internal make rules)
CC              = msp430-gcc

.PHONY: all FORCE clean download download-jtag download-bsl dist

#all should be the first target. it's built when make is run without args
all: ${NAME}.elf ${NAME}.a43 ${NAME}.lst

#confgigure the next line if you want to use the serial download
download: download-jtag
#download: download-bsl

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS}

${NAME}.a43: ${NAME}.elf
	msp430-objcopy -O ihex $^ $@

${NAME}.lst: ${NAME}.elf
	msp430-objdump -dSt $^ >$@

${NAME}.dmp: ${NAME}.elf
	msp430-objdump -d $^ >$@

#main.pp.c: main.c
#	${CC} -E $(CFLAGS) $^ >$@

#main.s: main.c
#	${CC} -S $(CFLAGS) $^ >$@

download-jtag: all
	msp430-jtag -e ${NAME}.elf

download-bsl: all
	msp430-bsl -e ${NAME}.elf

clean:
	rm -f ${NAME}.elf ${NAME}.a43 ${NAME}.lst ${OBJECTS} wsim* *~

#backup archive
dist:
	cd ..; tar czf leds.tgz ot2007-leds/main.c  ot2007-leds/Makefile

#dummy target as dependecy if something has to be build everytime
FORCE:

#project dependencies
test.o: test.c 
bargraph.o: bargraph.c
platform.o: platform.c
