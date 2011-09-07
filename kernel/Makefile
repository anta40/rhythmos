VPATH=../includes: ..lib

# Target defs
ARCH = $(shell arch)
ifeq ($(ARCH),x86_64)
TARGET_CC = i386-elf-gcc
TARGET_CPP = i386-elf-cpp
TARGET_LD = i386-elf-ld
TARGET_AS = i386-elf-as
HOST_CFLAGS = -Wall
else
TARGET_CC = gcc
TARGET_CPP = cpp
TARGET_LD = ld
TARGET_AS = as
HOST_CFLAGS = -Wall -m32
endif

INCLUDE = -I../include
include Makefile.inc
include asm/Makefile.inc
include ../lib/Makefile.inc
#C_SRCS = $(wildcard *.c)
#C_DEPS = $(C_SRCS:.c=.d)
#C_OBJS = $(C_SRCS:.c=.o)
#C_PROGS = $(basename $(C_SRCS))
USER_OBJECTS = crtso.o libc.o calls.o buddy.o


TARGET_CFLAGS = \
-Wall \
-Wextra \
-m32 \
-fno-stack-protector \
-nostdinc \
-fno-builtin

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) $(INCLUDE) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# If target isn't 'clean' include C_DEPS
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

VPATH=../scripts:.

all: objects default $(KERNEL_IMG)

objects: $(COREUTILS_OBJECTS)

default: $(COREUTILS_OBJECTS) $(COREUTILS)

%.o: %.c $(C_OBJS)
	$(TARGET_CC) $(INCLUDE) $(TARGET_CFLAGS) -c $< 

%.o: %.s
	$(TARGET_CPP) $< | $(TARGET_AS) -o $*.o

%.o: %.c *.h
	$(TARGET_CC)  -c $<

%.o: %.s
	$(TARGET_CPP) $< | $(TARGET_AS) -o $*.o

## Link using `link.ld'
$(KERNEL_IMG): $(KERNEL_OBJECTS)
	$(TARGET_LD) -T link.ld -o $(KERNEL_IMG) $(KERNEL_OBJECTS)

## Link using `link-user.ld'
$(COREUTILS): %: %.o $(USER_OBJECTS)
	$(TARGET_LD) -T link-user.ld -o $@ $(USER_OBJECTS) $@.o


.PHONY: clean clean-all
clean:
	-rm --force $(C_PROGS) $(C_OBJS) 

clean-all:
	-rm --force $(C_PROGS) $(C_OBJS) $(C_DEPS)
