#include<iostream>
using namespace std;

class Product {
    string name;
    double price;

public:
    Product(const string &name, const double &price) {
        this->name = name;
        this->price = price;
    }
};

class ShoppingCart {
    vector<Product*> products;

public:
    void addProduct(const Product* &product) {
        this->products.push_back(product);
    }

    const vector<Product*> getProducts() {
        return this->products;
    }
}

class CalculateShoppingCartValue {
    ShoppingCart* cart;

    double calculateCartValue() {
        double totalValue = 0.0;
        vector<Product*> allProducts = cart->getProducts;
        
        for (Product* product : allProducts) {
            totalValue += product.price;
        }

        return totalValue;
    }
}

class GenerateOrderInvoice {
    ShoppingCart* cart;
    
    
}

int main() {
    return 0;

    
}
