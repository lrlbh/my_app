#include"servers.h"

//函数功能：创建基TCP select 的服务器
void select_servers()
{
	int s_sock, c_sock;
	struct sockaddr_in s_addr, c_addr;
	fd_set reads, cpy_reads;
	char buf[BUF_SIZE];
	int fd_max, str_len, fd_num;
	socklen_t adr_sz;

	s_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (s_sock == -1)
	{
		go_error_log("select_servers()->socket():");
		return;
		
	}

	// memset(&s_addr, 0, sizeof(s_addr));
	// s_addr.sin_family = AF_INET;
	// s_addr.sin_port = htons(atoi(SOCK_PORT));
	// //inet_aton(SOCK_IP, (struct in_addr*)&s_addr.sin_addr);
	// s_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//获取IP和端口
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(SOCK_PORT));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//给套接字分配ip和端口
	if (bind(s_sock,
		(struct sockaddr*)&s_addr, sizeof(s_addr)) == -1)
	{
		go_error_log("select()->bind():");
		return;
	}

	if (listen(s_sock, 100) == -1)
	{
		go_error_log("select_servers()->listen():");
		return;
	}

	FD_ZERO(&reads);
	FD_SET(s_sock, &reads);
	fd_max = s_sock;

	while (1)
	{
		cpy_reads = reads;

		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, NULL)) == -1)
		{
			go_error_log("select_servers()->select():");
			return;
		}

		if (FD_ISSET(s_sock, &cpy_reads))
		{
			memset(&c_addr,0,sizeof(c_addr));
			adr_sz = sizeof(c_addr);
			c_sock = accept(s_sock, (struct sockaddr*)&c_addr, &adr_sz);

			FD_SET(c_sock, &reads);
			if (fd_max < c_sock)
				fd_max = c_sock;

			//打印连接日志
			go_ip_log((inet_ntoa((struct in_addr)c_addr.sin_addr)));

			if (--fd_num <= 0)
				continue;
		}

		for (int i = 0; i < fd_max + 1; i++)
		{

			if (s_sock == i)
				continue;

			if (FD_ISSET(i, &cpy_reads))
			{
				//核心代码执行位置
				read(i, buf, BUF_SIZE);
				//第一个字节用于获取请求类型
				if (memcmp(buf, "1", 1)==0)
				{
					char * gua_buf = get_one_gua();
					if (gua_buf != NULL)
					{
						//go_error_log(gua_buf);
						write(i, gua_buf, 2400);
						free(gua_buf);
						goto c_close;
					}
					else
					{
						go_error_log("select_servers()->get_one_gua():");
						goto c_close;
					}

				}

				if (memcmp(buf, "2", 1)==0)
				{

				}

				if (memcmp(buf, "3", 1)==0)
				{

				}

				if (0)
				{
				c_close:
					FD_CLR(i, &reads);
					close(i);
				}
			}
		}
	}


}