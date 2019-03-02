#release: all
#	psppacker 400I resurrection.prx

TARGET = resurrection
OBJS = crt0.o main.o install.o nandoperations.o flash_format.o idstools.o dump.o tools.o ./S/DcManager.o ./S/pspPSAR.o ./S/pspDecrypt.o ./S/pspIplUpdate.o ./S/idsRegeneration.o

INCDIR = ./include
CFLAGS = -O2 -G0 -DPSPFW3xx -Wall -fshort-wchar -fno-pic -mno-check-zero-division 
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

LIBDIR = ./lib
LDFLAGS = -nostdlib -nodefaultlibs
LIBS = -lpsppower -lpsprtc -lvlfgui -lvlfgu -lvlfutils -lvlflibc -lpspreg -lpspsystemctrl_user -lpspkubridge -lpspwlan
# -lpspusb -lpspusbstor

PSP_FW_VERSION = 500

BUILD_PRX = 1

PSPSDK=$(shell psp-config --pspsdk-path)
#include ./build.mak
include $(PSPSDK)/lib/build.mak



install:
	@cp resurrection.prx "G:/TM/DC9/kd/resurrection.prx"