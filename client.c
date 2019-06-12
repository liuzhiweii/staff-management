#include<stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "common.h"



void show_userinfo(MSG *msg)
{
	printf("*******************************\n");
	printf("%s\n",msg->recvmsg);
	return;

}

/**************************************
 *函数名：do_admin_query
 *参   数：消息结构体
 *功   能：管理员查询
 ****************************************/
void do_admin_query(int sockfd,MSG *msg)
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	int n;
	msg->msgtype =ADMIN_QUERY;
	while(1)
	{
		memset(&msg->info,0,sizeof(staff_info_t));
		if(msg->usertype == ADMIN)
		{
			printf("************************************************\n");
			printf("******1:按人名查找　　2:查找所有　　3:退出******\n");
			printf("************************************************\n");
			printf("请输入您的选择（数字）>>");
			scanf("%d",&n);
			getchar();
			switch(n)
			{
				case 1:
					msg->flags = 1;
					break;
				case 2:
					msg->flags=0;
					break;
				case 3:
					return;
			}


			if(msg->flags == 1)
			{
				printf("请输入您要查找的用户名:");
				scanf("%s",msg->info.name);
				getchar();
				send(sockfd,msg,sizeof(MSG),0);
				recv(sockfd,msg,sizeof(MSG),0);
				printf("工号\t用户类型\t姓名\t密码\t年龄\t电话\t地址\t职位\t入职年月\t等级\t工资\n");
				show_userinfo(msg);

			}else{
				send(sockfd,msg,sizeof(MSG),0);
				printf("工号\t用户类型\t姓名\t密码\t年龄\t电话\t地址\t职位\t入职年月\t等级\t工资\n");
				while(1)
				{
					//循环接受服务器发送过来的信息
					recv(sockfd,msg,sizeof(MSG),0);
					if(strncmp(msg->recvmsg,"over",4)==0)
						break;
					show_userinfo(msg);

				}

			}

		}

	}

}


/**************************************
 *函数名：do_admin_modification
 *参   数：消息结构体
 *功   能：管理员修改
 ****************************************/
void do_admin_modification(int sockfd,MSG *msg)
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	int n;
	msg->msgtype=ADMIN_MODIFY ;
	memset(&msg->info,0,sizeof(staff_info_t));
	send(sockfd,msg,sizeof(MSG),0);
	recv(sockfd,msg,sizeof(MSG),0);
	
	printf("请输入您要修改的工人的工号：");
	scanf("%d",&msg->info.no);
	getchar();
	printf("******************请输入要修改的选项*******************\n");
	printf("******1:姓名   　2:年龄   　3:家庭住址   　4:电话******\n");
	printf("******5:职位   　6:工资   　7:入职年月   　8:评级******\n");
	printf("******9:密码   　10:退出  　　　　　　　　　　  ******\n");
	printf("******************************************************\n");
	printf("请输入您选择的数字>>");
	scanf("%d",&n);
	getchar();
	switch(n)
	{
		case 1:
			msg->recvmsg[0]='n';
			printf("请输入您的姓名：");
			scanf("%s",msg->info.name);
			getchar();
			break;
		case 2:
			msg->recvmsg[0]='a';
			printf("请输入年龄：");
			scanf("%d",&msg->info.age);
			getchar();
			break;
		case 3:
			msg->recvmsg[0]='r';
			printf("请输入家庭住址：");
			scanf("%s",msg->info.addr);
			getchar();
			break;
		case 4:
			msg->recvmsg[0]='p';
			printf("请输入电话：");
			scanf("%s",msg->info.phone);
			getchar();
			break;
		case 5:
			msg->recvmsg[0]='w';
			printf("请输入职位：");
			scanf("%s",msg->info.work);
			getchar();
			break;
		case 6:
			msg->recvmsg[0]='s';
			printf("请输入工资：");
			scanf("%lf",&msg->info.salary);
			getchar();
			break;
		case 7:
			msg->recvmsg[0]='d';
			printf("请输入入职年月：");
			scanf("%s",msg->info.date);
			getchar();
			break;
		case 8:
			msg->recvmsg[0]='l';
			printf("请输入评级：");
			scanf("%d",&msg->info.level);
			getchar();
			break;
		case 9:
			msg->recvmsg[0]='P';
			printf("请输入密码：");
			scanf("%s",msg->info.passwd);
			getchar();
			break;
		case 10:
				msg->msgtype = QUIT;
				send(sockfd, msg, sizeof(MSG), 0);
				close(sockfd);
				exit(0);
			default:
				printf("您输入有误，请重新输入！\n");
			

	}


	send(sockfd,msg,sizeof(MSG),0);
	recv(sockfd,msg,sizeof(MSG),0);
	printf("%s\n",msg->recvmsg);




}


/**************************************
 *函数名：do_admin_adduser
 *参   数：消息结构体
 *功   能：管理员创建用户
 ****************************************/
void do_admin_adduser(int sockfd,MSG *msg)//管理员添加用户
{	
	
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	char temp;
	msg->msgtype = ADMIN_ADDUSER;
	msg->usertype = ADMIN;
	memset(&msg->info,0,sizeof(staff_info_t));
	
	while(1)
	{
	printf("*************热烈欢迎新员工**************\n");
	printf("请输入工号：");
	scanf("%d",&msg->info.no);
	printf("您输入的工号是：%d\n",msg->info.no);
	printf("工号信息一旦录入无法更改，请确认您所输入的是否正确！(Y/N)");
	scanf("%c",&temp);
	getchar();
	if(temp=='N')
		{
			printf("请重新添加用户\n");
			break;
		}	
	printf("请输入用户名：");
	scanf("%s",msg->info.name);
	getchar();
	printf("请输入用户密码：");
	scanf("%s",msg->info.passwd);
	getchar();
	printf("请输入年龄：");
	scanf("%d",&msg->info.age);
	getchar();
	printf("请输入电话：");
	scanf("%s",msg->info.phone);
	getchar();
	printf("请输入家庭住址:");
	scanf("%s",msg->info.addr);
	getchar();
	printf("请输入职位:");
	scanf("%s",msg->info.work);
	getchar();
	printf("请收入入职日期(格式：0000.00.00)：");
	scanf("%s",msg->info.date);
	getchar();
	printf("请输入评级(1~5,5为最高，新员工为 1)：");
	scanf("%d",&msg->info.level);
	getchar();
	printf("请输入工资：");
	scanf("%lf",&msg->info.salary);
	getchar();
	printf("是否为管理员：(Y/N)");
	scanf("%c",&temp);
	getchar();
	if(temp=='Y')
	msg->info.usertype=ADMIN;
	else if(temp == 'N') 
	{
		msg->info.usertype=USER;
		printf("msg->info.usertype:%d\n",msg->info.usertype);
	}
	send(sockfd,msg,sizeof(MSG),0);
	recv(sockfd,msg,sizeof(MSG),0);
	if(strncmp(msg->recvmsg,"ok",2)==0)
		printf("成功添加\n");
	else
	printf("%s",msg->recvmsg);

	printf("数据库修改成功！是否继续添加员工：（Y/N）");	
	scanf("%c",&temp);
	getchar();
	if(temp=='N')
		break;
	}

}


/**************************************
 *函数名：admin_deluser
 *参   数：消息结构体
 *功   能：管理员删除用户
 ****************************************/
void do_admin_deluser(int sockfd,MSG *msg)//管理员删除用户
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	msg->msgtype = ADMIN_DELUSER;
	memset(&msg->info,0,sizeof(staff_info_t));

	printf("请输入要删除的用户工号:");
	scanf("%d",&msg->info.no);
	getchar();

	printf("请输入要删除的用户名：");
	scanf("%s",msg->info.name);
	getchar();

	send(sockfd,msg,sizeof(MSG),0);
	recv(sockfd,msg,sizeof(MSG),0);
	if(strncmp(msg->recvmsg,"ok",2)==0)
		printf("删除用户成功\n");
	else
	printf("%s",msg->recvmsg);
	printf("数据库修改成功！已删除工号为：%d",msg->info.no);

}



/**************************************
 *函数名：do_history
 *参   数：消息结构体
 *功   能：查看历史记录
 ****************************************/
void do_admin_history (int sockfd,MSG *msg)
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	msg->msgtype =ADMIN_HISTORY;
	send(sockfd,msg,sizeof(MSG),0);
	while(1)
	{
		recv(sockfd,msg,sizeof(MSG),0);
		if(strncmp(msg->recvmsg,"over",4)==0)
			break;
		printf("msg->recvmsg:%s",msg->recvmsg);

	}
	printf("查询历史信息结束！\n");
}


/**************************************
 *函数名：admin_menu
 *参   数：套接字、消息结构体
 *功   能：管理员菜单
 ****************************************/
void admin_menu(int sockfd,MSG *msg)
{
	int n;

	while(1)
	{
		printf("*************************************************************\n");
		printf("* 1：查询  2：修改 3：添加用户  4：删除用户  5：查询历史记录*\n");
		printf("* 6：退出													*\n");
		printf("*************************************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&n);
		getchar();

		switch(n)
		{
			case 1:
				do_admin_query(sockfd,msg);
				break;
			case 2:
				do_admin_modification(sockfd,msg);
				break;
			case 3:
				do_admin_adduser(sockfd,msg);
				break;
			case 4:
				do_admin_deluser(sockfd,msg);
				break;
			case 5:
				do_admin_history(sockfd,msg);
				break;
			case 6:
				msg->msgtype = QUIT;
				send(sockfd, msg, sizeof(MSG), 0);
				close(sockfd);
				exit(0);
			default:
				printf("您输入有误，请重新输入！\n");
		}
	}
}






/**************************************
 *函数名：do_query
 *参   数：消息结构体
 *功   能：登陆
 ****************************************/
void do_user_query(int sockfd,MSG *msg)
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	msg->msgtype = USER_QUERY;
	
	printf("工号\t用户类型\t姓名\t密码\t年龄\t电话\t地址\t职位\t入职年月\t等级\t工资\n");
	send(sockfd,msg,sizeof(MSG),0);
	recv(sockfd,msg,sizeof(MSG),0);
	 show_userinfo(msg);


}



/**************************************
 *函数名：do_modification
 *参   数：消息结构体
 *功   能：普通用户修改
 ****************************************/
void do_user_modification(int sockfd,MSG *msg)
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	int n;
	msg->msgtype = USER_MODIFY;

	printf("请输入您要修改的工人的工号：");
	scanf("%d",&msg->info.no);
	getchar();
	
	printf("**********请输入您要修改的选项（其他信息请联系管理员）**********\n");
	printf("**********1：家庭住址   2：电话  3：密码  4：退出***********\n");
	printf("*************************************************************\n");

	printf("请输入您的选择（数字）>>");
	scanf("%d",&n);
	getchar();
	switch(n)
	{
		case 1:
			msg->recvmsg[0]='r';
			printf("请输入家庭住址：");
			scanf("%s",msg->info.addr);
			getchar();
			break;
		case 2:
			msg->recvmsg[0]='p';
			printf("请输入电话：");
			scanf("%s",msg->info.phone);
			getchar();
			break;
		case 3:
			msg->recvmsg[0]='P';
			printf("请输入密码：");
			scanf("%s",msg->info.passwd);
			getchar();
			break;
		case 4:
			return	;
			default:
				printf("您输入有误，请重新输入！\n");
	}

	send(sockfd,msg,sizeof(MSG),0);
	recv(sockfd,msg,sizeof(MSG),0);
	printf("%s\n",msg->recvmsg);

}


/**************************************
 *函数名：user_menu
 *参   数：消息结构体
 *功   能：管理员菜单
 ****************************************/
void user_menu(int sockfd,MSG *msg)
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	int n;
	while(1)
	{
		printf("*************************************************************\n");
		printf("*************  1：查询  	2：修改		3：退出	 *************\n");
		printf("*************************************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&n);
		getchar();

		switch(n)
		{
			case 1:
				do_user_query(sockfd,msg);
				break;
			case 2:
				do_user_modification(sockfd,msg);
				break;
			case 3:
				msg->msgtype = QUIT;
				send(sockfd, msg, sizeof(MSG), 0);
				close(sockfd);
				exit(0);
			default:
				printf("您输入有误，请输入数字\n");
				break;
		}
	}
}




int admin_or_user_login(int sockfd,MSG *msg)
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	//输入用户名和密码
	memset(msg->username, 0, NAMELEN);//清空数组
	printf("请输入用户名：");
	scanf("%s",msg->username);
	getchar();

	memset(msg->passwd, 0, DATALEN);
	printf("请输入密码（6位）");
	scanf("%s",msg->passwd);
	getchar();

	//发送登陆请求
	send(sockfd, msg, sizeof(MSG), 0);
	//接受服务器响应
	recv(sockfd, msg, sizeof(MSG), 0);
	printf("msg->recvmsg :%s\n",msg->recvmsg);

	//判断是否登陆成功
	if(strncmp(msg->recvmsg, "OK", 2) == 0)
	{
		if(msg->usertype == ADMIN)
		{
			printf("亲爱的管理员，欢迎您登陆员工管理系统！\n");
			admin_menu(sockfd,msg);
		}
		else if(msg->usertype == USER)
		{
			printf("亲爱的用户，欢迎您登陆员工管理系统！\n");
			user_menu(sockfd,msg);
		}
	}
	else
	{
		printf("登陆失败！%s\n", msg->recvmsg);
		return -1;
	}

	return 0;
}


/************************************************
 *函数名：do_login
 *参   数：套接字、消息结构体
 *返回值：是否登陆成功
 *功   能：登陆
 *************************************************/
int do_login(int sockfd)
{	
	int n;
	MSG msg;

	while(1){
		printf("*************************************************************\n");
		printf("********  1：管理员模式    2：普通用户模式    3：退出********\n");
		printf("*************************************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&n);
		getchar();

		switch(n)
		{
			case 1:
				msg.msgtype  = ADMIN_LOGIN;
				msg.usertype = ADMIN;
				break;
			case 2:
				msg.msgtype =  USER_LOGIN;
				msg.usertype = USER;
				break;
			case 3:
				msg.msgtype = QUIT;
				if(send(sockfd, &msg, sizeof(MSG), 0)<0)
				{
					perror("do_login send");
					return -1;
				}
				close(sockfd);
				exit(0);
			default:
				printf("您的输入有误，请重新输入\n"); 
		}

		admin_or_user_login(sockfd,&msg);
	}

}


int main(int argc, const char *argv[])
{
	//socket->填充->绑定->监听->等待连接->数据交互->关闭 
	int sockfd;
	int acceptfd;
	ssize_t recvbytes,sendbytes;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	socklen_t cli_len = sizeof(clientaddr);

	//创建网络通信的套接字
	sockfd = socket(AF_INET,SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket failed.\n");
		exit(-1);
	}
	printf("sockfd :%d.\n",sockfd); 

	//填充网络结构体
	memset(&serveraddr,0,sizeof(serveraddr));//清空数组
	memset(&clientaddr,0,sizeof(clientaddr));
	serveraddr.sin_family = AF_INET;
//	serveraddr.sin_port   = htons(atoi(argv[2]));
//	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port   = htons(5001);
	serveraddr.sin_addr.s_addr = inet_addr("192.168.1.200");

	if(connect(sockfd,(const struct sockaddr *)&serveraddr,addrlen) == -1){
		perror("connect failed.\n");
		exit(-1);
	}

	do_login(sockfd);

	close(sockfd);

	return 0;
}



