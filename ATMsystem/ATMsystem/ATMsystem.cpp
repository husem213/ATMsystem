#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>

using namespace std;
static string ClientsData = "ClientsData.txt";
void ATMmainMenuScreen();
void Login();

enum enMainMenuChoices
{
    eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5
};

enum enQuickWithdrawChoices
{
    Twenty = 1, Fifty = 2, OneHundred = 3, TwoHundred = 4, FourHundred = 5,
    SixHundred = 6, EightHundred = 7, OneThousand = 8, Exit = 9
};

struct stClient
{
    string AccountNumber, PinCode, Name, Phone;
    float AccountBalance = 0;
};

stClient CurrentClient;

void LoginScreen()
{
    system("cls");
    cout << "\n================================================\n";
    cout << "\t\tLogin Screen\n";
    cout << "================================================\n";
}

stClient ReadAccNumberAndPinCode()
{
    stClient Client;

    cout << "\nEnter Account Number: ";
    getline(cin >> ws, Client.AccountNumber);

    cout << "\nEnter Pin Code: ";
    getline(cin, Client.PinCode);

    return Client;
}

vector <string> SplitString(string S, string Delimeter)
{
    vector <string> vWords;
    short DelimPosition;
    string Word;

    while ((DelimPosition = S.find(Delimeter)) != string::npos)
    {
        Word = S.substr(0, DelimPosition);
        if (Word != "")
            vWords.push_back(Word);

        S.erase(0, DelimPosition + Delimeter.length());
    }

    if (S != "")
        vWords.push_back(S);

    return vWords;
}

stClient ConvertLineToClientRecord(string LineOfData, string Dilemeter = "#//#")
{
    stClient Client;
    vector <string> vClientData = SplitString(LineOfData, Dilemeter);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stoi(vClientData[4]);

    return Client;
}

string ConvertClientRecordToLine(stClient Client, string Dilemeter = "#//#")
{
    string ClientData = "";

    ClientData += Client.AccountNumber + Dilemeter;
    ClientData += Client.PinCode + Dilemeter;
    ClientData += Client.Name + Dilemeter;
    ClientData += Client.Phone + Dilemeter;
    ClientData += to_string(Client.AccountBalance);

    return ClientData;
}

void SaveClientsVectorToFile(vector <stClient>& vClients)
{
    fstream MyFile;
    MyFile.open(ClientsData, ios::out);

    if (MyFile.is_open())
    {
        for (stClient& C : vClients)
        {
            MyFile << ConvertClientRecordToLine(C) << endl;
        }
        MyFile.close();
    }
}

vector <stClient> LoadClientsinVector()
{
    fstream MyFile;
    vector <stClient> vClientsData;
    string ClientDataLine;
    MyFile.open(ClientsData, ios::in);

    if (MyFile.is_open())
    {
        while (getline(MyFile, ClientDataLine))
            vClientsData.push_back(ConvertLineToClientRecord(ClientDataLine));

        MyFile.close();
    }
    return vClientsData;
}

bool FindClientByAccNumberAndPinCode(stClient Client1, stClient& Client2)
{
    vector <stClient> vClients = LoadClientsinVector();

    for (stClient& C : vClients)
    {
        if (C.AccountNumber == Client1.AccountNumber && C.PinCode == Client1.PinCode)
        {
            Client2 = C;
            return true;
        }
    }
    return false;
}

void CheckBalanceScreen()
{
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "==================================================\n";
    cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;
}

void BackToMainMenuScreen()
{
    cout << "\nPress any botton to back to main menu...";
    system("pause>0");
    ATMmainMenuScreen();
}

short QuickWithdrawScreen()
{
    short Choice = 0;
    system("cls");
    cout << "===================================================\n";
    cout << "\t\tQuick Withdraw\n";
    cout << "===================================================\n";
    cout << "\t[1] 20         [2] 50\n";
    cout << "\t[3] 100        [4] 200\n";
    cout << "\t[5] 400        [6] 600\n";
    cout << "\t[7] 800        [8] 1000\n";
    cout << "\t[9] EXIT\n";
    cout << "===================================================\n";
    cout << "Your balance is: " << CurrentClient.AccountBalance << endl;
    cout << "Choose the amount [1 to 9]: ";
    cin >> Choice;
    return Choice;
}

short TransformEnumToAmount(enQuickWithdrawChoices Choice)
{
    switch (Choice)
    {
    case Twenty:
        return 20;
    case Fifty:
        return 50;
    case OneHundred:
        return 100;
    case TwoHundred:
        return 200;
    case FourHundred:
        return 400;
    case SixHundred:
        return 600;
    case EightHundred:
        return 800;
    case OneThousand:
        return 1000;
    }
}

void QuickWithdraw()
{
    enQuickWithdrawChoices Choice = (enQuickWithdrawChoices)QuickWithdrawScreen();
    short Amount = TransformEnumToAmount(Choice);
    char Perform = 'n';

    if (Choice == Exit)
        return;

    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nAmount exceeds your balance. Enter another amount.";
        return;
    }

    cout << "\nAre you sure you want to perform this transaction? [y/n]: ";
    cin >> Perform;

    if (toupper(Perform) != 'Y')
        return;

    CurrentClient.AccountBalance -= Amount;
    cout << "\nDone successfully! New balance is: " << CurrentClient.AccountBalance;
}

int NormalWithdrawScreen()
{
    int Amount = 0;
    system("cls");
    cout << "===================================================\n";
    cout << "\t\tNormal Withdraw\n";
    cout << "===================================================\n";

    do
    {
        cout << "\nEnter an amount that is multiple of 5: ";
        cin >> Amount;
    } while (Amount % 5 != 0);

    return Amount;
}

void NormalWithdraw()
{
    int Amount = NormalWithdrawScreen();
    char Perform = 'n';

    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nAmount exceeds your balance. Enter another amount.";
        return;
    }

    cout << "\nAre you sure you want to perform this transaction? [y/n]: ";
    cin >> Perform;

    if (toupper(Perform) != 'Y')
        return;

    CurrentClient.AccountBalance -= Amount;

    cout << "\nDone successfully! New balance is: " << CurrentClient.AccountBalance;
}

int DepositScreen()
{
    int Amount = 0;
    system("cls");
    cout << "===================================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===================================================\n";

    do
    {
        cout << "\nEnter a positive amount: ";
        cin >> Amount;
    } while (Amount <= 0);

    return Amount;
}

void Deposit()
{
    int Amount = DepositScreen();
    char Perform = 'n';

    cout << "\nAre you sure you want to preform this transaction? [y/n]: ";
    cin >> Perform;

    if (toupper(Perform) != 'Y')
        return;

    CurrentClient.AccountBalance += Amount;
    cout << "\nDone successfully! New balance is: " << CurrentClient.AccountBalance;
}

void SaveClientData()
{
    vector <stClient> vClients = LoadClientsinVector();
    stClient Client;

    for (stClient& C : vClients)
    {
        if (CurrentClient.AccountNumber == C.AccountNumber)
            C = CurrentClient;
    }

    SaveClientsVectorToFile(vClients);
}

void PerformMainMenuChoice(enMainMenuChoices Choice)
{
    switch (Choice)
    {
    case eQuickWithdraw:
        QuickWithdraw();
        SaveClientData();
        BackToMainMenuScreen();
        break;
    case eNormalWithdraw:
        NormalWithdraw();
        SaveClientData();
        BackToMainMenuScreen();
        break;
    case eDeposit:
        Deposit();
        SaveClientData();
        BackToMainMenuScreen();
        break;
    case eCheckBalance:
        CheckBalanceScreen();
        BackToMainMenuScreen();
        break;
    case eLogout:
        Login();
        break;
    }
}

short ReadMainMenuChoice()
{
    short Choice = 0;
    do
    {
        cout << "Choose what do you want to do [1 to 5]: ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 5);

    return Choice;
}

void ATMmainMenuScreen()
{
    system("cls");
    cout << "=====================================================\n";
    cout << "\t\tATM Manin Menu Screen\n";
    cout << "=====================================================\n";
    cout << "\t[1] Quick Withdraw\n";
    cout << "\t[2] Normal Withdraw\n";
    cout << "\t[3] Deposit\n";
    cout << "\t[4] Check Balance\n";
    cout << "\t[5] Logout\n";
    cout << "=====================================================\n";
    PerformMainMenuChoice((enMainMenuChoices)ReadMainMenuChoice());
}

void Login()
{
    stClient Client;

    LoginScreen();
    Client = ReadAccNumberAndPinCode();

    while (!FindClientByAccNumberAndPinCode(Client, CurrentClient))
    {
        LoginScreen();
        cout << "\nInvalid Account Number/Password!";
        Client = ReadAccNumberAndPinCode();
    }
    ATMmainMenuScreen();
}

int main()
{
    Login();
    return 0;
}