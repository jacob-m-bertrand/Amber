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

#include "List.h"
#include "C:\Users\jacob\Desktop\NationStates\TargetFinder\TargetFinder\Region.h"
#include "SettingsProfile.h"

using namespace std;

void List::createMap(ifstream fin) {
	string temp;

	while (getline(fin, temp)) {
		// Set up the regex to capture all the info we need
		regex toMatch("(.*)\\t(\\d\\d):(\\d\\d):(\\d\\d)\\t(\\d\\d):(\\d\\d):(\\d\\d)\\t(\\d+)\\t(\\d+)"
			"\\t(Founder|No|Password)\\t(Y|N)\\t(Y|N)\\t(.*)");

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

			// If the index exists (e.g. theres already a nation at that time), push back into the set (otherwise initialize)
			if (regions.find(major) == regions.end()) {
				unordered_set<Region*> toIns;
				toIns.insert(newRegion);

				regions[major] = toIns;
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

void List::makeList() {

}
string List::getList() {
	int numTargets = 0;
	int targetNum = 1;

	int okSecond = 0;

	int trigger = *intSettings[TRIGGER];
	int separation = *intSettings[SEPARATION];

	ostringstream out;

	while (targetNum <= numTargets) {
		if (regions.find(okSecond) != regions.end()) {

			auto sIter = regions[okSecond].begin();

			while (sIter != regions[okSecond].end()) {
				if ((*sIter)->isGoodTarget()) {
					out << getLongTime(okSecond) << " - " << (*sIter)->toString();


					for (int i = okSecond - trigger; i > 0; --i) {
						if (regions.find(i) != regions.end()) {
							auto tIter = regions[i].begin();

							while (tIter != regions[okSecond].end()) {
								if ((*tIter)->isGoodTrigger() && (*tIter)->toString() != (*sIter)->toString()) {
									out << "Trigger: " << (*tIter)->toString() << endl;
									i = 0;
									break;
								}
								++tIter;
							}
						}

					}



					okSecond += separation;
					++targetNum;
					break;
				}

				++sIter;
			}

			++okSecond;
		}
		else {
			++okSecond;
		}
	}


	for (auto& iter : regions) {
		for (auto& sIter : iter.second) {
			// cout << sIter->toString() << endl;
			delete sIter;
		}

		iter.second.clear();
	}
}