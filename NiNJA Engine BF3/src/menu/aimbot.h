#pragma once
#include <math.h>

struct WhiteBlackList {
	char szName[0x50];
	bool bBlacklist;
	bool bWhitelist;

	WhiteBlackList() {}

	WhiteBlackList(char* pName, bool blacklist, bool whitelist) {
		strcpy(szName, pName);
		bBlacklist = blacklist;
		bWhitelist = whitelist;
	}

	static friend bool operator==(const WhiteBlackList& left, const char* right) {
		return !strcmp(left.szName, right);
	}
};

float DegToRad(float d);

class Aimbot {
public:
	bool szWallbangable[24];
	bool szVisible[24];
	vector<WhiteBlackList> WBList;
	Vec2 vSavedAngles;

	ClientPlayer* GetTarget(TargetInformation& target);
	void ProcessAutoWall();
	void ProcessVisibility();

	float GetAngleDelta(float firstAngle, float secondAngle);
	Vec2 SmoothAimbotAngles(Vec2 pClientActiveViewAngles, Vec2 &pAimbotAngles);
	float AngleNormalize(float angle);

	void MovementFix(EntryInputStateNetworkMove* pState) {
		float threesixty = (M_PI * 2); // 360 degrees
		float ninety = (M_PI / 2); // 90 degrees
		float value = 0.f;

		if (vSavedAngles.x < pState->State.AuthoritiveViewAngles.x)
			value = fabs(vSavedAngles.x - pState->State.AuthoritiveViewAngles.x);
		else
			value = threesixty - fabs(pState->State.AuthoritiveViewAngles.x - vSavedAngles.x);

		// similar to what i do in my aimbot
		if (vSavedAngles.x > M_PI)
			value = threesixty - value;

		// -1.f -> 1.f
		pState->State.fAnalogInput[1] = (cos(value + ninety) * pState->State.fAnalogInput[0]) + (cos(value) * pState->State.fAnalogInput[1]);
		pState->State.fAnalogInput[0] = (sin(value + ninety) * pState->State.fAnalogInput[0]) + (sin(value) * pState->State.fAnalogInput[1]);
	}
};

extern Aimbot pAimbot;