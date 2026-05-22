#include<iostream>

class I_Coffee {
public:
    virtual double cost() = 0;
    virtual std::string description() = 0;

    virtual ~I_Coffee() = default;
};

class C_Coffee : public I_Coffee {
public:
    double cost() override {
        return 50.0;
    }

    std::string description() override {
        return "Just a simple Coffee\n";
    }
};

class I_CoffeeDecorator : public I_Coffee {
protected:
    C_Coffee* coffee;

public:
    virtual double cost() = 0;
    virtual std::string description() = 0;

    virtual ~I_CoffeeDecorator() = default;
};

class C_LatteDecorator : public I_CoffeeDecorator {
public:
    C_LatteDecorator(C_Coffee* coffee) {
        this->coffee = coffee;
    }

    double cost() override {
        return coffee->cost() + 10.0;
    }

    std::string description() override {
        return "Adding a latte decorator to the simple coffee\n";
    }
};

class C_CreameDecorator : public I_CoffeeDecorator {

public:
    C_CreameDecorator(C_Coffee* coffee) {
        this->coffee = coffee;
    }

    double cost() override {
        return coffee->cost() + 15.0;
    }

    std::string description() override {
        return "Adding a creame decorator to the simple coffee\n";
    }
};

int main() {
    I_Coffee* coffee = new C_Coffee();

    double coffeeCost = coffee->cost();
    std::string coffeeDescription = coffee->description();

    std::cout << "Coffee cost: " << coffeeCost << "\n" << "Coffee description: " << coffeeDescription << "\n";

    std::cout << "Let's try some decorators now\n";

    C_Coffee* coffeeBase = new C_Coffee();

    C_LatteDecorator* latteCoffee = new C_LatteDecorator(coffeeBase);
    std::cout << coffeeBase->description();
    std::cout << latteCoffee->description();
    std::cout << "Latte coffee cost: " << latteCoffee->cost() << "\n";

    C_CreameDecorator* creameCoffee = new C_CreameDecorator(coffeeBase);
    std::cout << coffeeBase->description();
    std::cout << creameCoffee->description();
    std::cout << "Creame coffee cost: " << creameCoffee->cost() << "\n";

    return EXIT_SUCCESS;
}
