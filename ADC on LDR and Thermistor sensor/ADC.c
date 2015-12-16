#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
void Adcsetup()
{
  cli();
  ADMUX = 0b00000000; // AREF,Right Adjusted,Analog channel Selectio Bits: 0000 for ADC0
  ADCSRA = 0b10101111; // ADC Enable, ADC Start Conversion - off, Auto Trigger - on , ADC Interrupt Flag - off,ADC interrupt enabel -on
  ADCSRB = 0b00000011;// Timer 0 overflow interrupt enabled
  DIDR0 = 0b11111100; // disable unnecessary pins
  sei();
}

void Timer0Setup()
{
  cli();
  TCNT0 = 0x00;
  OCR0A = 255;
  TCCR0A = 0b00000010; // last two bits Timer Overflow 
  TCCR0B = 0b00000101; // input capture noise canceller deactivated ; Prescalar 101 - 1024; Normal Mode
  TIMSK0 = 0b00000010; // using Timer1 compare match 100
  sei();
}
void sleepsetup()
{
  cli();
  SMCR = 0b00000001; // 000 for ADC noise Reduction Mode
  MCUCR = 0b01100000;
  MCUCR = 0b01000000;
  PRR = 0b11010110; // TWI,Timer2,Timer0,Reserved,SPI,USART0,ADC
  sei();
}
int main()
{
  DDRB |= 0x20;
  PORTB |= 0x20;
  DDRD |= 0b11000000;
  PORTD |= 0b11000000;
  DDRC &= 0b11111100;
  Adcsetup();
  Timer0Setup();
  sleepsetup(); 
  while(1)
  {
    SMCR |= 0b00000001; // SE sleep enable
    sleep_cpu();
  }
  return 0;
}

int x = 0;
ISR(ADC_vect)
{
  	cli ();
	SMCR &= 0b11111110;
	uint16_t valueADC, valL, valH;
	valL = ADCL;
	valH = ADCH;
	valueADC = (valH << 8) + valL;
	if (ADMUX == 0x00)
	{
		ADMUX = 0x01;
		if (valueADC <= 0x000A)
                        PORTD &= 0b10111111; // toggling 6
		else
			PORTD |= 0b01000000; // led6 on
        }
	else
	{
                //PORTB &= 0b11011111;
		ADMUX = 0x00;
		if (valueADC >= 0x07E)
			PORTD |= 0b10000000;
		else
			PORTD &= 0b01111111;
	}         
        TCNT0 = 0x00;
        sei();
}  