#ifndef _LOG_H_
#define _LOG_H_

#include <fstream>
#include <time.h>

#define LOG(level) tg::Log::Get().WriteLog(level)
#define Note tg::Log::Get().GetNoteStream() << '[' << tg::Log::GetNowDateAndTime() << '] '

namespace tg {

    enum level {
        CONNECT,
        DISCONNECT,
        SERVER_START,
    };

    class Log {
    public:
        Log();
        ~Log();

        //设置Log文件的路径
        void SetLogFilePath(const char* path);
        //设置记录文件路径
        void SetNoteFilePath(const char* path);
        //将文本追加到Log文件尾部
        void WriteLog(level L);
        //返回Log输入文件流
        std::ofstream& GetNoteStream();

        //returns a static Log object so as to use the same Log object in multiple cpp files.
        //返回一个静态的Log对象,以便于在多个cpp文件中使用同一个Log对象.
        static Log &Get();

        //获得完整日期和时间
        static std::string GetNowDateAndTime();
        //获得当前时间
        static std::string GetNowTime();

    private:
        std::ofstream sToLog;
        std::ofstream sToNote;
    };



}
#endif
