#ifndef _SERVER_SERVER_H_
#define _SERVER_SERVER_H_

#include <thread>
#include <mutex>
#include <winsock2.h>
#include <iostream>
#include <string>
#include <list>
#include "log.h"

#define MAX_CONNECT 1024

namespace tg {
    class Server {
    public:
        Server() = default;
        ~Server() = default;

        void StartServer();
    private:

        void ListenForConnect(SOCKET& sockServ);

        void Communication(SOCKET tClient);

        std::list<SOCKET> LoginList;
        std::mutex mux;
    };
}

#endif
