#include "node.h"


template<class T>
bool compare(const Comparison &comp, const T &lhs, const T &rhs) {
    switch (comp) {
        case Comparison::Less:              return lhs < rhs;
        case Comparison::LessOrEqual:       return lhs <= rhs;
        case Comparison::Greater:           return lhs > rhs;
        case Comparison::GreaterOrEqual:    return lhs >= rhs;
        case Comparison::Equal:             return lhs == rhs;
        case Comparison::NotEqual:          return lhs != rhs;
    }
}

LogicalOperationNode::LogicalOperationNode(const LogicalOperation &logicalOperation,
                                           const shared_ptr<Node> &left,
                                           const shared_ptr<Node> &right)
        : logical_operation_(logicalOperation),
          left_(left),
          right_(right) {}

bool LogicalOperationNode::Evaluate(const Date &date, const string &event) const {
    switch(logical_operation_) {
        case Or:  return left_->Evaluate(date, event) || right_->Evaluate(date, event);
        case And: return left_->Evaluate(date, event) && right_->Evaluate(date, event);
    }
}

EventComparisonNode::EventComparisonNode(const Comparison &cmp, const string &value) : value_(value), cmp_(cmp) {}

bool EventComparisonNode::Evaluate(const Date &date, const string &event) const {
    return compare(cmp_, event, value_);
}

DateComparisonNode::DateComparisonNode(const Comparison &cmp, const Date &date) : date_(date), cmp_(cmp) {}

bool DateComparisonNode::Evaluate(const Date &date, const string &event) const {
    return compare(cmp_, date, date_);
}

bool EmptyNode::Evaluate(const Date &date, const string &event) const {
    return true;
}
