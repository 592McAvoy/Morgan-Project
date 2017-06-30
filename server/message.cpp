#include"message.h"



Order::Order(string mes) {
	stringstream ss;
	ss << mes;
	char ch='0';
	while (ss.str()!="") {		
		int tag;
		double temp;
		ss >> ch;
		if (ch == ';') return;

		while (ch != '=') {
			ss.putback(ch);
			ss >> tag;
			ss >> ch;
		}
		ss >> temp;
		if (tag == 35);
		if (tag == 54)
			status = temp;
		if (tag == 1)
			clientID = temp;
		if (tag == 2)
			goodID = temp;
		if (tag == 151) {
			amount = temp;
			ini_amount = amount;
		}			
		if (tag == 44)
			price = temp;
		if (tag == 11)
			orderID = temp;
		if (tag == 99)
			finish = temp;
		ss >> ch;
	}
	
	return;
}

void Order::try_to_fill() {
	for (int i = 0; i < orders.size(); i++) {
		Order* temp = orders[i];
		if (status != temp->status&&
			clientID != temp->clientID&&
			amount!=0&&
			goodID == temp->goodID) {
			if (compare(*temp))
				exchange(*temp);
		}
	}
}

bool Order::cancel() {
	for (int i = 0; i < orders.size(); i++) {
		Order* temp = orders[i];
		if (clientID == temp->clientID) {
			temp->done();
			return true;
		}
	}
	return false;
}

string Order::fullfill(double pp)
{
	int temp = amount;
	amount = 0;
	finish = 1;
	stringstream ss;
	ss << "35=8;150=2;39=2;"
		<< "11=" << orderID
		<< ";54=" << status
		<< ";1=" << clientID
		<< ";2=" << goodID
		<< ";151=" << amount
		<< ";32=" << temp
		<< ";14=" << ini_amount - amount
		<< ";6=" << average()
		<< ";31=" << pp
		<< ";99=" << finish << ";";
	record(ss.str());
	return ss.str();
}

string Order::partialfill(double pp,int n)
{
	amount -= n;
	stringstream ss;
	ss << "35=8;150=1;39=1;"
		<< "11=" << orderID
		<< ";54=" << status
		<< ";1=" << clientID
		<< ";2=" << goodID
		<< ";151=" << amount
		<< ";32=" << n
		<< ";14=" << ini_amount - amount
		<< ";6="<<average()
		<< ";31=" << pp
		<< ";99=" << finish << ";";
	record(ss.str());
	return ss.str();
}

int Order::average()
{
	double sum=0;
	for (int i = 0; i < pri.size(); i++)
		sum += pri[i];
	return sum / pri.size();
}

bool Order::compare(const Order&other) {
	if (status == 1 && price >= other.price)
		return true;
	if (status == 2 && price <= other.price)
		return true;
	return false;
}

void Order::exchange(Order&other) {
	double pp;
	pp = other.price;
	pri.push_back(pp);

	if (amount < other.amount) {
		response.push(fullfill(pp));
		other.partialfill(pp,amount);
		return;
	}
	if (amount > other.amount) {
		other.fullfill(pp);
		response.push(partialfill(pp,other.amount));
		return;
	}
	else {
		response.push(fullfill(pp));
		other.fullfill(pp);
	}
}

void Order::print()
{
	cout << clientID << " " << orderID << endl;
}

string Order::information()
{
	stringstream ss;
	ss << "11=" << orderID
		<< ";54=" << status
		<< ";1=" << clientID
		<< ";2=" << goodID
		<< ";151=" << amount
		<< ";44=" << price
		<< ";99=" << finish << ";\n";
	return ss.str();
}






/******************************************************************************************/

void inicialize(const string ss) {
	orders.clear();
	ifstream ff;
	ff.open(ss);
	if (ff.fail())
		return;
	while (!ff.eof()) {
		string temp;
		getline(ff, temp);
		if (temp != "") {
			Order* pre=new Order(temp);
			orders.push_back(pre);
		}		
	}
}

void check(const string mes) {
	int pos = mes.find("35=");
	char ch = mes[pos + 3];
	string mm = mes.substr(pos + 5);
	stringstream ss;
	switch (ch) {
	case'D': {
		time_t tt = time(NULL);//用当前时间作为order ID
		int id = tt;
		ss << "35=8;150=0;39=0;" << mm << "11=" << id << ";99=0;";
		Order* new_order=new Order(ss.str());
		stringstream ff;
		ff << new_order->get_goodID();
		filename = ff.str()+".txt";
		inicialize(filename);
		record(ss.str());

		orders.push_back(new_order);
		response.push(ss.str());
		new_order->try_to_fill();

		return;
	}

	case'F': {
		int pos = mm.find("41=");
		mm.replace(pos, 3, "11=");
		Order mes(mm);
		stringstream ss;
		ss << mes.get_goodID() << ".txt";
		filename = ss.str();
		inicialize(filename);
		bool result=mes.cancel();
		string temp;
		if (result)
			temp = "35=8;150=4;39=4;" + mm;
		else
			temp = "35=9;39=8;" + mm;
		response.push(temp);
		record(temp);
		return;
	}
	default:return;
	}
}

void save(const string fn) {
	ofstream ff;
	ofstream fini;
	fini.open("Finish.txt", ios::out | ios::app);
	ff.open(fn,ios::out);
	for (int i = 0; i < orders.size(); i++) {		
		if (orders[i]->check_finish()==0)
			ff << orders[i]->information();
		else
			fini<< orders[i]->information();
	}	
	ff.close();
	fini.close();
	return;
	
}

void record(const string ss) {
	ofstream ff;
	ff.open("Record.txt", ios::out | ios::app);
	ff << ss+'\n';
	ff.close();
}