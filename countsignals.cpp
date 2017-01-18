

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <list>


#define ASSERT(x) if(!(x)){ error << "ASSERT FAILED " << #x "\nat line " << __LINE__ <<endl; exit(__LINE__); }


using namespace std;

bool startsWith(const string it,const string with){
	return it.substr(0, (with.length()) ) == with;
}

int main(int argc, char* argv[]){

	ostream& output = cout;
	ostream& debug  = cerr;
	ostream& error  = cerr;
	
	if(argc < 2) {
		error << "usage: " << argv[0] << " netlist.net <your life story>"<< endl;
		return 1;
	}
	ifstream input("io.net");

	string line; 

	int lineCount(1);

	do{
		input >> ws;
		getline(input, line);
		lineCount++;
	}while(!startsWith(line, "(nets"));

	int code;
	string name;
	string test;

	string currentNet;
	int currentNetCode;

	int pinCount(0);
	int signalCount(0);
	map<string, int> pinCountPerSignal;

	while(input.good()){
		lineCount++;
		debug << '.';
		input.ignore(1000, '(');
		test = "i am finally done";
		input >> test;
		if(test == "net")
		{
			signalCount++;
			input.ignore(1000, '(');
			input >> ws >> test;
			ASSERT(test == "code");
			input >> ws >> currentNetCode;
			input.ignore(1000, '(');
			input >> ws >> test;
			ASSERT(test == "name");
			input >> ws >> currentNet;
			goto next;
		}
		if(test == "node")
		{
			pinCount++;
			pinCountPerSignal[currentNet]++;

			string ref;
			input.ignore(1000, '(');
			input >> ws >> test;
			ASSERT(test == "ref");
			input >> ws >> ref;
			goto next;
		}
		if(test == "i am finally done"){
			break;
		}
		error << "FOUND>" <<test << "<at line " << lineCount << endl;
		return lineCount;
next:
		input.ignore(1000, '\n');
	}


	int singlePinNets(0);
	for (auto pair : pinCountPerSignal){
			cout << "net " << pair.first << "has " << pair.second << " pins " ;
		if(pair.second == 0)
		{
			singlePinNets++;
			cout << "#################3ZERO###############";
		}
		else if(pair.second == 1)
		{
			singlePinNets++;
			cout << "ONE";
		}
		cout << endl;
	}

	cout<< "found \n"
		<< "\t" << singlePinNets << "\tnets with only one pin,\n"
		<< "\t" << (signalCount-singlePinNets) << "\tother nets,\n"
		<< "\t" << signalCount << "\tnets in total\n" << endl;

	cout<< "found \n"
		<< "\t" << singlePinNets << "\tpins on a single-pin net,\n"
		<< "\t" << (pinCount-singlePinNets) << "\tother pins,\n"
		<< "\t" << pinCount << "\tpins in total\n" << endl;
}

