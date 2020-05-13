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
sbit ewG = P0^0;
sbit ewY = P0^1;
sbit ewR = P0^2;
sbit nsG = P0^3;
sbit nsY = P0^4;
sbit nsR = P0^5;


//#define TESTING
unsigned char walkState = 0;
unsigned char trafficSign[] = "Heavy traffic on 405";

void main()
{
	unsigned char state = 0;
	IE = 0x81; //enable external interupt P3.2
	LCD_Init();
	
	while(1){
		ewG = 1;
		ewY = 1;
		ewR = 1;
		ewG = 1;
		ewY = 1;
		ewR = 1;
		switch(state){
			case 0x00:
			{
				nsY = 1;
				ewR = 1;
				ewG = 0;
				nsR = 0;
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
				ewG = 1;
				ewY = 0;
				//start Counters
				if(walkState == 0xff){
					//flash
					Counter(0x03, walkState);
					walkState = 0;
				}
				else{
					//display road sign
					Counter(0x03, 0x00);
				}
				state++;
			}
			case 0x02:
			{
				ewY = 1;
				nsG = 0;
				ewR = 0;
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
				nsG = 1;
				nsY = 0;
				ewR = 0;
				//start Counters
				if(walkState == 0xff){
					//flash
					Counter(0x03, walkState);
					walkState = 0;
				}
				else{
					//display road sign
					Counter(0x03, 0x00);
				}
				state = 0;
			}
		}
	}
}

void walk(void) interrupt 0{
	walkState = 0x01;
}

void Counter(unsigned short int lengthSeconds, unsigned char walkState)
{
	int counter;
	if (walkState != 0){
		if(walkState == 0x01){
			//count down
			counter = 0;
			//send byte
			//printByte(lengthSeconds - (counter/1000));
			LCD_Command (0x01);	//clear display
			LCD_Command (0x80); //move cursor to home
			LCD_String("print counter");
			while(counter < lengthSeconds*1000)
			{
				//counter
				TMOD = 0x01;
				TL0 = 0x66;
				TH0 = 0xFC;
				TR0 = 1;
				while(~TF0);
				counter++;
				TF0 = 0;
			}
			TR0 = 0;
		}
		else{
			//flash
			counter = 0;
			//flash lcd on-off
			LCD_Command (0x01);	//clear display
			LCD_Command (0x80); //move cursor to home
			LCD_String("flash lcd");
			while(counter < lengthSeconds*1000)
			{
				//counter
				TMOD = 0x01;
				TL0 = 0x66;
				TH0 = 0xFC;
				TR0 = 1;
				while(~TF0);
				counter++;
				TF0 = 0;
			}
			TR0 = 0;
		}
	}
	else{
		
		counter = 0;
		//show road sign
		LCD_Command (0x01);	//clear display
		LCD_Command (0x80); //move cursor to home
		LCD_String("show sign");
		while(counter < lengthSeconds*1000)
		{
			//counter
			TMOD = 0x01;
			TL0 = 0x66;
			TH0 = 0xFC;
			TR0 = 1;
			while(~TF0);
			counter++;
			TF0 = 0;
		}
		TR0 = 0;
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



