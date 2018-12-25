
#include "sys.h"
#include "math.h"

#include "stdio.h"
#include "windows.h"

struct Sys_Tem sys_tem;

/*
*	功能：系统启动初始化
*	参数：
*	说明：系统启动初始化函数胡，流程为
			1、数据库连接初始化
			2、获取柜子基本信息，并填充至结构体中
			3、获取称重板基本信息，并填充至对应结构体中
			4、串口链接初始化
			5、测试称重板是否都链接正常
			6、测试柜子连接服务器链路是否正常

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
		exit(-1);//数据库连接失败时严重错误，因此直接退出程序
	}
	else
	{
		LogWrite(INFO, "%s", "Init DataBase SUCCESS!");
	}
	//init_db();

}



/*
*	功能：系统异常输出
*	参数：
*	说明：
*/
//这个函数是一个可变参数函数，主要用于日志输出，在此分析一下
void sys_die(const char *fmt, ...) {
	va_list ap;//这个是一个char * 
	va_start(ap, fmt);//此宏是将ap指向省略号"..."前面最后一个变量的地址，此例子为fmt
	vfprintf(stderr, fmt, ap);//此函数是将fmt输出按照ap参数要求发送到stderr流中，stderr流为系统定义的异常输出流，fmt可以理解为一个字符串，ap可以认为是
							  //是fmt字符串中格式化输出的参数，如“aa%d”，则在ap中要有%d所对应的值的，因此我认为ap在调用va_start时指向了fmt，但其实际
							  //使用时是从第二个元素开始使用的
	va_end(ap);//释放ap资源
	fprintf(stderr, "\n");//输出stderr流信息
	exit(1);//退出系统
}


/*
*	功能：char 字符（内部编码为ASCII形式的十六进制字符如 AB则为  0x41  0x42） 转换位  byte形式的 如0xAB
*	参数：
*	说明：char指向的必须为数字的ascii，否则为错误,
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
*	功能：char 字符 数字字符串 转 double型
*	参数：
*	说明：
*/

double CharNum_To_Double( const unsigned char * data)
{
	char * p = data;
	double value = 0;
	double value_dec = 0;
	int sw = 0;//整数小数切换
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
				//不能出现两个小数点
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
			//代码到达此处代表字符串里面有非double型数据
			return 0;
		}
		p++;
	}

	value = (value + value_dec / pow(10,i));
	return value;
}

/*
*	功能：double 数字转 char 数组
*	参数：
*	说明：小数点保留两位,第三位四舍五入,正负数均试用，要保证接收数据的数组长度足够用，函数会自动在末尾添加结束标识符'\0'
*/

void Double_To_CharArray(double num, unsigned char * data)
{
	unsigned char buf[20];
	unsigned char temp;
	int num_buf = abs(num * 1000);//不论正负，先统一改成正数
	if ((num_buf % 10) >= 5) { num_buf += 10; }
	num_buf /= 10;
	int num_len = 1;
	if (num_buf == 0)
	{
		//如果输入的数字是0,则直接赋值并返回
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
	//前后顺序颠倒
	for (int i = 0, j = num_len - 1; i < j; i++, j--)
	{
		temp = buf[i];
		buf[i] = buf[j];
		buf[j] = temp;
	}
	//到此处，根据小数点保留两位的方式，num_len此时必须为3，下面再增加1位小数点为，最终num_len最小应该为4位
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
		//如果输入的double数字小于1，则此处该指数会变成字符串“??”,两个问好为小数点后保留的两位数字，因此此处要在该字符串开头加一个0字符
		//否则转化出来的数据小数点左侧没有数据
		for (int i = num_len; i > 0; i--)
		{
			buf[i] = buf[i - 1];
		}
		buf[0] = '0';//增加开头的0字符
	}
	*/
	buf[num_len] = buf[num_len - 1];
	buf[num_len-1] = buf[num_len - 2];
	buf[num_len - 2] = '.';//在原数组的倒数第二个位置上添加小数点，整个字符串后移一位，长度加1
	num_len++;//整个数组长度加1

	if (num < 0)
	{
		data[0] = '-';//若原数小于零，则增加负号
		data[num_len + 1] = '\0';
	}
	else
	{
		data[num_len] = '\0';
	}
		
	for (int i = 0; i < num_len; i++)//注意此处是小等于num_len，因为要加一个小数点因此要多一位
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
*	功能：整数数字转 char 数组
*	参数：
*	说明：若输入的为浮点型数字，则直接去除小数,正负数均试用，要保证接收数据的数组长度足够用，函数会自动在末尾添加结束标识符'\0'
*/

void Int_To_CharArray(int num, unsigned char * data)
{
	unsigned char buf[20];
	unsigned char temp;
	int num_buf = abs(num);//不论正负，先同意改成正数
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
		data[0] = '-';//若原数小于零，则增加负号
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
*	功能：hex字符串按照两个字节为一个hex数进行格式化并返回byte数组
*	参数：
*	说明：

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
*	功能：GBK转utf8
*	参数：[in] GBK 编码字符串
*		  [out] UTF8 编码字符串
*		  [in] UTF8 编码字符串长度，只要保证长度能够承接输出的数据即可
*	说明：
*/
int GBKToUTF8(unsigned char * lpGBKStr, unsigned char * lpUTF8Str, int nUTF8StrLen)
{

	wchar_t * lpUnicodeStr = NULL;

	int nRetLen = 0;

	if (!lpGBKStr) //如果GBK字符串为NULL则出错退出
	{
		return 0;
	}

	nRetLen = MultiByteToWideChar(CP_ACP, 0, (char *)lpGBKStr, -1, NULL, NULL); //获取转换到Unicode编码后所需要的字符空间长度

	lpUnicodeStr = (wchar_t *)malloc(sizeof(wchar_t) * (nRetLen + 1));	//new WCHAR[nRetLen + 1]; //为Unicode字符串空间

	nRetLen = MultiByteToWideChar(CP_ACP, 0, (char *)lpGBKStr, -1, lpUnicodeStr, nRetLen); //转换到Unicode编码

	if (!nRetLen) //转换失败则出错退出
	{
		return 0;
	}
	nRetLen = WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, NULL, 0, NULL, NULL); //获取转换到UTF8编码后所需要的字符空间长度

	if (!lpUTF8Str) //输出缓冲区为空则返回转换后需要的空间大小
	{
		if (lpUnicodeStr)
		{
			free(lpUnicodeStr);
		}
		return nRetLen;

	}

	if (nUTF8StrLen < nRetLen) //如果输出缓冲区长度不够则退出

	{
		if (lpUnicodeStr)
		{
			free(lpUnicodeStr);
		}
		return 0;
	}

	nRetLen = WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, (char *)lpUTF8Str, nUTF8StrLen, NULL, NULL); //转换到UTF8编码

	if (lpUnicodeStr)//转换完毕，最终释放该资源
	{
		free(lpUnicodeStr);
	}
	return nRetLen;

}

/*
*	功能：utf8转GBK
*	参数：[in] UTF8 编码字符串
*		  [out] GBK 编码字符串
*		  [in] GBK 编码字符串长度，只要保证长度能够承接输出的数据即可
*	说明：
*/
int UTF8ToGBK(unsigned char * lpUTF8Str, unsigned char * lpGBKStr, int nGBKStrLen)

{

	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if (!lpUTF8Str) //如果UTF8字符串为NULL则出错退出
	{
		return 0;
	}

	nRetLen = MultiByteToWideChar(CP_UTF8, 0, (char *)lpUTF8Str, -1, NULL, NULL); //获取转换到Unicode编码后所需要的字符空间长度

	lpUnicodeStr = (wchar_t *)malloc(sizeof(wchar_t) * (nRetLen + 1));//new WCHAR[nRetLen + 1]; //为Unicode字符串空间

	nRetLen = MultiByteToWideChar(CP_UTF8, 0, (char *)lpUTF8Str, -1, lpUnicodeStr, nRetLen); //转换到Unicode编码
	if (!nRetLen) //转换失败则出错退出
	{
		return 0;
	}

	nRetLen = WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr, -1, NULL, NULL, NULL, NULL); //获取转换到GBK编码后所需要的字符空间长度

	if (!lpGBKStr) //输出缓冲区为空则返回转换后需要的空间大小
	{
		if (lpUnicodeStr)
		{
			free(lpUnicodeStr);
		}
		return nRetLen;
	}

	if (nGBKStrLen < nRetLen) //如果输出缓冲区长度不够则退出
	{
		if (lpUnicodeStr)
		{
			free(lpUnicodeStr);
		}
		return 0;
	}

	nRetLen = WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr, -1, (char *)lpGBKStr, nRetLen, NULL, NULL); //转换到GBK编码

	if (lpUnicodeStr)
	{
		free(lpUnicodeStr);
	}
		
	return nRetLen;
}


/*临时测试函数-----------------------------------------------------------*/
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

/*临时测试函数完-------------------------------------------------*/


/*
*	以下函数是系统经过简化后的所用的函数---------------------------------------------------------------------------------------------------
*/

/*
*	功能：执行方案生成
*	参数：[in] 接收到的json数据
*		  [in] 返回的结果
*		  [out] 函数执行结果
*	说明：数据发送过来的格式应该为devid	cmdid	scheme_id	scheme_name	error_value	issave data[[] [] [] []]*data为item内容*
*/
char *  Procedure_Create_Scheme(JSON_Object * json_object)
{
	int error_code = 0;//0代表一切正常
					   //先判定接收到的json字符串中的data域中的数据是否合法
	if (json_object == NULL)
	{
		LogWrite(ERR, "%s", "SHEME_JSON_OBJECT_NULL");
		error_code = SHEME_JSON_OBJECT_NULL;
	}

	JSON_Array * sub_array_parse = json_object_get_array(json_object, "Data");
	JSON_Array  * sub_sub_array_parse = NULL;
	if (sub_array_parse != NULL)
	{
		//此处是服务器的方案生成，因此对于传入的items的合法性应该有应用程序负责处理，如商品的计价类型等，在此函数中不再进行处理
		//以下代码是建立items链表数据结构
		int items_num = json_array_get_count(sub_array_parse);//获取一共上传了多少个items
		int items_list_pos = 0;
		struct Items * item_list_head = NULL;
		struct Items * item_list_p = NULL;
		for (int i = 0; i < items_num; i++)
		{
			//传入数据只要求有商品唯一编号，商品名称，预计上架数量，商品单重，商品零售价即可
			if (items_list_pos == 0)//如果为首个节点
			{
				sub_sub_array_parse = json_array_get_array(sub_array_parse, i);//获取第i个item信息
				item_list_p = item_list_head = (struct Items *)malloc(sizeof(struct Items));
				memset(item_list_head, 0, sizeof(struct Items));
				item_list_head->item_id = json_array_get_string(sub_sub_array_parse, 0);//item_id
				item_list_head->name = json_array_get_string(sub_sub_array_parse, 1);//name
				item_list_head->ind_count = json_array_get_string(sub_sub_array_parse, 2);//上架数量
				item_list_head->ind_weight = json_array_get_string(sub_sub_array_parse, 3);//商品单重
				item_list_head->ind_price = json_array_get_string(sub_sub_array_parse, 4);//零售价
				items_list_pos = 1;
			}
			else//如果其它节点
			{
				while (item_list_p->next != NULL) { item_list_p = item_list_p->next; }
				sub_sub_array_parse = json_array_get_array(sub_array_parse, i);//获取第i个item信息
				item_list_p->next = (struct Item *)malloc(sizeof(struct Items));
				item_list_p = item_list_p->next;
				memset(item_list_p, 0, sizeof(struct Items));
				item_list_p->item_id = json_array_get_string(sub_sub_array_parse, 0);//item_id
				item_list_p->name = json_array_get_string(sub_sub_array_parse, 1);//name
				item_list_p->ind_count = json_array_get_string(sub_sub_array_parse, 2);//上架数量
				item_list_p->ind_weight = json_array_get_string(sub_sub_array_parse, 3);//商品单重
				item_list_p->ind_price = json_array_get_string(sub_sub_array_parse, 4);//零售价
			}

		}
		//代码到达此处代表链表构建完成，调用构建函数
		error_code = Server_Scheme_Create(json_object_get_string(json_object, "scheme_id"), \
			json_object_get_string(json_object, "scheme_name"),
			item_list_head/*items链表*/,
			CharNum_To_Double(json_object_get_string(json_object, "error_value"))/*误差值*/,
			CharNum_To_Double(json_object_get_string(json_object, "issave"))/*是否将方案保存*/);

		//释放items链表资源
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
		//数据域错误
		error_code = 4;

	}

	JSON_Value *root_value = json_value_init_object();//初始化了一个value，其类型为object，value为json数据的一个类型，不同的value类型可以嵌套
	JSON_Object *root_object = json_value_get_object(root_value);//获取value中的object的地址

	json_object_set_string(root_object, "cmdid", json_object_get_string(json_object, "cmdid"));
	time_t timep;
	time(&timep);
	char * res_num[20];
	Int_To_CharArray(error_code, res_num);
	json_object_dotset_string(root_object, "Result.Res", res_num);
	json_object_dotset_string(root_object, "Result.Date", ctime(&timep));
	//json_object_set_string(root_object, "Counter-SN", counter->sn);
	json_object_dotset_value(root_object, "Result.Data", "NULL");//没有数据
	char * json_string = (char *)malloc(json_serialization_size(root_value));//json_serialization_size这个函数的返回值已经考虑了将原字符串长度进行扩展，因为还要增加一些额外的字符，如换行、反斜杠、大括号等
	memset(json_string, 0, json_serialization_size(root_value));
	json_serialize_to_buffer(root_value, json_string, json_serialization_size(root_value));
	//释放json资源
	json_value_free(root_value);//只需释放根资源，其内部关联的所有资源会被递归释放

	return json_string;//改指针会随着函数跳出而失效，但指向的该内存必须在外部显式释放


}




/*
*	功能：形成向服务器发送的应答的消息
*	参数：	[in] 接收到的消息串号（可以用时间戳等实现）
			[in] 命令名称，即编号
			[in] 命令执行的结果
			[in] 要返回的数据子数组，该值应该是一个已经填充好的数组，每一个元素都为数组，即[ [] , [] , [] , [] , [] , [] ]
			[out]将消息返回给调用函数，其中json数据要由调用函数显示释放
*	说明：
*/
char *  Procedure_Answer_Message(char * message_sn , char * cmd_name , int Res , JSON_Value *sub_value)
{
	JSON_Value *root_value = json_value_init_object();//初始化了一个value，其类型为object，value为json数据的一个类型，不同的value类型可以嵌套
	JSON_Object *root_object = json_value_get_object(root_value);//获取value中的object的地址
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
	//以下内存会最终通过smb.message指针指向，并且在发送成功后被释放
	//若执行的是sql语句，而且返回的数据过多，以下代码会有问题，需要调整
	char * json_string = (char *)malloc(json_serialization_size(root_value));//json_serialization_size这个函数的返回值已经考虑了将原字符串长度进行扩展，因为还要增加一些额外的字符，如换行、反斜杠、大括号等
	json_serialize_to_buffer(root_value, json_string, json_serialization_size(root_value));
	
	//向up_message表中添加数据
	SQL_INSERT_INTO_Up_Message(message_sn, json_string, timep);

	//释放json资源
	json_value_free(root_value);//只需释放根资源，其内部关联的所有资源会被递归释放
	return json_string;//改指针会随着函数跳出而失效，但指向的该内存必须在外部显式释放

}
