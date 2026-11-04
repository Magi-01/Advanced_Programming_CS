#include <iostream>
#include <stdlib.h>

class MyClass {
public:
    int x;

    void fun() {
        this->x = 10; // Using 'this' pointer to access member variable
        (*this).x = 20; // Using dereferenced 'this' pointer to access member variable
    }
};

class Vector {
public:
    double * data;
    Vector() : data(nullptr) {} // Default constructor. This is similar to __init__ in Python.

    Vector(double n) : data(new double[n]) {}// Default constructor. This is similar to __init__ in Python with input.
                                            //  It initializes the data member with a new array of doubles of size n.

    Vector(double n) {
        this->data = new double[n]; // Initialize the data member with the provided pointer. "this" is similar to self in Python.
    } // equivalent to the above constructor

    Vector(const Vector& other) : data(other.data) {
        // Copy the data from the other object to this object.
        // Copy constructor. This is similar to __copy__ in Python.
        // It initializes the data member with the data from the other object.
    }

    void func() const { // Default constructor for const objects
        std::cout << "This is a const member function." << std::endl;
    }

    /*Vector& operator=(const Vector& other) {
        // Assignment operator. This is similar to __assign__ in Python.
        // It assigns the data from the other object to this object.
        if (this != &other) { // Check for self-assignment
            delete[] data; // Free existing resource
            data = new double[other.size()]; // Allocate new resource
            std::copy(other.data, other.data + other.size(), data); // Copy data
        }
        return *this; // Return the current object by reference
    }*/

    ~Vector() {
        delete[] data; // Destructor. This is similar to __del__ in Python.
        // It frees the memory allocated for the data member.
    }

    // Didn't understand the purpose of this function. 
    // It seems to be a placeholder for a function that returns the size of the vector. 
    // Specifically on copying and what would happen if del operation was done on the original object. 
    // The size() function is used to get the size of the vector, which is needed in the assignment operator to allocate the correct amount of memory 
    //for the new data array. If the original object is deleted, the data pointer in the copied object would become dangling,
    // leading to undefined behavior when trying to access or delete it. 
    // To avoid this, a deep copy should be made in the copy constructor and assignment operator.

    // When declaring a constuctor, it is important to ensure that the default operatoins by compiler on the constructor is either manually defined or deleted.

    std::size_t size;
};

int main() {
    MyClass obj;
    obj.fun();
    std::cout << "Value of x: " << obj.x << std::endl; // Output: Value of x: 20

    Vector v; // Vector object created using default constructor
    v = Vector(5); // Vector object created using parameterized constructor
    Vector x = v; // Pointer to Vector object
    //Vector y{2.1,-2.2,3.2,4.2,5.1}; // Pointer to Vector object
    Vector z = {100}; // parameterised constructor
    //z[0] = 100; // Pointer to Vector object
    //z[50] = 100; // Pointer to Vector object
    Vector w;
    w=v; // copy assignment operator
    delete[] v.data; // Free the allocated memory for v
    delete[] w.data; // Free the allocated memory for w
    delete[] z.data; // Free the allocated memory for z
    return 0;
}