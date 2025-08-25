#include <cstddef>
#include <vector>


template <typename T>
class FixedSizeVector {
private:
    std::vector<T> data;
    size_t currentSize;

public:
    FixedSizeVector(size_t startSize)
        : data(startSize)
        , currentSize(startSize) {}

    void push_back(const T& move) { data[currentSize++] = move; }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        data[currentSize++] = T(std::forward<Args>(args)...);
    }

    void clear() { currentSize = 0; }

    const T& operator[](size_t index) const { return data[index]; }
    T& operator[](size_t index) { return data[index]; }

    size_t size() const { return currentSize; }

    typename std::vector<T>::iterator begin() { return data.begin(); }
    typename std::vector<T>::const_iterator begin() const { return data.begin(); }

    typename std::vector<T>::iterator end() { return data.begin() + currentSize; }
    typename std::vector<T>::const_iterator end() const { return data.begin() + currentSize; }
};
