//#pragma once 
#include"head_file.h"

//函数功能：把当前程序变为守护进程
void serversd()
{
	//父进程fork子进程, 然后退出
	pid_t pid = fork();
	if (pid > 0 || pid < 0)
	{
		//perror("fork()");
		exit(0);
	}

	//子进程调用setsid函数创建会话
	setsid();

	//修改当前的工作目录
	chdir("./");

	//重设文件掩码
	umask(0000);

	//关闭文件描述符
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	//close(STDERR_FILENO);
}

//函数功能：获取务器异常、崩溃状态
void servers_status()
{
	int wstatus = 0;
	//pid_t error_ppid = wait(&wstatus);
	wait(&wstatus);
	if (WIFSIGNALED(wstatus))
	{
		char error_buf[60] = {0};
		sprintf(error_buf,"servers_error_voer:SIG=[%d]:RETURN=[%d]:",
			WTERMSIG(wstatus), WEXITSTATUS(wstatus));
		go_error_log(error_buf);
	}

	if (WIFEXITED(wstatus))
	{
		char error_buf[60] = { 0 };
		sprintf(error_buf, "servers_normal_voer:SIG=[%d]:RETURN=[%d]:",
			WTERMSIG(wstatus), WEXITSTATUS(wstatus));
		go_error_log(error_buf);
	}
}



int main(int argc, char** argv)
{
	
	//守护进程
#ifdef DEBUG
	serversd();
#endif // DEBUG


	pid_t pid = 0;
	
//服务器终止后重启服务器
servers_reboot:

	pid = fork();
	//父进程获取服务器崩溃、退出情况
	if (pid>0)
	{
		servers_status();
		sleep(10);
		goto servers_reboot;
	}
	//子进程  服务器
	else if(pid ==0)
	{
		select_servers();
	}
	else
	{
		go_error_log("main()->fork():");
	}
	
}
