
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
C_SRCS = $(wildcard *.c)
C_DEPS = $(C_SRCS:.c=.d)
C_OBJS = $(C_SRCS:.c=.o)
C_PROGS = $(basename $(C_SRCS))
USER_OBJECTS = crtso.o libc.o calls.o buddy.o

TARGET_CFLAGS = -m32 -fno-stack-protector -Wall -nostdinc -fno-builtin

KERNEL_IMG = kernel.img
FILESYSTEM_IMG = filesystem.img
GRUB_IMG = grub.img
KERNEL_SOURCES = $(wildcard *.c)
KERNEL_DEPS = $(KERNEL_DEPS:.c=.d)
KERNEL_OBJECTS = $(KERNEL_SOURCES:.c=.o)



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

all: default
objects: $(COREUTILS_OBJECTS)

default: $(COREUTILS_OBJECTS) $(COREUTILS)

boot: default fs
	./mk_boot_image.sh

fs: default
	./mkfstree.sh
	cp $(COREUTILS) $(TESTS) $(FILE_SYSTEM_BIN)
	./mk_filesystem_image.sh

fstool: fstool.c filesystem.c filesystem.h
	$(CC) $(HOST_CFLAGS) -DUSE_UNIXPROC -DUSERLAND -Wall -o fstool fstool.c filesystem.c

testbuddy: testbuddy.c buddy.c buddy.h
	$(CC) $(HOST_CFLAGS) -DUSERLAND -Wall -o testbuddy testbuddy.c buddy.c

%.o: %.c $(C_OBJS)
	$(TARGET_CC) $(INCLUDE) $(TARGET_CFLAGS) -c $< 

%.o: %.s
	$(TARGET_CPP) $< | $(TARGET_AS) -o $*.o


# Link using `link.ld'
$(KERNEL_IMG): $(KERNEL_OBJECTS)
	$(TARGET_LD) -T link.ld -o $(KERNEL_IMG) $(KERNEL_OBJECTS)

# Link using `link-user.ld'
$(C_PROGS): %: %.o $(USER_OBJECTS)
	$(TARGET_LD) -T link-user.ld -o $@ $(USER_OBJECTS) $@.o

$(USER_OBJCTS): %: %.o $(USER_OBJECTS)
	$(TARGET_LD) -T link-user.ld -o $@ $(USER_OBJECTS) $@.o	

$(TESTS): %: %.o $(USER_OBJECTS)
	$(TARGET_LD) -T link-user.ld -o $@ $(USER_OBJECTS) $@.o


# Misc targets
.PHONY : run run-qemu install clean
run: |run-qemu

run-qemu: boot
	qemu -daemonize -fda $(GRUB_IMG)

install:

clean:
	-rm --force $(COREUTILS) $(COREUTILS_OBJECTS)

#clean:
#	-rm --force $(KERNEL_IMG) $(KERNEL_OBJECTS) $(FILESYSTEM_IMG) \
#	$(USER_OBJECTS) \
#	$(COREUTILS) $(COREUTILS_OBJECTS) \
#	$(TESTS) $(TESTS_OBJECTS) \
#	$(TEST_UTILS) $(TEST_UTILS_OBJECTS) \
#	$(PROGRAMS) $(PROGRAMS_OBJECTS)
