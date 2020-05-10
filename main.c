#include <reg51.h>

void LCD_Init (void);
void delay(unsigned int count);
void LCD_Command (unsigned char cmd);
void LCD_Char (unsigned char char_data);
void LCD_String (unsigned char *str);
void LCD_String_xy (char row, char pos, char *str);
void Counter(unsigned short int lengthSeconds, unsigned char displayCount);

sfr lcd_data_port=0x90;		/* P1 port as data port */
sbit rs=P2^0;			/* Register select pin */
sbit rw=P2^1;			/* Read/Write pin */
sbit en=P2^2;			/* Enable pin */

//sbits for lights
sbit ewG = P3^0;
sbit ewY = P3^1;
sbit ewR = P3^2;
sbit ewG = P3^3;
sbit ewY = P3^4;
sbit ewR = P3^5;

unsigned char walkState = 0;
void main()
{
	unsigned char[] trafficSign = "Heavy traffic on 405";
	unsigned char state = 0;
	while(1){
		ewG = 0;
		ewY = 0;
		ewR = 0;
		ewG = 0;
		ewY = 0;
		ewR = 0;
		switch(state){
			case 0x00:
			{
				ewG = 1;
				nsR = 1;
				//start Counters
				
				if(walkState != 0){
					//display count down
					Counter(0x06, walkState);
					walkState = 0xff;
				}
				else{
					//display road sign
					Counter(0x06, 0x00);
				}
				state++;
			}
			case 0x01:
			{
				ewY = 1;
				nsR = 1;
				//start Counters
				if(walkState == 0xff){
					//flash
					Counter(0x06, walkState);
					walkState = 0;
				}
				else{
					//display road sign
					Counter(0x06, 0x00);
				}
				state++;
			}
			case 0x02:
			{
				nsG = 1;
				ewR = 1;
				//start Counters
				
				if(walkState != 0){
					//display count down
					Counter(0x06, walkState);
					walkState = 0xff;
				}
				else{
					//display road sign
					Counter(0x06, 0x00);
				}
				state++;
			}
			case 0x03:
			{
				nsY = 1;
				ewR = 1;
				//start Counters
				if(walkState == 0xff){
					//flash
					Counter(0x06, walkState);
					walkState = 0;
				}
				else{
					//display road sign
					Counter(0x06, 0x00);
				}
				state = 0;
			}
		}
	}
	
	/*
	LCD_Init();	

	LCD_String("ElectronicWINGS"); 
	LCD_Command(0xC0);
	LCD_String("Hello World"); 

	while(1);
	*/
}

void Counter(unsigned short int lengthSeconds, unsigned char walkState)
{
	int counter;
	if (walkState != 0){
		if(walkState == 0x01){
			//count down
			counter = 0;
			while(counter < lengthSeconds*1000)
			{
				//send byte
				//printByte(lengthSeconds - (counter/1000));
				//counter
				TMOD = 0x01;
				TL0 = ;
				TH0 = ;
				TR0 = 1
				while(~TF0);
				counter++;
				TF0 = 0;
			
		}
		else{
			//flash
			//flash led on-off
			//counter
			TMOD = 0x01;
			TL0 = ;
			TH0 = ;
			TR0 = 1
			while(~TF0);
			counter++;
			TF0 = 0;
		}
	}
	else{
		//show road sign
		//counter
		TMOD = 0x01;
		TL0 = ;
		TH0 = ;
		TR0 = 1
		while(~TF0);
		counter++;
		TF0 = 0;
	}
}

void LCD_Init (void)		/* LCD Initialize function */
{	
	delay(20);		/* LCD Power ON Initialization time >15ms */
	LCD_Command (0x38);	/* Initialization of 16X2 LCD in 8bit mode */
	LCD_Command (0x0C);	/* Display ON Cursor OFF */
	LCD_Command (0x06);	/* Auto Increment cursor */
	LCD_Command (0x01);	/* clear display */
	LCD_Command (0x80);	/* cursor at home position */
}

void delay(unsigned int count)  /* Function to provide delay Approx 1ms */
{
	int i,j;
	for(i=0;i<count;i++)
	for(j=0;j<112;j++);
}

void LCD_Command (unsigned char cmd)  /* LCD16x2 command funtion */
{
	lcd_data_port= cmd;
	rs=0;			/* command reg. */
	rw=0;			/* Write operation */
	en=1; 
	delay(1);
	en=0;
	delay(5);
}

void LCD_Char (unsigned char char_data)  /* LCD data write function */
{
	lcd_data_port=char_data;
	rs=1;			/* Data reg.*/
	rw=0;			/* Write operation*/
	en=1;   				
	delay(1);
	en=0;
	delay(5);
}

void LCD_String (unsigned char *str) /* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)  /* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);  /* Call LCD data write */
	}
}

void LCD_String_xy (char row, char pos, char *str)  /* Send string to LCD function */
{
	if (row == 0)
	LCD_Command((pos & 0x0F)|0x80);
	else if (row == 1)
	LCD_Command((pos & 0x0F)|0xC0);
	LCD_String(str);	/* Call LCD string function */
}



