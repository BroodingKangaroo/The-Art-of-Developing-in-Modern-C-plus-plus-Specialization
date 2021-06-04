#include <iostream>
#include <cstdint>
#include <vector>

int main() {
    int n;
    uint16_t r, w, h, d;
    std::cin >> n >> r;

    uint64_t blocks_weight = 0;
    while(n--) {
        std::cin >> w >> h >> d;
        blocks_weight += static_cast<uint64_t>(w) * h * d;
    }
    blocks_weight *= r;
    std::cout << blocks_weight;
    return 0;
}