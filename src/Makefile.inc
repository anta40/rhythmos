
FILE_SYSTEM_BASE = ../rootfs
FILE_SYSTEM_BIN = $(FILE_SYSTEM_BASE)/bin

SOURCE = $(wildcard *.c)
OBJECTS = $(SOURCE:.c=.o)
DEPS = $(SOURCE:.c=.d)
PROGS = $(basename $(SOURCE))

COREUTILS = sh ls cat find pwd echo hello dsh kill
COREUTILS_OBJECTS = $(addsuffix .o, $(COREUTILS))
USER_OBJECTS = crtso.o ../lib/libc.o calls.o buddy.o
