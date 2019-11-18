#include <ya_getopt.h>
#include <iostream>
using std::cout;
#include <chrono>
using namespace std::chrono_literals;
#include <thread>
#include <cstring>
#include <IRI_API_get_send.h>
#include <read_IRI_JSON.h>
#include <csignal>

bool abortPgr = false; //flag to end program

IRI_API_get_send oAPI;
read_IRI_JSON parse_answer;
void test();
void get_Neighbors(string& URL, bool complete = false, bool refresh = false);
void getInfo(string& URL, bool complete = false);

void sig_Handler(int signal) { //gets called at CTRL-C

	if (signal != SIGINT) return;
	cout << "\nexiting...!\n";
	abortPgr = true;
	exit(0);
}

int main(int argc, char* argv[]) {
	bool abort = false;

	//flags for params
	bool a = false, b = false, c = false, u = false;
	bool refresh = false, complete = false;
	string URL;
	
	signal(SIGINT, sig_Handler); //catch CTRL-C, run Handler
	do { //read all params, args
		char curArg = 0; //curent read short option
		string long_opt; //curent read long option
		int option_index = 0;
		static struct option long_options[] = { //configure long options
			{"url", required_argument, 0, 'u'},
			{"refresh", no_argument, 0, 'r'},
			{"raw", no_argument, 0, 0 },
			{"complete", no_argument, 0, 0},
			{0, 0, 0, 0 }
		};

		curArg = getopt_long(argc, argv, "abcu:r", long_options, &option_index);

		switch (curArg) {
		case -1: abort = true; break;
		case '?': cout << "\nshow help!"; break; //TODO
		case ':': abort = true; break;
		case 0: {
			long_opt.append(long_options[option_index].name);
			//read long option
			if (long_opt == "complete") complete = true;
		} break;
		case 'a': a = true; break;
		case 'b': b = true; break;
		case 'c': c = true; break;
		case 'u': u = true; URL.append(optarg); break;
		case 'r': refresh = true; break;
		}

	} while (abort != true);

#ifdef _WIN32
	system("CLS"); //clear before fist output, windows only
#endif

	//test();
	if (b) getInfo(URL, complete);
	if (c) get_Neighbors(URL, complete, refresh);

	return 0;
}

/*void test() {
	string Info, Neighbors;
	string URL = ;
	IOTAaddress addressen[2];
	char c1[] = "JKPNBVXIFLISXOXLSGHFCYIY9WJHHMORXAOWUXTLGCCHCCKEBHVBWSEEMBIYXMIEZ9FCRHFOHJRANSGB9";
	char c2[] = "BKDEARVZVOWC9LZKTAB9AUSJSHCGVDQQGJUVNWHV9XNICMDFHEZOVLYRJYMHXKZZXSNRZRPYFSUFAFIP9";
	strcpy(addressen[0], c1);
	strcpy(addressen[1], c2);

	oAPI.setURL(URL);
	oAPI.GetNodeInfo();
	oAPI.run(Info);
	oAPI.reset();
	oAPI.GetNeighbors();
	oAPI.run(Neighbors);
}*/

void get_Neighbors(string& URL, bool complete, bool refresh) {
	string Neighbors;
	const char* pError;
	string error;
	bool abort = false;

	cout << "connecting...\r";
	oAPI.setURL(URL);
	oAPI.GetNeighbors();
	pError = oAPI.run(Neighbors);
	error.assign(pError); //check for error
	if (error.size() != 0) {
		cout << error;
		return;
	}
	try {
		if (refresh) {
			parse_answer.get_Neighbors_refresh(Neighbors);

			//refrehsing
			while (abort == false) {
				std::this_thread::sleep_for(3s); //wait

				//request neighbors again
				cout << "connecting...\r";
				pError = oAPI.run(Neighbors);
				error.assign(pError); //check for error
				if (error.size() != 0) {
					cout << error;
					return;
				}
				abort = parse_answer.get_Neighbors_refresh(Neighbors);
			}
		}
		else {
			parse_answer.get_Neighbors_once(Neighbors, complete);
		}
	}
	catch (nlohmann::detail::exception exp) { //TODO in Klasse abfangen
		cout << exp.what();
		return;
	}

	oAPI.reset();
}

void getInfo(string& URL, bool complete) {
	string Info;

	oAPI.setURL(URL);
	oAPI.GetNodeInfo();
	cout << "connecting...\r";
	oAPI.run(Info);
	parse_answer.get_Info(Info, complete);

	oAPI.reset();
}
