STFLASH			= $(shell which st-flash)
JFLASH			= $(shell which JLinkExe)

# Object files
TMP1			:= $(patsubst %.c, $(OBJDIR)/%.o, $(SOURCES))
TMP2			:= $(patsubst %.cpp, $(OBJDIR)/%.o, $(TMP1))
TMP3			:= $(patsubst %.cxx, $(OBJDIR)/%.o, $(TMP2))
OBJS			:= $(patsubst %.s, $(OBJDIR)/%.o, $(TMP3))

BINARY			:= $(OUTPUT_DIR)$(TARGET)

# C/C++ standards
CSTD			?= c11
CXXSTD			?= c++17

# Compiler flags
SPECS			?= --specs=nano.specs

CFLAGS			+= -std=$(CSTD) $(OPTIMIZATION) -fno-builtin-log

CPPFLAGS		+= -ffunction-sections -fdata-sections -fno-common
CPPFLAGS		+= -pedantic -Wall -Wextra
CPPFLAGS		+= -Wno-unused-parameter -Wno-unused-variable
CPPFLAGS		+= -fexec-charset=cp1252
CPPFLAGS		+= $(DEFS:%=-D%)
CPPFLAGS		+= $(C_DEFS) $(AS_DEFS)
CPPFLAGS		+= $(INCDIRS:%=-I%)
CPPFLAGS		+= -MMD -MP -MF"$(@:%.o=%.d)"
CPPFLAGS		+= $(SPECS)

CXXFLAGS		+= -std=$(CXXSTD) $(OPTIMIZATION)
CXXFLAGS		+= -fno-exceptions -fno-rtti -fno-unwind-tables -Wno-register -fno-math-errno

# Linker flags
LDFLAGS			+= $(SPECS)
LDFLAGS			+= --static
LDFLAGS			+= -Wl,--gc-sections
LDFLAGS			+= -Wl,--print-memory-usage
LDFLAGS			+= -Wl,-Map=$(BINARY).map
LDLIBS			+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q			:= @
endif

# Toolchain stuff
include $(BASEDIR)mk/gcc-config.mk

# Rules
.PHONY: clean all stflash jflash
.SECONDARY:

all: $(BINARY).bin $(BINARY).list

print-%:
	@echo $*=$($*)

%.bin:	%.elf
	@printf "  OBJCOPY $@\n"
	$(Q)$(OBJCOPY) -Obinary $< $@

%.list: %.elf
	@printf "  OBJDUMP $@\n"
	$(Q)$(OBJDUMP) -S $< > $@

%.elf: $(OBJS) $(LIBDEPS)
	@printf "  LD      $@\n"
	$(Q)$(LD) $(OBJS) $(LDLIBS) $(LDFLAGS) -T$(LDSCRIPT) $(MCU) -o $@

$(OBJDIR)/%.o: %.s
	@printf "  AS      $<\n"
	@mkdir -p $(dir $@)
	$(Q)$(CC) -x assembler-with-cpp $(CFLAGS) $(CPPFLAGS) $(MCU) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@printf "  CC      $<\n"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) $(MCU) -c $< -o $@

$(OBJDIR)/%.o: %.cpp
	@printf "  CXX     $<\n"
	@mkdir -p $(dir $@)
	$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(MCU) -c $< -o $@

$(OBJDIR)/%.o: %.cxx
	@printf "  CXX     $<\n"
	@mkdir -p $(dir $@)
	$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(MCU) -c $< -o $@

clean:
	@$(RM) -Rf build dsdlc_generated

stflash:$(BINARY).bin
	$(STFLASH) $(FLASHSIZE) write $(BINARY).bin 0x8000000

jflash:	$(BINARY).bin
	$(JFLASH) -Autoconnect 1 -Device $(DEVICE) -If SWD -Speed 4000 -CommandFile flash_commands.jlink

-include $(OBJS:.o=.d)
