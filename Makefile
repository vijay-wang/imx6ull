TOOLS 		:= arm-linux-gnueabihf
ARM-OBJCOPY 	:= $(TOOLS)-objcopy
ARM-OBJDUMP 	:= $(TOOLS)-objdump
ARM-GCC 	:= $(TOOLS)-gcc
ARM-LD 		:= $(TOOLS)-ld
GCC-STD		:= gnu99

BSP-SUBDIRS	:= $(foreach dir, $(shell ls bsp), bsp/$(dir))

TARGET		?=  $(shell git branch --show-current)

$(shell mv *.lds $(TARGET).lds)

INCDIRS		:= imx6ull stdio $(BSP-SUBDIRS) 
			
SRCDIRS 	:= project stdio $(BSP-SUBDIRS) 

LIBPATH	:= -lgcc -L /home/wang/tool-chain/lib/gcc/arm-linux-gnueabihf/4.9.4

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
	$(ARM-LD) -T$(TARGET).lds -o $(TARGET).elf $^ $(LIBPATH)
	$(ARM-OBJCOPY) -O binary -S -g  $(TARGET).elf $@ 
	$(ARM-OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(COBJS): obj/%.o: %.c
	$(ARM-GCC) -fno-builtin -Wall -std=$(GCC-STD) -nostdlib -c $(INCLUDE) -O2 $^ -o $@ $(LIBPATH)

$(SOBJS): obj/%.o: %.s
	$(ARM-GCC) -fno-builtin -Wall -nostdlib -c $(INCLUDE) -O2 $^ -o $@

clean:
	rm load.imx $(TARGET).bin $(TARGET).elf $(TARGET).dis obj/*

print:
	@echo $(INCDIRS)
	@echo $(SRCDIRS)
