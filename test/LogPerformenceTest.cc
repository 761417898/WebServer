#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "../base/log/Logging.h"

using namespace GaoServer;

void sigExit(int) {
    exit(0);
}

const int logLen = 100;

int main(int argc, char *argv[])
{
    signal(SIGALRM, sigExit);
    char str[logLen];
    for (int i = 0; i < logLen; ++i) {
        str[i] = '0';
    }
    str[100] = '\0';
    const char *path = "PerformenceTest.log";
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 777);
    alarm(10);
    while (1) {
        LOG << str;
    }
    return 0;
}
