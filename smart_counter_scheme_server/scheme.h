#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "global.h"
#include "sys.h"
#include "database_fun.h"

#define MAX_SCHEMES 10//���������е�����
#define MAX_NUMBER 200//number�ṹ��Ľڵ��������ýڵ�����������һ����Ʒ��Ϣ
//�����ṹ�壬�˽ṹ��Ӧ�û�����������Ʒ����Ϣ��������ȷ�������ڴ˼�¼��,�ýṹ�������ɺ�ͨ�����ͬboard���й�����һ��boardֻ�ܹ���һ��scheme
struct scheme
{
	char * scheme_id;
	char * scheme_name;
	int interval;//���������ͬ�������֮������������СӦ��Ϊ���٣���ֵӦ��Ϊ�������ֵ������
	int scheme_count;//һ���ж��ٸ�������������н���������һ�����������ÿһ���ڵ㶼��Ӧһ������
	int error_count;
	double error_per;//����İٷֱ�����
	struct scheme_node * schemes;
	int Isused;//��ȫ�ַ���������ʹ�ã���ʾ�ýڵ��Ƿ�ʹ��,�ڷ������ɺ����в�ʹ�ã�0Ϊδʹ�ã�1Ϊʹ��
};

struct scheme_node
{
	int schemem_node_id;
	char * scheme_id;//�����Ľ���������
	//char * schemem_node_name;
	int schemem_node_weight;//����������������ֵ��Ҫ�����ҵ�
	double  schemem_node_price;
	struct scheme_node * next;
	struct scheme_node_items_list_node * items_list_node;

};

struct scheme_node_items_list_node
{
	int  scheme_node_items_list_node_id;
	int  schemem_node_id;
	char *  scheme_id;
	char * item_id;
	char * item_name;
	double ind_price;
	int item_num;
	struct scheme_node_items_list_node * next;
};


//�㷨ʹ�ýṹ��
struct number
{
	struct Items * addr;
	int num;
};

int Server_Scheme_Create(char * scheme_id, char * scheme_name, struct Items * item_p/*items����*/, int error_value/*���ֵ*/, int save/*�Ƿ񽫷�������*/);
//int Scheme_Create(char * scheme_id, char * scheme_name, int error_value, int save);
void Free_scheme(struct scheme * scheme_p);
void Free_Scheme_Arrya_Node(char * scheme_id, int delete_db/*�Ƿ�ɾ���÷��������б������Ϣ*/);
//int Scheme_Create()

#ifdef	__cplusplus
}
#endif




