# Parallelize
CPUS ?= $(shell nproc || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

# Force ccache
CXX = ccache g++

TARGET_EXEC ?= assembler

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cc -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -pipe -O3

# Link libstc++ and libstdc statically if compiling for Windows
ifeq ($(findstring mingw32,$(CXX)),mingw32) 
	LDFLAGS = -static -static-libstdc++ -static-libgcc
endif

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cc.o: %.cc
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

windows:
	$(MAKE) CXX="ccache x86_64-w64-mingw32-g++" $(BUILD_DIR)/$(TARGET_EXEC)
	mv $(BUILD_DIR)/$(TARGET_EXEC) $(BUILD_DIR)/$(TARGET_EXEC).exe

-include $(DEPS)

MKDIR_P ?= mkdir -p