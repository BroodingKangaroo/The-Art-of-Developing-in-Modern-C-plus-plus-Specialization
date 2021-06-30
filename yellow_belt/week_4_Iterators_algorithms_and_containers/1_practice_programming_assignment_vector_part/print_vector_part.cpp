#include <vector>
#include <iostream>
#include <algorithm>

void PrintVectorPart(const std::vector<int> &numbers) {
    auto first_negative = std::find_if(numbers.begin(), numbers.end(), [](int elem) {
        return elem < 0;
    });
    while(first_negative > numbers.begin()) {
        --first_negative;
        std::cout << *first_negative << " ";
    }
}

int main() {

    PrintVectorPart({6, 1, 8, -5, 4});
    std::cout << std::endl;
    PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
    std::cout << std::endl;
    PrintVectorPart({6, 1, 8, 5, 4});
    std::cout << std::endl;


    return 0;
}