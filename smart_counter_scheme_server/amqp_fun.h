#pragma once //���Ԥ���빦��ͬifndef define ������������

#include "global.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parson.h"

#include <amqp.h>
#include <amqp_tcp_socket.h>

#include <assert.h>

#include "utils.h"

#define AMQP_FUN_SUCCESS 0
#define AMQP_FUN_FAILURE -1

struct Send_Message_Buf
{
	char * message;
	int Isused;//message���Ƿ���δ�������ݣ�0Ϊû�У�1Ϊ�У�message����ִ��ĳһ��ҵ����Ϻ������ݵ�ʱ��û�з��ͳɹ������������ݾͲ��ܱ��ͷ�
			   //�����������̣������ֲ��ܷ������ݵ����������̻߳��˳��������������ӷ�����������ڷ�����ÿ�����ӳɹ��󣬶����ж����Ƿ���û��
			   //���͵����ݣ���������ȷ��ͺ����ͷ�
};

extern amqp_connection_state_t conn;

int init_amqp();
int run_listen(void * dummy);
void Destory_connection(amqp_connection_state_t conn, amqp_channel_t channel);

