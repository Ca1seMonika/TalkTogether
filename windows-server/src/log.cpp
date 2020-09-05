#include "log.h"



namespace tg {
    Log::~Log() {
        sToLog.close();
        sToNote.close();
    }

    Log & Log::Get() {
        static Log log;
        return log;
    }

    void Log::logDown(const char* logInfo) {
        streamMutex.lock();
        sToLog << WHENDATE << logInfo << std::endl;
        streamMutex.unlock();
    }

    void Log::noteDown(const char* id,
                       const char* name,
                       const char* note) {
        streamMutex.lock();
        sToNote << WHENDATE << WHO(name) << WHO(id) << note << std::endl;
        streamMutex.unlock();
    }

    void Log::SetLogFilePath(const char* path) {
        char tmp[256] = {0};
        _getcwd(tmp, sizeof(tmp));
        strcat(tmp, path);
        sToLog.open(tmp, std::ios::app);
    }

    void Log::SetNoteFilePath(const char* path) {
        char tmp[256] = {0};
        _getcwd(tmp, sizeof(tmp));
        strcat(tmp, path);
        sToNote.open(tmp, std::ios::app);
    }

    std::string Log::GetNowDateAndTime() {
        time_t t = time(nullptr);
        char tmp[64] = {0};
        strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));
        return tmp;
    }

    std::string Log::GetNowTime() {
        time_t t = time(nullptr);
        tm* pt = localtime(&t);
        char tmp[9] = {0};
        sprintf(tmp, "%d:%d:%d", pt->tm_hour, pt->tm_min, pt->tm_sec);
        return tmp;
    }
}