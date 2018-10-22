#toolchina config
TOOLCHAIN           ?= arm-none-eabi-
CC                  := $(TOOLCHAIN)gcc
CXX                 := $(TOOLCHAIN)g++
AS                  := $(TOOLCHAIN)as
#LD                  := $(TOOLCHAIN)gcc
LD                  := $(TOOLCHAIN)g++
OBJCOPY             := $(TOOLCHAIN)objcopy
STRIP               := $(TOOLCHAIN)strip
SIZE                := $(TOOLCHAIN)size

#product config
PRODUCT_NAME        := main
BUILD               := .build
DEPS_DIR            := $(BUILD)/deps
PRODUCT_BASE        := $(BUILD)/$(PRODUCT_NAME)
PRODUCT_TMP         := $(PRODUCT_BASE).tmp
PRODUCT_ELF         := $(PRODUCT_BASE).elf
PRODUCT_HEX         := $(PRODUCT_BASE).hex
PRODUCT_BIN         := $(PRODUCT_BASE).bin
DEBUG_SYMBOLS       := $(PRODUCT_BASE).debug
LINK_SCRIPT         := ./Linkscript/stm32f746.ld

#auto find srcs,include,libs in .mk files in sub dirs 
SRCS                :=
INCLUDE             :=
LIBS                :=
ASMS                :=

SUB_DIRS            := $(shell find . -maxdepth 20 -type d)
MK_FILES            := $(foreach dir,$(SUB_DIRS),$(wildcard $(dir)/*.mk))
include $(MK_FILES)

SRCS                := $(subst $(shell pwd),.,$(SRCS))
INCLUDE             := $(subst $(shell pwd),.,$(INCLUDE))
LIBS                := $(subst $(shell pwd),.,$(LIBS))
ASMS                := $(subst $(shell pwd),.,$(ASMS))

SRC_OBJS            := $(addsuffix .o, $(basename $(SRCS)))
ASM_OBJS            := $(addsuffix .o, $(basename $(ASMS)))
BUILD_SRC_OBJS      := $(subst ./,$(BUILD)/,$(SRC_OBJS))
BUILD_ASM_OBJS      := $(subst ./,$(BUILD)/,$(ASM_OBJS))
BUILD_OBJS          := $(BUILD_SRC_OBJS) $(BUILD_ASM_OBJS)
DEPS                := $(addprefix $(DEPS_DIR)/, $(patsubst %.o,%.d,$(notdir $(BUILD_OBJS))))
VPATH               := $(shell dirname $(SRCS)) $(shell dirname $(ASMS))

#build args config
MCPU                := cortex-m7
PART                := STM32F746xx
FLAGS               := -mcpu=$(MCPU) -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 
CFLAGS              := $(FLAGS) -Os -nostdlib -ffunction-sections -fdata-sections $(INCLUDE) -D$(PART)  #-std=gnu99
CXXFLAGS            := $(CFLAGS)
LDFLAGS             := $(FLAGS) -Wl,--gc-sections -specs=nano.specs
ASFLAGS             := $(FLAGS)


#common build roles

all: clean_elf $(PRODUCT_ELF)
	@echo "\e[32;1m Build Success, build file info:"
	@$(SIZE) $(PRODUCT_ELF)
	@echo "\33[0m"

$(PRODUCT_ELF): $(BUILD_SRC_OBJS) $(BUILD_ASM_OBJS)
	@echo Linking $(PRODUCT_ELF)
	@$(LD) -T $(LINK_SCRIPT) $(LDFLAGS) -o $(PRODUCT_TMP) $(BUILD_OBJS) $(LIBS)
	@$(OBJCOPY) --only-keep-debug $(PRODUCT_TMP) $(DEBUG_SYMBOLS)
	@cp $(PRODUCT_TMP) $(PRODUCT_ELF)
	$(STRIP) --strip-debug --strip-unneeded $(PRODUCT_ELF)
	$(OBJCOPY) --add-gnu-debuglink=$(DEBUG_SYMBOLS) $(PRODUCT_ELF)
	$(OBJCOPY) -O binary $(PRODUCT_ELF) $(PRODUCT_BIN)
	$(OBJCOPY) -O ihex $(PRODUCT_ELF) $(PRODUCT_HEX)

clean_elf:
	@rm -rf $(PRODUCT_ELF)

$(BUILD)/%.o : %.c
	@echo Building $@ 
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/%.o : %.cpp
	@echo Building $@ 
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/%.o : %.S
	@echo Building $@
	@mkdir -p $(shell dirname $@)
	@$(AS) $(ASFLAGS) -o $@ $<

$(DEPS_DIR)/%.d : %.c
	@echo Depening $@
	@mkdir -p $(DEPS_DIR)
	@$(CC) -MM $(CFLAGS) $< -MT $(subst ./,$(BUILD)/,$(patsubst %.c,%.o,$<)) > $@

$(DEPS_DIR)/%.d : %.cpp
	@echo Depening $@
	@mkdir -p $(DEPS_DIR)
	@$(CC) -MM $(CFLAGS) $< -MT $(subst ./,$(BUILD)/,$(addsuffix .o, $(basename $<))) > $@

$(DEPS_DIR)/%.d : %.S
	@echo Depening $@
	@mkdir -p $(DEPS_DIR)
	@$(CC) -MM $(CFLAGS) $< -MT $(subst ./,$(BUILD)/,$(addsuffix .o, $(basename $<))) > $@

-include $(DEPS)

deploy: all 
	@echo "Writing $(PRODUCT_BIN) to the board via ST-LINK"
	@$(Q)(st-flash write $(PRODUCT_BIN) 0x8000000)
	
clean:
	@echo Cleaning project......
	@rm -rf $(BUILD)

.PHONY: clean all deploy
