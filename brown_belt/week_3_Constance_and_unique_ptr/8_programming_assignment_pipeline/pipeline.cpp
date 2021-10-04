#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>

using namespace std;


struct Email {
    string from;
    string to;
    string body;
};


class Worker {
public:
    virtual ~Worker() = default;

    virtual void Process(unique_ptr<Email> email) = 0;

    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        if (next_) {
            next_->Process(move(email));
        }
    }

public:
    void SetNext(unique_ptr<Worker> next) {
        next_ = move(next);
    }

private:
    unique_ptr<Worker> next_;
};


class Reader : public Worker {
public:
    explicit Reader(istream& in) : in_(in) {
    }

    void Process(unique_ptr<Email> email) override {
        PassOn(move(email));
    }

    void Run() override {
        for (Email email; ConstructEmail(email);) {
            Process(move(make_unique<Email>(email)));
        }
    }

private:
    bool ConstructEmail(Email& email) const {
        if (!getline(in_, email.from)) {
            return false;
        }
        getline(in_, email.to);
        getline(in_, email.body);
        return true;
    }

    istream& in_;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;

    explicit Filter(Function filter) : filter_(filter) {
    }

    void Process(unique_ptr<Email> email) override {
        if (filter_(*email)) {
            PassOn(move(email));
        }
    }

private:
    Function filter_;
};


class Copier : public Worker {
public:
    explicit Copier(string recipient) : recipient_(std::move(recipient)) {
    }

    void Process(unique_ptr<Email> email) override {
        string from = email->from, to = email->to, body = email->body;
        PassOn(move(email));

        if (recipient_ != to) {
            PassOn(make_unique<Email>(Email{from, recipient_, body}));
        }
    }

private:
    string recipient_;
};


class Sender : public Worker {
public:

    explicit Sender(ostream& out) : out_(out) {
    }

    void Process(unique_ptr<Email> email) override {
        out_ << email->from << endl << email->to << endl << email->body << endl;
        PassOn(move(email));
    }
    // реализуйте класс
private:
    ostream& out_;
};


// реализуйте класс
class PipelineBuilder {
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in) {
        pipeline_.push(make_unique<Reader>(in));
    }

    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter) {
        pipeline_.push(make_unique<Filter>(filter));
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient) {
        pipeline_.push(make_unique<Copier>(recipient));
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out) {
        pipeline_.push(make_unique<Sender>(out));
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() {
        while (pipeline_.size() != 1) {
            unique_ptr<Worker> current = move(pipeline_.top());
            pipeline_.pop();
            pipeline_.top()->SetNext(move(current));
        }
        return move(pipeline_.top());
    }

private:
    stack<unique_ptr<Worker>> pipeline_;
};


void TestSanity() {
    string input = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "ralph@example.com\n"
            "erich@example.com\n"
            "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "erich@example.com\n"
            "richard@example.com\n"
            "Are you sure you pressed the right button?\n"
    );

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}