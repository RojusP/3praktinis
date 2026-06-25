//praktinis3 RojusPetravicius
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
             << menuList[i].menuPrice << " EUR\n";
    }

    cout << "\n0 - Baigti uzsakyma\n";
}

// Spausdinti saskaita
void printCheck(menuItemType menuList[], int orderQty[], int size) {
    ofstream outFile("receipt.txt");

    double subtotal = 0;

    cout << "\nSASKAITA: \n";
    outFile << "SASKAITA: \n";

    for (int i = 0; i < size; i++) {
        if (orderQty[i] > 0) {
            double suma = orderQty[i] * menuList[i].menuPrice;
            subtotal += suma;

            cout << orderQty[i] << " "
                 << setw(35) << left << menuList[i].menuItem
                 << fixed << setprecision(2)
                 << suma << " EUR\n";

            outFile << orderQty[i] << " "
                    << setw(35) << left << menuList[i].menuItem
                    << suma << " EUR\n";
        }
    }

    double pvm = subtotal * 0.21;
    double total = subtotal + pvm;

    cout << "\nPVM (21%): " << fixed << setprecision(2) << pvm << " EUR\n";
    cout << "Galutine suma: " << total << " EUR\n";

    outFile << "\nPVM (21%): " << pvm << " EUR\n";
    outFile << "Galutine suma: " << total << " EUR\n";

    outFile.close();
}

// MAIN
int main() {

    menuItemType menuList[MAX_ITEMS];
    int size;

    int orderQty[MAX_ITEMS] = {0};

    getData(menuList, size);

    cout << "Sveiki atvyke i restorana!\n";

    int choice;

    while (true) {

        showMenu(menuList, size);

        cout << "\nPasirinkimas: ";
        cin >> choice;

        // Jei bloga ivestis
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Klaida! Iveskite skaiciu.\n";
            continue;
        }

        if (choice == 0) {
            cout << "Uzsakymas baigtas!\n";
            break;
        }

        if (choice < 1 || choice > size) {
            cout << "Tokio pasirinkimo nera!\n";
            continue;
        }

        int qty;

        // Kiekio tikrinimas
        while (true) {
            cout << "Kiekis: ";
            cin >> qty;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Iveskite skaiciu!\n";
            }
            else if (qty <= 0) {
                cout << "Kiekis turi buti didesnis nei 0!\n";
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
