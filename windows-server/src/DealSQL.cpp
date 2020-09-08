#include "DealSQL.h"

namespace tg {

    DealSQL & DealSQL::Get() {
        static DealSQL sql;
        return sql;
    }

    void DealSQL::StartSQLService(SQL_Info info){
        sqlInfo = std::move(info);

        if(SQLConnect() == 0){
            LOG("Database connect failed");
            exit(-1);
        }


    }

    void DealSQL::Lock() {
        mux.lock();
    }

    void DealSQL::UnLock() {
        mux.unlock();
    }

    int DealSQL::SQLConnect() {
        mysql_init(&mysql);

        char value = 1;
        mysql_options(&mysql, MYSQL_OPT_RECONNECT, (char*)&value);

        if(!mysql_real_connect(&mysql,
                               sqlInfo.host.c_str(),
                               sqlInfo.user.c_str(),
                               sqlInfo.password.c_str(),
                               sqlInfo.database.c_str(),
                               sqlInfo.port,
                               nullptr,
                               0)){
            mysql_close(&mysql);
            return 0;
        }
        mysql_query(&mysql, "set names gbk");
        CleanUpTable("client_info");
        return 1;
    }

    int DealSQL::InsertData(const char *table, const char *fields, const char *values) {
        mysql_ping(&mysql);

        const char* fm = "insert %s (%s) values (%s)";
        char query[FORMAT_SIZE] = {0};
        sprintf(query, fm, table, fields, values);

        sqlRes = mysql_store_result(&mysql);
        mysql_free_result(sqlRes);
        if(mysql_query(&mysql, query)){
            LOG(mysql_error(&mysql));
            exit(-1);
        }
        return 1;
    }

    int DealSQL::DeleteDate(const char *table, const char *fields, const char *values) {
        mysql_ping(&mysql);

        const char* fm = "delete from %s where %s = '%s'";
        char query[FORMAT_SIZE] = {0};
        sprintf(query, fm, table, fields, values);

        sqlRes = mysql_store_result(&mysql);
        mysql_free_result(sqlRes);
        if(mysql_query(&mysql, query)){
            LOG(mysql_error(&mysql));
            exit(-1);
        }
        return 1;
    }

    int DealSQL::QueryValueRows(const char *table, const char *fields, const char *values) {
        mysql_ping(&mysql);

        const char* fm = "select %s from %s where %s = '%s'";
        char query[FORMAT_SIZE] = {0};
        sprintf(query, fm, fields, table, fields, values);

        sqlRes = mysql_store_result(&mysql);
        mysql_free_result(sqlRes);
        if(mysql_query(&mysql, query)){
            LOG(mysql_error(&mysql));
            exit(-1);
        }
        sqlRes = mysql_store_result(&mysql);
        return mysql_num_rows(sqlRes);
    }

    int DealSQL::CleanUpTable(const char *table) {
        mysql_ping(&mysql);

        const char* fm = "truncate table %s";
        char query[FORMAT_SIZE] = {0};
        sprintf(query, fm, table);

        sqlRes = mysql_store_result(&mysql);
        mysql_free_result(sqlRes);
        if(mysql_query(&mysql, query)){
            LOG(mysql_error(&mysql));
            exit(-1);
        }
        return 1;
    }
}