objs 	:= main.o start.o
tools 	:= arm-linux-gnueabihf
COPY 	:= $(tools)-objcopy
DUMP 	:= $(tools)-objdump
GCC 	:= $(tools)-gcc
LD 	:= $(tools)-ld

ledc.bin: ledc.lef
	$(COPY) -O binary -S -g $< $@ 
	$(DUMP) -D -m arm $^ > ledc.dis

ledc.lef: $(objs)
	$(LD) -Tledc.lds $^ -o $@ 

%.o: %.c
	$(GCC) -Wall -nostdlib -c -O2 $^ -o $@

%.o: %.s
	$(GCC) -Wall -nostdlib -c -O2 $^ -o $@

clean:
	rm load.imx ledc.bin ledc.lef *.o *.dis
