#include"app_one.h"

//函数功能:获得一爻
//参数1：传入49
//参数2：传入0，控制递归
//返回值：24、28、32、36
//24表示太阴，32表示少阴
//28表示少阳，36表示太阳
int one_yao(int nn, int* p)
{
	// Sleep(rand()%800);
	//数50用49 
	int n = nn;
	//分而为二
	int nt = (rand() % (n - 1) + 1);//随机数为1-48
	int nd = n - nt;

	//挂一象三
	if (rand() % 2 == 0)
	{
		nt--;
	}
	else
	{
		nd--;
	}

	//揲之以四 以象四时
	//除以4产生的余数不要，如果余数为0减4
	int tmpnt = nt;
	int tmpnd = nd;
	nt = nt / 4 * 4;
	nd = nd / 4 * 4;
	if (nt == tmpnt && nt != 0)
	{
		nt -= 4;
	}
	if (nd == tmpnd && nd != 0)
	{
		nd -= 4;
	}

	n = nt + nd;

	// 3次得一爻
	*p += 1;
	if (*p == 3)
	{
		return n;
	}

	//归奇于扐以象闰
	one_yao(n, p);
}

//函数功能：获取一挂
//函数参数1：传入参数，保存卦的信息
void one_gua(char buf[6])
{

	//循环调用one_yao得到6个爻
	for (int i = 0; i < 6; i++)
	{
		//a用于控制递归
		int a = 0;
		//固定n传49即可
		int n = 50;
		n = n - 1;
		n = one_yao(n, &a);
		//除以4给得6、7、8、9
		n = n / 4;

		//现在n等于一个爻
		//把这个爻存储到buf里面
		//buf[0]是第一个爻、初爻
		//6为太阴，8为少阴，7为少阳，9为太阳。
		if (n == 6)
		{
			/*memset(&buf[i][0], '0', 1);
			memset(&buf[i][1], '1', 1);*/
			memset(&buf[i], '6', 1);
		}
		else if (n == 7)
		{
			/*memset(&buf[i][0], '0', 1);
			memset(&buf[i][1], '0', 1);*/
			memset(&buf[i], '7', 1);
		}
		else if (n == 8)
		{
			//memset(&buf[i][0], '1', 1);
			//memset(&buf[i][1], '0', 1);
			memset(&buf[i], '8', 1);
		}
		else if (n == 9)
		{
			/*memset(&buf[i][0], '1', 1);
			memset(&buf[i][1], '1', 1);*/
			memset(&buf[i], '9', 1);
		}
	}
}

//函数功能：从数据库获取一挂信息
//函数参数1：传入挂的ID
//函数参数2:传入堆区空间，保存查询结果
//成功返回:保存有挂信息的字符串。不要管成功返回值，代码改过，成功返回值已经废弃
//失败返回：NULL
char* getGuaInfo(int gua_id,char* my_buf)
{
	int ret = -1;
	//int i, j;
	//数据库操作
	MYSQL *mysql = NULL;
	//结果集
	MYSQL_RES *res = NULL;
	//一行结果
	MYSQL_ROW row = NULL;
	//结果集中的行数
	unsigned long row_nums = 0;
	//结果集中的列数
	unsigned int col_nums = 0;


	char buf[APPONESIZE] = { 0 };

	//初始化
	mysql = mysql_init(NULL);
	if (NULL == mysql)
	{
		//perror("mysql_init()");
		return NULL;
	}
	//连接mysql数据库
	mysql = mysql_real_connect(mysql, __APPONEHOTST__, __APPONEUSER__,
		__APPONEPASSWD__, __APPONEDB__, 0, NULL, 0);
	if (NULL == mysql)
	{
		//perror("mysql_real_connect()");
		return NULL;
	}

	mysql_query(mysql, "set names utf8");

	//执行查询语句
	sprintf(buf,
		"select gua_name,gua_message from gua_64 where gua_id = %d", gua_id);
	ret = mysql_query(mysql, buf);
	if (0 != ret)
	{
		//printf("mysql_query()");
		return NULL;
	}

	//获取结果集
	res = mysql_store_result(mysql);
	if (NULL == res)
	{
		//perror("mysql_store_result()");
		return NULL;
	}

	//获取结果集合中的行数
	row_nums = mysql_num_rows(res);
	//获取结果集中的列数
	col_nums = mysql_num_fields(res);

	for (int i = 0; i < row_nums; i++)
	{
		row = mysql_fetch_row(res);
		if (NULL == row)
		{
			//perror("mysql_fetch_row()");
			return NULL;

		}
		for (int j = 0; j < col_nums; j++)
		{
			strcpy(my_buf + ((i + j) * 150), row[j]);
			//printf("[%s]", my_buf + (j * 100));
		}
		//printf("\n");
	}

	sprintf(buf,
		"select yao_message from yao_386 where gua_id = %d; ", gua_id);
	ret = mysql_query(mysql, buf);
	if (0 != ret)
	{
		//printf("mysql_query()");
		return NULL;
	}

	//获取结果集
	res = mysql_store_result(mysql);
	if (NULL == res)
	{
		//perror("mysql_store_result()");
		return NULL;
	}

	//获取结果集合中的行数
	row_nums = mysql_num_rows(res);
	//获取结果集中的列数
	col_nums = mysql_num_fields(res);

	for (int i = 0; i < row_nums; i++)
	{
		row = mysql_fetch_row(res);
		if (NULL == row)
		{
			//perror("mysql_fetch_row()");
			return NULL;

		}
		for (int j = 0; j < col_nums; j++)
		{
			strcpy((my_buf + ((i + j + 2) * 150)), row[j]);
			//printf("[%s]", my_buf + ((j + 2) * 100));
			//printf("%d\n", j);
		}
		//printf("\n");
	}

	return my_buf;
}

//函数功能：根据生产的卦ID，获取指定的卦信息
//成功返回：保存有挂信息的堆区数据
//失败返回：NULL
char* get_one_gua()
{
	srand(time(NULL));

	//存储查询到的结果
	char *my_buf = malloc(APP_ONE_RETURN_BUF_SIZE);
	memset(my_buf, 0, APP_ONE_RETURN_BUF_SIZE);

	int gua_id = 0;
	//按顺序存储384爻减少查询数据量、查询次数
	char buf11[64][7] = { {'1','1','1','1','1','1'},
	{'0','0','0','0','0','0'},{'1','0','0','0','1','0'},
	{'0','1','0','0','0','1'},{'1','1','1','0','1','0'},
	{'0','1','0','1','1','1'},{'0','1','0','0','0','0'},
	{'0','0','0','0','1','0'},{'1','1','1','0','1','1'},
	{'1','1','0','1','1','1'},{'1','1','1','0','0','0'},
	{'0','0','0','1','1','1'},{'1','0','1','1','1','1'},
	{'1','1','1','1','0','1'},{'0','0','1','0','0','0'},
	{'0','0','0','1','0','0'},{'1','0','0','1','1','0'},
	{'0','1','1','0','0','1'},{'1','1','0','0','0','0'},
	{'0','0','0','0','1','1'},{'1','0','0','1','0','1'},
	{'1','0','1','0','0','1'},{'0','0','0','0','0','1'},
	{'1','0','0','0','0','0'},{'1','0','0','1','1','1'},
	{'1','1','1','0','0','1'},{'1','0','0','0','0','1'},
	{'0','1','1','1','1','0'},{'0','1','0','0','1','0'},
	{'1','0','1','1','0','1'},{'0','0','1','1','1','0'},
	{'0','1','1','1','0','0'},{'0','0','1','1','1','1'},
	{'1','1','1','1','0','0'},{'0','0','0','1','0','1'},
	{'1','0','1','0','0','0'},{'1','0','1','0','1','1'},
	{'1','1','0','1','0','1'},{'0','0','1','0','1','0'},
	{'0','1','0','1','0','0'},{'1','1','0','0','0','1'},
	{'1','0','0','0','1','1'},{'1','1','1','1','1','0'},
	{'0','1','1','1','1','1'},{'0','0','0','1','1','0'},
	{'0','1','1','0','0','0'},{'0','1','0','1','1','0'},
	{'0','1','1','0','1','0'},{'1','0','1','1','1','0'},
	{'0','1','1','1','0','1'},{'1','0','0','1','0','0'},
	{'0','0','1','0','0','1'},{'0','0','1','0','1','1'},
	{'1','1','0','1','0','0'},{'1','0','1','1','0','0'},
	{'0','0','1','1','0','1'},{'0','1','1','0','1','1'},
	{'1','1','0','1','1','0'},{'0','1','0','0','1','1'},
	{'1','1','0','0','1','0'},{'1','1','0','0','1','1'},
	{'0','0','1','1','0','0'},{'1','0','1','0','1','0'},
	{'0','1','0','1','0','1'} };
	//传入one_gua获取一个卦
	char buf[6] = { 0 };
	one_gua(buf);

	//循环和64卦比较找到卦id
	for (int i = 0; i < 64; i++)
	{

		//   1        0       1        0 
		//6为太阴，8为少阴，7为少阳，9为太阳。
		//6、8为阴爻
		//具体和某一个卦比较
		for (int j = 0; j < 6; j++)
		{
			//变卦中6、8为阴爻
			char tmp_buf = '0';
			//变卦中7、9为阳爻
			if (buf[j] == '7' || buf[j] == '9')
			{
				tmp_buf = '1';
			}
			if (tmp_buf == buf11[i][j])
			{
				if (j == 5)
				{
					gua_id = i + 1;
					char* buf1 = getGuaInfo(gua_id,my_buf);
					//存储是否变爻的信息
					memcpy(buf1+50,buf,6);
					memset(buf1+56,0,5);
					//printf("[%s]\n",buf);
					//printf("[%s]\n", buf+100);
					//printf("[%s]\n", (buf + 200));
					//printf("[%s]\n", (buf + 300));
					//printf("[%s]\n", buf + 400);
					//printf("[%s]\n", buf + 500);
					//printf("[%s]\n", buf + 600);
					//printf("[%s]\n", buf + 700);
				}
			}
			else
			{
				//一爻不对立马退出
				//goto dzz;
				break;
			}
		}
	}

	for (int i = 0; i < 64; i++)
	{
		//具体和某一个卦比较
		for (int j = 0; j < 6; j++)
		{
			//   1        0       1        0 
			//6为太阴，8为少阴，7为少阳，9为太阳。
			//6、8为阴爻
			char tmp_buf = '0';
			//6、7为阳爻
			if (buf[j] == '7' || buf[j] == '6')
			{
				tmp_buf = '1';
			}
			if (tmp_buf == buf11[i][j])
			{
				if (j == 5)
				{
					gua_id = i + 1;
					char* buf1 = getGuaInfo(gua_id, (my_buf+1200));
					//memcpy(buf1 + 50, buf, 6);
					//memset(buf1 + 56, 0, 5);
					return my_buf;
					//printf("[%s]\n",buf);
					//printf("[%s]\n", buf+100);
					//printf("[%s]\n", (buf + 200));
					//printf("[%s]\n", (buf + 300));
					//printf("[%s]\n", buf + 400);
					//printf("[%s]\n", buf + 500);
					//printf("[%s]\n", buf + 600);
					//printf("[%s]\n", buf + 700);
				}
			}
			else
			{
				//一爻不对立马退出
				break;
			}
		}
	}

dzz:
	return NULL;
}

