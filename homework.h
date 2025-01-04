#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
using namespace std;

bool format_check_insert(string s);

void save();

int password_block(string &password, fstream &password_file, bool &retFlag);

void help();

void quit(int &retFlag);

int check_in(bool &retFlag, fstream &password_file, string &password);

int root(bool &retFlag);

bool init();

void insert();

void modify(int &retFlag);

void print();

void shell_sort_info();
