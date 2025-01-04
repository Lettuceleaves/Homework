#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
using namespace std;

bool format_check_insert(string s);

int save();

int password_block(string &password, fstream &password_file, bool &retFlag);

void help();

int check_in(bool &retFlag, fstream &password_file, string &password);

int root(bool &retFlag);

int quit(bool &retFlag);

void delete_operation();

bool init();

void insert();

void modify(int &retFlag);

void modify_particular(std::__cxx11::string &target, std::__cxx11::string &modify_info, int &ret_flag, int mode);

void print();

void shell_sort_info();
