#include "server.h"
#include "log.h"

int main() {
    system("chcp 936");
    tg::Server server;

    tg::Log::Get().SetLogFilePath("\\log.txt");
    tg::Log::Get().SetNoteFilePath("\\note.txt");

    server.StartServer();
}
