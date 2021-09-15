#include "stats.h"

HttpRequest ParseRequest(string_view line) {
    line.remove_prefix(line.find_first_not_of(' '));
    auto first_space = line.find(' ');
    auto second_space = line.find(' ', first_space + 1);
    return {
            line.substr(0, first_space),
            line.substr(first_space + 1, second_space - first_space - 1),
            line.substr(second_space + 1)
    };
}

void Stats::AddMethod(string_view method) {
    if (available_methods_.count(method)) method_stats_[method]++;
    else method_stats_["UNKNOWN"]++;
}

void Stats::AddUri(string_view uri) {
    if (available_uris_.count(uri)) uri_stats_[uri]++;
    else uri_stats_["unknown"]++;
}

const map<string_view, int> &Stats::GetMethodStats() const {
    return method_stats_;
}

const map<string_view, int> &Stats::GetUriStats() const {
    return uri_stats_;
}
