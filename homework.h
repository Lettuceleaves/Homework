#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

int book();

void bfs_find_cheapest_route(string& start, string& end);

int check_in(bool &ret_flag, fstream &password_file, string &password);

void delete_operation();

void delete_user();

void destination();

void floyd(int v);

bool format_check_insert(string s);

bool init();

void insert();

void help();

void help_user();

void modify(int &ret_flag);

void modify_particular(string &target, string &modify_info, int &ret_flag, int mode);

int password_block(string &password, fstream &password_file, bool &retFlag);

void print(int mode);

void print_dist();

void print_flight(string start);

int quit(bool &ret_flag, int mode);

int root(bool &ret_flag);

int save(int mode);

int save_deal(string f, int mode);

void shell_sort_info();

int user(bool &root_ret_flag);