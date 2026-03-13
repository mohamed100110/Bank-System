
#include <iostream>
#include <string>
#include<vector>
#include <fstream>
#include <iomanip>

using namespace std;

const string FileName = "FileClient.txt";
enum choice
{
    show=1,add,Delete,update,find
};
struct st_client_data
{
    string acount_number;
    string pin_code;
    string name;
    string phone;
    double account_balance;
    bool make_delete = false;
};
string ReadAccountNum()
{
    string s;
    cout << "Please enter account number?  ";
    cin >> s;
    return s;
}
vector <string > split_string(string s, string delim = " ")
{
    vector <string >v_words;
    string sword;
    short pos = 0;
    while ((pos = s.find(delim)) != string::npos)
    {
        sword = s.substr(0, pos);
        if (sword != "")
        {
            v_words.push_back(sword);
        }
        s.erase(0, pos + delim.length());
    }
    if (s != "")
    {
        v_words.push_back(s);
    }

    return v_words;
}
st_client_data convert_line_to_record(string line, string delim = "#//#")
{
    vector <string >v_data;
    v_data = split_string(line, delim);

    st_client_data client_data;

    client_data.acount_number = v_data[0];
    client_data.pin_code = v_data[1];
    client_data.name = v_data[2];
    client_data.phone = v_data[3];
    client_data.account_balance = stod(v_data[4]);

    return client_data;
}
vector <string > get_lines_from_file(string FileName)
{
    vector <string >v_lines;
    fstream FileClient;
    FileClient.open(FileName, ios::in);
    string line;
    if (FileClient.is_open())
    {
        while (getline(FileClient, line))
        {
            v_lines.push_back(line);
        }

        FileClient.close();
    }
    return v_lines;
}
vector< st_client_data> get_clients_data()
{
    vector< st_client_data>v_client_records;

    vector <string >v_lines;
    v_lines = get_lines_from_file(FileName);

    st_client_data client_data;
    for (string& line : v_lines)
    {
        client_data = (convert_line_to_record(line));
        v_client_records.push_back(client_data);
    }
    return v_client_records;
}
bool is_found_client_by_account_number(vector< st_client_data>& v_client_records, st_client_data& client_data, string search)
{

    for (st_client_data& client_record : v_client_records)
    {
        if (client_record.acount_number == search)
        {
            client_data = client_record;
            return true;
        }
    }
    return false;
}
string convert_record_to_line(st_client_data client_data, string delim = "#//#")
{
    string line = "";
    line += client_data.acount_number + delim;
    line += client_data.pin_code + delim;
    line += client_data.name + delim;
    line += client_data.phone + delim;
    line += to_string(client_data.account_balance);

    return line;
}
void print_client_data(st_client_data& client_data)
{
    cout << "\n\nthe following are the client details: \n\n";

    cout << "account number:" << client_data.acount_number << endl;
    cout << "pin code      :" << client_data.pin_code << endl;
    cout << "name          :" << client_data.name << endl;
    cout << "phone         :" << client_data.phone << endl;
    cout << "account balance:" << client_data.account_balance << endl;
}
void save_vector_to_file_by_mark_delete(vector< st_client_data>& v_client_records, string FileName)
{

    fstream FileClient;

    FileClient.open(FileName, ios::out);
    string line;
    if (FileClient.is_open())
    {
        for (st_client_data& c : v_client_records)
        {
            if (!c.make_delete)
            {
                line = convert_record_to_line(c);
                FileClient << line << endl;
            }
        }


        FileClient.close();
    }
}
bool is_delete()
{
    char is_remove;
    cout << "\n\nAre you sure you want delete this client? y/n ? ";
    cin >> is_remove;

    is_remove = tolower(is_remove);

    if (is_remove == 'y')
        return true;
    else
        return false;
}
void mark_client_for_delete_by_account_number(vector< st_client_data>& v_client_records, string account_number)
{
    for (st_client_data& client_record : v_client_records)
    {
        if (account_number == client_record.acount_number)
        {
            client_record.make_delete = true;
            break;
        }
    }
}
bool delete_client_by_account_number(vector< st_client_data>& v_client_records, string account_number)
{
    st_client_data client_data;
    if (is_found_client_by_account_number(v_client_records, client_data, account_number))
    {
        print_client_data(client_data);

        if (is_delete())
        {
            mark_client_for_delete_by_account_number(v_client_records, account_number);
            save_vector_to_file_by_mark_delete(v_client_records, FileName);

            //refresh vector
            v_client_records = get_clients_data();
            cout << "\n\nclient deleted successfully.\n";
            return true;
        }
    }
    else
    {
        cout << "\n\nClient with Account Number (" << account_number << ") is Not Found!\n";
        return false;
    }
}
void delete_client()
{
    cout << "_________________________________\n\n";
    cout << "       Delete Client Screan    \n";
    cout << "_________________________________\n\n\n";
    vector< st_client_data>v_client_records = get_clients_data();
    string account_number = ReadAccountNum();

    delete_client_by_account_number(v_client_records, account_number);
}
st_client_data read_client_data(st_client_data& client_data)
{


    cout << "enter pin code?   ";
    cin >> client_data.pin_code;

    cout << "enter name?    ";
    getline(cin >> ws, client_data.name);

    cout << "enter phone?    ";
    cin >> client_data.phone;

    cout << "enter acountbalance?   ";
    cin >> client_data.account_balance;

    return client_data;
}
bool is_update()
{
    char is_remove;
    cout << "\n\nAre you sure you want update this client? y/n ? ";
    cin >> is_remove;

    is_remove = tolower(is_remove);

    if (is_remove == 'y')
        return true;
    else
        return false;
}
void mark_client_update_by_account_number(vector< st_client_data>& v_client_records, string account_number)
{
    for (st_client_data& client_record : v_client_records)
    {
        if (account_number == client_record.acount_number)
        {
            read_client_data(client_record);
            break;
        }
    }
}
bool update_client_by_account_number(vector< st_client_data>& v_client_records, string account_number)
{
    st_client_data client_data;
    if (is_found_client_by_account_number(v_client_records, client_data, account_number))
    {
        print_client_data(client_data);

        if (is_update())
        {
            mark_client_update_by_account_number(v_client_records, account_number);
            save_vector_to_file_by_mark_delete(v_client_records, FileName);

            cout << "\n\nclient updated successfully.\n";
            return true;
        }
    }
    else
    {
        cout << "\n\nClient with Account Number (" << account_number << ") is Not Found!\n";
        return false;
    }
}
void update_client()
{
    cout << "_________________________________\n\n";
    cout << "     Update Client Info Screan    \n";
    cout << "_________________________________\n\n\n";
    vector< st_client_data>v_client_records = get_clients_data();
    string account_number = ReadAccountNum();

    update_client_by_account_number(v_client_records, account_number);
}
void save_line_in_file(string FileName, string line)
{
    fstream FileClient;
    FileClient.open(FileName, ios::out | ios::app);

    if (FileClient.is_open())
    {
        FileClient << line << endl;
        FileClient.close();
    }


}
void add_new_client()
{
    bool is_found = false;
    vector< st_client_data> v_client_records= get_clients_data();
    st_client_data client_data;
    cout << "enter acount number?  ";
    cin >> client_data.acount_number;
    do
    {
        if (is_found)
        {
            cout << "Client with [" << client_data.acount_number << "] aready exists,Enter another account number?  ";
            cin >> client_data.acount_number;
        }
        is_found = is_found_client_by_account_number(v_client_records, client_data, client_data.acount_number);
    } while (is_found);
    client_data = read_client_data(client_data);
   
    save_line_in_file(FileName, convert_record_to_line(client_data));
}
void add_clients()
{
    char is_more = 'y';

    while (tolower(is_more) == 'y')
    {
        system("cls");
        cout << "_________________________________\n\n";
        cout << "         Add new clients screan  \n";
        cout << "_________________________________\n";


        cout << "adding new client:\n\n";

        add_new_client();

        cout << "\nClient Added Successfully, do you want to add more clients ? Y / N ? ";
        cin >> is_more;

    }

}
void header(int num)
{
    printf("                          client list (%d) client(s).\n", num);
    cout << "__________________________________________________________________________________________\n\n";
    cout << "|Count Number     |Pin code     |Client Name                 |Phone         |Balance \n";
    cout << "__________________________________________________________________________________________\n";
}
void show_clients_list()
{
    vector< st_client_data>v_client_records = get_clients_data();

    header(v_client_records.size());
    for (st_client_data& client_record : v_client_records)
    {
        cout << left << setw(18) << "|" + client_record.acount_number
            << setw(14) << "|" + client_record.pin_code
            << setw(29) << "|" + client_record.name
            << setw(15) << "|" + client_record.phone
            << "|" << client_record.account_balance << endl;
    }
    cout << "__________________________________________________________________________________________\n";

}
void find_client()
{
    cout << "_________________________________\n\n";
    cout << "       Find Client Screan    \n";
    cout << "_________________________________\n\n\n";
    vector< st_client_data> v_client_records = get_clients_data();

    st_client_data client_data;
    string account_number = ReadAccountNum();

    if (is_found_client_by_account_number(v_client_records, client_data, account_number))
    {
        print_client_data(client_data);
    }
    else
    {
        cout << "\n\nClient with Account Number (" << account_number << ") is Not Found!\n";
    }
   
}
int read_num_from1_to6()
{
    int n;
    do
    {
        cout << "Choose what do you want to do? [1 to 6 ]?  ";
        cin >> n;
    } while (n < 1 || n>6);
    return n;
}
void choices_center(bool &is_exit)
{
    int num = read_num_from1_to6();

    system("cls");
    switch(num)
    {
    case choice::show:
        show_clients_list();
        break;
    case choice::add :
        add_clients();
        break;
    case choice::Delete :
        delete_client();
        break;
    case choice::update :
        update_client();
        break;
    case choice::find :
        find_client();
        break;
    default:
        is_exit = true;
        break;
    }

}
void show_main_menu()
{
    system("cls");
    cout << "=============================================\n";
    cout << "                Main Menu Screan             \n";
    cout << "=============================================\n";
    cout << "        [1] Show Client List.\n";
    cout << "        [2] Add New Client.\n";
    cout << "        [3] Delete Client.\n";
    cout << "        [4] Update Client Data.\n";
    cout << "        [5] find Client.\n";
    cout << "        [6] Exist.\n";
    cout << "=============================================\n";
}
void bank1()
{
    bool is_exit=false ;
    do
    {
        show_main_menu();
        choices_center(is_exit);
        if (is_exit)
        {
            cout << "_________________________________\n\n";
            cout << "         Program Ends :-)    \n";
            cout << "_________________________________\n\n\n";

            break;
        }

        cout << "\n\n\nPress any key to go back to main menue...";
         system("pause > 0");
    } while (true);

}
int main()
{
    bank1();
    system("pause");
}

