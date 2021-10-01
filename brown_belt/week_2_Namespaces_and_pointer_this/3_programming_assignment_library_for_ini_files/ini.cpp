#include "ini.h"


Ini::Document Ini::Load(istream &input) {
    Document doc;
    Section *current_section;
    for (string node; getline(input, node);) {
        if (node.empty()) continue;
        if (node.front() == '[' && node.back() == ']') {
            current_section = &doc.AddSection(node.substr(1, node.length() - 2));
        } else {
            size_t delimiter_position = node.find('=');
            auto key = node.substr(0, delimiter_position);
            auto value = node.substr(delimiter_position + 1);
            current_section->insert({key, value});
        }
    }
    return doc;
}

size_t Ini::Document::SectionCount() const {
    return sections.size();
}

const Ini::Section &Ini::Document::GetSection(const string &name) const {
    return sections.at(name);
}

Ini::Section &Ini::Document::AddSection(string name) {
    return sections[name];
}
