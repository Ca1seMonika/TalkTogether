## TalkTogether
写一个简单的聊天室软件来学习socket,多线程和数据库  
软件分为服务端(Server)和客户端(Client)  
# windows-server
服务端的windows版本,利用windows平台的socket接口写成,具有以下功能
```
支持至多1024人在线聊天(你也可以自己更改MAX_SIZE来更改最大在线人数)
管理员命令(ban, unban, shutdown)
保存log记录
保存聊天记录
```
**编译准备**  
1.确保你安装了cmake和mysql服务  
2.下载windows-server文件夹  
3.windows-server/Mysql中放入mysql相关的头文件  
4.在windows-server文件夹下打开powershell(或cmd)并运行命令:
```
cmake -G "Unix Makefiles" -B out -DCMAKE_BUILD_TYPE=Release ; cd out ; make
```
5.在windows-server/out/config.ini文件中填入你mysql服务的相关信息  
6.双击windows-server/out文件夹中的TalkTogether-Server.exe即可启动服务器
7.如遇到服务器窗口异常关闭可在log.txt文件夹中查看原因  
# windows-client(待完成)
```
1.第一次运行自动创建客户端的id
2.同时收发信息
```
