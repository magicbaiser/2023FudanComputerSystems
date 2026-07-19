#include <cassert>
#include <iostream>

#define CO_BEGIN switch(_line) { case 0:

#define CO_END _line = __LINE__; case __LINE__:; } return 0;

#define CO_YIELD(...)  _line = __LINE__; return __VA_ARGS__; case __LINE__:;

#define CO_RETURN(...) { _done = true; return __VA_ARGS__; } 

struct coroutine_base {
    int _line;
    bool _done;

    coroutine_base() : _line(0), _done(false) {}

    bool done() const { return _done; }
};

class fib : public coroutine_base {
private:
	int a = 0;
    int b = 1;

public:
    int operator()()
    {
        CO_BEGIN;
        while (!_done)
        {
            CO_YIELD(a);
            int tmp = a;
            a = b;
            b = tmp + b;
        }
        CO_RETURN(-1);
        CO_END;
    }
};

int main() {
    int ans[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
    fib foo;
    for (int i = 0; i < 10; i++)
        assert(foo() == ans[i]);
    std::cout << "libco_v3 test passed!" << std::endl;
}
