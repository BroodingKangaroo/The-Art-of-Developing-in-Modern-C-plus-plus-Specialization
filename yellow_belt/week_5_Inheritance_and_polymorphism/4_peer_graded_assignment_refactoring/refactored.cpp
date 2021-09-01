#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class Human {
public:

    Human(const string &name, const string &type) : Name(name), Type(type) {}

    virtual void Walk(const string &destination) const {
        cout << Type + ": " << Name << " walks to: " << destination << endl;
    }

    const string Name;
    const string Type;
};

class Student : public Human {
public:

    Student(const string &name, const string &favouriteSong) : Human(name, "Student"), FavouriteSong(favouriteSong) {
    }

    void Learn() const {
        cout << "Student: " << Name << " learns" << endl;
    }

    void SingSong() const {
        cout << "Student: " << Name << " sings a song: " << FavouriteSong << endl;
    }

    void Walk(const string &destination) const override {
        Human::Walk(destination);
        SingSong();
    }

private:
    string FavouriteSong;
};


class Teacher : public Human {
public:

    Teacher(const string &name, const string &subject) : Human(name, "Teacher"), Subject(subject) {
    }

    void Teach() const {
        cout << "Teacher: " << Name << " teaches: " << Subject << endl;
    }

private:
    string Subject;
};


class Policeman : public Human {
public:
    Policeman(const string &name) : Human(name, "Policeman") {
    }

    void Check(const Human &h) const {
        cout << "Policeman: " << Name << " checks " + h.Type + ". " + h.Type + "'s name is: " << h.Name << endl;
    }

};

void VisitPlaces(const Human &h, const vector<string> &places) {
    for (const auto &p: places) {
        h.Walk(p);
    }
}

int main() {
    vector<shared_ptr<Human>> humans = {
            make_shared<Teacher>(Teacher("Jim", "Math")),
            make_shared<Student>(Student("Ann", "We will rock you"))
    };

    for (const auto &h: humans) {
        VisitPlaces(*h, {"Moscow", "London"});
    }

    Policeman p("Bob");
    p.Check(*humans[1]);

    return 0;
}
