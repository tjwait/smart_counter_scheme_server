
#include "serial_port.h"

/*�������ݴ����� lumanman_ ��CSDN ���� ��ȫ�ĵ�ַ������https://blog.csdn.net/lumanman_/article/details/76275513?utm_source=copy */

HANDLE hCom;

/*
*	���ܣ���ʼ������
*	������
*	˵������ʧ��������˳�
*/
void init_serial_port(char * port, int baudrate)
{
	//�ȼ򵥵�ʵ������char * �򿪴���
	if (strcmp(port, "com1") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com1",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com2") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com2",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com3") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com3",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com4") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com4",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com5") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com5",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com6") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com6",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com7") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com7",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com8") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com8",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com9") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com9",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com10") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com10",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com11") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com11",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com12") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com12",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com13") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com13",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com14") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com14",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com15") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com15",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}
	else if (strcmp(port, "com16") == 0)
	{
		hCom = CreateFile(L"\\\\.\\com16",//COM1��		
			GENERIC_READ, //�����		
			0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
			NULL,
			OPEN_EXISTING, //�򿪶����Ǵ��� 		
			0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
			NULL);
	}


	/*CreateFile�����ĵ�һ������ԭ����ΪTEXT("comX")����"X"Ϊ������,TEXT�Ĺ��ܾ�����"com"ǰ�����һ��L����ΪL"com",��L������ַ���ΪUnicode����*/
	//hCom = CreateFile(port/*L"com8"*/,//COM1��		
	//			      GENERIC_READ, //�����		
	//	              0, //ָ���������ԣ����ڴ��ڲ��ܹ������Ըò�������Ϊ0		
	//	              NULL,		
	//	              OPEN_EXISTING, //�򿪶����Ǵ��� 		
	//	              0, //������������ֵΪFILE_FLAG_OVERLAPPED����ʾʹ���첽I/O���ò���Ϊ0����ʾͬ��I/O����		
	//	              NULL);

	if (hCom == INVALID_HANDLE_VALUE)
	{
		sys_die("��COMʧ��!\n");
	}
	else
	{
		printf("COM�򿪳ɹ���\n");
	}
	SetupComm(hCom, 1024, 1024); //���뻺����������������Ĵ�С����1024

	/*********************************��ʱ����**************************************/
	/* ����readFile��WriteFile��д���ڵ�ʱ����Ҫ���ǳ�ʱ���⣬��ʱ����������ָ����ʱ����û�ж������ָ���������ַ���
	ReadFile��WriteFile�Ĳ�����Ȼ�������Ҫ��ѯ��ǰ�ĳ�ʱ����Ӧ�õ���GetCommTimeouts�������ú��������һ��COMMTIMEOUTS
	�ṹ��
	*  ����SetCommTimeouts������ĳһ��COMMTIMEOUTS�����ó�ʱ����д���ڵĳ�ʱ�ܹ������֣������ʱ���ܳ�ʱ��
	   �����ʱ��ָ��ȡ�����ַ�֮�����ʱ���ܳ�ʱ��ָ��д�����ܹ����ѵ�ʱ�䡣д����ֻ֧���ܳ�ʱ�����������ֳ�ʱ��֧�֡�
	   ���г����ж�Ӧ�趨��ʱ�Ĳ�������ָ��ʱʱ���Ƕ�û������������Ҫ����֤
	*/
	COMMTIMEOUTS TimeOuts;	
	//�趨����ʱ,�����趨�Ķ���ʱ�����Ƿ�����ʽ�ģ���ִ�ж�ȡ�����Ͻ��Ѿ����յ������ݷ��أ�����ȴ�����ʹû���յ��κ��ַ�Ҳ����ȴ���
	//���⻹�������趨�����罫����������ʱ���趨ΪMAXDWORD���򽫻�ʵ�ֶ�ȡ����Ҫ���ֽ����Ż�ֹͣ����֪������ָ����Ҫ���ֽ����ںδ��趨��
	//���⻹���Խ�ReadIntervalTimeout�趨Ϊ0��������ʱ���������ã�ֻʹ���ܳ�ʱ
	TimeOuts.ReadIntervalTimeout = MAXDWORD;//�������ʱ�����������ַ�֮���ȡ�ĳ�ʱʱ�䣬��Ϊ0��Ĳ�����������
	//��ʱ��ϵ���Ͷ�ʱ�䳣��������Ҫ�໥��ϲ���ʹ�ã��乫ʽΪ
	//�ܵĶ�/д��ʱʱ�� = Read(Write)TotalTimeoutMultiplier x Ҫ��/д���ֽ��� + Read(Write)TotalTimeoutConstant
	TimeOuts.ReadTotalTimeoutMultiplier = 0;//��ʱ��ϵ��	
	TimeOuts.ReadTotalTimeoutConstant = 0;//��ʱ�䳣��	
	//�趨д��ʱ	
	TimeOuts.WriteTotalTimeoutMultiplier = 1;//дʱ��ϵ��	
	TimeOuts.WriteTotalTimeoutConstant = 1;//дʱ�䳣��	
	SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ 	
	
	/*****************************************���ô���***************************/	
	DCB dcb;	
	GetCommState(hCom, &dcb);	
	dcb.BaudRate = baudrate; //������Ϊ9600	
	dcb.ByteSize = 8; //ÿ���ֽ���8λ	
	dcb.Parity = NOPARITY; //����żУ��λ	
	dcb.StopBits = ONESTOPBIT; //һ��ֹͣλ	
	SetCommState(hCom, &dcb);

	return SERIAL_SUCCESS;

}

/*
*	���ܣ���ȡһ�δ�������
*	������[in]  hcom ���ھ��
		  [in]	* data ��ȡ�������ݣ�Ӧ��֤�����㹻��
		  [out] ����ֵΪ��ȡ�������ݳ���
*	˵����
*/
DWORD serial_read_once(HANDLE hCom , char * data)
{
	if (hCom == NULL)
	{
		return SERIAL_FAILURE;
	}
	DWORD wCount;//ʵ�ʶ�ȡ���ֽ���
	int bReadStat;

	//char str[100] = { 0 };

	bReadStat = ReadFile(hCom, data, sizeof(data), &wCount, NULL);
	if (!bReadStat)
	{
		printf("������ʧ��!");
		CloseHandle(hCom);
		return FALSE;
	}
	else
	{
		return wCount;
	}
}

void serial_read(HANDLE hCom)
{
	if (hCom == NULL)
	{
		return;
	}
	DWORD wCount;//ʵ�ʶ�ȡ���ֽ���
	int bReadStat;

	char str[100] = { 0 };

	while (1)
	{
		//PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR); //��ջ�����
		/* ���ɹ��򷵻ط�0�����ɹ��򷵻�0
		BOOL ReadFile(
						HANDLE       hFile,
						LPVOID       lpBuffer,
						DWORD        nNumberOfBytesToRead,
						LPDWORD      lpNumberOfBytesRead,
						LPOVERLAPPED lpOverlapped
					);
		*/
		bReadStat = ReadFile(hCom, str, sizeof(str), &wCount, NULL);
		if (!bReadStat)
		{
			printf("������ʧ��!");
			CloseHandle(hCom);
			return FALSE;
		}
		else
		{
			//��ִ��ReadFile���������ɹ�����˶�ȡ�����������û�ж�ȡ���κ����ݣ������Ҳ�ᵽ��˴������Ϊ�����õĶ�ȡ�����жϣ�
			//�˴������ж϶�ȡ���˶��ٸ��ֽ�������ʹ��wCount����
			printf("info : %s\r\n", str);//str��������ȡ�����ݣ��������ݵ����һλ��ӽ���������˴˴����Ὣstr��100��Ԫ��ȫ����ӡ����
			memset(str, 0, strlen(str));
		}
		Sleep(1000);
	}
}

/*
*	���ܣ�дȡһ�δ�������
*	������[in]  hcom ���ھ��
		  [in]	* data ��д�������
          [in]  int ��д�����ݳ���
		  [out] ִ�н��
*	˵����
*/
DWORD serial_write_once(HANDLE hCom, BYTE * data , DWORD len)
{
	if (hCom == NULL)
	{
		return SERIAL_FAILURE;
	}
	DWORD wRitten = 0;

	//char str[100] = { 0 };

	//if (WriteFile(hCom, data, len, &wRitten, 0))
	if (WriteFile(hCom, data, len, &wRitten, 0))
	{

		return wRitten;

	}
	else
	{
		DWORD dwError = GetLastError();
		return SERIAL_FAILURE;
	}
}





