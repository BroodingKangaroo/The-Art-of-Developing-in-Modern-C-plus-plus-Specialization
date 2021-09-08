#include <string>
#include <vector>

using namespace std;

#define ADD_NAME(y) var##y

#define INTERMEDIATE(x) ADD_NAME(x)

#define UNIQ_ID INTERMEDIATE(__LINE__)

int main() {
    int UNIQ_ID = 0;
    string UNIQ_ID = "hello";
    vector<string> UNIQ_ID = {"hello", "world"};
    vector<int> UNIQ_ID = {1, 2, 3, 4};
}