#include "Common.h"

using namespace std;


// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Rectangle : public IShape {
public:
    ~Rectangle() override = default;

    unique_ptr<IShape> Clone() const override {
        auto shape = make_unique<Rectangle>();
        shape->SetPosition(position_);
        shape->SetSize(size_);
        shape->SetTexture(texture_);
        return shape;
    }

    void SetPosition(Point point) override {
        position_ = point;
    }

    Point GetPosition() const override {
        return position_;
    }

    void SetSize(Size size) override {
        size_ = size;
    }

    Size GetSize() const override {
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> ptr) override {
        texture_ = ptr;
    }

    ITexture* GetTexture() const override {
        return texture_.get();
    }

    void Draw(Image& image) const override {
        for (size_t y = 0; y < size_.height && y + position_.y < image.size(); y++) {
            for (size_t x = 0; x < size_.width && x + position_.x < image[y].size(); x++) {
                if (texture_ && y < texture_->GetSize().height && x < texture_->GetSize().width) {
                    image[y + position_.y][x + position_.x] = texture_->GetImage()[y][x];
                } else {
                    image[y + position_.y][x + position_.x] = '.';
                }
            }
        }
    }

private:
    Point position_;
    Size size_;
    shared_ptr<ITexture> texture_;
};


class Ellipse : public IShape {
public:
    ~Ellipse() override = default;

    unique_ptr<IShape> Clone() const override {
        auto shape = make_unique<Ellipse>();
        shape->SetPosition(position_);
        shape->SetSize(size_);
        shape->SetTexture(texture_);
        return shape;
    }

    void SetPosition(Point point) override {
        position_ = point;
    }

    Point GetPosition() const override {
        return position_;
    }

    void SetSize(Size size) override {
        size_ = size;
    }

    Size GetSize() const override {
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> ptr) override {
        texture_ = ptr;
    }

    ITexture* GetTexture() const override {
        return texture_.get();
    }

    void Draw(Image& image) const override {
        for (size_t y = 0; y < size_.height && y + position_.y < image.size(); y++) {
            for (size_t x = 0; x < size_.width && x + position_.x < image[y].size(); x++) {
                if (IsPointInEllipse({int(x), int(y)}, size_)) {
                    if (texture_
                        && y < texture_->GetSize().height
                        && x < texture_->GetSize().width) {
                        image[y + position_.y][x + position_.x] = texture_->GetImage()[y][x];
                    } else {
                        image[y + position_.y][x + position_.x] = '.';
                    }
                }
            }
        }
    }

private:
    Point position_;
    Size size_;
    shared_ptr<ITexture> texture_;
};


// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    if (shape_type == ShapeType::Rectangle) {
        return make_unique<Rectangle>();
    } else {
        return make_unique<Ellipse>();
    }
}