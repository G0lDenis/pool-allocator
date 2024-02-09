#include "../addons/pool_allocator.h"

#include <iostream>
#include <vector>
#include <chrono>

using Manager = pool_allocator::PoolManager;
using Pool = pool_allocator::Pool;
using IntAllocator = pool_allocator::PoolAllocator<int>;

int main([[maybe_unused]] int argc, char** argv) {

    auto N = atol(argv[1]);
    auto CHECKS = atol(argv[2]);

    auto start = std::chrono::steady_clock::now();

    auto pool1 = Pool(malloc((N * sizeof(int) + 8) * N), (N * sizeof(int) + 8), N);
    std::vector<Pool*> pools = {&pool1};
    Manager main_manager(pools);
    const IntAllocator int_alloc(main_manager);

    for (auto i = 0; i < CHECKS; i++) {
        std::vector<int, IntAllocator> vec1(N, int_alloc);
    }

    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();

    return 0;
}
