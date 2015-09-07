CC=gcc
AS=as
CFLAGS=-O1 -std=c99 -D__thread= -Wall -Werror -nostdinc -Iinclude -msoft-float -mno-sse -mno-red-zone -fno-builtin -fPIC -march=amdfam10 -g3
LD=gcc
LDLAGS=
AR=ar

ROOTFS=rootfs
ROOTBIN=$(ROOTFS)/bin
ROOTBOOT=$(ROOTFS)/boot

SHELL=/bin/sh

BIN_SRCS:=$(shell find bin/* -name \*.c)
INCLUDES:=$(shell find include/ -type f -name \*.h)
BINS:=$(addprefix $(ROOTFS)/,$(wildcard bin/*))

SRC_DIR = src
OBJ_DIR = obj
LIB_SRCS:=$(wildcard $(SRC_DIR)/*.c)
LIB_OBJS:=$(LIB_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all binary

all: $(BINS)

$(BINS): $(patsubst %.s,obj/%.asm.o,$(CRT_SRCS:%.c=obj/%.o)) $(BIN_SRCS) $(INCLUDES) $(LIB_OBJS)
	@$(MAKE) --no-print-directory BIN=$@ binary

binary: $(patsubst %.c,obj/%.o,$(wildcard $(BIN:rootfs/%=%)/*.c)) $(LIB_OBJS)
	$(LD) $(LDLAGS) -o $(BIN) $(patsubst %.s,obj/%.asm.o,$(CRT_SRCS:%.c=obj/%.o)) $^

$(LIB_OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

obj/%.o: %.c $(INCLUDES)
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

obj/%.asm.o: %.s
	@mkdir -p $(dir $@)
	$(AS) -o $@ $<

.PHONY: submit clean

SUBMITTO:=~mferdman/cse506-submit/

submit: clean
	tar -czvf $(USER).tgz --exclude=.empty --exclude=.*.sw? --exclude=*~ LICENSE README Makefile bin include $(ROOTFS)
	@gpg --quiet --import cse506-pubkey.txt
	gpg --yes --encrypt --recipient 'CSE506' $(USER).tgz
	rm -fv $(SUBMITTO)$(USER)=*.tgz.gpg
	cp -v $(USER).tgz.gpg $(SUBMITTO)$(USER)=`date +%F=%T`.tgz.gpg

clean:
	find $(ROOTBIN) -type f ! -name .empty -print -delete
	rm -rfv obj kernel newfs.506 $(ROOTBOOT)/kernel/kernel $(USER).iso
