PROJECT_ROOT_DIR := ./


#PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AR = $(PREFIX)ar
STRIP = $(PREFIX)strip



INCLUDE_DIR := $(PROJECT_ROOT_DIR)include/
LIB_DIR := $(PROJECT_ROOT_DIR)lib/


DEBUG_C_FLAG = -g

NODEBUG_C_FLAG = -o2


CFLAGS = $(NODEBUG_C_FLAG) -Wall -Wno-strict-aliasing -lpthread -Wno-deprecated -D_REENTRANT  -fPIC $(patsubst %, -I%, $(INCLUDE_DIR)) -Wpointer-to-int-cast


VPATH += src

SOURCES = $(foreach dir,$(VPATH),$(wildcard $(dir)/*))
C_SRCS   = $(filter %.c,$(SOURCES))
CPP_SRCS = $(filter %.cpp,$(SOURCES))
C_OBJS   = $(C_SRCS:%.c=%.o)
CPP_OBJS = $(CPP_SRCS:%.cpp=%.o)
OBJS = $(C_OBJS) $(CPP_OBJS)

LIB = libliteosmem.a

INCLUDE_DIR += $(PROJECT_ROOT_DIR)include/

all : $(LIB) 
$(LIB):$(OBJS)
	$(AR) r $@ $(OBJS)
	cp $@ $(LIB_DIR)

%.o:%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $^ -o $@
%.o:%.cpp
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $^ -o $@

.PHONY:clean
clean:
	-rm $(OBJS) $(LIB) -rf