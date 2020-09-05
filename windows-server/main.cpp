/**
 * TalkTogether-Server
 * main.cpp
 * @author Wu Ye
 * @date 2020.9.5
 */

#include <fstream>
#include "server.h"

int main() {
    tg::Server server;

    tg::Log::Get().SetLogFilePath("\\log.txt");
    tg::Log::Get().SetNoteFilePath("\\note.txt");

    std::ifstream ifs("config.ini");
    std::string t;

    tg::SQL_Info sqlInfo;

    ifs >> t;
    sqlInfo.host = t.substr(5);

    ifs >> t;
    sqlInfo.user = t.substr(5);

    ifs >> t;
    sqlInfo.password = t.substr(9);

    ifs >> t;
    sqlInfo.port = std::atoi(t.substr(5).c_str());
    sqlInfo.database = DEFAULT_BASENAME;

    SQL.StartSQLService(sqlInfo);

    server.StartServer();

    return 0;
}
