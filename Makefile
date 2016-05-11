CC = xtensa-lx106-elf-gcc
CFLAGS = -I. -mlongcalls
LDLIBS = -nostdlib -Wl,--start-group -lmain -lnet80211  -lwpa -llwip -lpp -lphy -Wl,--end-group -lgcc
LDFLAGS = -Teagle.app.v6.ld

sniff-0x00000.bin: sniff
	esptool.py elf2image $^

sniff: sniff.o  driver/uart.o

sniff.o: sniff.c



uart.o: driver/uart.c

flash: sniff-0x00000.bin
	esptool.py write_flash 0 sniff-0x00000.bin 0x40000 sniff-0x40000.bin

clean:
	rm -f sniff sniff.o sniff-0x00000.bin sniff-0x40000.bin
