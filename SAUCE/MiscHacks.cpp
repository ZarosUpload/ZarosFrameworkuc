#define _CRT_SECURE_NO_WARNINGS

#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "Menu.h"
#include <time.h>

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

Vector AutoStrafeView;

void CMiscHacks::Init()
{
	// Any init
}

void CMiscHacks::Draw()
{
	// Any drawing	
	// Spams
	switch (Menu::Window.MiscTab.OtherChatSpam.GetIndex())
	{
	case 0:
		// No Chat Spam
		break;

	case 1:
		// Regular
		ChatSpamRegular();
		break;

	case 2:
		//DisperseName
		ChatSpamDisperseName();
		break;
		Junk();

	case 3:
		// NameSpam
		Namespam();
		break;
	}
}

void CMiscHacks::Junk() {
	float pJunkcode = 51132711453;
	pJunkcode = 13411136543654369242;
	if (pJunkcode = 171365436542425)
		pJunkcode = 1832654365111425;
	pJunkcode = 173114436543654325;
	pJunkcode = 111654365436543911;
	if (pJunkcode = 349273490534);
	pJunkcode = 23048976654348943;
	pJunkcode = 4399065436543453;
	if (pJunkcode = 178654365439425)
		pJunkcode = 181965654312425;
	pJunkcode = 173654365436543295;
	pJunkcode = 43578195346534;
	float YxkzwDVvzw = 5009581315699; YxkzwDVvzw = 45778724628306; if (YxkzwDVvzw = 93240584898819) YxkzwDVvzw = 12816646086521; YxkzwDVvzw = 9754675611885; YxkzwDVvzw = 6118859754675;
	if (YxkzwDVvzw = 41428341281664)YxkzwDVvzw = 48988199872805; YxkzwDVvzw = 59578021660707;
	if (YxkzwDVvzw = 39654448185877)YxkzwDVvzw = 48988199872805; YxkzwDVvzw = 59578021660707;
	if (YxkzwDVvzw = 39654448185877)YxkzwDVvzw = 48988199872805; YxkzwDVvzw = 59578021660707;
	if (YxkzwDVvzw = 39654448185877)YxkzwDVvzw = 48988199872805; YxkzwDVvzw = 59578021660707;
	if (YxkzwDVvzw = 39654448185877)YxkzwDVvzw = 48988199872805; YxkzwDVvzw = 59578021660707; YxkzwDVvzw = 11315097950501;
	float bWKOaQtpOb = 83691262945092; bWKOaQtpOb = 52043247390025; if (bWKOaQtpOb = 7251831932185) bWKOaQtpOb = 9793895368530; bWKOaQtpOb = 82030025142393; bWKOaQtpOb = 51423938203002;
	if (bWKOaQtpOb = 8802212979389)bWKOaQtpOb = 9321853857841; bWKOaQtpOb = 66584788340416;
	if (bWKOaQtpOb = 811033874547)bWKOaQtpOb = 9321853857841; bWKOaQtpOb = 66584788340416;
	if (bWKOaQtpOb = 811033874547)bWKOaQtpOb = 9321853857841; bWKOaQtpOb = 66584788340416;
	if (bWKOaQtpOb = 811033874547)bWKOaQtpOb = 9321853857841; bWKOaQtpOb = 66584788340416;
	if (bWKOaQtpOb = 811033874547)bWKOaQtpOb = 9321853857841; bWKOaQtpOb = 66584788340416; bWKOaQtpOb = 58384853656232;
	float oNnLzYKJGw = 43545094766289; oNnLzYKJGw = 66512513149740; if (oNnLzYKJGw = 34518198417797) oNnLzYKJGw = 50856575154820; oNnLzYKJGw = 27140334869799; oNnLzYKJGw = 48697992714033;
	if (oNnLzYKJGw = 37112115085657)oNnLzYKJGw = 84177977146881; oNnLzYKJGw = 37942506677975;
	if (oNnLzYKJGw = 45838085570288)oNnLzYKJGw = 84177977146881; oNnLzYKJGw = 37942506677975;
	if (oNnLzYKJGw = 45838085570288)oNnLzYKJGw = 84177977146881; oNnLzYKJGw = 37942506677975;
	if (oNnLzYKJGw = 45838085570288)oNnLzYKJGw = 84177977146881; oNnLzYKJGw = 37942506677975;
	if (oNnLzYKJGw = 45838085570288)oNnLzYKJGw = 84177977146881; oNnLzYKJGw = 37942506677975; oNnLzYKJGw = 8022059566202;
	float ylhmuJVPOA = 1874009253767; ylhmuJVPOA = 9646950600366; if (ylhmuJVPOA = 5302126340994) ylhmuJVPOA = 3660350544842; ylhmuJVPOA = 19679819673873; ylhmuJVPOA = 96738731967981;
	if (ylhmuJVPOA = 99044363660350)ylhmuJVPOA = 3409946791092; ylhmuJVPOA = 72326151272546;
	if (ylhmuJVPOA = 35710897795455)ylhmuJVPOA = 3409946791092; ylhmuJVPOA = 72326151272546;
	if (ylhmuJVPOA = 35710897795455)ylhmuJVPOA = 3409946791092; ylhmuJVPOA = 72326151272546;
	if (ylhmuJVPOA = 35710897795455)ylhmuJVPOA = 3409946791092; ylhmuJVPOA = 72326151272546;
	if (ylhmuJVPOA = 35710897795455)ylhmuJVPOA = 3409946791092; ylhmuJVPOA = 72326151272546; ylhmuJVPOA = 23328946107820;
	float AiReEKzVrE = 48607639269168; AiReEKzVrE = 43378116341570; if (AiReEKzVrE = 90261755928480) AiReEKzVrE = 66614908265273; AiReEKzVrE = 86127536544976; AiReEKzVrE = 65449768612753;
	if (AiReEKzVrE = 51608316661490)AiReEKzVrE = 59284802802024; AiReEKzVrE = 52808159792743;
	if (AiReEKzVrE = 87661143693534)AiReEKzVrE = 59284802802024; AiReEKzVrE = 52808159792743;
	if (AiReEKzVrE = 87661143693534)AiReEKzVrE = 59284802802024; AiReEKzVrE = 52808159792743;
	if (AiReEKzVrE = 87661143693534)AiReEKzVrE = 59284802802024; AiReEKzVrE = 52808159792743;
	if (AiReEKzVrE = 87661143693534)AiReEKzVrE = 59284802802024; AiReEKzVrE = 52808159792743; AiReEKzVrE = 86764629590542;
}

void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	// Any Move Stuff
	Vector vecOriginalView;
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	
	
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (Menu::Window.RageBotTab.WeaponCheck.GetState())
	{
		IClientEntity* pLocal = hackManager.pLocal();
		if (GameUtils::IsScout(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.scoutmindmg.GetValue());
		}
		else if (GameUtils::IsAWP(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.AWPmindmg.GetValue());
		}
		else if (GameUtils::IsAuto(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.Automindmg.GetValue());
		}
		else if (GameUtils::IsR8(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.R8mindmg.GetValue());
		}
		else if (GameUtils::IsPistol(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.Pistolmindmg.GetValue());
		}
		else if (GameUtils::IsRifle(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.Riflemindmg.GetValue());
		}
		else if (GameUtils::IsMP(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyMinimumDamage.SetValue((float)Menu::Window.RageBotTab.MPmindmg.GetValue());
		}
		if (GameUtils::IsScout(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.scoutChance.GetValue());
		}
		else if (GameUtils::IsAWP(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.AWPChance.GetValue());
		}
		else if (GameUtils::IsAuto(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.AutoChance.GetValue());
		}
		else if (GameUtils::IsR8(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.R8Chance.GetValue());
		}
		else if (GameUtils::IsPistol(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.PistolChance.GetValue());
		}
		else if (GameUtils::IsRifle(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.RifleChance.GetValue());
		}
		else if (GameUtils::IsMP(pWeapon))
		{
			Menu::Window.RageBotTab.AccuracyHitchance.SetValue((float)Menu::Window.RageBotTab.MPChance.GetValue());
		}


	}

	// Bhop
	switch (Menu::Window.MiscTab.OtherAutoJump.GetIndex())
	{
	case 0:
		break;
	case 1:
		AutoJump(pCmd);
		break;
		Junk2();
	}

	// AutoStrafe
	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	switch (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex())
	{
	case 0:
		// Off
		break;
	case 1:
		LegitStrafe(pCmd);
		break;
	case 2:
		RageStrafe(pCmd);
		break;
	}

	if (Menu::Window.MiscTab.OtherCircle.GetState())
	{
		CircleStrafer(pCmd, vecOriginalView);
	}
	if (Menu::Window.MiscTab.FakeWalk.GetKey())
		FakeWalk(pCmd, bSendPacket);

	if (Menu::Window.RageBotTab.AutoPistol.GetState())
		AutoPistol(pCmd);
}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm 
	{
		push    ebp
			mov     ebp, esp
			and     esp, 0FFFFFFF8h
			sub     esp, 44h
			push    ebx
			push    esi
			push    edi
			mov     edi, cvar
			mov     esi, value
			jmp     pfn
	}
}

void CMiscHacks::Junk2() {
	float OaieXuGoMG = 1162875421380; OaieXuGoMG = 86847644572816; if (OaieXuGoMG = 18947384340737) OaieXuGoMG = 38903337992518; OaieXuGoMG = 82653354819895; OaieXuGoMG = 48198958265335;
	if (OaieXuGoMG = 46677493890333)OaieXuGoMG = 4340737938064; OaieXuGoMG = 47601347415577;
	if (OaieXuGoMG = 39652385143283)OaieXuGoMG = 4340737938064; OaieXuGoMG = 47601347415577;
	if (OaieXuGoMG = 39652385143283)OaieXuGoMG = 4340737938064; OaieXuGoMG = 47601347415577;
	if (OaieXuGoMG = 39652385143283)OaieXuGoMG = 4340737938064; OaieXuGoMG = 47601347415577;
	if (OaieXuGoMG = 39652385143283)OaieXuGoMG = 4340737938064; OaieXuGoMG = 47601347415577; OaieXuGoMG = 52668492535447;
	float StJpYcsLps = 40865087142813; StJpYcsLps = 31205367289613; if (StJpYcsLps = 82981167199379) StJpYcsLps = 25580165646002; StJpYcsLps = 35745448705502; StJpYcsLps = 87055023574544;
	if (StJpYcsLps = 71060352558016)StJpYcsLps = 71993792208586; StJpYcsLps = 71978678274984;
	if (StJpYcsLps = 56191941234881)StJpYcsLps = 71993792208586; StJpYcsLps = 71978678274984;
	if (StJpYcsLps = 56191941234881)StJpYcsLps = 71993792208586; StJpYcsLps = 71978678274984;
	if (StJpYcsLps = 56191941234881)StJpYcsLps = 71993792208586; StJpYcsLps = 71978678274984;
	if (StJpYcsLps = 56191941234881)StJpYcsLps = 71993792208586; StJpYcsLps = 71978678274984; StJpYcsLps = 62921574199512;
	float pKeNKwxtWn = 52938301832739; pKeNKwxtWn = 74172717223293; if (pKeNKwxtWn = 4018072694712) pKeNKwxtWn = 4784475184260; pKeNKwxtWn = 85962101041626; pKeNKwxtWn = 10416268596210;
	if (pKeNKwxtWn = 3027379478447)pKeNKwxtWn = 26947129564779; pKeNKwxtWn = 41460326315988;
	if (pKeNKwxtWn = 35896884792881)pKeNKwxtWn = 26947129564779; pKeNKwxtWn = 41460326315988;
	if (pKeNKwxtWn = 35896884792881)pKeNKwxtWn = 26947129564779; pKeNKwxtWn = 41460326315988;
	if (pKeNKwxtWn = 35896884792881)pKeNKwxtWn = 26947129564779; pKeNKwxtWn = 41460326315988;
	if (pKeNKwxtWn = 35896884792881)pKeNKwxtWn = 26947129564779; pKeNKwxtWn = 41460326315988; pKeNKwxtWn = 22197059955688;
	float GSKFgaGssx = 18657716912193; GSKFgaGssx = 60205439634553; if (GSKFgaGssx = 83847173161711) GSKFgaGssx = 891116321677; GSKFgaGssx = 5673497070348; GSKFgaGssx = 7070348567349;
	if (GSKFgaGssx = 59859338911163)GSKFgaGssx = 31617114696176; GSKFgaGssx = 75535577644927;
	if (GSKFgaGssx = 36983305930580)GSKFgaGssx = 31617114696176; GSKFgaGssx = 75535577644927;
	if (GSKFgaGssx = 36983305930580)GSKFgaGssx = 31617114696176; GSKFgaGssx = 75535577644927;
	if (GSKFgaGssx = 36983305930580)GSKFgaGssx = 31617114696176; GSKFgaGssx = 75535577644927;
	if (GSKFgaGssx = 36983305930580)GSKFgaGssx = 31617114696176; GSKFgaGssx = 75535577644927; GSKFgaGssx = 6819403134864;
	float xMjCnFfBSc = 95252141697506; xMjCnFfBSc = 7460695454406; if (xMjCnFfBSc = 98465651388996) xMjCnFfBSc = 3481542640170; xMjCnFfBSc = 75588675938322; xMjCnFfBSc = 59383227558867;
	if (xMjCnFfBSc = 8746209348154)xMjCnFfBSc = 13889963933569; xMjCnFfBSc = 35333422282478;
	if (xMjCnFfBSc = 8049675302809)xMjCnFfBSc = 13889963933569; xMjCnFfBSc = 35333422282478;
	if (xMjCnFfBSc = 8049675302809)xMjCnFfBSc = 13889963933569; xMjCnFfBSc = 35333422282478;
	if (xMjCnFfBSc = 8049675302809)xMjCnFfBSc = 13889963933569; xMjCnFfBSc = 35333422282478;
	if (xMjCnFfBSc = 8049675302809)xMjCnFfBSc = 13889963933569; xMjCnFfBSc = 35333422282478; xMjCnFfBSc = 92745274609092;
	float fbhKiqqmnk = 1809607401369; fbhKiqqmnk = 92268314773415; if (fbhKiqqmnk = 18102856070339) fbhKiqqmnk = 87431991678273; fbhKiqqmnk = 1312988698325; fbhKiqqmnk = 8698325131298;
	if (fbhKiqqmnk = 48533578743199)fbhKiqqmnk = 60703394578824; fbhKiqqmnk = 89300977428880;
	if (fbhKiqqmnk = 1057506930486)fbhKiqqmnk = 60703394578824; fbhKiqqmnk = 89300977428880;
	if (fbhKiqqmnk = 1057506930486)fbhKiqqmnk = 60703394578824; fbhKiqqmnk = 89300977428880;
	if (fbhKiqqmnk = 1057506930486)fbhKiqqmnk = 60703394578824; fbhKiqqmnk = 89300977428880;
	if (fbhKiqqmnk = 1057506930486)fbhKiqqmnk = 60703394578824; fbhKiqqmnk = 89300977428880; fbhKiqqmnk = 54205715168764;
}

void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	if (setaddr != 0) 
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}

void change_name(const char* name)
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		NET_SetConVar(name, "name");
}

void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
	{
		int iFlags = hackManager.pLocal()->GetFlags();
		if (!(iFlags & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;

		if (hackManager.pLocal()->GetVelocity().Length() <= 50)
		{
			pCmd->forwardmove = 450.f;
		}
	}
}

void CMiscHacks::LegitStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (!(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = 0.0f;

		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -450.0f;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 450.0f;
		}
	}
}

void CMiscHacks::RageStrafe(CUserCmd *pCmd)
{
	float strafespeed = Menu::Window.MiscTab.OtherStrafeSpeed.GetValue();

	IClientEntity* pLocal = hackManager.pLocal();

	bool bKeysPressed = true;
	if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44)) bKeysPressed = false;

	if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
	{
		pCmd->forwardmove = (1550.f * strafespeed) / pLocal->GetVelocity().Length2D();
		pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
		if (pCmd->forwardmove > 450.f)
			pCmd->forwardmove = 450.f;
	}
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

void CMiscHacks::Junk3() {
	float pJunkcode = 51153278119111453;
	pJunkcode = 13411111111189242;
	if (pJunkcode = 1711115132425)
		pJunkcode = 183211111111425;
	pJunkcode = 17321111111425;
	pJunkcode = 111178955890800011;
	if (pJunkcode = 34925151173490534);
	pJunkcode = 23048925193748943;
	pJunkcode = 43907513087600453;
	if (pJunkcode = 17138559425)
		pJunkcode = 1811138912425;
	pJunkcode = 1732411111895;
	pJunkcode = 4357819534534;
	float OKINWDKMaQ = 69327461415829; OKINWDKMaQ = 88359748152465; if (OKINWDKMaQ = 46393515177143) OKINWDKMaQ = 27251683148287; OKINWDKMaQ = 94903252956449; OKINWDKMaQ = 29564499490325;
	if (OKINWDKMaQ = 32118462725168)OKINWDKMaQ = 51771433245398; OKINWDKMaQ = 14999551195137;
	if (OKINWDKMaQ = 56294906141737)OKINWDKMaQ = 51771433245398; OKINWDKMaQ = 14999551195137;
	if (OKINWDKMaQ = 56294906141737)OKINWDKMaQ = 51771433245398; OKINWDKMaQ = 14999551195137;
	if (OKINWDKMaQ = 56294906141737)OKINWDKMaQ = 51771433245398; OKINWDKMaQ = 14999551195137;
	if (OKINWDKMaQ = 56294906141737)OKINWDKMaQ = 51771433245398; OKINWDKMaQ = 14999551195137; OKINWDKMaQ = 39977332001976;
	float IdlCejNyAo = 4726730707483; IdlCejNyAo = 34471852758698; if (IdlCejNyAo = 18262263851693) IdlCejNyAo = 88920121853450; IdlCejNyAo = 24790504522502; IdlCejNyAo = 45225022479050;
	if (IdlCejNyAo = 59932808892012)IdlCejNyAo = 38516937135420; IdlCejNyAo = 90031457030293;
	if (IdlCejNyAo = 80898413863223)IdlCejNyAo = 38516937135420; IdlCejNyAo = 90031457030293;
	if (IdlCejNyAo = 80898413863223)IdlCejNyAo = 38516937135420; IdlCejNyAo = 90031457030293;
	if (IdlCejNyAo = 80898413863223)IdlCejNyAo = 38516937135420; IdlCejNyAo = 90031457030293;
	if (IdlCejNyAo = 80898413863223)IdlCejNyAo = 38516937135420; IdlCejNyAo = 90031457030293; IdlCejNyAo = 63903547133682;
	float rrRposLOqE = 4179917428153; rrRposLOqE = 2563590840334; if (rrRposLOqE = 46564549244973) rrRposLOqE = 52530239443304; rrRposLOqE = 1770224835568; rrRposLOqE = 4835568177022;
	if (rrRposLOqE = 8813655253023)rrRposLOqE = 92449737456083; rrRposLOqE = 1749564557395;
	if (rrRposLOqE = 9594658651568)rrRposLOqE = 92449737456083; rrRposLOqE = 1749564557395;
	if (rrRposLOqE = 9594658651568)rrRposLOqE = 92449737456083; rrRposLOqE = 1749564557395;
	if (rrRposLOqE = 9594658651568)rrRposLOqE = 92449737456083; rrRposLOqE = 1749564557395;
	if (rrRposLOqE = 9594658651568)rrRposLOqE = 92449737456083; rrRposLOqE = 1749564557395; rrRposLOqE = 29593336385486;
	float mGKQkuapXf = 6271613256812; mGKQkuapXf = 57339224831936; if (mGKQkuapXf = 17018552241831) mGKQkuapXf = 78185345751887; mGKQkuapXf = 28259479602417; mGKQkuapXf = 96024172825947;
	if (mGKQkuapXf = 33313347818534)mGKQkuapXf = 22418317534750; mGKQkuapXf = 37663938704904;
	if (mGKQkuapXf = 26729408793125)mGKQkuapXf = 22418317534750; mGKQkuapXf = 37663938704904;
	if (mGKQkuapXf = 26729408793125)mGKQkuapXf = 22418317534750; mGKQkuapXf = 37663938704904;
	if (mGKQkuapXf = 26729408793125)mGKQkuapXf = 22418317534750; mGKQkuapXf = 37663938704904;
	if (mGKQkuapXf = 26729408793125)mGKQkuapXf = 22418317534750; mGKQkuapXf = 37663938704904; mGKQkuapXf = 72258081187378;
	float rqIvEYnJda = 18831666593895; rqIvEYnJda = 87808579844819; if (rqIvEYnJda = 16662738612455) rqIvEYnJda = 5679627475595; rqIvEYnJda = 71594512606542; rqIvEYnJda = 26065427159451;
	if (rqIvEYnJda = 8041902567962)rqIvEYnJda = 86124558414743; rqIvEYnJda = 3370654606903;
	if (rqIvEYnJda = 68749044213356)rqIvEYnJda = 86124558414743; rqIvEYnJda = 3370654606903;
	if (rqIvEYnJda = 68749044213356)rqIvEYnJda = 86124558414743; rqIvEYnJda = 3370654606903;
	if (rqIvEYnJda = 68749044213356)rqIvEYnJda = 86124558414743; rqIvEYnJda = 3370654606903;
	if (rqIvEYnJda = 68749044213356)rqIvEYnJda = 86124558414743; rqIvEYnJda = 3370654606903; rqIvEYnJda = 76388001739173;
}

void CMiscHacks::Namespam()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name("Zaros Framework");
			useSpace = !useSpace;
		}
		else
		{
			change_name("Zaros Framework");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamDisperseName()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;

	if (wasSpamming)
	{
		change_name("\n");
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamRegular()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		SayInTeamChat("Zaros Framework>all");
	}
	else
	{
		SayInChat("Zaros Framework>all");
	}

	start_t = clock();
}

void CMiscHacks::CircleStrafer(CUserCmd* pCmd, Vector& vecOriginalView) {
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	static int iCircleFact = 0;
	iCircleFact++;
	if (iCircleFact > 361)
		iCircleFact = 0;

	float flRotation = 3.f * iCircleFact - Interfaces::Globals->interval_per_tick;

	Vector StoredViewAngles = pCmd->viewangles;
	int CircleKey = Menu::Window.MiscTab.OtherCircleKey.GetKey();
	if (CircleKey > 0 && GUI.GetKeyState(CircleKey)) {
		pCmd->forwardmove = 450.f;
		pCmd->viewangles = vecOriginalView;
		flRotation = DEG2RAD(flRotation);

		float cos_rot = cos(flRotation);
		float sin_rot = sin(flRotation);

		float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
		float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

		pCmd->forwardmove = new_forwardmove;
		pCmd->sidemove = new_sidemove;
	}

	return;
}

void CMiscHacks::PostProcces()
{
	ConVar* Meme = Interfaces::CVar->FindVar("mat_postprocess_enable");
	SpoofedConvar* meme_spoofed = new SpoofedConvar(Meme);
	meme_spoofed->SetString("mat_postprocess_enable 0");
}

void CMiscHacks::AutoPistol(CUserCmd* pCmd)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (pWeapon)
	{
		if (GameUtils::IsBomb(pWeapon))
		{
			return;
		}

		if (!GameUtils::IsNotPistol(pWeapon))
		{
			return;
		}
	}

	static bool WasFiring = false;

	if (GameUtils::IsPistol)
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
			}
		}
		WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
	}
}

void CMiscHacks::FakeWalk(CUserCmd* pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	int FakeWalkKey = Menu::Window.MiscTab.FakeWalk.GetKey();
	if (FakeWalkKey > 0 && GUI.GetKeyState(FakeWalkKey))
	{
		static int iChoked = -1;
	iChoked++;

	if (iChoked < 3)
	{
		bSendPacket = false;
		pCmd->tick_count += 10;
		pCmd += 7 + pCmd->tick_count % 2 ? 0 : 1;
		pCmd->buttons |= pLocal->GetMoveType() == IN_BACK;
		pCmd->forwardmove = pCmd->sidemove = 0.f;
	}
	else
	{
		bSendPacket = true;
		iChoked = -1;
		Interfaces::Globals->frametime *= (pLocal->GetVelocity().Length2D()) / 1.f;
		pCmd->buttons |= pLocal->GetMoveType() == IN_FORWARD;
	}

	}
}


