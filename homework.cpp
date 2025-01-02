#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
using namespace std;

enum{from, to, getoff, arrive, discount, price, empty};

unordered_map<string, int> info_table;
vector<vector<string>> info(0, vector<string>(7));

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
            vector<string> temp(7);
            for(int i = 0; i < 7; i++){
                temp[i] = info_string.substr((i + 1) * 5, 4);
            }
            info_table[index] = info.size() - 1;
            info.push_back(temp);
        }
    }
    return true;
}

int main(void){
    if(!init()){
        cout << "Could not open file: information.txt";
        return 1;
    }
    string mode;
    cout << "mode:";
    cin >> mode;
    if(mode == "root"){
        fstream password_file;
        password_file.open("password.txt", ios::in);
        if(!password_file.is_open()){
            cout << "Can not open the file" << endl;
            return 1;
        }
        string password;
        if(!getline(password_file, password)){
            cout << "EOF" << endl;
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
        password_file.open("password.txt", ios::in);
        password_file >> password;
        password_file.close();
        int attempts = 0;
        while(password != user_in && attempts < 4){
            cout << "wrong password, try again:" << endl;
            cin >> user_in;
            attempts++;
        }
        if(attempts == 4){
            cout << "Too many failed attempts" << endl;
            return 2;
        }
        while(1){
            cout << "ROOT MODE, PLEASE ENTER COMMAND" << endl;
            string cmd;
            cin >> cmd;
            if(cmd == "q"){
                fstream information_file;
                information_file.open("information.txt", ios::out | ios::trunc);
                for(const auto& row : info){
                    for(int i = 0; i < 7; i++){
                        information_file << row[i];
                        if(i < 6) information_file << "-";
                    }
                    information_file << endl;
                }
                information_file.close();
                break;
            }
            if(cmd == "save"){
                fstream information_file;
                information_file.open("information.txt", ios::out | ios::trunc);
                for(const auto& row : info){
                    for(int i = 0; i < 7; i++){
                        information_file << row[i];
                        if(i < 6) information_file << "-";
                    }
                    information_file << endl;
                }
                information_file.close();
            }
            else if(cmd == "passwort"){
                cout << "New a password:" << endl;
                cin >> password;
                system("cls");
                password_file.close();
                password_file.open("password.txt", ios::out | ios::trunc);
                password_file << password;
                password_file.close();
            }
            else if(cmd == "insert"){
                while(1)
                {
                    cout << "new info:" << endl;
                    string new_info;
                    cin >> new_info;
                    if(new_info == "q") break;
                    if(!format_check_insert(new_info)){
                        cout << "format wrong" << endl;
                        continue;
                    }
                    vector<string> temp(7);
                    for(int i = 0; i < 7; i++){
                        temp[i] = new_info.substr(i * 5, 4);
                    }
                    info.push_back(temp);
                }
            }
            else if(cmd == "print"){
                cout << "+------+-------+-------+-------+--------+-------+-------+" << endl;
                cout << "| from |  to   | getoff| arrive|discount| price | empty |" << endl;
                cout << "+------+-------+-------+-------+--------+-------+-------+" << endl;
                for(const auto& row : info){
                    cout << "|";
                    cout << " " << row[0] << " |";
                    cout << " " << row[1] << "  |";
                    cout << " " << row[2] << "  |";
                    cout << " " << row[3] << "  |";
                    cout << " " << row[4] << "   |";
                    cout << " " << row[5] << "  |";
                    cout << " " << row[6] << "  |";
                    cout << endl;
                    cout << "+------+-------+-------+-------+--------+-------+-------+" << endl;
                }
            }
            else if(cmd == "modify"){
                cout << "Enter info to modify:" << endl;
                string modify_info;
                cin >> modify_info;
                if(!format_check_insert(modify_info)){
                    cout << "format wrong" << endl;
                    continue;
                }
                string index = modify_info.substr(0, 4);
                if(info_table.find(index) == info_table.end()){
                    vector<string> temp(7);
                    for(int i = 0; i < 7; i++){
                        temp[i] = modify_info.substr(i * 5, 4);
                    }
                    info_table[index] = info.size();
                    info.push_back(temp);
                    continue;
                }
                cout << "Are you sure to modify? (yes/no)" << endl;
                string confirm;
                cin >> confirm;
                if(confirm == "yes"){
                    vector<string> temp(7);
                    for(int i = 0; i < 7; i++){
                        temp[i] = modify_info.substr(i * 5, 4);
                    }
                    info[info_table[index]] = temp;
                    cout << "info modified" << endl;
                } else {
                    cout << "modification cancelled" << endl;
                }
            }
        }
    }
    else{
        cout << "mode wrong" << endl;
    }
}