
include asm/Makefile.inc

FILE_SYSTEM_BASE = $(PRJ_ROOT)/rootfs
FILE_SYSTEM_BIN = $(FILE_SYSTEM_BASE)/bin

KERNEL_IMG = kernel.img
FILESYSTEM_IMG = filesystem.img
GRUB_IMG = grub.img
KERNEL_SOURCES = $(wildcard *.c)
KERNEL_DEPS = $(KERNEL_DEPS:.c=.d)
KERNEL_OBJECTS = $(KERNEL_SOURCES:.c=.o)



#all: $(C_DEPS)

#%.d: %.c
	#@set -e; rm -f $@; \
	#$(CC) $(INCLUDE) -MM $(CPPFLAGS) $< > $@.$$$$; \
	#sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	#rm -f $@.$$$$

## If target isn't 'clean' include C_DEPS
#ifneq ($(MAKECMDGOALS),clean)
#ifneq ($(strip $(C_DEPS)),)
#-include $(C_DEPS)
#endif
#endif


#VPATH=../scripts:.

#%.o: %.c *.h
	#$(TARGET_CC) -m32 -fno-stack-protector -Wall -nostdinc -fno-builtin -c $<

#%.o: %.s
	#$(TARGET_CPP) $< | $(TARGET_AS) -o $*.o

## Link using `link.ld'
#$(KERNEL_IMG): $(KERNEL_OBJECTS)
	#$(TARGET_LD) -T link.ld -o $(KERNEL_IMG) $(KERNEL_OBJECTS)

## Link using `link-user.ld'
#$(COREUTILS): %: %.o $(USER_OBJECTS)
	#$(TARGET_LD) -T link-user.ld -o $@ $(USER_OBJECTS) $@.o
