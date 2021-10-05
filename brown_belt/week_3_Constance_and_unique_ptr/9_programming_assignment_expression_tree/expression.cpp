#include <memory>

#include "Common.h"

class Addition : public Expression {
public:
    ~Addition() override = default;

    Addition(ExpressionPtr lhs, ExpressionPtr rhs) : lhs_(move(lhs)), rhs_(move(rhs)) {}

    int Evaluate() const override {
        return lhs_->Evaluate() + rhs_->Evaluate();
    }

    std::string ToString() const override {
        return "(" + lhs_->ToString() + ")" + "+" + "(" + rhs_->ToString() + ")";
    }

private:
    ExpressionPtr lhs_;
    ExpressionPtr rhs_;
};

class Multiplication : public Expression {
public:
    ~Multiplication() override = default;

    Multiplication(ExpressionPtr lhs, ExpressionPtr rhs) : lhs_(move(lhs)), rhs_(move(rhs)) {}

    int Evaluate() const override {
        return lhs_->Evaluate() * rhs_->Evaluate();
    }

    std::string ToString() const override {
        return "(" + lhs_->ToString() + ")" + "*" + "(" + rhs_->ToString() + ")";
    }

private:
    ExpressionPtr lhs_;
    ExpressionPtr rhs_;
};

class Number : public Expression {
public:
    ~Number() override = default;

    explicit Number(int data) : data_(data) {}

    int Evaluate() const override {
        return data_;
    }

    std::string ToString() const override {
        return std::to_string(data_);
    }

private:
    int data_;
};

ExpressionPtr Value(int value) {
    return std::make_unique<Number>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<Addition>(move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<Multiplication>(move(left), move(right));
}
