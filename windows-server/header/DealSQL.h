/**
 * TalkTogether-Server
 * DealSQL.h
 * @author Wu Ye
 * @date 2020.9.5
 */

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
        //Meyers' Singleton
        static DealSQL& Get();
        //连接mysql服务器
        void StartSQLService(SQL_Info info);
        //单例线程锁
        void Lock();
        //单例线程锁
        void UnLock();
        //增
        int InsertData(const char* table, const char* fields, const char* values);
        //删
        int DeleteDate(const char* table, const char* fields, const char* values);
        //返回数据库查询结果的行数, 1代表存在, 0代表不存在
        int QueryValueRows(const char* table, const char* fields, const char* values);
        //清空数据表
        int CleanUpTable(const char* table);
    private:
        DealSQL() = default;
        //初始化和连接mysql
        int SQLConnect();

        SQL_Info sqlInfo;
        MYSQL mysql;
        MYSQL_RES* sqlRes;
        std::mutex mux;
    };

}
#endif
