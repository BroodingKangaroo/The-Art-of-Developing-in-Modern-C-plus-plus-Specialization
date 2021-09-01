#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>
#include <cmath>


using namespace std;

class Figure {
public:
    virtual double Perimeter() = 0;

    virtual double Area() = 0;

    virtual string Name() = 0;
};

class Triangle : public Figure {
    const int a_;
    const int b_;
    const int c_;
public:
    Triangle(int a, int b, int c) : a_(a), b_(b), c_(c) {}

    double Perimeter() override {
        return a_ + b_ + c_;
    }

    double Area() override {
        double halfPerimeter = Perimeter() / 2;
        return sqrt(halfPerimeter * (halfPerimeter - a_) * (halfPerimeter - b_) * (halfPerimeter - c_));
    }

    string Name() override {
        return "TRIANGLE";
    }
};

class Rect : public Figure {
    const int width_;
    const int height_;
public:
    Rect(int width, int height) : width_(width), height_(height) {}

    double Area() override {
        return width_ * height_;
    }

    double Perimeter() override {
        return width_ * 2 + height_ * 2;
    }

    string Name() override {
        return "RECT";
    }
};

class Circle : public Figure {
    const int r_;
public:
    Circle(int r) : r_(r) {}

    double Perimeter() override {
        return 2 * 3.14 * r_;
    }

    double Area() override {
        return 3.14 * r_ * r_;
    }

    string Name() override {
        return "CIRCLE";
    }
};

shared_ptr<Figure> CreateFigure(istringstream &is) {
    string figure;
    is >> figure;
    if (figure == "RECT") {
        int width, height;
        is >> width >> height;
        return make_shared<Rect>(width, height);
    } else if (figure == "CIRCLE") {
        int r;
        is >> r;
        return make_shared<Circle>(r);
    } else {
        int a, b, c;
        is >> a >> b >> c;
        return make_shared<Triangle>(a, b, c);
    }
}

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line); ) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            // Пропускаем "лишние" ведущие пробелы.
            // Подробнее об std::ws можно узнать здесь:
            // https://en.cppreference.com/w/cpp/io/manip/ws
            is >> ws;
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}