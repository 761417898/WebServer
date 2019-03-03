#ifndef BASE_NONCOPYABLE_H
#define BASE_NONCOPYABLE_H

namespace GaoServer {

class noncopyable {
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

}

#endif // BASE_NONCOPYABLE_H


