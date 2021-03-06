#pragma once
#include <string>
#include <iostream>
using namespace std;

struct Item {
public:
	Item(int id, string name, string description, int price, string dosage, int vendorId, string expectedDeliveryDate, long whRefillLevel, long whRefillQty, long whLevel, bool isActive) {
		this->id = id;
		this->name = name;
		this->description = description;
		this->price = price;
		this->dosage = dosage;
		this->vendorId = vendorId;
		this->expectedDeliveryDate = expectedDeliveryDate;
		this->whRefillLevel = whRefillLevel;
		this->whRefillQty = whRefillQty;
		this->whLevel = whLevel;
		this->isActive = isActive;
	}

	int getId() { return id; }

	string getName() { return name; }

	string getDescription() { return description; }

	int getPrice() { return price; }

	string getDosage() { return dosage; }

	int getVendorId() { return vendorId; }

	string getExpectedDeliveryDate() { return expectedDeliveryDate; }

	long getWhRefillLevel() { return whRefillLevel; }

	long getWhRefillQty() { return whRefillQty; }

	long getWhLevel() { return whLevel; }

	bool operator == (const Item& i) {
		if (id == i.id && name == i.name && description == i.description && price == i.price && dosage == i.dosage && vendorId == i.vendorId &&
			expectedDeliveryDate == i.expectedDeliveryDate && whRefillLevel == i.whRefillLevel && whRefillQty == i.whRefillQty && whLevel == i.whLevel && isActive == i.isActive) {
			return true;
		}
		return false;
	}

	bool operator != (const Item& i) {
		if (*this == i) {
			return false;
		}
		return true;
	}

	void printDescription() {
		cout << "Id: " << id << endl;
		cout << "Name: " << name << endl;
		cout << "Description: " << description << endl;
		cout << "Price: $" << price << endl;
		cout << "Dosage: " << dosage << endl;
		cout << "Vendor: " << vendorId << endl;
		cout << "Expected Delivery Date: " << expectedDeliveryDate << endl;
		cout << "Warehouse Refill Level: " << whRefillLevel << endl;
		cout << "Warehouse Refill Quantity: " << whRefillQty << endl;
		cout << "Warehouse Level: " << whLevel << endl;
		cout << "Is Active: " << isActive << endl;
	}

private:
	int id;
	string name;
	string description;
	int price;
	string dosage;
	int vendorId;
	string expectedDeliveryDate;
	long whRefillLevel;
	long whRefillQty;
	long whLevel;
	bool isActive;
};