
#include "stdio.h"
#include <process.h>
#include "database_fun.h"//mysqlΪ64λ�汾�������Ҫ��x64ģʽ�µ���
#include "amqp_fun.h"
#include "sys.h"

//�������ݿ����ȫ�ֱ���

//mq���ȫ�ֱ���
int main()
{
	LogWrite(INFO, "%s", "---------------------------------------------------------------");
	LogWrite(INFO, "%s", "---------------------------------------------------------------");
	LogWrite(INFO, "%s", "Smart Counter Scheme Server Start!");

	LogWrite(INFO, "%s", "Init System Start!");
	Init_System();

	//��ʼ��������������
	init_amqp();
	//_beginthread(run_listen, 0, NULL);
	while (1)
	{
		run_listen("dummy");
		Destory_connection(conn , atoi(server->mq_channel));
		Sleep(5000);
		Sleep(5000);
	}
	getchar();
}








