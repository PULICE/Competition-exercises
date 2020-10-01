#include "stm32f10x.h"

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+12) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+12) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+8) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+8) //0x40022010 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 


#define LED1 PCout(13)


/*******************I2C IO口定?**********************/
	//IO方向设置
	//#define SDA_IN()  {GPIOB->CRL&=~(0x0F<<24);GPIOB->CRL|=0x04<<24;}	//PB6输入模式
	//#define SDA_OUT() {GPIOB->CRL&=~(0x0F<<24);GPIOB->CRL|=0x03<<24;} //PB6输出模式
	//IO操作函数	 
	//#define IIC_SCL    PBout(7) //SCL
	//#define IIC_SDA    PBout(6) //SDA	 
	//#define READ_SDA   PBin(6)  //输入SDA 
	
//全局配置

#ifndef __cplusplus
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#endif

#define use_capacitive_touch_panel//使用触摸屏

#endif
