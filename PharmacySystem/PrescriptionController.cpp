#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm> //for max()
#include "CommandUtils.cpp"
#include "DatabaseUtils.h"
#include "DatabaseManager.h"
#include "ItemTablePrinter.cpp"
#include "CommonUserPrompts.cpp"
using namespace std;


class PrescriptionController {
public:

	/* Starts the create prescription process of user input
		Returns: A prescription object if one was created after the entire process and put into the database
	*/
	Prescription* startCreatePrescription() {
		
		User *customer = CommonUserPrompts::getUserFromInput();
		if (customer == nullptr) {
			// Customer user was not able to be retrived
			cout << endProcessMessage << endl << endl;
			return nullptr;
		}

		Store *store = CommonUserPrompts::getStoreFromInput();
		if (store == nullptr) {
			// Store was not able to be retrived
			cout << endProcessMessage << endl << endl;
			return nullptr;
		}

		//1. Prompt and get Items from user to include in the prescription
		vector<ItemOrder*> ordersToInclude;
		ItemOrder *currentOrder;
		do {
			currentOrder = getItemOrderFromInput(store);
			if (currentOrder != nullptr) {
				ordersToInclude.push_back(currentOrder);
			}
			
		} while (currentOrder != nullptr);

		if (ordersToInclude.size() == 0) { //Guard: No items added for prescription
			cout << "No items added to prescription." << endl;
			cout << endProcessMessage << endl;
			return nullptr;
		}

		//2. Create a new Prescription in the database
		string today = getDate();
		Prescription *prescription = DatabaseManager::shared()->createPrescription(today, customer->getUserID(), store->getId());
		int prescriptionId = prescription->getId();

		//3. Create Purchases for every item specified and tie it to the created prescription
		int prescriptionCost = 0;
		for (ItemOrder* itemOrder : ordersToInclude) {
			Item *item = itemOrder->item;

			int salePrice = item->getPrice();
			
			//4. Apply any associated discount
			Discount *discount = DatabaseManager::shared()->getDiscount(store->getId(), item->getId());
			if (discount != nullptr) {
				bool discountIsValid = DatabaseUtils::dateStringIsBetweenDates(today, discount->getStartDate(), discount->getEndDate());
			
				if (discountIsValid) {
					int amountOff = (discount->getPercentOff() * salePrice) / 100;
					int discountedPrice = salePrice - amountOff;
					salePrice = max(0, discountedPrice);
				}
			}

			prescriptionCost += salePrice;
			DatabaseManager::shared()->createPurchase(prescriptionId, item->getId(), itemOrder->quantity, salePrice);
		}

		cout << endl << "Prescription #" << prescription->getId() << " successfully created." << endl;
		cout << "Total Cost: " << prescriptionCost << endl << endl;

		return prescription;
	}

private:

	struct ItemOrder {
		Item *item;
		int quantity;

		ItemOrder(Item *item, int quantity) {
			this->item = item;
			this->quantity = quantity;
		}
	};

	const char *exitAnytimeMessage = "Enter 'exit' anytime in order to exit the create prescription process.";
	const char *endProcessMessage = "Ended create prescription process.";
	const char *itemDoesNotExistMessage = "That item does not exist for the given store.";

	/* Prompts user for input of an item name and quantity. Will return an ItemOrder if successful, or nullptr if user types 'exit' during method
		Parameter store: A existing store object to be used for printing items from the store
	*/
	ItemOrder* getItemOrderFromInput(Store *store) {
		ItemOrder *itemOrder = nullptr;

		while (itemOrder == nullptr) {

			cout << endl << "Enter an item's id to add to the prescription:" << endl << "(Or type 'list items' to a view a list of all items) ('exit' to cancel)" << endl;
			string userInput = getInput("item id");

			vector<string> input = splitString(userInput, " ");

			try {
				if ("exit" == input.at(0)) {
					return nullptr;
				}
				else if (input.size() == 2 &&  "list items" == input.at(0) + " " + input.at(1)) {
					printItemTable(store);
				}
				else {
					int itemId = stoi(userInput);

					Item *resultForInput = DatabaseManager::shared()->getItem(itemId);
					if (resultForInput == nullptr) {
						throw exception("Result nonexistant.");
					}
					else if (DatabaseManager::shared()->getInventory(store->getId(), resultForInput->getId()) == nullptr) {
						throw exception("Item is not carried by this store.");
					}

					cout << "Enter quantity to order: " << endl;
					string quantityStr = getInput("quantity");
					int quantity = stoi(quantityStr);

					if (quantity > 0) {
						itemOrder = new ItemOrder(resultForInput, quantity);
					}
					else {
						cout << "Invalid quantity entered." << endl;
					}
				}
			}
			catch (const exception &err) {
				cout << itemDoesNotExistMessage << endl;
			}
		}
		
		// Reaches here if itemOrder != nullptr
		return itemOrder;
	}


	/* HELPERS */
	void printItemTable(Store *store) {
		DatabaseManager *dbm = DatabaseManager::shared();
		ItemTablePrinter::printItemTable(dbm, store);
	}

};