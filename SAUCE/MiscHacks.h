

#pragma once

#include "Hacks.h"

Vector GetAutostrafeView();

class CMiscHacks : public CHack
{
public:
	void Junk();
	void Junk2();
	void Junk3();
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:
	void AutoJump(CUserCmd *pCmd);
	void LegitStrafe(CUserCmd *pCmd);
	void RageStrafe(CUserCmd *pCmd);
	void CircleStrafer(CUserCmd* pCmd, Vector& vecOriginalView);
	void PostProcces();
	void ChatSpamDisperseName();
	void AutoPistol(CUserCmd* pCmd);
	void Namespam();
	void FakeWalk(CUserCmd* pCmd, bool & bSendPacket);
	void ChatSpamRegular();
	//void ChatSpamAchievement();
	//void NameDispersion();
	//void ChatSpamAchievement();

	//std::string GetNameChange();

	/*void ChatSpam();
	void Namechanger();

	std::string GetRegularSpam();
	std::string GetCustomSpam();
	std::string GetPlayerSpam();
	std::string GetSinglePlayerSpam();

	std::string GetNameChange();
	std::string GetNameCalloutChange();*/
};
