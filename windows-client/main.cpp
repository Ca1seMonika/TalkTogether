#include "client.h"

int main() {
    system("chcp 936 & cls");
    WORD wVersion = MAKEWORD(2, 2);
    WSADATA wsa;
    tg::Client client;
    if(WSAStartup(wVersion, &wsa) != 0){
        std::cerr << "Initialize WSADATA failed" << std::endl;
        return -1;
    }

    SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);
    client.StartClient(socketClient);

    closesocket(socketClient);
    WSACleanup();

    system("pause");
    return 0;
}