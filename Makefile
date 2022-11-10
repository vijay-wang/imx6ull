led.bin: led.s
	arm-linux-gnueabihf-gcc -g -c led.s -o led.o
	arm-linux-gnueabihf-ld -Ttext 0x80000000 led.o -o led.elf
	arm-linux-gnueabihf-objcopy -O binary -S -g led.elf led.bin

clean:
	rm led.bin load.imx  led.o led.elf
