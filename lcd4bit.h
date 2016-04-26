
extern void LCD_Ready(void);
extern void Init_Ports(void);
extern void Init_LCD(void);
extern void Write_Command(unsigned char);
extern void Write_data(unsigned char);
extern void Print_String(char *);
extern void gotoXy(unsigned char  x,unsigned char y);
extern void Init_Signal_Char();
extern void PrintSignalChar(unsigned char strength);