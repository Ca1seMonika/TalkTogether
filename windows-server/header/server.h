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

        void StartServer();
    private:

        int FindLogin(const char* id, const char* name);

        void ListenForConnect(SOCKET& sockServ);

        void Communication(SOCKET tClient);

        void broadcast(const char* content);

        void ban(const char* cid);

        void unban(const char* cid);

        void shutDown(int waitSec);

        void administrator();

        std::list<tg::LoginInfo> LoginList;

        std::mutex listMutex;
    };
}

#endif
