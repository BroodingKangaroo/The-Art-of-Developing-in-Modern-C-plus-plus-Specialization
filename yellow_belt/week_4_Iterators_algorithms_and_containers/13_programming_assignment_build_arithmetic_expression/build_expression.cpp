#include <iostream>
#include <string>

using namespace std;

void buildExpression(string& init, const string& operation, int number) {
    init += string(")") + " " + operation + " " + to_string(number);
}

int main() {

    string init, operation;
    int n, number;

    cin >> init >> n;
    string brackets = string(n, '(');
    while(n--) {
        cin >> operation >> number;
        buildExpression(init, operation, number);
    }

    cout << brackets + init;

    return 0;
}
