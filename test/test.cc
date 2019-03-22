#include <map>
#include <cstdio>
#include <iostream>
#include <memory>

using namespace std;

class S{


    public:
    int a = 123;

    S() {
        printf("构造函数\n");
    }

    ~S() {
        printf("析构函数~S()\n");
    }

};

typedef shared_ptr<S> PS;

typedef map<int, PS> mmap;

void func(mmap mp) {
    PS ps = make_shared<S>();
    cout << ps->a << endl;
    printf("临时对象销毁\n");
    mp.insert(pair<int, PS>(123, ps));
    printf("end of func\n");
}

int main() {
    mmap mp;
    func(mp);
    printf("end of main\n");
}
