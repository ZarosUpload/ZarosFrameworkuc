

#pragma once

#include "Hacks.h"

// All misc visuals (Crosshairs etc) go in this class

class CVisuals : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	void BulletTrace(IClientEntity* pEntity, Color color);
private:
	void NoScopeBorder();
	void DrawCrosshair();
	void Junk();
	void Junk2();
	void DrawRecoilCrosshair();
	void Junk3();
	void DrawCenterRadar();
};