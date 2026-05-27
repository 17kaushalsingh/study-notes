#include<iostream>
#include<unordered_set>
#include<cstdlib>

class I_Observer {
public:
    virtual void update() = 0;

    virtual ~I_Observer() = default;
};

class I_Observable {
public:
    virtual void add(I_Observer* observer) = 0;
    virtual void remove(I_Observer* observer) = 0;
    virtual void notify() = 0;

    virtual ~I_Observable() = default;
};

class C_Observable : public I_Observable {
    std::unordered_set<I_Observer*> observers;
    
public:
    void add(I_Observer* observer) override {
        if (observers.find(observer) == observers.end()) {
            std::cout << "Added observer object to observable object\n";
            observers.insert(observer);
        }
    }
    
    void remove(I_Observer* obv) override {
        if (observers.find(obv) != observers.end()) {
            std::cout << "Removed observer object from observable object\n";
            observers.erase(obv);
        }
    }
    
    void notify() override {
        for (auto &observer : observers) {
            observer->update();
        }
    }
};

class C_Observer : public I_Observer {
    I_Observable* subject;
    
public:
    C_Observer(I_Observable* subject) {
        this->subject = subject;
    }
    
    void update() override {
        std::cout << "The subject has been updated\n";
        std::cout << "Updates recieved by observer: " << this << "\n";
    }
};


int main() {
    C_Observable* subject = new C_Observable();

    C_Observer* observer1 = new C_Observer(subject);
    C_Observer* observer2 = new C_Observer(subject);

    subject->add(observer1);
    subject->add(observer2);

    subject->notify();

    subject->remove(observer1);

    subject->notify();

    return EXIT_SUCCESS;
}
