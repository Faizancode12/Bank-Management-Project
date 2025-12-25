#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits> // For numeric_limits used in error handling
using namespace std;

// variables to store account information
int accNumber[2000];
string accName[2000];
int accPin[2000];
int accBalance[2000];
int accActive[2000];
int totalAccounts = 0;

// ===== FUNCTION PROTOTYPES =====
bool saveToFile(int accounts[], string names[], int pins[], int balances[], int active[], int count);
bool loadFromFile(int accounts[], string names[], int pins[], int balances[], int active[], int &count);
bool findAccount(int accounts[], int count, int accNo, int &position);
void createAccount(int accounts[], string names[], int pins[], int balances[], int active[], int &count);
void viewAccounts(int accounts[], string names[], int balances[], int active[], int count);
void updateAccount(int accounts[], string names[], int balances[], int &count);
void deleteAccount(int accounts[], string names[], int pins[], int balances[], int active[], int &count);
void activateDeactivate(int accounts[], int &count);
bool verifyPIN(int accounts[], int pins[], int active[], int count, int accNo, int &position);
void checkBalance(int accounts[], int count);
void depositMoney(int accounts[], int &count);
void withdrawMoney(int accounts[], int &count);

// Helper function to get safe integer input
bool getInt(int &value)
{
    cin >> value;
    if(cin.fail())
    {
        cin.clear(); //resets error state 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// This function saves all accounts to a file
bool saveToFile(int accounts[], string names[], int pins[], int balances[], int active[], int count)
{
    ofstream file;
    file.open("accounts.txt");
    
    if(!file.is_open())
        {
        cout << "Error Cannot save file" << endl;
        return false;
    }
    
    for(int i = 0; i < count; i++)
    {
        file << accounts[i] << endl;
        file << names[i] << endl;
        file << pins[i] << endl;
        file << balances[i] << endl;
        file << active[i] << endl;
    }
    
    file.close();
    return true;
}

// This function loads accounts from the file
bool loadFromFile(int accounts[], string names[], int pins[], int balances[], int active[], int &count)
{
    ifstream file;
    file.open("accounts.txt");
    
    count = 0;    
    if(!file.is_open())
    {
        return false;
    }
    
    while(count < 2000 && file >> accounts[count])
    {
        file.ignore();
        getline(file, names[count]);
        file >> pins[count];
        file >> balances[count];
        file >> active[count];
        
        if(file.fail())
            break;
        
        count++;
    }
    file.close();
    return true;
}

// This function finds an account and returns true if found
bool findAccount(int accounts[], int count, int accNo, int &position)
{
    if(count <= 0)
        return false;
    
    for(int i = 0; i < count; i++)
    {
        if(accounts[i] == accNo)
        {
            position = i;
            return true;
        }
    }
    position = -1;
    return false;
}

// This function creates a new account
void createAccount(int accounts[], string names[], int pins[], int balances[], int active[], int &count)
{
    cout << "\n--- CREATE NEW ACCOUNT ---" << endl;
    
    if(count >= 2000)
    {
        cout << "Error Maximum accounts reached" << endl;
        return;
    }
    
    int accountNo;
    string name;
    string pinStr;
    int pin;
    int balance;
    int position = -1;
    
    // Get account number from user
    cout << "Enter Account Number (1000-9999): ";
    if(!getInt(accountNo))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input!" << endl;
        return;
    }
    cin.ignore();
    
    // Check if account number already exists
    if(findAccount(accounts, count, accountNo, position))
    {
        cout << "Account already exists!" << endl;
        return;
    }
    
    // Check if account number is in valid range
    if(accountNo < 1000 || accountNo > 9999)
    {
        cout << "Invalid account number!" << endl;
        return;
    }
    
    // Get name from user with validation
    while(true)
    {
        cout << "Enter Name (max 8 characters): ";
        getline(cin, name);
        
        if(name.empty())
        {
            cout << "Name cannot be empty!" << endl;
            continue;
        }
        
        if(name.length() > 8)
        {
            cout << "Name cannot exceed 8 characters!" << endl;
            continue;
        }
        
        break;
    }
    
    // Get PIN from user with validation
    while(true)
    {
        cout << "Enter PIN (digits only, max 8): ";
        getline(cin, pinStr);
        
        if(pinStr.empty())
        {
            cout << "PIN cannot be empty!" << endl;
            continue;
        }
        
        if(pinStr.length() > 8)
        {
            cout << "PIN cannot exceed 8 digits!" << endl;
            continue;
        }
        
        // Check if all characters are digits
        bool allDigits = true;
        for(char c : pinStr)
        {
            if(!isdigit(c)) //isdigit is a built in function
            {
                allDigits = false;
                break;
            }
        }
        
        if(!allDigits)
        {
            cout << "PIN must contain only digits!" << endl;
            continue;
        }
        
        pin = stoi(pinStr); //convert string into integer
        break;
    }
    
    // Get initial balance
    cout << "Enter Initial Balance:  ";
    if(!getInt(balance) || balance < 0)
    {
        cout << "Invalid balance!" << endl;
        return;
    }
    
    // Add account to arrays
    accounts[count] = accountNo;
    names[count] = name;
    pins[count] = pin;
    balances[count] = balance;
    active[count] = 1;
    
    count++;
    
    cout << "Account created successfully" << endl;
    saveToFile(accounts, names, pins, balances, active, count);
}

// This function shows all accounts
void viewAccounts(int accounts[], string names[], int balances[], int active[], int count)
{
    if(count == 0)
    {
        cout << "\nNo accounts found" << endl;
        return;
    }
    
    cout << "\n--- ALL ACCOUNTS ---" << endl;
    cout << "Account No\tName\t\tBalance\t\tStatus" << endl;

    for (int i = 0; i < count; i++)
{
    cout << left
         << setw(15) << accounts[i]
         << setw(15) << names[i]
         << setw(15) << balances[i];

    if (active[i] == 1)
        cout << setw(10) << "Active";
    else
        cout << setw(10) << "Inactive";

    cout << endl;
}
}  

// This function updates an account
void updateAccount(int accounts[], string names[], int balances[], int &count)
{
    cout << "\n--- UPDATE ACCOUNT ---" << endl;
    
    if(count == 0)
    {
        cout << "No accounts to update!" << endl;
        return;
    }
    
    int accountNo;
    cout << "Enter Account Number to update: ";
    if(!getInt(accountNo))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore();
    
    int position = -1;
    
    // Search for the account
    if(!findAccount(accounts, count, accountNo, position))
    {
        cout << "Account not found!" << endl;
        return;
    }
    
    // Show current details
    cout << "\nCurrent Details:" << endl;
    cout << "Name: " << names[position] << endl;
    cout << "Balance: " << balances[position] << endl;
    
    // Update name with validation
    while(true)
    {
        cout << "\nEnter new name (max 8 characters): ";
        getline(cin, names[position]);
        
        if(names[position].empty())
        {
            cout << "Name cannot be empty!" << endl;
            continue;
        }
        
        if(names[position].length() > 8)
        {
            cout << "Name cannot exceed 8 characters!" << endl;
            continue;
        }
        
        break;
    }
    
    // Update balance
    cout << "Enter new balance: ";
    if(!getInt(balances[position]) || balances[position] < 0)
    {
        cout << "Invalid balance!" << endl;
        return;
    }
    
    cout << "Account updated successfully!" << endl;
    saveToFile(accounts, names, accPin, balances, accActive, count);
}

// This function deletes an account
void deleteAccount(int accounts[], string names[], int pins[], int balances[], int active[], int &count)
{
    cout << "\n--- DELETE ACCOUNT ---" << endl;
    
    if(count == 0)
    {
        cout << "No accounts to delete!" << endl;
        return;
    }
    
    int accountNo;
    cout << "Enter Account Number to delete: ";
    if(!getInt(accountNo))
        return;
    
    int position = -1;
    
    // Search for the account
    if(!findAccount(accounts, count, accountNo, position))
    {
        cout << "Account not found!" << endl;
        return;
    }
    
    // Move all accounts after this one forward by one position
    for(int i = position; i < count - 1; i++)
    {
        accounts[i] = accounts[i + 1];
        names[i] = names[i + 1];
        pins[i] = pins[i + 1];
        balances[i] = balances[i + 1];
        active[i] = active[i + 1];
    }
    
    count--;
    
    cout << "Account deleted successfully!" << endl;
    saveToFile(accounts, names, pins, balances, active, count);
}

// This function activates or deactivates an account
void activateDeactivate(int accounts[], int &count)
{
    cout << "\n--- ACTIVATE/DEACTIVATE ACCOUNT ---" << endl;
    
    if(count == 0)
    {
        cout << "No accounts to modify!" << endl;
        return;
    }
    
    int accountNo;
    cout << "Enter Account Number: ";
    if(!getInt(accountNo))
        return;
    
    int position = -1;
    
    // Search for the account
    if(!findAccount(accounts, count, accountNo, position))
    {
        cout << "Account not found!" << endl;
        return;
    }
    
    // active status
    if(accActive[position] == 1)
    {
        accActive[position] = 0;
        cout << "Account is now inactive!" << endl;
    }
    else
    {
        accActive[position] = 1;
        cout << "Account is now active!" << endl;
    }
    
    saveToFile(accNumber, accName, accPin, accBalance, accActive, count);
}

// This function verifies PIN and returns true if correct
bool verifyPIN(int accounts[], int pins[], int active[], int count, int accNo, int &position)
{
    if(count <= 0)
    {
        cout << "No accounts available!" << endl;
        return false;
    }
    
    int pin;
    
    // Search for account
    if(!findAccount(accounts, count, accNo, position))
    {
        cout << "Account not found!" << endl;
        return false;
    }
    
    // Check if account is active
    if(active[position] == 0)
    {
        cout << "This account is inactive!" << endl;
        return false;
    }
    
    // Get PIN
    cout << "Enter PIN: ";
    if(!getInt(pin))
        return false;
    
    // Verify PIN
    if(pins[position] != pin)
    {
        cout << "Wrong PIN!" << endl;
        return false;
    }
    
    return true;
}

// This function checks balance
void checkBalance(int accounts[], int count)
{
    cout << "\n--- CHECK BALANCE ---" << endl;
    
    if(count == 0)
    {
        cout << "No accounts available!" << endl;
        return;
    }
    
    int accountNo;
    cout << "Enter Account Number: ";
    if(!getInt(accountNo))
        return;
    
    int position = -1;
    
    if(!verifyPIN(accounts, accPin, accActive, count, accountNo, position))
    {
        return;
    }
    
    // Show balance
    cout << "\nAccount Holder: " << accName[position] << endl;
    cout << "Balance: " << accBalance[position] << endl;
}

// This function deposits money
void depositMoney(int accounts[], int &count)
{
    cout << "\n--- DEPOSIT MONEY ---" << endl;
    
    if(count == 0)
    {
        cout << "No accounts available!" << endl;
        return;
    }
    
    int accountNo;
    cout << "Enter Account Number: ";
    if(!getInt(accountNo))
        return;
    
    int position = -1;
    
    if(!verifyPIN(accounts, accPin, accActive, count, accountNo, position))
    {
        return;
    }
    
    // Get amount to deposit
    int amount;
    cout << "Enter amount to deposit: ";
    if(!getInt(amount) || amount <= 0)
    {
        cout << "Amount must be positive!" << endl;
        return;
    }
    
    // Add amount to balance
    accBalance[position] = accBalance[position] + amount;
    
    cout << "Deposit successful!" << endl;
    cout << "New Balance: " << accBalance[position] << endl;
    
    saveToFile(accNumber, accName, accPin, accBalance, accActive, count);
}

// This function withdraws money
void withdrawMoney(int accounts[], int &count)
{
    cout << "\n--- WITHDRAW MONEY ---" << endl;
    
    if(count == 0)
    {
        cout << "No accounts available!" << endl;
        return;
    }
    
    int accountNo;
    cout << "Enter Account Number: ";
    if(!getInt(accountNo))
        return;
    
    int position = -1;
    
    if(!verifyPIN(accounts, accPin, accActive, count, accountNo, position))
    {
        return;
    }
    
    // Get amount to withdraw
    int amount;
    cout << "Enter amount to withdraw: ";
    if(!getInt(amount) || amount <= 0)
    {
        cout << "Amount must be positive!" << endl;
        return;
    }
    
    // Check if balance is enough
    if(amount > accBalance[position])
    {
        cout << "Insufficient balance!" << endl;
        cout << "Your balance: " << accBalance[position] << endl;
        return;
    }
    
    // Subtract amount from balance
    accBalance[position] = accBalance[position] - amount;
    
    cout << "Withdrawal successful!" << endl;
    cout << "New Balance: " << accBalance[position] << endl;
    
    saveToFile(accNumber, accName, accPin, accBalance, accActive, count);
}

// This is the main function  where program starts
int main()
{
    loadFromFile(accNumber, accName, accPin, accBalance, accActive, totalAccounts);
    int choice;
    while(true)
    {
        cout << "\n===== BANK MANAGEMENT SYSTEM =====" << endl;
        cout << "1. Admin\n2. Manager\n3. User\n4. Exit\nChoice: ";
        if(!getInt(choice))
        {
            cout << "Invalid input! Please enter a number.";
            continue;
        }
        
        // ===== ADMIN SECTION =====
        if(choice == 1)
        {
            system("cls");
            int ch;
            cout << "\n1.Create 2.View 3.Update 4.Delete 5.Back\nChoice: ";
            
            if(!getInt(ch))
            {
                cout << "Invalid input!" << endl;
                continue;
            }
            
            if(ch == 1)
                createAccount(accNumber, accName, accPin, accBalance, accActive, totalAccounts);
            else if(ch == 2)
                viewAccounts(accNumber, accName, accBalance, accActive, totalAccounts);
            else if(ch == 3)
                updateAccount(accNumber, accName, accBalance, totalAccounts);
            else if(ch == 4)
                deleteAccount(accNumber, accName, accPin, accBalance, accActive, totalAccounts);
        }
        
        // ===== MANAGER SECTION =====
        else if(choice == 2)
        {
            system("cls");
            int ch;
            cout << "\n1.View 2.Activate/Deactivate 3.Back\nChoice: ";
            
            if(!getInt(ch))
            {

                cout << "Invalid input!" << endl;
                continue;
            }
            
            if(ch == 1)
                viewAccounts(accNumber, accName, accBalance, accActive, totalAccounts);
            else if(ch == 2)
                activateDeactivate(accNumber, totalAccounts);
        }
        
        // ===== USER SECTION =====
        else if(choice == 3)
        {
            system("cls");
            int ch;
            cout << "\n1.Check Balance 2.Deposit 3.Withdraw 4.Back\nChoice: ";
            
            if(!getInt(ch))
            {
                cout << "Invalid input!" << endl;
                continue;
            }
            
            if(ch == 1)
                checkBalance(accNumber, totalAccounts);
            else if(ch == 2)
                depositMoney(accNumber, totalAccounts);
            else if(ch == 3)
                withdrawMoney(accNumber, totalAccounts);
        }
        
        // ===== EXIT =====
        else if(choice == 4)
        {
            cout << "Thank you for using Bank System!" << endl;
            break;
        }
        else
        {
            cout << "Invalid choice!" << endl;
        }
    }
    
    return 0;
}