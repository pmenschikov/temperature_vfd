MCU=atmega128
C_OPTS=-Os -DF_CPU=16000000U

main.o:main.c
	avr-gcc -o $@ -c -mmcu=$(MCU) $(C_OPTS) $^

main.elf:main.o
	avr-gcc -o $@ -mmcu=$(MCU) $^
	avr-size $@

main.hex:main.elf
	avr-objcopy -O ihex -R eeprom $^ $@ 

hex:main.hex

program: main.hex
	avrdude -c usbasp -p $(MCU) -U flash:w:main.hex

clean:
	rm *.o
	rm *.elf
	rm *.hex

.PHONY: hex program clean
