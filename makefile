#       Makefile
#
#       Copyright 2011 Dustin Dorroh <dustindorroh@gmail.com>
#
PRJ = .
SRC = src
SCRIPTS = scripts
TOOLS = tools
IMAGES = images
ROOTFS = rootfs
LIB = lib
TESTS = tests
INCLUDE = -Iinclude
KERNEL = kernel
KERNEL_ASM = kernel/asm
FS = fs
DOC = doc


# Host defs
CC = gcc
CPP = cpp
LD = ld
AS = as
TARGET_CFLAGS = \
-Wall \
-Wextra \
-m32 \
-fno-stack-protector \
-nostdinc \
-fno-builtin


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

# IMAGES
KERNEL_IMG = $(IMAGES)/kernel.img
FILESYSTEM_IMG = $(IMAGES)/filesystem.img
GRUB_IMG = $(IMAGES)/grub.img

# LIBC DEFS
TARGET_LIBC_SRC = libc.c
TARGET_LIBC_SRC = $(addprefix $(LIB)/,$(TARGET_LIBC_SRC))
#TARGET_LIBC_OBJ = $(TARGET_LIBC_SRC:.c=.o)

# KERNEL DEFS
KERNEL_SRC = buddy.c filedesc.c filesystem.c fscalls.c interrupts.c list.c \
	main.c page.c pipe.c process.c segmentation.c syscall.c unixproc.c
KERNEL_SRC = $(addprefix $(KERNEL)/, $(KERNEL_SRC))
#KERNEL_OBJ = $(KERNEL_SRC:.c=.o)

KERNEL_ASM = calls.s crtso.s start.s
KERNEL_ASM = $(addprefix $(KERNEL_ASM)/, $(KERNEL_SRC))
#KERNEL_ASM_OBJ = $(KERNEL_ASM_SRC:.s=.o)

#USER_OBJECTS = crtso.o calls.o libc.o buddy.o

# Host and Target programs
COREUTILS_SRC = \
sh.c \
ls.c \
cat.c \
find.c \
pwd.c \
echo.c \
hello.c \
dsh.c \
kill.c
COREUTILS_SRC =$(addprefix $(SRC)/, $(COREUTILS_SRC))

#COREUTILS_SRC = $(wildcard *.c)
#COREUTILS_OBJ = $(addsuffix .o, $(TARGETS))
#COREUTILS_PROG = $(basename $(wildcard *.c))
#
#COREUTILS_SRC =$(addprefix $(SRC)/, $(COREUTILS_SRC))
#COREUTILS_OBJ = $($(COREUTILS_SRC):.c=.o)
#COREUTILS = $($(COREUTILS_SRC):.c=)

PROGRAMS_SRC =
PROGRAMS_OBJ =
PROGRAMS =

HOST_UTILS_SRC = $(TOOLS)/fstool.c
#HOST_UTILS_OBJ = $(BUILD)/fstool.o
#HOST_UTILS = $(build)/fstool

HOST_TESTS_SRC = tests/testbuddy.c
#HOST_TESTS_OBJ = build/testbuddy.o
#HOST_TESTS = build/testbuddy

TARGET_UTILS_SRC =
TARGET_UTILS_OBJC =
TARGET_UTILS_SRC =

TARGET_TESTS_SRC = build/mptest.c
#TARGET_TESTS_OBJ = build/mptest.o
#TARGET_TESTS = build/mptest



# Host and Target program objects
#PROGRAMS_OBJECTS = $(addsufix .o, $(PROGRAMS))
#PROGRAMS_SRC = $(addsufix .c, $(PROGRAMS))
#HOST_UTILS_OBJECTS = $(addsufix .o, $(HOST_UTILS))
#HOST_TEST_UTILS_SRC = $(addsufix .c, $(HOST_TEST_UTILS))
#HOST_TESTS_OBJECTS = $(addsuffix .o, $(HOST_TESTS))
#HOST_TESTS_SRC = $(addsuffix .c, $(HOST_TESTS))
#TARGET_UTILS_OBJECTS = $(addsufix .o, $(TARGET_UTILS))
#TARGET_UTILS_SRC = $(addsufix .c, $(TARGET_UTILS))
#TARGET_TESTS_OBJECTS = $(addsuffix .o, $(TARGET_TESTS))
#TARGET_TESTS_SRC = $(addsuffix .c, $(TARGET_TESTS))

# File system defs
FILE_SYSTEM_BASE = $(PRJ_ROOT)/rootfs
FILE_SYSTEM_BIN = $(FILE_SYSTEM_BASE)/bin

#OBJPROG = $(addprefix $(OBJDIR)/, $(PROGS))




all: default $(FILESYSTEM_IMG) $(GRUB_IMG)

$(KERNEL_IMG):

default: $(HOST_UTILS) $(HOST_TESTS) $(TARGET_LIBC) $(COREUTILS) $(PROGRAMS) $(KERNEL_IMG)

fstool: fstool.c filesystem.c filesystem.h
	$(CC) $(INCLUDE) $(HOST_CFLAGS) -DUSE_UNIXPROC -DUSERLAND -o fstool fstool.c filesystem.c

testbuddy: testbuddy.c buddy.c buddy.h
	$(CC) $(INCLUDE) $(HOST_CFLAGS) -DUSERLAND -o testbuddy testbuddy.c buddy.c

cc -c $(CFLAGS) $^ -o $@


%.o: %.c *.h
	$(TARGET_CC) $(TARGET_CFLAGS)  -c $<

%.o: %.s
	$(TARGET_CPP) $< | $(TARGET_AS) -o $*.o

$(FILESYSTEM_IMG): default
	./mkfstree.sh
	cp $(COREUTILS) $(TESTS) $(FILE_SYSTEM_BIN)
	./mk_filesystem_image.sh

$(GRUB_IMG): default $(FILESYSTEM_IMG)
	./mkfstree.sh
	cp $(COREUTILS) $(TESTS) $(FILE_SYSTEM_BIN)
	./mk_filesystem_image.sh

# Link using `link.ld'
$(KERNEL_IMG): $(KERNEL_OBJECTS)
	$(TARGET_LD) -T link.ld -o $(KERNEL_IMG) $(KERNEL_OBJECTS)

# Link using `link-user.ld'
$(COREUTILS): %: %.o $(USER_OBJECTS)
	$(TARGET_LD) -T link-user.ld -o $@ $(USER_OBJECTS) $@.o

$(TARGET_TESTS): %: %.o $(USER_OBJECTS)
	$(TARGET_LD) -T link-user.ld -o $@ $(USER_OBJECTS) $@.o

# Misc targets
.PHONY : run run-qemu install clean
run: |run-qemu

run-qemu: boot
	qemu -daemonize -fda $(GRUB_IMG)

install:

clean:
	-rm --force $(KERNEL_IMG) $(FILESYSTEM_IMG)
	$(KERNEL_OBJECTS) $(USER_OBJECTS) \
	$(COREUTILS) $(COREUTILS_OBJECTS) \
	$(PROGRAMS) $(PROGRAMS_OBJECTS) \
	$(TARGET_TESTS) $(TARGET_TESTS_OBJECTS) \
	$(HOST_TEST_UTILS) $(HOST_TEST_UTILS_OBJECTS)
