/*
 * lab2 digital2 2.c
 *
 * Created: 22/07/2024 22:15:28
 * Author : diego
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "Librerias/LCD8b.h"
#include "Librerias/ADC.h"
#include "Librerias/UART.h"
volatile uint8_t adc_activo = 0;
volatile uint16_t adc = 0;
volatile uint8_t adc1c = 0;
volatile uint8_t adc1d = 0;
volatile uint8_t adc1u = 0;
volatile uint8_t adc2c = 0;
volatile uint8_t adc2d = 0;
volatile uint8_t adc2u = 0;
volatile uint8_t uart = 0;
volatile uint8_t s3 = 9;
volatile int8_t neg = 0;
const uint8_t mylist[] = {57, 56, 55, 54,53, 52, 51, 50, 49, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57};

int main(void)
{
	cli();
	init_uart();
	initLCD8bits();
	ADC_(128);
	ADC0();
	LCD_Set_Cursor(1,1);
	LCD_Write_String("S1:");
	LCD_Set_Cursor(6,1);
	LCD_Write_String("S2:");
	LCD_Set_Cursor(2,2);
	LCD_WriteCaracter('.');
	LCD_Set_Cursor(7,2);
	LCD_WriteCaracter('.');
	LCD_Set_Cursor(11,1);
	LCD_Write_String("S3:");
	
	sei();
    while (1) 
    {
		ADCSRA |= (1<<ADSC);
		_delay_ms(200);
		LCD_Set_Cursor(1,2);
		LCD_WriteCaracter(adc1c);
		LCD_Set_Cursor(3,2);
		LCD_WriteCaracter(adc1d);
		LCD_Set_Cursor(4,2);
		LCD_WriteCaracter(adc1u);
		LCD_Set_Cursor(6,2);
		LCD_WriteCaracter(adc2c);
		LCD_Set_Cursor(8,2);
		LCD_WriteCaracter(adc2d);
		LCD_Set_Cursor(9,2);
		LCD_WriteCaracter(adc2u);
		if(s3 < 9){
			LCD_Set_Cursor(11,2);
			LCD_WriteCaracter('-');	
		}else{
			LCD_Set_Cursor(11,2);
			LCD_WriteCaracter(' ');
		}
		LCD_Set_Cursor(12,2);
		LCD_WriteCaracter(mylist[s3]);
		if (uart == 43){
			s3++;
			uart = 0;
			writeUART(10);
			writetextUART("Sensor 1:");
			writeUART(10);
			writeUART(adc1c);
			writeUART('.');
			writeUART(adc1d);
			writeUART(adc1u);
			writeUART(10);
			writetextUART("Sensor 2:");
			writeUART(10);
			writeUART(adc2c);
			writeUART('.');
			writeUART(adc2d);
			writeUART(adc2u);
			writeUART(10);
			writeUART(s3);
		}else if(uart == 45){
			s3--;
			uart = 0;
			writetextUART("Sensor 1:");
			writeUART(10);
			writeUART(adc1c);
			writeUART('.');
			writeUART(adc1d);
			writeUART(adc1u);
			writeUART(10);
			writetextUART("Sensor 2:");
			writeUART(10);
			writeUART(adc2c);
			writeUART('.');
			writeUART(adc2d);
			writeUART(adc2u);
			writeUART(10);
			writeUART(s3);
		}
		
    }
}


ISR(ADC_vect){
	adc_activo = ADMUX & ((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
	ADCSRA &= ~(1<<ADEN);
	if (adc_activo == 0){
		adc = ADCH;//*500/255;
		
		//adc = ADCH*500/255;
		adc1c = adc/100 + 48;
		adc1d = (adc%100)/10 + 48;
		adc1u = adc%10 + 48;
		ADC1();
		} else if (adc_activo == 1){
			adc = ADCH;
			adc2c = adc/100 + 48;
			adc2d = (adc%100)/10 + 48;
			adc2u = adc%10 + 48;
			ADC0();
	}
	ADCSRA |= (1<<ADIF); //apagar bandera
	ADCSRA |= (1<<ADEN);
}

ISR(USART_RX_vect){
	uart = UDR0;
	
	
	if (uart == 43){//+
		//while (!(UCSR0A & (1<<UDRE0)));
		//s3++;
		
	}else if(uart == 45){//-
		//s3--;
	
	}
}
ISR(USART_TX_vect){
	s3 = s3;
}
