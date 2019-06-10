#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <string.h>
#include <math.h>
#include <cstring>
#include <algorithm>
#include <fstream>

using namespace std;

string toBinary(string number) {
	int numberInInt = stoi(number);
	string result = "";
	int x;

	while (numberInInt > 0) {
		result = result + to_string(numberInInt % 2);
		numberInInt = numberInInt / 2;
	}

	x = 8 - result.size();

	if (x > 0) {
		for (int i = 0; i < x; i++) {
			result = result + "0";
		}
	}

	reverse(result.begin(), result.end());

	return result;
}

string toDecimal(string number) {
	int x = 0;
	int p = number.size();

	for (int i = 0; i < p; i++) {
		if (number[i] == '1') {
			x = x + pow(2, p - 1 - i);
		}
	}

	return to_string(x);
}

void IPToDecimal(string* IPLike) {
	for (int i = 0; i < 4; i++) {
		IPLike[i] = toDecimal(IPLike[i]);
	}
}

void IPToBinary(string* IPLike) {
	for (int i = 0; i < 4; i++) {
		IPLike[i] = toBinary(IPLike[i]);
	}
}

string* splitByChar(char* text, char sep) {
	int x = 0;
	int first = 0;
	string* separated;

	for (int i = 0; i < strlen(text); i++) {
		if (text[i] == sep) {
			x++;
		}
	}

	if (sep == '/' && x != 1) {
		separated = new string[1];
		separated[0] = "blad";
	}
	else if (sep == '.' && x != 3) {
		separated = new string[1];
		separated[0] = "blad";
	}
	else {
		separated = new string[x + 1];
		x = 0;

		for (int i = 0; i <= strlen(text); i++) {
			if (text[i] == sep || i == strlen(text)) {
				separated[x] = "";
				for (int j = first; j < i; j++) {
					separated[x] = separated[x] + text[j];
				}
				first = i + 1;
				x++;
			}
		}
	}
	return separated;
}

bool isCorrect(string IP) {
	bool correct = true;
	int temp;
	string* IPArray;
	char* bufor = new char[IP.size() + 1];
	strcpy(bufor, IP.c_str());
	IPArray = splitByChar(bufor, '.');

	if (IPArray[0] != "blad") {
		for (int i = 0; i < 4; i++) {
			temp = stoi(IPArray[i]);
			if (temp < 0 || temp > 255) {
				correct = false;
				break;
			}
		}
	}
	else {
		correct = false;
	}

	return correct;
}

string* getMask(int n) {
	string* result = new string[4];
	int x = 0;

	for (int i = 0; i < 4; i++) {
		result[i] = "00000000";
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			if (x < n) {
				result[i][j] = '1';
				x++;
			}
			else {
				break;
			}
		}
	}

	return result;
}

string* getSubnet(string* IP, string* mask) {
	string* result = new string[4];

	for (int i = 0; i < 4; i++) {
		result[i] = "00000000";
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			if (mask[i][j] == '1') {
				result[i][j] = IP[i][j];
			}
			else if (mask[i][j] == '0') {
				result[i][j] = '0';
			}
		}
	}

	return result;
}

string* getBroadcast(string* IP, string* mask) {
	string* result = new string[4];

	for (int i = 0; i < 4; i++) {
		result[i] = "00000000";
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			if (mask[i][j] == '1') {
				result[i][j] = IP[i][j];
			}
			else if (mask[i][j] == '0') {
				result[i][j] = '1';
			}
		}
	}

	return result;
}

string* getFirstHost(string* subnet) {
	string* result = new string[4];

	for (int i = 0; i < 4; i++) {
		result[i] = subnet[i];
	}

	result[3] = toBinary(to_string(stoi(toDecimal(subnet[3])) + 1));

	return result;
}

string* getLastHost(string* broadcast) {
	string* result = new string[4];

	for (int i = 0; i < 4; i++) {
		result[i] = broadcast[i];
	}

	result[3] = toBinary(to_string(stoi(toDecimal(broadcast[3])) - 1));

	return result;
}

char getSubnetClass(string* IP) {
	if (IP[0][0] == '0') return 'A';
	if (IP[0][0] == '1' && IP[0][1] == '0') return 'B';
	if (IP[0][0] == '1' && IP[0][1] == '1' && IP[0][2] == '0') return 'C';
}

string privOrPubl(char c) {
	if (c == 'A' || c == 'B' || c == 'C') {
		return "prywatny";
	}
	else return "publiczny";
}

int main(int argc, char** argv)
{
	string *input;
	string *IP;
	string *mask;
	string *subnet;
	string *broadcast;
	string *first_host;
	string *last_host;
	string priv_or_publ;
	char subnet_class;
	char* bufor;
	int maskInInt;
	int maxhost;
	ofstream wyjscie("wyjscie.txt");
	
	if (argc == 1) {
		cout << "Podaj IP wraz z maską jako argument." << endl;
	}
	else if(argc > 2) {
		cout << "Podaj tylko jeden argument: IP wraz z maską." << endl;
	}
	else if(argc == 2) {
		input = splitByChar(argv[1],'/');

		if (input[0] != "blad" && isCorrect(input[0])) {
			bufor = new char[input[0].size() + 1];
			strcpy(bufor, input[0].c_str());
			IP = splitByChar(bufor, '.');
			IPToBinary(IP);

			maskInInt = stoi(input[1]);
			maxhost = pow(2,(32 - maskInInt)) - 2;
			mask = getMask(maskInInt);
			subnet = getSubnet(IP, mask);
			broadcast = getBroadcast(IP, mask);
			first_host = getFirstHost(subnet);
			last_host = getLastHost(broadcast);
			subnet_class = getSubnetClass(IP);
			priv_or_publ = privOrPubl(subnet_class);

			cout << "Wyniki analizy: ";
			cout << endl << endl;

			cout << "Adres IP: ";
			cout << IP[0] << "." << IP[1] << "." << IP[2] << "." << IP[3];
			wyjscie << IP[0] << "." << IP[1] << "." << IP[2] << "." << IP[3] << endl;
			IPToDecimal(IP);
			cout << " (";
			cout << IP[0] << "." << IP[1] << "." << IP[2] << "." << IP[3];
			wyjscie << IP[0] << "." << IP[1] << "." << IP[2] << "." << IP[3] << endl << endl;
			cout << ")" << endl;

			cout << "Maska podsieci: ";
			cout << mask[0] << "." << mask[1] << "." << mask[2] << "." << mask[3];
			wyjscie << mask[0] << "." << mask[1] << "." << mask[2] << "." << mask[3] << endl;
			IPToDecimal(mask);
			cout << " (";
			cout << mask[0] << "." << mask[1] << "." << mask[2] << "." << mask[3];
			wyjscie << mask[0] << "." << mask[1] << "." << mask[2] << "." << mask[3] << endl << endl;
			cout << ")" << endl;

			cout << "Adres sieci: ";
			cout << subnet[0] << "." << subnet[1] << "." << subnet[2] << "." << subnet[3];
			wyjscie << subnet[0] << "." << subnet[1] << "." << subnet[2] << "." << subnet[3] << endl;
			IPToDecimal(subnet);
			cout << " (";
			cout << subnet[0] << "." << subnet[1] << "." << subnet[2] << "." << subnet[3];
			wyjscie << subnet[0] << "." << subnet[1] << "." << subnet[2] << "." << subnet[3] << endl << endl;
			cout << ")" << endl;

			cout << "Adres broadcast: ";
			cout << broadcast[0] << "." << broadcast[1] << "." << broadcast[2] << "." << broadcast[3];
			wyjscie << broadcast[0] << "." << broadcast[1] << "." << broadcast[2] << "." << broadcast[3] << endl;
			IPToDecimal(broadcast);
			cout << " (";
			cout << broadcast[0] << "." << broadcast[1] << "." << broadcast[2] << "." << broadcast[3];
			wyjscie << broadcast[0] << "." << broadcast[1] << "." << broadcast[2] << "." << broadcast[3] << endl << endl;
			cout << ")" << endl;

			cout << "Adres pierwszego hosta: ";
			cout << first_host[0] << "." << first_host[1] << "." << first_host[2] << "." << first_host[3];
			wyjscie << first_host[0] << "." << first_host[1] << "." << first_host[2] << "." << first_host[3] << endl;
			IPToDecimal(first_host);
			cout << " (";
			cout << first_host[0] << "." << first_host[1] << "." << first_host[2] << "." << first_host[3];
			wyjscie << first_host[0] << "." << first_host[1] << "." << first_host[2] << "." << first_host[3] << endl << endl;
			cout << ")" << endl;

			cout << "Adres ostatniego hosta: ";
			cout << last_host[0] << "." << last_host[1] << "." << last_host[2] << "." << last_host[3];
			wyjscie << last_host[0] << "." << last_host[1] << "." << last_host[2] << "." << last_host[3] << endl;
			IPToDecimal(last_host);
			cout << " (";
			cout << last_host[0] << "." << last_host[1] << "." << last_host[2] << "." << last_host[3];
			wyjscie << last_host[0] << "." << last_host[1] << "." << last_host[2] << "." << last_host[3] << endl << endl;
			cout << ")" << endl;

			cout << "Hostow do zaadresowania: ";
			cout << maxhost << endl;
			wyjscie << maxhost << endl << endl;

			cout << "Klasa adresu: ";
			cout << subnet_class << endl;
			wyjscie << subnet_class << endl << endl;

			cout << "Typ adresu: ";
			cout << priv_or_publ << endl;
			wyjscie << priv_or_publ << endl;

			wyjscie.close();
		}
		else {
			cout << "Nieprawidłowy format IP." << endl;
		}
	}
	else {
		cout<<"Wystąpił nieznany błąd."<<endl;
	}

	return 0;
}
