#######################################
# binaries
#######################################
PREFIX    =
CC        = $(PREFIX)gcc
AS        = $(PREFIX)gcc -x assembler-with-cpp
OBJCOPY   = $(PREFIX)objcopy
OBJDUMP   = $(PREFIX)objdump
AR        = $(PREFIX)ar
SZ        = $(PREFIX)size
LD        = $(PREFIX)ld



# C defines
C_DEFS =  \
-DUSE_RB_TREE \
-DHAVE_UT_HASH \
-DHAVE_KFIFO


TARGET = algorithm
TARGET_LIB = libalgo.so

#######################################
PROJECTBASE = $(PWD)
override PROJECTBASE    := $(abspath $(PROJECTBASE))
TOP_DIR = $(PROJECTBASE)


# Build path
BUILD_DIR = build
LIBDIR = lib

######################################
# source
######################################
# C sources
ALGO_SRC =  \
        ${wildcard $(TOP_DIR)/src/*.c}
        C_SOURCES += $(ALGO_SRC)
        


	
	
USER_SRC =  \
        ${wildcard $(TOP_DIR)/test/*.c}	
		C_SOURCES += $(USER_SRC)





# C includes
ALGORITHM_INC = \
        -I $(TOP_DIR)/include
        C_INCLUDES += $(ALGORITHM_INC)	


USER_INC = \
        -I $(TOP_DIR)/test
        C_INCLUDES += $(USER_INC)
		

CFLAGS = $(C_DEFS) $(C_INCLUDES)  -Wall




# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"


# libraries
LIBS = -lalgo
LDFLAGS =   -L$(LIBDIR) $(LIBS)

# default action: build all
all: $(BUILD_DIR)/$(TARGET) $(LIBDIR)/$(TARGET_LIB)


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(USER_SRC:.c=.o)))
vpath %.c $(sort $(dir $(USER_SRC)))


LIBOBJECTS = $(addprefix $(LIBDIR)/,$(notdir $(ALGO_SRC:.c=.o)))
vpath %.c $(sort $(dir $(ALGO_SRC)))



$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile  | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS) Makefile $(LIBDIR)/$(TARGET_LIB)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

	
$(BUILD_DIR):
	mkdir $@		
	

	
$(LIBDIR)/%.o: %.c Makefile | $(LIBDIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(LIBDIR)/$(notdir $(<:.c=.lst)) $< -o $@
	
$(LIBDIR)/$(TARGET_LIB): $(LIBOBJECTS) Makefile
	$(AR) r $@ $(LIBOBJECTS)


$(LIBDIR):
	mkdir $@
	

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR) $(LIBDIR)
  
#######################################
# dependencies
#######################################
#-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***