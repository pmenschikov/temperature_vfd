MCU=atmega128
C_OPTS= -mmcu=$(MCU) -Os -DF_CPU=16000000U

hex:main.hex

main.o:main.c
	avr-gcc -o $@ -c $(C_OPTS) $^

lcd.o:lcd.c
	avr-gcc -o $@ -c $(C_OPTS) $^

debug.o: debug.c
	avr-gcc -o $@ -c $(C_OPTS) $^

main.elf:main.o lcd.o debug.o
	avr-gcc -o $@ -mmcu=$(MCU) $^
	avr-size $@

main.hex:main.elf
	avr-objcopy -O ihex -R eeprom $^ $@ 

program: main.hex
	avrdude -c usbasp -p $(MCU) -U flash:w:main.hex

clean:
	rm *.o
	rm *.elf
	rm *.hex

.PHONY: hex program clean
