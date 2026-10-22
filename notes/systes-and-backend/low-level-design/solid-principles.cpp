#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Product {
    string name;
    double price;

public:
    Product(const string& name, const double price) : name(name), price(price) {}

    string getName() const {
        return name;
    }

    double getPrice() const {
        return price;
    }
};

class ShoppingCart {
    vector<const Product*> products;

public:
    void addProduct(const Product& product) {
        products.push_back(&product);
    }

    const vector<const Product*>& getProducts() const {
        return products;
    }
};

class ShoppingCartValueCalculator {
public:
    double calculateCartValue(const ShoppingCart& cart) const {
        double totalValue = 0.0;

        for (const Product* product : cart.getProducts()) {
            totalValue += product->getPrice();
        }

        return totalValue;
    }
};

class OrderInvoiceGenerator {
public:
    string generateTextInvoice(const ShoppingCart& cart, const double totalValue) const {
        ostringstream invoice;
        invoice << "----- Invoice -----\n";

        int index = 1;
        for (const Product* product : cart.getProducts()) {
            invoice << index++ << ". " << product->getName() << " - " << product->getPrice() << "\n";
        }

        invoice << "-------------------\n";
        invoice << "Total: " << totalValue << "\n";
        return invoice.str();
    }
};

int main() {
    Product laptop("Laptop", 75000.0);
    Product mouse("Mouse", 1200.0);
    Product keyboard("Keyboard", 2500.0);

    ShoppingCart cart;
    cart.addProduct(laptop);
    cart.addProduct(mouse);
    cart.addProduct(keyboard);

    ShoppingCartValueCalculator calculator;
    const double totalValue = calculator.calculateCartValue(cart);

    OrderInvoiceGenerator invoiceGenerator;
    const string invoice = invoiceGenerator.generateTextInvoice(cart, totalValue);

    cout << invoice;
    return 0;
}
