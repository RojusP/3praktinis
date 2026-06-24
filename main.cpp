#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const int MAX_ITEMS = 20;

struct menuItemType {
    string menuItem;
    double menuPrice;
};

// Nuskaityti meniu
void getData(menuItemType menuList[], int &size) {
    ifstream inFile("menu.txt");

    if (!inFile) {
        cout << "Nepavyko atidaryti menu.txt!\n";
        exit(1);
    }

    size = 0;

    while (inFile >> menuList[size].menuItem >> menuList[size].menuPrice) {
        for (char &c : menuList[size].menuItem) {
            if (c == '_') c = ' ';
        }
        size++;
    }

    inFile.close();
}

// Rodyti meniu
void showMenu(menuItemType menuList[], int size) {
    cout << "\n----- MENIU -----\n";

    for (int i = 0; i < size; i++) {
        cout << i + 1 << ". "
             << setw(40) << left << menuList[i].menuItem
             << fixed << setprecision(2)
             << menuList[i].menuPrice << " €\n";
    }

    cout << "\n0 - Baigti užsakymą\n";
}

// Spausdinti sąskaitą
void printCheck(menuItemType menuList[], int orderQty[], int size) {
    ofstream outFile("receipt.txt");

    double subtotal = 0;

    cout << "\n--- SĄSKAITA ---\n";
    outFile << "--- SĄSKAITA ---\n";

    for (int i = 0; i < size; i++) {
        if (orderQty[i] > 0) {
            double suma = orderQty[i] * menuList[i].menuPrice;
            subtotal += suma;

            cout << orderQty[i] << " "
                 << setw(35) << left << menuList[i].menuItem
                 << fixed << setprecision(2)
                 << suma << " €\n";

            outFile << orderQty[i] << " "
                    << setw(35) << left << menuList[i].menuItem
                    << suma << " €\n";
        }
    }

    double pvm = subtotal * 0.21;
    double total = subtotal + pvm;

    cout << "\nPVM (21%): " << fixed << setprecision(2) << pvm << " €\n";
    cout << "Galutinė suma: " << total << " €\n";

    outFile << "\nPVM (21%): " << pvm << " €\n";
    outFile << "Galutinė suma: " << total << " €\n";

    outFile.close();
}

// MAIN
int main() {

    menuItemType menuList[MAX_ITEMS];
    int size;

    int orderQty[MAX_ITEMS] = {0};

    getData(menuList, size);

    cout << "Sveiki atvykę į restoraną!\n";

    int choice;

    while (true) {

        showMenu(menuList, size);

        cout << "\nPasirinkimas: ";
        cin >> choice;

        // Jei bloga įvestis
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Klaida! Įveskite skaičių.\n";
            continue;
        }

        // ✅ SVARBIAUSIA DALIS (FIX)
        if (choice == 0) {
            cout << "Užsakymas baigtas!\n";
            break;
        }

        if (choice < 1 || choice > size) {
            cout << "Tokio pasirinkimo nėra!\n";
            continue;
        }

        int qty;

        // kiekio tikrinimas
        while (true) {
            cout << "Kiekis: ";
            cin >> qty;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Įveskite skaičių!\n";
            }
            else if (qty <= 0) {
                cout << "Kiekis turi būti didesnis nei 0!\n";
            }
            else {
                break;
            }
        }

        orderQty[choice - 1] += qty;
    }

    printCheck(menuList, orderQty, size);

    return 0;
}