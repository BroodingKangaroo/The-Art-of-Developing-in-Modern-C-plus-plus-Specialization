#include "geo2d.h"
#include "game_object.h"

#include "test_runner.h"

#include <vector>
#include <memory>

using namespace std;

// Определите классы Unit, Building, Tower и Fence так, чтобы они наследовались от
// GameObject и реализовывали его интерфейс.

template<typename T>
struct Collider : GameObject {
    bool Collide(const GameObject &that) const override {
        return that.CollideWith(static_cast<const T &>(*this));
    }
};

class Unit : public Collider<Unit> {
public:
    explicit Unit(geo2d::Point position) : position_(position) {
    }

    bool CollideWith(const Unit &that) const override;

    bool CollideWith(const Building &that) const override;

    bool CollideWith(const Tower &that) const override;

    bool CollideWith(const Fence &that) const override;

    const geo2d::Point &GetPosition() const { return position_; }

private:
    const geo2d::Point position_;
};

class Building : public Collider<Building> {
public:
    explicit Building(geo2d::Rectangle geometry) : geometry_(geometry) {
    }

    bool CollideWith(const Unit &that) const override;

    bool CollideWith(const Building &that) const override;

    bool CollideWith(const Tower &that) const override;

    bool CollideWith(const Fence &that) const override;

    const geo2d::Rectangle &GetGeometry() const { return geometry_; }

private:
    const geo2d::Rectangle geometry_;
};

class Tower : public Collider<Tower> {
public:
    explicit Tower(geo2d::Circle geometry) : geometry_(geometry) {
    }

    bool CollideWith(const Unit &that) const override;

    bool CollideWith(const Building &that) const override;

    bool CollideWith(const Tower &that) const override;

    bool CollideWith(const Fence &that) const override;

    const geo2d::Circle &GetGeometry() const { return geometry_; }

private:
    const geo2d::Circle geometry_;
};


class Fence : public Collider<Fence> {
public:
    explicit Fence(geo2d::Segment geometry) : geometry_(geometry) {
    }

    bool CollideWith(const Unit &that) const override;

    bool CollideWith(const Building &that) const override;

    bool CollideWith(const Tower &that) const override;

    bool CollideWith(const Fence &that) const override;

    const geo2d::Segment &GetGeometry() const { return geometry_; }

private:
    const geo2d::Segment geometry_;
};

//----------------Unit-------------------

bool Unit::CollideWith(const Unit &that) const {
    return geo2d::Collide(this->position_, that.GetPosition());
}

bool Unit::CollideWith(const Building &that) const {
    return geo2d::Collide(this->position_, that.GetGeometry());
}

bool Unit::CollideWith(const Tower &that) const {
    return geo2d::Collide(this->position_, that.GetGeometry());
}

bool Unit::CollideWith(const Fence &that) const {
    return geo2d::Collide(this->position_, that.GetGeometry());
}

#define COLLIDE_WITH_MACROS(Class, ArgClass)                           \
    bool Class::CollideWith(const ArgClass& that) const {              \
        return geo2d::Collide(this->geometry_, that.GetGeometry());    \
    }


//----------------Building---------------

COLLIDE_WITH_MACROS(Building, Building);

COLLIDE_WITH_MACROS(Building, Tower);

COLLIDE_WITH_MACROS(Building, Fence);

bool Building::CollideWith(const Unit &that) const {
    return geo2d::Collide(this->geometry_, that.GetPosition());
}

//------------------Tower------------------

COLLIDE_WITH_MACROS(Tower, Building);

COLLIDE_WITH_MACROS(Tower, Tower);

COLLIDE_WITH_MACROS(Tower, Fence);

bool Tower::CollideWith(const Unit &that) const {
    return geo2d::Collide(this->geometry_, that.GetPosition());
}

//------------------Fence------------------

COLLIDE_WITH_MACROS(Fence, Building);

COLLIDE_WITH_MACROS(Fence, Tower);

COLLIDE_WITH_MACROS(Fence, Fence);

bool Fence::CollideWith(const Unit &that) const {
    return geo2d::Collide(this->geometry_, that.GetPosition());
}

// Реализуйте функцию Collide из файла GameObject.h
bool Collide(const GameObject &first, const GameObject &second) {
    return first.Collide(second);
}


void TestAddingNewObjectOnMap() {
    // Юнит-тест моделирует ситуацию, когда на игровой карте уже есть какие-то объекты,
    // и мы хотим добавить на неё новый, например, построить новое здание или башню.
    // Мы можем его добавить, только если он не пересекается ни с одним из существующих.
    using namespace geo2d;

    const vector<shared_ptr<GameObject>> game_map = {
            make_shared<Unit>(Point{3, 3}),
            make_shared<Unit>(Point{5, 5}),
            make_shared<Unit>(Point{3, 7}),
            make_shared<Fence>(Segment{{7, 3},
                                       {9, 8}}),
            make_shared<Tower>(Circle{Point{9, 4}, 1}),
            make_shared<Tower>(Circle{Point{10, 7}, 1}),
            make_shared<Building>(Rectangle{{11, 4},
                                            {14, 6}})
    };

    for (size_t i = 0; i < game_map.size(); ++i) {
        Assert(
                Collide(*game_map[i], *game_map[i]),
                "An object doesn't collide with itself: " + to_string(i)
        );

        for (size_t j = 0; j < i; ++j) {
            Assert(
                    !Collide(*game_map[i], *game_map[j]),
                    "Unexpected collision found " + to_string(i) + ' ' + to_string(j)
            );
        }
    }

    auto new_warehouse = make_shared<Building>(Rectangle{{4, 3},
                                                         {9, 6}});
    ASSERT(!Collide(*new_warehouse, *game_map[0]));
    ASSERT(Collide(*new_warehouse, *game_map[1]));
    ASSERT(!Collide(*new_warehouse, *game_map[2]));
    ASSERT(Collide(*new_warehouse, *game_map[3]));
    ASSERT(Collide(*new_warehouse, *game_map[4]));
    ASSERT(!Collide(*new_warehouse, *game_map[5]));
    ASSERT(!Collide(*new_warehouse, *game_map[6]));

    auto new_defense_tower = make_shared<Tower>(Circle{{8, 2}, 2});
    ASSERT(!Collide(*new_defense_tower, *game_map[0]));
    ASSERT(!Collide(*new_defense_tower, *game_map[1]));
    ASSERT(!Collide(*new_defense_tower, *game_map[2]));
    ASSERT(Collide(*new_defense_tower, *game_map[3]));
    ASSERT(Collide(*new_defense_tower, *game_map[4]));
    ASSERT(!Collide(*new_defense_tower, *game_map[5]));
    ASSERT(!Collide(*new_defense_tower, *game_map[6]));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestAddingNewObjectOnMap);
    return 0;
}