#ifndef SETTINGS_PROFILE_H
#define SETTINGS_PROFILE_H

enum SettingType {DETAGS, PASSWORDED, ENDOS, EXEC_DEL, FOUNDER, NUM_TEAMS, TRIG_POP, TRIG_PASS, BIRB_FIND_ENABLED, 
	FIND_TRIG, TRIGGER, SEPARATION, NUM_TARGETS, DELAY, CRAZY_TAGGING, MAJOR, MINOR};

template <typename T>
struct SettingsProfile {
public:
	map<SettingType, T*> profile;

	SettingsProfile() {};
	SettingsProfile(map<SettingType, T*> p) : profile(p) {};

	void setSetting(SettingType type, T* value) { profile[type] = value; }

	T* operator[](SettingType type) { return profile[type]; }

};

#endif
