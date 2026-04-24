#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";

void ShowMainMenue();
void ShowQuickWithdrawMenu();
void ShowNormalWithdrawScreen();
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

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient &CurrentClient)
{

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient &C : vClients)
    {

        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            CurrentClient = C;
            return true;
        }
    }
    return false;
}

bool LoadClientInfo(string Username, string Password)
{
    return FindClientByAccountNumberAndPinCode(Username, Password, CurrentClient);
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}

enum enMainMenueOptions
{
    eQuickWithdraw = 1,
    eNormalWithdraw = 2,
    eDeposit = 3,
    eCheckBalance = 4,
    eExit = 5,
};

short ReadQuickWithdrawOption()
{
    short Choice = 0;
    while (Choice < 1 || Choice > 9)
    {
        cout << "\nChoose what to do from [1] to [9] ? ";
        cin >> Choice;
    }

    return Choice;
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

vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out); // overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient &C : vClients)
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

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<sClient> &vClients)
{
    char Answer = 'n';

    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient &C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }
        }

        return false;
    }
    return false;
}

short getQuickWithDrawAmount(short QuickWithDrawOption)
{
    switch (QuickWithDrawOption)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}

void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{

    if (QuickWithDrawOption == 9)
        return;

    short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption);

    if (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance ,make another choice";
        cout << "\n\nPress any key to continue...";
        system("pause>0");
        ShowQuickWithdrawMenu();
        return;
    }

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);

    CurrentClient.AccountBalance -= WithDrawBalance;
}

void ShowQuickWithdrawMenu()
{
    cout << "===========================================\n";
    cout << "\t\tQucik Withdraw\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance;

    PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}

int ReadWithDrawAmount()
{
    int Amount;

    do
    {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> Amount;
    } while (Amount % 5 != 0);

    return Amount;
}

void PerfromNormalWithdrawOption()
{
    int WithDrawBalance = ReadWithDrawAmount();

    if (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance ,make another choice";
        cout << "\n\nPress any key to continue...";
        system("pause>0");
        ShowNormalWithdrawScreen();
        return;
        /*
            How does the program handle this code?
            When the client enters an amount greater than their balance 3 times, this is what happens behind the scenes:

            1. First Attempt (Error):
               - The program enters the NormalWithdraw() function.
               - It detects that the amount exceeds the balance.
               - It calls the GoBackToShowNormalWithdrawScreen() function.
               - IMPORTANT NOTE: At this moment, the first function hasn't finished yet!
                 The program pauses at this line and leaves the first "return;" command in memory (waiting for its turn).

            2. Second Attempt (Error):
               - The GoBack function opens the menu again, which in turn calls NormalWithdraw()
                 as a brand-new attempt opened on top of the old one.
               - The client makes a mistake again, so the program calls GoBack again.
               - Now we have a second "return;" pending in memory.

            3. Third Attempt (Error):
               - A third screen opens.
               - The client makes a mistake, a fourth menu opens, and we have a third "return;" pending in memory.

            4. Fourth Attempt (Success):
               - If the client finally enters a correct amount, the deduction is made, and the fourth function finishes successfully.

            Stack Unwinding Phase:
            Once the fourth operation succeeds and finishes, the computer starts going backward to clean the memory:
            - It goes back to the pending third function, finds the "return;" command, and executes it (to end the third attempt).
            - Then it goes back to the pending second function, finds the "return;" command, and executes it (to end the second attempt).
            - Finally, it goes back to the pending first function, finds the "return;" command, and executes it (to end the first attempt).

            Summary:
            Every time the client makes a mistake and you call a new screen from within the current screen,
            you are actually placing a new "layer" in the program's memory (Stack).
            When finished, the program closes these layers one by one, executing "return;" for every layer that was opened.
        */
    }

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithDrawBalance;
}

void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "===================================\n";
    cout << "\tNormal Withdraw Menu\n";
    cout << "===================================\n";

    PerfromNormalWithdrawOption();
}

double ReadDepositAmount()
{
    double Amount;

    do
    {
        cout << "\nEnter a positive Deposit Amount? ";
        cin >> Amount;
    } while (Amount <= 0);

    return Amount;
}

void PerformDepositOption()
{

    double DepositAmount = ReadDepositAmount();

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
    CurrentClient.AccountBalance += DepositAmount;
}

void ShowDepositScreen()
{
    cout << "===========================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===========================================\n";

    PerformDepositOption();
}

void ShowCheckBalanceScreen()
{
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";

    cout << "Your Balance is " << CurrentClient.AccountBalance;
    GoBackToMainMenue();
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{

    switch (MainMenueOption)
    {
    case enMainMenueOptions::eQuickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawMenu();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eNormalWithdraw:
    {
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eCheckBalance:
    {
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eExit:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";

    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

void Login()
{

    string AccountNumber, PinCode;

    bool LoginFaild = false;

    do
    {
        system("cls");
        cout << "===========================================\n";
        cout << "\tLogin Screen";
        cout << "\n===========================================\n";

        if (LoginFaild)
            cout << "Invlaid Account Number/PinCode!\n";

        cout << "Enter Account Number? ";
        cin >> AccountNumber;

        cout << "Enter PinCode? ";
        cin >> PinCode;

        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFaild);

    ShowMainMenue();
}

int main()
{
    Login();

    return 0;
}
