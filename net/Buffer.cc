#include "Buffer.h"
#include <errno.h>
#include <cstdio>

namespace GaoServer {

/*
 *  这样做比起之前来说,节省了ioctl(socketfd, FIONREAD, &length)系统调用，
 *　同时只需要一次read操作，没必要read直到返回EAGAIN
 */
ssize_t Buffer::readFd(int fd, int *savedErrno) {
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable = writableBytes();
    vec[0].iov_base = begin()+writeIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;
    const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = ::readv(fd, vec, iovcnt);
    if (n < 0) {
        *savedErrno = errno;
    } else if (static_cast<size_t>(n) <= writable) {
        writeIndex_ += n;
    } else {
        writeIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }
    return n;
}

}
