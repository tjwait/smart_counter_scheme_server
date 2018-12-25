
#ifndef _database_fun_h
#define _database_fun_h

#ifdef	__cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include <mysql.h>
#include <assert.h>
#include "global.h"
#include "sys.h"
#include "scheme.h"
#include "parson.h"

#define DB_SUCCESS 0
#define DB_FAILURE 1



	struct server_info
	{
		char * mq_server_sn;
		char * mq_server_name;
		char * mq_server_ip;
		char * mq_server_port;
		char * mq_exchange_name;
		char * mq_queue_name;
		char * mq_routingkey;
		char * mq_channel;
		char * mq_name;
		char * mq_pw;
		int max_kind;
		int max_buy;
		int error_value;
	};

//#define BOARD_STAT_UNKNOW 0//状态未知，启动后在连接测试之前是这个状态
//#define BOARD_STAT_OK	1//连接正常
//#define BOARD_STAT_ERROR	2//状态异常
//
//#define ISNEEDCUR_NO 0
//#define ISNEEDCUR_YES 1

	//struct Board_Info
	//{
	//	unsigned char * id;//称重板地址
	//	char * name;//名称，此值我认为可以暂不使用
	//	char * type;//此称重板计价类型
	//	char * scheme_id;//当计价模式为2时，绑定的方案编号
	//	struct Board_Info * next;
	//	int Board_Stat;//板子连接情况
	//	struct Items * items;
	//	UINT16 board_items_weight_all;//该层称重板上所有货物的总重量,两个字节足够，而且不能出现负值
	//	UINT16 board_items_weight_all_after_close_door;//每次销售货物的时候关门后各个称重板上的重量
	//	UINT8 IsBasicValue;//判定该称重板是否已经去皮完毕
	//	UINT8 IsBasicValueSave;//去皮是否成功保存
	//	UINT8 ISNeedCur;//是否需要校准
	//	UINT8 ISCurvatureValue;//判定校准是否成功
	//	UINT8 ISCurvatureValueSave;//判定校准是否保存成功
	//	union {
	//		float f;
	//		unsigned char x[4];
	//	} Curvature;
	//	//float Curvature;
	//};

	//销售的商品
	struct Items 
	{
		char * item_id;
		char * barcode;//商品流通用条码，未来计划面向商超码或者其他使用，暂预留
		unsigned char * board_id;
		char * name;
		char * supplier;
		char * saletype;
		char * ind_count;
		char * ind_weight;
		char * ind_price;
		char * weight_sum;
		char * weight_price;
		struct Items * next;
	};

	extern struct server_info * server;

	//数据库初始化函数，使用之前先调用该函数
	int init_db();
	//获取目标服务器的连接信息
	int Get_Server_Info();
	//获取柜子里面的称重板信息
	int Get_Board_Info();
	//获取柜子里面的商品信息，并将各个商品同称重板进行数据连接，即不同的称重板内的items链表的初始化
	int Get_Item_Info();
	//释放items链表内存
	void Free_Item_Info();
	//利用数据库信息获取各个称重板商品总重
	void Get_Boards_Items_Weight();
	//获取方案信息
	void Get_Scheme(char * scheme_id);
	//删除指定编号方案的数据库数据
	void Free_Scheme_DB(char * scheme_id);
	//关闭数据库
	int db_close();


	//SQL相关函数
	INT64 SQL_SELECT(char * tablename, char * condition_name, char * condition_value);
	//更新的值为字符串的
	INT64 SQL_UPDATA1(char * tablename, char * col_name1, char * col_value1, char * condition_name, char * condition_value);
	//更新的值为数字的，注意数字也是通过字符串传输的，只是sql语句不一样
	INT64 SQL_UPDATA2(char * tablename, char * col_name1, char * col_value1, char * condition_name, char * condition_value);

	INT64 SQL_INSERT_INTO_ItemsTable(char * item_id, char * barcode, char * board_id, char * name, char * supplier, char * saletype, \
		char * ind_count, char * ind_weight, char * ind_price, char * weight_sum, char * weight_price);
	INT64 SQL_INSERT_INTO_Scheme(struct scheme * scheme_p);
	INT64 SQL_DELETE(char * tablename, char * condition_name, char * condition_value);

	//向上行数据流水表中插入新的数据
	INT64 SQL_INSERT_INTO_Up_Message(char * msn, char * message, time_t timep);
	//获取上行数据流水表中的某一个记录的message数据
	char * Get_up_message_message(char * tablename, char * condition_name, char * condition_value);

	char * Procedure_SQL_Select(JSON_Object * json_object);
	char * Procedure_SQL_Updata(JSON_Object * json_object);

	int Scheme_Parse(char * scheme_id, double weight_buf, double * weight_buf_fix, double * price, JSON_Value **sub_sub_value);

#ifdef	__cplusplus
}
#endif

#endif /* _database_fun_h */
