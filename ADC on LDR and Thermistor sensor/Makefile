CC=avr-gcc
CFLAGS=-Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)
MCU=atmega328p
F_CPU=16000000UL

OBJCOPY=avr-objcopy
BIN_FORMAT=ihex

PORT=/dev/ttyUSB0
BAUD=115200
PROTOCOL=arduino
PART=m328p
AVRDUDE=avrdude -F -V

RM=rm -f

all: ADC.hex
	sudo $(AVRDUDE) -c$(PROTOCOL) -p $(PART) -P $(PORT) -b $(BAUD) -D -U flash:w:$<

compile: ADC.hex

ADC.hex: ADC.elf 
	 $(OBJCOPY) -O $(BIN_FORMAT) -R .eeprom $< $@

ADC.elf: ADC.o 
	 $(CC) $(CFLAGS) -o $@ $<

ADC.o: ADC.c 
	 $(CC) $(CFLAGS) -c -o $@ $<

upload : 
	sudo $(AVRDUDE) -c $(PROTOCOL) -p $(PART) -P $(PORT) $(BAUD) -U flash:w:ADC.hex

clean:
	$(RM) ADC.elf ADC.hex ADC.s ADC.o


