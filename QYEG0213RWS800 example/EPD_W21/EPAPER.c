#include "epaper.h"

extern unsigned char Epaper_border;
void driver_delay_xms(unsigned long xms)	
{	
    unsigned long i = 0 , j=0;

    for(j=0;j<xms;j++)
	{
        for(i=0; i<256*40; i++);
    }
}

void Epaper_Spi_WriteByte(unsigned char TxData)//spi写
{				   			 
	unsigned char TempData;
	unsigned char scnt;
	TempData=TxData;

  EPD_W21_CLK_0;  
	for(scnt=0;scnt<8;scnt++)
	{ 
		if(TempData&0x80)
		  EPD_W21_MOSI_1 ;
		else
		  EPD_W21_MOSI_0 ;
		EPD_W21_CLK_1;  
	  EPD_W21_CLK_0;  
		TempData=TempData<<1;
  }
}



void Epaper_READBUSY(void)
{ 
  while(1)
  {	 //=1 BUSY
     if(isEPD_W21_BUSY==0) break;;
  }  
}



void Epaper_Write_Command(unsigned char cmd)
{
	EPD_W21_CS_1;
	EPD_W21_CS_0;
	EPD_W21_DC_0;  // D/C#   0:command  1:data

	Epaper_Spi_WriteByte(cmd);
	EPD_W21_CS_1;
}



void Epaper_Write_Data(unsigned char data)
{
	EPD_W21_CS_1;
	EPD_W21_CS_0;
	EPD_W21_DC_1;  // D/C#   0:command  1:data

	Epaper_Spi_WriteByte(data);
	EPD_W21_CS_1;
}



/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void EPD_HW_Init(void)
{
	EPD_W21_RST_0;     
	driver_delay_xms(100); 
	EPD_W21_RST_1; //hard reset  
	driver_delay_xms(100); 
#if 0
	Epaper_READBUSY();
	Epaper_Write_Command(0x12); // soft reset
	Epaper_READBUSY();
	
			Epaper_Write_Command(0x06);         //boost设定
		Epaper_Write_Data (0x17);
		Epaper_Write_Data (0x17);
		Epaper_Write_Data (0x17);
		
		Epaper_Write_Command(0x04);         //上电 
	//	lcd_chkstatus();                //查看芯片状态  
		
		Epaper_Write_Command(0X00);
		Epaper_Write_Data(0x0f);		//选择分辨率
		Epaper_Write_Data(0x0d);
		
		Epaper_Write_Command(0X50);
		Epaper_Write_Data(0x77);
		
		Epaper_Write_Command(0x61);			//像素设定
		Epaper_Write_Data (0x80);       //128像素	 
    Epaper_Write_Data (0x01);		//290像素 
    Epaper_Write_Data (0x28);
		
	//	pic_display();
		
		Epaper_Write_Command(0x12); 
	//	lcd_chkstatus(); 
		
		Epaper_Write_Command(0x02);			//power off
 //       lcd_chkstatus();
		Epaper_Write_Command(0X07);			//deep sleep
		Epaper_Write_Data(0xA5);
		driver_delay_xms(50000);
#endif
#if 1
	Epaper_READBUSY();
	Epaper_Write_Command(0x12); // soft reset
	Epaper_READBUSY();
	Epaper_Write_Command(0x06);         //boost设定
	Epaper_Write_Data (0x17);
	Epaper_Write_Data (0x17);
	Epaper_Write_Data (0x17);



	Epaper_Write_Command(0X00);
	Epaper_Write_Data(0x0f);		//选择分辨率
	Epaper_Write_Data(0x0d);
	Epaper_Write_Command(0x61); //set Ram-Y address start/end position          
	Epaper_Write_Data (0x80);       //128像素	 
	Epaper_Write_Data (0x01);		//290像素 
	Epaper_Write_Data (0x28); 


#endif	
	Epaper_READBUSY();
	
}
//////////////////////////////All screen update////////////////////////////////////////////
void EPD_ALL_image(const unsigned char *datas1,const unsigned char *datas2)
{
   unsigned int i;
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
    for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
			Epaper_Write_Data(*datas1); 
			datas1++;
   }	
    Epaper_Write_Command(0x26);   //write RAM for red(1)/white (0)
    for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
			Epaper_Write_Data(*datas2); 
			datas2++;
   }
   EPD_Update();		 
}



/////////////////////////////////////////////////////////////////////////////////////////
void EPD_Update(void)
{   
  Epaper_Write_Command(0x22); 
  Epaper_Write_Data(0xF7);   
  Epaper_Write_Command(0x20); 
  Epaper_READBUSY();   
}



void EPD_DeepSleep(void)
{  	
  Epaper_Write_Command(0x10); //enter deep sleep
  Epaper_Write_Data(0x01); 
  driver_delay_xms(100);
}



/////////////////////////////////Single display////////////////////////////////////////////////
void EPD_WhiteScreen_Red(void)

{
   unsigned int i,k;
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
	for(k=0;k<296;k++)
  {
		for(i=0;i<20;i++)
		{
			Epaper_Write_Data(0xff);
			}
  }
	
    Epaper_Write_Command(0x26);   //write RAM for red(1)/white (0)
	for(k=0;k<296;k++)
  {
		for(i=0;i<20;i++)
		{
			Epaper_Write_Data(0xff);
			}
  }
	
	EPD_Update();
}



void EPD_WhiteScreen_Black(void)

{
   unsigned int i,k;
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
	for(k=0;k<296;k++)
  {
		for(i=0;i<20;i++)
		{
			Epaper_Write_Data(0x00);
			}
  }
	
    Epaper_Write_Command(0x26);   //write RAM for red(1)/white (0)
	for(k=0;k<296;k++)
  {
		for(i=0;i<20;i++)
		{
			Epaper_Write_Data(0x00);
			}
  }
	
		EPD_Update();
}




void EPD_WhiteScreen_White(void)

{
   unsigned int i,k;
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
	for(k=0;k<296;k++)
  {
		for(i=0;i<20;i++)
		{
			Epaper_Write_Data(0xff);
			}
  }
	
    Epaper_Write_Command(0x26);   //write RAM for red(1)/white (0)
	for(k=0;k<296;k++)
  {
		for(i=0;i<20;i++)
		{
			Epaper_Write_Data(0x00);
			}
  }
	
	EPD_Update();
}


///////////////////////////Part update//////////////////////////////////////////////
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,const unsigned char color_mode,unsigned int PART_COLUMN,unsigned int PART_LINE)
{
	unsigned int i; 
	unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
	
	x_start=(x_start+8)/8;//转换为字节
	x_end=x_start+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_start;
	if(y_start>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_start+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_start);    		// RAM x address start
	Epaper_Write_Data(x_end);    			// RAM x address end
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    	// RAM y address start
	Epaper_Write_Data(y_start1);   	 	// RAM y address start
	Epaper_Write_Data(y_end2);    		// RAM y address end
	Epaper_Write_Data(y_end1);    		// RAM y address end


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_start); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);

  if(color_mode==MONO)
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)

  if(color_mode==RED)
    Epaper_Write_Command(0x26);   //write RAM for black(0)/white (1)

	for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(* datas);
			datas++;
   } 

	EPD_Update();

}



void EPD_Dis_Part_mult(unsigned int x_startA,unsigned int y_startA,const unsigned char * datasA1,const unsigned char * datasA2,
	                     unsigned int x_startB,unsigned int y_startB,const unsigned char * datasB1,const unsigned char * datasB2,
											 unsigned int PART_COLUMN,unsigned int PART_LINE)
{
	unsigned int i;  
	unsigned int x_endA,y_startA1,y_startA2,y_endA1,y_endA2;
	unsigned int x_endB,y_startB1,y_startB2,y_endB1,y_endB2;	
	
	//Data A////////////////////////////
	x_startA=(x_startA+8)/8;//转换为字节
	x_endA=x_startA+PART_LINE/8-1; 
	
	y_startA1=0;
	y_startA2=y_startA;
	if(y_startA2>=256)
	{
		y_startA1=y_startA2/256;
		y_startA2=y_startA2%256;
	}
	y_endA1=0;
	y_endA2=y_startA+PART_COLUMN-1;
	if(y_endA2>=256)
	{
		y_endA1=y_endA2/256;
		y_endA2=y_endA2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startA);    // RAM x address start at 00h;
	Epaper_Write_Data(x_endA);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_startA2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_startA1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_endA2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_endA1);   

	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startA); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_startA2);
	Epaper_Write_Data(y_startA1);
	
//	 Epaper_Write_Command(0x24);   //Write Black and White image to RAM
//   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
//   {                         
//     Epaper_Write_Data(*datasA1);
//			datasA1++;
//   } 
	 
	 Epaper_Write_Command(0x26);   //Write Black and White image to RAM
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(*datasA2);
			datasA2++;
   } 
	 
	//Data B/////////////////////////////////////
	x_startB=(x_startB+8)/8;//转换为字节
	x_endB=x_startB+PART_LINE/8-1; 
	
	y_startB1=0;
	y_startB2=y_startB;
	if(y_startB2>=256)
	{
		y_startB1=y_startB2/256;
		y_startB2=y_startB2%256;
	}
	y_endB1=0;
	y_endB2=y_startB+PART_COLUMN-1;
	if(y_endB2>=256)
	{
		y_endB1=y_endB2/256;
		y_endB2=y_endB2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startB);    // RAM x address start at 00h;
	Epaper_Write_Data(x_endB);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_startB2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_startB1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_endB2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_endB1);    

	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startB); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_startB2);
	Epaper_Write_Data(y_startB1);
	
	 Epaper_Write_Command(0x24);   //Write Black and White image to RAM
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(*datasB1);
			datasB1++;
   } 	

	 Epaper_Write_Command(0x26);   //Write Black and White image to RAM
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(*datasB2);
			datasB2++;
   } 		 
	 
	 EPD_Update();

}
//////////////////////////////////////////////////////////////////////////////////////
