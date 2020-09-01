#include "log.h"



namespace tg {
    Log::Log() {}
    Log::~Log() {
        sToLog.close();
        sToNote.close();
    }

    Log & Log::Get() {
        static Log log;
        return log;
    }

    std::ofstream & Log::GetNoteStream(){
        return sToNote;
    }

    void Log::SetLogFilePath(const char *path) {
        sToLog.open(path);
    }

    void Log::SetNoteFilePath(const char *path) {
        sToNote.open(path);
    }
    /*
    void Log::WriteLog(level L) {
        if(L == CONNECT){
            sToNote << '[' << GetNowDateAndTime() << "][" <<
        }
    }*/
    std::string Log::GetNowDateAndTime() {
        time_t t = time(NULL);
        return ctime(&t);
    }

    std::string Log::GetNowTime() {
        time_t t = time(NULL);
        tm* pt = localtime(&t);
        char tmp[9] = {0};
        sprintf(tmp, "%d:%d:%d", pt->tm_hour, pt->tm_min, pt->tm_sec);
        return tmp;
    }
}