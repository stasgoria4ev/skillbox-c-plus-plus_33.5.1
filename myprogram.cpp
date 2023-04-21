#include <iostream>
#include <map>
#include <exception>

/*class AddRemoveException: public std::exception {
    const char* what() const noexcept override {
        return "Invalid command specified(add/remove)";
    }
};*/

struct OnlineStore {
    std::string vendorCode;             //артикул товара
    int amount = 0;                         //количество товара
    std::map<std::string, int> database;//база данных товаров
    std::map<std::string, int> electronicShoppingCart;//корзина
};

void AddingData(std::string& vendorCode, int& amount) {
    std::string amountStr;
    std::cout << "Enter the item number and quantity: ";
    std::cin >> vendorCode >> amountStr;
    for (int i = 0; i < amountStr.length(); i++)
        if (amountStr[i] < '0' || amountStr[i] > '9')
            throw std::invalid_argument(amountStr[i] < '0' ? "< 0" : "> 9");
        else
            amount = std::stoi(amountStr);
}

void Database(std::map<std::string, int>& database) {
    int count = 0;
    for (auto & it : database) {
        if (it.second == 0) count++;
    }
    if (database.empty() || count == database.size()) std::cout << "No products.";
    else {
        for (auto & it : database)
            std::cout << it.first << ' ' << it.second << ", ";
    }
}

void Display(std::map<std::string, int>& database, std::map<std::string, int>& electronicShoppingCart) {
    std::cout << "\nList of goods in the Warehouse of the Store: "; Database(database); std::cout << '\n';
    std::cout << "List of items in the Shopping Cart : "; Database(electronicShoppingCart); std::cout << "\n\n";
}

void AddToCart(OnlineStore& onlineStore) {
    AddingData(onlineStore.vendorCode, onlineStore.amount);
    //-------------проверка на наличие товара в базе данных магазина-------------
    auto it = onlineStore.database.find(onlineStore.vendorCode);                    //поиск в базе
    auto item = onlineStore.electronicShoppingCart.find(onlineStore.vendorCode);    //поиск в корзине
    if (it != onlineStore.database.end() && onlineStore.amount <= it->second && item == onlineStore.electronicShoppingCart.end()) {
        onlineStore.electronicShoppingCart.insert(std::pair<std::string, int>(onlineStore.vendorCode, onlineStore.amount));
        it->second -= onlineStore.amount;
    } else if (it != onlineStore.database.end() && onlineStore.amount <= it->second) {
        item->second += onlineStore.amount;
        it->second -= onlineStore.amount;
    } else
        //std::cout << "Валидация товара на Добавление не прошла.\n";
        throw std::invalid_argument("Goods validation for Adding failed.");
}

void RemoveFromCart(OnlineStore& onlineStore) {
    if (onlineStore.electronicShoppingCart.empty()) std::cout << "There are no items in the cart.\n";
    else {
        AddingData(onlineStore.vendorCode, onlineStore.amount);
        //-------------проверка на наличие товара в базе данных магазина-------------
        auto it = onlineStore.database.find(onlineStore.vendorCode);                    //поиск в базе
        auto item = onlineStore.electronicShoppingCart.find(onlineStore.vendorCode);    //поиск в корзине
        if (item != onlineStore.electronicShoppingCart.end() && onlineStore.amount <= item->second && it != onlineStore.database.end()) {
            it->second += onlineStore.amount;
            item->second -= onlineStore.amount;
        } else
            //std::cout << "Валидация товара на Удаление не прошла.\n";
            throw std::invalid_argument("Product validation for Deletion failed.");

        if (item->second == 0) onlineStore.electronicShoppingCart.erase(item);
    }
}

void AddRemove(OnlineStore& onlineStore) {
    std::string addOrRemove;
            std::cout << "Choose to add or remove a product(add/remove): ";
            std::cin >> addOrRemove;
            try {
                if (addOrRemove == "add") {
                    AddToCart(onlineStore);
                    Display(onlineStore.database, onlineStore.electronicShoppingCart);
                } else if (addOrRemove == "remove") {
                    RemoveFromCart(onlineStore);
                    Display(onlineStore.database, onlineStore.electronicShoppingCart);
                } else {
                    throw std::invalid_argument("!= add/remove");
                }
            }
            catch (const std::exception &x) {
                std::cerr << "Cought exception: " << x.what() << '\n';
            }
}

int main() {
    OnlineStore onlineStore;

    char yesOrNo;
    do {
        try {
            AddingData(onlineStore.vendorCode, onlineStore.amount);
            onlineStore.database.insert(std::pair<std::string, int>(onlineStore.vendorCode, onlineStore.amount));
        }
        catch (const std::exception& x)
        {
            std::cerr << "Cought exception: " << x.what() << '\n';
        }

        std::cout << "Enter another item?(Y/N): ";
        std::cin >> yesOrNo;
    } while(yesOrNo != 'n' && yesOrNo != 'N');

    Display(onlineStore.database, onlineStore.electronicShoppingCart);

    bool input = true;
    while (input) {
        try {
            AddRemove(onlineStore);
            input = false;
        }
        catch (const std::exception &x) {
            std::cerr << "Cought exception: " << x.what() << '\n';
        }
    }
}