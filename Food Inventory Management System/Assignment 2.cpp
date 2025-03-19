#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <windows.h>  //for decoration purposes: use to make loading screen
#include <conio.h> //for decoration purposes: take user input and display '*' instead of actual input (use for take user's password input)
using namespace std;

struct account {
    string name, pwd, pwd_con;
};

struct item {
    string name, description;
    double price;
    int quantity;
};

void system_ui();
void system_ui_2();
void promptuserinput1(int&);
void promptuserinput2(int&);
void prompttodel(char&);
void promptto_viewhistory(char&);
void inputDeveloper();
void showuserpwd();

void input(string, string, string&);
void inputdigit(string, double&);
void inputdigit(string, int&);

void inputuserinfo(account&, ifstream&);
void regisadd();
void regisdel();
void showregislist();

void hidepwd(string&);
void removespaces(string&);
void asktocon(string, string, char&);
bool issamename(string&, ifstream&);
bool ispwdmatch(account&, ifstream&);

void login(bool&);

void addfood();
void delfood();
void editfood(item&);
void showfoodlist();
void history_foodlist();
void showhistory_foodlist();

void searchfood();
void loading();
void searching();

int main()
{
    char choice = 'y', delete_choice, viewhistory_choice;
    int main_choice, login_choice;
    bool islogin, exit = false;
    do
    {
        system("cls");
        promptuserinput1(main_choice);
        if (main_choice == 1)
            regisadd();
        else if(main_choice == 2)
        {
            prompttodel(delete_choice);
            if (delete_choice == 'y')
                regisdel();
        }
        else if(main_choice == 3)
        {
            login(islogin);
            if (islogin)
            {
                exit = false;
                do
                {
                    system("cls");
                    promptuserinput2(login_choice);
                    switch (login_choice)
                    {
                    case 1:
                    {
                        system("cls");
                        promptto_viewhistory(viewhistory_choice);
                        system("cls");
                        if (viewhistory_choice == 'y')
                        {
                            showhistory_foodlist();
                            system("pause");
                        }
                        break;
                    }
                    case 2:
                    {
                        addfood();
                        break;
                    }
                    case 3:
                    {
                        delfood();
                        break;
                    }
                    case 4:
                    {
                        searching();
                        searchfood();
                        break;
                    }
                    case 5:
                    {
                        exit = true;
                        loading();
                    }
                    }
                } while (exit != true);
            }
        }
        else if (main_choice == 4)
        {
            inputDeveloper();
            system("pause");
        }
        else
        {
            loading();
            cout << "The system is successfully exit." << endl;
        }
    } while (main_choice != 5);

    system("pause");
    return 0;
}


void regisadd() //add new account
{
    ofstream outfile("register_list.txt", ios::app);
    ifstream infile("register_list.txt");
    system("cls");
    char ch, choice;
    account regis;
    string line;

    do {
        inputuserinfo(regis, infile);
        if (regis.name == "-1")
            break;
        while (true) {
            if (regis.pwd_con != regis.pwd)
            {
                system("cls");
                cout << "Wrong password!\nPlease re-enter your password or type \"edit\" to make new register: ";
                hidepwd(regis.pwd_con);
                if (regis.pwd_con == "edit")
                    inputuserinfo(regis, infile);
            }
            else
                break;
        }
        asktocon("Password matched!", "\nDo you want to edit your information?(y/n): ", choice);
        if (choice == 'y')
            system("cls");
        else
            break;
    } while (choice == 'y');
    loading();
    if (regis.name != "-1")
    {
        cout << "Your account has successfully registered!" << endl;
        outfile << regis.name << endl;
        outfile << regis.pwd << endl;
        Sleep(2000);
    }

    infile.close();
    outfile.close();
}

void regisdel() //delete account
{
    account user;
    string line;
    ifstream infile("register_list.txt");
    ofstream tempfile("temp.txt");

    showregislist();
    input("Enter the account user name that you want to delete(-1 to exit): ", "ERROR! You must enter a name.", user.name);
    if (user.name == "-1")
    {
        infile.clear();
        infile.seekg(0);
        infile.close();
        tempfile.close();
        return;
    }
    while (!issamename(user.name, infile))
    {
        system("cls");
        showregislist();
        cout << "The user name is not found!" << endl;
        cout << "Please enter the account user name that you want to delete: ";
        getline(cin, user.name);
    }
    infile.clear(); // reset the file stream
    infile.seekg(0); // move the file position indicator to the beginning of the file

    cout << "Please enter the password(-1 to exit): ";
    getline(cin, user.pwd);
    if (user.pwd == "-1")
    {
        loading();
        infile.close();
        tempfile.close();
        return;
    }
    while (!ispwdmatch(user, infile))
    {
        system("cls");
        cout << "Wrong password!" << endl;
        cout << "Please enter the password(-1 to exit): ";
        getline(cin, user.pwd);
        if (user.pwd == "-1")
        {
            loading();
            return;
        }
    }
    loading();
    infile.clear();
    infile.seekg(0);
    int count = 0;
    while (getline(infile, line))
    {
        if (count == 1)
        {
            count = 0;
            continue;
        }
        else if (line != user.name)
            tempfile << line << endl;
        else
            count++;
    }

    infile.clear();
    infile.seekg(0);

    infile.close();
    tempfile.close();

    remove("register_list.txt");
    rename("temp.txt", "register_list.txt");
    cout << "Your account has successfully deleted!" << endl;
    Sleep(2000);
}

void inputuserinfo(account& info, ifstream& infile) //prompt user to enter user name and password
{
    bool con = true;
    input("Enter the user name (-1 to exit): ", "ERROR! You must enter a name.", info.name);

    while (issamename(info.name, infile)) //check file list is there same user name
    {
        system("cls");
        cout << "The user name has already used!" << endl;
        cout << "Enter new user name: ";
        getline(cin, info.name);
        removespaces(info.name);
        while (info.name.length() == 0)
        {
            system("cls");
            cout << "ERROR: You must enter a name. Enter a new user name: ";
            getline(cin, info.name);
            removespaces(info.name);
        }
    }
    infile.clear();
    infile.seekg(0);

    if (info.name == "-1")
        con = false;

    while (con)//prompt user to input password
    {

        cout << endl << "Enter the password: ";
        hidepwd(info.pwd);

        if (info.pwd == "edit")//password cannot be "edit"
        {
            system("cls");
            cout << "The password is invalid! Please use password other than \"edit\"\n";
            cout << "Enter new user name: " << info.name << endl;
        }
        else if (info.pwd.length() < 6)//ask user to input password that not too short
        {
            while (info.pwd.length() < 6)
            {
                system("cls");
                cout << "The password is too short! Please use password more than 6 characters\n";
                cout << "Enter the password: ";
                hidepwd(info.pwd);
            }
        }
        if (info.pwd != "edit")
            break;
    }

    if (con)
    {
        cout << endl << "Confirm password: ";
        hidepwd(info.pwd_con);
    }
}

void showregislist()
{
    system("cls");
    cout << setw(25) << left << '-';
    cout << "Register list";
    cout << setw(25) << right << '-';
    cout << endl;
    for (int i = 1; i <= 63; i++)
        cout << '#';
    cout << endl;

    ifstream infile("register_list.txt");
    string line;
    int line_num = 0, count = 0;
    while (getline(infile, line))
    {
        line_num++;
        if (count == 1)
        {
            line_num--;
            count = 0;
            continue;
        }
        count++;
        cout << "# " << line_num << ". " << setw(57) << left << line << '#' << endl;
    }
    for (int i = 1; i <= 63; i++)
        cout << '-';
    cout << endl;
    infile.close();
}

void asktocon(string content, string content2, char& choice) //ask user to continue or not until user enter y or n
{
    string line;
    do
    {
        system("cls");
        cout << content;
        cout << content2;
        getline(cin, line);
        while (line.length() > 1)
        {
            system("cls");
            cout << content;
            cout << content2;
            getline(cin, line);
        }
        choice = line[0];
    } while (choice != 'y' && choice != 'n');
}

bool issamename(string& user_name, ifstream& infile) //check file list is there same user name
{
    string line;
    int count = 0;
    infile.clear(); // reset the file stream
    infile.seekg(0); // move the file position indicator to the beginning of the file
    while (getline(infile, line)) //check file list is there same user name
    {
        if (count == 1)
        {
            count = 0;
            continue;
        }
        count++;
        if (user_name == line)
            return true;
    }
    return false;
}

bool ispwdmatch(account& regis, ifstream& infile) //check password is match with user name
{
    string line;
    int count = 0;
    infile.clear();
    infile.seekg(0);
    while (getline(infile, line)) //check file list is there same user name
    {
        if (count == 1)
        {
            count = 0;
            continue;
        }
        count++;
        if (regis.name == line)
        {
            getline(infile, line);
            if (regis.pwd == line)
                return true;
            else
                return false;
        }
    }
    return false;
}

void removespaces(string& user) //remove spaces in the front and back of user name
{
    if (user.empty() || user.find_first_not_of(' ') == string::npos) {
        user.clear();
        return;
    }

    user.erase(0, user.find_first_not_of(' '));
    user.erase(user.find_last_not_of(' ') + 1);
}

void hidepwd(string& pwd)//a function that is used to take user input password but show * instead
{
    char ch; //character variable to store the inputted character
    pwd = ""; //reset the pwd variable to an empty string
    while (true)
    {
        ch = _getch();//_getch() is used to take input from the user without displaying it on the console screen.
        if (ch == 13)//13 is the ASCII value of Enter key, when the user presses Enter, the loop is terminated.
            break;
        else if (ch == 8) //8 is the ASCII value of Backspace key, when the user presses Backspace, the last inputted character is removed from the pwd variable.
        {
            if (pwd.length() > 0)
            {
                cout << "\b \b"; //\b is used to move the cursor one position back, and a space is printed to overwrite the last inputted character, and then the cursor is moved back one position.
                pwd = pwd.substr(0, pwd.length() - 1); //the last character is removed from the pwd variable.
            }
        }
        else
        {
            cout << "*"; //a * is printed on the console screen. instead of showing the actual character.
            pwd += ch; //the inputted character is added to the pwd variable.
        }
    }

    cout << endl;
}

void login(bool& islogin) //login if user already have account
{
    ifstream infile("register_list.txt");
    account login;
    islogin = false;
    char choice, again;
    do
    {
        system("cls");
        cout << "Username(-1 to exit): ";
        getline(cin, login.name);
        removespaces(login.name);
        if (login.name == "-1")
        {
            loading();
            break;
        }
        if (!issamename(login.name, infile))
            asktocon("The user name is not found! Please try again!\n", "Do you want to try again? (y/n): ", again);
        else
        {
            do {
                choice = 'y';
                system("cls");
                cout << "Username: " << login.name << endl;
                cout << "Password: ";
                hidepwd(login.pwd);
                if (ispwdmatch(login, infile))
                {
                    system("cls");
                    loading();
                    cout << "Login successfully!" << endl;
                    islogin = true;
                    Sleep(2000);
                    again = 'n';
                    break;
                }
                else
                {
                    islogin = false;
                    asktocon("The password is incorrect! Please try again!\n", "Do you want to try again? (y/n): ", choice);
                    if (choice == 'n')
                        asktocon("", "Do you want to login again? (y/n): ", again);
                    else
                        again = 'n';
                }
            } while (choice == 'y');
            if (login.name == "-1")
                break;
        }
    } while (again == 'y');

    infile.close();
}

void input(string content, string content2, string& input) //prompt user to input that is not empty and if inputare spaces at the front and end, it will remove the spaces
{
    cout << content;
    getline(cin, input);
    removespaces(input);
    if (input == "-1")
        return;
    while (input.length() == 0)
    {
        system("cls");
        cout << content2 << endl;
        cout << content;
        getline(cin, input);
        removespaces(input);
        if (input == "-1")
            return;
    }
}

void inputdigit(string content, double& price)//prompt user to enter the food price
{
    system("cls");
    cout << content;
    do {
        cin >> price;
        while (cin.fail()) {
            system("cls");
            cout << "ERROR! You must enter a number." << endl << endl << content;
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> price;
        }
        while (price < 0) {
            system("cls");
            cout << "ERROR! You must enter a positive number." << endl << endl << content;
            cin.ignore(1000, '\n');
            cin >> price;
        }
    } while (cin.fail() || price < 0);
    cin.ignore();
}

void inputdigit(string content, int& quantity)//prompt user to enter the quantity
{
    system("cls");
    cout << content;
    do {
        cin >> quantity;
        while (cin.fail()) {
            system("cls");
            cout << "ERROR! You must enter a number." << endl << endl << content;
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> quantity;
        }
        while (quantity < 0) {
            system("cls");
            cout << "ERROR! You must enter a positive number." << endl << endl << content;
            cin.ignore(1000, '\n');
            cin >> quantity;
        }
    } while (cin.fail() || quantity < 0);
    cin.ignore();
}

void addfood() //add food to the food inventory
{
    ofstream outfile("food_list.txt", ios::app);
    ifstream infile("food_list.txt");
    item food;
    char choice = 'y';

    while (choice == 'y')
    {
        while (choice == 'y')
        {
            char edit = 'y';
            bool skip = false;
            system("cls");
            input("Enter the food name(-1 to exit): ", "Error! You must enter a name.", food.name);
            if (food.name == "-1")
            {
                infile.clear();
                infile.seekg(0);
                infile.close();
                outfile.close();
                return;
            }
            while (issamename(food.name, infile))
            {
                system("cls");
                cout << "The food has already in the inventory!" << endl;
                asktocon("", "Do you want to edit the food information? (y/n): ", edit);
                if (edit == 'y')
                    skip = true;
                system("cls");
                break;
            }
            infile.clear();
            infile.seekg(0);

            if (edit == 'n')
            {
                asktocon("", "Do you want to continue add food? (y/n): ", choice);
                break;
            }

            inputdigit("Enter the food price: ", food.price);
            inputdigit("Enter the food quantity: ", food.quantity);
            system("cls");
            cout << "Enter the food description: ";
            getline(cin, food.description);
            if (food.description.length() == 0)
                food.description = ' ';
            history_foodlist();

            if (skip == false)
            {
                outfile << food.name << endl;
                outfile << food.price << endl;
                outfile << food.quantity << endl;
                outfile << food.description << endl;
            }
            else
            {
                infile.close();
                outfile.close();
                history_foodlist();
                editfood(food);
            }

            asktocon("The food is added successfully!\n", "Do you want to add another food? (y/n): ", choice);
        }
    }
    infile.close();
    outfile.close();
}

void editfood(item& food) //edit the food information
{
    ifstream infile("food_list.txt");
    ofstream outfile("temp.txt");
    string line;

    while (getline(infile, line))
    {
        if (line != food.name)
            outfile << line << endl;
        else
        {
            outfile << food.name << endl;
            getline(infile, line);
            outfile << food.price << endl;
            getline(infile, line);
            outfile << food.quantity << endl;
            getline(infile, line);
            outfile << food.description << endl;
        }
    }

    infile.close();
    outfile.close();

    remove("food_list.txt");
    rename("temp.txt", "food_list.txt");

}

void delfood()//delete the food that added by the user
{
    ifstream infile("food_list.txt");
    ofstream outfile("temp.txt");
    string name, line;
    char choice = 'y';

    while (choice == 'y')
    {
        system("cls");
        showfoodlist();
        input("Enter the food name(-1 to exit): ", "Error! You must enter a name.", name);
        if (name == "-1")
            break;
        if (!issamename(name, infile))
            asktocon("The food is not found! Please try again!\n", "Do you want to try again? (y/n): ", choice);
        else
        {

            infile.clear();
            infile.seekg(0);
            while (getline(infile, line))
            {
                if (line != name)
                    outfile << line << endl;
                else
                {
                    getline(infile, line);
                    getline(infile, line);
                    getline(infile, line);
                }
            }

            infile.close();
            outfile.close();
            remove("food_list.txt");
            rename("temp.txt", "food_list.txt");
            history_foodlist();
            asktocon("The food is deleted successfully!\n", "Do you want to delete another food? (y/n): ", choice);
        }
    }
    infile.clear();
    infile.seekg(0);
    infile.close();
    outfile.close();
}

void showfoodlist()
{
    ifstream infile("food_list.txt");
    string line;
    int count = 0;

    for (int i = 1; i <= 120; i++)
        cout << '#';
    cout << endl;
    cout << '*' << setw(119) << '*';
    cout << endl;
    cout << '*' << setw(65) << "@ Food Inventory @" << setw(54) << '*';
    cout << endl;
    cout << '*' << setw(119) << '*';

    for (int i = 1; i <= 120; i++)
        cout << '-';

    cout << '#' << setw(3) << "No" << setw(2) << '#';
    cout << setw(17) << "Food_Name" << setw(10) << '#'
        << setw(8) << "Price" << setw(4) << '#'
        << setw(9) << "Quantity" << setw(2) << '#'
        << setw(36) << "Description" << setw(28) << '#';
    cout << endl;

    cout << '#' << "----#";
    for (int i = 1; i <= 26; i++)
        cout << '-';

    cout << "#-----------#----------#";

    for (int i = 1; i <= 63; i++)
        cout << '-';
    cout << '#';
    cout << endl;

    while (getline(infile, line))
    {
        count++;
        cout << '#' << right << setw(3) << count << setw(2) << "#" << " " 
            << left << setw(25) << line << '#';
        getline(infile, line);
        cout << " " << setw(10) << fixed << setprecision(2) << showpoint << line << '#';
        getline(infile, line);
        cout << setw(1) << " " << setw(9) << line << '#';
        getline(infile, line);
        cout << setw(1) << " " << setw(62) << line << '#' << endl;
    }
    for (int i = 1; i <= 120; i++)
        cout << '#';
    infile.close();
}

void history_foodlist()//copy the food list to the history food list
{
    ifstream infile("food_list.txt");
    ofstream outfile("history_foodlist.txt");
    string line;

    while (getline(infile, line))
        outfile << line << endl;

    infile.close();
}

void showhistory_foodlist()
{
    ifstream infile("history_foodlist.txt");
    string line;
    int count = 0;

    system("cls");
    for (int i = 1; i <= 120; i++)
        cout << '#';
    cout << endl;
    cout << '*' << setw(119) << right << '*';
    cout << endl;
    cout << '*' << setw(65) << "@ Food Inventory @" << setw(54) << '*';
    cout << endl;
    cout << '*' << setw(119) << '*';

    for (int i = 1; i <= 120; i++)
        cout << '-';

    cout << '#' << setw(3) << "No" << setw(2) << '#';
    cout << setw(17) << "Food_Name" << setw(10) << '#'
        << setw(8) << "Price" << setw(4) << '#'
        << setw(9) << "Quantity" << setw(2) << '#'
        << setw(36) << "Description" << setw(28) << '#';
    cout << endl;

    cout << '#' << "----#";
    for (int i = 1; i <= 26; i++)
        cout << '-';

    cout << "#-----------#----------#";

    for (int i = 1; i <= 63; i++)
        cout << '-';
    cout << '#';
    cout << endl;

    while (getline(infile, line))
    {
        count++;
        cout << '#' << right << setw(3) << count << setw(2) << "#" << " "
            << left << setw(25) << line << '#';
        getline(infile, line);
        cout << " " << setw(10) << fixed << setprecision(2) << showpoint << line << '#';
        getline(infile, line);
        cout << setw(1) << " " << setw(9) << line << '#';
        getline(infile, line);
        cout << setw(1) << " " << setw(62) << line << '#' << endl;
    }
    for (int i = 1; i <= 120; i++)
        cout << '#';
    infile.close();
}

void searchfood()//search the food that the food name is included user input
{
    string name, line;
    char choice = 'y';

    while (choice == 'y')
    {
        ifstream infile("food_list.txt");
        int notfound = 0;
        int count = 0;
        do {
            system("cls");
            showfoodlist();
            cout << "Enter the food name you want to search or -1 to exit: ";
            getline(cin, name);
            removespaces(name);
        } while (name.length() == 0);

        if (name == "-1")
            break;
        else
        {
            infile.clear();//clear the eof flag
            infile.seekg(0);//reset the pointer to the beginning of the file
            system("cls");
            cout << "Searching Result: " << endl;
            for (int i = 1; i <= 120; i++)
                cout << '#';
            cout << '#' << right << setw(3) << "No" << setw(2) << '#';
            cout << setw(17) << "Food_Name" << setw(10) << '#'
                << setw(8) << "Price" << setw(4) << '#'
                << setw(9) << "Quantity" << setw(2) << '#'
                << setw(36) << "Description" << setw(28) << '#';
            cout << endl;

            cout << '#' << "----#";
            for (int i = 1; i <= 26; i++)
                cout << '-';

            cout << "#-----------#----------#";

            for (int i = 1; i <= 63; i++)
                cout << '-';
            cout << '#';
            cout << endl;

            

            while (getline(infile, line))
            {
                count++;
                if (line.find(name) != string::npos)
                {
                    cout << '#' << right << setw(3) << count << setw(2) << "#" << " "
                        << left << setw(25) << line << '#';
                    getline(infile, line);
                    cout << " " << setw(10) << fixed << setprecision(2) << showpoint << line << '#';
                    getline(infile, line);
                    cout << setw(1) << " " << setw(9) << line << '#';
                    getline(infile, line);
                    cout << setw(1) << " " << setw(62) << line << '#' << endl;
                }
                else
                {
                    notfound++;
                    getline(infile, line);
                    getline(infile, line);
                    getline(infile, line);
                }
            }
            for (int i = 1; i <= 120; i++)
                cout << '#';
        }
        infile.close();
        system("pause");

        if (notfound == count)
            asktocon("The food is not found!\n", "Do you want to search another food? (y/n): ", choice);
        else
            asktocon("", "Do you want to search another food? (y/n): ", choice);
    }
}

void loading() //display loading message
{
    for (int i = 0; i < 2; i++)
    {
        cout << "Loading";
        for (int j = 0; j < 5; j++)
        {
            cout << ".";
            Sleep(500);
        }
        system("cls");
    }
}

void searching()
{
    for (int i = 0; i < 2; i++)
    {
        cout << "Searching for the Result";
        for (int j = 0; j < 5; j++)
        {
            cout << ".";
            Sleep(250);
        }
        system("cls");
    }
}

void system_ui()
{
    for (int i = 1; i <= 100; i++)
        cout << "=";

    cout << endl;
    for (int i = 1; i <= 4; i++)
        cout << "$" << right << setw(99) << "$" << endl;
    cout << "$" << setw(65) << right << "Welcome to OnlyFun Food Inventory Store" << setw(34) << "$" << endl;

    for (int i = 1; i <= 4; i++)
        cout << "$" << right << setw(99) << "$" << endl;

    cout << "$";
    for (int i = 1; i <= 98; i++)
        cout << "*";
    cout << "$" << endl;

    for (int i = 1; i <= 3; i++)
        cout << "$" << right << setw(99) << "$" << endl;
    cout << "$" << setw(55) << right << "& 1. Register an account &" << setw(44) << "$" << endl;
    cout << "$" << right << setw(99) << "$" << endl;
    cout << "$" << setw(55) << right << "& 2. View account        &" << setw(44) << "$" << endl;
    cout << "$" << right << setw(99) << "$" << endl;
    cout << "$" << setw(55) << right << "& 3. Login               &" << setw(44) << "$" << endl;
    cout << "$" << right << setw(99) << "$" << endl;
    cout << "$" << setw(55) << right << "& 4. View user password  &" << setw(44) << "$" << endl;
    cout << "$" << right << setw(99) << "$" << endl;
    cout << "$" << setw(55) << right << "& 5. Exit                &" << setw(44) << "$" << endl;

    for (int i = 1; i <= 3; i++)
        cout << "$" << right << setw(99) << "$" << endl;

    for (int i = 1; i <= 100; i++)
        cout << "=";
    cout << endl;
}

void promptuserinput1(int& input) 
{
    string content = "Select an option: ";
    do {
        system_ui();
        cout << content;
        cin >> input;
        while (cin.fail()) {
            system("cls");
            system_ui();
            cout << "ERROR! You must enter a number." << endl << content;
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> input;
        }
        while (input < 0 || input > 5) {
            system("cls");
            system_ui();
            cout << "Please select between 1 to 5." << endl << content;
            cin.ignore(1000, '\n');
            cin >> input;
        }
    } while (cin.fail() || input < 0 || input > 5);
    cin.ignore();
}

void prompttodel( char& choice) //ask user to continue or not until user enter y or n
{
    string content = "Do you want to delete user? (y/n): ";
    string line;
    do
    {
        showregislist();
        cout << content;
        getline(cin, line);
        while (line.length() > 1)
        {
            system("cls");
            showregislist();
            cout << content;
            getline(cin, line);
        }
        choice = line[0];
    } while (choice != 'y' && choice != 'n');
}

void system_ui_2()
{
    for (int i = 0; i <= 100; i++)
    {
        if (i % 2 == 0)
            cout << '*';
        else
            cout << "=";
    }

    cout << endl;
    for (int i = 1; i <= 4; i++)
    {
        if (i % 2 == 0)
            cout << "*" << right << setw(100) << "*" << endl;
        else
            cout << "$" << right << setw(100) << "$" << endl;
    }
    cout << "$" << setw(61) << right << "OnlyFun Food Inventory Store" << setw(39) << "$" << endl;

    for (int i = 1; i <= 4; i++)
    {
        if (!(i % 2 == 0))
            cout << "*" << right << setw(100) << "*" << endl;
        else
            cout << "$" << right << setw(100) << "$" << endl;
    }

    for (int i = 0; i <= 100; i++)
    {
        if (i % 2 == 0)
            cout << '*';
        else
            cout << "@";
    }

    cout << endl;
    for (int i = 1; i <= 3; i++)
    {
        if (i % 2 == 0)
            cout << "*" << right << setw(100) << "*" << endl;
        else
            cout << "$" << right << setw(100) << "$" << endl;
    }

    cout << "*" << setw(56) << right << "& 1. View Inventory      &" << setw(44) << "*" << endl;
    cout << "$" << right << setw(100) << "$" << endl;
    cout << "*" << setw(56) << right << "& 2. Add Food            &" << setw(44) << "*" << endl;
    cout << "$" << right << setw(100) << "$" << endl;
    cout << "*" << setw(56) << right << "& 3. Delete Food         &" << setw(44) << "*" << endl;
    cout << "$" << right << setw(100) << "$" << endl;
    cout << "*" << setw(56) << right << "& 4. Search Food         &" << setw(44) << "*" << endl;
    cout << "$" << right << setw(100) << "$" << endl;
    cout << "*" << setw(56) << right << "& 5. Exit                &" << setw(44) << "*" << endl;

    for (int i = 1; i <= 3; i++)
    {
        if (i % 2 == 0)
            cout << "*" << right << setw(100) << "*" << endl;
        else
            cout << "$" << right << setw(100) << "$" << endl;
    }

    for (int i = 0; i <= 100; i++)
    {
        if (i % 2 == 0)
            cout << '*';
        else
            cout << "=";
    }
}

void promptuserinput2(int& input)
{
    string content = "Select an option: ";
    do {
        system_ui_2();
        cout << endl;
        cout << content;
        cin >> input;
        while (cin.fail()) {
            system("cls");
            system_ui_2();
            cout << endl;
            cout << "ERROR! You must enter a number." << endl << content;
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> input;
        }
        while (input < 0 || input > 5) {
            system("cls");
            system_ui_2();
            cout << endl;
            cout << "Please select between 1 to 5." << endl << content;
            cin.ignore(1000, '\n');
            cin >> input;
        }
    } while (cin.fail() || input < 0 || input > 5);
    cin.ignore();
}

void promptto_viewhistory(char& choice) //ask user to continue or not until user enter y or n
{
    string content = "Do you want to view previous changes? (y/n): ";
    string line;
    do
    {
        showfoodlist();
        cout << content;
        getline(cin, line);
        while (line.length() > 1)
        {
            system("cls");
            showfoodlist();
            cout << content;
            getline(cin, line);
        }
        choice = line[0];
    } while (choice != 'y' && choice != 'n');
}

void inputDeveloper()
{
    char choice, again;
    string username, pwd;
    do
    {
        system("cls");
        cout << "Developer username: Sudo" << endl;
        cout << "Developer password: sudosudo" << endl << endl;
        cout << "Enter Developer username(-1 to exit): ";
        getline(cin, username);
        removespaces(username);
        if (username == "-1")
            return;
        if (username != "Sudo")
            asktocon("The user name is not found! Please try again!\n", "Do you want to try again? (y/n): ", again);
        else
        {
            do {
                choice = 'y';
                system("cls");
                cout << "Developer username: Sudo" << endl;
                cout << "Developer password: sudosudo" << endl << endl;
                cout << "Enter Developer username(-1 to exit): " << username;
                cout << "Password: ";
                hidepwd(pwd);
                if (pwd == "sudosudo")
                {
                    system("cls");
                    loading();
                    showuserpwd();
                    again = 'n';
                    break;
                }
                else
                {
                    asktocon("The password is incorrect! Please try again!\n", "Do you want to try again? (y/n): ", choice);
                    if (choice == 'n')
                        asktocon("", "Do you want to view password again? (y/n): ", again);
                    else
                        again = 'n';
                }
            } while (choice == 'y');

        }
    } while (again == 'y');
}

void showuserpwd()
{
    system("cls");
    cout << setw(25) << left << '-';
    cout << "Register list";
    cout << setw(25) << right << '-';
    cout << endl;
    for (int i = 1; i <= 63; i++)
        cout << '#';
    cout << endl;

    ifstream infile("register_list.txt");
    string line;
    int line_num = 0;
    while (getline(infile, line))
    {
        line_num++;
        cout << "# " << line_num << ". " << line << " - ";
        getline(infile, line);
        cout << setw(49) << left << line << '#' << endl;
        
    }
    for (int i = 1; i <= 63; i++)
        cout << '-';
    cout << endl;
    infile.close();
}

