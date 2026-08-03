#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
const string ClientFileName = "ClientData10.txt";

void ShowMainMenuen();

void ShowTransactionScreen();

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	bool MarkForDelete = false;
	double AccountBalance;

};

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	short pos = 0;
	string sWord;
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length());

	}
	if (S1 != "")
	{
		vString.push_back(S1);
	}
	return vString;
}

sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

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

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}

void PrintClientCard(sClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nName : " << Client.Name;
	cout << "\nPhone : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";
}

sClient ChageClientRecord(string AccountNumber)
{

	sClient client;

	client.AccountNumber = AccountNumber;
	cout << "\n\nEnter Pin Code : ";
	getline(cin >> ws, client.PinCode);

	cout << "Enter Name : ";
	getline(cin, client.Name);

	cout << "Enter Phone : ";
	getline(cin, client.Phone);

	cout << "Enter Account Balance : ";
	cin >> client.AccountBalance;
	return client;


}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

sClient ReadNewClint()
{
	sClient client;

	cout << "\n\nEnter Account Number? ";
	getline(cin >> ws, client.AccountNumber);

	while (ClientExistsByAccountNumber(client.AccountNumber, ClientFileName))
	{
		cout << "\nClient with [" << client.AccountNumber << "]	already exists, Enter another Account Number ? ";
		getline(cin >> ws, client.AccountNumber);
	}

	cout << "Enter Pin Code : ";
	getline(cin, client.PinCode);

	cout << "Enter Name : ";
	getline(cin, client.Name);

	cout << "Enter Phone : ";
	getline(cin, client.Phone);

	cout << "Enter Account Balance : ";
	cin >> client.AccountBalance;
	return client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

void PrintClientRecordBalanceLien(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;

}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClient)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want delete this client? [Y \ N] ? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{

			MarkClientForDeleteByAccountNumber(AccountNumber, vClient);
			SaveCleintsDataToFile(ClientFileName, vClient);
			vClient = LoadCleintsDataFromFile(ClientFileName);

			cout << "\n\nClient Deleted, Successfully.\n";
			return true;
		}


	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;

	}


}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>vClient)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want update this client? [Y \ N] ? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{

			for (sClient& C : vClient)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChageClientRecord(AccountNumber);
					break;
				}
			}

			SaveCleintsDataToFile(ClientFileName, vClient);

			cout << "\n\nClient Updated, Successfully.\n";
			return true;
		}

	}



}

bool DepositTransaction(string AccountNumber, double amount, vector<sClient>vClient)
{
	char Answr = 'n';

	cout << "\n\nAre you sure you want perform this Transaction ? [Y / N] ? ";
	cin >> Answr;

	if (Answr == 'y' || Answr == 'Y')
	{
		for (sClient& C : vClient)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += amount;

				SaveCleintsDataToFile(ClientFileName, vClient);
				cout << "\n \nDone Successfully , New balance is. ";
				return true;
			}
		}
		return false;
	}

}

bool WithdrawTransaction(string AccountNumber, double amount, vector<sClient>vClient)
{
	char Answr = 'n';

	cout << "\n\nAre you sure you want perform this Transaction ? [Y / N] ? ";
	cin >> Answr;
	if (Answr == 'y' || Answr == 'Y')
	{
		for (sClient& C : vClient)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += amount;

				SaveCleintsDataToFile(ClientFileName, vClient);
				cout << "\n\nDone Successfully , New balance is. ";
				return true;
			}
		}
		return false;
	}

}

string ReadClientAccountNumber()
{
	string AccountNumber = "";

	cout << "\nPlease Enter Account Number : ";
	cin >> AccountNumber;
	return AccountNumber;
}

void ShowTotalBalance()
{

	vector <sClient>vClient = LoadCleintsDataFromFile(ClientFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClient.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	double TotalBalances = 0;
	if (vClient.size() == 0)
	{
		cout << "\n\nNot Client Available In The System!";
	}
	else
	{
		for (sClient Client : vClient)
		{
			PrintClientRecordBalanceLien(Client);
			TotalBalances += Client.AccountBalance;
			cout << endl;
		}
		cout << "\n_______________________________________________________";
		cout << "_________________________________________\n" << endl;
		cout << "\t\t\t\t\tTotal Balance = " << TotalBalances;
	}
}

void LoadDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;

		MyFile.close();
	}

}

void AddNewClint()
{
	sClient Clints;
	Clints = ReadNewClint();
	LoadDataLineToFile(ClientFileName, ConvertRecordToLine(Clints));
}

void AddClints()
{
	char AddMore = 'Y';

	do
	{
		//system("cls");
		cout << "\nAdding New Clint : \n";
		AddNewClint();
		cout << "\n\nClient Added Successfully , do you want to add more client ? Y/N ?  ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

void PrintClients(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowAllClientMainMenue()
{
	vector <sClient>vClient = LoadCleintsDataFromFile(ClientFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClient.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	for (sClient& C : vClient)
	{
		PrintClients(C);
		cout << endl;
	}
	cout << "\n___________________________________________________________________";
	cout << "_______________________________________\n" << endl;
}

void ShowAddNewClientScreen()
{
	cout << "\n___________________________________________________\n";
	cout << "\n\t\tAdd New Client Screen\n";
	cout << "\n___________________________________________________\n";
	AddClints();


}

void ShowDeleteClintScren()
{

	cout << "\n___________________________________________________\n";
	cout << "\n\t\tDelete Client Screen\n";
	cout << "\n___________________________________________________\n";

	vector<sClient>vClient = LoadCleintsDataFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();

	DeleteClientByAccountNumber(AccountNumber, vClient);
}

void ShowUpdateClientScreen()
{
	cout << "\n___________________________________________________\n";
	cout << "\n\t\tUpdate Client Info Screen\n";
	cout << "\n___________________________________________________\n";

	vector<sClient>vClient = LoadCleintsDataFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, vClient);

}

void ShowFindClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientFileName);
	sClient Client;
	string AccountNumber = ReadClientAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

void ShowEndScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n-----------------------------------\n";

}

void ShowDepositScreen()
{

	cout << "\n___________________________________________________\n";
	cout << "\n\t\tDeposit Screen";
	cout << "\n___________________________________________________\n";

	string AccountNumber = ReadClientAccountNumber();
	sClient Client;
	vector <sClient>vClients = LoadCleintsDataFromFile(ClientFileName);

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{

		cout << "\nClient with [" << AccountNumber << "] does not exit!\n ";
		AccountNumber = ReadClientAccountNumber();

	}

	PrintClientCard(Client);

	double amount;
	cout << "\nPleas enter deposit amount ?";
	cin >> amount;
	DepositTransaction(AccountNumber, amount, vClients);

}

void ShowWithdrawScreen()
{
	cout << "\n___________________________________________________\n";
	cout << "\n\t\tDeposit Screen";
	cout << "\n___________________________________________________\n";

	string AccountNumber = ReadClientAccountNumber();
	sClient Client;
	vector <sClient>vClients = LoadCleintsDataFromFile(ClientFileName);

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{

		cout << "\nClient with [" << AccountNumber << "] does not exit!.\n ";
		AccountNumber = ReadClientAccountNumber();

	}

	PrintClientCard(Client);


	double Amount;
	cout << "\nPleas enter Withdraw amount ?";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "Amount Exceeds the balance , you can Withdraw up to : " << Amount << "";
		cout << "\nPleas enter Withdraw amount ?";
		cin >> Amount;
	}

	WithdrawTransaction(AccountNumber, Amount * -1, vClients);

}

void ShowTotalBalanceScreen()
{

	ShowTotalBalance();

}

enum enTransactionOption
{
	eDeposit = 1, eWithdraw = 2,
	eTotalBalances = 3, eMainMenue = 4

};

enum enMainMenueOption
{
	eShowList = 1, eAdd = 2,
	eDelete = 3, eUpdate = 4,
	eFind = 5, Transaction = 6,
	eExit = 7

};

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to main Menue...";
	system("pause>0");
	ShowMainMenuen();
}

void GoBackToMainTransaction()
{
	cout << "\n\nPress any key to go back to Transactions  Menue...";
	system("pause>0");
	ShowTransactionScreen();
}

short ReadTransactionsMenueOption()
{
	cout << "Choose what do you want to do ? [1 to 4] ? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

void PerformTransaction(enTransactionOption Transactions)
{
	switch (Transactions)
	{
	case enTransactionOption::eDeposit:
	{
		system("cls");
		ShowDepositScreen();
		GoBackToMainTransaction();
		break;
	}
	case enTransactionOption::eWithdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBackToMainTransaction();
		break;
	case enTransactionOption::eTotalBalances:
		system("cls");
		ShowTotalBalanceScreen();
		GoBackToMainTransaction();
		break;
	case enTransactionOption::eMainMenue:
		ShowMainMenuen();
	}
}

void ShowTransactionScreen()
{

	system("cls");
	cout << "=======================================================\n";
	cout << "\t\t Transaction Menue Screen \n";
	cout << "=======================================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw. \n";
	cout << "\t[3] Total Balances. \n";
	cout << "\t[4] MainMenue. \n";
	cout << "=======================================================\n";

	PerformTransaction((enTransactionOption)ReadTransactionsMenueOption());

}

short ReadMainMnue()
{
	cout << "Choose what do you want to do? [1 to 7] ? ";
	short Choise = 0;
	cin >> Choise;
	return  Choise;
}

void PerformMainMenue(enMainMenueOption MainMenueOption)
{

	switch (MainMenueOption)
	{
	case enMainMenueOption::eShowList:
	{
		system("cls");
		ShowAllClientMainMenue();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOption::eAdd:
		system("cls");
		ShowAddNewClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOption::eDelete:
		system("cls");
		ShowDeleteClintScren();
		GoBackToMainMenue();
		break;
	case enMainMenueOption::eUpdate:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOption::eFind:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOption::Transaction:
		system("cls");
		ShowTransactionScreen();
		break;
	case enMainMenueOption::eExit:
		system("cls");
		ShowEndScreen();
		GoBackToMainMenue();
		break;
	default:
		exit(0);
	}

}

void ShowMainMenuen()
{
	system("cls");
	cout << "=======================================================\n";
	cout << "\t\tMain Menue Screen \n";
	cout << "=======================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Clients. \n";
	cout << "\t[3] Delete Clients. \n";
	cout << "\t[4] Update Clients. \n";
	cout << "\t[5] Find Clients Info.\n";
	cout << "\t[6] Transaction.\n";
	cout << "\t[7] Exit.";
	cout << "\n=======================================================\n";

	PerformMainMenue((enMainMenueOption)ReadMainMnue());

}

int main()
{
	ShowMainMenuen();
	system("pause>0");
	return 0;
}
