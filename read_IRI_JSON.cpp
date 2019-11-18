#include "read_IRI_JSON.h"
#include <iostream>
using namespace std;

read_IRI_JSON::~read_IRI_JSON()
{
	//clean-up heap
	delete[] pNeighbors;
	delete[] pPrev_tx;
}

bool read_IRI_JSON::get_Neighbors_refresh(std::string json_string)
{
	json j;
	static int count = 0; //number of neighbors
	int i = 0;
	static bool first_time = true; //first time method gets called?

	j = json::parse(json_string);
	auto neigh1 = j["neighbors"];

	if (first_time == true) {
		cout << "\33[2K\r"; //delete first line
		//count neighbors, create array of neighbors
		for (json::iterator it = neigh1.begin(); it != neigh1.end(); ++it) {
			count++;
		}
		pNeighbors = new json[count];
		pPrev_tx = new txs_change[count];

		//write every neighbor as json object, print
		for (json::iterator it = neigh1.begin(); it != neigh1.end(); ++it) {
			pNeighbors[i] = *it;
			i++;
		}
		for (i = 0; i < count; i++) {
			cout << "IP: ";
			if (pNeighbors[i]["connected"] == false) cout << "\033[0;31m";
			cout << pNeighbors[i]["address"] << "\033[0m\n";
			cout << "Domain: " << pNeighbors[i]["domain"] << "\n";
			unsigned long TX = pNeighbors[i]["numberOfAllTransactions"];
			pPrev_tx[i].all_tx = TX;
			cout << "All tx: " << TX << "\n";
			TX = pNeighbors[i]["numberOfNewTransactions"];
			pPrev_tx[i].new_tx = TX;
			cout << "New tx: ";
			if (TX == 0) cout << "\033[0;33m";
			cout << TX << "\033[0m\n";
			TX = pNeighbors[i]["numberOfRandomTransactionRequests"];
			pPrev_tx[i].requ_tx = TX;
			cout << "Requested tx: " << TX << "\n";
			cout << "Invalid tx: ";
			if (pNeighbors[i]["numberOfInvalidTransactions"] > 0) cout << "\033[0;31m";
			cout << pNeighbors[i]["numberOfInvalidTransactions"] << "\033[0m\n";
			TX = pNeighbors[i]["numberOfSentTransactions"];
			pPrev_tx[i].sent_tx = TX;
			cout << "Sent tx: " << TX << "\n";
			cout << "\n";
		}
		first_time = false;
	}
	else {
		int new_count = 0;
		
#ifdef _WIN32
		system("CLS"); //clear console, windows only
#elif true 	
		system("clear"); //UNIX
#endif

		//count again
		for (json::iterator it = neigh1.begin(); it != neigh1.end(); ++it) {
			new_count++;
		}
		if (new_count != count) {
			cout << "\nA neighbor was removed or added!";
			return true;
		}

		//write every neighbor as json object, print
		for (json::iterator it = neigh1.begin(); it != neigh1.end(); ++it) {
			pNeighbors[i] = *it;
			i++;
		}
		for (i = 0; i < new_count; i++) {
			cout << "IP: ";
			if (pNeighbors[i]["connected"] == false) cout << "\033[0;31m";
			cout << pNeighbors[i]["address"] << "\033[0m\n";
			cout << "Domain: " << pNeighbors[i]["domain"] << "\n";
			unsigned long TX = pNeighbors[i]["numberOfAllTransactions"];
			unsigned long cngTX = TX - pPrev_tx[i].all_tx; //calc difference between TXs
			pPrev_tx[i].all_tx = TX;
			cout << "All tx: " << TX;
			if (cngTX == 0) cout << "\033[0;31m";
			cout << " [+" << cngTX << "]\033[0m\n";
			TX = pNeighbors[i]["numberOfNewTransactions"];
			cngTX = TX - pPrev_tx[i].new_tx;
			pPrev_tx[i].new_tx = TX;
			cout << "New tx: ";
			if (TX == 0) cout << "\033[0;33m";
			cout << TX << "\033[0m" << " [+" << cngTX << "]\n";
			TX = pNeighbors[i]["numberOfRandomTransactionRequests"];
			cngTX = TX - pPrev_tx[i].requ_tx;
			pPrev_tx[i].requ_tx = TX;
			cout << "Requested tx: " << TX << " [+" << cngTX << "]\n";
			cout << "Invalid tx: ";
			if (pNeighbors[i]["numberOfInvalidTransactions"] > 0) cout << "\033[0;31m";
			cout << pNeighbors[i]["numberOfInvalidTransactions"] << "\033[0m\n";
			TX = pNeighbors[i]["numberOfSentTransactions"];
			cngTX = TX - pPrev_tx[i].sent_tx;
			cout << "Sent tx: " << TX;
			if (cngTX == 0) cout << "\033[0;31m";
			pPrev_tx[i].sent_tx = TX;
			cout << " [+" << cngTX << "]\033[0m\n";
			cout << "\n";
		}
	}
	return false;
}

void read_IRI_JSON::get_Neighbors_once(std::string json_string, bool complete)
{
	json j;
	static int count = 0;
	int i = 0;
	json* pNeighbors = nullptr;

	j = json::parse(json_string);
	auto neigh1 = j["neighbors"];

	cout << "\33[2K\r"; //delete first line

	//count neighbors, create array of neighbors
	for (json::iterator it = neigh1.begin(); it != neigh1.end(); ++it) {
		count++;
	}
	pNeighbors = new json[count];

	//write every neighbor as json object, print
	for (json::iterator it = neigh1.begin(); it != neigh1.end(); ++it) {
		pNeighbors[i] = *it;
		i++;
	}

	if (complete) {
		for (i = 0; i < count; i++) {
			for (json::iterator it = pNeighbors[i].begin(); it != pNeighbors[i].end(); ++it) {
				std::cout << it.key() << " : " << it.value() << "\n";
			}
			cout << "\n";
		}
	}
	else {
		for (i = 0; i < count; i++) {
			cout << "IP: ";
			if (pNeighbors[i]["connected"] == false) cout << "\033[0;31m";
			cout << pNeighbors[i]["address"] << "\033[0m\n";
			cout << "Domain: " << pNeighbors[i]["domain"] << "\n";
			unsigned long TX = pNeighbors[i]["numberOfAllTransactions"];
			cout << "All tx: " << TX << "\n";
			TX = pNeighbors[i]["numberOfNewTransactions"];
			cout << "New tx: ";
			if (TX == 0) cout << "\033[0;33m";
			cout << TX << "\033[0m\n";
			TX = pNeighbors[i]["numberOfRandomTransactionRequests"];
			cout << "Requested tx: " << TX << "\n";
			cout << "Invalid tx: ";
			if (pNeighbors[i]["numberOfInvalidTransactions"] > 0) cout << "\033[0;31m";
			cout << pNeighbors[i]["numberOfInvalidTransactions"] << "\033[0m\n";
			TX = pNeighbors[i]["numberOfSentTransactions"];
			cout << "Sent tx: " << TX << "\n";
			cout << "\n";
		}
	}
	//clean-up
	delete[] pNeighbors;
}

void read_IRI_JSON::get_Info(std::string json_string, bool complete)
{
	json answer_obj;

	answer_obj = json::parse(json_string);
	cout << "\33[2K\r"; //delete first line

	if (complete) {
		for (json::iterator it = answer_obj.begin(); it != answer_obj.end(); ++it) {
			std::cout << it.key() << " : " << it.value() << "\n";
		}
	}
	else {
		unsigned long milestone, subtangle_milestone, ms_diff;
		unsigned long convert = 1024 * 1024 * 1024; //convert to GByte
		float max_mem, total_mem, free_mem;

		cout << "Name: " << answer_obj["appName"] << "\n";
		cout << "Version: " << answer_obj["appVersion"] << "\n";
		cout << "Neighbors: " << answer_obj["neighbors"] << "\n";
		cout << "Tips: " << answer_obj["tips"] << "\n";
		cout << "Tx to request: " << answer_obj["transactionsToRequest"] << "\n";
		milestone = answer_obj["latestMilestoneIndex"];
		subtangle_milestone = answer_obj["latestSolidSubtangleMilestoneIndex"];
		cout << "Latest milestone: " << milestone << "\n";
		cout << "Latest subtangle milestone: " << subtangle_milestone << "\n";
		//check if sync
		ms_diff = milestone - subtangle_milestone;
		if (ms_diff == 0) cout << "\033[0;32mNode is synched!\033[0m\n";
		else cout << "\033[0;31mNode is " << ms_diff << " milestones behind!\033[0m\n";
		cout << "\nJRE:\n";
		cout << "Version: " << answer_obj["jreVersion"] << "\n";
		max_mem = answer_obj["jreMaxMemory"];
		max_mem = max_mem / convert;
		cout << "Max memory: " << max_mem << " GByte\n";
		total_mem = answer_obj["jreTotalMemory"];
		total_mem = total_mem / convert;
		cout << "Total memory: " << total_mem << " GByte\n";
		free_mem = answer_obj["jreFreeMemory"];
		free_mem = free_mem / convert;
		cout << "Free memory: " << free_mem << " GByte\n";
		cout << "\n";
		cout << "Last snapshotted milestone: " << answer_obj["lastSnapshottedMilestoneIndex"] << "\n";
		cout << "Features: " << answer_obj["features"] << "\n";
	}
}
