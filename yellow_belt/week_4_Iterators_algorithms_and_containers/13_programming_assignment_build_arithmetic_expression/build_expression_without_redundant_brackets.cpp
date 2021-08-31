#include <iostream>
#include <string>
#include <map>

using namespace std;


int operandPriority[6] = {2, 1, 0, 1, 0, 2};

void buildExpressionWithBrackets(string &init, const string &operation, int number) {
    init += string(")") + " " + operation + " " + to_string(number);
}

void buildExpression(string &init, const string &operation, int number) {
    init += " " + operation + " " + to_string(number);
}

int main() {

    string init;
    char operand;
    int n, number;

    cin >> init >> n;

    int prevOperandPriority = INT32_MAX;
    string brackets;

    while (n--) {
        cin >> operand >> number;
        if (operandPriority[operand - '*'] > prevOperandPriority) {
            brackets += "(";
            buildExpressionWithBrackets(init, string(1, operand), number);
        } else {
            buildExpression(init, string(1, operand), number);
        }
        prevOperandPriority = operandPriority[operand - '*'];
    }

    cout << brackets + init;

    return 0;
}
