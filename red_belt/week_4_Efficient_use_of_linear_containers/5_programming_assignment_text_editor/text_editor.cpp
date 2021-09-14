#include <string>
#include <list>
#include "test_runner.h"

using namespace std;

class Editor {
public:
    // Реализуйте конструктор по умолчанию и объявленные методы
    Editor() : cursor_position_(text_.begin()) {}

    void Left() {
        cursor_position_ = (cursor_position_ != text_.begin()) ? prev(cursor_position_) : cursor_position_;
    }

    void Right() {
        cursor_position_ = (cursor_position_ != text_.end()) ? next(cursor_position_) : cursor_position_;
    }

    void Insert(char token) {
        text_.insert(cursor_position_, token);
    }

    void Cut(size_t tokens = 1) {
        buffer_.clear();
        int i = 0;
        for (; cursor_position_ != text_.end() && i < tokens; cursor_position_ = text_.erase(cursor_position_), i++) {
            buffer_.push_back(*cursor_position_);
        }
    }

    void Copy(size_t tokens = 1) {
        buffer_.clear();
        int i = 0;
        for (auto it = cursor_position_; it != text_.end() && i < tokens; it = next(it), i++) {
            buffer_.push_back(*it);
        }
    }

    void Paste() {
        text_.insert(cursor_position_, buffer_.begin(), buffer_.end());
    }

    string GetText() const {
        return {text_.begin(), text_.end()};
    }

private:
    list<char> text_;
    list<char> buffer_;
    list<char>::iterator cursor_position_;
};

void TypeText(Editor &editor, const string &text) {
    for (char c: text) {
        editor.Insert(c);
    }
}

void TestEditing() {
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");
        for (size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for (size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
}

void TestReverse() {
    Editor editor;

    const string text = "esreveR";
    for (char c: text) {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "");

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
    Editor editor;

    editor.Paste();
    TypeText(editor, "example");
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
    return 0;
}