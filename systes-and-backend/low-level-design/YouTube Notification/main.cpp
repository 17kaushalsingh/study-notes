#include<iostream>
#include<unordered_set>

class I_User {
protected:
    std::string userName;

public:
    virtual void update() = 0;
    virtual ~I_User() = default;
};

class I_Channel {
protected:
    std::string channelName;
    std::unordered_set<I_User*> subscribers;

public: 
    virtual void addSubscriber(I_User* subscriber) = 0;
    virtual void removeSubscriber(I_User* subscriber) = 0;
    virtual void notifySubscriber() = 0;

    virtual ~I_Channel() = default;
};

class C_Channel : public I_Channel {
public:
    C_Channel(std::string channelName) {
        this->channelName = channelName;
        subscribers.clear();
        std::cout << "New channel created\n";
        std::cout << "Channel name: " << channelName << "\n\n";
    }   

private:
    void addSubscriber(I_User* subscriber) override {
        if (subscribers.find(subscriber) == subscribers.end()) {
            subscribers.insert(subscriber);
        }
    }

    void removeSubscriber(I_User* subscriber) override {
        if (subscribers.find(subscriber) != subscribers.end()) {
            subscribers.erase(subscriber);
        }
    }

    void notifySubscriber() override {
        for (const auto &subscriber : subscribers) {
            subscriber->update();
        }
    }
};

class C_User : public I_User {
    I_Channel* subject;

    void update() override {
        std::cout << "Notification received by user: " << this->userName << "\n";
        std::cout << "New video uploaded by channel id: " << subject << "\n\n";
    }

public:
    C_User(const std::string userName, I_Channel* subject) {
        this->userName = userName;
        this->subject = subject;
    }
};

int main() {

    I_Channel* channel1 = new C_Channel("Sports Tak");

    I_User* user1 = new C_User("Kaushal", channel1);
    I_User* user2 = new C_User("Kanha", channel1);

    channel1->notifySubscriber(); // no notification send, as no subscribers added

    channel1->addSubscriber(user1);
    channel1->addSubscriber(user2);

    channel1->notifySubscriber();

    channel1->removeSubscriber(user1);

    channel1->notifySubscriber();

    return EXIT_SUCCESS;
}