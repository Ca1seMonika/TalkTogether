#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <winsock2.h>
#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdlib>

#define BUFSIZE 2048

namespace tg {
    class Client {
    public:
        Client();
        ~Client() = default;

        void StartClient(SOCKET sockClient);

    private:
        void StartSendMsg(SOCKET sockClient) const;
        void StartRecvMsg(SOCKET sockClient) const;
        static std::string GetSelfId();

        std::string id;
        std::string host;
        std::fstream fs;
        std::ifstream ifs;
    };
}

#endif
