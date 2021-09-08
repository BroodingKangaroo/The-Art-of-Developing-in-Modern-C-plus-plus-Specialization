#include "test_runner.h"

#include <ostream>
using namespace std;

// #define PRINT_VALUES(out, x, y) out << (x) << endl; out << (y) << endl
#define PRINT_VALUES(out, x, y) (out) << (x) << endl << (y) << endl

int main() {
    TestRunner tr;
    tr.RunTest([] {
        ostringstream output;
        PRINT_VALUES(output, 5, "red belt");
        ASSERT_EQUAL(output.str(), "5\nred belt\n");
    }, "PRINT_VALUES usage example");
    return 0;
}

// #1 surround all parameters with parentheses
// #2 use arguments only once
