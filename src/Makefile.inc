
FILE_SYSTEM_BASE = $(PRJ_ROOT)/rootfs
FILE_SYSTEM_BIN = $(FILE_SYSTEM_BASE)/bin

C_SRCS = $(wildcard *.c)
C_DEPS = $(C_SRCS:.c=.d)
C_OBJS = $(C_SRCS:.c=.o)
C_PROGS = $(basename $(C_SRCS))

COREUTILS = sh ls cat find pwd echo hello dsh kill
COREUTILS_OBJECTS = $(addsuffix .o, $(COREUTILS))
USER_OBJECTS = crtso.o libc.o calls.o buddy.o
