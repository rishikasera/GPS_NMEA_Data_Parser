


#define LCD_DATA_DDR     DDRD
#define LCD_DATA_PORT    PORTD
#define LCD_CTRL_DDR     DDRB
#define LCD_CTRL_PORT    PORTB

#define RS          PB6
#define RW          PB7
#define EN          PB0

#define BF          PD7


#include<avr/io.h>
#include<util/delay.h>
#include "lcd4bit.h"

void LCD_Ready(void)
{	    
	    LCD_DATA_DDR  &=~(1<<BF); //set 0
		LCD_DATA_PORT |= (1<<BF); //SET 1
        //LCD_DATA_PORT |= (1<<BF); //bf = 1;
	    LCD_CTRL_PORT &=~(1<<RS);//RS=0
	    LCD_CTRL_PORT |= (1<<RW);//RW=1
		//while((LCD_DATA_PORT >> 7) & 1){
	      while(PIND & 0x80){
            LCD_CTRL_PORT &= ~(1<<EN);//EN=0
           _delay_us(100);
	       LCD_CTRL_PORT |= (1<<EN);//EN=1
	       _delay_us(10);
        }
		LCD_DATA_DDR = 0xf0;
	
}

void Init_Ports()
{

	LCD_DATA_DDR = 0xf0;//LCD_DATA_PORT UPPER nibble output
	LCD_CTRL_DDR = (1<<RS)|(1<<RW)|(1<<EN); // 0100 0000 | 1000 000 | 0000 0001 = 1100 0001
}
 

void Init_LCD()
{
	Write_Command(0x33);//?
	Write_Command(0x32);//?
	Write_Command(0x28);//4bit mode, 5x8 font, 2 line display
	Write_Command(0x0c);
	Write_Command(0x06);//increament cursor
	Write_Command(0x01);//clear display	
	_delay_ms(1000);
}


void Write_Command(unsigned char command)
{

	//write higher nibble of command
    LCD_Ready();
	LCD_DATA_PORT = command & 0xf0;// write higher nibble
	LCD_CTRL_PORT &=~(1<<RS);//RS=0 for command write
	LCD_CTRL_PORT &=~(1<<RW);//RW=0 for write operation
        
        
	LCD_CTRL_PORT |= (1<<EN);//EN=1
	_delay_us(1);
	LCD_CTRL_PORT &= ~(1<<EN);//EN=0
	_delay_us(100);


	//write lower nibble of command
    LCD_Ready();
	LCD_DATA_PORT = command<<4;// shift lower nibble to higher nibble & write lower nibble
	LCD_CTRL_PORT &=~(1<<RS);//RS=0 for command write
	LCD_CTRL_PORT &=~(1<<RW);//RW=0 for write operation
	LCD_CTRL_PORT |= (1<<EN);//EN=1
	_delay_us(1);
	LCD_CTRL_PORT &= ~(1<<EN);//EN=0
	_delay_us(100);	
}


void Write_data(unsigned char data1)
{
	
		//write higher nibble of data
    LCD_Ready(); 
	LCD_DATA_PORT = data1 & 0xf0;// write higher nibble
	LCD_CTRL_PORT |=(1<<RS);//RS=1 for data on DDRAM write
	LCD_CTRL_PORT &=~(1<<RW);//RW=0 for write operation
	LCD_CTRL_PORT |= (1<<EN);//EN=1
	_delay_us(1);
	LCD_CTRL_PORT &= ~(1<<EN);//EN=0
	_delay_us(100);


	//write lower nibble of data
    LCD_Ready();
	LCD_DATA_PORT = data1<<4;// shift lower nibble to higher nibble & write lower nibble
	LCD_CTRL_PORT |=(1<<RS);//RS=1 for data on DDRAM write
	LCD_CTRL_PORT &=~(1<<RW);//RW=0 for write operation
	LCD_CTRL_PORT |= (1<<EN);//EN=1
	_delay_us(1);
	LCD_CTRL_PORT &= ~(1<<EN);//EN=0
	_delay_us(100);
}


void Print_String(char *string)
{

	while(*string != '\0')
	{
		Write_data(*string++);
	}
}

void gotoXy(unsigned char  x,unsigned char y)
{
	if(x<40)
	{
		if(y) x|=0b01000000;
		x|=0b10000000;
		Write_Command(x);
	}
}

void Init_Signal_Char(){
	Write_Command(0x40);

	Write_data(0x0);  //0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x1f  1
	Write_data(0x0);
	Write_data(0x0);
	Write_data(0x0);
	Write_data(0x0);
	Write_data(0x0);
	Write_data(0x1f);
	Write_data(0x1f);

	Write_data(0x0);  //0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x1f, 0x1f  2
	Write_data(0x0);
	Write_data(0x0);
	Write_data(0x0);
	Write_data(0x0);
	Write_data(0x8);
	Write_data(0x1f);
	Write_data(0x1f);

	Write_data(0x0);  //0x0, 0x0, 0x0, 0x0, 0x4, 0xc, 0x1f, 0x1f 3
	Write_data(0x0);
	Write_data(0x0);
	Write_data(0x0);
	Write_data(0x4);
	Write_data(0xc);
	Write_data(0x1f);
	Write_data(0x1f);
	
	Write_data(0x0);  //0x0, 0x0, 0x0, 0x2, 0x6, 0xe, 0x1f, 0x1f 4
	Write_data(0x0);
	Write_data(0x0);
	Write_data(0x2);
	Write_data(0x6);
	Write_data(0xe);
	Write_data(0x1f);
	Write_data(0x1f);

	Write_data(0x0);  //0, 0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x1f 5
	Write_data(0x0);
	Write_data(0x1);
	Write_data(0x3);
	Write_data(0x7);
	Write_data(0xf);
	Write_data(0x1f);
	Write_data(0x1f);
	gotoXy(0,0);
}
void PrintSignalChar(unsigned char strength){
	gotoXy(15,0);
	Write_data(strength-1);
	gotoXy(0,0);
}


