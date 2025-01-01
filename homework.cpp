#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

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

int main(void){
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
        if(password != user_in){
            cout << "wrog password" << ":" << password << endl;
            return 1;
        }
        while(1){
            cout << "ROOT MODE, PLEASE ENTER COMMAND" << endl;
            string cmd;
            cin >> cmd;
            if(cmd == "q") break;
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
                    fstream information_file;
                    information_file.open("information.txt", ios::out | ios::in);
                    information_file << new_info << endl;
                    information_file.close();
                }
            }
        }
    }
    else{
        cout << "mode wrong" << endl;
    }
}