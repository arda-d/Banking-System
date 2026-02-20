#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class User {
public:
	int userID;
	string userName;
	string userPassword;
	double balance;

	User(int ui, string un, string up, double b = 0.0)
	{
		userID = ui;
		userName = un;
		userPassword = up;
		balance = b;
	}
};

class Bank {
private:
	vector<User> users;
	int logged = -1;
public:
	void userAdd()
	{
		int userID;
		string userName, userPassword;
		cout << "Please enter your user ID: " << "\n";
		cin >> userID;
		cin.ignore();

		cout << "Please enter your username: " << "\n";
		getline(cin, userName);

		cout << "Please enter your user password: " << "\n";
		getline(cin, userPassword);

		for (const auto& User : users)
		{
			if (User.userID == userID)
			{
				cout << "User ID already exists. Aborting add.\n";
				return;
			}
			if (User.userName == userName)
			{
				cout << "Username already exists. Aborting add.\n";
				return;
			}
		}

		users.emplace_back(userID, userName, userPassword);
	}

	void userCheck()
	{
		string userName, userPassword;

		if (users.empty())
		{
			cout << "No user found in the system." << "\n";
			return;
		}

		cout << "Please enter your username: " << "\n";
		getline(cin, userName);
		cout << "Please enter your password: " << "\n";
		getline(cin, userPassword);

		for (size_t i = 0; i < users.size(); i++)
		{
			if (users[i].userName == userName)
			{
				if (users[i].userPassword == userPassword)
				{
					logged = i;
					cout << "Login successful.\n";
					return;
				}
				else
				{
					cout << "Incorrect password.\n";
					return;
				}
			}
		}
		cout << "The user couldn't be found." << "\n";
	}

	void deposit()
	{
		if (logged == -1)
		{
			cout << "No users logged in." << "\n";
			return;
		}

		double cash;
		cout << "Enter the amount to deposit: " << "\n";
		cin >> cash;

		if (cash <= 0)
		{
			cout << "Invalid amount." << "\n";
			return;
		}

		users[logged].balance += cash;
		saveUserFile();

		cout << "Deposit successful." << "\n";
		cout << "Current balance: " << users[logged].balance << "\n";
	}

	void withdraw()
	{
		if (logged == -1)
		{
			cout << "No users logged in." << "\n";
			return;
		}

		double cash;
		cout << "Enter the amount to withdraw: " << "\n";
		cin >> cash;

		if (cash <= 0)
		{
			cout << "Invalid amount." << "\n";
			return;
		}

		if (cash > users[logged].balance)
		{
			cout << "Insufficient amount." << "\n";
			return;
		}

		users[logged].balance -= cash;
		saveUserFile();

		cout << "Withdraw successful." << "\n";
		cout << "Current balance: " << users[logged].balance << "\n";
	}

	void balance()
	{
		if (logged == -1)
		{
			cout << "No users logged in." << "\n";
			return;
		}

		cout << "Current balance: " << users[logged].balance << "\n";
	}

	void transfer()
	{
		if (logged == -1)
		{
			cout << "No user logged in." << "\n";
			return;
		}

		int receiver;
		double cash;

		cout << "Enter the receiver ID: " << "\n";
		cin >> receiver;

		cout << "Enter the transfer amount: " << "\n";
		cin >> cash;

		if (cash <= 0)
		{
			cout << "Invalid amount." << "\n";
			return;
		}

		if (cash > users[logged].balance)
		{
			cout << "Insufficient amount." << "\n";
			return;
		}

		for (auto& User : users)
		{
			if (User.userID == receiver)
			{
				users[logged].balance -= cash;
				User.balance += cash;
				saveUserFile();

				cout << "Transfered successfully." << "\n";
				return;
			}
		}

		cout << "No ID found in the system." << "\n";
	}

	void logout()
	{
		if (logged == -1)
		{
			cout << "No user logged in." << "\n";
			return;
		}

		logged = -1;
		cout << "Logged out successfully." << "\n";
	}

	void saveUserFile()
	{
		ofstream file("users.txt");
		if (!file.is_open())
		{
			cout << "File does not exist" << "\n";
			return;
		}

		for (const auto& User : users)
		{
			file << User.userID << " | ";
			file << User.userName << " | ";
			file << User.userPassword << " | ";
			file << User.balance << "\n";
		}
		file.close();

		cout << "File saved successfully." << "\n";
	}

	void loadUserFile()
	{
		ifstream file("users.txt");
		if (!file.is_open())
		{
			cout << "File does not exists, creating one.." << "\n";
			return;
		}

		users.clear();
		string user;

		while (getline(file, user))
		{
			int userID;
			string userName;
			string userPassword;
			double balance;
			bool Pass;

			size_t size;

			size = user.find(" | ");
			userID = stoi(user.substr(0, size));
			user.erase(0, size + 3);

			size = user.find(" | ");
			userName = user.substr(0, size);
			user.erase(0, size + 3);

			size = user.find(" | ");
			userPassword = user.substr(0, size);
			user.erase(0, size + 3);

			balance = stod(user);

			users.emplace_back(userID, userName, userPassword, balance);
		}
		file.close();
		cout << "Users loaded successfully." << "\n";
	}
};


void Usermenu()
{
	cout << "---User Menu---" << "\n";
	cout << "1. Signin		2. Login" << "\n";
	cout << "3. Exit." << "\n";
	cout << "Choice: " << "\n";
}

void menu()
{
	cout << "---System Menu---" << "\n";
	cout << "1. Deposit Cash		2. Withdraw Cash" << "\n";
	cout << "3. Check Balance		4. Transfer Cash" << "\n";
	cout << "5. Logout" << "\n";
	cout << "Choice: " << "\n";
}

int main()
{
	Bank bank;
	bank.loadUserFile();

	int choice;

	do {
		Usermenu();
		cin >> choice;
		cin.ignore();

		switch (choice)
		{
		case 1:
			bank.userAdd();
			bank.saveUserFile();
			break;
		case 2:
			bank.userCheck();
			int systemChoice;
			do {
				menu();
				cin >> systemChoice;
				cin.ignore();

				switch (systemChoice)
				{
				case 1:
					bank.deposit();
					break;
				case 2:
					bank.withdraw();
					break;
				case 3:
					bank.balance();
					break;
				case 4:
					bank.transfer();
					break;
				case 5:
					cout << "Logging out.." << "\n";
					bank.logout();
					break;
				default:
					cout << "Invalid.." << "\n";
					break;
				}
			} while (systemChoice != 5);
			bank.saveUserFile();
			break;
		case 3:
			bank.saveUserFile();
			cout << "Exiting the program.." << "\n";
			break;
		default:
			cout << "Invalid.." << "\n";
			break;
		}
	} while (choice != 3);

	return 0;
}