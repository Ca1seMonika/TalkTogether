#ifndef _LOG_H_
#define _LOG_H_

#include <fstream>
#include <ctime>
#include <mutex>
#include <direct.h>

#define WHENDATE '[' << tg::Log::GetNowDateAndTime() << ']'
#define WHEN '[' << tg::Log::GetNowTime() << ']'
#define WHO(name) '[' << name << ']'
#define LOG(content) tg::Log::Get().logDown(content)
#define NOTE(id, name, content) tg::Log::Get().noteDown(id, name, content)


namespace tg {
    class Log {
    public:

        ~Log();

        //设置Log文件的路径
        void SetLogFilePath(const char* path);
        //设置记录文件路径
        void SetNoteFilePath(const char* path);

        void logDown(const char* logInfo);

        void noteDown(const char* id,
                      const char* name,
                      const char* note);

        //Meyers' Singleton
        static Log &Get();

        //获得完整日期和时间
        static std::string GetNowDateAndTime();
        //获得当前时间
        static std::string GetNowTime();

    private:
        Log() = default;

        std::ofstream sToLog;
        std::ofstream sToNote;
        std::mutex mux;
    };



}
#endif
