#include <iostream>

// Strategy for Walk
class Walk
{
public:
    virtual void walk() = 0;
    virtual ~Walk() = default;
};

// Concrete classes for walk
class NormalWalk : public Walk
{
    void walk()
    {
        std::cout << "Normal walking behaviour\n";
    }
};

class WalkWithWheels : public Walk
{
    void walk()
    {
        std::cout << "Walks with wheels\n";
    }
};

class NoWalk : public Walk
{
    void walk()
    {
        std::cout << "Can not walk\n";
    }
};

// Strategy for Talk
class Talk
{
public:
    virtual void talk() = 0;
    virtual ~Talk() = default;
};

// Concrete classes for talk
class NormalTalk : public Talk
{
    void talk()
    {
        std::cout << "Normal talking behaviour\n";
    }
};

class NoTalk : public Talk
{
    void talk()
    {
        std::cout << "Can not talk\n";
    }
};

// Robot class
class Robot
{
private:
    Walk *walkBehavior;
    Talk *talkBehavior;

public:
    Robot(Walk *walk, Talk *talk)
    {
        this->walkBehavior = walk;
        this->talkBehavior = talk;
    }

    void walk() {
        walkBehavior->walk();
    }

    void talk() {
        talkBehavior->talk();
    }
};

int main()
{
    std::cout << "Hello world" << std::endl;

    Robot* robot1 = new Robot(new WalkWithWheels(), new NormalTalk());
    std::cout << "robot1 in action\n";
    robot1->walk();
    robot1->talk();

    Robot* robot2 = new Robot(new WalkWithWheels(), new NoTalk());
    std::cout << "robot2 in action\n";
    robot2->walk();
    robot2->talk();

    return EXIT_SUCCESS;
}