#include <memory>
#include <iostream>

class MyClass {
public:
    MyClass() {
        std::cout << "MyClass Constructor" << std::endl;
    }

    ~MyClass() {
        std::cout << "MyClass Destructor" << std::endl;
    }

    void SomeMethod() {
        std::cout << "Executing SomeMethod" << std::endl;
    }
};

int main() {
    // Create a unique_ptr to an instance of MyClass
    std::unique_ptr<MyClass> myPtr(new MyClass());

    // Access the object using the arrow operator
    myPtr->SomeMethod();

    // Use the unique_ptr as needed
    if (myPtr) {
        // Do something with the object
    }

    // The unique_ptr will automatically deallocate the object when it goes out of scope

    return 0;
}
