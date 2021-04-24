#ifndef LIST_H
#define LIST_H

class List {
public:
	List(string* iS, SettingsProfile<int>& intS, SettingsProfile<bool>& bS) : inputString(iS), intSettings(intS), boolSettings(bS) {
		regions.rehash(200);
		createMap();
		createFactbookChecks();
	}

	void createMap();
	void createFactbookChecks();
	void makeList();
	string* getList();
	short int getTime(string, string, string);
	short int str2num(string);
	string getLongTime(short int);
	unordered_map<short int, unordered_set<Region*>>& getMap() { return regions; }
	bool inMap(short int time);
	bool isGoodTrigger(Region*);
	bool isGoodTarget(Region*);
	Region* findTarget(short int&);
	Region* findTrigger(short int);
	Region* findLayeredThorn(short int&);
	void advancedTagChecking();

private:
	unordered_map<short int, unordered_set<Region*>> regions;
	unordered_map<string, Region*> regionsByName;
	string* list;
	string* inputString;
	SettingsProfile<int> intSettings;
	SettingsProfile<bool> boolSettings;
	unordered_set<Region*> seen;
	int lastTargetTime;
	vector<string> factbookChecks;
};

#endif