#include <iostream>
#include <vector>
#include <cstdint>

int main() {
    size_t n;
    std::cin >> n;
    std::vector<int32_t> temperatures = std::vector<int32_t>(n);
    double average = 0;
    for (auto& temperature : temperatures) {
        std::cin >> temperature;
        average += temperature;
    }
    average /= static_cast<double>(n);
    std::vector<int32_t>above_average;
    for (size_t i = 0; i < temperatures.size(); ++i) {
        if (temperatures[i] > average) above_average.push_back(i);
    }

    std::cout << above_average.size() << std::endl;
    for (const auto temperature : above_average) {
        std::cout << temperature << " ";
    }
    return 0;
}