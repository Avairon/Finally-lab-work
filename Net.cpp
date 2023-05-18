#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;;

class Category {
public:
    int id;
    string name;

    Category(int ID, string Name) {
        id = ID;
        this->name = Name;
    }
};
class Balance {
public:
    int id, balance;
    
    Balance(int ID, int Balance) {
        this->id = ID;
        this->balance = Balance;
    }
};

void write(string input) {
    ofstream Log;

    Log.open("Logs.txt", ios::app);

    if (Log.is_open()) {
        Log << input << "\n";
    }
    Log.close();
}

string read(int line) {
    string out;
    ifstream Log;
        Log.open("Logs.txt");

    for (int i = 0; i < line; i++) {
        Log >> out;
    }

    if (Log.eof()) return "";

    Log.close();

    return out;
}
int allPaymentsFM(int input, int a, int b) {
    int i = 1;
    int sumIndustry = 0;

    while (read(i) != "") {
        if (read(i) == "payment") {
            if (read(i + 1) == to_string(input) && read(i + 3) == to_string(a) && read(i + 4) == to_string(b)) {
                sumIndustry += stoi(read(i + 2));
                i += 5;
            }
            else i += 5;
        }
        else i += 5;
    }
    return sumIndustry;
}
int allPayments(int input, int a) {
    int i = 1;
    int sumIndustry = 0;

    while (read(i) != "") {
        if (read(i) == "payment") {
            if (read(i + 1) == to_string(input) && read(i + 3) == to_string(a)) {
                sumIndustry += stoi(read(i + 2));
                i += 5;
            }
            else i += 5;
        }
        else i += 5;
    }
    return sumIndustry;
}

float prcnt(int a, int b) {
    return ((float)b / (float)a) * 100.00;
}

Balance balances[2]{ *new Balance(0, 1000), *new Balance(1, 500) };
Category category[4]{ *new Category(0, "food"), *new Category(1, "receipts"), *new Category(2, "other"), *new Category(3, "study") };

//###################################
// Формат в главном логе:
// Тип операции
// No Балланса
// Кол-во средств
// Область траты(Вариативно)
// Дата(No Месяца) момента операции
//###################################

int main()
{
    time_t now = time(0);
    tm* ltm = localtime(&now);

    int input = 1, money, industry;

    while (input != 0) {
        cout << "Choose action: 1-check balance, 2-top up, 3-pay anything, 4-payments for last mount, 0-exit\n";
        cin >> input;

        switch (input) {
        case(1): //проверка баланса
            cout << "Sum all of you balances = " << balances[0].balance + balances[1].balance << "\n";

            for (int i = 0; i < 2; i++) {
                cout << "Balance #" << i + 1 << " money: " << balances[i].balance << "\n";
            }

            break;
        case(2): //пополнение счёта
            cout << "you have: \n";

            for(int i = 0; i < 2; i++) {
                cout << "Balance #" << i + 1 << " money: " << balances[i].balance << "\n";
            }

            cout << "choose balance\n";
            cin >> input;
            cout << "how many?\n";
            cin >> money;
            balances[input - 1].balance += money;

            write("topup");
            write(to_string(input));
            write(to_string(money));
            write("");
            write(to_string(1 + ltm->tm_mon));


            break; 
        case(3): //оплата счёта
            for (int i = 0; i < 2; i++) {
                cout << "Balance #" << i + 1 << " money: " << balances[i].balance << "\n";
            }

            cout << "choose balance\n";
            cin >> input;

            for (int i = 0; i < 4; i++) {
                cout << category[i].id + 1 << " " << category[i].name << "\n";
            }

            cout << "payment for which industry?\n";
            cin >> industry;
            cout << "how many?\n";
            cin >> money;

            balances[input - 1].balance -= money;

            write("payment");
            write(to_string(input));
            write(to_string(money));
            write(to_string(industry));
            write(to_string(1 + ltm->tm_mon));

            break;
        case(4): //платежи за последний месяц
            for (int i = 0; i < 2; i++) {
                cout << "Balance #" << i + 1 << " money: " << balances[i].balance << "\n";
            }

            cout << "choose balance\n";
            cin >> input;

            int a;
            int b;

            cout << "number of mounth?\n";
            cin >> b;

            int allFood = allPaymentsFM(input, 1, b); // map ключ - значение
            int allReceipts = allPaymentsFM(input, 2, b);
            int allOther = allPaymentsFM(input, 3, b);
            int allStudy = allPaymentsFM(input, 4, b);

            int sumPayments = allFood + allReceipts + allOther + allStudy;

            cout << "\npayments for mount\n";
            cout << "1 " << category[0].name << " - " << round(prcnt(sumPayments, allFood) * 100) / 100 << "%" << "\n";
            cout << "2 " << category[1].name << " - " << round(prcnt(sumPayments, allReceipts) * 100) / 100 << "%" << "\n";
            cout << "3 " << category[2].name << " - " << round(prcnt(sumPayments, allOther) * 100) / 100 << "%" << "\n";
            cout << "4 " << category[3].name << " - " << round(prcnt(sumPayments, allStudy) * 100) / 100 << "%" <<"\n";

            allFood = allPaymentsFM(input, 1, b);
            allReceipts = allPaymentsFM(input, 2, b);
            allOther = allPaymentsFM(input, 3, b);
            allStudy = allPaymentsFM(input, 4, b);

            sumPayments = allFood + allReceipts + allOther + allStudy;

            cout << "\npayments for all time\n";
            cout << "1 " << category[0].name << " - " << round(prcnt(sumPayments, allFood) * 100) / 100 << "%" << "\n";
            cout << "2 " << category[1].name << " - " << round(prcnt(sumPayments, allReceipts) * 100) / 100 << "%" << "\n";
            cout << "3 " << category[2].name << " - " << round(prcnt(sumPayments, allOther) * 100) / 100 << "%" << "\n";
            cout << "4 " << category[3].name << " - " << round(prcnt(sumPayments, allStudy) * 100) / 100 << "%" << "\n";

            break;
        }
    }
}
