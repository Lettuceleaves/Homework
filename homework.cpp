#include "homework.h"

enum{flight, from, to, getoff, arrive, discount, price, empty};

vector<vector<string>> info(0, vector<string>(8));
unordered_map<string, int> info_table;
unordered_map<string, int> location;
vector<vector<string>> flight_table;
vector<vector<int>> dist;
vector<string> ticket;
vector<string> booked;
string id_number;
int bill_length;

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
                if (stoi(info[info_table[flight_number]][empty]) > 0) {
                    info[info_table[flight_number]][empty] = to_string(stoi(info[info_table[flight_number]][empty]) - 1);
                } else {
                    cout << "No available seats" << endl;
                    bfs_find_cheapest_route(info[info_table[flight_number]][from], info[info_table[flight_number]][to]);
                    return 0;
                }
                ticket.push_back(flight_number);
                save_deal(flight_number, 1);
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
    return 0;
}

void bfs_find_cheapest_route(string& start, string& end) {
    unordered_map<string, int> min_price;
    unordered_map<string, string> parent;
    queue<string> q;
    q.push(start);
    min_price[start] = 0;
    while (!q.empty()) {
        string current = q.front();
        q.pop();
        for (const auto& row : info) {
            if (row[from] == current && stoi(row[empty]) > 0) {
                string next = row[to];
                int price = stoi(row[price]);
                if (min_price.find(next) == min_price.end() || min_price[next] > min_price[current] + price) {
                    min_price[next] = min_price[current] + price;
                    parent[next] = current;
                    q.push(next);
                }
            }
        }
    }

    if(min_price.find(end) == min_price.end()){
        cout << "No available route from " << start << " to " << end << endl;
    }
    else{
        cout << "Suggest route from " << start << " to " << end << " with price " << min_price[end] << ":" << endl;
        vector<string> route;
        for(string at = end; at != start; at = parent[at]){
            route.push_back(at);
        }
        route.push_back(start);
        reverse(route.begin(), route.end());
        for(const string& location : route){
            cout << location << " ";
        }
        cout << endl;
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

void delete_user(){
    while(1){
        cout << "Enter flight number to delete(\"quit\" to break):" << endl;
        string flight_number;
        cin >> flight_number;
        system("cls");
        print(1);
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
            print(1);
            if(confirm == "yes"){
                auto it = find(booked.begin(), booked.end(), flight_number);
                if (it != booked.end()) {
                    booked.erase(it);
                    save_deal(flight_number, 0);
                    shell_sort_info();
                    system("cls");
                    print(1);
                    cout << "info deleted" << endl;
                    return 0;
                } else {
                    cout << "The ticket does not exist in your bookings" << endl;
                    return 0;
                }
            }
            else if(confirm == "no"){
                cout << "deletion cancelled" << endl;
                break;
            }
            else cout << "Invalid input, please enter again (yes/no):" << endl;
        }
    }
}

void destination(){
    unordered_map<string, set<string>> adj_list;
    for(const auto &row : info){
        adj_list[row[from]].insert(row[to]);
    }
    flight_table.clear();
    for(const auto &entry : adj_list){
        vector<string> destinations(entry.second.begin(), entry.second.end());
        flight_table.push_back({entry.first});
        flight_table.back().insert(flight_table.back().end(), destinations.begin(), destinations.end());
    }
    sort(flight_table.begin(), flight_table.end());
}

void floyd(int v){
    dist = vector<vector<int>>(v, vector<int>(v, INT_MAX));
    for (const auto &row : info) {
        if (location.find(row[from]) == location.end()) {
            location[row[from]] = location.size();
        }
        if (location.find(row[to]) == location.end()) {
            location[row[to]] = location.size();
        }
    }
    for(const auto &row : info){
        int u = location[row[from]];
        int v = location[row[to]];
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
    for(int i = 0; i < v; ++i){
        for(int j = 0; j < v; ++j){
            if(dist[i][j] == INT_MAX){
                dist[i][j] = 0;
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
    cout << "destination: show all destinations" << endl;
    cout << "delete: delete a ticket" << endl;
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

void modify_particular(string &target, string &modify_info, int &ret_flag, int mode){
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

void print_dist(){
    cout << "+--------------+";
    for(size_t i = 0; i < dist.size(); ++i) cout << " " << setw(12) << info[i][from] << " |";
    cout << endl;
    cout << "+--------------+";
    for(size_t i = 0; i < dist.size(); ++i) cout << "--------------+";
    cout << endl;
    for(size_t i = 0; i < dist.size(); ++i){
        cout << "| " << setw(12) << info[i][from] << " |";
        for(size_t j = 0; j < dist.size(); ++j){
            if(dist[i][j] == INT_MAX) cout << "     INF      |";
            else cout << " " << setw(12) << dist[i][j] << " |";
        }
        cout << endl;
        cout << "+--------------+";
        for(size_t i = 0; i < dist.size(); ++i) cout << "--------------+";
        cout << endl;
    }
}

void print_flight(string start){
    bool found = false;
    for(const auto &row : info){
        if(row[from] == start){
            cout << "Flight: " << row[flight] << ", Destination: " << row[to] << endl;
            found = true;
        }
    }
    if(!found) cout << "No flights found from " << start << endl;
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
        user_file.open(id_number + ".txt", ios::out | ios::trunc);
        if(!user_file.is_open()){
            cout << "Could not open file: " + id_number + ".txt";
            return 1;
        }
        fstream deal_file;
        deal_file.open("deal.txt", ios::in | ios::out);
        if (!deal_file.is_open()) {
            cout << "Could not open file: deal.txt";
            return 1;
        }
        vector<string> lines;
        string line;
        while (getline(deal_file, line)) {
            lines.push_back(line);
        }
        deal_file.close();
        deal_file.open("deal.txt", ios::out | ios::trunc);
        if (!deal_file.is_open()) {
            cout << "Could not open file: deal.txt";
            return 1;
        }
        for (int i = 0; i < lines.size(); ++i) {
            if (i >= bill_length && lines[i].find("not completed") != string::npos) {
            lines[i].replace(lines[i].find("not completed"), 13, "completed");
            }
            deal_file << lines[i] << endl;
        }
        deal_file.close();
        for(const auto &row : ticket){
            user_file << row << endl;
        }
        user_file.close();
    }
    system("cls");
    print(mode);
    return 0;
}

int save_deal(string f, int mode){
    fstream deal_file;
    deal_file.open("deal.txt", ios::in);
    if(!deal_file.is_open()){
        cout << "Could not open file: deal.txt";
        return 1;
    }
    string deal_string;
    set<string> deal_set;
    while(deal_file >> deal_string){
        istringstream iss(deal_string);
        string first_word;
        iss >> first_word;
        deal_set.insert(first_word);
    }
    deal_file.close();
    deal_file.open("deal.txt", ios::out | ios::app);
    if(!deal_file.is_open()){
        cout << "Could not open file: deal.txt";
        return 1;
    }
    string new_deal;
    int count = 0;
    do{
        new_deal = "";
        for(int i = 0; i < 10; ++i){
            new_deal += 'A' + rand() % 26;
        }
        if(count > 100000000){
            cout << "System Error, Could not generate a new deal" << endl;
            return 1;
        }
    }
    while(deal_set.find(new_deal) != deal_set.end());
    deal_file << new_deal;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    deal_file << " " << id_number << " " << f << " " << info[info_table[f]][from] << " " << info[info_table[f]][to] << " " << info[info_table[f]][getoff] << " " << info[info_table[f]][arrive] << " " << info[info_table[f]][price] << " " 
              << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday << " ";
    if(mode == 1){
        deal_file << "build" << " ";
    }
    else{
        deal_file << "cancel" << " ";
    }
    deal_file << "not completed" << endl;
    deal_file.close();
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
            else break;
        }
    }
    if(user_file.peek() != ifstream::traits_type::eof()) {
        string line;
        while(getline(user_file, line)){
            if(line.length() != 4 || info_table.find(line) == info_table.end()){
                cout << "Error, please get in touch with assistance" << endl;
                return 1;
            }
            else ticket.push_back(line);
        }
    }
    string line;
    while (getline(user_file, line)) {
        if (info_table.find(line) != info_table.end()) {
            booked.push_back(line);
        } else {
            cout << "System Error: Invalid flight number found in user file." << endl;
            return 1;
        }
    }
    user_file.close();
    floyd(info.size());
    fstream deal_file;
    deal_file.open("deal.txt", ios::in);
    if (!deal_file.is_open()) {
        cout << "Could not open file: deal.txt";
        return 1;
    }
    bill_length = count(istreambuf_iterator<char>(deal_file), istreambuf_iterator<char>(), '\n');
    deal_file.close();
    print(1);
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
        else if(cmd == "help") help_user();
        else if(cmd == "destination"){
            destination();
            cout << "Enter the starting location:" << endl;
            string start;
            cin >> start;
            system("cls");
            print(1);
            print_flight(start);
        }
        else if(cmd == "delete") delete_user();
        else{
            cout << "Invalid command" << endl;
            continue;
        }
    }
    root_ret_flag = false;
    return 0;
}