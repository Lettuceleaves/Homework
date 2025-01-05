#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
using namespace std;

int book();

int check_in(bool &retFlag, fstream &password_file, string &password);

void delete_operation();

void floyd(int v);

bool format_check_insert(string s);

bool init();

void insert();

void help();

void modify(int &retFlag);

void modify_particular(std::__cxx11::string &target, std::__cxx11::string &modify_info, int &ret_flag, int mode);

int password_block(string &password, fstream &password_file, bool &retFlag);

void print(int mode);

void print_dist();

int quit(bool &retFlag, int mode);

int root(bool &retFlag);

int save(int mode);

void shell_sort_info();

int user(bool &root_ret_flag);