#include <iostream>
#include <cstring>
#include <Windows.h>

using namespace std;

class Contact
{
private:
	char* name;
	char* surname;
	char* second_surname;
	int home_phone;
	int work_phone;
	int mobile_phone;
	char* additional_info;
public: 
	Contact()
	{
		name = nullptr;
		surname = nullptr;
		second_surname = nullptr;
		home_phone = 0;
		work_phone = 0;
		mobile_phone = 0;
		additional_info = nullptr;
	}
	Contact(const char* n, const char* s, const char* ss, int home, int work, int mobile, const char* info) 
	{
		name = new char[strlen(n) + 1];
		strcpy_s(name, strlen(n) + 1, n);

		surname = new char[strlen(s) + 1];
		strcpy_s(surname, strlen(s) + 1, s);

		second_surname = new char[strlen(ss) + 1];
		strcpy_s(second_surname, strlen(ss) + 1, ss);

		home_phone = home;
		work_phone = work;
		mobile_phone = mobile;

		additional_info = new char[strlen(info) + 1];
		strcpy_s(additional_info, strlen(info) + 1, info);
	}

	~Contact() 
	{
		if (name != nullptr)delete[] name;
		if (surname != nullptr)delete[] surname;
		if (second_surname != nullptr)delete[] second_surname;
		if (additional_info != nullptr)delete[] additional_info;
	}
	void setName(const char* n, const char* s, const char* ss)
	{
		if (name != nullptr)delete[] name;
		if (surname != nullptr)delete[] surname;
		if (second_surname != nullptr)delete[] second_surname;

		name = new char[strlen(n) + 1];
		strcpy_s(name, strlen(n) + 1, n);

		surname = new char[strlen(s) + 1];
		strcpy_s(surname, strlen(s) + 1, s);

		second_surname = new char[strlen(ss) + 1];
		strcpy_s(second_surname, strlen(ss) + 1, ss);
	}

	char* GetName() const
	{
		return name;
	}
	char* GetSurname() const
	{
		return surname;
	}
	char* GetSecSurname() const
	{
		return second_surname;
	}
	void Print()
	{
		cout << name << " " << surname << " " << second_surname << endl;
		cout << additional_info << endl;
		cout << "Home phone: " << home_phone << endl;
		cout << "Work phone: " << work_phone << endl;
		cout << "Mobile phone: " << mobile_phone << endl << endl;
	}
	void Input()
	{
		char buff[100];
		cout << "Enter name -> ";
		cin >> buff;
		if (name != nullptr) delete[] name;

		name = new char[strlen(buff) + 1];
		strcpy_s(name, strlen(buff) + 1, buff);

		cout << "Enter surname -> ";
		cin >> buff;
		if (surname != nullptr) delete[] surname;

		surname = new char[strlen(buff) + 1];
		strcpy_s(surname, strlen(buff) + 1, buff);

		cout << "Enter second surname -> ";
		cin >> buff;
		if (second_surname != nullptr) delete[] second_surname;

		second_surname = new char[strlen(buff) + 1];
		strcpy_s(second_surname, strlen(buff) + 1, buff);

		cout << "Enter home phone -> ";
		cin >> home_phone;

		cout << "Enter work phone -> ";
		cin >> work_phone;

		cout << "Enter mobile phone -> ";
		cin >> mobile_phone;

		cout << "Enter additional info -> ";
		cin >> buff;
		if (additional_info != nullptr) delete[] additional_info;

		additional_info = new char[strlen(buff) + 1];
		strcpy_s(additional_info, strlen(buff) + 1, buff);
	}

	void WriteContactToFile(FILE* file)
	{
		fprintf(file, "%s ", name);
		fprintf(file, " %s ", surname);
		fprintf(file, " %s ", second_surname);
		fprintf(file, " %d ", home_phone);
		fprintf(file, " %d ", work_phone);
		fprintf(file, " %d ", mobile_phone);
		fprintf(file, "%s ", additional_info);
		fputc('\n\n', file);
	}
};

class Phonebook {
private:
	Contact* contacts[100];
	int contactCount;

public:
	Phonebook()
	{
		contactCount = 0;
		for (int i = 0; i < 100; ++i) {contacts[i] = nullptr;}
	}

	~Phonebook() 
	{
		for (int i = 0; i < contactCount; i++) 
		{
			delete contacts[i];
		}
	}

	void ViewContactsFromFile()
	{
		FILE* file;
		errno_t err = fopen_s(&file, "phonebook.txt", "r"); 
		if (err == 0 && file != nullptr)
		{
			char name[50], surname[50], s_surname[50], h_phone[50], w_phone[50], m_phone[50], a_info[50];
			while (fscanf_s(file, "%49s %49s %49s %49s %49s %49s %49s", name, (unsigned)_countof(name),
				surname, (unsigned)_countof(surname),
				s_surname, (unsigned)_countof(s_surname),
				h_phone, (unsigned)_countof(h_phone),
				w_phone, (unsigned)_countof(w_phone),
				m_phone, (unsigned)_countof(m_phone),
				a_info, (unsigned)_countof(a_info)) == 7)
			{
				cout << name << " " << surname << " " << s_surname << endl;
				cout << a_info << endl;
				cout << "Home phone: " << h_phone << endl;
				cout << "Work phone: " << w_phone << endl;
				cout << "Mobile phone: " << m_phone << endl << endl;
			}
			fclose(file);
		}
		else
		{
			cout << "Error: Unable to open file for reading." << endl;
		}
	}

	void AddContact() 
	{
		contacts[contactCount] = new Contact();
		(*contacts[contactCount]).Input();
		contactCount++;
	}

	void DeleteContact(const char* name, const char* surname, const char* sec_surname)
	{
		bool found = false;

		for (int i = 0; i < contactCount; i++)
		{
			if (strcmp((*contacts[i]).GetName(), name) == 0 && strcmp((*contacts[i]).GetSurname(), surname) == 0 && strcmp((*contacts[i]).GetSecSurname(), sec_surname) == 0)
			{
				delete[] contacts[i]; // Звільняємо пам'ять
				contacts[i] = nullptr; // Уникаємо випадкового повторного доступу

				// Переміщуємо всі наступні контакти на одну позицію вліво
				for (int j = i; j < contactCount - 1; j++) 
				{
					contacts[j] = contacts[j + 1];
				}

				contactCount--; // Зменшуємо кількість контактів
				found = true;
				break; // Виходимо з циклу, оскільки контакт знайдено і видалено
			}
		}

		if (!found) 
		{
			cout << "Contact " << name << " not found." << endl;
		}
	}

	void SearchByName(const char* name, const char* surname, const char* sec_surname) const 
	{
		for (int i = 0; i < contactCount; i++) 
		{
			if (strcmp((*contacts[i]).GetName(), name) == 0 && strcmp((*contacts[i]).GetSurname(), surname) == 0 && strcmp((*contacts[i]).GetSecSurname(), sec_surname) == 0)
			{
				(*contacts[i]).Print();
				return;
			}
		}
	}


	void WriteToFile(const char* filename) const 
	{
		FILE* file;
		errno_t err = fopen_s(&file, filename, "a");
		if (err == 0 && file != nullptr)
		{
			for (int i = 0; i < contactCount; i++)
			{
				(*contacts[i]).WriteContactToFile(file);
			}
			fclose(file);
		}
		else 
		{
			cout << "Error: Unable to open file for writing." << endl;
		}
	}
};

void ClearFileContents(char filename[])
{
	FILE* file;
	errno_t err = fopen_s(&file, filename, "w");
	if (err == 0 && file != nullptr)
	{
		fclose(file);
	}
	else
	{
		cout << "Error: Unable to open file." << endl;
	}
}

void SearchByName(Phonebook list)
{
	char* name = new char[20];
	cout << "Enter name : ";
	cin >> name;
	int size = strlen(name);
	char* Name = new char[size + 1];
	strcpy_s(Name, size + 1, name);

	char* surname = new char[20];
	cout << "Enter surname : ";
	cin >> surname;
	size = strlen(surname);
	char* Surname = new char[size + 1];
	strcpy_s(Surname, size + 1, surname);

	char* s_surname = new char[20];
	cout << "Enter second surname : ";
	cin >> s_surname;
	size = strlen(s_surname);
	char* S_Surname = new char[size + 1];
	strcpy_s(S_Surname, size + 1, s_surname);

	delete[] name;
	delete[] surname;
	delete[] s_surname;

	list.SearchByName(Name, Surname, S_Surname);

	delete[] Name;
	delete[] Surname;
	delete[] S_Surname;
}

void RemoveContact(Phonebook list, char filename[])
{
	int temp = -1;

	char* name = new char[20];
	cout << "Enter name : ";
	cin >> name;
	int size = strlen(name);
	char* Name = new char[size + 1];
	strcpy_s(Name, size + 1, name);

	char* surname = new char[20];
	cout << "Enter surname : ";
	cin >> surname;
	size = strlen(surname);
	char* Surname = new char[size + 1];
	strcpy_s(Surname, size + 1, surname);

	char* s_surname = new char[20];
	cout << "Enter second surname : ";
	cin >> s_surname;
	size = strlen(s_surname);
	char* S_Surname = new char[size + 1];
	strcpy_s(S_Surname, size + 1, s_surname);

	delete[] name;
	delete[] surname;
	delete[] s_surname;

	list.DeleteContact(Name, Surname, S_Surname);

	delete[] Name;
	delete[] Surname;
	delete[] S_Surname;

	ClearFileContents(filename);
	list.WriteToFile(filename);
}
void ShowMenu()
{
	cout << " 1. Show all contacts " << endl << " 2. Search by full name " << endl << " 3. Add new contact " << endl << " 4. Delete contact " << endl << " 0. Exit " << endl;
}
void PerformAction(int option, Phonebook list, char filename[])
{
	if (option == 1) list.ViewContactsFromFile();
	else if (option == 2) SearchByName(list);
	else if (option == 3) {list.AddContact(); list.WriteToFile(filename);}
	else if (option == 4) RemoveContact(list, filename);
	else if (option == 0) return;

	system("pause");

}

int main()
{
	Phonebook contacts;
	int option = 0;
	char list[] = "phonebook.txt"; //название текстового документа в который будут записываться изменения массива
	ClearFileContents(list); //очищение файла после предыдущей работы программы
	//цикл основной работы программы
	do
	{
		system("cls");
		ShowMenu();
		cout << " Enter your option: ";
		cin >> option;
		PerformAction(option, contacts, list);
	} while (option != 0);

	return 0;
}