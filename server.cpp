#include "server.h"

namespace tg {
    void Server::StartServer() {
        WSADATA wsa;
        WORD wVersion = MAKEWORD(2, 2);
        if(WSAStartup(wVersion, &wsa) != 0){
            LOG("Failed to boot server: socket library initialization failed");
            exit(-1);
        }

        SOCKET sockServ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sockServ == INVALID_SOCKET){
            LOG("Failed to boot server: socket initialization failed");
            exit(-1);
        }

        SOCKADDR_IN addrServ = {0};
        addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        addrServ.sin_family = AF_INET;
        addrServ.sin_port = htons(5555);

        bind(sockServ, (sockaddr*)&addrServ, sizeof(addrServ));

        listen(sockServ, 25);

        ListenForConnect(sockServ);
    }

    void Server::ListenForConnect(SOCKET& sockServ) {
        LOG("Booted server");

        SOCKADDR_IN addrClient = {0};
        SOCKET tClient;
        int len = sizeof(addrClient);

        while (true){
            tClient = accept(sockServ, (sockaddr*)&addrClient, &len);
            if(tClient == SOCKET_ERROR){
                continue;
            }
            std::thread com([this, tClient]{
                Communication(tClient);
            });
            com.detach();
        }
    }

    void Server::Communication(SOCKET tClient) {
        char name[30] = {0};
        char id[30] = {0};
        recv(tClient, id, 30, 0);
        recv(tClient, name, 30, 0);
        send(tClient, "You have connected server, welcome to TalkTogether!", 64, 0);

        std::list<SOCKET>::iterator it;
        mux.lock();
        if(LoginList.size() < 1024) {
            LoginList.push_back(tClient);
            it = --LoginList.end();
        }else{
            mux.unlock();
            send(tClient, "Login full", 11, 0);
            return;
        }
        mux.unlock();
        std::cout << WHEN << WHO(name) << WHO(id) << "Connected\n";
        NOTE(id, name, "Connected");

        char buf[1024] = {0};
        while (true){
            if(recv(tClient, buf, 1024, 0) <= 0){
                std::cout << WHEN << WHO(name) << WHO(id) << "Disconnected\n";
                NOTE(id, name, "Disconnected");
                break;
            }
            std::cout << WHEN << WHO(name) << WHO(id) << buf << std::endl;

            char tmp[1100] = {0}; int len;
            sprintf(tmp, "[%s][%s] %s", tg::Log::GetNowTime().c_str(), name, buf);
            len = strlen(tmp);

            mux.lock();
            for(auto& ch: LoginList){
                send(ch, tmp, len + 1, 0);
            }
            mux.unlock();
        }

        mux.lock();
        LoginList.erase(it);
        mux.unlock();
    }
}
