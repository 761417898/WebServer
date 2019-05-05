#ifndef BASE_LOGFILE_H
#define BASE_LOGFILE_H

#include "../MutexLock.h"
#include "FileUtil.h"
#include <string>
#include <memory>

namespace GaoServer {

class LogFile : noncopyable {
public:
    explicit LogFile(const std::string& basename, int flushEveryN = 1024);
    ~LogFile();

    void append(const char* logline, int len);
    void flush();
    bool rollFile();
private:
    void append_unlocked(const char* logline, int len);

    const std::string basename_;
    const int flushEveryN_;

    int count_;
    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;
};

}

#endif //LOGFILE
