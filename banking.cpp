#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

class Manager;
class RegularManager;
class VIPManager;

class Customer {
protected:
	int id;
public:
	virtual void accept(RegularManager*)=0;
	virtual void accept(VIPManager*)=0;
	virtual void accept(Manager*)=0;
	Customer() {
		this->id = rand();
	}
	virtual ~Customer() {
	}
};

class Regular: public Customer {
public:
	void accept(RegularManager*);
	void accept(VIPManager*);
	void accept(Manager*);
};

class VIP: public Customer {
public:
	void accept(RegularManager*);
	void accept(VIPManager*);
	void accept(Manager*);
};

class Manager {
protected:
	int id;
public:
	Manager() {
		this->id = rand();
	}
	int getID() {
		return this->id;
	}
	virtual void call(Customer*)=0;
	virtual ~Manager() {
	}
};

class RegularManager: public Manager {
public:
	void call(Customer *customer) {
		customer->accept(this);
	}
};

class VIPManager: public Manager {
public:
	void call(Customer *customer) {
		customer->accept(this);
	}
};

void Regular::accept(RegularManager *manager) {
	cout << "Customer " << this->id << " receives a call from manager "
			<< manager->getID() << endl;
}

void Regular::accept(VIPManager *manager) {
	cout << "Customer " << this->id << " receives a call from VIP manager "
			<< manager->getID() << endl;
}

void Regular::accept(Manager*) {
	cout << "Error! This method should never be called!" << endl;
}

void VIP::accept(RegularManager *manager) {
	throw 1;
}

void VIP::accept(VIPManager *manager) {
	cout << "VIP customer " << this->id << " receives a call from VIP manager "
			<< manager->getID() << endl;
}

void VIP::accept(Manager*) {
	cout << "Error! This method should never be called!" << endl;
}

class Bank {
	RegularManager *regular_manager;
	VIPManager *vip_manager;
	vector<Customer*> customers;
public:
	Bank() {
		this->regular_manager = new RegularManager;
		this->vip_manager = new VIPManager;
		this->customers = vector<Customer*>(100);
		generate(this->customers.begin(), this->customers.end(),
				[]() -> Customer* {
					if (rand() % 2)
						return new Regular;
					else
						return new VIP;
				});
	}
	void callEverybody() {
		for (auto customer : this->customers) {
			try {
				this->regular_manager->call(customer);
			} catch (int e) {
				this->vip_manager->call(customer);
			}
		}
	}
};

int main() {
	srand(time(NULL));
	Bank b;
	b.callEverybody();
	return 0;
}
