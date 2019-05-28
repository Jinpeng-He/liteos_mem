    
#!/bin/bash

prjPath = .
GLOBAL_LIB ?= lib
BIN ?= bin
GLOBAL_PREFIX ?=

VPATH += .
SOURCES = $(foreach dir,$(VPATH),$(wildcard $(dir)/*.c))
HEADERS = $(foreach dir,$(VPATH),$(wildcard $(dir)/*.h))
OBJS = $(SOURCES:%.c=%.o)


CC = gcc

CFLAGS = -Wall -o2
CFLAGS += -I../include
CFLAGS += -L../lib -lliteosmem


TARGET = mem_test




%.d:%c
	@set -e; rm -f $@
	$(CC) -MM $(CFLAGS) $^ > $@.$$$;\
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' <$@.$$$ > $@;\
	rm -f $@$$$
	
%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@
	
$(TARGET):$(OBJS)
	$(CC) $^ $(CFLAGS) -o $@
	cp $@ ../$(BIN)
	rm -rf $@
	
.PHONY : clean
clean:
	rm -f *.o *.bak $(TARGET).* ../$(BIN)/$(TARGET).*