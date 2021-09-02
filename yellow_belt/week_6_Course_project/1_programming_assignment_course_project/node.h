#include "date.h"

#include <string>
#include <memory>
#include <map>

#pragma once

using namespace std;

enum class Comparison {
    Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual
};

enum LogicalOperation {
    Or, And
};

template<class T>
bool compare(const Comparison &comp, const T &lhs, const T &rhs);

class Node {
public:
    virtual bool Evaluate(const Date &date, const string &event) const = 0;
};

class EmptyNode : public Node {
public:
    bool Evaluate(const Date &date, const string &event) const override;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(
            const LogicalOperation &logicalOperation,
            const shared_ptr<Node> &left,
            const shared_ptr<Node> &right
    );

    bool Evaluate(const Date &date, const string &event) const override;

private:
    const LogicalOperation logical_operation_;
    const shared_ptr<Node> left_;
    const shared_ptr<Node> right_;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison &cmp, const string &value);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    const string value_;
    const Comparison cmp_;
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison &cmp, const Date &date);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    const Date date_;
    const Comparison cmp_;
};