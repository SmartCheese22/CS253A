#include <bits/stdc++.h>
#include <chrono>

using namespace std;

vector<vector<string>> content;
vector<string> to_update;

// Forward Declaration of Classes
class User;
class Customer;
class Employee;
class Manager;
class Car;

enum UserType
{
    eCustomer = 0,
    eEmployee,
    eManager
};

// Function to read csv file and store in Content vector
void readFile(string filename)
{
    vector<string> row;
    string line, word;

    fstream file(filename, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();
            stringstream str(line);
            while (getline(str, word, ','))
            {
                row.push_back(word);
            }
            content.push_back(row);
        }
    }
    else
    {
        cout << "Error Opening the file " << filename << endl;
    }
    file.close();
}

// Writing to a given csv file
void writeFile(vector<vector<string>> &writeContent, string filename)
{
    fstream fout(filename, ios::out);

    for (auto &line : writeContent)
    {
        for (auto &word : line)
        {
            fout << word;
            if (word != line.back())
            {
                fout << ",";
            }
        }
        fout << "\n";
    }
    fout.close();
}

void printData(vector<vector<string>> &printContent)
{
    int count = 1;
    for (auto &line : printContent)
    {
        cout << (count++) << ". ";
        for (auto &word : line)
        {
            cout << word;
            if (word != line.back())
            {
                cout << " | ";
            }
        }
        cout << "\n";
    }
}

void writeFileAppend(vector<string> &line, string filename)
{
    fstream fapp(filename, ios::app | ios::out);

    for (auto word : line)
    {
        fapp << word;
        if (word != line.back())
        {
            fapp << ",";
        }
    }
    fapp << "\n";
    fapp.close();
}

// Does not prints the User Password
void privacyPrint(vector<vector<string>> &printData)
{
    int count = 1;
    for (auto line : printData)
    {
        string type;
        if (line[3] == "0")
        {
            type = "Customer";
        }
        else if (line[3] == "1")
        {
            type = "Employee";
        }
        else
        {
            type = "Manager";
        }
        cout << count << ". ";
        cout << line[0] << " | " << line[1] << " | " << type << "\n";
        count++;
    }
}

// A metric which determines maxRentable Cars depending on the CustomerRecord or Employee Record
int calculateRentableCars(float metric)
{
    if (metric >= 0 && metric <= 20)
    {
        return 0;
    }
    else if (metric <= 40)
    {
        return 1;
    }
    else if (metric <= 60)
    {
        return 2;
    }
    else if (metric <= 80)
    {
        return 3;
    }
    else
    {
        return 4;
    }
}

class User
{
private:
    string password;
    string name;

public:
    string id;
    float record;
    float fineDue;

    void login(void);
    void logout(void);
    virtual void display_menu(void);
    void all_cars(void);
    void rented_cars(void);

    bool check_availability(string licenseNo);

protected:
    void rent_car(string carName, UserType usertype);
    void return_car(string licenseNo);
    void clear_due(void);
    void user_add(void);
    void user_update(void);
    void user_delete(void);
};

class Customer : public User
{
public:
    void display_menu(void);

    Customer(string id, float record = 50.0, float fineDue = 0.0, string name = "", string passwordc = "")
    {
        this->id = id;
        this->fineDue = fineDue;
        this->record = record;
    }
};

class Employee : public User
{
public:
    void display_menu();

    Employee(string id, float record = 50.0, float fineDue = 0.0, string name = "", string password = "")
    {
        this->id = id;
        this->fineDue = fineDue;
        this->record = record;
    }
};

class Manager : public User
{
public:
    Manager(string id, string name = "", string password = "")
    {
        this->id = id;
    }
    void display_menu();
    void all_users();

    void add_user();
    void update_user();
    void delete_user();

    void add_car();
    void update_car();
    void delete_car();

    void see_rented_to_user();
    void see_rented_car_to(string &licenseNo);

    void repair_car();
};

class Car
{
    void car_add();
    void car_update(string licenseNo);
    void car_delete(string licenseNo);
    void car_search(string licenseNo);

public:
    string licenseNo;
    string model;
    int condition = 100;
    bool isRented;
    int cost;

    void rent_request(string carName, UserType usertype);
    void show_duedate(string licenseNo);

    friend class Manager;
};

//? Member functions of User Class implemented

void User::display_menu()
{
    char c;
    cout << "--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*\n";

    cout << "\n\n Welcome to the Car Rental Portal!! \n";
    cout << "1. Press 1 to log in : \n";
    cout << "2. Press 2 to exit the application\n\n";

    cout << "--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*\n";
    cin >> c;
    if (c == '1')
    {
        User u;
        u.login();
    }
    else
    {
        cout << "\nThanks for using the system !" << endl;
        int temp;
        cin >> temp;
        exit(1);
    }
}

void User::login()
{
    string id, password;
    cout << "..................................................................\n";
    cout << "..................\t\t   Login Page   \t\t........................\n";
    cout << "\nPlease enter your id :\n";
    cin >> id;
    cout << "\nPlease enter your password :\n";
    cin >> password;

    vector<string> words_in_row;
    string line, word;
    int count = 0;

    fstream file("users.csv", ios::in);

    if (file.is_open())
    {
        while (getline(file, line))
        {
            words_in_row.clear();
            stringstream str(line);

            while (getline(str, word, ','))
            {
                words_in_row.push_back(word);
            }

            if (words_in_row[1] == id)
            {
                count = 1;
                if (words_in_row[2] == password)
                {
                    count = 2;
                    break;
                }
                else
                {
                    int chances = 6;
                    while (chances >= 0 and password != words_in_row[2])
                    {
                        chances--;
                        cout << "\nYou have entered the wrong Password!\nPress 1 to re-enter password. \nPress 2 to exit the program.\n";
                        char c;
                        cin >> c;
                        if (c == '1')
                        {
                            cout << "\nEnter the password :\n";
                            cin >> password;
                            if (password == words_in_row[2])
                                count = 2;
                        }
                        else if (c == '2')
                        {
                            cout << "\nExiting the program...\n";
                            cout << "Thank You for using the program\n";
                            return;
                        }
                        else if (chances == 0)
                        {
                            cout << "\nYou have exceed the maximum number of invalid attempts. Exiting the program ...\n";
                            return;
                        }
                        else
                        {
                            cout << "\nPlease Enter a valid input\n";
                        }
                    }
                    if (count == 1 or count == 2)
                        break;
                }
            }
            if (count == 1 or count == 2)
            {
                break;
            }
        }

        if (count == 0)
        {
            cout << "User not Found!\n Contact the Manager\n";
            display_menu();
        }
        else if (count == 1)
        {
            cout << "Password was entered wrong multiple times\n";
            display_menu();
        }
        else if (count == 2)
        {
            string type = words_in_row[3];
            if (type == "0")
            {
                Customer C(id, stof(words_in_row[4]), stof(words_in_row[5]));
                C.display_menu();
            }
            else if (type == "1")
            {
                Employee E(id, stof(words_in_row[4]), stof(words_in_row[5]));
                E.display_menu();
            }
            else
            {
                Manager M(id);
                M.display_menu();
            }
        }
    }
    else
    {
        cout << "\nError Opening file users.csv\n";
        return;
    }
    file.close();
}

void User::all_cars()
{
    content.clear();
    readFile("cars.csv");

    cout << "\nBelow are all the Cars. 1 at the end means it is currently rented and 0 means it is available to rent\n";
    cout << "The data is given in the form of\n Model,\t LicenseNo,\t Condition, Cost, IsRented\n";
    int count = 1;
    for (auto &line : content)
    {
        cout << (count++) << ". ";
        for (auto &word : line)
        {
            if (word != line.back())
            {
                cout <<word<< " | ";
            }
            else{
                if(word == "0"){
                    cout << "Available to Rent";
                }
                else
                    cout << "Rented";
            }
        }
        cout << "\n";
    }
    content.clear();
}

void User ::rent_car(string carName, UserType usertype)
{
    content.clear();
    bool availability = false;

    int count = 0;
    content.clear();
    readFile("rented_cars.csv");
    for (auto line : content)
    {
        if (line[0] == id)
        {
            count++;
        }
    }
    content.clear();
    if (count >= calculateRentableCars(record))
    {
        cout << "\nYou have rented the maximum Number of cars. Contact the Manager to Know more.\n";
    }
    else
    {
        readFile("cars.csv");
        for (auto &line : content)
        {
            if (line[0] == carName)
            {
                if (line[4] == "1")
                {
                    cout << "\nCar Not Available\n";
                    break;
                }
                else if (stoi(line[2]) < 5)
                {
                    cout << "\nThe car is damaged\n. Please contact the Manager\n";
                }
                else
                {
                    float cost = stof(line[3]);
                    if (usertype == eCustomer)
                    {
                        cout << "\nCar is available!\n The cost is " << cost << "\n";
                    }
                    if (usertype == eEmployee)
                    {
                        cost *= 0.85;
                        cout << "Car is available!\n The cost is " << cost << "\n";
                        cout << "\nYou have been given a 15% discount for being an Emplyoee!\n";
                    }
                    cout << "\nEnter the number of days you want to lease the car\n";
                    string days;
                    cin >> days;
                    line[4] = "1";
                    to_update.clear();
                    to_update.push_back(id);
                    to_update.push_back(carName);
                    to_update.push_back(line[1]);
                    to_update.push_back(to_string(time(0)));
                    to_update.push_back(days);
                    writeFileAppend(to_update, "rented_cars.csv");
                    cout << "You have to pay Rupees " << stoi(days) * cost<< " to rent the car\n";
                    cout << "\nCar rented Successfully! \n";
                    break;
                }
            }
        }
        writeFile(content, "cars.csv");
        content.clear();
    }
}

void User::return_car(string licenseNo)
{
    content.clear();
    readFile("rented_cars.csv");
    bool freed = false;
    float damage;
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][2] == licenseNo and content[i][0] == id)
        {
            string ctime = content[i][3];
            string crented = content[i][4];

            int curtime = time(0);
            int isstime = stoi(ctime);
            int maxDays = stoi(crented);
            int elapsed = (curtime - isstime) / 86400;
            if (elapsed > maxDays)
            {
                fineDue += 1000 * (elapsed - maxDays);
                damage = ((elapsed)*1.0) / (record + 1);
                record -= 0.5 * (elapsed - maxDays) + 0.3 * damage;
                if (record <= 0)
                    record = 0.0;
            }
            else
            {
                record += 2.0;
                if (record >= 100.0)
                    record = 100.0;
                damage = (elapsed) / (record + 1);
            }
            content.erase(content.begin() + i, content.begin() + i + 1);
            cout << "\nCar was rented by you and is now being given back\n";
            if (elapsed > maxDays)
                cout << "\nThis has incurred you a fine of rupees " << 1000.0 * (elapsed - maxDays) << "\n";
            writeFile(content, "rented_cars.csv");
            content.clear();
            readFile("cars.csv");
            for (auto &line : content)
            {
                if (line[1] == licenseNo)
                {
                    line[4] = "0";
                    damage = stof(line[2]) - damage;
                    if (damage < 0)
                    {
                        damage = 0;
                    }
                    line[2] = to_string(damage);
                    break;
                }
            }
            writeFile(content, "cars.csv");

            content.clear();
            readFile("users.csv");
            for (auto &line : content)
            {
                if (line[1] == id)
                {
                    line[4] = to_string(record);
                    line[5] = to_string(fineDue);
                    break;
                }
            }
            writeFile(content, "users.csv");
            freed = true;
            break;
        }
    }
    if (!freed)
    {
        cout << "The car is not rented by you or you have entered Invalid Details!\n";
    }
    content.clear();
}

void User::rented_cars()
{
    int count = 1;
    content.clear();
    readFile("rented_cars.csv");

    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][0] == id)
        {
            cout << "\nFollowing are the details of your rented Car in the given format\n";
            cout << "id, \tCar Name, \tLicense Number, Rent Date\t\n\n";
            cout << count << ". ";
            count++;
            for (int k = 0; k + 2 < content[i].size(); k++)
            {
                cout << content[i][k] << " | ";
            }
            time_t stamp = stoi(content[i][3]);
            tm *dateformat = localtime(&stamp);
            cout << dateformat->tm_mday << "/" << (1 + dateformat->tm_mon) << "/" << (1900 + dateformat->tm_year) << "\n";
        }
    }

    if (count == 1)
    {
        cout << "\nYou haven't rented any car as of now\n\n";
    }
    content.clear();
}

bool User::check_availability(string licenseNo)
{
    content.clear();
    readFile("cars.csv");
    string rentedBy;
    int count = 1;
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][1] == licenseNo)
        {
            rentedBy = content[i][0];
        }
        if (content[i][1] == licenseNo and content[i][4] == "0")
        {
            cout << "\nCar is available!\n";
            cout << count << ". ";
            count++;
            for (auto j : content[i])
            {
                cout << j << " | ";
            }
            cout << "\n";
            content.clear();
            return true;
        }
    }
    if (rentedBy == id)
        cout << "You have rented the Car!\n";
    else
        cout << "\nCar is Not Available\n";
    content.clear();
    return false;
}

void User::logout()
{
    cout << "\nLogging Out...\n";
    User u;
    u.display_menu();
}

void User::clear_due()
{
    cout << "Your pending due were Rupees " << fineDue << ".\n";
    {
        fineDue = 0.0;
        content.clear();
        readFile("users.csv");
        for (auto &line : content)
        {
            if (line[1] == id)
            {
                line[5] = to_string(fineDue);
                break;
            }
        }
        writeFile(content, "users.csv");
        cout << "Your pending Dues are cleared successfully!\n";
    }
    content.clear();
}

void User::user_add()
{
    Manager m(id);
    m.add_user();
}
void User::user_delete()
{
    Manager m(id);
    m.delete_user();
}
void User::user_update()
{
    Manager m(id);
    m.update_user();
}

void Customer::display_menu()
{
    cout << "----------------------------------C----------------------------------\n";
    cout << "\nYou have been logged in as a Customer\n";
    cout << "Press 1 to see all the Cars\n";
    cout << "Press 2 to view cars rented by you\n";
    cout << "Press 3 to check if a car is available to rent or not\n";
    cout << "Press 4 to view the fineDue\n";
    cout << "Press 5 to rent a car\n";
    cout << "Press 6 to return a car\n";
    cout << "Press 7 to clear your fine\n";
    cout << "Press 8 to logout\n";
    cout << "----------------------------------C-----------------------------------\n";
    char c;
    cin >> c;
    string carName, licenseNo;
    switch (c)
    {
    case '1':
        all_cars();
        display_menu();
        break;
    case '2':
        rented_cars();
        display_menu();
        break;
    case '3':
        cout << "\nEnter the license No. of the Car you want to check : \n";
        cin.ignore();
        getline(cin, licenseNo);
        check_availability(licenseNo);
        display_menu();

        break;
    case '4':
        cout << "\nFine due is Rupees" << fineDue << "\n";
        display_menu();
        break;
    case '5':
        cout << "\nEnter the name of the car you want to issue : \n";
        cin.ignore();
        getline(cin, carName);
        rent_car(carName, eCustomer);
        display_menu();
        break;
    case '6':
        cout << "\nEnter the licenseNo  of the car you want to return : \n";
        cin >> licenseNo;
        return_car(licenseNo);
        display_menu();
        break;
    case '7':
        clear_due();
        display_menu();
        break;
    case '8':
        logout();
        break;
    }
}

void Employee::display_menu()
{
    cout << "--------------------------------E------------------------------------\n";
    cout << "\nYou have been logged in as Employee\n";
    cout << "Press 1 to see all the Cars\n";
    cout << "Press 2 to view cars rented by you\n";
    cout << "Press 3 to check if a car is available to rent or not\n";
    cout << "Press 4 to view the fine\n";
    cout << "Press 5 to rent a car\n";
    cout << "Press 6 to return a car\n";
    cout << "Press 7 to clear your fine\n";
    cout << "Press 8 to logout\n";
    cout << "--------------------------------E------------------------------------\n";
    char c;
    cin >> c;
    string carName, licenseNo;
    switch (c)
    {
    case '1':
        all_cars();
        display_menu();
        break;
    case '2':
        rented_cars();
        display_menu();
        break;
    case '3':
        cout << "\nEnter the licenseNo of the car you want to check : \n";
        cin >> licenseNo;
        check_availability(licenseNo);
        display_menu();
        break;
    case '4':
        cout << "\nFine due is " << fineDue << "\n";
        display_menu();
        break;
    case '5':
        cout << "\nEnter the name of the car you want to issue : \n";
        cin.ignore();
        getline(cin, carName);
        rent_car(carName, eEmployee);
        display_menu();
        break;
    case '6':
        cout << "\nEnter the licenseNo  of the car you want to return : \n";
        cin >> licenseNo;
        return_car(licenseNo);
        display_menu();
        break;
    case '7':
        clear_due();
        display_menu();
        break;
    case '8':
        logout();
        break;
    }
}

void Manager::all_users()
{
    content.clear();
    readFile("users.csv");
    cout << "\nBelow are all the users. 0 at the end signifies a customer, 1 signifies an Employee and 2 signifies Manager\n";
    cout << "\nThe data is given in the form of\n \tName, \tId, \tUserType\n\n";
    privacyPrint(content);
    content.clear();
}

// void Manager::all_cars()
// {
//     content.clear();
//     readFile("cars.csv");

//     cout << "Below are all the Cars. 1 at the end means it is currently rented and 0 means it is available to rent\n";
//     cout << "The data is given in the form of Model, LicenseNo, Condition, Cost, IsRented\n";
//     printData(content);
//     content.clear();
// }

void Manager::add_user()
{
    cout << "Please enter the details of the new user : \n";
    string _name, _id, _password, _type;
    cout << "Enter name of the User: \n";
    cin.ignore();
    getline(cin, _name);
    cout << "Enter id (a single word) : \n";
    cin >> _id;
    cout << "Enter password : \n";
    cin >> _password;
    cout << "Enter type of the user : 0 if Customer, 1 if Employee, 2 if Manager : \n";
    cin >> _type;
    if (_type != "0" and _type != "1" and _type != "2")
    {
        cout << "\nYou have entered invalid User Type!\n";
    }
    fstream fout("users.csv", ios::out | ios::app);
    fout << _name << "," << _id << "," << _password << "," << _type << ","
         << "50.0"
         << ","
         << "0.0" << '\n';
    cout << "New user has been added.\n";
}

void Manager::delete_user()
{
    string uid;
    cout << "Enter the user id of the user : \n";
    cin >> uid;
    content.clear();
    bool found = false;
    readFile("users.csv");
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][1] == uid)
        {
            found = true;
            content.erase(content.begin() + i, content.begin() + i + 1);
            break;
        }
    }
    writeFile(content, "users.csv");
    content.clear();

    vector<string> licenseNo;
    readFile("rented_cars.csv");
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][0] == uid)
        {
            licenseNo.push_back(content[i][2]);
            content.erase(content.begin() + i, content.begin() + i + 1);
        }
    }
    writeFile(content, "rented_cars.csv");
    content.clear();

    readFile("cars.csv");
    for (int i = 0; i < content.size(); i++)
    {
        for (int j = 0; j < licenseNo.size(); j++)
        {
            if (licenseNo[j] == content[i][1])
            {
                content[i][4] = "0";
            }
        }
    }
    writeFile(content, "cars.csv");
    if (!found)
        cout << "User not found.\n";
    else
        cout << "Removed Successfully!\n";
}

void Manager::update_user()
{
    string uid;
    cout << "Enter the id of the user you want to update : \n";
    cin >> uid;
    cout << "Enter the serial number of the field you want to update : \n";
    cout << "1. Password\n";
    cout << "2. Name of the User\n";
    cout << "3. User Record\n";
    content.clear();
    char c;
    // cout << "input not taken";
    cin >> c;
    // cout << "input taken";
    bool found = false;
    readFile("users.csv");
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][1] == uid)
        {
            found = true;
            string new_field;
            cout << "Enter the new value of the field : \n";
            cin.ignore();
            getline(cin, new_field);
            if (c == '1')
            {
                content[i][2] = new_field;
            }
            else if (c == '2')
            {
                content[i][0] = new_field;
            }
            else if (c == '3')
            {
                content[i][4] = new_field;
            }
            else
            {
                cout << "Invalid input!\n\n";
            }
            break;
        }
    }
    cout << "User updated\n";
    writeFile(content, "users.csv");
    content.clear();
    if (!found)
        cout << "User was not found.\n\n";
}

void Manager::add_car()
{
    cout << "Please enter the details of the new Car : \n";
    string name, licenseNo, cost;
    cout << "Enter Model of the car: \n";
    cin.ignore();
    getline(cin, name);
    cout << "Enter licenseNo : \n";
    cin >> licenseNo;
    cout << "Enter per day cost of the Car: \n";
    cin >> cost;
    fstream fout("cars.csv", ios::out | ios::app);
    fout << name << "," << licenseNo << ","
         << "100.0"
         << "," << cost << ",0" << '\n';
    cout << "New Car has been added.\n";
}

void Manager::delete_car()
{
    string licenseNo;
    cout << "Enter the license number of the Car : \n";
    cin >> licenseNo;
    content.clear();
    bool found = false;
    readFile("cars.csv");
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][1] == licenseNo)
        {
            found = true;
            content.erase(content.begin() + i, content.begin() + i + 1);
            break;
        }
    }
    writeFile(content, "cars.csv");
    content.clear();

    readFile("rented_cars.csv");
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][2] == licenseNo)
        {
            content.erase(content.begin() + i, content.begin() + i + 1);
        }
    }
    writeFile(content, "rented_cars.csv");
    content.clear();
    if (!found)
        cout << "Car not found.\n";
}

void Manager::update_car()
{
    string licenseNo;
    cout << "Enter the License number of the Car you want to update : \n";
    cin >> licenseNo;
    cout << "Enter the serial number of the field you want to update : \n";
    cout << "1. Model Name of the Car\n";
    cout << "2. Cost per Day of the Car\n";
    cout << "3. Send Car to Mechanic ( improve the condition )\n";
    char c;
    cin >> c;
    content.clear();
    bool found = false;
    string new_field;
    readFile("cars.csv");
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][1] == licenseNo)
        {
            found = true;
            if (c != '3')
            {
                cout << "Enter the new value of the field : \n";
                cin.ignore();
                getline(cin, new_field);
            }
            if (c == '1')
            {
                content[i][0] = new_field;
            }
            else if (c == '2')
            {
                content[i][3] = new_field;
            }
            else if (c == '3')
            {
                content[i][2] = "100.0";
                cout << "\nYour Car have been repaired to 100% health!!\n";
            }
            else
            {
                cout << "Invalid input!\n\n";
            }
            break;
        }
    }
    writeFile(content, "cars.csv");
    content.clear();

    readFile("rented_cars.csv");
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][2] == licenseNo)
        {
            found = true;
            if (c == '1')
            {
                content[i][1] = new_field;
            }
            break;
        }
    }
    writeFile(content, "rented_cars.csv");
    cout << "Car Details updated\n";

    if (!found)
        cout << "Car was not found.\n\n";
}

void Manager::see_rented_to_user()
{
    string uid;
    cout << "\nEnter Id of the User you want to know about :\n";
    cin >> uid;
    vector<string> licenseNo;
    content.clear();

    int count = 1;
    readFile("rented_cars.csv");
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][0] == uid)
        {
            licenseNo.push_back(content[i][2]);
        }
    }
    content.clear();

    if (licenseNo.size() == 0)
        cout << "\nNo Car has been rented by this User.\n\n";
    else
    {
        readFile("cars.csv");
        for (int d = 0; d < licenseNo.size(); d++)
        {
            for (int i = 0; i < content.size(); i++)
            {
                if (content[i][1] == licenseNo[d])
                {
                    ;
                    cout << count << ". ";
                    count++;
                    for (auto y : content[i])
                    {
                        if (y != content[i].back())
                            cout << y << " | ";
                        else
                            cout << y << "\n";
                    }
                }
            }
        }
        content.clear();
    }
}

void Manager::see_rented_car_to(string &licenseNo)
{
    content.clear();
    int count = 0;
    readFile("rented_cars.csv");
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][2] == licenseNo)
        {
            count = 1;
            cout << "Car with LicenseNo " << licenseNo << " and Model Name " << content[i][1] << " is rented to user with id : " << content[i][0] << "\n";
        }
    }
    if (!count)
        cout << "Car has not been rented to anyone.\n\n";
}

void Car::show_duedate(string licenseNo)
{

    content.clear();
    string uid;
    time_t stamp;
    readFile("rented_cars.csv");
    int fl = 0;
    int days = 0;
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][2] == licenseNo)
        {
            fl = 1;
            uid = content[i][0];
            stamp = stoi(content[i][3]);
            days = stoi(content[i][4]);
            break;
        }
    }
    content.clear();

    stamp += days * 86400;
    tm *due_time = localtime(&stamp);
    if (fl == 0)
        cout << "Car was not rented!\n";
    else
    {
        cout << "Due date of the Car is : ";
        cout << due_time->tm_mday << "/" << 1 + due_time->tm_mon << "/" << 1900 + due_time->tm_year << "\n";
    }
}

// void Car::rent_request(string carName, UserType usertype){
//     rent_car(carName, usertype);
// }

void Manager::repair_car()
{
    cout << "\nEnter the licenseNo of the Car you want to repair :\n";
    string licenseNo;
    cin >> licenseNo;
    bool found = false;
    content.clear();
    readFile("cars.csv");
    for (auto &line : content)
    {
        if (line[1] == licenseNo)
        {
            found = true;
            line[2] = "100.0";
            break;
        }
    }
    writeFile(content, "cars.csv");
    if (found)
        cout << "\nYour Car has been repaired to best Condition!\n";
    else
    {
        cout << "Could not find the given License Number!\n";
    }
    content.clear();
}

void Manager::display_menu()
{
    cout << "----------------------------M-----------------------------------------\n";
    cout << "\nWelcome " << id << "! You are logged in as Manager.\n\n";
    cout << "Press 1 to add a User\n";
    cout << "Press 2 to update a User\n";
    cout << "Press 3 to delete a User\n";
    cout << "Press 4 to add a Car\n";
    cout << "Press 5 to update details of a Car\n";
    cout << "Press 6 to remove a Car\n";
    cout << "Press 7 to see all Cars rented by particular User\n";
    cout << "Press 8 to see which user has rented a particular Car\n";
    cout << "Press 9 to view all Users\n";
    cout << "Press A to view all Cars\n";
    cout << "Press B to show due date of a Car\n";
    cout << "Press C to repair a Car\n";
    cout << "Press D to logout\n";
    cout << "----------------------------M-----------------------------------------\n";
    char c;
    cin >> c;
    Car car;
    string licenseNo, uid;
    switch (c)
    {
    case '1':
        add_user();
        display_menu();
        break;
    case '2':
        update_user();
        display_menu();
        break;
    case '3':
        user_delete();
        display_menu();
        break;
    case '4':
        add_car();
        display_menu();
        break;
    case '5':
        update_car();
        display_menu();
        break;
    case '6':
        delete_car();
        display_menu();
        break;
    case '7':
        see_rented_to_user();
        display_menu();
        break;
    case '8':
        cout << "Enter the License number of the Car : ";
        cin >> licenseNo;
        see_rented_car_to(licenseNo);
        display_menu();
        break;
    case '9':
        all_users();
        display_menu();
        break;
    case 'A':
        all_cars();
        display_menu();
        break;
    case 'B':
        cout << "Enter the licenseNo number of the Car : ";
        cin >> licenseNo;
        car.show_duedate(licenseNo);
        display_menu();
        break;
    case 'C':
        repair_car();
        display_menu();
        break;
    case 'D':
        logout();
        break;
    }
}

int main()
{
    User temp;
    temp.display_menu();
    return 0;
}