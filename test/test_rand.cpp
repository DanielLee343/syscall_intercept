#include <iostream>
#include <array>

class MyClass {
public:
    MyClass(int value) : data(value) {}

    int getData() const {
        return data;
    }

private:
    int data;
};

int main() {
    std::array<MyClass, 3> myArray = {MyClass(1), MyClass(2), MyClass(3)};

    // Accessing and printing the data of each object in the array
    for (const auto& obj : myArray) {
        std::cout << obj.getData() << " ";
    }
    std::cout << std::endl;

    return 0;
}
