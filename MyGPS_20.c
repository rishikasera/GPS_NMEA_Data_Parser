/*LATITUDE  LONGITUDE DISPLAY THROUGH GPS*/
/* hardware(PCB) No. -  1*/

/*********************************************************************************
* Program  :  Program to DISPLAY LATITUDE LONGITUDE                              *
* Filename :  MyGSM_20.c                                          		  		 *
* Date     :  20/07/14                                                  		 *
* Author   :  Rishi Kasera                                                     	 *
* Version  :  1.00                                                               *
* Hardware :  AT MEGA 8 							             		 *
* IDE      :  AVR STUDIO 4                                               		 *
* Language :  Embedded C                                                         *
* Compiler :  AVR-GCC.exe                                                        *
*                                                                                *
* Oscillator Frequency : 8Mhz                                              		 *
* Dependencies         : lcd4bit.H                                               *
* Modification History :  --                                                     *
**********************************************************************************
/*Procedure: 			 *
/********************************************************************************/

#include <avr/io.h>

#include <util/delay.h>

#include <avr/interrupt.h>

/*===============================================================================*/
/* 				     	    Macro declaration       	    					 */
/*===============================================================================*/

#define F_CPU 8000000

#define USART_BAUDRATE 9600

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/*==============================================================================*/
/* 					Local variable defination								    */
/*==============================================================================*/

unsigned char Start_of_CMD[] = "$GPGGA,";

unsigned char Buffer[100];

int Buffer_Index = 0;

int CMD_Index = 0;

int is_CMD_Found = 0;

/*==============================================================================*/
/*                           Function Declaration                               */
/*==============================================================================*/

void PrintLatitudeLongitude();

void Fetch_Latitude_Longitude(unsigned char rByte);

void SerialInterruptDisable(void);

void SerialInterruptEnable(void);

/*==============================================================================*/
/*                           Function Defination                                */
/*==============================================================================*/



/********************************************************************************
* Function Defination  : ISR(USART_RXC_vect)               	           			*
* Input Parametres     : void					                                *
* Return Value         : void                                                   *
* Working              : This function define ISR for serial interrupt   		*
********************************************************************************/

ISR(USART_RXC_vect)
{
	char ReceivedByte;
	ReceivedByte = UDR; // Fetch the received byte value into the variable "ByteReceived"
	//UDR = ReceivedByte; // Echo back the received byte back to the computer
	//Write_data(ReceivedByte);
    Fetch_Latitude_Longitude(ReceivedByte);
}


/********************************************************************************
* Function Defination  : void PrintLatitudeLongitude()              	       	*
* Input Parametres     : void					                                *
* Return Value         : void                                                   *
* Working              : This function print the value of latitude and          *
*                        longitude in lcd.		                                *
********************************************************************************/


void PrintLatitudeLongitude()
{
	    int a = 0 ;
		for(;Buffer[a]!=',';a++){} 
	    	Write_Command(0x33);//?
		Write_Command(0x32);//?
		Write_Command(0x28);//4bit mode, 5x8 font, 2 line display
		Write_Command(0x0c);
		Write_Command(0x06);//increament cursor
		Write_Command(0x01);//clear display
		_delay_ms(10);
     	Print_String("Latitude : ");
		Write_Command(0xC0);
		a++;
		for(;Buffer[a]!=',';a++){
			Write_data(Buffer[a]);
		}
		Write_data(' ');
		a++;
		for(;Buffer[a]!=',';a++){
			Write_data(Buffer[a]);
		}
		a++;
		_delay_ms(2000);
		Write_Command(0x33);//?
		Write_Command(0x32);//?
		Write_Command(0x28);//4bit mode, 5x8 font, 2 line display
		Write_Command(0x0c);
		Write_Command(0x06);//increament cursor
		Write_Command(0x01);//clear display
		_delay_ms(10);
		Print_String("Longitude : ");
		Write_Command(0xC0);
		for(;Buffer[a]!=',';a++){
			Write_data(Buffer[a]);
		}
		a++;
		Write_data(' ');
		for(;Buffer[a]!=',';a++){
			Write_data(Buffer[a]);
		}
		a++;
		_delay_ms(2000);
	
		
}


/********************************************************************************
* Function Defination  : void Fetch_Latitude_Longitude(unsigned char rByte)		*
* Input Parametres     : void					                                *
* Return Value         : void                                                   *
* Working              : This function fetch the value of latitude and          *
*                        longitude from cmd.		                                *
********************************************************************************/

void Fetch_Latitude_Longitude(unsigned char rByte)
{
	       	
			if(is_CMD_Found==1){
			    	if(rByte=='$'){
						Buffer[Buffer_Index]='%';
						is_CMD_Found = 0;
						Buffer_Index=0;
						SerialInterruptDisable();
						PrintLatitudeLongitude();
						SerialInterruptEnable();
						//display();
						//Print_String("formate complete");
						//while(1);
					}else{
						Buffer[Buffer_Index]=rByte;
						Buffer_Index++;
					}
			}
			   		
			else if(Start_of_CMD[CMD_Index]==rByte)
			{
				if(CMD_Index == 6){
					CMD_Index=0;
				//	Print_String("formate found");
				//	SerialInterruptDisable();
				//	while(1){}
					is_CMD_Found = 1;
				}else{
					CMD_Index++;
				}
			}else{
				CMD_Index = 0;
			}
			
		
}


/********************************************************************************
* Function Defination  : void SerialInterruptDisable()              			*
* Input Parametres     : void					                                *
* Return Value         : void                                                   *
* Working              : This function use to disable the serial interrupt		*                                *
********************************************************************************/


void SerialInterruptDisable()
{
	UCSRB = (0<<RXEN)|(0<<TXEN);
}


/********************************************************************************
* Function Defination  : void SerialInterruptEnable()               	           		*
* Input Parametres     : void					                                *
* Return Value         : void                                                   *
* Working              : This function enable ISR for serial interrupt   		*
********************************************************************************/


void SerialInterruptEnable()
{
	UBRRH = (BAUD_PRESCALE >> 8);
	UBRRL = BAUD_PRESCALE;
	
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRB |= (1 << RXCIE);
	sei();
}

int main(void){
	SerialInterruptDisable();
	Init_Ports();
	Init_LCD();
	Print_String(".*..LocatioN..*.");
	Write_Command(0xc0);
	Print_String(".....TRACER.....");
    _delay_ms(4000);
	
	UBRRH = (BAUD_PRESCALE >> 8);
	UBRRL = BAUD_PRESCALE;
	
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRB |= (1 << RXCIE);
	sei();

	while(1)
	{

	}
}
