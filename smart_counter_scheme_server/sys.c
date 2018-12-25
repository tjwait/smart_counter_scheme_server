
#include "sys.h"
#include "math.h"

#include "stdio.h"
#include "windows.h"

struct Sys_Tem sys_tem;

/*
*	���ܣ�ϵͳ������ʼ��
*	������
*	˵����ϵͳ������ʼ��������������Ϊ
			1�����ݿ����ӳ�ʼ��
			2����ȡ���ӻ�����Ϣ����������ṹ����
			3����ȡ���ذ������Ϣ�����������Ӧ�ṹ����
			4���������ӳ�ʼ��
			5�����Գ��ذ��Ƿ���������
			6�����Թ������ӷ�������·�Ƿ�����

*/
void Init_System(void)
{
	LogWrite(INFO, "%s", "Init DataBase Start!");
	int recon = 0;
	while ( (init_db() == DB_FAILURE) && (recon < 4) )
	{
		Sleep(2000);
		recon++;
	}
	if (recon >= 4)
	{
		LogWrite(ERR, "%s", "Init DataBase Failed!");
		exit(-1);//���ݿ�����ʧ��ʱ���ش������ֱ���˳�����
	}
	else
	{
		LogWrite(INFO, "%s", "Init DataBase SUCCESS!");
	}
	//init_db();

}



/*
*	���ܣ�ϵͳ�쳣���
*	������
*	˵����
*/
//���������һ���ɱ������������Ҫ������־������ڴ˷���һ��
void sys_die(const char *fmt, ...) {
	va_list ap;//�����һ��char * 
	va_start(ap, fmt);//�˺��ǽ�apָ��ʡ�Ժ�"..."ǰ�����һ�������ĵ�ַ��������Ϊfmt
	vfprintf(stderr, fmt, ap);//�˺����ǽ�fmt�������ap����Ҫ���͵�stderr���У�stderr��Ϊϵͳ������쳣�������fmt�������Ϊһ���ַ�����ap������Ϊ��
							  //��fmt�ַ����и�ʽ������Ĳ������硰aa%d��������ap��Ҫ��%d����Ӧ��ֵ�ģ��������Ϊap�ڵ���va_startʱָ����fmt������ʵ��
							  //ʹ��ʱ�Ǵӵڶ���Ԫ�ؿ�ʼʹ�õ�
	va_end(ap);//�ͷ�ap��Դ
	fprintf(stderr, "\n");//���stderr����Ϣ
	exit(1);//�˳�ϵͳ
}


/*
*	���ܣ�char �ַ����ڲ�����ΪASCII��ʽ��ʮ�������ַ��� AB��Ϊ  0x41  0x42�� ת��λ  byte��ʽ�� ��0xAB
*	������
*	˵����charָ��ı���Ϊ���ֵ�ascii������Ϊ����,
*/

byte ASCII_To_byte(unsigned char buf)
{
	if (buf >= '0' && buf <= '9')
	{
		return (buf - '0');
	}
	else if (buf >= 'A' && buf <= 'F')
	{
		return (buf - 'A' + 10);
	}
	else if (buf >= 'a' && buf <= 'f')
	{
		return (buf - 'a' + 10);
	}
}

/*
*	���ܣ�char �ַ� �����ַ��� ת double��
*	������
*	˵����
*/

double CharNum_To_Double( const unsigned char * data)
{
	char * p = data;
	double value = 0;
	double value_dec = 0;
	int sw = 0;//����С���л�
	int i = 0;
	while (*p != '\0')
	{
		if (*p >= '0' && *p <= '9' && sw == 0)
		{
			value = value * 10 + (*p - '0');
		}
		else if (*p == '.')
		{
			if(sw == 0)
			{
				sw = 1;
			}
			else
			{
				//���ܳ�������С����
				return 0;
			}
			
		}
		else if (*p >= '0' && *p <= '9' && sw == 1)
		{
			value_dec = value_dec * 10 + (*p - '0');
			i++;
		}
		else
		{
			//���뵽��˴������ַ��������з�double������
			return 0;
		}
		p++;
	}

	value = (value + value_dec / pow(10,i));
	return value;
}

/*
*	���ܣ�double ����ת char ����
*	������
*	˵����С���㱣����λ,����λ��������,�����������ã�Ҫ��֤�������ݵ����鳤���㹻�ã��������Զ���ĩβ��ӽ�����ʶ��'\0'
*/

void Double_To_CharArray(double num, unsigned char * data)
{
	unsigned char buf[20];
	unsigned char temp;
	int num_buf = abs(num * 1000);//������������ͳһ�ĳ�����
	if ((num_buf % 10) >= 5) { num_buf += 10; }
	num_buf /= 10;
	int num_len = 1;
	if (num_buf == 0)
	{
		//��������������0,��ֱ�Ӹ�ֵ������
		data[0] = '0';
		data[1] = '.';
		data[2] = '0';
		data[3] = '0';
		data[4] = '\0';
		return;
	}
	while (num_buf / (pow(10, num_len)) >= 1)
	{
		num_len++;
	}
	
	for (int i = 0 ; i < num_len; i++)
	{
		buf[i] = num_buf % 10;
		buf[i] += '0';
		num_buf /= 10;
	}
	//ǰ��˳��ߵ�
	for (int i = 0, j = num_len - 1; i < j; i++, j--)
	{
		temp = buf[i];
		buf[i] = buf[j];
		buf[j] = temp;
	}
	//���˴�������С���㱣����λ�ķ�ʽ��num_len��ʱ����Ϊ3������������1λС����Ϊ������num_len��СӦ��Ϊ4λ
	if (num_len < 3)
	{
		for (int i = 2; i >= 0; i--)
		{
			if (num_len == 0)
			{
				buf[i] = '0';
			}
			else
			{
				buf[i] = buf[--num_len];
			}
			
		}
		num_len = 3;
	}
	/*
	if (num >= 0.1 && num < 1)
	{
		//��������double����С��1����˴���ָ�������ַ�����??��,�����ʺ�ΪС�����������λ���֣���˴˴�Ҫ�ڸ��ַ�����ͷ��һ��0�ַ�
		//����ת������������С�������û������
		for (int i = num_len; i > 0; i--)
		{
			buf[i] = buf[i - 1];
		}
		buf[0] = '0';//���ӿ�ͷ��0�ַ�
	}
	*/
	buf[num_len] = buf[num_len - 1];
	buf[num_len-1] = buf[num_len - 2];
	buf[num_len - 2] = '.';//��ԭ����ĵ����ڶ���λ�������С���㣬�����ַ�������һλ�����ȼ�1
	num_len++;//�������鳤�ȼ�1

	if (num < 0)
	{
		data[0] = '-';//��ԭ��С���㣬�����Ӹ���
		data[num_len + 1] = '\0';
	}
	else
	{
		data[num_len] = '\0';
	}
		
	for (int i = 0; i < num_len; i++)//ע��˴���С����num_len����ΪҪ��һ��С�������Ҫ��һλ
	{
		if (num < 0)
		{
			data[i+1] = buf[i];
		}
		else
		{
			data[i] = buf[i];
		}
	}

}

/*
*	���ܣ���������ת char ����
*	������
*	˵�����������Ϊ���������֣���ֱ��ȥ��С��,�����������ã�Ҫ��֤�������ݵ����鳤���㹻�ã��������Զ���ĩβ��ӽ�����ʶ��'\0'
*/

void Int_To_CharArray(int num, unsigned char * data)
{
	unsigned char buf[20];
	unsigned char temp;
	int num_buf = abs(num);//������������ͬ��ĳ�����
	int num_len = 1;
	while (num_buf / (pow(10, num_len)) >= 1)
	{
		num_len++;
	}

	for (int i = 0; i < num_len; i++)
	{
		buf[i] = num_buf % 10;
		buf[i] += '0';
		num_buf /= 10;
	}
	for (int i = 0, j = num_len - 1; i < j; i++, j--)
	{
		temp = buf[i];
		buf[i] = buf[j];
		buf[j] = temp;
	}
	
	if (num < 0)
	{
		data[0] = '-';//��ԭ��С���㣬�����Ӹ���
		data[num_len + 1] = '\0';
	}
	else
	{
		data[num_len] = '\0';
	}

	for (int i = 0; i < num_len; i++)
	{
		if (num < 0)
		{
			data[i + 1] = buf[i];
		}
		else
		{
			data[i] = buf[i];
		}
	}

}

/*
*	���ܣ�hex�ַ������������ֽ�Ϊһ��hex�����и�ʽ��������byte����
*	������
*	˵����

*/

void HexStringFormatForPrintf(unsigned char * in , int in_len , unsigned char * out)
{
	unsigned char buf[200];
	unsigned char * p = in;
	int k = 0;
	int out_len = 0;

	for (int i = 0; i < in_len; i++)
	{
		if (*p != ' ')
		{
			buf[out_len++] = *p;
		}
		p++;
	}

	buf[out_len] = '\0';

	for (int j = 0; j < out_len; j++)
	{
		/*
		if (buf[j] >= 0x00 && buf[j] <= 0x09)
		{
			out[k++] = buf[j] + '0';
		}
		*/
		/*
		else if (buf[j] >= 0x0A && buf[j] <= 0x0F)
		{
			out[k++] = buf[j] - 10 + 'A';
		}
		*/
		if (buf[j] == '$' || buf[j] == '@' || buf[j] == '#')
		{
			out[k++] = buf[j];
		}
		else
		{
			out[k++] = (buf[j] >> 4 & 0x0f) <= 9 ? (buf[j] >> 4 & 0x0f) + '0' : (buf[j] >> 4 & 0x0f) - 10 + 'A';
			out[k++] = (buf[j] & 0x0f) <= 9 ? (buf[j] & 0x0f) + '0' : (buf[j] & 0x0f) - 10 + 'A';
		}
	}
	out[k] = '\0';
}


/*
*	���ܣ�GBKתutf8
*	������[in] GBK �����ַ���
*		  [out] UTF8 �����ַ���
*		  [in] UTF8 �����ַ������ȣ�ֻҪ��֤�����ܹ��н���������ݼ���
*	˵����
*/
int GBKToUTF8(unsigned char * lpGBKStr, unsigned char * lpUTF8Str, int nUTF8StrLen)
{

	wchar_t * lpUnicodeStr = NULL;

	int nRetLen = 0;

	if (!lpGBKStr) //���GBK�ַ���ΪNULL������˳�
	{
		return 0;
	}

	nRetLen = MultiByteToWideChar(CP_ACP, 0, (char *)lpGBKStr, -1, NULL, NULL); //��ȡת����Unicode���������Ҫ���ַ��ռ䳤��

	lpUnicodeStr = (wchar_t *)malloc(sizeof(wchar_t) * (nRetLen + 1));	//new WCHAR[nRetLen + 1]; //ΪUnicode�ַ����ռ�

	nRetLen = MultiByteToWideChar(CP_ACP, 0, (char *)lpGBKStr, -1, lpUnicodeStr, nRetLen); //ת����Unicode����

	if (!nRetLen) //ת��ʧ��������˳�
	{
		return 0;
	}
	nRetLen = WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, NULL, 0, NULL, NULL); //��ȡת����UTF8���������Ҫ���ַ��ռ䳤��

	if (!lpUTF8Str) //���������Ϊ���򷵻�ת������Ҫ�Ŀռ��С
	{
		if (lpUnicodeStr)
		{
			free(lpUnicodeStr);
		}
		return nRetLen;

	}

	if (nUTF8StrLen < nRetLen) //���������������Ȳ������˳�

	{
		if (lpUnicodeStr)
		{
			free(lpUnicodeStr);
		}
		return 0;
	}

	nRetLen = WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, (char *)lpUTF8Str, nUTF8StrLen, NULL, NULL); //ת����UTF8����

	if (lpUnicodeStr)//ת����ϣ������ͷŸ���Դ
	{
		free(lpUnicodeStr);
	}
	return nRetLen;

}

/*
*	���ܣ�utf8תGBK
*	������[in] UTF8 �����ַ���
*		  [out] GBK �����ַ���
*		  [in] GBK �����ַ������ȣ�ֻҪ��֤�����ܹ��н���������ݼ���
*	˵����
*/
int UTF8ToGBK(unsigned char * lpUTF8Str, unsigned char * lpGBKStr, int nGBKStrLen)

{

	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if (!lpUTF8Str) //���UTF8�ַ���ΪNULL������˳�
	{
		return 0;
	}

	nRetLen = MultiByteToWideChar(CP_UTF8, 0, (char *)lpUTF8Str, -1, NULL, NULL); //��ȡת����Unicode���������Ҫ���ַ��ռ䳤��

	lpUnicodeStr = (wchar_t *)malloc(sizeof(wchar_t) * (nRetLen + 1));//new WCHAR[nRetLen + 1]; //ΪUnicode�ַ����ռ�

	nRetLen = MultiByteToWideChar(CP_UTF8, 0, (char *)lpUTF8Str, -1, lpUnicodeStr, nRetLen); //ת����Unicode����
	if (!nRetLen) //ת��ʧ��������˳�
	{
		return 0;
	}

	nRetLen = WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr, -1, NULL, NULL, NULL, NULL); //��ȡת����GBK���������Ҫ���ַ��ռ䳤��

	if (!lpGBKStr) //���������Ϊ���򷵻�ת������Ҫ�Ŀռ��С
	{
		if (lpUnicodeStr)
		{
			free(lpUnicodeStr);
		}
		return nRetLen;
	}

	if (nGBKStrLen < nRetLen) //���������������Ȳ������˳�
	{
		if (lpUnicodeStr)
		{
			free(lpUnicodeStr);
		}
		return 0;
	}

	nRetLen = WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr, -1, (char *)lpGBKStr, nRetLen, NULL, NULL); //ת����GBK����

	if (lpUnicodeStr)
	{
		free(lpUnicodeStr);
	}
		
	return nRetLen;
}


/*��ʱ���Ժ���-----------------------------------------------------------*/
int is_valid_utf8(const char *string, size_t string_len)
{
	int len = 0;
	const char *string_end = string + string_len;
	while (string < string_end) {
		if (!verify_utf8_sequence((const unsigned char*)string, &len)) {
			return 0;
		}
		string += len;
	}
	return 1;
}
#define IS_CONT(b) (((unsigned char)(b) & 0xC0) == 0x80) /* is utf-8 continuation byte */
static int verify_utf8_sequence(const unsigned char *string, int *len) {
	unsigned int cp = 0;
	*len = num_bytes_in_utf8_sequence(string[0]);

	if (*len == 1) {
		cp = string[0];
	}
	else if (*len == 2 && IS_CONT(string[1])) {
		cp = string[0] & 0x1F;
		cp = (cp << 6) | (string[1] & 0x3F);
	}
	else if (*len == 3 && IS_CONT(string[1]) && IS_CONT(string[2])) {
		cp = ((unsigned char)string[0]) & 0xF;
		cp = (cp << 6) | (string[1] & 0x3F);
		cp = (cp << 6) | (string[2] & 0x3F);
	}
	else if (*len == 4 && IS_CONT(string[1]) && IS_CONT(string[2]) && IS_CONT(string[3])) {
		cp = string[0] & 0x7;
		cp = (cp << 6) | (string[1] & 0x3F);
		cp = (cp << 6) | (string[2] & 0x3F);
		cp = (cp << 6) | (string[3] & 0x3F);
	}
	else {
		return 0;
	}

	/* overlong encodings */
	if ((cp < 0x80 && *len > 1) ||
		(cp < 0x800 && *len > 2) ||
		(cp < 0x10000 && *len > 3)) {
		return 0;
	}

	/* invalid unicode */
	if (cp > 0x10FFFF) {
		return 0;
	}

	/* surrogate halves */
	if (cp >= 0xD800 && cp <= 0xDFFF) {
		return 0;
	}

	return 1;
}

static int num_bytes_in_utf8_sequence(unsigned char c) {
	if (c == 0xC0 || c == 0xC1 || c > 0xF4 || IS_CONT(c)) {
		return 0;
	}
	else if ((c & 0x80) == 0) {    /* 0xxxxxxx */
		return 1;
	}
	else if ((c & 0xE0) == 0xC0) { /* 110xxxxx */
		return 2;
	}
	else if ((c & 0xF0) == 0xE0) { /* 1110xxxx */
		return 3;
	}
	else if ((c & 0xF8) == 0xF0) { /* 11110xxx */
		return 4;
	}
	return 0; /* won't happen */
}

/*��ʱ���Ժ�����-------------------------------------------------*/


/*
*	���º�����ϵͳ�����򻯺�����õĺ���---------------------------------------------------------------------------------------------------
*/

/*
*	���ܣ�ִ�з�������
*	������[in] ���յ���json����
*		  [in] ���صĽ��
*		  [out] ����ִ�н��
*	˵�������ݷ��͹����ĸ�ʽӦ��Ϊdevid	cmdid	scheme_id	scheme_name	error_value	issave data[[] [] [] []]*dataΪitem����*
*/
char *  Procedure_Create_Scheme(JSON_Object * json_object)
{
	int error_code = 0;//0����һ������
					   //���ж����յ���json�ַ����е�data���е������Ƿ�Ϸ�
	if (json_object == NULL)
	{
		LogWrite(ERR, "%s", "SHEME_JSON_OBJECT_NULL");
		error_code = SHEME_JSON_OBJECT_NULL;
	}

	JSON_Array * sub_array_parse = json_object_get_array(json_object, "Data");
	JSON_Array  * sub_sub_array_parse = NULL;
	if (sub_array_parse != NULL)
	{
		//�˴��Ƿ������ķ������ɣ���˶��ڴ����items�ĺϷ���Ӧ����Ӧ�ó�����������Ʒ�ļƼ����͵ȣ��ڴ˺����в��ٽ��д���
		//���´����ǽ���items�������ݽṹ
		int items_num = json_array_get_count(sub_array_parse);//��ȡһ���ϴ��˶��ٸ�items
		int items_list_pos = 0;
		struct Items * item_list_head = NULL;
		struct Items * item_list_p = NULL;
		for (int i = 0; i < items_num; i++)
		{
			//��������ֻҪ������ƷΨһ��ţ���Ʒ���ƣ�Ԥ���ϼ���������Ʒ���أ���Ʒ���ۼۼ���
			if (items_list_pos == 0)//���Ϊ�׸��ڵ�
			{
				sub_sub_array_parse = json_array_get_array(sub_array_parse, i);//��ȡ��i��item��Ϣ
				item_list_p = item_list_head = (struct Items *)malloc(sizeof(struct Items));
				memset(item_list_head, 0, sizeof(struct Items));
				item_list_head->item_id = json_array_get_string(sub_sub_array_parse, 0);//item_id
				item_list_head->name = json_array_get_string(sub_sub_array_parse, 1);//name
				item_list_head->ind_count = json_array_get_string(sub_sub_array_parse, 2);//�ϼ�����
				item_list_head->ind_weight = json_array_get_string(sub_sub_array_parse, 3);//��Ʒ����
				item_list_head->ind_price = json_array_get_string(sub_sub_array_parse, 4);//���ۼ�
				items_list_pos = 1;
			}
			else//��������ڵ�
			{
				while (item_list_p->next != NULL) { item_list_p = item_list_p->next; }
				sub_sub_array_parse = json_array_get_array(sub_array_parse, i);//��ȡ��i��item��Ϣ
				item_list_p->next = (struct Item *)malloc(sizeof(struct Items));
				item_list_p = item_list_p->next;
				memset(item_list_p, 0, sizeof(struct Items));
				item_list_p->item_id = json_array_get_string(sub_sub_array_parse, 0);//item_id
				item_list_p->name = json_array_get_string(sub_sub_array_parse, 1);//name
				item_list_p->ind_count = json_array_get_string(sub_sub_array_parse, 2);//�ϼ�����
				item_list_p->ind_weight = json_array_get_string(sub_sub_array_parse, 3);//��Ʒ����
				item_list_p->ind_price = json_array_get_string(sub_sub_array_parse, 4);//���ۼ�
			}

		}
		//���뵽��˴�������������ɣ����ù�������
		error_code = Server_Scheme_Create(json_object_get_string(json_object, "scheme_id"), \
			json_object_get_string(json_object, "scheme_name"),
			item_list_head/*items����*/,
			CharNum_To_Double(json_object_get_string(json_object, "error_value"))/*���ֵ*/,
			CharNum_To_Double(json_object_get_string(json_object, "issave"))/*�Ƿ񽫷�������*/);

		//�ͷ�items������Դ
		item_list_p = item_list_head;
		struct Items * item_list_n = item_list_p;
		while (item_list_p != NULL)
		{
			item_list_n = item_list_p;
			item_list_p = item_list_p->next;
			free(item_list_n);
		}


	}
	else if (error_code == 0)
	{
		//���������
		error_code = 4;

	}

	JSON_Value *root_value = json_value_init_object();//��ʼ����һ��value��������Ϊobject��valueΪjson���ݵ�һ�����ͣ���ͬ��value���Ϳ���Ƕ��
	JSON_Object *root_object = json_value_get_object(root_value);//��ȡvalue�е�object�ĵ�ַ

	json_object_set_string(root_object, "cmdid", json_object_get_string(json_object, "cmdid"));
	time_t timep;
	time(&timep);
	char * res_num[20];
	Int_To_CharArray(error_code, res_num);
	json_object_dotset_string(root_object, "Result.Res", res_num);
	json_object_dotset_string(root_object, "Result.Date", ctime(&timep));
	//json_object_set_string(root_object, "Counter-SN", counter->sn);
	json_object_dotset_value(root_object, "Result.Data", "NULL");//û������
	char * json_string = (char *)malloc(json_serialization_size(root_value));//json_serialization_size��������ķ���ֵ�Ѿ������˽�ԭ�ַ������Ƚ�����չ����Ϊ��Ҫ����һЩ������ַ����绻�С���б�ܡ������ŵ�
	memset(json_string, 0, json_serialization_size(root_value));
	json_serialize_to_buffer(root_value, json_string, json_serialization_size(root_value));
	//�ͷ�json��Դ
	json_value_free(root_value);//ֻ���ͷŸ���Դ�����ڲ�������������Դ�ᱻ�ݹ��ͷ�

	return json_string;//��ָ������ź���������ʧЧ����ָ��ĸ��ڴ�������ⲿ��ʽ�ͷ�


}




/*
*	���ܣ��γ�����������͵�Ӧ�����Ϣ
*	������	[in] ���յ�����Ϣ���ţ�������ʱ�����ʵ�֣�
			[in] �������ƣ������
			[in] ����ִ�еĽ��
			[in] Ҫ���ص����������飬��ֵӦ����һ���Ѿ����õ����飬ÿһ��Ԫ�ض�Ϊ���飬��[ [] , [] , [] , [] , [] , [] ]
			[out]����Ϣ���ظ����ú���������json����Ҫ�ɵ��ú�����ʾ�ͷ�
*	˵����
*/
char *  Procedure_Answer_Message(char * message_sn , char * cmd_name , int Res , JSON_Value *sub_value)
{
	JSON_Value *root_value = json_value_init_object();//��ʼ����һ��value��������Ϊobject��valueΪjson���ݵ�һ�����ͣ���ͬ��value���Ϳ���Ƕ��
	JSON_Object *root_object = json_value_get_object(root_value);//��ȡvalue�е�object�ĵ�ַ
	char * change_code[100] = { 0 };
	//JSON_Value *sub_value = json_value_init_array();
	//JSON_Array *sub_array = json_value_get_array(sub_value);

	time_t timep;
	time(&timep);
	json_object_set_string(root_object, "MSN", message_sn);
	json_object_set_string(root_object, "devid", counter->sn);
	json_object_set_string(root_object, "cmdid", cmd_name);
	Int_To_CharArray(Res, change_code);
	json_object_dotset_string(root_object, "Result.Res", change_code);
	char * ctime_buf = ctime(&timep);
	ctime_buf[strlen(ctime_buf) - 1] = NULL;
	json_object_dotset_string(root_object, "Result.Date", ctime_buf);
	json_object_dotset_value(root_object, "Result.Data", sub_value);
	//�����ڴ������ͨ��smb.messageָ��ָ�򣬲����ڷ��ͳɹ����ͷ�
	//��ִ�е���sql��䣬���ҷ��ص����ݹ��࣬���´���������⣬��Ҫ����
	char * json_string = (char *)malloc(json_serialization_size(root_value));//json_serialization_size��������ķ���ֵ�Ѿ������˽�ԭ�ַ������Ƚ�����չ����Ϊ��Ҫ����һЩ������ַ����绻�С���б�ܡ������ŵ�
	json_serialize_to_buffer(root_value, json_string, json_serialization_size(root_value));
	
	//��up_message�����������
	SQL_INSERT_INTO_Up_Message(message_sn, json_string, timep);

	//�ͷ�json��Դ
	json_value_free(root_value);//ֻ���ͷŸ���Դ�����ڲ�������������Դ�ᱻ�ݹ��ͷ�
	return json_string;//��ָ������ź���������ʧЧ����ָ��ĸ��ڴ�������ⲿ��ʽ�ͷ�

}
