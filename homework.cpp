#include "homework.h"

enum{flight, from, to, getoff, arrive, discount, price, empty};
enum{username};

unordered_map<string, int> info_table;
vector<vector<string>> info(0, vector<string>(8));
vector<string> ticket;
vector<vector<int>> dist;

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
        system("cls");
        if(ret_flag) return ret_val;
    }
    else if(mode == "user"){
        cout << "user mode" << endl;
        bool ret_flag;
        int ret_val = user(ret_flag);
        if(ret_flag) return ret_val;
    }
    else cout << "mode wrong" << endl;
}

int book(){
    while(1){
        cout << "Enter flight number to book(\"quit\" to break):" << endl;
        string flight_number;
        cin >> flight_number;
        system("cls");
        print(1);
        if(flight_number == "quit") break;
        if(info_table.find(flight_number) == info_table.end()){
            cout << "The flight does not exist" << endl;
            continue;
        }
        cout << "Are you sure to book? (yes/no)" << endl;
        string confirm;
        while(1){
            cin >> confirm;
            system("cls");
            print(1);
            if(confirm == "yes"){
                ticket.push_back(flight_number);
                system("cls");
                print(1);
                cout << "booked" << endl;
                return 0;
            }
            else if(confirm == "no"){
                cout << "booking cancelled" << endl;
                return 0;
            }
            else cout << "Invalid input, please enter again (yes/no):" << endl;
        }
    }
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
    print(0);
    ret_flag = false;
    return 0;
}

void delete_operation(){
    while (1){
        cout << "Enter flight number to delete(\"quit\" to break):" << endl;
        string flight_number;
        cin >> flight_number;
        system("cls");
        print(0);
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
            print(0);
            if(confirm == "yes"){
                info.erase(info.begin() + info_table[flight_number]);
                info_table.erase(flight_number);
                shell_sort_info();
                system("cls");
                print(0);
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

void floyd(int v){
    dist = vector<vector<int>>(v, vector<int>(v, INT_MAX));
    for(const auto &row : info){
        int u = info_table[row[from]];
        int v = info_table[row[to]];
        int weight = stoi(row[price]);
        dist[u][v] = weight;
    }
    for(int i = 0; i < v; ++i) dist[i][i] = 0;
    for(int k = 0; k < v; ++k){
        for(int i = 0; i < v; ++i){
            for(int j = 0; j < v; ++j){
                if(dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]){
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
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

void insert(){
    while(1){
        cout << "new info(\"quit\" to break):" << endl;
        string new_info;
        cin >> new_info;
        system("cls");
        print(0);
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
        system("cls");
        print(0);
    }
}

void help(){
    cout << "quit: quit root mode" << endl;
    cout << "save: save the information" << endl;
    cout << "password: change the password" << endl;
    cout << "insert: insert new information" << endl;
    cout << "modify: modify information" << endl;
}

void help_user(){
    cout << "quit: quit user mode" << endl;
    cout << "save: save the information" << endl;
    cout << "book: book a flight" << endl;
}

void modify(int &ret_flag){
    ret_flag = 1;
    cout << "Enter info to modify(\"quit\" to break):" << endl;
    string modify_info;
    cin >> modify_info;
    system("cls");
    print(0);
    if(modify_info == "quit"){
        ret_flag = 1;
        return;
    }
    else if(info_table.find(modify_info) != info_table.end()){
        string target;
        cout << "Which part do you want to modify?" << endl;
        cin >> target;
        system("cls");
        print(0);
        modify_particular(target, modify_info, ret_flag, 0);
        return;
    }
    if(!format_check_insert(modify_info)){
        cout << "format wrong" << endl;
        ret_flag = 3;
        return;
    }
    string index = modify_info.substr(0, 4);
    if(info_table.find(index) == info_table.end()){
        cout << "Are you sure to add: " + modify_info + " ? (yes/no)" << endl;
        string confirm;
        while(1){
            cin >> confirm;
            system("cls");
            print(0);
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
        cout << "Are you sure to modify:" << endl << endl;
        for (int i = 0; i < 8; i++) {
            cout << info[info_table[index]][i];
            if (i < 7) cout << "-";
        }
        cout << endl << endl << " to: " << endl << endl << modify_info << endl << endl << " ? (yes/no)" << endl;
        string confirm;
        while(true){
            cin >> confirm;
            system("cls");
            print(0);
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

void modify_particular(std::__cxx11::string &target, std::__cxx11::string &modify_info, int &ret_flag, int mode){
    if(target == "from" || target == "to" || target == "getoff" || target == "arrive" || target == "discount" || target == "price" || target == "empty"){
        if(mode == 1 && target != "empty"){
            ret_flag = 3;
            return;
        }
        int part_index = info_table[modify_info];
        cout << "Enter new value for " << target << ":" << endl;
        string new_value;
        while(1){
            cin >> new_value;
            if(target == "from" || target == "to" || target == "getoff" || target == "arrive"){
                if(new_value.size() != 4){
                    cout << "Invalid input, please enter again:" << endl;
                    continue;
                }
                bool flag = true;
                for(int i = 0; i < 4; i++){
                    if(!isupper(new_value[i])){
                        flag = false;
                        break;
                    }
                }
                if(!flag){
                    cout << "Invalid input, please enter again:" << endl;
                    continue;
                }
                else break;
            }
            else{
                if(new_value.size() != 4){
                    cout << "Invalid input, please enter again:" << endl;
                    continue;
                }
                bool flag = true;
                for(int i = 0; i < 4; i++){
                    if(!isdigit(new_value[i])){
                        flag = false;
                        break;
                    }
                }
                if(!flag){
                    cout << "Invalid input, please enter again:" << endl;
                    continue;
                }
                else break;
            }
            system("cls");
            print(0);
        }
        while(true){
            cout << "Are you sure to modify: " << endl << endl;
            for(int i = 0; i < 8; i++){
                cout << info[part_index][i];
                if(i < 7) cout << "-";
            }
            cout << endl << endl << "to: " << endl << endl;
            for(int i = 0; i < 8; i++){
                if(i == from && target == "from") cout << new_value;
                else if(i == to + 2 && target == "to") cout << new_value;
                else if(i == getoff + 3 && target == "getoff") cout << new_value;
                else if(i == arrive && target == "arrive") cout << new_value;
                else if(i == discount && target == "discount") cout << new_value;
                else if(i == price && target == "price") cout << new_value;
                else if(i == empty && target == "empty") cout << new_value;
                else cout << info[part_index][i];
                if(i < 7) cout << "-";
            }
            cout << endl << endl << " ? (yes/no)" << endl;
            string confirm;
            cin >> confirm;
            system("cls");
            print(0);
            if(confirm == "yes") break;
            else if(confirm == "no"){
                cout << "Modification cancelled" << endl;
                ret_flag = 3;
                return;
            }
            else cout << "Invalid input, please enter again (yes/no):" << endl;
        }
        if(target == "from") info[part_index][1] = new_value;
        else if(target == "to") info[part_index][2] = new_value;
        else if(target == "getoff") info[part_index][3] = new_value;
        else if(target == "arrive") info[part_index][4] = new_value;
        else if(target == "discount") info[part_index][5] = new_value;
        else if(target == "price") info[part_index][6] = new_value;
        else if(target == "empty") info[part_index][7] = new_value;
        system("cls");
        print(0);
        cout << "Modification successful" << endl;
        ret_flag = 0;
    }
    else{
        cout << "Invalid part specified" << endl;
        ret_flag = 3;
    }
}

int password_block(string &password, fstream &password_file, bool &ret_flag){
    ret_flag = true;
    while(1){
        bool success_set_flag = false;
        cout << "Enter the current password:" << endl;
        string current_password;
        cin >> current_password;
        system("cls");
        if (current_password == "quit") break;
        if (current_password != password){
            cout << "Wrong password, try again or type 'quit' to cancel" << endl;
            continue;
        }
        print(0);
        cout << "New a password:" << endl;
        while(true){
            cout << "Enter new password:" << endl;
            string new_password1, new_password2;
            cin >> new_password1;
            system("cls");
            print(0);
            if(new_password1 == "quit") break;
            cout << "Confirm new password:" << endl;
            cin >> new_password2;
            system("cls");
            print(0);
            if(new_password1 == new_password2){
                password = new_password1;
                password_file.close();
                while(1){
                    password_file.open("password.txt", ios::out | ios::trunc);
                    if(!password_file.is_open()){
                        cout << "Could not open file: password.txt";
                        cout << "Try again?(yes/no)" << endl;
                        string comfirm;
                        cin >> comfirm;
                        if(comfirm == "yes") continue;
                        else return 1;
                    }
                    else break;
                }
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
    return 0;
}

void print(int mode){
    cout << "+--------+------+-------+-------+-------+--------+-------+-------+" << endl;
    cout << "| flight | from |  to   | getoff| arrive|discount| price | empty |" << endl;
    cout << "+--------+------+-------+-------+-------+--------+-------+-------+" << endl;
    for(const auto &row : info){
        cout << "| " << row[0] << "   |";
        cout << " " << row[1] << " |";
        cout << " " << row[2] << "  |";
        cout << " " << row[3] << "  |";
        cout << " " << row[4] << "  |";
        cout << "  " << row[5] << "  |";
        cout << " " << row[6] << "  |";
        cout << " " << row[7] << "  |";
        cout << endl;
        cout << "+--------+------+-------+-------+-------+--------+-------+-------+" << endl;
    }
    if(mode == 1) print_dist();
}

void print_dist() {
    cout << "+--------+";
    for(const auto& entry : info_table) cout << " " << entry.first << " |";
    cout << endl;
    cout << "+--------+";
    for(size_t i = 0; i < info_table.size(); ++i) cout << "-------+";
    cout << endl;
    for(const auto& entry : info_table){
        cout << "| " << entry.first << " |";
        int u = entry.second;
        for(size_t v = 0; v < info_table.size(); ++v){
            if(dist[u][v] == INT_MAX) cout << "  INF  |";
            else cout << setw(5) << dist[u][v] << " |";
        }
        cout << endl;
        cout << "+--------+";
        for(size_t i = 0; i < info_table.size(); ++i) cout << "-------+";
        cout << endl;
    }
}

int quit(bool &ret_flag, int mode){
    ret_flag = true;
    while(1){
        cout << "Are you sure to quit? (yes/no)" << endl;
        string confirm;
        cin >> confirm;
        system("cls");
        print(mode);
        if(confirm == "yes"){
            while(1){
                int ret_val = save(mode);
                if(ret_val){
                    cout << "save failed" << endl;
                    cout << "Try to save again? (yes/no)" << endl;
                    string confirm;
                    cin >> confirm;
                    system("cls");
                    print(mode);
                    if(confirm == "yes") continue;
                    else{              
                        ret_flag = false;
                        return 0;
                    }
                }
                else{
                    ret_flag = false;
                    return 0;
                }
            }
        }
        else if(confirm == "no") break;
        else cout << "Invalid input, please enter again (yes/no):" << endl;
    }
    ret_flag = false;
    return 0;
}

int root(bool &root_ret_flag){
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
        print(0);
        if(cmd == "quit"){
            bool ret_flag;
            int ret_val = quit(ret_flag, 0);
            if(ret_flag) return ret_val;
            else{
                root_ret_flag = false;
                return 0;
            }
        }
        else if(cmd == "help") help();
        else if(cmd == "save") save(0);
        else if(cmd == "password"){
            bool ret_flag;
            int ret_val = password_block(password, password_file, ret_flag);
            if(ret_flag){
                system("cls");
                return ret_val;
            }
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
        else if(cmd == "delete") delete_operation();
        else{
            cout << "Invalid command" << endl;
            continue;
        }
    }
    root_ret_flag = false;
    system("cls");
    return 0;
}

int save(int mode){
    fstream information_file;
    information_file.open("information.txt", ios::out | ios::trunc);
    if(!information_file.is_open()){
        cout << "Could not open file: information.txt";
        return 1;
    }
    shell_sort_info();
    for(const auto &row : info){
        for(int i = 0; i < 8; i++){
            information_file << row[i];
            if (i < 7) information_file << "-";
        }
        information_file << endl;
    }
    information_file.close();
    if(mode == 1){
        fstream user_file;
        user_file.open(ticket[username] + ".txt", ios::out | ios::trunc);
        if(!user_file.is_open()){
            cout << "Could not open file: " + ticket[username] + ".txt";
            return 1;
        }
        for(const auto &row : ticket){
            user_file << row << endl;
        }
        user_file.close();
    }
    system("cls");
    print(mode);
    return 0;
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

int user(bool &root_ret_flag){
    root_ret_flag = true;
    bool ret_flag;
    cout << "Please enter your ID number:" << endl;
    string id_number;
    cin >> id_number;
    system("cls");
    fstream user_file;
    user_file.open(id_number + ".txt", ios::in);
    if(!user_file.is_open()){
        while(1){
            user_file.open(id_number + ".txt", ios::out);
            if(!user_file.is_open()){
                cout << "Could not open or create file: " << id_number << ".txt" << endl;
                cout << "Try again?(yes/no)" << endl;
                string comfirm;
                cin >> comfirm;
                system("cls");
                if(comfirm == "yes") continue;
                else if(comfirm == "no") return 1;
                else{
                    cout << "Invalid input" << endl;
                    continue;
                }
            }
        }
    }
    string line;
    while(getline(user_file, line)){
        if(line.length() != 4 || info_table.find(line) == info_table.end()){
            cout << "error, please get in touch with assistance" << endl;
            return 1;
        }
        else ticket.push_back(line);
    }
    user_file.close();
    floyd(info.size());
    while(1){
        cout << "USER MODE, PLEASE ENTER COMMAND(print \"help\" to check command)" << endl;
        string cmd;
        cin >> cmd;
        system("cls");
        print(1);
        if(cmd == "quit"){
            bool ret_flag;
            int ret_val = quit(ret_flag, 1);
            if(ret_flag) return ret_val;
            else{
                root_ret_flag = false;
                return 0;
            }
        }
        else if(cmd == "save"){
            while(!save(1)){
                cout << "save failed" << endl;
                cout << "Try to save again? (yes/no)" << endl;
                string confirm;
                cin >> confirm;
                system("cls");
                print(1);
                if(confirm == "yes") continue;
                else if(confirm == "no") return 1;
                else{
                    cout << "Invalid input" << endl;
                    continue;
                }
            }
        }
        else if(cmd == "book"){
            if(book()) return 1;
        }
    }
    root_ret_flag = false;
    return 0;
}