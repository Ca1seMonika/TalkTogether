
#include "client.h"

namespace tg {
    Client::Client() {
        srand(time(NULL));
        ifs.open("identify");
        if(!ifs){
            std::string id = GetSelfId();
            fs.open("identify", std::ios::trunc | std::ios::out);
            fs << id;
            fs.close();
            this->id = id;
        }else{
            ifs >> this->id;
        }
        ifs.close();
        ifs.open("config.ini");
        ifs >> this->host;
        ifs.close();
        this->host = this->host.substr(12);
    }

    void Client::StartClient(SOCKET sockClient) {
        SOCKADDR_IN addrClient = {0};
        addrClient.sin_addr.S_un.S_addr = inet_addr(host.c_str());
        addrClient.sin_family = AF_INET;
        addrClient.sin_port = htons();//input server's port

        std::cout << "type your name: ";
        std::string name;

        while (std::cin >> name && name.size() > 21){
            std::cout << "Name is too long, type again: ";
        }

        if(connect(sockClient, (sockaddr*)&addrClient, sizeof(addrClient)) < 0){
            std::cerr << "Connect server failed" << std::endl;
            return;
        }

        send(sockClient, id.c_str(), id.size(), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        send(sockClient, name.c_str(), name.size(), 0);

        std::thread taskSend([this, sockClient]{StartSendMsg(sockClient);});
        taskSend.detach();
        std::thread taskRecv([this, sockClient]{StartRecvMsg(sockClient);});
        taskRecv.join();


    }

    std::string Client::GetSelfId(){
        char id[64] = {0};
        char tmp[64] = {0};
        time_t tim = time(NULL);
        strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", localtime(&tim));
        int l = strlen(id);
        strcat(tmp, "%lld");
        sprintf(id, tmp, rand());
        return std::string(id).substr(strlen(id) - 12);
    }

    void Client::StartRecvMsg(SOCKET sockClient) const {
        char buf[BUFSIZE] = {0};
        while (recv(sockClient, buf, BUFSIZE, 0) > 0){
            std::cout << buf << std::endl;
            memset(buf, 0, sizeof(buf));
        }
    }

    void Client::StartSendMsg(SOCKET sockClient) const {
        char buf[BUFSIZE] = {0};
        while (true){
            std::cin >> buf;
            send(sockClient, buf, strlen(buf), 0);
            memset(buf, 0, sizeof(buf));
        }
    }
}
