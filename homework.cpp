#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

int main(void){
    cout << "111" << endl;
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
        cout << "111:" << password << ":111" << endl;
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
        cout << "ROOT MODE, PLEASE ENTER COMMAND" << endl;
        while(1){
            string cmd;
            cin >> cmd;
            if(cmd == "abort") break;
        }
    }
    else{
        cout << "mode wrong" << endl;
    }
}