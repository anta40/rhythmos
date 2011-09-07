#USER_OBJECTS ?= crtso.o libc.o calls.o buddy.o
SOURCE = libc.c
OBJECTS = $(SOURCE:.c=.o)
DEPS = $(SOURCE:.c=.d)
