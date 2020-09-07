/**
 * TalkTogether-Server
 * server.h
 * @author Wu Ye
 * @date 2020.9.5
 */

#ifndef _SERVER_SERVER_H_
#define _SERVER_SERVER_H_

#include <thread>
#include <winsock2.h>
#include <string>
#include <list>
#include <chrono>
#include "log.h"
#include "DealSQL.h"

#define BUF_SIZE 64
#define MAX_SIZE 1024
#define DEFAULT_WAITSEC 300

namespace tg {
    //登录人员信息
    struct LoginInfo {
        LoginInfo(SOCKET sid,
                  std::string cid,
                  std::string name):
                  socketId(sid), clientId(std::move(cid)), name(std::move(name)) {}
        SOCKET socketId;
        std::string clientId;
        std::string name;
    };

    class Server {
    public:
        Server() = default;
        ~Server() = default;

        //启动服务
        void StartServer();
    private:
        //查询请求连接的客户端是否已经登录或被ban
        int VerifiedLogin(SOCKET sockClient, const char* id, const char* name) const;
        //监听连接的客户端
        void ListenForConnect(SOCKET& sockServ);
        //接受客户端的id和称呢,接受聊天信息并广播
        void Communication(SOCKET tClient);
        //将传入的信息广播
        void broadcast(const char* content);
        //封禁
        void ban(const char* cid);
        //解封
        void unban(const char* cid);
        //关闭服务器
        void shutDown(int waitSec);
        //单独使用一个线程,管理服务器
        void administrator();

        std::list<tg::LoginInfo> LoginList;
        std::mutex listMutex;//管理list
    };
}

#endif
