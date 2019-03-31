#ifndef NET_BUFFER_H
#define NET_BUFFER_H

#include <vector>

namespace GaoServer {

class Buffer {
private:
    std::vector<char> buffer_;
    size_t readIndex_;
    size_t writeIndex_;
public:

};

}

#endif
