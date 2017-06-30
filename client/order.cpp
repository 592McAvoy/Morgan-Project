#include"order.h"

bool get_order(void) {
	order.str("");
	menu();
	char ch;
	string order;
	cin >> ch;
	switch (ch) {
	case'1': {
		new_order();
		return true;
	}
	case'2': {
		check_order();
		return false;
	}
	case'3': {
		cancel_order();
		return true;
	}
	case'4':throw Quit();
	default:
		cout << "Bad input\n";
		return false;//报错
	}

}

void save_order(const string & order,string fn) {
	ofstream ff;
	ff.open(fn, ios::out | ios::app);
	ff << order;
	return;
}

void check_order() {
	string clientID;
	cin.get();
	cout << "Enter your ID:";
	getline(cin, clientID);
	ifstream ff;
	ff.open(clientID+"record.txt");
	if (ff.fail()) {
		cout << "Not exist.\n";
		return;
	}
	while (!ff.eof()) {
		string temp;
		getline(ff, temp);
		set_map();
		for (string key : mykeys) {
			rreplace(temp, key, tochange[key]);
		}
		cout << temp << endl << endl;
	}
	return;
}

void new_order() {
	string status;
	string clientID;
	string goodID;
	string amount;
	string price;
	cin.get();
	cout << "Enter your status(1 for buyer; 2 for seller): ";
	getline(cin, status);
	cout << "Enter your ID: ";
	getline(cin, clientID);
	cout << "Enter good ID: ";
	getline(cin, goodID);
	cout << "Enter the total amount: ";
	getline(cin, amount);
	cout << "Enter your ideal price: ";
	getline(cin, price);

	order.str("");
	order << "35=D;"
		<< "54=" << status
		<< ";1=" << clientID
		<< ";2=" << goodID
		<< ";151=" << amount
		<< ";44=" << price << ";";

	filename = clientID ;
	if_new = true;

	save_order(order.str() + "\n", clientID + "record.txt");
	return;
}

void cancel_order() {
	string clientID;
	cin.get();
	cout << "Enter your ID:";
	getline(cin, clientID);
	filename = clientID;
	ifstream ff;
	ff.open(clientID + ".txt");
	if (ff.fail()) {
		cout << "Not exist.\n";
		return;
	}
	string temp;
	string trans = "";
	//找到最新一条记录
	while (!ff.eof()) {
		temp = trans;
		getline(ff, trans);
	}
	string orderID;
	string goodID;
	int pos1 = temp.find("11=");
	int pos2 = temp.find(";", pos1);
	orderID = temp.substr(pos1 + 3, pos2 - pos1 - 3);
	pos1 = temp.find("2=");
	pos2 = temp.find(";", pos1);
	goodID = temp.substr(pos1 + 2, pos2 - pos1 - 2);

	order.str("");
	order << "35=F;"
		<< "1=" << clientID
		<< ";2=" << goodID
		<< ";41=" << orderID << ";";

	save_order(order.str()+"\n", clientID + "record.txt");
	if_new = false;
	return;
}

void menu() {
	cout << "What do you want to do?\n";
	cout << "1.Send a new order\n"
		<< "2.Check all orders\n"
		<< "3.Cancel last order\n"
		<< "4.Quit\n";

	cout << "Your choice is: ";
}

void set_map()
{
	mykeys.insert("35=");
	mykeys.insert("11=");
	mykeys.insert("150=");
	mykeys.insert("39=");
	mykeys.insert("40=");
	mykeys.insert("44=");
	mykeys.insert("31=");
	mykeys.insert("32=");
	mykeys.insert("35=");
	mykeys.insert("151=");
	mykeys.insert("6=");
	mykeys.insert("14=");
	mykeys.insert("54=");
	mykeys.insert("1=");
	mykeys.insert("2=");

	tochange["35="] = "ORDER: ";
	tochange["11="] = "ORDER ID: ";
	tochange["150="] = "ORDER STATUES: ";
	tochange["39="] = "ORDER STATUES: ";
	tochange["40="] = "ORDER TYPE: ";
	tochange["44="] = "INITIAL PRICE: ";
	tochange["31="] = "PREVIOUS FILLED PRICE: ";
	tochange["32="] = "PREVIOUS FILLED AMOUNT: ";
	tochange["151="] = "REMAINED AMOUNT: ";
	tochange["6="] = "AVERAGE FILLED PRICE: ";
	tochange["14="] = "TOTAL FILLED AMOUNT: ";
	tochange["54="] = "CLIENT STATUES: ";
	tochange["1="] = "CLIENT ID: ";
	tochange["2="] = "GOOD ID: ";
}

void rreplace(string & str, string key,string value)
{
	int pos = str.find(key);
	if (pos < 0) return;
	int size = key.size();
	str.replace(pos,size,value);
	return;
}
