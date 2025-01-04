#include "homework.h"

enum{flight, from, to, getoff, arrive, discount, price, empty};

unordered_map<string, int> info_table;
vector<vector<string>> info(0, vector<string>(8));

int main(void){
    if(!init()){
        cout << "Could not open file: information.txt";
        return 1;
    }
    string mode;
    system("cls");
    cout << "mode:";
    cin >> mode;
    system("cls");
    if(mode == "root"){
        cout << "root mode" << endl;
        bool ret_flag;
        int ret_val = root(ret_flag);
        if(ret_flag) return ret_val;
    }
    else if(mode == "user"){
        cout << "user mode" << endl;
        bool ret_flag;
        int ret_val = root(ret_flag);
        if(ret_flag) return ret_val;
    }
    else cout << "mode wrong" << endl;
}

int root(bool &root_ret_flag)
{
    root_ret_flag = true;
    bool ret_flag;
    fstream password_file;
    string password;
    int ret_val = check_in(ret_flag, password_file, password);
    if(ret_flag) return ret_val;
    while(1){
        cout << "ROOT MODE, PLEASE ENTER COMMAND(print \"help\" to check command)" << endl;
        string cmd;
        cin >> cmd;
        system("cls");
        print();
        if(cmd == "quit"){
            int ret_flag;
            quit(ret_flag);
            if(ret_flag == 2) break;
        }
        else if(cmd == "help") help();
        else if(cmd == "save") save();
        else if(cmd == "password"){
            bool ret_flag;
            int ret_val = password_block(password, password_file, ret_flag);
            if (ret_flag) return ret_val;
        }
        else if(cmd == "template") cout << "AAAA-AAAA-AAAA-1111-1111-1111-1111-1111" << endl;
        else if(cmd == "insert") insert();
        else if(cmd == "modify"){
            int ret_flag;
            while(true){
                modify(ret_flag);
                if(ret_flag == 3) continue;
                else if(ret_flag == 1) break;
            }
        }
        else if(cmd == "delete"){
            while(1){
                cout << "Enter flight number to delete(\"quit\" to break):" << endl;
                string flight_number;
                cin >> flight_number;
                system("cls");
                print();
                if(flight_number == "quit") break;
                if(info_table.find(flight_number) == info_table.end()){
                    cout << "The flight does not exist" << endl;
                    continue;
                }
                cout << "Are you sure to delete? (yes/no)" << endl;
                string confirm;
                while(1){
                    cin >> confirm;
                    system("cls");
                    print();
                    if(confirm == "yes"){
                        info.erase(info.begin() + info_table[flight_number]);
                        info_table.erase(flight_number);
                        shell_sort_info();
                        cout << "info deleted" << endl;
                        break;
                    }
                    else if(confirm == "no"){
                        cout << "deletion cancelled" << endl;
                        break;
                    }
                    else cout << "Invalid input, please enter again (yes/no):" << endl;
                }
            }
        }
        else{
            cout << "Invalid command" << endl;
            continue;
        }
    }
    root_ret_flag = false;
    return 0;
}

int user(bool &root_ret_flag)
{
    root_ret_flag = true;
    bool ret_flag;
    while(1){
        cout << "USER MODE, PLEASE ENTER COMMAND(print \"help\" to check command)" << endl;
        string cmd;
        cin >> cmd;
        system("cls");
        print();
    }
    root_ret_flag = false;
    return 0;
}

int check_in(bool &ret_flag, fstream& password_file, string& password){
    ret_flag = true;
    password_file.open("password.txt", ios::in);
    if(!password_file.is_open()){
        cout << "Can not open the file" << endl;
        system("cls");
        return 1;
    }
    if(!getline(password_file, password)){
        cout << "EOF" << endl;
        system("cls");
        return 1;
    }
    if(password == "123456"){
        cout << "Create a password:" << endl;
        cin >> password;
        system("cls");
        password_file.close();
        password_file.open("password.txt", ios::out | ios::trunc);
        password_file << password;
        password_file.close();
    }
    cout << "Please enter password:" << endl;
    string user_in;
    cin >> user_in;
    system("cls");
    password_file.open("password.txt", ios::in);
    password_file >> password;
    password_file.close();
    int attempts = 0;
    while(password != user_in && attempts < 4){
        cout << "wrong password, try again:" << endl;
        cin >> user_in;
        system("cls");
        attempts++;
    }
    if(attempts == 4){
        cout << "Too many failed attempts" << endl;
        return 2;
    }
    print();
    ret_flag = false;
    return {};
}

void quit(int &ret_flag)
{
    ret_flag = 1;

    fstream information_file;
    information_file.open("information.txt", ios::out | ios::trunc);
    for (const auto &row : info)
    {
        for (int i = 0; i < 8; i++)
        {
            information_file << row[i];
            if (i < 7)
                information_file << "-";
        }
        information_file << endl;
    }
    information_file.close();
    system("cls");
    print();
    {
        ret_flag = 2;
        return;
    };
}

void help(){
    cout << "quit: quit root mode" << endl;
    cout << "save: save the information" << endl;
    cout << "password: change the password" << endl;
    cout << "insert: insert new information" << endl;
    cout << "modify: modify information" << endl;
}

int password_block(string &password, fstream &password_file, bool &ret_flag)
{
    ret_flag = true;
    while (1)
    {
        bool success_set_flag = false;
        cout << "Enter the current password:" << endl;
        string current_password;
        cin >> current_password;
        system("cls");
        if (current_password == "quit")
            break;
        if (current_password != password)
        {
            cout << "Wrong password, try again or type 'quit' to cancel" << endl;
            continue;
        }
        print();
        cout << "New a password:" << endl;
        while (true)
        {
            cout << "Enter new password:" << endl;
            string new_password1, new_password2;
            cin >> new_password1;
            system("cls");
            print();
            if (new_password1 == "quit")
                break;
            cout << "Confirm new password:" << endl;
            cin >> new_password2;
            system("cls");
            print();
            if (new_password1 == new_password2)
            {
                password = new_password1;
                password_file.close();
                password_file.open("password.txt", ios::out | ios::trunc);
                if (!password_file.is_open())
                    return -1;
                password_file << password;
                password_file.close();
                cout << "Password changed successfully" << endl;
                success_set_flag = true;
                break;
            }
            else cout << "Passwords do not match, try again or type 'quit' to cancel" << endl;
        }
        if(success_set_flag) break;
    }
    ret_flag = false;
    return {};
}

void save()
{
    fstream information_file;
    information_file.open("information.txt", ios::out | ios::trunc);
    if 
    for(const auto &row : info){
        for(int i = 0; i < 8; i++){
            information_file << row[i];
            if (i < 7) information_file << "-";
        }
        information_file << endl;
    }
    information_file.close();
    system("cls");
    print();
}

bool format_check_insert(string s){
    int n = s.size();
    if(n != 39) return false;
    for(int i = 0; i < n; i++){
        if(i % 5 == 4){
            if(s[i] != '-') return false;
            continue;
        }
        if((i < 15 && !isupper(s[i])) || (i >= 15 && !isdigit(s[i]))) return false;
    }
    return true;
}

bool init(){
    fstream information_file;
    information_file.open("information.txt", ios::in);
    if(!information_file.is_open()) return false;
    string info_string;
    while(getline(information_file, info_string)){
        if(format_check_insert(info_string)){
            string index = info_string.substr(0, 4);
            vector<string> temp(8);
            for(int i = 0; i < 8; i++){
                temp[i] = info_string.substr(i * 5, 4);
            }
            info_table[index] = info.size();
            info.push_back(temp);
        }
        else return false;
    }
    shell_sort_info();
    
    return true;
}

void shell_sort_info() {
    int n = info.size();
    for(int gap = n / 2; gap > 0; gap /= 2){
        for(int i = gap; i < n; i++){
            vector<string> temp = info[i];
            string temp_index = temp[0];
            int j;
            for(j = i; j >= gap && info[j - gap][0] > temp_index; j -= gap){
                info[j] = info[j - gap];
                info_table[info[j][0]] = j;
            }
            info[j] = temp;
            info_table[temp_index] = j;
        }
    }
}

void modify(int &ret_flag)
{
    ret_flag = 1;
    cout << "Enter info to modify(\"quit\" to break):" << endl;
    string modify_info;
    cin >> modify_info;
    system("cls");
    print();
    if(modify_info == "quit"){
        ret_flag = 1;
        return;
    }
    if(!format_check_insert(modify_info)){
        cout << "format wrong" << endl;
        {
            ret_flag = 3;
            return;
        };
    }
    string index = modify_info.substr(0, 4);
    if(info_table.find(index) == info_table.end()){
        cout << "Are you sure to add? (yes/no)" << endl;
        string confirm;
        while(1){
            cin >> confirm;
            system("cls");
            print();
            if(confirm == "yes"){
                vector<string> temp(8);
                for (int i = 0; i < 8; i++)
                {
                    temp[i] = modify_info.substr(i * 5, 4);
                }
                info_table[index] = info.size();
                info.push_back(temp);
                shell_sort_info();
                break;
            }
            else if(confirm == "no"){
                cout << "modification cancelled" << endl;
                break;
            }
            else{
                cout << "Invalid input, please enter again (yes/no):" << endl;
                continue;
            }
        }
    }
    else{
        cout << "Are you sure to modify? (yes/no)" << endl;
        string confirm;
        while(true){
            cin >> confirm;
            system("cls");
            print();
            if(confirm == "yes"){
                vector<string> temp(8);
                for (int i = 0; i < 8; i++) temp[i] = modify_info.substr(i * 5, 4);
                info[info_table[index]] = temp;
                cout << "info modified" << endl;
                break;
            }
            else if(confirm == "no"){
                cout << "modification cancelled" << endl;
                break;
            }
            else cout << "Invalid input, please enter again (yes/no):" << endl;
        }
    }
}

void print(){
    cout << "+--------+------+-------+-------+-------+--------+-------+-------+" << endl;
    cout << "| flight | from |  to   | getoff| arrive|discount| price | empty |" << endl;
    cout << "+--------+------+-------+-------+-------+--------+-------+-------+" << endl;
    for(const auto &row : info){
        cout << "| " << row[0] << "   |";
        cout << " " << row[1] << " |";
        cout << " " << row[2] << "  |";
        cout << " " << row[3] << "  |";
        cout << " " << row[4] << "  |";
        cout << " " << row[5] << "   |";
        cout << " " << row[6] << "  |";
        cout << " " << row[7] << "  |";
        cout << endl;
        cout << "+--------+------+-------+-------+-------+--------+-------+-------+" << endl;
    }
}

void insert(){
    while(1){
        cout << "new info(\"quit\" to break):" << endl;
        string new_info;
        cin >> new_info;
        system("cls");
        print();
        if(new_info == "quit") break;
        if(!format_check_insert(new_info)){
            cout << "format wrong, enter again" << endl;
            continue;
        }
        if(info_table.find(new_info.substr(0, 4)) != info_table.end()){
            cout << "The flight already exists" << endl;
            continue;
        }
        vector<string> temp(8);
        for(int i = 0; i < 8; i++) temp[i] = new_info.substr(i * 5, 4);
        info_table[temp[0]] = info.size();
        info.push_back(temp);
        shell_sort_info();
    }
}