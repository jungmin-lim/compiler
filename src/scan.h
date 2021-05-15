#ifndef _SCAN_H_
#define _SCAN_H_

#define MAXTOKENLEN 45

extern char tokenString[MAXTOKENLEN+1];

TokenType getToken(void);

#endif