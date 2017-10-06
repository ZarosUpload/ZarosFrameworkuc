#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"

#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "Resolver.h"
#include "hitmarker.h"
#include "chams.cpp"
#include <intrin.h>

#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))

#ifdef NDEBUG
#define strenc( s ) std::string( cx_make_encrypted_string( s ) )
#define charenc( s ) strenc( s ).c_str()
#define wstrenc( s ) std::wstring( strenc( s ).begin(), strenc( s ).end() )
#define wcharenc( s ) wstrenc( s ).c_str()
#else
#define strenc( s ) ( s )
#define charenc( s ) ( s )
#define wstrenc( s ) ( s )
#define wcharenc( s ) ( s )
#endif

bool flipAA;
Vector LastAngleAA;
bool Resolver::didhitHS;
CUserCmd* Globals::UserCmd;
IClientEntity* Globals::Target;
int Globals::Shots;
bool Globals::change;
int Globals::TargetID;
std::map<int, QAngle>Globals::storedshit;
int Globals::missedshots;
BYTE bMoveData[0x200];

typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef bool(__thiscall *FireEventClientSideFn)(PVOID, IGameEvent*);
typedef void(__thiscall* RenderViewFn)(void*, CViewSetup&, CViewSetup&, int, int);
typedef void(__thiscall* PlaySoundFn)(const char*);

using OverrideViewFn = void(__fastcall*)(void*, void*, CViewSetup*);
typedef float(__stdcall *oGetViewModelFOV)();

PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
FrameStageNotifyFn oFrameStageNotify;
OverrideViewFn oOverrideView;
FireEventClientSideFn oFireEventClientSide;
RenderViewFn oRenderView;
PlaySoundFn oPlaySound;


void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event);
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __stdcall CreateMoveClient_Hooked(float frametime, CUserCmd* pCmd);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup);
float __stdcall GGetViewModelFOV();
void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw);
void __stdcall hookedPlaySound(const char* name);

int kek = 0;
int autism = 0;

namespace Hooks
{
	Utilities::Memory::VMTManager VMTPanel;
	Utilities::Memory::VMTManager VMTClient;
	Utilities::Memory::VMTManager VMTClientMode;
	Utilities::Memory::VMTManager VMTModelRender;
	Utilities::Memory::VMTManager VMTPrediction;
	Utilities::Memory::VMTManager VMTPlaySound;
	Utilities::Memory::VMTManager VMTRenderView;
	Utilities::Memory::VMTManager VMTEventManager;
};

// Undo our hooks
void Hooks::UndoHooks()
{
	VMTPanel.RestoreOriginal();
	VMTPrediction.RestoreOriginal();
	VMTModelRender.RestoreOriginal();
	VMTClientMode.RestoreOriginal();
}

void Hooks::Initialise()
{
	Interfaces::CVar->ConsoleColorPrintf(Color(0, 255, 0, 255), ("\n Successfully loaded Zaros Framework! \n"));
	Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 0, 255), ("\n Build Date: %s \n"), __DATE__);
	Interfaces::CVar->ConsoleColorPrintf(Color(0, 0, 255, 255), ("\n Build Time: %s \n"), __TIME__);

	VMTPanel.Initialise((DWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)&PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);

	VMTPrediction.Initialise((DWORD*)Interfaces::Prediction);
	VMTPrediction.HookMethod((DWORD)&Hooked_InPrediction, 14);

	VMTModelRender.Initialise((DWORD*)Interfaces::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.HookMethod((DWORD)&Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);

	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);

	oOverrideView = (OverrideViewFn)VMTClientMode.HookMethod((DWORD)&Hooked_OverrideView, 18);
	VMTClientMode.HookMethod((DWORD)&GGetViewModelFOV, 35);

	VMTClient.Initialise((DWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 36);

	VMTEventManager.Initialise((DWORD*)Interfaces::EventManager);
	oFireEventClientSide = (FireEventClientSideFn)VMTEventManager.HookMethod((DWORD)&Hooked_FireEventClientSide, 9);

	VMTPlaySound.Initialise((DWORD*)Interfaces::Surface);
	oPlaySound = (PlaySoundFn)VMTPlaySound.HookMethod((DWORD)&hookedPlaySound, 82);
}

void MovementCorrection(CUserCmd* pCmd)
{
}

float clip(float n, float lower, float upper)
{
	return (std::max)(lower, (std::min)(n, upper));
}

//---------------------------------------------------------------------------------------------------------
//                                         Hooked Functions
//---------------------------------------------------------------------------------------------------------

VOID WINAPI_INLINE hookedPlaySound(const char* name)
{
	__asm
	{

		PUSH name
		CALL oPlaySound
	}

	if (Interfaces::Engine->IsConnected() || !(Menu::Window.MiscTab.AutoAccept.GetState()))
		return;

	if (!strcmp(name, "\x55\x49\x2F\x63\x6F\x6D\x70\x65\x74\x69\x74\x69\x76\x65\x5F\x61\x63\x63\x65\x70\x74\x5f\x62\x65\x65\x70\x2E\x77\x61\x76"))
	{
		Offsets::IsReady = (IsReadyFn)(Offsets::Functions::IsReadySig);
		Offsets::IsReady;
	}
}

bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event)
{
	if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 3)
	{
		if (!strcmp(Event->GetName(), "player_hurt"))
		{
			int deadfag = Event->GetInt("userid");
			int attackingfag = Event->GetInt("attacker");
			IClientEntity* pLocal = hackManager.pLocal();
			if (Interfaces::Engine->GetPlayerForUserID(deadfag) != Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerForUserID(attackingfag) == Interfaces::Engine->GetLocalPlayer())
			{
				IClientEntity* hittedplayer = (IClientEntity*)(Interfaces::Engine->GetPlayerForUserID(deadfag));
				int hit = Event->GetInt("hitgroup");
				if (hit == 1 && hittedplayer && deadfag && attackingfag)
				{
					Resolver::didhitHS = true;
					Globals::missedshots = 0;
				}
				else
				{
					Resolver::didhitHS = false;
					Globals::missedshots++;
				}
			}
		}
	}

	if (Menu::Window.RageBotTab.FlipAA.GetState())
	{
		if (!strcmp(Event->GetName(), "player_hurt"))
		{
			int deadfag = Event->GetInt("userid");
			int attackingfag = Event->GetInt("attacker");
			IClientEntity* pLocal = hackManager.pLocal();
			if (Interfaces::Engine->GetPlayerForUserID(deadfag) == Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerForUserID(attackingfag) != Interfaces::Engine->GetLocalPlayer())
			{
				flipAA = true;

			}
			else
			{
				flipAA = false;
			}
		}
	}
	return oFireEventClientSide(ECX, Event);
}

// Animated ClanTag Function
void SetClanTag(const char* tag, const char* name)
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
	pSetClanTag(tag, name);
}

// Blank Clantag
void NoClantag()
{
	SetClanTag("", "");
}

// Clantag Functions
void ClanTag()
{
	int speed = Menu::Window.MiscTab.OtherClantagspeed.GetValue();
	static int counter = 0;
	switch (Menu::Window.MiscTab.OtherClantag.GetIndex())
	{
	case 0:
		break;
	case 1:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * speed;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 9;
		switch (value) {
		case 0:SetClanTag("Z", "Zaros"); break;
		case 1:SetClanTag("Za", "Zaros"); break;
		case 2:SetClanTag("Zar", "Zaros"); break;
		case 3:SetClanTag("Zaro", "Zaros"); break;
		case 4:SetClanTag("Zaros", "Zaros"); break;
		case 5:SetClanTag("Zaro", "Zaros"); break;
		case 6:SetClanTag("Zar", "Zaros"); break;
		case 7:SetClanTag("Za", "Zaros"); break;
		case 8:SetClanTag("Z", "Zaros"); break;
		}
		counter++; //test xd
	}
	break;
	case 2:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * speed;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 6;
		switch (value) {
		case 0:SetClanTag("Zaros", "Zaros"); break;
		case 1:SetClanTag("Framework", "Zaros"); break;
		case 2:SetClanTag("by", "Zaros"); break;
		case 3:SetClanTag("NewbOwnerton", "Zaros"); break;
		case 4:SetClanTag("and", "Zaros"); break;
		case 5:SetClanTag("LaMe1337", "Zaros"); break;
		}
		counter++;
	}
	break;
	case 3:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * speed;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 31;
		switch (value) {
		case 0:SetClanTag("/", "Zaros"); break;
		case 1:SetClanTag("\\", "Zaros"); break;
		case 2:SetClanTag("Z", "Zaros"); break;
		case 3:SetClanTag("Z/", "Zaros"); break;
		case 4:SetClanTag("Z\\", "Zaros"); break;
		case 5:SetClanTag("Za/", "Zaros"); break;
		case 6:SetClanTag("Za\\", "Zaros"); break;
		case 7:SetClanTag("Zar", "Zaros"); break;
		case 8:SetClanTag("Zar/", "Zaros"); break;
		case 9:SetClanTag("Zar\\", "Zaros"); break;
		case 10:SetClanTag("Zaro", "Zaros"); break;
		case 11:SetClanTag("Zaro/", "Zaros"); break;
		case 12:SetClanTag("Zaro\\", "Zaros"); break;
		case 13:SetClanTag("Zaros", "Zaros"); break;
		case 14:SetClanTag("Zaros", "Zaros"); break;
		case 15:SetClanTag("Zaros", "Zaros"); break;
		case 16:SetClanTag("Zaros", "Zaros"); break;
		case 17:SetClanTag("Zaros", "Zaros"); break;
		case 18:SetClanTag("Zaros", "Zaros"); break;
		case 19:SetClanTag("Zaro\\", "Zaros"); break;
		case 20:SetClanTag("Zaro/", "Zaros"); break;
		case 21:SetClanTag("Zaro", "Zaros"); break;
		case 22:SetClanTag("Zar\\", "Zaros"); break;
		case 23:SetClanTag("Zar/", "Zaros"); break;
		case 24:SetClanTag("Zar", "Zaros"); break;
		case 25:SetClanTag("Za\\", "Zaros"); break;
		case 26:SetClanTag("Za/", "Zaros"); break;
		case 27:SetClanTag("Za", "Zaros"); break;
		case 28:SetClanTag("Z\\", "Zaros"); break;
		case 29:SetClanTag("Z/", "Zaros"); break;
		case 30:SetClanTag("Z", "Zaros"); break;
			
		}
		counter++;
	}
	break;
	case 4:
		time_t now = time(0);
		char timestamp[10] = "";
		strftime(timestamp, 10, "%H:%M:%S", localtime(&now));
		SetClanTag(timestamp, "Time");
		break;
	}
}


void Prediction(CUserCmd* pCmd, IClientEntity* LocalPlayer)
{
	if (Interfaces::MoveHelper && Menu::Window.RageBotTab.EnginePrediction.GetState() && LocalPlayer->IsAlive())
	{
		float curtime = Interfaces::Globals->curtime;
		float frametime = Interfaces::Globals->frametime;
		int iFlags = LocalPlayer->GetFlags();

		Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick;
		Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;

		Interfaces::MoveHelper->SetHost(LocalPlayer);

		Interfaces::Prediction1->SetupMove(LocalPlayer, pCmd, nullptr, bMoveData);
		Interfaces::GameMovement->ProcessMovement(LocalPlayer, bMoveData);
		Interfaces::Prediction1->FinishMove(LocalPlayer, pCmd, bMoveData);

		Interfaces::MoveHelper->SetHost(0);

		Interfaces::Globals->curtime = curtime;
		Interfaces::Globals->frametime = frametime;
		*LocalPlayer->GetPointerFlags() = iFlags;
	}
}

bool __stdcall CreateMoveClient_Hooked(float frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number)
		return true;


	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;

		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		IClientEntity* pEntity;
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());


		if (Menu::Window.MiscTab.OtherClantag.GetIndex() > 0)
			ClanTag();

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive())
		{
			Hacks::MoveHacks(pCmd, bSendPacket);
			ResolverSetup::GetInst().CM(pEntity);
		}


		static bool nosmoke;
		if (Menu::Window.VisualsTab.OtherNoSmoke.GetState())
		{

			if (!nosmoke) {
				ConVar* particles = Interfaces::CVar->FindVar("r_drawparticles");
				SpoofedConvar* particles_spoofed = new SpoofedConvar(particles);
				particles_spoofed->SetInt(0);
				nosmoke = true;
			}

		}
		else if (!Menu::Window.VisualsTab.OtherNoSmoke.GetState())
		{
			if (nosmoke)
			{
				ConVar* particles = Interfaces::CVar->FindVar("r_drawparticles");
				SpoofedConvar* particles_spoofed = new SpoofedConvar(particles);
				particles_spoofed->SetInt(1);
				nosmoke = false;
			}

		}



		qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
		Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
		Vector vRightNorm;			Normalize(viewright, vRightNorm);
		Vector vUpNorm;				Normalize(viewup, vUpNorm);

		float forward = pCmd->forwardmove;
		float right = pCmd->sidemove;
		float up = pCmd->upmove;
		if (forward > 450) forward = 450;
		if (right > 450) right = 450;
		if (up > 450) up = 450;
		if (forward < -450) forward = -450;
		if (right < -450) right = -450;
		if (up < -450) up = -450;
		pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
		pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
		pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);

		if (Menu::Window.MiscTab.OtherSafeMode.GetState())
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);

			if (pCmd->viewangles.z != 0.0f)
			{
				pCmd->viewangles.z = 0.00;
			}

			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
			{
				Utilities::Log("Having to re-normalise!");
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
				Beep(750, 800);
				if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				{
					pCmd->viewangles = origView;
					pCmd->sidemove = right;
					pCmd->forwardmove = forward;
				}
			}
		}

		if (pCmd->viewangles.x > 90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		if (pCmd->viewangles.x < -90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		if (bSendPacket)
			LastAngleAA = pCmd->viewangles;
	}

	return false;
}



int RandomInt(int min, int max)
{
	return rand() % max + min;
}


std::string GetTimeString()
{
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;
}

void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	if (Menu::Window.VisualsTab.OtherNoScope.GetState() && !strcmp("HudZoom", Interfaces::Panels->GetName(vguiPanel)))
		return;

	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, "MatSystemTopPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		if (Menu::Window.MiscTab.WaterMark.GetState() == true)
		{
			static float rainbow;
			rainbow += 0.005f;
			if (rainbow > 1.f) rainbow = 0.f;
			Render::Text(10, 10, Color::FromHSB(rainbow, 1.f, 1.f), Render::Fonts::Menu, "Zaros Memework v4.2.0");

		}
		if (!FoundPanel)
		{
			PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
			if (strstr(szPanelName, "MatSystemTopPanel"))
			{
				FocusOverlayPanel = vguiPanel;
				FoundPanel = true;
			}
		}
		else if (FocusOverlayPanel == vguiPanel)
		{
			if (Menu::Window.MiscTab.Info.GetState() == true)
			{
				if (Menu::Window.MiscTab.OtherSafeMode.GetState() == true)
				{
					Render::Text(10, 30, Color(33, 255, 0, 255), Render::Fonts::Info, "Anti-Untrusted: ON");
				}
				else
				{
					Render::Text(10, 30, Color(255, 4, 0, 255), Render::Fonts::Info, "Anti-Untrusted: OFF");
				}
				if (Menu::Window.VisualsTab.Clock.GetState())
				{
					Render::Textf(225, 10, Color(255, 255, 255, 255), Render::Fonts::Info, ("%s"), GetTimeString().c_str());
				}

				if (Menu::Window.RageBotTab.Active.GetState() == true) {

					if (Menu::Window.RageBotTab.AntiAimEnable.GetState() == true)
					{
						Render::Text(10, 40, Color(242, 255, 0, 255), Render::Fonts::Info, "Anti Aim: ON");
					}
					else
					{
						Render::Text(10, 40, Color(255, 255, 255, 255), Render::Fonts::Info, "Anti Aim: OFF");
					}
					if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 0)
					{
						Render::Text(10, 50, Color(255, 255, 255, 255), Render::Fonts::Info, "Pitch: None");
					}

					if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 1)
					{
						Render::Text(10, 50, Color(255, 4, 0, 255), Render::Fonts::Info, "Pitch: Half Down");
					}
					if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 2)
					{
						Render::Text(10, 50, Color(255, 4, 0, 255), Render::Fonts::Info, "Pitch: Jitter");
					}
					if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 3)
					{
						Render::Text(10, 50, Color(255, 4, 0, 255), Render::Fonts::Info, "Pitch: Emotion");
					}
					if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 4)
					{
						Render::Text(10, 50, Color(255, 4, 0, 255), Render::Fonts::Info, "Pitch: Up");
					}
					if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 5)
					{
						Render::Text(10, 50, Color(255, 4, 0, 255), Render::Fonts::Info, "Pitch: Zero");
					}
					if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 6)
					{
						Render::Text(10, 50, Color(255, 4, 0, 255), Render::Fonts::Info, "Pitch: Mixed");
					}
					if (Menu::Window.RageBotTab.AntiAimPitch.GetIndex() == 7)
					{
						Render::Text(10, 50, Color(255, 4, 0, 255), Render::Fonts::Info, "Pitch: Custom");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 0)
					{
						Render::Text(10, 60, Color(255, 255, 255, 255), Render::Fonts::Info, "Real Yaw: None");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 1)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: Fast Spin");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 2)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: Slow Spin");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 3)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: Jitter");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 4)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: 180 Jitter");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 5)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: Backwards");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 6)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: Backwards Jitter");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 7)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: Sideways Right");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 8)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: Sideways Left");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 9)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: LBY");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 10)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: LBY Jitter");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 11)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: LBY Sideways");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 12)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: LBY Spin");
					}
					if (Menu::Window.RageBotTab.AntiAimYaw.GetIndex() == 13)
					{
						Render::Text(10, 60, Color(255, 4, 0, 255), Render::Fonts::Info, "Real Yaw: LBY Break");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 0)
					{
						Render::Text(10, 70, Color(255, 255, 255, 255), Render::Fonts::Info, "Fake Yaw: None");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 1)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: Fast Spin");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 2)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: Slow Spin");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 3)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: Jitter");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 4)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: 180 Jitter");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 5)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: Backwards");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 6)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: Backwards Jitter");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 7)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: Sideways Right");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 8)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: Sideways Left");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 9)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: LBY");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 10)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: LBY Jitter");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 11)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: LBY Sideways");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 12)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: LBY Spin");
					}
					if (Menu::Window.RageBotTab.FakeYaw.GetIndex() == 13)
					{
						Render::Text(10, 70, Color(255, 4, 0, 255), Render::Fonts::Info, "Fake Yaw: LBY Break");
					}
				}

				if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && Menu::Window.MiscTab.OtherHitmarker.GetState())
					hitmarker::singleton()->on_paint();

			}

		}

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
			Hacks::DrawHacks();

		// Update and draw the menu
		Menu::DoUIFrame();
	}
}

bool __stdcall Hooked_InPrediction()
{
	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::VMTPrediction.GetOriginalFunction(14);
	static DWORD *ecxVal = Interfaces::Prediction;
	result = origFunc(ecxVal);

	if (Menu::Window.VisualsTab.OtherNoVisualRecoil.GetState() && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
	{
		IClientEntity* pLocalEntity = NULL;

		float* m_LocalViewAngles = NULL;

		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}

		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();

		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}

	return result;
}

// DrawModelExec for chams and shit
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	Color color;
	float flColor[3] = { 0.f };
	static IMaterial* CoveredLit = CreateMaterial(true);
	static IMaterial* OpenLit = CreateMaterial(false);
	static IMaterial* CoveredFlat = CreateMaterial(true, false);
	static IMaterial* OpenWireframe = CreateMaterial(true, false);
	static IMaterial* CoveredWireframe = CreateMaterial(true, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false);
	bool DontDraw = false;

	const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);
	IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Menu::Window.VisualsTab.Active.GetState())
	{
		// Player Chams
		int ChamsStyle = Menu::Window.VisualsTab.OptionsChams.GetIndex();
		if (ChamsStyle != 0 && Menu::Window.VisualsTab.FiltersPlayers.GetState() && strstr(ModelName, "models/player"))
		{
			if (pLocal /* && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() || pModelEntity->GetTeamNum() != pLocal->GetTeamNum())*/)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				

				IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
				if (pModelEntity)
				{
					IClientEntity *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
					if (local)
					{
						if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 /*&& pModelEntity->GetTeamNum() != local->GetTeamNum()*/)
						{
							float alpha = 1.f;

							if (pModelEntity->HasGunGameImmunity())
								alpha = 0.5f;

							if (pModelEntity->GetTeamNum() == 2)
							{
								flColor[0] = Menu::Window.GUITab.TChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.GUITab.TChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.GUITab.TChamsB.GetValue() / 255.f;
							}
							else
							{
								flColor[0] = Menu::Window.GUITab.CTChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.GUITab.CTChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.GUITab.CTChamsB.GetValue() / 255.f;
							}

							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(covered);
							oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

							if (pModelEntity->GetTeamNum() == 2)
							{
								flColor[0] = Menu::Window.GUITab.TChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.GUITab.TChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.GUITab.TChamsB.GetValue() / 255.f;
							}
							else
							{
								flColor[0] = Menu::Window.GUITab.CTChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.GUITab.CTChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.GUITab.CTChamsB.GetValue() / 255.f;
							}

							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(open);
						}
						
					}
				}
			}
		}
		else if (ChamsStyle != 0 && Menu::Window.VisualsTab.FiltersWeapons.GetState() && strstr(ModelName, "_dropped.mdl"))
		{
			IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
			color.SetColor(255, 255, 255, 255);
			ForceMaterial(color, covered);
		}
	}

	if (!DontDraw)
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(NULL);
}

bool bGlovesNeedUpdate;
void ApplyCustomGloves(IClientEntity* pLocal)
{
	if (Menu::Window.SkinTab.EnableGloves.GetState())
	{
		if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
			return;

		if (bGlovesNeedUpdate || !pLocal->IsAlive())
		{
			DWORD* hMyWearables = (DWORD*)((size_t)pLocal + 0x2EF4);

			if (!Interfaces::EntList->GetClientEntity(hMyWearables[0] & 0xFFF))
			{
				for (ClientClass* pClass = Interfaces::Client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
				{
					if (pClass->m_ClassID != (int)CSGOClassID::CEconWearable)
						continue;

					int iEntry = (Interfaces::EntList->GetHighestEntityIndex() + 1);
					int	iSerial = RandomInt(0x0, 0xFFF);

					pClass->m_pCreateFn(iEntry, iSerial);
					hMyWearables[0] = iEntry | (iSerial << 16);

					break;
				}
			}

			player_info_t LocalPlayerInfo;
			Interfaces::Engine->GetPlayerInfo(Interfaces::Engine->GetLocalPlayer(), &LocalPlayerInfo);

			CBaseCombatWeapon* glovestochange = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntity(hMyWearables[0] & 0xFFF);

			if (!glovestochange)
				return;


			switch (Menu::Window.SkinTab.GloveModel.GetIndex())
			{
			case 1:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5027;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
				break;
			}
			case 2:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5032;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
				break;
			}
			case 3:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5031;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
				break;
			}
			case 4:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5030;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
				break;
			}
			case 5:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5033;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
				break;
			}
			case 6:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5034;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
				break;
			}
			default:
				break;
			}

			switch (Menu::Window.SkinTab.GloveSkin.GetIndex())
			{
			case 0:
				break;
			case 1:
				*glovestochange->FallbackPaintKit() = 10007;
				break;
			case 2:
				*glovestochange->FallbackPaintKit() = 10006;
				break;
			case 3:
				*glovestochange->FallbackPaintKit() = 10039;
				break;
			case 4:
				*glovestochange->FallbackPaintKit() = 10008;
				break;
			case 5:
				*glovestochange->FallbackPaintKit() = 10021;
				break;
			case 6:
				*glovestochange->FallbackPaintKit() = 10036;
				break;
			case 7:
				*glovestochange->FallbackPaintKit() = 10009;
				break;
			case 8:
				*glovestochange->FallbackPaintKit() = 10010;
				break;
			case 9:
				*glovestochange->FallbackPaintKit() = 10016;
				break;
			case 10:
				*glovestochange->FallbackPaintKit() = 10013;
				break;
			case 11:
				*glovestochange->FallbackPaintKit() = 10040;
				break;
			case 12:
				*glovestochange->FallbackPaintKit() = 10015;
				break;
			case 13:
				*glovestochange->FallbackPaintKit() = 10037;
				break;
			case 14:
				*glovestochange->FallbackPaintKit() = 10038;
				break;
			case 15:
				*glovestochange->FallbackPaintKit() = 10018;
				break;
			case 16:
				*glovestochange->FallbackPaintKit() = 10019;
				break;
			case 17:
				*glovestochange->FallbackPaintKit() = 10026;
				break;
			case 18:
				*glovestochange->FallbackPaintKit() = 10028;
				break;
			case 19:
				*glovestochange->FallbackPaintKit() = 10027;
				break;
			case 20:
				*glovestochange->FallbackPaintKit() = 10024;
				break;
			case 21:
				*glovestochange->FallbackPaintKit() = 10033;
				break;
			case 22:
				*glovestochange->FallbackPaintKit() = 10034;
				break;
			case 23:
				*glovestochange->FallbackPaintKit() = 10035;
				break;
			case 24:
				*glovestochange->FallbackPaintKit() = 10030;
				break;
			}

			*glovestochange->m_AttributeManager()->m_Item()->ItemIDHigh() = -1;
			*glovestochange->FallbackWear() = 0.001f;
			*glovestochange->m_AttributeManager()->m_Item()->AccountID() = LocalPlayerInfo.xuidlow;


			glovestochange->PreDataUpdate(0);
			bGlovesNeedUpdate = false;
		}
	}
}

void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{
	DWORD eyeangles = NetVar.GetNetVar(0xBFEA4E7B);
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	IClientEntity* pEntity;

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_RENDER_START)
	{

		if (pLocal->IsAlive())
		{
			if (*(bool*)((DWORD)Interfaces::pInput + 0xA5))
				*(Vector*)((DWORD)pLocal + 0x31C8) = LastAngleAA;
		}

		if (Menu::Window.VisualsTab.OtherThirdperson.GetState())
		{
			if (Menu::Window.VisualsTab.OtherThirdpersonRange.GetValue() > 0)
			{
				*(bool *)((DWORD)Interfaces::pInput + 0xA5) = true;
				*(float *)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = Menu::Window.VisualsTab.OtherThirdpersonRange.GetValue();
			}
			else
			{
				*(bool *)((DWORD)Interfaces::pInput + 0xA5) = false;
				*(float *)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = 0;
			}
			if (Menu::Window.VisualsTab.OtherThirdpersonKey.GetKey())
			{
				*(bool *)((DWORD)Interfaces::pInput + 0xA5) = false;
				*(float *)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = 0;

				if (GetKeyState(Menu::Window.VisualsTab.OtherThirdpersonKey.GetKey()))
				{
					*(bool *)((DWORD)Interfaces::pInput + 0xA5) = true;
					*(float *)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = Menu::Window.VisualsTab.OtherThirdpersonRange.GetValue();
				}
			}
		}
		if (!Menu::Window.VisualsTab.OtherThirdperson.GetState())
		{
			*(bool *)((DWORD)Interfaces::pInput + 0xA5) = false;
			*(float *)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = 0;
		}
	}


	if (Menu::Window.RageBotTab.PVSFix.GetState())
	{
		for (int i = 1; i < Interfaces::Globals->maxClients; i++)
		{
			if (i == Interfaces::Engine->GetLocalPlayer()) continue;
			IClientEntity* pCurEntity = Interfaces::EntList->GetClientEntity(i);
			if (!pCurEntity) continue;
			*(int*)((uintptr_t)pCurEntity + 0xA30) = Interfaces::Globals->framecount;
			*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;
		}
	}

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		ResolverSetup::GetInst().FSN(pEntity, curStage);
		int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
		int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
		int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
		int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
		int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
		int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
		int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
		int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
		int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
		int iGunGame = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

		for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (pEntity)
			{
				ApplyCustomGloves(pLocal);

				ULONG hOwnerEntity = *(PULONG)((DWORD)pEntity + 0x148);

				IClientEntity* pOwner = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hOwnerEntity);

				if (pOwner)
				{
					if (pOwner == pLocal)
					{
						std::string sWeapon = Interfaces::ModelInfo->GetModelName(pEntity->GetModel());

						if (sWeapon.find("models/weapons", 0) != std::string::npos)
							continue;

						if (sWeapon.find("c4_planted", 0) != std::string::npos)
							continue;

						if (sWeapon.find("thrown", 0) != std::string::npos)
							continue;

						if (sWeapon.find("smokegrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("flashbang", 0) != std::string::npos)
							continue;

						if (sWeapon.find("fraggrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("molotov", 0) != std::string::npos)
							continue;

						if (sWeapon.find("decoy", 0) != std::string::npos)
							continue;

						if (sWeapon.find("incendiarygrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("ied", 0) != std::string::npos)
							continue;

						if (sWeapon.find("w_eq_", 0) != std::string::npos)
							continue;

						CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;

						ClientClass *pClass = Interfaces::Client->GetAllClasses();

						if (Menu::Window.SkinTab.SkinEnable.GetState())
						{
							int Model = Menu::Window.SkinTab.KnifeModel.GetIndex();
							int M41S = Menu::Window.SkinTab.M41SSkin.GetIndex();
							int M4A4 = Menu::Window.SkinTab.M4A4Skin.GetIndex();
							int AK47 = Menu::Window.SkinTab.AK47Skin.GetIndex();
							int AWP = Menu::Window.SkinTab.AWPSkin.GetIndex();
							int GLOCK = Menu::Window.SkinTab.GLOCKSkin.GetIndex();
							int USPS = Menu::Window.SkinTab.USPSSkin.GetIndex();
							int DEAGLE = Menu::Window.SkinTab.DEAGLESkin.GetIndex();
							int FIVE7 = Menu::Window.SkinTab.FIVESEVENSkin.GetIndex();
							int AUG = Menu::Window.SkinTab.AUGSkin.GetIndex();
							int FAMAS = Menu::Window.SkinTab.FAMASSkin.GetIndex();
							int G3SG1 = Menu::Window.SkinTab.G3SG1Skin.GetIndex();
							int Galil = Menu::Window.SkinTab.GALILSkin.GetIndex();
							int M249 = Menu::Window.SkinTab.M249Skin.GetIndex();
							int MAC10 = Menu::Window.SkinTab.MAC10Skin.GetIndex();
							int P90 = Menu::Window.SkinTab.P90Skin.GetIndex();
							int UMP45 = Menu::Window.SkinTab.UMP45Skin.GetIndex();
							int XM1014 = Menu::Window.SkinTab.XM1014Skin.GetIndex();
							int BIZON = Menu::Window.SkinTab.BIZONSkin.GetIndex();
							int MAG7 = Menu::Window.SkinTab.MAG7Skin.GetIndex();
							int NEGEV = Menu::Window.SkinTab.NEGEVSkin.GetIndex();
							int SAWEDOFF = Menu::Window.SkinTab.SAWEDOFFSkin.GetIndex();
							int TEC9 = Menu::Window.SkinTab.TECNINESkin.GetIndex();
							int P2000 = Menu::Window.SkinTab.P2000Skin.GetIndex();
							int MP7 = Menu::Window.SkinTab.MP7Skin.GetIndex();
							int MP9 = Menu::Window.SkinTab.MP9Skin.GetIndex();
							int NOVA = Menu::Window.SkinTab.NOVASkin.GetIndex();
							int P250 = Menu::Window.SkinTab.P250Skin.GetIndex();
							int SCAR20 = Menu::Window.SkinTab.SCAR20Skin.GetIndex();
							int SG553 = Menu::Window.SkinTab.SG553Skin.GetIndex();
							int SSG08 = Menu::Window.SkinTab.SSG08Skin.GetIndex();
							int Magnum = Menu::Window.SkinTab.RevolverSkin.GetIndex();
							int Duals = Menu::Window.SkinTab.DUALSSkin.GetIndex();

							if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CKnife)
							{
								if (Menu::Window.SkinTab.SkinName.getText().length() > 1)
								{
									auto pCustomName = MakePtr(char*, pWeapon, 0x301C);
									strcpy_s(pCustomName, 32, Menu::Window.SkinTab.SkinName.getText().c_str());
								}
							}

							if (Menu::Window.SkinTab.StatTrackAmount.getText().c_str() != NULL && Menu::Window.SkinTab.StatTrackAmount.getText().c_str() != "")
							{
								int st = atoi(Menu::Window.SkinTab.StatTrackAmount.getText().c_str());

								if (Menu::Window.SkinTab.StatTrakEnable.GetState())
									*pWeapon->FallbackStatTrak() = st;
							}

							int weapon = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();

							switch (weapon)
							{
							case 7: // AK47 
							{
								switch (AK47)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 341;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 14;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 44;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 172;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 180;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 394;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 300;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 226;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 282;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 302;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 316;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 340;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 380;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 656;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 456;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 474;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 490;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 506;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 524;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 600;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 639;
									break;
								default:
									break;
								}
							}
							break;
							case 16: // M4A4
							{
								switch (M4A4)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 155;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 187;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 255;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 309;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 215;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 336;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 384;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 400;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 449;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 471;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 480;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 512;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 533;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 588;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 632;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 664;
									break;
								default:
									break;
								}
							}
							break;
							case 60:
							{
								switch (M41S)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 60;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 430;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 644;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 254;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 189;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 301;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 217;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 257;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 321;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 326;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 360;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 383;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 440;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 445;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 497;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 548;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 587;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 631;
									break;
								default:
									break;
								}
							}
							break;
							case 9:
							{
								switch (AWP)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 174;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 344;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 84;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 640;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 51;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 181;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 259;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 395;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 212;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 227;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 251;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 279;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 662;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 446;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 451;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 475;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 525;
									break;
								default:
									break;
								}
							}
							break;
							case 61:
							{
								switch (USPS)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 60;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 183;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 339;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 217;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 221;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 653;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 277;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 290;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 313;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 318;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 332;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 364;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 454;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 489;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 504;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 540;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 637;
									break;
								default:
									break;
								}
							}
							break;
							case 4:
							{
								switch (GLOCK)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 586;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 437;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 48;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 353;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 532;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 381;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 367;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 607;
									break;
								default:
									break;
								}
							}
							break;
							case 1: // Deagle
							{
								switch (DEAGLE)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 37;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 645;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 527;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 469;
									break;
								default:
									break;
								}
							}
							break;
							case 2: //duals
							{
								switch (Duals)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 491;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 28;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 447;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 220;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 261;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 396;
									break;
								default:
									break;
								}
							}
							break;
							case 3: // Five Seven
							{
								switch (FIVE7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 44;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 352;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 510;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 530;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 464;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 427;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 274;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 660;
									break;
								default:
									break;
								}
							}
							break;
							case 8: // AUG
							{
								switch (AUG)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 9;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 33;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 280;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 305;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 375;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 442;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 444;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 455;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 507;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 541;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 601;
									break;
								default:
									break;
								}
							}
							break;
							case 10: // Famas
							{
								switch (FAMAS)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 429;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 154;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 178;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 194;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 244;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 218;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 260;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 288;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 371;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 477;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 492;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 529;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 604;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 626;
									break;
								default:
									break;
								}
							}
							break;
							case 11: // G3SG1
							{
								switch (G3SG1)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 195;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 229;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 294;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 465;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 464;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 382;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 438;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 493;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 511;
									break;
								default:
									break;
								}
							}
							break;
							case 13: // Galil
							{
								switch (Galil)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 83;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 428;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 494;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 379;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 460;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 398;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 546;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 478;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 661;
									break;
								default:
									break;
								}
							}
							break;
							case 14: // M249
							{
								switch (M249)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 401;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 452;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 472;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 496;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 547;
									break;
								default:
									break;
								}
							}
							break;
							case 17: // Mac 10
							{
								switch (MAC10)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 433;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 98;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 157;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 188;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 337;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 246;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 284;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 310;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 333;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 343;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 372;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 402;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 498;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 534;
									break;
								default:
									break;
								}
							}
							break;
							case 19: // P90
							{
								switch (P90)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 67;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 156;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 516;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 182;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 359;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 20;
									break;
								default:
									break;
								}
							}
							break;
							case 24: // UMP-45
							{
								switch (UMP45)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 37;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 556;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 441;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 436;
									break;
								default:
									break;
								}
							}
							break;
							case 25: // XM1014
							{
								switch (XM1014)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 393;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 521;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 505;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 314;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 407;
									break;
								default:
									break;
								}
							}
							break;
							case 26: // Bizon
							{
								switch (BIZON)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 13;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 267;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 542;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 349;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 508;
									break;
								default:
									break;
								}
							}
							break;
							case 27: // Mag 7
							{
								switch (MAG7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 462;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 39;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 431;
									break;
								default:
									break;
								}
							}
							break;
							case 28: // Negev
							{
								switch (NEGEV)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 28;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 432;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 317;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 355;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 369;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 483;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 514;
									break;
								default:
									break;
								}
							}
							break;
							case 29: // Sawed Off
							{
								switch (SAWEDOFF)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 405;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 83;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 256;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 638;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 390;
									break;
								default:
									break;
								}
							}
							break;
							case 30: // Tec 9
							{
								switch (TEC9)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 463;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 303;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 248;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 520;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 374;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 614;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 555;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 459;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 179;
									break;
								default:
									break;
								}
							}
							break;
							case 32: // P2000
							{
								switch (P2000)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 485;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 184;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 211;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 389;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 591;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 246;
									break;
								default:
									break;
								}
							}
							break;
							case 33: // MP7
							{
								switch (MP7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 102;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 481;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 536;
									break;
								default:
									break;
								}
							}
							break;
							case 34: // MP9
							{
								switch (MP9)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 482;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 33;
									break;
								default:
									break;
								}
							}
							break;
							case 35: // Nova
							{
								switch (NOVA)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 3;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 166;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 164;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 25;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 62;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 99;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 107;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 158;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 170;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 191;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 214;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 225;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 263;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 286;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 294;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 299;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 356;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 450;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 484;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 537;
									break;
								default:
									break;
								}
							}
							break;
							case 36: // P250
							{
								switch (P250)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 102;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 168;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 162;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 258;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 551;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 271;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 295;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 358;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 388;
									break;
								default:
									break;
								}
							}
							break;
							case 38: // Scar 20
							{
								switch (SCAR20)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 165;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 196;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 232;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 391;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 597;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 312;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 406;
									break;
									break;
								}
							}
							break;
							case 39: // SG553
							{
								switch (SG553)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 39;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 98;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 410;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 347;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 287;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 298;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 363;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 378;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 487;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 519;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 553;
									break;
								default:
									break;
								}
							}
							break;
							case 40: // SSG08
							{
								switch (SSG08)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 624;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 222;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 554;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 319;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 361;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 503;
									break;
								default:
									break;
								}
							}
							break;
							case 64: // Revolver
							{
								switch (Magnum)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 522;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 12;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 523;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 595;
									break;
								default:
									break;
								}
							}
							break;
							default:
								break;
							}

							if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
							{
								auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);
								if (Menu::Window.SkinTab.KnifeName.getText().length() > 1)
								{
									strcpy_s(pCustomName1, 32, Menu::Window.SkinTab.KnifeName.getText().c_str());
								}

								if (Model == 0) // Bayonet
								{
									*pWeapon->ModelIndex() = iBayonet;
									*pWeapon->ViewModelIndex() = iBayonet;
									*pWeapon->WorldModelIndex() = iBayonet + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 500;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 1) // Bowie Knife
								{
									*pWeapon->ModelIndex() = iBowie;
									*pWeapon->ViewModelIndex() = iBowie;
									*pWeapon->WorldModelIndex() = iBowie + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 514;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 2) // Butterfly Knife
								{
									*pWeapon->ModelIndex() = iButterfly;
									*pWeapon->ViewModelIndex() = iButterfly;
									*pWeapon->WorldModelIndex() = iButterfly + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 515;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 3) // Falchion Knife
								{
									*pWeapon->ModelIndex() = iFalchion;
									*pWeapon->ViewModelIndex() = iFalchion;
									*pWeapon->WorldModelIndex() = iFalchion + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 512;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 4) // Flip Knife
								{
									*pWeapon->ModelIndex() = iFlip;
									*pWeapon->ViewModelIndex() = iFlip;
									*pWeapon->WorldModelIndex() = iFlip + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 505;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 5) // Gut Knife
								{
									*pWeapon->ModelIndex() = iGut;
									*pWeapon->ViewModelIndex() = iGut;
									*pWeapon->WorldModelIndex() = iGut + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 506;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 6) // Huntsman Knife
								{
									*pWeapon->ModelIndex() = iHuntsman;
									*pWeapon->ViewModelIndex() = iHuntsman;
									*pWeapon->WorldModelIndex() = iHuntsman + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 509;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 7) // Karambit
								{
									*pWeapon->ModelIndex() = iKarambit;
									*pWeapon->ViewModelIndex() = iKarambit;
									*pWeapon->WorldModelIndex() = iKarambit + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 507;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 8) // M9 Bayonet
								{
									*pWeapon->ModelIndex() = iM9Bayonet;
									*pWeapon->ViewModelIndex() = iM9Bayonet;
									*pWeapon->WorldModelIndex() = iM9Bayonet + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 508;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 10) // Shadow Daggers
								{
									*pWeapon->ModelIndex() = iDagger;
									*pWeapon->ViewModelIndex() = iDagger;
									*pWeapon->WorldModelIndex() = iDagger + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 516;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinTab.KnifeSkin.getText().c_str());
								}
							}

							*pWeapon->OwnerXuidLow() = 0;
							*pWeapon->OwnerXuidHigh() = 0;
							*pWeapon->FallbackWear() = 0.001f;
							*pWeapon->m_AttributeManager()->m_Item()->ItemIDHigh() = 1;

						}
					}
				}

			}
		}
	}

	oFrameStageNotify(curStage);
}

void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup)
	{
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
		{
			if (Menu::Window.VisualsTab.Active.GetState() && pLocal->IsAlive() && !pLocal->IsScoped())
			{
				if (pSetup->fov = 90)
					pSetup->fov = Menu::Window.VisualsTab.OtherFOV.GetValue();
			}

			oOverrideView(ecx, edx, pSetup);
		}

	}

void Hooks::Junk2() {
	float woUliSRijs = 62443696972436; woUliSRijs = 79567672857880; if (woUliSRijs = 11099796422799) woUliSRijs = 83527662350582; woUliSRijs = 69092862794291; woUliSRijs = 27942916909286;
	if (woUliSRijs = 92272288352766)woUliSRijs = 64227997002669; woUliSRijs = 22029148993489;
	if (woUliSRijs = 75127549656985)woUliSRijs = 64227997002669; woUliSRijs = 22029148993489;
	if (woUliSRijs = 75127549656985)woUliSRijs = 64227997002669; woUliSRijs = 22029148993489;
	if (woUliSRijs = 75127549656985)woUliSRijs = 64227997002669; woUliSRijs = 22029148993489;
	if (woUliSRijs = 75127549656985)woUliSRijs = 64227997002669; woUliSRijs = 22029148993489; woUliSRijs = 99844091664017;
	float zOFLlxVpeu = 71851109391196; zOFLlxVpeu = 42934792113069; if (zOFLlxVpeu = 33730372622811) zOFLlxVpeu = 53178738161108; zOFLlxVpeu = 88887221981717; zOFLlxVpeu = 19817178888722;
	if (zOFLlxVpeu = 25496095317873)zOFLlxVpeu = 26228116444883; zOFLlxVpeu = 25022801580496;
	if (zOFLlxVpeu = 80464096921984)zOFLlxVpeu = 26228116444883; zOFLlxVpeu = 25022801580496;
	if (zOFLlxVpeu = 80464096921984)zOFLlxVpeu = 26228116444883; zOFLlxVpeu = 25022801580496;
	if (zOFLlxVpeu = 80464096921984)zOFLlxVpeu = 26228116444883; zOFLlxVpeu = 25022801580496;
	if (zOFLlxVpeu = 80464096921984)zOFLlxVpeu = 26228116444883; zOFLlxVpeu = 25022801580496; zOFLlxVpeu = 40947398135516;
	float cRGXazdVYW = 70568316765906; cRGXazdVYW = 24407708183071; if (cRGXazdVYW = 43938113728953) cRGXazdVYW = 54140556610347; cRGXazdVYW = 28518861087097; cRGXazdVYW = 10870972851886;
	if (cRGXazdVYW = 26297355414055)cRGXazdVYW = 37289531357077; cRGXazdVYW = 81868647005991;
	if (cRGXazdVYW = 30821811627903)cRGXazdVYW = 37289531357077; cRGXazdVYW = 81868647005991;
	if (cRGXazdVYW = 30821811627903)cRGXazdVYW = 37289531357077; cRGXazdVYW = 81868647005991;
	if (cRGXazdVYW = 30821811627903)cRGXazdVYW = 37289531357077; cRGXazdVYW = 81868647005991;
	if (cRGXazdVYW = 30821811627903)cRGXazdVYW = 37289531357077; cRGXazdVYW = 81868647005991; cRGXazdVYW = 68452547641540;
	float tclOwdmUvh = 78789547407799; tclOwdmUvh = 54809928636018; if (tclOwdmUvh = 75033692661223) tclOwdmUvh = 53247379355173; tclOwdmUvh = 88266269792516; tclOwdmUvh = 97925168826626;
	if (tclOwdmUvh = 45967735324737)tclOwdmUvh = 26612231724984; tclOwdmUvh = 50315842710846;
	if (tclOwdmUvh = 60005766523842)tclOwdmUvh = 26612231724984; tclOwdmUvh = 50315842710846;
	if (tclOwdmUvh = 60005766523842)tclOwdmUvh = 26612231724984; tclOwdmUvh = 50315842710846;
	if (tclOwdmUvh = 60005766523842)tclOwdmUvh = 26612231724984; tclOwdmUvh = 50315842710846;
	if (tclOwdmUvh = 60005766523842)tclOwdmUvh = 26612231724984; tclOwdmUvh = 50315842710846; tclOwdmUvh = 16509473140235;
	float fzUtLcBMUe = 91313858026699; fzUtLcBMUe = 61463076834259; if (fzUtLcBMUe = 9896017543628) fzUtLcBMUe = 50306189494875; fzUtLcBMUe = 58552244018003; fzUtLcBMUe = 40180035855224;
	if (fzUtLcBMUe = 7071165030618)fzUtLcBMUe = 75436288778997; fzUtLcBMUe = 1741807625831;
	if (fzUtLcBMUe = 68686563830488)fzUtLcBMUe = 75436288778997; fzUtLcBMUe = 1741807625831;
	if (fzUtLcBMUe = 68686563830488)fzUtLcBMUe = 75436288778997; fzUtLcBMUe = 1741807625831;
	if (fzUtLcBMUe = 68686563830488)fzUtLcBMUe = 75436288778997; fzUtLcBMUe = 1741807625831;
	if (fzUtLcBMUe = 68686563830488)fzUtLcBMUe = 75436288778997; fzUtLcBMUe = 1741807625831; fzUtLcBMUe = 30915139251884;
}

void GetViewModelFOV(float& fov)
{
	IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		if (!localplayer)
			return;


		if (Menu::Window.VisualsTab.Active.GetState())
		fov += Menu::Window.VisualsTab.OtherViewmodelFOV.GetValue();
	}
}

float __stdcall GGetViewModelFOV()
{
	float fov = Hooks::VMTClientMode.GetMethod<oGetViewModelFOV>(35)();

	GetViewModelFOV(fov);

	return fov;
}

void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
{
	static DWORD oRenderView = Hooks::VMTRenderView.GetOriginalFunction(6);

	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	__asm
	{
		PUSH whatToDraw
		PUSH nClearFlags
		PUSH hudViewSetup
		PUSH setup
		MOV ECX, ecx
		CALL oRenderView
	}
}