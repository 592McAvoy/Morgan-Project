#include <array>
#include <iostream>
#include <string>
#include<sstream>
#include<fstream>
#include<map>
#include<set>

using namespace std;

bool get_order();
void save_order(const string & order,string fn);
void check_order();
void new_order();
void cancel_order();
void menu();
void set_map();
void rreplace(string& str, string key, string value);

struct Quit{};

extern map<string, string> tochange;
extern set<string> mykeys;

extern stringstream order;
extern string filename;
extern bool if_new;