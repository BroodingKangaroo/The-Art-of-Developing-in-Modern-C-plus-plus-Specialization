#include <vector>
#include <memory>


namespace RAII {

    template <typename Provider>
    class Booking {
    public:

    Booking(Provider* provider, int& counter): provider_(provider), counter_(&counter) {
    }

    Booking(const Booking&other): provider_(other.provider_), counter_(other.counter_) {
        (*counter_)++;
    }

    Booking(Booking&&other): provider_(other.provider_), counter_(other.counter_) {
        other.provider_ = nullptr;
        other.counter_ = nullptr;
    }

    Booking& operator = (const Booking&other) {
        provider_ = other.provider_;
        counter_ = other.counter_;
        (*counter_)++;
        return *this;
    }

    Booking& operator = (Booking&&other) {
        provider_ = std::move(other.provider_);
        counter_ = std::move(other.counter_);
        other.provider_ = nullptr;
        other.counter_ = nullptr;
        return *this;
    }

    ~Booking() {
        if (counter_) (*counter_)--;
    }

    private:
        Provider* provider_;
        int *counter_;
    };



}