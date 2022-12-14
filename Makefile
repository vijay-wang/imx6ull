TOOLS 		:= arm-linux-gnueabihf
ARM-OBJCOPY 		:= $(TOOLS)-objcopy
ARM-OBJDUMP 		:= $(TOOLS)-objdump
ARM-GCC 		:= $(TOOLS)-gcc
ARM-LD 		:= $(TOOLS)-ld

TARGET		?= beep

INCDIRS		:= imx6ull \
			bsp/clk \
			bsp/led \
			bsp/delay \
			bsp/beep

SRCDIRS 	:= project \
			bsp/clk \
			bsp/led \
			bsp/delay \
			bsp/beep

INCLUDE		:= $(patsubst %, -I %, $(INCDIRS))

SFILES 		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.s))
CFILES 		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILESNDIR	:= $(notdir $(SFILES))
CFILESNDIR	:= $(notdir $(CFILES))

SOBJS		:= $(patsubst %, obj/%, $(SFILESNDIR:.s=.o))
COBJS		:= $(patsubst %, obj/%, $(CFILESNDIR:.c=.o))
OBJS	 	:= $(COBJS) $(SOBJS)

VPATH		:= $(SRCDIRS)

$(TARGET).bin: $(OBJS)
	$(ARM-LD) -T$(TARGET).lds -o $(TARGET).elf $^
	$(ARM-OBJCOPY) -O binary -S -g  $(TARGET).elf $@ 
	$(ARM-OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(COBJS): obj/%.o: %.c
	$(ARM-GCC) -Wall -nostdlib -c $(INCLUDE) -O2 $^ -o $@

$(SOBJS): obj/%.o: %.s
	$(ARM-GCC) -Wall -nostdlib -c $(INCLUDE) -O2 $^ -o $@

clean:
	rm load.imx $(TARGET).bin $(TARGET).elf $(TARGET).dis obj/*

print:
	@echo $(COBJS)
	@echo $(SOBJS)
	@echo $(OBJS)
	@echo $(TARGET)
