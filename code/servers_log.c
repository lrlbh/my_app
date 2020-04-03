#include"servers_log.h"

//保存当前系统时间
char time_buf[20] = {0};

//int cip = 0;

//打开文件
FILE* get_fopen(const char *buf)
{
	FILE * fp = fopen(buf,"a+");
	if (fp ==NULL)
		return NULL;
	else
		return fp;
}

//获取当前系统时间
char* get_time()
{
	memset(time_buf,0,sizeof(time_buf));
	time_t timep;
	struct tm *p;

	time(&timep);
	p = gmtime(&timep);

	sprintf(time_buf,"%d-%d-%d-%d-%d-%d:",
		1900 + p->tm_year, 1 + p->tm_mon,
		p->tm_mday, 8 + p->tm_hour,
		p->tm_min, p->tm_sec);
	//printf("%s\n", buf);
	return time_buf;
	
}

//打印错误日志
//参数1：自定义打印的内容
void go_error_log(const char * my_error)
{
	FILE* fp_error = get_fopen("./error.log");
	if (fp_error==NULL)
		return;

	if (fprintf(fp_error, "%s%s strerror(errno)=[%s]\n",
		get_time(), my_error, strerror(errno)) == -1)
		return;

	fclose(fp_error);

	return;
}


//函数功能：记录连接过服务器的不重复ip
//函数参数1：传入客服端ip地址
void new_ip_log(const char * buf)
{
	char  new_ip_buf[50] = { 0 };
	FILE* fp_new_ip = get_fopen("./new_ip.log");
	if (fp_new_ip == NULL)
	{
		go_error_log("new_ip_long()->get_fopen():");
		return ;
	}

	while (!feof(fp_new_ip))
	{
		memset(new_ip_buf,0x0,sizeof(new_ip_buf));
		fgets(new_ip_buf,(sizeof(new_ip_buf)-1), fp_new_ip);
		//if (!(strstr(new_ip_buf, buf)))
		if (strstr(new_ip_buf, buf)!=NULL)
			goto dzz;
	}

	if (fprintf(fp_new_ip, "%s newIP[%s]\n",
		get_time(), buf) == -1)
	{
		go_error_log("new_ip_long()->fprintf():");
		fclose(fp_new_ip);
		return;
	}
		

dzz:
	fclose(fp_new_ip);
}

//函数功能：记录最近连接记录
//函数参数1：传入客服端ip地址
void connect_ip_log(const char * buf)
{
	char * connect_ip_buf[50] = { 0 };
	FILE* fp_connect_ip = get_fopen("./connect_ip.log");
	if (fp_connect_ip == NULL)
	{
		go_error_log("connect_ip_log()->get_fopen()");
		return;
	}
		

	if (fprintf(fp_connect_ip, "%s connectIP[%s]\n",
		get_time(), buf) == -1)
	{
		go_error_log("connect_ip_log()->fprintf()");
		fclose(fp_connect_ip);
		return;
	}

	fclose(fp_connect_ip);
	return;
}

//函数功能：记录最近连接记录和不重复的连接IP
//函数参数1：传入客服端ip地址
void go_ip_log(const char * buf)
{
	new_ip_log(buf);
	connect_ip_log(buf);
	return;
}

//函数功能：打印错误
//函数参数1：自定义打印内容
void my_preeor(char * error)
{
#ifndef DEBUG
	perror(error);
#endif // !1

}
