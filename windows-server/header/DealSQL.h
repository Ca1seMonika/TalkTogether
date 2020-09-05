#ifndef _SERVER_DEALSQL_H_
#define _SERVER_DEALSQL_H_

#include <string>
#include <fstream>
#include <mutex>
#include <mysql.h>
#include "log.h"

#define DEFAULT_BASENAME "talktogether_server"
#define FORMAT_SIZE 1024
#define SQL tg::DealSQL::Get()

namespace tg {
    struct SQL_Info {
        std::string host;
        std::string user;
        std::string password;
        std::string database;
        unsigned int port;
    };

    class DealSQL {
    public:
        ~DealSQL() = default;

        static DealSQL& Get();

        void StartSQLService(SQL_Info info);

        void Lock();

        void UnLock();

        int InsertData(const char* table, const char* fields, const char* values);

        int DeleteDate(const char* table, const char* fields, const char* values);

        int QueryValueRows(const char* table, const char* fields, const char* values);

        int CleanUpTable(const char* table);
    private:
        DealSQL() = default;
        int SQLConnect();

        SQL_Info sqlInfo;
        MYSQL mysql;
        MYSQL_RES* sqlRes;
        std::mutex mux;
    };

}
#endif
