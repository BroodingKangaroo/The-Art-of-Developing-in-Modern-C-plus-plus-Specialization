#include "test_runner.h"

#include <vector>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template<typename Token>
using Sentence = vector<Token>;

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template<typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    vector<Sentence<Token>> sentences;
    Sentence<Token> current_sentence;
    bool isPrevPunctuation = false;
    for (auto &token: tokens) {
        if (!token.IsEndSentencePunctuation()) {
            if (isPrevPunctuation) {
                sentences.push_back(move(current_sentence));
                current_sentence = Sentence<Token>();
            }
        }
        isPrevPunctuation = token.IsEndSentencePunctuation();
        current_sentence.push_back(move(token));
    }
    if (!current_sentence.empty()) sentences.push_back(move(current_sentence));
    return sentences;
}


struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }

    bool operator==(const TestToken &other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream &operator<<(ostream &stream, const TestToken &token) {
    return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"},
                                                  {"into"},
                                                  {"sentences"},
                                                  {"!"}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"}, {"into"}, {"sentences"}, {"!"}}
                                        })
    );

    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"},
                                                  {"into"},
                                                  {"sentences"},
                                                  {"!", true}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
                                        })
    );

    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"},
                                                  {"into"},
                                                  {"sentences"},
                                                  {"!", true},
                                                  {"!", true},
                                                  {"Without"},
                                                  {"copies"},
                                                  {".", true}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"},   {"into"},   {"sentences"}, {"!", true}, {"!", true}},
                                                {{"Without"}, {"copies"}, {".", true}},
                                        })
    );
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSplitting);
    return 0;
}