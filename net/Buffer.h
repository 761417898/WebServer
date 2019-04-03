#ifndef NET_BUFFER_H
#define NET_BUFFER_H

/*vector:
 *    |<- 8 Bytes ->|<---------- 1024 Bytes ------------>|       
 *    
 *    ^             ^                                    ^
 *    |             |                                    |
 *  begin       ridx/widx        real buffer           capacity
 */

#include <vector>
#include <unistd.h>
#include <sys/uio.h>
#include <string>

namespace GaoServer {

class Buffer {

    /// +-------------------+------------------+------------------+
    /// | prependable bytes |  readable bytes  |  writable bytes  |
    /// |                   |     (CONTENT)    |                  |
    /// +-------------------+------------------+------------------+
    /// |                   |                  |                  |
    /// 0      <=      readerIndex   <=   writerIndex    <=     size
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
      : buffer_(kCheapPrepend + initialSize),
      readIndex_(kCheapPrepend),
      writeIndex_(kCheapPrepend){}


private:
    std::vector<char> buffer_;
    //这里的index是int而非指针，为了防止迭代器失效的问题
    size_t readIndex_;
    size_t writeIndex_;
public:

    ssize_t readFd(int fd, int* savedErrno);

    size_t readableBytes() const
    { return writeIndex_ - readIndex_; }

    size_t writableBytes() const
    { return buffer_.size() - writeIndex_; }

    //缓冲区最前面的几个字节，防止可能出现的需要在前方添加字符的情况，
    //这样出现的话，添加的新字节的效率也不会太低
    size_t prependableBytes() const
    { return readIndex_; }

    //used for append
    void append(const char* /*restrict*/ data, size_t len) {
        ensureWritableBytes(len);
        std::copy(data, data+len, beginWrite());
        hasWritten(len);
    }

    void append(const void* /*restrict*/ data, size_t len) {
        append(static_cast<const char*>(data), len);
    }

    void ensureWritableBytes(size_t len) {
        if (writableBytes() < len) {
            makeSpace(len);
        }
    }

    std::string readAsString(size_t len) {
        std::string result(peek(), len);
        std::copy(peek(), peek() + len, result.begin());
        return result;
    }

    const char* peek()
    { return begin() + readIndex_; }

    void retrieve(size_t len) {

    }

private:
    char* begin()
    { return &*buffer_.begin(); }

    void makeSpace(size_t len) {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
            buffer_.resize(writeIndex_+len);
        }
        else {
        // move readable data to the front, make space inside buffer
            size_t readable = readableBytes();
            std::copy(begin()+readIndex_,
                      begin()+writeIndex_,
                      begin()+kCheapPrepend);
            readIndex_ = kCheapPrepend;
            writeIndex_ = readIndex_ + readable;
        }
    }

    void hasWritten(size_t len) {
        writeIndex_ += len;
    }

    char* beginWrite()
    { return begin() + writeIndex_; }



};

}

#endif
