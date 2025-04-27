#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
using namespace std;

class Prod;
class PaymentStrategy;
class Order;

const int MAX_PRODUCTS = 150;
const int MAX_ORDERS = 50;
const int MAX_INPUT_LENGTH = 100;
const int MAX_CART_ITEMS = 100;


class Prod {
private:
    char id[10];
    char name[50];
    double price;


public:
    Prod() {
        strcpy(id, "");
        strcpy(name, "");
        price = 0.0;
    }


    Prod(const char* pid, const char* pname, double pprice) {
        strcpy(id, pid);
        strcpy(name, pname);
        price = pprice;
    }


    const char* getId() const { return id; }
    const char* getName() const { return name; }
    double getPrice() const { return price; }
};

class CartItem {
private:
    Prod product;
    int quantity;


public:
    CartItem() : quantity(0) {}
   
    CartItem(const Prod& p, int q) : product(p), quantity(q) {}
   
    const Prod& getProduct() const { return product; }
    int getQuantity() const { return quantity; }
    void setQuantity(int q) { quantity = q; }
    double getTotalPrice() const { return product.getPrice() * quantity; }
};

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() {}
    virtual void pay(double amount) = 0;
    virtual const char* getMethodName() const = 0;
    virtual PaymentStrategy* clone() const = 0; 
};


class CashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid $" << fixed << setprecision(2) << amount << " using Cash" << endl;
    }
   
    const char* getMethodName() const override {
        return "Cash";
    }
   
    PaymentStrategy* clone() const override {
        return new CashPayment(*this);
    }
};


class CardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid $" << fixed << setprecision(2) << amount << " using the payment method of Credit/Debit Card" << endl;
    }
   
    const char* getMethodName() const override {
        return "Credit / Debit";
    }
   
    PaymentStrategy* clone() const override {
        return new CardPayment(*this);
    }
};


class GCashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid $" << fixed << setprecision(2) << amount << " using the payment method of GCash" << endl;
    }
   
    const char* getMethodName() const override {
        return "GCash";
    }
   
    PaymentStrategy* clone() const override {
        return new GCashPayment(*this);
    }
};

class Order {
private:
    int id;
    CartItem items[MAX_CART_ITEMS];
    int itemCount;
    double totalAmount;
    PaymentStrategy* paymentMethod; 
    char paymentMethodName[20];  


public:
    Order() : id(0), itemCount(0), totalAmount(0.0), paymentMethod(nullptr) {
        strcpy(paymentMethodName, "");
    }
   
    Order(int orderId, const CartItem* cartItems, int count, PaymentStrategy* payment)
        : id(orderId), itemCount(0), totalAmount(0.0), paymentMethod(nullptr) {

        for (int i = 0; i < count && i < MAX_CART_ITEMS; i++) { 
            items[i] = cartItems[i];
            itemCount++;
            totalAmount += cartItems[i].getTotalPrice();
        }
       

        if (payment != nullptr) {
            paymentMethod = payment->clone(); 
            strcpy(paymentMethodName, payment->getMethodName());
        } else {
            strcpy(paymentMethodName, "Unknown");
        }
    }
   

    ~Order() {
        delete paymentMethod;  
        paymentMethod = nullptr;  
    }

    Order(const Order& other) : id(other.id), itemCount(other.itemCount), totalAmount(other.totalAmount), paymentMethod(nullptr) {

        for (int i = 0; i < itemCount && i < MAX_CART_ITEMS; i++) { 
            items[i] = other.items[i];
        }

        strcpy(paymentMethodName, other.paymentMethodName);

        if (other.paymentMethod != nullptr) {
            paymentMethod = other.paymentMethod->clone();
        }
    }
   

    Order& operator=(const Order& other) {
        if (this != &other) {
            id = other.id;
            itemCount = other.itemCount;
            totalAmount = other.totalAmount;

            for (int i = 0; i < itemCount && i < MAX_CART_ITEMS; i++) { 
                items[i] = other.items[i];
            }

            strcpy(paymentMethodName, other.paymentMethodName);

            delete paymentMethod;
            paymentMethod = nullptr; 

            if (other.paymentMethod != nullptr) {
                paymentMethod = other.paymentMethod->clone();
            }
        }
        return *this;
    }
   
    int getId() const { return id; }
    const CartItem* getItems() const { return items; }
    int getItemCount() const { return itemCount; }
    double getTotalAmount() const { return totalAmount; }
    const PaymentStrategy* getPaymentMethod() const { return paymentMethod; }
    const char* getPaymentMethodName() const { return paymentMethodName; }
};


class ProductCatalog {
private:
    Prod products[MAX_PRODUCTS];
    int productCount;
   
    ProductCatalog() : productCount(0) {
        addProduct(Prod("A", "Lipstick", 159));
        addProduct(Prod("B", "Blush", 299));
        addProduct(Prod("C", "Mascara", 149));
        addProduct(Prod("D", "Eye Shadow Palette", 399));
        addProduct(Prod("E", "Brush for Blush", 79));
        addProduct(Prod("F", "Lip Gloss", 88));
        addProduct(Prod("G", "Highligter", 115));
        addProduct(Prod("H", "Eyebrow Pencil", 129));
        addProduct(Prod("I", "Eyeliner", 69));
        addProduct(Prod("J", "Foundation Liquid", 599));
    }
   
public:

    static ProductCatalog& getInstance() {
        static ProductCatalog instance;
        return instance;
    }

    ProductCatalog(const ProductCatalog&) = delete;
    ProductCatalog& operator=(const ProductCatalog&) = delete;
   
    void addProduct(const Prod& product) {
        if (productCount < MAX_PRODUCTS) {
            products[productCount++] = product;
        } else {
            cout << "Error: Prod catalog is full!" << endl;
        }
    }
   
    const Prod* getProducts() const {
        return products;
    }
   
    int getProductCount() const {
        return productCount;
    }
   
    const Prod* findProductById(const char* id) const {
        for (int i = 0; i < productCount; i++) {

            if (strcasecmp(products[i].getId(), id) == 0) {
                return &products[i];
            }
        }
        return nullptr;
    }
   
    void displayProducts() const {
        cout << "\nAvailable Products\n";
        cout << setw(15) << left << "Prod ID"
             << setw(20) << left << "Name"
             << setw(10) << right << "Price ($)" << endl;
       
        for (int i = 0; i < productCount; i++) {
            cout << setw(15) << left << products[i].getId()
                 << setw(20) << left << products[i].getName()
                 << setw(10) << right << fixed << setprecision(2) << products[i].getPrice() << endl;
        }
        cout << endl;
    }
};

class ShoppingCart {
private:
    CartItem items[MAX_CART_ITEMS];
    int itemCount;
   
public:
    ShoppingCart() : itemCount(0) {}
   
    void addProduct(const Prod& product, int quantity) {

        for (int i = 0; i < itemCount; i++) {
            if (strcasecmp(items[i].getProduct().getId(), product.getId()) == 0) {
                items[i].setQuantity(items[i].getQuantity() + quantity);
                return;
            }
        }

        if (itemCount < MAX_CART_ITEMS) {
            items[itemCount++] = CartItem(product, quantity);
        } else {
            cout << "Error: Shopping cart is full!" << endl;
        }
    }
   
    const CartItem* getItems() const {
        return items;
    }
   
    int getItemCount() const {
        return itemCount;
    }
   
    double getTotalAmount() const {
        double total = 0.0;
        for (int i = 0; i < itemCount; i++) {
            total += items[i].getTotalPrice();
        }
        return total;
    }
   
    void clear() {
        itemCount = 0;
    }
   
    void displayCart() const {
        if (itemCount == 0) {
            cout << "Your shopping cart is currently empty." << endl;
            return;
        }
       
        cout << "\nShopping Cart \n";
        cout << setw(15) << left << "Product ID"
             << setw(20) << left << "Name"
             << setw(10) << right << "Price ($)"
             << setw(10) << right << "Quantity"
             << setw(12) << right << "Total ($)" << endl;
       
        for (int i = 0; i < itemCount; i++) {
            const Prod& product = items[i].getProduct();
            cout << setw(15) << left << product.getId()
                 << setw(20) << left << product.getName()
                 << setw(10) << right << fixed << setprecision(2) << product.getPrice()
                 << setw(10) << right << items[i].getQuantity()
                 << setw(12) << right << fixed << setprecision(2) << items[i].getTotalPrice() << endl;
        }
       
        cout << string(67, '-') << endl;
        cout << setw(55) << right << "Total Amount: $"
             << setw(10) << right << fixed << setprecision(2) << getTotalAmount() << endl;
        cout << endl;
    }
};

class OrderManager {
private:
    Order orders[MAX_ORDERS];
    int orderCount;
   

    OrderManager() : orderCount(0) {}
   
public:

    static OrderManager& getInstance() {
        static OrderManager instance;
        return instance;
    }

    OrderManager(const OrderManager&) = delete;
    OrderManager& operator=(const OrderManager&) = delete;
   
    int createOrder(const ShoppingCart& cart, PaymentStrategy* paymentMethod) {
        if (orderCount >= MAX_ORDERS) {
            throw runtime_error("Error: Maximum number of orders reached!");
        }
       
        int newOrderId = orderCount + 1;
        orders[orderCount] = Order(newOrderId, cart.getItems(), cart.getItemCount(), paymentMethod);
       
        // Log the order
        ofstream logFile("order_log.txt", ios::app);
        if (logFile.is_open()) {
            logFile << "[LOG] -> Order ID: " << newOrderId
                   << " has been successfully checked out and paid using "
                   << paymentMethod->getMethodName() << "." << endl;
            logFile.close();
        } else {
            cerr << "Warning: Could not open log file!" << endl;
        }
       
        orderCount++;
        return newOrderId;
    }
   
    void displayOrders() const {
        if (orderCount == 0) {
            cout << "No orders have been placed yet." << endl;
            return;
        }
       
        for (int i = 0; i < orderCount; i++) {
            const Order& order = orders[i];
            cout << "\nOrder ID: " << order.getId() << endl;
            cout << "Total Amount: $" << fixed << setprecision(2) << order.getTotalAmount() << endl;
            cout << "Payment Method: " << order.getPaymentMethodName() << endl;
            cout << "Order Details: " << endl;
           
            cout << setw(15) << left << "Product ID"
                 << setw(20) << left << "Name"
                 << setw(10) << right << "Price ($)"
                 << setw(10) << right << "Quantity" << endl;
           
            const CartItem* items = order.getItems();
            for (int j = 0; j < order.getItemCount(); j++) {
                const Prod& product = items[j].getProduct();
                cout << setw(15) << left << product.getId()
                     << setw(20) << left << product.getName()
                     << setw(10) << right << fixed << setprecision(2) << product.getPrice()
                     << setw(10) << right << items[j].getQuantity() << endl;
            }
           
            if (i < orderCount - 1) {
                cout << endl;
            }
        }
    }
};


class InvalidProductException : public exception {
private:
    char message[100];
   
public:
    InvalidProductException(const char* id) {
        sprintf(message, "Error: Product with ID '%s' not found!", id);
    }
   
    const char* what() const noexcept override {
        return message;
    }
};


class InvalidInputException : public exception {
private:
    char message[100];
   
public:
    InvalidInputException(const char* msg) {
        strcpy(message, msg);
    }
   
    const char* what() const noexcept override {
        return message;
    }
};


int strcasecmp(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        char c1 = tolower(*s1);
        char c2 = tolower(*s2);
        if (c1 != c2) {
            return c1 - c2;
        }
        s1++;
        s2++;
    }
    return tolower(*s1) - tolower(*s2);
}


bool isWhitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}


int parseFirstInteger(const char* input) {
    int result = 0;
    int i = 0;
    bool foundDigit = false;
   

    while (input[i] != '\0' && isWhitespace(input[i]))
        i++;
       

    if (input[i] >= '0' && input[i] <= '9') {
        foundDigit = true;

        while (input[i] >= '0' && input[i] <= '9') {
            result = result * 10 + (input[i] - '0');
            i++;
        }

        while (input[i] != '\0') {
            if (!isWhitespace(input[i])) {
                return -1; 
            }
            i++;
        }
       
        return result;
    }

    return -1;
}


char getYesNoResponse() {
    char input[MAX_INPUT_LENGTH];
    cin.getline(input, MAX_INPUT_LENGTH);

    int i = 0;
    while (input[i] != '\0' && isWhitespace(input[i]))
        i++;

    return (input[i] != '\0') ? toupper(input[i]) : 'N';
}


void trimString(char* str) {
    if (!str) return;

    char* start = str;
    while (*start && isWhitespace(*start)) {
        start++;
    }

    if (!*start) {
        *str = '\0';
        return;
    }

    char* end = start;
    char* lastNonSpace = start;
    while (*end) {
        if (!isWhitespace(*end)) {
            lastNonSpace = end;
        }
        end++;
    }

    *(lastNonSpace + 1) = '\0';

    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

bool isValidProductId(const char* input) {

    char trimmedInput[MAX_INPUT_LENGTH];
    strncpy(trimmedInput, input, MAX_INPUT_LENGTH - 1);
    trimmedInput[MAX_INPUT_LENGTH - 1] = '\0';
    trimString(trimmedInput);

    if (strlen(trimmedInput) != 1) {
        return false;
    }
    
    char c = toupper(trimmedInput[0]);
    return (c >= 'A' && c <= 'J');
}

class ShoppingApplication {
private:
    ShoppingCart cart;
   
    void displayMenu() const {
        cout << "\nShopping System Menu\n";
        cout << "1. View Products\n";
        cout << "2. View Shopping Cart\n";
        cout << "3. View Orders\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
    }
   
    void viewProducts() {
        ProductCatalog& catalog = ProductCatalog::getInstance();
        catalog.displayProducts();
        
        char input[MAX_INPUT_LENGTH];
        char productId[10];
        char choice;
        
        do {
            bool validInput = false;
            while (!validInput) {
                cout << "Enter the ID of the product you want to add to the shopping cart: ";
                cin.getline(input, MAX_INPUT_LENGTH);
                
                if (!isValidProductId(input)) {
                    cout << "Invalid product ID. Please enter a single letter from A to J." << endl;
                    continue;
                }

                productId[0] = toupper(input[0]);
                productId[1] = '\0';

                const Prod* product = catalog.findProductById(productId);
                if (product == nullptr) {
                    cout << "Product with ID '" << productId << "' not found." << endl;
                    continue;
                }

                int quantity = 0;
                bool validQuantity = false;
                while (!validQuantity) {
                    cout << "Enter quantity: ";
                    cin.getline(input, MAX_INPUT_LENGTH);
                    
                    quantity = parseFirstInteger(input);
                    if (quantity <= 0) {
                        cout << "Quantity must be a positive number." << endl;
                        continue;
                    }
                    
                    validQuantity = true;
                }
                
                cart.addProduct(*product, quantity);
                cout << "Product added successfully!" << endl;
                validInput = true;
            }
            
            cout << "Do you want to add another product to the shopping cart? (Y/N): ";
            choice = getYesNoResponse();
            
        } while (choice == 'Y');
    }
   
    void viewShoppingCart() {
        cart.displayCart();
       
        if (cart.getItemCount() == 0) {
            return;
        }
       
        cout << "Do you want to check out all the products? (Y/N): ";
        char choice = getYesNoResponse();
       
        if (choice == 'Y') {
            checkout();
        }
    }
   
    void checkout() {

        cout << "\nItems for Checkout \n";
        cart.displayCart();

        char input[MAX_INPUT_LENGTH];
        int paymentChoice;
        PaymentStrategy* paymentMethod = nullptr;
       
        cout << "Select payment method:\n";
        cout << "1. Cash\n";
        cout << "2. Credit/Debit Card\n";
        cout << "3. GCash\n";
        cout << "Enter your choice: ";
        cin.getline(input, MAX_INPUT_LENGTH);
       
        paymentChoice = parseFirstInteger(input);
       
        try {
            if (paymentChoice < 1 || paymentChoice > 3) {
                throw InvalidInputException("Error: Invalid payment method selected!");
            }
           
            switch (paymentChoice) {
                case 1:
                    paymentMethod = new CashPayment();
                    break;
                case 2:
                    paymentMethod = new CardPayment();
                    break;
                case 3:
                    paymentMethod = new GCashPayment();
                    break;
            }

            OrderManager& orderManager = OrderManager::getInstance();
            int orderId = orderManager.createOrder(cart, paymentMethod);

            paymentMethod->pay(cart.getTotalAmount());
           
            cout << "You have successfully checked out the products!" << endl;
            cout << "Your order ID is: " << orderId << endl;

            cart.clear();
           
        } catch (const exception& e) {
            cout << e.what() << endl;
        }

        delete paymentMethod;
    }
   
    void viewOrders() {
        OrderManager& orderManager = OrderManager::getInstance();
        orderManager.displayOrders();
    }
   
public:
    void run() {
        int choice;
        bool exitCondition = false;
        char input[MAX_INPUT_LENGTH];
       
        while (!exitCondition) {
            displayMenu();

            cin.getline(input, MAX_INPUT_LENGTH);

            trimString(input);

            choice = parseFirstInteger(input);

            if (choice == -1) {
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
           
            switch (choice) {
                case 1:
                    viewProducts();
                    break;
                case 2:
                    viewShoppingCart();
                    break;
                case 3:
                    viewOrders();
                    break;
                case 4:
                    exitCondition = true;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
       
        cout << "Thank you for using our Shopping System!" << endl;
    }
};


int main() {
    try {
        ShoppingApplication app;
        app.run();
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
   
    return 0;
}