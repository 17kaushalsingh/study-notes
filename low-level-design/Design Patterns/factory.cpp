#include<iostream>

/* // Simple factory
class Burger{
public:
    virtual void prepare() = 0;
    virtual ~Burger() = default;
};

class BasicBurger : public Burger {
    void prepare() {
        std::cout << "Preparing a basic burger with bun, patty and ketchup\n";
    }
};

class StandardBurger : public Burger {
    void prepare() {
        std::cout << "Preparing a standard burger with bun, patty, cheese and lettuce\n";
    }
};

class PremiumBurger : public Burger {
    void prepare() {
        std::cout << "Preparing a premium burger with bun, patty, cheese, lettuce, sauces and paneer\n";
    }
};

class BurgerFactory{
public:
    Burger* createObject(std::string burgerType) {
        if (burgerType == "Basic") {
            Burger* burger = new BasicBurger();
            return burger;
        } else if (burgerType == "Standard") {
            Burger* burger = new StandardBurger();
            return burger;
        } else if (burgerType == "Premium") {
            Burger* burger = new PremiumBurger();
            return burger;
        } else {
            std::cout << "Can not prepare burger of this type\n";
            return nullptr;
        }
    }
};

int main() {

    BurgerFactory* burgerFactory = new BurgerFactory();

    Burger* burger1 = burgerFactory->createObject("Basic");
    std::cout << "Craeted a burger of type: " << typeid(*burger1).name() << "\n";
    burger1->prepare();

    Burger* burger2 = burgerFactory->createObject("Standard");
    std::cout << "Craeted a burger of type: " << typeid(*burger2).name() << "\n";
    burger2->prepare();

    Burger* burger3 = burgerFactory->createObject("Premium");
    std::cout << "Craeted a burger of type: " << typeid(*burger3).name() << "\n";
    burger3->prepare();

    Burger* burger4 = burgerFactory->createObject("Veg whopper");
    std::cout << "Craeted a burger of type: " << typeid(*burger4).name() << "\n";
    burger4->prepare();

    return EXIT_SUCCESS;
}
*/

/* // Factory method

class Burger{
public:
    virtual void prepare() = 0;
    virtual ~Burger() = default;
};

class BasicBurger : public Burger {
    void prepare() {
        std::cout << "Using typical dough for buns\n";
        std::cout << "Preparing a basic burger with bun, patty and ketchup\n";
    }
};

class StandardBurger : public Burger {
    void prepare() {
        std::cout << "Using typical dough for buns\n";
        std::cout << "Preparing a standard burger with bun, patty, cheese and lettuce\n";
    }
};

class PremiumBurger : public Burger {
    void prepare() {
        std::cout << "Using typical dough for buns\n";
        std::cout << "Preparing a premium burger with bun, patty, cheese, lettuce, sauces and paneer\n";
    }
};

class MultigrainBasicBurger : public Burger {
    void prepare() {
        std::cout << "Using multigrain dough for buns\n";
        std::cout << "Preparing a basic burger with bun, patty and ketchup\n";
    }
};

class MultigrainStandardBurger : public Burger {
    void prepare() {
        std::cout << "Using multigrain dough for buns\n";
        std::cout << "Preparing a standard burger with bun, patty, cheese and lettuce\n";
    }
};

class MultigrainPremiumBurger : public Burger {
    void prepare() {
        std::cout << "Using multigrain dough for buns\n";
        std::cout << "Preparing a premium burger with bun, patty, cheese, lettuce, sauces and paneer\n";
    }
};

class BurgerFactory{
public:
    virtual Burger* getBurger(std::string burgerType) = 0;
    virtual ~BurgerFactory() = default;
};

class KingBurgers : public BurgerFactory {
    Burger* getBurger(std::string burgerType) {
        if (burgerType == "Basic") {
            Burger* burger = new BasicBurger();
            return burger;
        } else if (burgerType == "Standard") {
            Burger* burger = new StandardBurger();
            return burger;
        } else if (burgerType == "Premium") {
            Burger* burger = new PremiumBurger();
            return burger;
        } else {
            std::cout << "Can not prepare burger of this type\n";
            return nullptr;
        }
    }
};

class SinghBurgers : public BurgerFactory {
    Burger* getBurger(std::string burgerType) {
        if (burgerType == "Basic") {
            Burger* burger = new MultigrainBasicBurger();
            return burger;
        } else if (burgerType == "Standard") {
            Burger* burger = new MultigrainStandardBurger();
            return burger;
        } else if (burgerType == "Premium") {
            Burger* burger = new MultigrainPremiumBurger();
            return burger;
        } else {
            std::cout << "Can not prepare multigrain burger of this type\n";
            return nullptr;
        }
    }
};

int main() {

    // BurgerFactory* burgerFactory = new KingBurgers();

    // Burger* burger1 = burgerFactory->getBurger("Basic");
    // std::cout << "Craeted a burger of type: " << typeid(*burger1).name() << "\n";
    // burger1->prepare();
    // std::cout << "\n";

    // Burger* burger2 = burgerFactory->getBurger("Standard");
    // std::cout << "Craeted a burger of type: " << typeid(*burger2).name() << "\n";
    // burger2->prepare();
    // std::cout << "\n";

    // Burger* burger3 = burgerFactory->getBurger("Premium");
    // std::cout << "Craeted a burger of type: " << typeid(*burger3).name() << "\n";
    // burger3->prepare();
    // std::cout << "\n";

    BurgerFactory* burgerFactory = new SinghBurgers();

    Burger* burger1 = burgerFactory->getBurger("Basic");
    std::cout << "Craeted a burger of type: " << typeid(*burger1).name() << "\n";
    burger1->prepare();
    std::cout << "\n";

    Burger* burger2 = burgerFactory->getBurger("Standard");
    std::cout << "Craeted a burger of type: " << typeid(*burger2).name() << "\n";
    burger2->prepare();
    std::cout << "\n";

    Burger* burger3 = burgerFactory->getBurger("Premium");
    std::cout << "Craeted a burger of type: " << typeid(*burger3).name() << "\n";
    burger3->prepare();
    std::cout << "\n";

    return EXIT_SUCCESS;
}
*/

// Abstract factory methods
class Burger {
public:
    virtual void prepare() = 0;
    virtual ~Burger() = default;
};

class ClassicBurger : public Burger {
    void prepare() override {
        std::cout << "Using normal dough for making burger\n";
        std::cout << "Preparing classic burger\n";
    }
};

class CheeseBurger : public Burger {
    void prepare() override {
        std::cout << "Using normal dough for making burger\n";
        std::cout << "Preparing cheese burger\n";
    }
};

class MultigrainClassicBurger : public Burger {
    void prepare() override {
        std::cout << "Using multigrain dough for making burger\n";
        std::cout << "Preparing classic burger\n";
    }
};

class MultigrainCheeseBurger : public Burger {
    void prepare() override {
        std::cout << "Using multigrain dough for making burger\n";
        std::cout << "Preparing cheese burger\n";
    }
};

class GarlicBread {
public:
    virtual void prepare () = 0;
    virtual ~GarlicBread() = default;
};

class ClassicGarlicBread : public GarlicBread {
    void prepare() override {
        std::cout << "Using normal dough for making garlic bread\n";
        std::cout << "Preparing classic garlic bread\n";
    }
};

class CheeseGarlicBread : public GarlicBread {
    void prepare() override {
        std::cout << "Using normal dough for making garlic bread\n";
        std::cout << "Preparing cheese garlic bread\n";
    }
};

class MultigrainClassicGarlicBread : public GarlicBread {
    void prepare() override {
        std::cout << "Using multigrain dough for making garlic bread\n";
        std::cout << "Preparing classic garlic bread\n";
    }
};

class MultigrainCheeseGarlicBread : public GarlicBread {
    void prepare() override {
        std::cout << "Using multigrain dough for making garlic bread\n";
        std::cout << "Preparing cheese garlic bread\n";
    }
};

class MealFactory {
public:
    virtual Burger* createBurger(std::string &burgerType) = 0;
    virtual GarlicBread* createGarlicBread(std::string &garlicBreadType) = 0;
};

class KingsFastFood : public MealFactory {
    Burger* createBurger(std::string &burgerType) override {
        if (burgerType == "Classic") {
            Burger* burger = new ClassicBurger();
            return burger;
        } else if (burgerType == "Cheese") {
            Burger* burger = new CheeseBurger();
            return burger;
        } else {
            std::cout << "No burger exists for the chosen type\n";
            std::cout << "Can not instantiate burger\n";
            return nullptr;
        }
    }

    GarlicBread* createGarlicBread(std::string &garlicBreadType) override {
        if (garlicBreadType == "Classic") {
            GarlicBread* garlicBread = new ClassicGarlicBread();
            return garlicBread;
        } else if (garlicBreadType == "Cheese") {
            GarlicBread* garlicBread = new CheeseGarlicBread();
            return garlicBread;
        } else {
            std::cout << "No garlic bread exists for the chosen type\n";
            std::cout << "Can not instantiate garlic bread\n";
            return nullptr;
        }
    }
};

class SinghsFastFood : public MealFactory {
    Burger* createBurger(std::string &burgerType) override {
        if (burgerType == "Classic") {
            Burger* burger = new MultigrainClassicBurger();
            return burger;
        } else if (burgerType == "Cheese") {
            Burger* burger = new MultigrainCheeseBurger();
            return burger;
        } else {
            std::cout << "No burger exists for the chosen type\n";
            std::cout << "Can not instantiate burger\n";
            return nullptr;
        }
    }

    GarlicBread* createGarlicBread(std::string &garlicBreadType) override {
        if (garlicBreadType == "Classic") {
            GarlicBread* garlicBread = new MultigrainClassicGarlicBread();
            return garlicBread;
        } else if (garlicBreadType == "Cheese") {
            GarlicBread* garlicBread = new MultigrainCheeseGarlicBread();
            return garlicBread;
        } else {
            std::cout << "No garlic bread exists for the chosen type\n";
            std::cout << "Can not instantiate garlic bread\n";
            return nullptr;
        }
    }
};

int main() {
    MealFactory* shop1 = new SinghsFastFood();
    std::string burgerType = "Cheese";
    Burger* burger1 = shop1->createBurger(burgerType);
    burger1->prepare();

    return EXIT_SUCCESS;
}