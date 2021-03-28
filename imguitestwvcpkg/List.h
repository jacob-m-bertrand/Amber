#ifndef LIST_H
#define LIST_H

class List {
public:
	List(string iS, SettingsProfile<int>& intS, SettingsProfile<bool>& bS) : inputString(iS), intSettings(intS), boolSettings(bS) {
		toMatch = regex("(.*)\\t(\\d\\d):(\\d\\d):(\\d\\d)\\t(\\d\\d):(\\d\\d):(\\d\\d)\\t(\\d+)\\t(\\d+)\\t(Founder|No|Password)\\t(Y|N)\\t(Y|N)\\t(.*)");
		regions.rehash(200);
	}

	void createMap(ifstream);
	void makeList();
	string getList();
	short int getTime(string, string, string);
	short int str2num(string);
	string getLongTime(short int);
	unordered_map<short int, unordered_set<Region*>>& getMap() { return regions; }

private:
	unordered_map<short int, unordered_set<Region*>> regions;
	regex toMatch;
	string list;
	string inputString;
	SettingsProfile<int> intSettings;
	SettingsProfile<bool> boolSettings;

};

#endif