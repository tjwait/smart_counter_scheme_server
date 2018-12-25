
#ifndef _global_h
#define _global_h

#ifdef	__cplusplus
extern "C" {
#endif

#include "database_fun.h"
#include "log.h"

#define CF(a , b)  (a##b)
//#define CFF(a)     (#a)
//#define CFFF(a)    CFF(a)

//extern struct counter_info * counter;
//extern struct Board_Info * board_info;//称重板信息头指针
extern struct server_info * server;
extern struct scheme scheme_globle[10];
//extern HANDLE hCom_C;
//extern HANDLE hCom_Tem;

extern unsigned char  Tem[];

#ifdef	__cplusplus
}
#endif

#endif /* _global_h */

