CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=arm926ej-s -g -ffreestanding
LDFLAGS = -T linker.ld

all: kernel.bin

kernel.bin: startup.o main.o
	$(LD) $(LDFLAGS) startup.o main.o -o kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

startup.o: startup.s
	$(AS) -mcpu=arm926ej-s startup.s -o startup.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f *.o *.elf *.bin

run: kernel.bin
	qemu-system-arm -M versatilepb -m 128M -kernel kernel.bin -serial stdio