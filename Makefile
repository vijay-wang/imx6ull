objs = main.o start.o

ledc.bin: ledc.lef
	arm-linux-gnueabihf-objcopy -O binary -S -g $< $@ 
	arm-linux-gnueabihf-objdump -D -m arm $^ > ledc.dis

ledc.lef: $(objs)
	arm-linux-gnueabihf-ld -Tledc.lds $^ -o $@ 

%.o: %.c
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -O2 $^ -o $@

%.o: %.s
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -O2 $^ -o $@

clean:
	rm load.imx ledc.bin ledc.lef *.o *.dis
