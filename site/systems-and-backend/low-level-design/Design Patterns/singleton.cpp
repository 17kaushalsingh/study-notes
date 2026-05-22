#include<iostream>

/* // Regular classes and object instantitation
class SingletonClass {
public:
    SingletonClass() {
        std::cout << "Object of singleotn class created\n";
    }
};

int main() {

    SingletonClass* s1 = new SingletonClass();
    SingletonClass* s2 = new SingletonClass();

    if (s1 == s2) {
        std::cout << "s1 and s2 are one and the same\n";
    } else {
        std::cout << "s1 and s2 are different\n";
    }

    return EXIT_SUCCESS;
}
*/

/* // Making the constructor public and using getter methods to create object
class SingletonClass {
private:
    SingletonClass() {
        std::cout << "Object of singleotn class created\n";
    }

public:
    static SingletonClass* getInstance() {
        SingletonClass* obj = new SingletonClass();
        return obj;
    }
};

int main() {

    SingletonClass* s1 = SingletonClass::getInstance();
    SingletonClass* s2 = SingletonClass::getInstance();

    if (s1 == s2) {
        std::cout << "s1 and s2 are one and the same\n";
    } else {
        std::cout << "s1 and s2 are different\n";
    }

    return EXIT_SUCCESS;
}
*/

/* // Singleton design pattern
class SingletonClass {
private:
    static SingletonClass* obj;

    SingletonClass() {
        std::cout << "Object of singleotn class created\n";
    }

public:
    static SingletonClass* getInstance() {
        if (obj == nullptr) {
            obj = new SingletonClass();
            std::cout << "Object created at location: " << obj << "\n";
        }

        return obj;
    }
};

SingletonClass* SingletonClass::obj = nullptr;

int main() {

    SingletonClass* s1 = SingletonClass::getInstance();
    SingletonClass* s2 = SingletonClass::getInstance();

    if (s1 == s2 && s1) {
        std::cout << "s1 and s2 are one and the same\n";
    } else {
        std::cout << "s1 and s2 are different\n";
    } 

    return EXIT_SUCCESS;
}
*/

/* // Thread-safe singleton design pattern
class SingletonClass {
private:
    static SingletonClass* obj;
    static std::mutex mtx;

    SingletonClass() {
        std::cout << "Object of singleotn class created\n";
    }

public:
    static SingletonClass* getInstance() {
        // Single lock

        // std::lock_guard<std::mutex> lock(mtx);
        // if (obj == nullptr) {
        //     obj = new SingletonClass();
        //     std::cout << "Object created at location: " << obj << "\n";
        // }

        // return obj;

        // Double lock
        if (obj == nullptr) {
            std::lock_guard<std::mutex> lock(mtx);
            if (obj == nullptr) {
                obj = new SingletonClass();
                std::cout << "Object created at location: " << obj << "\n";
            }
        }

        return obj;
    }
};

SingletonClass* SingletonClass::obj = nullptr;
std::mutex SingletonClass::mtx;

int main() {

    SingletonClass* s1 = SingletonClass::getInstance();
    SingletonClass* s2 = SingletonClass::getInstance();

    if (s1 == s2 && s1) {
        std::cout << "s1 and s2 are one and the same\n";
    } else {
        std::cout << "s1 and s2 are different\n";
    } 

    return EXIT_SUCCESS;
}
*/

// Eager initialization
class SingletonClass {
private:
    static SingletonClass* obj;

    SingletonClass() {
        std::cout << "Object of singleton class created and instantiated\n";
    }

public:
    static SingletonClass* getInstance() {
        return obj;
    }
};

SingletonClass* SingletonClass::obj = new SingletonClass();

int main() {

    std::cout << "Entering main()\n";
 
    SingletonClass* s1 = SingletonClass::getInstance();
    SingletonClass* s2 = SingletonClass::getInstance();

    std::cout << "Address of s1: " << s1 << "\n";
    std::cout << "Address of s2: " << s2 << "\n";

    if (s1 == s2 && s1) {
        std::cout << "s1 and s2 are one and the same\n";
    } else {
        std::cout << "s1 and s2 are different\n";
    } 

    return EXIT_SUCCESS;
}