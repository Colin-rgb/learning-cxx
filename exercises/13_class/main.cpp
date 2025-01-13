#include "../exercise.h"

// 斐波那契类
class Fibonacci {
    size_t cache[16]; // 存储前16个斐波那契数
    int cached; // 已缓存的数的个数

public:
    // 构造器，初始化前两个斐波那契数
    Fibonacci() : cached(2) {
        cache[0] = 0;
        cache[1] = 1;
    }

    // 获取第 i 个斐波那契数，进行缓存优化
    size_t get(int i) {
        // 如果请求的索引小于已缓存的范围，直接返回缓存值
        if (i < cached) {
            return cache[i];
        }
        // 否则，计算并缓存新的斐波那契数
        for (; cached <= i; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
    }
};

int main(int argc, char **argv) {
    Fibonacci fib;
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
