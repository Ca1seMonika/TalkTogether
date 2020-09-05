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

    int Server::FindLogin(const char *id, const char *name) {
        const char* who = "id,name";
        char qwho[256] = {0};
        int res = 0;
        sprintf(qwho, "'%s','%s'", id, name);

        SQL.Lock();
        if(SQL.QueryValueRows("client_info", "id", id)){
            res = 1;
        }else if(SQL.QueryValueRows("client_info", "name", name)){
            res = 2;
        }else if(SQL.QueryValueRows("banned_id", "id", id)){
            res = 3;
        }else{
            SQL.InsertData("client_info", who, qwho);
            res = 0;
        }
        SQL.UnLock();
        return res;
    }

    void Server::ListenForConnect(SOCKET& sockServ) {
        LOG("Booted server");
        std::cout << "TalkTogether Server is booted, edited by Wu Ye.\n";
        std::thread admin([this]{administrator();});
        admin.detach();

        SOCKADDR_IN addrClient = {0};
        SOCKET tClient;
        int len = sizeof(addrClient);

        while (true){
            tClient = accept(sockServ, (sockaddr*)&addrClient, &len);
            if(tClient == SOCKET_ERROR){
                continue;
            }
            int nNetTimeout = DEFAULT_WAITSEC * 1000;
            setsockopt(tClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
            std::thread com([this, tClient]{Communication(tClient);});
            com.detach();
        }
    }

    void Server::Communication(SOCKET tClient) {
        char name[64] = {0};
        char id[64] = {0};
        recv(tClient, id, BUF_SIZE, 0);
        recv(tClient, name, BUF_SIZE, 0);

        int res = FindLogin(id, name);
        if(res != 0){
            const char* buf[3] = {"Don't login again", "Existed name", "You has been banned"};
            send(tClient, buf[res - 1], BUF_SIZE, 0);
            closesocket(tClient);
            return;
        }

        send(tClient, "You have connected server, welcome to TalkTogether!", BUF_SIZE, 0);

        std::list<tg::LoginInfo>::iterator it;
        listMutex.lock();
        if(LoginList.size() < MAX_SIZE) {
            LoginList.emplace_back(tClient, id, name);
            it = --LoginList.end();
        }else{
            listMutex.unlock();
            send(tClient, "Login full", BUF_SIZE, 0);
            closesocket(tClient);
            return;
        }
        listMutex.unlock();

        std::cout << WHEN << WHO(name) << WHO(id) << "Connected\n";
        NOTE(id, name, "Connected");

        char buf[BUF_SIZE * 16] = {0};
        while (true){
            memset(buf, 0, sizeof(buf));
            if(recv(tClient, buf, BUF_SIZE * 16, 0) <= 0){
                std::cout << WHEN << WHO(name) << WHO(id) << "Disconnected\n";
                NOTE(id, name, "Disconnected");
                SQL.Lock();
                SQL.DeleteDate("client_info", "name", name);
                SQL.UnLock();
                break;
            }
            std::cout << WHEN << WHO(name) << WHO(id) << buf << std::endl;
            NOTE(id, name, buf);

            char tmp[BUF_SIZE * 17] = {0};
            sprintf(tmp, "[%s][%s] %s", tg::Log::GetNowTime().c_str(), name, buf);
            broadcast(tmp);
        }

        listMutex.lock();
        LoginList.erase(it);
        listMutex.unlock();

        closesocket(tClient);
    }

    void Server::broadcast(const char *buf) {
        int len = strlen(buf);
        listMutex.lock();
        for(auto& ch: LoginList){
            send(ch.socketId, buf, len, 0);
        }
        listMutex.unlock();
    }

    void Server::ban(const char *cid) {
        listMutex.lock();
        auto it = LoginList.begin();
        for(; it != LoginList.end(); it++){
            if(strcmp((*it).clientId.c_str(), cid) == 0){
                break;
            }
        }
        if(it != LoginList.end()){
            listMutex.unlock();
            broadcast(std::string((*it).name + "has been banned").c_str());
            listMutex.lock();
            closesocket((*it).socketId);
            listMutex.unlock();
        }

        char t[64] = {0};
        sprintf(t, "'%s'", cid);
        SQL.Lock();
        if(!SQL.QueryValueRows("banned_id", "id", cid)){
            SQL.InsertData("banned_id", "id", t);
        }
        SQL.UnLock();
    }

    void Server::unban(const char *cid) {
        SQL.Lock();
        SQL.DeleteDate("banned_id", "id", cid);
        SQL.UnLock();
    }

    void Server::shutDown(int waitSec) {
        char buf[BUF_SIZE] = {0};
        sprintf(buf, "Server will be shut in %d seconds", waitSec);
        broadcast(buf);

        std::this_thread::sleep_for(std::chrono::seconds(waitSec));

        listMutex.lock();
        for(auto& ch: LoginList){
            closesocket(ch.socketId);
        }
        listMutex.unlock();
        WSACleanup();

        LOG("admin shut down the server");
        exit(0);
    }

    void Server::administrator() {
        char command[BUF_SIZE * 16] = {0};
        while (true){
            std::cin >> command;
            if(command[0] == '-'){
                if(strcmp(command + 1, "shutdown") == 0){
                    int waitSec;
                    std::cin >> waitSec;
                    shutDown(waitSec);
                }else if(strcmp(command + 1, "ban") == 0){
                    char cid[64] = {0};
                    std::cin >> cid;
                    ban(cid);
                }else if(strcmp(command + 1, "unban") == 0){
                    char cid[64] = {0};
                    std::cin >> cid;
                    unban(cid);
                }else{
                    std::cout << "Error command" << std::endl;
                }
            }else{
                char buf[BUF_SIZE * 17] = {0};
                sprintf(buf, "[%s](administrator) %s", tg::Log::GetNowTime().c_str(), command);
                broadcast(buf);
            }
        }
    }
}

