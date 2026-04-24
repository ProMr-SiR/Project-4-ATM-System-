#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";

void ShowATMMainMenueScreen();
double GetClientBalance();
void GoBackToATMMainMenue();
void Login();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient CurrentClient;
sClient AccountBalance;

vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable

    // use find() function to get the position of the delimiters
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length()); /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]); // cast string to double

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

vector<sClient> LoadCleintsDataFromFile(string FileName)
{

    vector<sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in); // read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();
    }

    return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, string Pincode, sClient& Client)
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == Pincode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool FindClientByAccountNumber(sClient& Client)
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient C : vClients)
    {
        if (C.AccountNumber == Client.AccountNumber)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out); // overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                // we only write records that are not marked for delete.
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }

    return vClients;
}

//####################################[{3}Deposit Screen]###################################

bool QuickWithdrawBalanceToClient(sClient& Client, double QuickWithdrawChosen, vector<sClient>& vClients)
{
    char Answer = 'n';

    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (FindClientByAccountNumber(CurrentClient))
            {
                C.AccountBalance -= QuickWithdrawChosen;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
        return false;
    }
    return false;
}

double ReadDeposit()
{
    double Deposit;
    cout << "\nEnter an posative Deposit Amount? ";
    cin >> Deposit;
    return Deposit;
}

void ShowDepositScreen()
{
    cout << "\n=====================================================\n";
    cout << "\t\tDeposit Screen";
    cout << "\n=====================================================\n";
    sClient Client;
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    double DepositAmount;
    do
    {
        DepositAmount = ReadDeposit();
    } while (DepositAmount < 0);

    QuickWithdrawBalanceToClient(Client, DepositAmount * -1, vClients);
}

//####################################[{2}Quick Withdraw Screen]###################################

double ReadNormalWithdrow()
{
    double NormalWithd;
    cout << "\nEnter an amount multipale of 5's ? ";
    cin >> NormalWithd;
    return NormalWithd;
}

bool isMultipileOf5(int NormalWithd)
{
    return NormalWithd % 5 == 0;
}

void ShowNormalWithdrawScreen()
{
    cout << "\n=====================================================\n";
    cout << "\t\tNormal Withdraw Screen";
    cout << "\n=====================================================\n";
    sClient Client;
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    double NormalWithdrow;
    bool AmountFaild = false;
    bool ExceedAmount = false;
    do
    {
        NormalWithdrow = ReadNormalWithdrow();

        if (NormalWithdrow > GetClientBalance())
        {
            ExceedAmount = true;
            cout << "\nThe Amount exceeds your balance " << "[" << GetClientBalance() << "]." << endl;
            cout << "Press Any Key to countinue...\n";
            system("pause>0");
        }
        
        else
        {
            ExceedAmount = false;
        }

        AmountFaild = !isMultipileOf5(NormalWithdrow);
    } while (AmountFaild || ExceedAmount);

    QuickWithdrawBalanceToClient(Client, NormalWithdrow, vClients);
}

//####################################[{1}Quick Withdraw Screen]###################################

short ReadQuickWithdrawOption()
{
    cout << "Choose what do t0 withdrow  from[1] to [8] ? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

enum enQuickWithdrawOptions
{
    e20 = 1,
    e50 = 2,
    e100 = 3,
    e200 = 4,
    e400 = 5,
    e600 = 6,
    e800 = 7,
    e1000 = 8,
    eExit = 9
};

int PerfromenQuickWithdrawOptions(enQuickWithdrawOptions QuickWithdrawOptions)
{
    switch (QuickWithdrawOptions)
    {
    case enQuickWithdrawOptions::e20: 
        return 20;
        break;

    case enQuickWithdrawOptions::e50: return 50;

    case enQuickWithdrawOptions::e100: return 100;

    case enQuickWithdrawOptions::e200: return 200;

    case enQuickWithdrawOptions::e400: return 400;

    case enQuickWithdrawOptions::e600: return 600;

    case enQuickWithdrawOptions::e800: return 800;

    case enQuickWithdrawOptions::e1000: return 1000;

    case enQuickWithdrawOptions::eExit:
    {
        ShowATMMainMenueScreen();
        break;
    }
    }
}

void ShowQuickWithdrawScreen()
{
    cout << "\n=====================================================\n";
    cout << "\t\tQuick Withdraw";
    cout << "\n=====================================================\n";
    cout << "\t[1] 20             [2] 50" << endl;
    cout << "\t[3] 100            [4] 200" << endl;
    cout << "\t[5] 400            [6] 600" << endl;
    cout << "\t[7] 800            [8] 1000" << endl;
    cout << "\t[9] Exit ";
    cout << "\n====================================================\n";
    cout << "Your Balance is : " << GetClientBalance() << endl;
    sClient Client;
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    int QuickWithdrawOptions;

    bool ExceedAmount = false;
    do
    {
        QuickWithdrawOptions = PerfromenQuickWithdrawOptions((enQuickWithdrawOptions)ReadQuickWithdrawOption());
        
        if (QuickWithdrawOptions > GetClientBalance())
        {
            cout << "\nThe Amount exceeds your balance " << "[" << GetClientBalance() << "]." << endl;
            cout << "Press Any Key to countinue...\n";
            system("pause>0");
            ExceedAmount = true;
        }
        
        else
        {
            ExceedAmount = false;
        }

    } while (ExceedAmount);

    QuickWithdrawBalanceToClient(Client, QuickWithdrawOptions, vClients);
}

//####################################[{4}Check Balance Screen]###################################

double GetClientBalance()
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    double TotalBalances = 0;
    for (sClient& Client : vClients)
    {
        if (FindClientByAccountNumber(CurrentClient))
            return Client.AccountBalance;
    }
}

void ShowCheckBalanceScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tCheck Balance Screen";
    cout << "\n-----------------------------------\n";
    cout << "Your Balance is : " << GetClientBalance();
}

//####################################[ATM Screen]###################################

enum enATMMainMenueOptions
{
    eQuickWithdraw = 1,
    eNormalWithdraw = 2,
    eDeposit = 3,
    eCheckBalances = 4,
    eLogout = 5
};

void GoBackToATMMainMenue()
{
    cout << "\n\nPress any key to go back to ATM Main Menue...";
    system("pause>0");
    ShowATMMainMenueScreen();
}

void PerfromATMMainMenueOption(enATMMainMenueOptions ATMMenueOptions)
{
    switch (ATMMenueOptions)
    {
    case enATMMainMenueOptions::eQuickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToATMMainMenue();
        break;
    }

    case enATMMainMenueOptions::eNormalWithdraw:
    {
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToATMMainMenue();
        break;
    }

    case enATMMainMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToATMMainMenue();
        break;
    }

    case enATMMainMenueOptions::eCheckBalances:
    {
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToATMMainMenue();
        break;
    }

    case enATMMainMenueOptions::eLogout:
    {
        system("cls");
        Login();
        break;
    }
    }
}

short ReadATMMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void ShowATMMainMenueScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balances.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromATMMainMenueOption((enATMMainMenueOptions)ReadATMMainMenueOption());
}

//####################################[Login]#######################################

void ShowLoginScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tLogin Screen";
    cout << "\n-----------------------------------\n";
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    return FindClientByAccountNumber(AccountNumber, PinCode, CurrentClient);
}

void Login()
{

    string AccountNumber, PinCode;

    bool LoginFaild = false;

    do
    {
        system("cls");
        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
            cout << "Invlaid AccountNumber/PinCode!\n";

        cout << "Enter AccountNumber? ";
        cin >> AccountNumber;

        cout << "Enter PinCode? ";
        cin >> PinCode;

        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFaild);

    ShowATMMainMenueScreen();
}

int main()
{
    Login();
    return 0;
}
