include Makefile.inc

# Host defs
CC = gcc
CPP = cpp
LD = ld
AS = as

# Target defs
ARCH = $(shell arch)
ifeq ($(ARCH),x86_64)
HOST_CFLAGS = 
TARGET_CC = i386-elf-gcc
TARGET_CPP = i386-elf-cpp
TARGET_LD = i386-elf-ld
TARGET_AS = i386-elf-as
else
HOST_CFLAGS = -m32
TARGET_CC = gcc
TARGET_CPP = cpp
TARGET_LD = ld
TARGET_AS = as
endif


INCLUDE = -I../include
CFLAGS = -m32 -fno-stack-protector -Wall -nostdinc -fno-builtin

%.o: %.c
	$(TARGET_CC) $(INCLUDE) $(CFLAGS) -c -MM -MP -MF $(@:%.o=%.d) -MT $(@:%.o=%.d) -o $@ $<

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif


# All Target
all: $(OBJECTS)

clean:
	-rm $(OBJECTS)

clean-deps:
	-rm $(DEPS)

.PHONY: all clean clean-deps
