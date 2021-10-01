#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

//[секция 1]
//ключ1=значение1
//ключ2=значение2
//
//[секция 2]
//ключ3=значение3
//ключ4=значение4

namespace Ini {
    using Section = unordered_map<string, string>;

    class Document {
    public:
        Section &AddSection(string name);

        const Section &GetSection(const string &name) const;

        size_t SectionCount() const;

    private:
        unordered_map<string, Section> sections;
    };

    Document Load(istream &input);
}