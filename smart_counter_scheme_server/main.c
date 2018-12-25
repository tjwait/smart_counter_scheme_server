
#include "stdio.h"
#include <process.h>
#include "database_fun.h"//mysql为64位版本，因此需要在x64模式下调试
#include "amqp_fun.h"
#include "sys.h"

//连接数据库相关全局变量

//mq相关全局变量
int main()
{
	LogWrite(INFO, "%s", "---------------------------------------------------------------");
	LogWrite(INFO, "%s", "---------------------------------------------------------------");
	LogWrite(INFO, "%s", "Smart Counter Scheme Server Start!");

	LogWrite(INFO, "%s", "Init System Start!");
	Init_System();

	//初始化柜子网络内容
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








