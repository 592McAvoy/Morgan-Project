#include<string>
#include <array>
#include <iostream>
#include<sstream>
#include<fstream>
#include<queue>
#include<vector>
#include<ctime>

using namespace std;

class Order {
public:
	Order(string mes);
	void try_to_fill();
	string fullfill(double pp);
	string partialfill(double pp, int n);
	bool cancel();
	int average();
	bool compare(const Order&other);
	void exchange(Order&other);
	int check_finish() { return finish;}
	void done() { finish = 1; return; }
	void print();
	int get_goodID() { return goodID; }
	string information();
private:
	int status;
	int clientID;
	int orderID;
	int goodID;
	int amount;
	int finish;
	double price;
	int ini_amount;
	vector<double> pri;//记录每次的成交价格
};

void save(const string);
void check(const string);
void inicialize(const string);
void record(const string);

extern vector<Order*> orders;
extern queue<string> response;
extern string filename;



