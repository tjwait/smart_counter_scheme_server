#include "scheme.h"


struct scheme  scheme_globle[MAX_SCHEMES] = {0};//����10���������ĸ����ذ�����ַ����ɳ��ذ��Ӧ���Ծ���

int STOCK_KIND = 7;//���ֵ��Ϊ���⿪��
int MAX_BUY = 0;//���ֵӦ�����з��������������ÿһ�ֵ�����

/*
*	���ܣ�server������һ���������
*	������
*	˵�����˺����Ƿ����������ɽ�������������������뷽��Ψһ���ƺ�Ҫ�����items�������ݣ������������ɿͻ��˷�������
*			items�����еļƼ����ͱ���Ϊ2������������Ʒ�����λ������
*/
int Server_Scheme_Create(char * scheme_id, char * scheme_name, struct Items * item_p/*items����*/, int error_value/*���ֵ*/, int save/*�Ƿ񽫷�������*/)
{
	//�ȼ���·�����ź������Ƿ�����ͬ�ģ��������ʧ��
	
	if (SQL_SELECT("smart_sales_counter.scheme", "scheme_id", scheme_id) >= 1)
	{
		printf("���������ͬ���޷����㷽��\r\n");
		return -1;
	}
	if (SQL_SELECT("smart_sales_counter.scheme", "name", scheme_name) >= 1)
	{
		printf("����������ͬ���޷����㷽��\r\n");
		return -2;
	}
	
	if (item_p == NULL)
	{
		printf("��Ʒ����Ϊ�㣬�޷����н���\r\n");
		return -3;
	}

	//��ʼͳ����Ʒ��������������
	int items_kind_num = 0;
	int items_num = 0;
	struct Items * itp = item_p;
	//ͳ�����������ж��ٸ���Ʒ�����Լ���Ʒ�����ж��ٸ�
	while (itp != NULL)
	{
		items_kind_num++;
		items_num += (int)CharNum_To_Double(itp->ind_count);
		itp = itp->next;
	}

	if (items_kind_num > STOCK_KIND)//STOCK_KIND���ֵӦ�����㷨�Ļ���������Ӧ�÷������ݿ���
	{
		printf("������Ʒ������࣬�޷�����\r\n");
		return -4;
	}

	int stock_kind = items_kind_num;
	int max_buy = items_num;//�ܹ��������������Ǳ��������Ʒ���������

	//��ʼ����
	struct scheme scheme;
	memset(&scheme, 0, sizeof(struct scheme));
	scheme.scheme_id = (char *)malloc(strlen(scheme_id) + 1);
	strcpy(scheme.scheme_id, scheme_id);
	scheme.scheme_name = (char *)malloc(strlen(scheme_name) + 1);
	strcpy(scheme.scheme_name, scheme_name);
	scheme.interval = error_value * 2 ;
	//scheme.error_count = 0;
	//scheme.error_per = 0;
	//scheme.scheme_count = 0;
	scheme.schemes = NULL;//��������
	//��ʼ��number�ṹ��
	struct number number[200] = { 0 };//һ�����ӵ�һ�����ذ����Ҳ�������200����Ʒ
	int i = 0;
	itp = item_p;
	while (itp != NULL)
	{
		//��number��ÿһ��Ԫ��ͬ�ó��ذ��item����Ԫ�ض�Ӧ��
		number[i].addr = itp;
		number[i++].num = 0;//��ʾ�����Ʒ���ߵ��������ڷ����㷨�������ĸ����˴��ǳ�ʼ��Ϊ0
		itp = itp->next;
	}
	//���뵽��˴�����ĳһ�����ذ��item����ͬnumber����һһ��Ӧ��ϣ����Կ�ʼĳһ�����ذ���㷨
	itp = item_p;
	for (int buy_num = 1; buy_num <= max_buy; buy_num++)//��MAX_BUY��������ʵ�ʸó��ذ��Ʒ�����������ʵ�ʻᰴ�ղ�Ʒ����������㷽��
	{
		test_new(&scheme/*�������Ϊ���ս��*/, itp,
			buy_num /*�����������ܹ������������Ʒ*/, 0/*��ֵΪѭ���ݹ������ȣ����ֵ������Ʒ����,�״ε��øú���ʱ��ԶΪ0*/,
			&number/*���ֵΪ�ݹ����ʱ��¼֮ǰ���в���ȡ��Ʒ�����ģ�Ϊһ����������ֵ������������Ϻ�ʹ��*/ , stock_kind);

	}
	//ĳһ�����ذ巽���Ѿ�������ϣ����㷽���Ĵ���ٷֱȣ�������ָĳ�������������ļ��С�ڳ��ذ��趨�����
	struct scheme_node * node_p_head = scheme.schemes;//ָ�򷽰��ṹ�����ڵķ����ڵ��ַ
	struct scheme_node * node_p_tail = node_p_head->next;
	double between_count = 0;
	//����û��������˲��ܰ���ŷ����ķ������д���
	while (node_p_head != NULL)
	{
		node_p_tail = node_p_head->next;

		while (node_p_tail != NULL)
		{
			between_count++;
			if (abs(node_p_head->schemem_node_weight - node_p_tail->schemem_node_weight) <= scheme.interval)
			{
				scheme.error_count += 1;
			}
			node_p_tail = node_p_tail->next;
		}

		node_p_head = node_p_head->next;
	}

	//ע���쳣�������ֵ�������쳣��ֵ�����ܵķ�����ʽ�����ǳ���ÿһ��ֵͬ��������ֵ�Ƚϵ��ܴ������ظ��Ĳ�����
	scheme.error_per = (scheme.error_count / between_count) * 100;
	//�������ݿ�
	if (save)
	{
		SQL_INSERT_INTO_Scheme(&scheme);
	}
	Free_scheme(&scheme);
	return 0;
}


/*
*	���ܣ�����һ���������
*	������
*	˵�����˺���Ӧ���Ǳ��û�����,ÿһ�����ذ��Ӧһ���������
		  �ú����Ǹ��ݵ�ǰ���ݿ��б���ĳ��ذ��ϵ���Ʒ��Ϣ��������������ѡ���Ƿ񽫽�������洢�����ݿ���
		  �ú������漰��ϵͳ�е�scheme_globalȫ�ֱ������κβ���
*/

//int Scheme_Create(char * scheme_id , char * scheme_name , int error_value , int save )
//{
//	
//	struct scheme scheme;
//	memset(&scheme, 0, sizeof(struct scheme));
//	scheme.scheme_id = (char *)malloc( strlen(scheme_id) + 1);
//	strcpy(scheme.scheme_id, scheme_id);
//	scheme.scheme_name = (char *)malloc(strlen(scheme_name) + 1);
//	strcpy(scheme.scheme_name, scheme_name);
//	scheme.interval = error_value * 2;
//	//scheme.error_count = 0;
//	//scheme.error_per = 0;
//	//scheme.scheme_count = 0;
//	scheme.schemes = NULL;//��������
//	
//	
//	STOCK_KIND = CharNum_To_Double(counter->max_kind);//�趨���ذ�����ܹ����õĲ�Ʒ���࣬���ֵ�����ݿ������
//	MAX_BUY = CharNum_To_Double(counter->max_buy);
//
//	int i = 0;
//	struct number number[200] = { 0 };//һ�����ӵ�һ�����ذ����Ҳ�������200����Ʒ
//	struct Board_Info * bip = board_info;
//	while (bip != NULL)//�������г��ذ�
//	{
//		struct Items * item_p = bip->items;
//		while (item_p != NULL)
//		{
//			//��number��ÿһ��Ԫ��ͬ�ó��ذ��item����Ԫ�ض�Ӧ��
//			number[i].addr = item_p;
//			number[i++].num = 0;//��ʾ�����Ʒ���ߵ��������ڷ����㷨�������ĸ����˴��ǳ�ʼ��Ϊ0
//			item_p = item_p->next;
//		}
//		STOCK_KIND = STOCK_KIND > i ? i : STOCK_KIND;//��ʵ�ʷ��õĲ�Ʒ��������ͬSTOCK_KIND���бȽϣ����ʵ�ʷ��õ�����С��STOCK_KIND����
//													 //ʵ�ʷ��õ�����Ϊ׼��ע��������õ��������������趨ֵ��������δ������Ӧ���ǰ������ݿ�
//													 //¼��˳����ڳ���STOCK_KIND�����Ĳ�Ʒ�������
//
//		//���뵽��˴�����ĳһ�����ذ��item����ͬnumber����һһ��Ӧ��ϣ����Կ�ʼĳһ�����ذ���㷨
//		for (int buy_num = 1; buy_num < MAX_BUY; buy_num++)//��MAX_BUY��������ʵ�ʸó��ذ��Ʒ�����������ʵ�ʻᰴ�ղ�Ʒ����������㷽��
//		{
//			test_new(&scheme/*�������Ϊ���ս��*/, bip->items,
//				buy_num /*�����������ܹ������������Ʒ*/, 0/*��ֵΪѭ���ݹ������ȣ����ֵ������Ʒ����,�״ε��øú���ʱ��ԶΪ0*/,
//				&number/*���ֵΪ�ݹ����ʱ��¼֮ǰ���в���ȡ��Ʒ�����ģ�Ϊһ����������ֵ������������Ϻ�ʹ��*/);
//
//		}
//
//		//ĳһ�����ذ巽���Ѿ�������ϣ����㷽���Ĵ���ٷֱȣ�������ָĳ�������������ļ��С�ڳ��ذ��趨�����
//		struct scheme_node * node_p_head = scheme.schemes;//ָ�򷽰��ṹ�����ڵķ����ڵ��ַ
//		struct scheme_node * node_p_tail = node_p_head->next;
//		double between_count = 0;
//		//����û��������˲��ܰ���ŷ����ķ������д���
//		while (node_p_head != NULL)
//		{
//			node_p_tail = node_p_head->next;
//
//			while (node_p_tail != NULL)
//			{
//				between_count++;
//				if (abs(node_p_head->schemem_node_weight - node_p_tail->schemem_node_weight) <= scheme.interval)
//				{
//					scheme.error_count += 1;
//				}
//				node_p_tail = node_p_tail->next;
//			}
//
//			node_p_head = node_p_head->next;
//		}
//
//		//ע���쳣�������ֵ�������쳣��ֵ�����ܵķ�����ʽ�����ǳ���ÿһ��ֵͬ��������ֵ�Ƚϵ��ܴ������ظ��Ĳ�����
//		scheme.error_per = (scheme.error_count / between_count) * 100;
//		//�������ݿ�
//		if(save)
//		{
//			SQL_INSERT_INTO_Scheme(&scheme);
//		}
//
//		bip = bip->next;
//	}
//	
//	//�ͷ���Դ
//	/*
//	struct scheme * scheme_p = &scheme;
//	if (scheme_p != NULL)
//	{
//		
//		struct scheme_node * scheme_node_p_next = scheme_p->schemes;
//		struct scheme_node * scheme_node_before = scheme_p->schemes;
//		while (scheme_node_p_next != NULL)
//		{
//
//			struct scheme_node_items_list_node * scheme_node_items_list_node_p_next = scheme_node_p_next->items_list_node;
//			struct scheme_node_items_list_node * scheme_node_items_list_node_p_before = scheme_node_p_next->items_list_node;
//			while (scheme_node_items_list_node_p_next != NULL)
//			{
//				scheme_node_items_list_node_p_before = scheme_node_items_list_node_p_next;
//				scheme_node_items_list_node_p_next = scheme_node_items_list_node_p_next->next;
//				free(scheme_node_items_list_node_p_before);
//
//			}
//
//			scheme_node_before = scheme_node_p_next;
//			scheme_node_p_next = scheme_node_p_next->next;
//			free(scheme_node_before);
//		}
//		//free(scheme_p); ����Ҫ�˾䣬scheme��һ���ֲ������������˳���ñ����ͻ�ʧЧ,�����ڲ��ı�ź���������������ͨ��malloc����ģ���Ҫ�����ͷ�
//		free(scheme_p->scheme_id);
//		free(scheme_p->scheme_name);
//	}
//	*/
//	Free_scheme(&scheme);
//
//}



int test_new(struct scheme * scheme_list/*�������Ϊ���ս��*/, struct Items * items_list,
	int max_buy /*�����������ܹ������������Ʒ*/, int depth/*��ֵΪѭ���ݹ������ȣ����ֵ������Ʒ����,�״ε��øú���ʱ��ԶΪ0*/,
	struct number * number/*���ֵΪ�ݹ����ʱ��¼֮ǰ���в���ȡ��Ʒ�����ģ�Ϊһ����������ֵ������������Ϻ�ʹ��*/,int stock_kind)
{
	int num_buf = 0;
	int max = CharNum_To_Double(items_list->ind_count) > max_buy ? max_buy : CharNum_To_Double(items_list->ind_count);//�趨��ǰ�㣨��ǰ������Ʒ����ܹ����򼸸���

	if (depth == stock_kind - 1)
	{
		for (int k = 0; k < depth; k++)
		{
			//printf("number are : %d\r\n", number[k]);
			num_buf += number[k].num;
		}

		for (int i = 0; i <= max; i++)
		{
			if (num_buf + i == max_buy)
			{
				number[depth].num = i;//number ��ʱ��¼�������ܹ����������������ϣ�ÿһ��Ԫ��λ�ö�Ӧitems_listλ�ã�ÿһ��Ԫ�ص�ֵ������ȡ�˻��������
								  //���number����ĳ���һ��������Ʒ������֮�ͣ����ڲ�����Ԫ��֮��һ������MAX_BUY

				scheme_list->scheme_count = scheme_list->scheme_count + 1;//�����ڵ�������1
				struct scheme_node * node_p = scheme_list->schemes;//ָ�򷽰��ṹ�����ڵķ����ڵ��ַ
				if (node_p == NULL)
				{
					//���뵽��˴���������Ϊ��
					scheme_list->schemes = node_p = (struct scheme_node *)malloc(sizeof(struct scheme_node));
					memset(node_p, 0, sizeof(struct scheme_node));
					node_p->scheme_id = scheme_list->scheme_id;
					node_p->schemem_node_id = 0;//�׸��ڵ�
					node_p->next = NULL;
					node_p->items_list_node = NULL;//�����ĳһ��������������е����в�Ʒ������Ϣ����������ÿһ���ڵ㶼�������������Ĳ�Ʒ��Ϣ������

					for (int k = 0; k <= depth; k++)//depth�����items_list��ÿһ��Ԫ�ر�ţ����������1����˴�ѭ��k��ֵҪһֱȡ��depth
					{
						if (number[k].num > 0)//�ڱ�����Ʒ����ȡ�˻���
						{
							node_p->schemem_node_weight += number[k].num * CharNum_To_Double(number[k].addr->ind_weight);
							node_p->schemem_node_price += number[k].num * CharNum_To_Double(number[k].addr->ind_price);
							struct scheme_node_items_list_node * list_node_p = node_p->items_list_node;
							if (list_node_p == NULL)
							{
								//���뵽��˴�����Ϊ������item�������׸�Ԫ��
								node_p->items_list_node = list_node_p = (struct scheme_node_items_list_node *)malloc(sizeof(struct scheme_node_items_list_node));
								memset(list_node_p, 0, sizeof(struct scheme_node_items_list_node));
								list_node_p->item_id = (char *)malloc(strlen(number[k].addr->item_id) + 1);
								strcpy(list_node_p->item_id, number[k].addr->item_id);
								list_node_p->item_name = (char *)malloc(strlen(number[k].addr->name) + 1);
								strcpy(list_node_p->item_name, number[k].addr->name);
								list_node_p->ind_price = CharNum_To_Double(number[k].addr->ind_price);//��Ʒ����
								//list_node_p->item_id = number[k].addr->item_id;
								//list_node_p->item_name = number[k].addr->name;
								list_node_p->item_num = number[k].num;
								list_node_p->schemem_node_id = node_p->schemem_node_id;//��ڵ�����ĳһ�������ڵ���
								list_node_p->scheme_id = scheme_list->scheme_id;//����ڵ������ܷ������
								list_node_p->scheme_node_items_list_node_id = 0;//�ýڵ���
								list_node_p->next = NULL;
							}
							else
							{
								while (list_node_p->next != NULL) { list_node_p = list_node_p->next; }//Ѱ�����һ��Ԫ��
								list_node_p->next = (struct scheme_node_items_list_node *)malloc(sizeof(struct scheme_node_items_list_node));
								list_node_p->next->scheme_node_items_list_node_id = list_node_p->scheme_node_items_list_node_id + 1;
								list_node_p = list_node_p->next;
								memset(list_node_p, 0, sizeof(struct scheme_node_items_list_node));
								list_node_p->item_id = (char *)malloc(strlen(number[k].addr->item_id) + 1);
								strcpy(list_node_p->item_id, number[k].addr->item_id);
								list_node_p->item_name = (char *)malloc(strlen(number[k].addr->name) + 1);
								strcpy(list_node_p->item_name, number[k].addr->name);
								list_node_p->ind_price = CharNum_To_Double(number[k].addr->ind_price);//��Ʒ����
								//list_node_p->item_id = number[k].addr->item_id;
								//list_node_p->item_name = number[k].addr->name;
								list_node_p->item_num = number[k].num;
								list_node_p->schemem_node_id = node_p->schemem_node_id;
								list_node_p->scheme_id = scheme_list->scheme_id;//����ڵ������ܷ������
								list_node_p->next = NULL;

							}


						}
						
					}

				}
				else
				{
					//����Ϊ��
					while (node_p->next != NULL) { node_p = node_p->next; }//Ѱ�ҵ���������һ��Ԫ��
																		   //���뵽��˴���������Ϊ��
					node_p->next = (struct scheme_node *)malloc(sizeof(struct scheme_node));
					memset(node_p->next, 0, sizeof(struct scheme_node));
					node_p->next->schemem_node_id = node_p->schemem_node_id + 1;
					node_p = node_p->next;
					node_p->scheme_id = scheme_list->scheme_id;
					//node_p->schemem_node_id = 0;//�׸��ڵ�
					node_p->next = NULL;
					node_p->items_list_node = NULL;

					for (int k = 0; k <= depth; k++)//depth�����items_list��ÿһ��Ԫ�ر�ţ����������1����˴�ѭ��k��ֵҪһֱȡ��depth
					{
						if (number[k].num > 0)//�ڱ�����Ʒ����ȡ�˻���
						{
							node_p->schemem_node_weight += number[k].num * CharNum_To_Double(number[k].addr->ind_weight);
							node_p->schemem_node_price += number[k].num * CharNum_To_Double(number[k].addr->ind_price);
							struct scheme_node_items_list_node * list_node_p = node_p->items_list_node;
							if (list_node_p == NULL)
							{
								//���뵽��˴�����Ϊ������item�������׸�Ԫ��
								node_p->items_list_node = list_node_p = (struct scheme_node_items_list_node *)malloc(sizeof(struct scheme_node_items_list_node));
								memset(list_node_p, 0, sizeof(struct scheme_node_items_list_node));
								list_node_p->item_id = (char *)malloc(strlen(number[k].addr->item_id) + 1);
								strcpy(list_node_p->item_id, number[k].addr->item_id);
								list_node_p->item_name = (char *)malloc(strlen(number[k].addr->name) + 1);
								strcpy(list_node_p->item_name, number[k].addr->name);
								list_node_p->ind_price = CharNum_To_Double(number[k].addr->ind_price);//��Ʒ����
								//list_node_p->item_id = number[k].addr->item_id;
								//list_node_p->item_name = number[k].addr->name;
								list_node_p->item_num = number[k].num;
								list_node_p->schemem_node_id = node_p->schemem_node_id;
								list_node_p->scheme_node_items_list_node_id = 0;
								list_node_p->scheme_id = scheme_list->scheme_id;//����ڵ������ܷ������
								list_node_p->next = NULL;
							}
							else
							{
								while (list_node_p->next != NULL) { list_node_p = list_node_p->next; }//Ѱ�����һ��Ԫ��
								list_node_p->next = (struct scheme_node_items_list_node *)malloc(sizeof(struct scheme_node_items_list_node));
								list_node_p->next->scheme_node_items_list_node_id = list_node_p->scheme_node_items_list_node_id + 1;
								list_node_p = list_node_p->next;
								memset(list_node_p, 0, sizeof(struct scheme_node_items_list_node));
								list_node_p->item_id = (char *)malloc(strlen(number[k].addr->item_id) + 1);
								strcpy(list_node_p->item_id, number[k].addr->item_id);
								list_node_p->item_name = (char *)malloc(strlen(number[k].addr->name) + 1);
								strcpy(list_node_p->item_name, number[k].addr->name);
								list_node_p->ind_price = CharNum_To_Double(number[k].addr->ind_price);//��Ʒ����
								//list_node_p->item_id = number[k].addr->item_id;
								//list_node_p->item_name = number[k].addr->name;
								list_node_p->item_num = number[k].num;
								list_node_p->schemem_node_id = node_p->schemem_node_id;
								list_node_p->scheme_id = scheme_list->scheme_id;//����ڵ������ܷ������
								list_node_p->next = NULL;

							}


						}

					}

				}

				/* ���Ժ���*/

				for (int k = 0; k <= depth; k++)
				{
					printf("Stoct  %s   number are  %d \t", number[k].addr->name , number[k].num);
				}
				printf("\r\n");

			}

		}
	}
	else
	{
		for (int i = 0; i <= max; i++)//ѭ�����ã�����ÿһ����Ʒ��ȡ��������0���ܹ���������֮�����еĿ��ܣ��������һ����Ʒ
									  //���벻�ᵽ��˴������һ����Ʒ��ʵ���ϲ������ã�������ֹ���һ����Ʒ֮ǰ����ȡ����Ʒ
									  //���������ܹ����������������������һ����Ʒ���������㣬������һ����Ʒ�޷����㣬����
									  //ǰһ����Ʒ�����������Ǹ�ǰһ����Ʒ���Դ����ƣ�ֱ������
		{
			number[depth].num = i;
			test_new(scheme_list, items_list->next, max_buy, depth + 1, number , stock_kind);
			//test(number, max, depth + 1);
		}
	}

}


/*
*	���ܣ��ͷŷ������������е�ĳһ����������
*	������
*	˵������ϵͳ�з�����һ��ȫ�������������������ṹ����˶���schemeָ�벻��Ҫ�ͷţ��������ڲ��ı�ź�������Ҫ�ͷţ����Ҫ�ͷŵĽڵ�
		  ��ֵӦ�ø���Ĭ��ֵ������֪����ֵ���ٺ��ʣ�
*/
void Free_scheme(struct scheme * scheme_p)
{
	if (scheme_p != NULL)
	{

		struct scheme_node * scheme_node_p_next = scheme_p->schemes;
		struct scheme_node * scheme_node_before = scheme_p->schemes;
		while (scheme_node_p_next != NULL)
		{

			struct scheme_node_items_list_node * scheme_node_items_list_node_p_next = scheme_node_p_next->items_list_node;
			struct scheme_node_items_list_node * scheme_node_items_list_node_p_before = scheme_node_p_next->items_list_node;
			while (scheme_node_items_list_node_p_next != NULL)
			{
				scheme_node_items_list_node_p_before = scheme_node_items_list_node_p_next;
				scheme_node_items_list_node_p_next = scheme_node_items_list_node_p_next->next;
				free(scheme_node_items_list_node_p_before->item_id);//��������Ǵ�items����п��������ģ�����free
				free(scheme_node_items_list_node_p_before->item_name);//��������Ǵ�items����п��������ģ�����free
				free(scheme_node_items_list_node_p_before);
				scheme_node_items_list_node_p_before = NULL;
			}

			scheme_node_before = scheme_node_p_next;
			scheme_node_p_next = scheme_node_p_next->next;
			free(scheme_node_before);
			scheme_node_before = NULL;
		}

		free(scheme_p->scheme_id);
		free(scheme_p->scheme_name);
		memset(scheme_p, 0, sizeof(struct scheme));
		scheme_p->scheme_id = NULL;
		scheme_p->scheme_name = NULL;
		scheme_p->schemes = NULL;
	}
	//���ýڵ�ĸ���ֵ����ʼ��ֵ����δ��д
	scheme_p->Isused = 0;
}


/*
*	���ܣ��ӷ���������ɾ��ָ������������ɾ�����ݿ����������
*	������[in]ɾ���������
[in]�Ƿ���ͬɾ���÷������������ݿ��е����ݣ�0��ɾ�� 1ɾ��
*	˵����
*/
void Free_Scheme_Arrya_Node(char * scheme_id, int delete_db/*�Ƿ�ɾ���÷��������б������Ϣ*/)
{
	for (int i = 0; i < MAX_SCHEMES; i++)
	{
		if (scheme_globle[i].scheme_id != NULL && strcmp(scheme_globle[i].scheme_id, scheme_id) == 0)
		{
			Free_scheme(&scheme_globle[i]);
			if (delete_db)
			{
				Free_Scheme_DB(scheme_id);
			}
			return;
		}
	}
}