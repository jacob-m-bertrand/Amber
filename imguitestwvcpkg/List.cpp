// C++
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <regex>
#include <unordered_set>

// Pugixml
#include <pugixml.hpp>

using namespace std;
using namespace pugi;

// Classes
#include "SettingsProfile.h"
#include "C:\Users\jacob\Desktop\NationStates\TargetFinder\TargetFinder\Region.h"
#include "List.h"

void List::createMap() {
	istringstream in(*inputString);
	string temp;

	while (getline(in, temp)) {
		// Set up the regex to capture all the info we need
		regex toMatch("(.*)(\\d\\d):(\\d\\d):(\\d\\d)(\\d\\d):(\\d\\d):(\\d\\d)(\\d+)(\\d+)"
			"(Founder|No|Password)(Y|N)(Y|N)(.*)");

		// This will contain all the data from the capture groups
		smatch matches;

		// If the regex properly matches
		if (regex_search(temp, matches, toMatch)) {
			// Get major and minor times
			short int major = getTime(matches.str(2), matches.str(3), matches.str(4));
			short int minor = getTime(matches.str(5), matches.str(6), matches.str(7));

			// Initialize a new region with all the appropriate data
			Region* newRegion = new Region(matches.str(1), str2num(matches.str(8)), str2num(matches.str(9)),
				(matches.str(10) == "Founder"), (matches.str(11) == "Y"), (matches.str(12) == "Y"), matches.str(13),
				(matches.str(10) == "Password"));

			regionsByName[newRegion->name] = newRegion;

			// If the index exists (e.g. theres already a nation at that time), push back into the set (otherwise initialize)
			if (regions.find(major) == regions.end()) {
				unordered_set<Region*> toIns;
				toIns.insert(newRegion);

				regions[major] = toIns;
				//if (major > lastTargetTime) lastTargetTime = major;
			}
			else {
				regions[major].insert(newRegion);
			}
		}
		else {
			cout << "match not found on " << temp << endl;
		}
	}
}

void List::advancedTagChecking() {
	xml_document doc;

	xml_parse_result result = doc.load_file("C:\\Users\\jacob\\source\\repos\\imguitestwvcpkg\\imguitestwvcpkg\\regions.xml");
		
	xml_node current = doc.root().first_child();
	string name = doc.child("REGIONS").child("REGION").attribute("NAME").as_string();
	bool first = true;

	do {
		if (!first) current = doc.first_child().next_sibling();
		first = false;

		cout << current.attribute("NAME") << endl;
	} while (doc.first_child().next_sibling());
}

short int List::getTime(string iH, string iM, string iS) {
	return (3600 * str2num(iH)) + (60 * str2num(iM)) + (str2num(iS));
}

string List::getLongTime(short int time) {
	short int hour = floor(time / 3600);
	short int min = floor((time - (hour * 3600)) / 60);
	short int sec = time - (hour * 3600) - (min * 60);

	ostringstream out;

	out << hour << ':' << min << ':' << sec;

	return out.str();
}

short int List::str2num(string s) {
	istringstream stream(s);

	short int x;
	stream >> x;

	return x;
}

Region* List::findTarget(short int& okSecond) {
	while (true) {
		for (Region* r : regions[okSecond]) {
			if (isGoodTarget(r)) {
				if(seen.insert(r).second) return r;
			}
		}
		do {
			okSecond++;
			//if (okSecond >= lastTargetTime) throw "Targets Cannot be Found Past Here!";
		} while (!inMap(okSecond));
	}
}

Region* List::findTrigger(short int okSecond) {
	for (short int time = okSecond - *(intSettings[TRIGGER]); time >*(intSettings[TRIGGER]); --time) {
		if (!inMap(time)) continue;
		for (Region* r : regions[time]) {
			if (isGoodTrigger(r)) return r;
		}
	}
}

Region* List::findLayeredThorn(short int& backtrack) {
	for (short int time = backtrack - *(intSettings[TRIGGER]); time > 0; --time) {
		if (!inMap(time)) continue;
		for (Region* r : regions[time]) {
			if (isGoodTarget(r)) {
				if (seen.insert(r).second) return r;
			}
		}
	}
}

void List::makeList() {
	// Will update this as targets are added, its mainly for outputting
	int targetNum = 1;

	// okSecond tracks what second of update we'll be good to get targets at (e.g. if okSecond is 600, then only targets after 
	// 10 minutes in are considered
	short int okSecond = *intSettings[DELAY];

	// User set values for spacing and number of targets
	int separation = *intSettings[SEPARATION];
	int numTargets = *intSettings[NUM_TARGETS];

	// Get ready to output the list
	ostringstream out;
	
	if (!(*boolSettings[CRAZY_TAGGING])) {
		// We'll keep going until we've gotten as many targets as we need
		while (targetNum <= numTargets) {
			Region* target = findTarget(okSecond);
			Region* thorn = nullptr;
			if(!boolSettings[DETAGS]) thorn = findTarget(okSecond);
			Region* trigger = findTrigger(okSecond);

			out << targetNum << ") " << target->link << endl;
			out << "Trigger: " << trigger->link << endl;
			if (!boolSettings[DETAGS]) out << "Thorn: " << thorn->link << endl << endl;

			okSecond += separation;
			++targetNum;
		}
	}
	else {
		srand(time(NULL));
		vector<int> types;
		//for (unsigned int i = 0; i < numTargets; ++i) types.push_back((rand() % 9) + 1);

		types = { 1,2,3,4,5,6,7,8,9 };

		for (int i : types) {
			switch (i) {
			case 1: {
				Region * target = findTarget(okSecond);
				out << "Target " << targetNum << ": " << target->link << "(" << getLongTime(okSecond) << ")" << endl;

				Region* thorn = findTarget(okSecond);
				Region* trigger = findTrigger(okSecond);

				out << "Thorn (two thorners): " << thorn->link << " (" << getLongTime(okSecond) << ")" << endl;
				out << "Trigger: " << trigger->link << endl;

				break;
			}

			case 2: {
				Region * target = findTarget(okSecond);
				out << "Target " << targetNum << ": " << target->link << "(" << getLongTime(okSecond) << ")" << endl;
				short int backtrack = okSecond;

				Region* thorn = findLayeredThorn(backtrack);
				Region* trigger = findTrigger(backtrack);

				out << "Thorn (layered two thorners): " << thorn->link << " (" << getLongTime(okSecond) << ")" << endl;
				out << "Trigger: " << trigger->link << endl;

				break;
			}

			case 4: {
				Region * targetA = findTarget(okSecond);
				out << "Target " << targetNum << "a: " << targetA->link << "(" << getLongTime(okSecond) << ")" << endl;

				Region* targetB = findTarget(okSecond);
				out << "Target " << targetNum << "b: " << targetB->link << "(" << getLongTime(okSecond) << ")" << endl;

				Region* thorn = findTarget(okSecond);
				Region* trigger = findTrigger(okSecond);

				out << "Thorn: " << thorn->link << " (" << getLongTime(okSecond) << ")" << endl;
				out << "Trigger: " << trigger->link << endl;

				break;
			}

			case 5: {
				Region * targetA = findTarget(okSecond);
				out << "Target " << targetNum << "a: " << targetA->link << "(" << getLongTime(okSecond) << ")" << endl;

				Region* targetB = findTarget(okSecond);
				out << "Target " << targetNum << "b: " << targetB->link << "(" << getLongTime(okSecond) << ")" << endl;

				short int backtrack = okSecond;

				Region* thorn = findLayeredThorn(backtrack);
				Region* trigger = findTrigger(backtrack);

				out << "Thorn (layered): " << thorn->link << " (" << getLongTime(okSecond) << ")" << endl;
				out << "Trigger: " << trigger->link << endl;

				break;
			}

			case 6: {
				Region * targetA = findTarget(okSecond);
				out << "Target " << targetNum << "a: " << targetA->link << "(" << getLongTime(okSecond) << ")" << endl;

				Region* targetB = findTarget(okSecond);
				out << "Target " << targetNum << "b: " << targetB->link << "(" << getLongTime(okSecond) << ")" << endl;

				short int backtrack = okSecond;

				Region* thorn = findLayeredThorn(backtrack);
				Region* trigger = findTrigger(backtrack);

				out << "Thorn (layered two thorners): " << thorn->link << " (" << getLongTime(okSecond) << ")" << endl;
				out << "Trigger: " << trigger->link << endl;

				break;
			}

			case 7: {
				Region * target = findTarget(okSecond);
				out << "Target " << targetNum << ": " << target->link << "(" << getLongTime(okSecond) << ")" << endl;

				short int backtrack = okSecond;

				Region* thornA = findLayeredThorn(backtrack);
				out << "Thorn a (layered): " << thornA->link << " (" << getLongTime(okSecond) << ")" << endl;

				Region* thornB = findLayeredThorn(backtrack);
				out << "Thorn b (layered): " << thornB->link << " (" << getLongTime(okSecond) << ")" << endl;

				Region* trigger = findTrigger(backtrack);
				out << "Trigger: " << trigger->link << endl;

				break;
			}

			case 8: {
				Region * targetA = findTarget(okSecond);
				out << "Target " << targetNum << "a: " << targetA->link << "(" << getLongTime(okSecond) << ")" << endl;

				Region* targetB = findTarget(okSecond);
				out << "Target " << targetNum << "b: " << targetB->link << "(" << getLongTime(okSecond) << ")" << endl;

				short int backtrack = okSecond;

				Region* thornA = findLayeredThorn(backtrack);
				out << "Thorn a (layered): " << thornA->link << " (" << getLongTime(okSecond) << ")" << endl;

				Region* thornB = findLayeredThorn(backtrack);
				out << "Thorn b (layered): " << thornB->link << " (" << getLongTime(okSecond) << ")" << endl;

				Region* trigger = findTrigger(backtrack);
				out << "Trigger: " << trigger->link << endl;

				break;
			}

			case 9: {
				Region* target = findTarget(okSecond);
				out << "Target " << targetNum << ": " << target->link << "(" << getLongTime(okSecond) << ")" << endl;
				short offset = okSecond + (*intSettings[SEPARATION] / 2);
				short backtrack = okSecond;

				Region* thornB = findTarget(offset);
				out << "Thorn B: " << thornB->link << "(" << getLongTime(offset) << endl;

				Region* thornA = findLayeredThorn(backtrack);
				out << "Thorn A: " << thornA->link << getLongTime(backtrack) << endl;

				Region* trigger = findTrigger(backtrack);
				out << "Trigger: " << trigger->link << endl;

				break;
			}
			}
			out << endl << endl;
			targetNum++;
		}
	}

	list = new string(out.str());
}

string* List::getList() { return list; }

bool List::inMap(short int time) {
	return regions.find(time) != regions.end();
}


bool List::isGoodTarget(Region* reg) { return (reg->endos < *intSettings[ENDOS] && reg->founder == *boolSettings[FOUNDER]
	&& reg->eDel == *boolSettings[EXEC_DEL] && reg->tagged == *boolSettings[DETAGS] && reg->password == *boolSettings[PASSWORDED]); }
bool List::isGoodTrigger(Region* reg) { return (reg->nations < *intSettings[TRIG_POP] && !reg->password == *boolSettings[TRIG_PASS]); }