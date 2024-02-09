#include <iostream>
#include <vector>
#include <chrono>

int main([[maybe_unused]] int argc, char** argv) {
    auto N = atol(argv[1]);
    auto CHECKS = atol(argv[2]);

    auto start = std::chrono::steady_clock::now();

    for (auto i = 0; i < CHECKS; i++) {
        std::vector<int> vec1(N);
    }

    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
    return 0;
}
