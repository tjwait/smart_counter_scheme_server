#pragma once

#include "database_fun.h"
#include "serial_port.h"
#include "global.h"
#include "parson.h"
#include "time.h"

struct Sys_Tem
{
	int Tem;//���һ��У׼ʱ���¶�
	signed char Tem_Cur;//������ֵ
	int Tem_Dis;//����У׼���¶�ƫ�����ʵ�ʲ���¶����� Tem��Tem_Re�����Χ�ھͲ���Ҫ����У׼����ͳ��
	int Time;//�¶ȳ�������У׼��Χ�Ĵ���
	int MaxTime;//��Time ���� MaxTimeʱ��IsCheck��λ
	int IsCheck;//�Ƿ���ҪУ׼ 0Ϊ����Ҫ 1Ϊ��Ҫ
	int delay;//�¶ȼ��ļ��
};


void Init_System(void);
void Board_Ready(void);
int Board_Basic_Value_Set(int times);
char * Board_Basic_Value_Set_With_ACK();
void Board_Curavture_Value_Set(int times, int save);
//void Board_Curavture_Value_Set(UINT16 weight, int times, int save);
void Board_Get_Weight();
void Init_Tem();
void Counter_Get_Tem();
int Locker_Open(); //��ϵͳ��ʹ��
int Locker_Open_Closed();//��ϵͳ��ʹ��
int Locker_Get_Stat();//��ϵͳ��ʹ��

char *  Procedure_Sales();
char *  Procedure_On_Shelf(JSON_Object * json_object);
char *  Procedure_Off_Shelf(JSON_Object * json_object);
char *  Procedure_Pick_And_Place(int Type);

void Parse_Usart_Data_Run();
void Send_CMD(HANDLE  * hCom, unsigned char * id, unsigned char CMD, unsigned char * cmd_data, int cmd_data_len, char dev_kind, int little);
void sys_die(const char *fmt, ...);
unsigned char Sum_Check(unsigned char * data, int len);
byte ASCII_To_byte(unsigned char buf);
double CharNum_To_Double(const unsigned char * data);
void Double_To_CharArray(double num, unsigned char * data);
void Int_To_CharArray(int num, unsigned char * data);
void HexStringFormatForPrintf(unsigned char * in, int in_len, unsigned char * out);
int GBKToUTF8(unsigned char * lpGBKStr, unsigned char * lpUTF8Str, int nUTF8StrLen);
int UTF8ToGBK(unsigned char * lpUTF8Str, unsigned char * lpGBKStr, int nGBKStrLen);
int is_valid_utf8(const char *string, size_t string_len);

//��ϵͳʹ�õ�ҵ��������ֵ����
#define LOCKER_UNLOCK_STATE_OK 0x00//�������������������Ѿ�����
#define LOCKER_UNLOCK_STATE_OPEN_ALREADY 0x02//�ڷ��Ϳ���ָ���ʱ�����Ѿ�����
#define LOCKER_UNLOCK_STATE_DONOT_OPEN 0x03//���������������ų�ʱ��û�б��򿪣����Զ�������
#define LOCKER_UNLOCK_STATE_ERROR 0xFF//�����ش���


#define LOCKER_GET_STATE_OPENING 0x01//�����ڱ���
#define LOCKER_GET_STATE_OK 0x02//������ִ����һ�ο��ض���
#define LOCKER_GET_STATE_WAITING_OPEN 0x04//�����ڵȴ�����
#define LOCKER_GET_STATE_ERROR 0xFF//�����ش���

#define SETTING_BASIC_VALUE_BOARD_ERROR 0x05//ȥƤ���ذ��Ŵ���
#define SETTING_CURAVTURE_VALUE_TOO_LIGHT 0x06//У׼����̫��
#define SETTING_CURAVTURE_VALUE_BOARD_ERROR 0x07//�趨����ֵ���ذ��Ŵ���

#define WEIGHT_VALUE_BOARD_ERROR 0x08//��ȡ�������ذ��Ŵ���
#define WEIGHT_VALUE_DATA_MISS 0x09 //��������ȱ�ٳ��ذ���Ϣ

#define SHOPPING_SETTING_CURAVTURE_VALUE_ERROR 0x10//�����������У�У׼���ڳ�������
#define SHOPPING_WEIGHT_VALUE_DATA_MISS 0x11//��������ȱ�ٳ��ذ���Ϣ

#define GEN_OK 0x00 //ͨ����������ֵ
#define GEN_MEM_OVER 0xFD //�ڴ����������
#define GEN_ERROR_COMMUNICATION_ERROR 0xFE //ͨ�Ŵ��󣬸ô�����һ��ͨ�ô�����ָ������ͬǶ��ʽ�豸ͨ���쳣



//��ϵͳ����ʹ�õĺ��������ϲ��ֺ���Ҳ����ϵͳ��ʹ��
char *  Procedure_Answer_Message(char * message_sn, char * cmd_name, int Res, JSON_Value *sub_value);
char *  Procedure_Open_Lock(JSON_Object * json_object);
char *  Procedure_Open_Close(JSON_Object * json_object);
char *  Procedure_Get_Locker_State(JSON_Object * json_object);
char *  Procedure_Basic_Value_Set(JSON_Object * json_object);
char *  Procedure_Set_Curavture_Value(JSON_Object * json_object);
char *  Procedure_Get_Board_State(void);
char *  Procedure_Get_Weight_Value(JSON_Object * json_object);
char *  Procedure_Sales_Ex(JSON_Object * json_object);

int Board_Basic_Value_Set_By_id(char * board_id, int times);
int Board_Curavture_Value_Set_Ex(char * board_id, UINT16 weight, int times , int save);
int Board_Get_Weight_Ex(char * board_id);
void Counter_Get_Tem_Ex();
