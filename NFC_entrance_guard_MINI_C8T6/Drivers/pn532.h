#ifndef _PN532_H
#define _PN532_H

typedef enum 
{
	PN532_OK = 0,
	PN532_ERR,
	
}PN532_STA;

PN532_STA pn532Read(void);




#endif
