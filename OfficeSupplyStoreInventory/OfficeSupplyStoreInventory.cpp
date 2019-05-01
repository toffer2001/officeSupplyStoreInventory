
#include "pch.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

class Inventory {
private:
	string id, name, cost, qty;
public:
	Inventory() {
		this->id = "";
		this->name = "";
		this->cost = "";
		this->qty = "";
	}

	Inventory(string id, string name, string cost, string qty) {
		this->id=id;
		this->name = name;
		this->cost = cost;
		this->qty = qty;
	}
	string getId() {
		return id;
	}

	string getName() {
		return name;
	}

	string getCost() {
		return cost;
	}

	string getQty() {
		return qty;
	}

	void setQty(string qty) {
		this->qty = qty;
	}

};

// function prototypes
void storeData(Inventory items[], int size);
void askSort(Inventory inventoryList[], int size);
void displayData(Inventory items[], int size);
int sortId(Inventory array[], int size);
int sortPrice(Inventory array[], int size);
string dollarFormat(string original);
bool checkQty(Inventory array[], int index, int qty);
int findItem(Inventory array[], int size, int value);
void fulfillPurchase(Inventory array[], int index, int qty, double &runningTotal);
void purchaseRequest(Inventory array[], int size, double &runningTotal);
void updateQty(Inventory array[], int index, int qty);

int main()
{
	const int SIZE = 10;
	Inventory inventoryList[SIZE];
	double runningTotal = 0;
	//string runningTotalString;

    cout << "SDEV 2310 Final Project by Kristoffer Keene!\n\n"; 

	storeData(inventoryList, SIZE);
	askSort(inventoryList, SIZE);

	purchaseRequest(inventoryList, SIZE, runningTotal);

	if (runningTotal > 0) {

		/*
		runningTotalString = to_string(runningTotal);
		runningTotalString = dollarFormat(runningTotalString);
		*/
		cout << fixed << showpoint << setprecision(2);
		cout << "Total cost for purchases: $" << runningTotal << endl;
	}
	cout << "Thank you for your business.\n";
	
	system("pause");
	return 0;

}

//  stores data from file to an array
void storeData(Inventory items[], int size) {
	fstream inputFile;
	string file_id, file_name, file_cost, file_qty;

	inputFile.open("inventory.txt");

	if (!inputFile) {
		cout << "Error opening file.\n";
	}
	else {
		for (int row = 0; row < size; row++) {
				inputFile >> file_id >> file_name >> file_cost >> file_qty;
				items[row] = Inventory(file_id, file_name, file_cost, file_qty);

		}
	}
	inputFile.close();
}

// Prompts user how to sort array
void askSort(Inventory inventoryList[], int size) {
	char choice;
	cout << "Do you want to see the inventory sorted by ID (d) or price (p)? ";
	cin >> choice;
	cout << endl;

	switch (choice) {
	case 'd':
	case 'D':
		sortId(inventoryList, size);
		displayData(inventoryList, size);
		break;
	case 'p':
	case 'P':
		sortPrice(inventoryList, size);
		displayData(inventoryList, size);
		break;
	default:
		displayData(inventoryList, size);
		break;
	}
}

// Prints array
void displayData(Inventory items[], int size) {
	string id, name, cost, qty;

	cout << "Inventory List\n";
	cout << setw(8) << "ID" << setw(12) << "Name" << setw(10) << "Cost" << setw(8) << "Qty\n";
	for (int i = 0; i < size; i++) {
		id = items[i].getId();
		name = items[i].getName();
		cost = items[i].getCost();
		qty = items[i].getQty();

		cost = dollarFormat(cost);

		cout << setw(8) << id << setw(12) << name << setw(10) << cost  << setw(7) << qty << endl;;
	}
	cout << endl;
}

// sorts array by ID
int sortId(Inventory array[], int size) {
	Inventory temp;
	bool madeAswap;
	int counter = 0;

	do {
		madeAswap = false;
		for (int count = 0; count < (size - 1); count++) {
			if (array[count].getId() > array[count + 1].getId()) {
				temp = array[count];
				array[count] = array[count + 1];
				array[count + 1] = temp;
				madeAswap = true;
				counter++;
			}
		}

	} while (madeAswap);
	return counter;
}

// sorts array by Price
int sortPrice(Inventory array[], int size) {
	Inventory temp;
	bool madeAswap;
	int counter = 0;

	do {
		madeAswap = false;
		for (int count = 0; count < (size - 1); count++) {
			if (array[count].getCost() > array[count + 1].getCost()) {
				temp = array[count];
				array[count] = array[count + 1];
				array[count + 1] = temp;
				madeAswap = true;
				counter++;
			}
		}

	} while (madeAswap);
	return counter;
}

// formats string to currency
string dollarFormat(string original) {
	string formatted = original;
	int dp = formatted.find('.');
	int pos = dp;
	while (pos > 3) {
		pos = pos - 3;
		formatted.insert(pos, ",");
	}
	formatted.insert(0, "$");
	return formatted;
}

// check if enough items are in stock
bool checkQty(Inventory array[], int index, int qty) {
	bool result = false;
	result = stod(array[index].getQty()) >= qty ? true : false;

	if (result == false) {
		cout << "We don't have that many in inventory." << endl << endl;
	}

	return result;
}

// this is extra to update the inventory qty after a purchase is made
void updateQty(Inventory array[], int index, int qty) {
	int remainingQty;
	string remainingQtyString;

	remainingQty = stod(array[index].getQty()) - qty;
	remainingQtyString = to_string(remainingQty);
	array[index].setQty(remainingQtyString);
}

// processes purchase of the item
void fulfillPurchase(Inventory array[], int index, int qty, double &runningTotal) {
	double subtotal;

	subtotal = stod(array[index].getCost()) * qty;
	updateQty(array, index, qty);

	cout << fixed << showpoint << setprecision(2);
	cout << "Cost for this item is $" << subtotal << endl << endl;

	// aggregate total purchases
	runningTotal += subtotal;
}

// asks user what item to buy
void purchaseRequest(Inventory array[], int size, double &runningTotal) {
	int id, position, qty;
	bool qtyCheck;

	do {
		cout << "What item do you want to purchase? (-1 to stop) ";
		cin >> id;
		
		if (id != -1) {
			position = findItem(array, size, id);

			if (position != -1) {
				cout << "Item " << array[position].getName() << ": How many do you want? ";
				cin >> qty;
				qtyCheck = checkQty(array, position, qty);

				if (qtyCheck) {
					fulfillPurchase(array, position, qty, runningTotal);
				}
			}
			else {
				cout << "Sorry couldn't find item.\n";
			}
		}
	} while (id != -1);

}

int findItem(Inventory array[], int size, int value) {
	int index = 0;
	int position = -1;
	bool found = false;

	while (index < size && !found) {
		if (stod(array[index].getId()) == value) {
			found = true;
			position = index;
		}
		index++;
	}
	//return position;
	return position;
}

// bubbleSort and selectionSort for reference
/*
int bubbleSort(int array[], int size) {
	int temp;
	bool madeAswap;
	int counter = 0;

	do {
		madeAswap = false;
		for (int count = 0; count < (size - 1); count++) {
			if (array[count] > array[count + 1]) {
				temp = array[count];
				array[count] = array[count + 1];
				array[count + 1] = temp;
				madeAswap = true;
				counter++;
			}
		}

	} while (madeAswap);
	return counter;
}


int selectionSort(int array[], int size) {
	int startScan, minIndex, minValue;
	int counter = 0;

	for (startScan = 0; startScan < (size - 1); startScan++) {
		minIndex = startScan;
		minValue = array[startScan];
		for (int index = startScan + 1; index < size; index++) {
			if (array[index] < minValue) {
				minValue = array[index];
				minIndex = index;
			}
		}
		array[minIndex] = array[startScan];
		array[startScan] = minValue;
		counter++;
	}
	return counter;
}

// formatting for reference

string dollarFormat(string original) {
	string formatted = original;
	int dp = formatted.find('.');
	int pos = dp;
	while (pos > 3) {
		pos = pos - 3;
		formatted.insert(pos, ",");
	}
	formatted.insert(0, "$");
	return formatted;
}

string stringFormat(double original) {
	ostringstream ostr;

	double formatted = original;

	ostr << fixed << showpoint << setprecision(2) << formatted;
	return ostr.str();

	//return formatted;
}

// searches for reference
int linearSearch(const int array[], int size, int value) {
	int index = 0;
	int position = -1;
	bool found = false;

	while (index < size && !found) {
		if(array[index] == value){
			found = true;
			position = index;
		}
		index++;
	}
	//return position;
	return index;
}

int binarySearch(const int array[], int size, int value) {
	int first = 0, last = size - 1,
		middle, position = -1;
	bool found = false;
	int counter = 0;

	while (!found && first <= last) {
		counter++;
		middle = (first + last) / 2;
		if (array[middle] == value)
		{
			found = true;
			position = middle;
		}
		else if (array[middle] > value)
			last = middle - 1;
		else
			first = middle + 1;
	}
	//return position;
	return counter;
}

*/