#ifndef REGION_H
#define REGION_H

struct Region {
public:
    string name;
    short int nations;
    short int endos;
    bool founder;
    bool eDel;
    bool tagged;
    bool password;
    string link;
    unordered_set<Region*> embassies;
    string wfe;


    Region() : name(""), nations(0), endos(0), founder(false), eDel(false), tagged(false), link(""), password(false) {}
    Region(string n, short int na, short int e, bool f, bool eD, bool t, string l, bool p) : name(n),
        nations(na), endos(e), founder(f), eDel(eD), tagged(t), link(l), password(p) {}

    string toString() {
        ostringstream oss;

        oss << name << " - " << link << endl; 

        return oss.str();
    }
};

#endif
