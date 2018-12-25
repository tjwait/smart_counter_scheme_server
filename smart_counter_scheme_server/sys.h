#pragma once

#include "database_fun.h"
#include "serial_port.h"
#include "global.h"
#include "parson.h"
#include "time.h"


void Init_System(void);

void sys_die(const char *fmt, ...);
byte ASCII_To_byte(unsigned char buf);
double CharNum_To_Double(const unsigned char * data);
void Double_To_CharArray(double num, unsigned char * data);
void Int_To_CharArray(int num, unsigned char * data);
void HexStringFormatForPrintf(unsigned char * in, int in_len, unsigned char * out);
int GBKToUTF8(unsigned char * lpGBKStr, unsigned char * lpUTF8Str, int nUTF8StrLen);
int UTF8ToGBK(unsigned char * lpUTF8Str, unsigned char * lpGBKStr, int nGBKStrLen);
int is_valid_utf8(const char *string, size_t string_len);


#define SHEME_JSON_OBJECT_NULL	1
#define SHEME_ID_USED			2
#define SHEME_NAME_USED			3
#define SHEME_PRODUCTION_INFO_NULL	4
#define SHEME_PRODUCTION_KIND_TOO_MUCH	5

//新系统的所使用的函数，以上部分函数也在新系统中使用
char *  Procedure_Answer_Message(char * message_sn, char * cmd_name, int Res, JSON_Value *sub_value);
char *  Procedure_Create_Scheme(JSON_Object * json_object);
