# ENDIAN is either EL (little endian) or EB (big endian)
ENDIAN         := EL

ifeq ($(ENDIAN),EL)
QEMU           := qemu-system-mipsel
else
QEMU           := qemu-system-mips
endif
CROSS_COMPILE  ?= mips-linux-gnu-
CC             := $(CROSS_COMPILE)gcc
CFLAGS         += --std=gnu99 -$(ENDIAN) -G 0 -mno-abicalls -fno-pic \
                  -ffreestanding -fno-stack-protector -fno-builtin \
                  -Wa,-xgot -Wall -mxgot -mno-fix-r4000 -march=4kc
LD             := $(CROSS_COMPILE)ld
LDFLAGS        += -$(ENDIAN) -G 0 -static -n -nostdlib --fatal-warnings

HOST_CC        := cc
HOST_CFLAGS    += --std=gnu99 -O2 -Wall
HOST_ENDIAN    := $(shell lscpu | grep -iq 'little endian' && echo EL || echo EB)

ifneq ($(HOST_ENDIAN), $(ENDIAN))
# CONFIG_REVERSE_ENDIAN is checked in tools/fsformat.c (lab5)
HOST_CFLAGS    += -DCONFIG_REVERSE_ENDIAN
endif
