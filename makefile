upload:
	avr-gcc -Os -mmcu=atmega328p -c main.c
	avr-gcc -mmcu=atmega328p main.o -o main.elf
	avr-objcopy -O ihex -R .eeprom main.elf main.hex
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/cu.wchusbserial1420 -b 57600 -U flash:w:main.hex
clean:
	rm *.elf
	rm *.o
	rm *.hex