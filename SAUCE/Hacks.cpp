#define _CRT_SECURE_NO_WARNINGS

#include "Hacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include "ESP.h"
#include "Visuals.h"
#include "RageBot.h"
#include "MiscHacks.h"


CEsp Esp;
CVisuals Visuals;
CMiscHacks MiscHacks;
CRageBot RageBot;

void Junk();


// Initialise and register ALL hackmanager hacks in here nigga
void Hacks::SetupHacks()
{
	Esp.Init();
	Visuals.Init();
	MiscHacks.Init();
	RageBot.Init();


	hackManager.RegisterHack(&Esp);
	hackManager.RegisterHack(&Visuals);
	hackManager.RegisterHack(&MiscHacks);
	hackManager.RegisterHack(&RageBot);


	//--------------------------------
	hackManager.Ready();
}


class CCacheAngle {
public:
	//Menu
	bool bCustomSetts = false;
	bool bCorrect = true;
	int iResolverType = 0;
	bool bPrioritize = false;
	int iHitbox = 1;
	bool bAutoBaim = false;
	int iAutoBaimAferShot = 2;
	int iHitboxAutoBaim = 4;
	bool bPrioritizeVis = true;
	float flMinDmg = 5.f;
	bool bInterpLagComp = false;

	//GlobalPlayer
	bool bFlip = false;
};

// Only gets called in game, use a seperate draw UI call for menus in the hook
void Hacks::DrawHacks()
{
	IClientEntity *pLocal = hackManager.pLocal();

	void Hooked__PlaySoundCSGO(const char* fileName);

	// Check the master visuals switch, just to be sure
	if (!Menu::Window.VisualsTab.Active.GetState())
		return;

	if (Menu::Window.VisualsTab.OptionsCompRank.GetState() && GUI.GetKeyState(VK_TAB))
		GameUtils::ServerRankRevealAll();


	hackManager.Draw();
	//--------------------------------
		
}

// Game Cmd Changes
void Hacks::MoveHacks(CUserCmd *pCmd, bool &bSendPacket)
{
	Vector origView = pCmd->viewangles;
	IClientEntity *pLocal = hackManager.pLocal();
	hackManager.Move(pCmd, bSendPacket);
	// ------------------------------

	// Put it in here so it's applied AFTER the aimbot
	int AirStuckKey = Menu::Window.MiscTab.OtherAirStuck.GetKey();
	if (AirStuckKey > 0 && GUI.GetKeyState(AirStuckKey))
	{
		if (!(pCmd->buttons & IN_ATTACK))
		{
			pCmd->tick_count = INT_MAX;//0xFFFFF or 16777216
		}
	}

}

HackManager hackManager;



// Register a new hack
void HackManager::RegisterHack(CHack* hake)
{
	Hacks.push_back(hake);
	hake->Init();
}

// Draw all the hakes
void HackManager::Draw()
{
	if (!IsReady)
		return;

	// Grab the local player for drawing related hacks
	pLocalInstance = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Draw();
	}
}

// Handle all the move hakes
void HackManager::Move(CUserCmd *pCmd,bool &bSendPacket)
{
	if (!IsReady)
		return;

	// Grab the local player for move related hacks
	pLocalInstance = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Move(pCmd,bSendPacket); // 
	}
}

//---------------------------------------------------------------------//
// Other Utils and shit

// Saves hacks needing to call a bunch of virtuals to get the instance
// Saves on cycles and file size. Recycle your plastic kids
IClientEntity* HackManager::pLocal()
{
	return pLocalInstance;
}

// Makes sure none of the hacks are called in their 
// hooks until they are completely ready for use
void HackManager::Ready()
{
	IsReady = true;
}

void Junk()
{
	void Hacks::Junk();
		 float pJunkcode = 532789111453;
		pJunkcode = 134111111189242;
		if (pJunkcode = 171111166632425)
			pJunkcode = 183211166611111425;
		pJunkcode = 17321116661111425;
		pJunkcode = 11117898911;
		if (pJunkcode = 3492511173490534);
		pJunkcode = 2304892193666748943;
		pJunkcode = 4390751390453;
		if (pJunkcode = 171389666425)
			pJunkcode = 1838912425;
		pJunkcode = 1732411896665;
		pJunkcode = 43578195346534;
		if (pJunkcode = 3492123473490534);
		pJunkcode = 230489213748943;
		pJunkcode = 439075139766453;
		if (pJunkcode = 1732189342425)
			pJunkcode = 1834132768543425;
		pJunkcode = 1732476823198465;
		pJunkcode = 435754325432534;
		if (pJunkcode = 3434254349053474);
		pJunkcode = 54325432943346;
		pJunkcode = 7854372543254325;
	}

