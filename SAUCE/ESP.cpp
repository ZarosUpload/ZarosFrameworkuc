

#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "AntiAntiAim.h"
#include "MiscClasses.h"
#include "entities.h"


DWORD GlowManager = *(DWORD*)(Utilities::Memory::FindPatternV2("client.dll", "0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00") + 3);

void CEsp::Init()
{
	BombCarrier = nullptr;
}

void CEsp::Move(CUserCmd *pCmd, bool &bSendPacket)
{

}

// Main ESP Drawing loop
void CEsp::Draw()
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	{
		if (Menu::Window.VisualsTab.GrenadeTrace.GetState())
		{
			GrenadeTrace();
		}

		if (Menu::Window.MiscTab.OtherSpectators.GetState())
		{
			SpecList();
		}

		// Loop through all active entitys
		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			// Get the entity
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
			player_info_t pinfo;

			// The entity isn't some laggy peice of shit or something
			if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
			{
				// Radar
				if (Menu::Window.VisualsTab.OtherRadar.GetState())
				{
					DWORD m_bSpotted = NetVar.GetNetVar(0x839EB159);
					*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
				}

				// Is it a player?!
				if (Menu::Window.VisualsTab.FiltersPlayers.GetState() && Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
				{
					DrawPlayer(pEntity, pinfo);
				}



				// ~ Other ESP's here (items and shit) ~ //
				ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();

				if (Menu::Window.VisualsTab.FiltersNades.GetState())
				{
					if (cClass->m_ClassID == (int)CSGOClassID::CBaseCSGrenadeProjectile)
						DrawHE(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CMolotovProjectile)
						DrawMolotov(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CDecoyProjectile)
						DrawDecoy(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CSensorGrenadeProjectile)
						DrawMolotov(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CSmokeGrenadeProjectile)
						DrawSmoke(pEntity, cClass);
				}

				// Dropped weapons
				if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDeagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
				{
					DrawDrop(pEntity, cClass);
				}

				// If entity is the bomb
				if (Menu::Window.VisualsTab.FiltersC4.GetState())
				{
					if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
						DrawBombPlanted(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CC4)
						DrawBomb(pEntity, cClass);
				}

				// If entity is a chicken
				if (Menu::Window.VisualsTab.FiltersChickens.GetState())
				{
					if (cClass->m_ClassID == (int)CSGOClassID::CChicken)
						DrawChicken(pEntity, cClass);


				}
			}
		}

		//Noflash
		if (Menu::Window.VisualsTab.OtherNoFlash.GetState())
		{
			DWORD m_flFlashMaxAlpha = NetVar.GetNetVar(0xFE79FB98);
			*(float*)((DWORD)pLocal + m_flFlashMaxAlpha) = Menu::Window.VisualsTab.OtherNoFlashAlpha.GetValue();
		}


	}


}

// Spectator List
void CEsp::SpecList()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();
	int ayy = 0;

	// Loop through all active entitys
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != pLocal)
		{
			if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
			{
				HANDLE obs = pEntity->GetObserverTargetHandle();

				if (obs)
				{
					IClientEntity *pTarget = Interfaces::EntList->GetClientEntityFromHandle(obs);
					player_info_t pinfo2;
					if (pTarget)
					{
						if (Interfaces::Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
						{
							if (strlen(pinfo.name) > 16)
							{
								pinfo.name[16] = 0;
								strcat(pinfo.name, "...");
								puts(pinfo.name);
								char buf[255]; sprintf(buf, "%s => %s", pinfo.name, pinfo2.name);
								RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, buf);
								Render::Clear(scrn.right - 260, (scrn.bottom / 2) + (16 * ayy), 260, 16, Color(0, 0, 0, 140));
								Render::Text(scrn.right - TextSize.right - 4, (scrn.bottom / 2) + (16 * ayy), pTarget->GetIndex() == pLocal->GetIndex() ? Color(240, 70, 80, 255) : Color(255, 255, 255, 255), Render::Fonts::ESP, buf);
								ayy++;
							}
						}
					}
				}
			}
		}
	}

	Render::Outline(scrn.right - 261, (scrn.bottom / 2) - 1, 262, (16 * ayy) + 2, Color(23, 23, 23, 255));
	Render::Outline(scrn.right - 260, (scrn.bottom / 2), 260, (16 * ayy), Color(90, 90, 90, 255));
}

//  Tickboxes
void CEsp::DrawPlayer(IClientEntity* pEntity, player_info_t pinfo)
{
	ESPBox Box;
	Color Color;

	// Show own team false? well gtfo teammate lol
	if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (pEntity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
		return;

	if (GetBox(pEntity, Box))
	{
		Color = GetPlayerColor(pEntity);

		if (Menu::Window.VisualsTab.OptionsBox.GetState())
			DrawBox(Box, Color);

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box);

		if (Menu::Window.VisualsTab.OptionHealthEnable.GetState())
			DrawHealth(pEntity, Box);


		if (Menu::Window.VisualsTab.OptionsInfo.GetState() || Menu::Window.VisualsTab.OptionsWeapon.GetState())
			DrawInfo(pEntity, Box);


		if (Menu::Window.VisualsTab.OptionsAimSpot.GetState())
			DrawCross(pEntity);

		if (Menu::Window.VisualsTab.OptionsSkeleton.GetState())
			DrawSkeleton(pEntity);


		if (Menu::Window.VisualsTab.OptionsGlow.GetState())
		{
			DrawGlow();
		}
		if (Menu::Window.VisualsTab.EntityGlow.GetState())
		{
			EntityGlow();
		}
		if (Menu::Window.VisualsTab.OptionsArmur.GetState() == 1)
			DrawArmor(pEntity, Box);

		if (Menu::Window.VisualsTab.OptionsFill.GetState())
			Fill(Box, Color);

		if (Menu::Window.VisualsTab.LBY.GetState())
			lbyup(pEntity, Box);

		Junk4();

	}
}



void CEsp::Junk4()
{
	float pJunkcode = 5113275511453;
	pJunkcode = 134111559242;
	if (pJunkcode = 171352425)
		pJunkcode = 183215511425;
	pJunkcode = 1731145525;
	pJunkcode = 1118955511;
	if (pJunkcode = 349273490534);
	pJunkcode = 230489748943;
	pJunkcode = 4399055453;
	if (pJunkcode = 17859425)
		pJunkcode = 181555912425;
	pJunkcode = 173255595;
	pJunkcode = 43578195346534;
	if (pJunkcode = 343555554);
	pJunkcode = 230489251374118943;
	pJunkcode = 43907513945113;
	if (pJunkcode = 1732189342425)
		pJunkcode = 183413254113425;
	pJunkcode = 1732423198465;
	pJunkcode = 435789513416534;
	if (pJunkcode = 349345574);
	pJunkcode = 230489214189143346;
	pJunkcode = 4390437910453;
	float LGGrxnFfzi = 2978678590020; LGGrxnFfzi = 90775251329102; if (LGGrxnFfzi = 79039549484475) LGGrxnFfzi = 99569238257106; LGGrxnFfzi = 72530676294199; LGGrxnFfzi = 62941997253067;
	if (LGGrxnFfzi = 1642709956923)LGGrxnFfzi = 94844755895034; LGGrxnFfzi = 87283343948475;
	if (LGGrxnFfzi = 90540445814364)LGGrxnFfzi = 94844755895034; LGGrxnFfzi = 87283343948475;
	if (LGGrxnFfzi = 90540445814364)LGGrxnFfzi = 94844755895034; LGGrxnFfzi = 87283343948475;
	if (LGGrxnFfzi = 90540445814364)LGGrxnFfzi = 94844755895034; LGGrxnFfzi = 87283343948475;
	if (LGGrxnFfzi = 90540445814364)LGGrxnFfzi = 94844755895034; LGGrxnFfzi = 87283343948475; LGGrxnFfzi = 32938883141831;
	float tLndPNdtwr = 84389253817008; tLndPNdtwr = 69382855689882; if (tLndPNdtwr = 62198694190957) tLndPNdtwr = 13171712273810; tLndPNdtwr = 66720781287899; tLndPNdtwr = 12878996672078;
	if (tLndPNdtwr = 62680231317171)tLndPNdtwr = 41909571580534; tLndPNdtwr = 68188391287070;
	if (tLndPNdtwr = 9579392093646)tLndPNdtwr = 41909571580534; tLndPNdtwr = 68188391287070;
	if (tLndPNdtwr = 9579392093646)tLndPNdtwr = 41909571580534; tLndPNdtwr = 68188391287070;
	if (tLndPNdtwr = 9579392093646)tLndPNdtwr = 41909571580534; tLndPNdtwr = 68188391287070;
	if (tLndPNdtwr = 9579392093646)tLndPNdtwr = 41909571580534; tLndPNdtwr = 68188391287070; tLndPNdtwr = 2612746667163;
	float NeErHGryYq = 10795069122931; NeErHGryYq = 70914965053646; if (NeErHGryYq = 2853822220011) NeErHGryYq = 8643882186244; NeErHGryYq = 80061521093048; NeErHGryYq = 10930488006152;
	if (NeErHGryYq = 92219908643882)NeErHGryYq = 2220011853385; NeErHGryYq = 15344902431864;
	if (NeErHGryYq = 3983138898258)NeErHGryYq = 2220011853385; NeErHGryYq = 15344902431864;
	if (NeErHGryYq = 3983138898258)NeErHGryYq = 2220011853385; NeErHGryYq = 15344902431864;
	if (NeErHGryYq = 3983138898258)NeErHGryYq = 2220011853385; NeErHGryYq = 15344902431864;
	if (NeErHGryYq = 3983138898258)NeErHGryYq = 2220011853385; NeErHGryYq = 15344902431864; NeErHGryYq = 92291349083648; float ygpoTKFpau = 4602847016106; ygpoTKFpau = 35182541943666; if (ygpoTKFpau = 60478344944090) ygpoTKFpau = 40797729687967; ygpoTKFpau = 2177702650335; ygpoTKFpau = 2650335217770;
	if (ygpoTKFpau = 73233664079772)ygpoTKFpau = 49440909067056; ygpoTKFpau = 65106185304992;
	if (ygpoTKFpau = 71243856433662)ygpoTKFpau = 49440909067056; ygpoTKFpau = 65106185304992;
	if (ygpoTKFpau = 71243856433662)ygpoTKFpau = 49440909067056; ygpoTKFpau = 65106185304992;
	if (ygpoTKFpau = 71243856433662)ygpoTKFpau = 49440909067056; ygpoTKFpau = 65106185304992;
	if (ygpoTKFpau = 71243856433662)ygpoTKFpau = 49440909067056; ygpoTKFpau = 65106185304992; ygpoTKFpau = 87714605684943;

};

// Gets the 2D bounding box for the entity
bool CEsp::GetBox(IClientEntity* pEntity, CEsp::ESPBox &result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!Render::WorldToScreen(points[3], flb) || !Render::WorldToScreen(points[5], brt)
		|| !Render::WorldToScreen(points[0], blb) || !Render::WorldToScreen(points[4], frt)
		|| !Render::WorldToScreen(points[2], frb) || !Render::WorldToScreen(points[1], brb)
		|| !Render::WorldToScreen(points[6], blt) || !Render::WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}

// Get an entities color depending on team and vis ect
Color CEsp::GetPlayerColor(IClientEntity* pEntity)
{
	int TeamNum = pEntity->GetTeamNum();
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head);

	int rctvisesp = Menu::Window.GUITab.RBoxCTV.GetValue();
	int gctvisesp = Menu::Window.GUITab.GBoxCTV.GetValue();
	int bctvisesp = Menu::Window.GUITab.BBoxCTV.GetValue();
	int actvisesp = Menu::Window.GUITab.ABoxCTV.GetValue();

	int rctesp = Menu::Window.GUITab.RBoxCT.GetValue();
	int gctesp = Menu::Window.GUITab.GBoxCT.GetValue();
	int bctesp = Menu::Window.GUITab.BBoxCT.GetValue();
	int actesp = Menu::Window.GUITab.ABoxCT.GetValue();

	int rtvisesp = Menu::Window.GUITab.RBoxTV.GetValue();
	int gtvisesp = Menu::Window.GUITab.GBoxTV.GetValue();
	int btvisesp = Menu::Window.GUITab.BBoxTV.GetValue();
	int atvisesp = Menu::Window.GUITab.ABoxTV.GetValue();

	int rtesp = Menu::Window.GUITab.RBoxT.GetValue();
	int gtesp = Menu::Window.GUITab.GBoxT.GetValue();
	int btesp = Menu::Window.GUITab.BBoxT.GetValue();
	int atesp = Menu::Window.GUITab.ABoxT.GetValue();

	Color color;

	if (TeamNum == TEAM_CS_T)
	{
		if (IsVis)
			color = Color(rtvisesp, gtvisesp, btvisesp, atvisesp);
		else
			color = Color(rtesp, gtesp, btesp, atesp);
	}
	else
	{
		if (IsVis)
			color = Color(rctvisesp, gctvisesp, bctvisesp, actvisesp);
		else
			color = Color(rctesp, gctesp, bctesp, actesp);
	}


	return color;
}

// 2D  Esp box
void CEsp::DrawBox(CEsp::ESPBox size, Color color)
{
	static float rainbow;
	rainbow += 0.005f;
	if (rainbow > 1.f) rainbow = 0.f;
	//if (PlayerBoxes->GetStringIndex() == 1)
	//{
	// Full Box
	//Render::Clear(size.x, size.y, size.w, size.h, color);
	//Render::Clear(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 150)); 
	//Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 150));
	//}
	//else
	if (Menu::Window.VisualsTab.BoxDesign.GetIndex() == 0)
	{
		// Corner Box
		int VertLine = (((float)size.w) * (0.20f));
		int HorzLine = (((float)size.h) * (0.20f));

		Render::Clear(size.x, size.y - 1, VertLine, 1, Color(10, 10, 10, 150));
		Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(10, 10, 10, 150));
		Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 150));
		Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 150));

		Render::Clear(size.x - 1, size.y, 1, HorzLine, Color(10, 10, 10, 150));
		Render::Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));
		Render::Clear(size.x + size.w - 1, size.y, 1, HorzLine, Color(10, 10, 10, 150));
		Render::Clear(size.x + size.w - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));

		Render::Clear(size.x, size.y, VertLine, 1, color);
		Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
		Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
		Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

		Render::Clear(size.x, size.y, 1, HorzLine, color);
		Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
		Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
		Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
	}
	else if (Menu::Window.VisualsTab.BoxDesign.GetIndex() == 1)
	{
		// Full Box
		Render::Outline(size.x, size.y, size.w, size.h, color);
		Render::Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 150));
		Render::Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 150));
	}
	else if (Menu::Window.VisualsTab.BoxDesign.GetIndex() == 2)
	{
		// Full Box Rainbow
		Render::Outline(size.x, size.y, size.w, size.h, color);
		Render::Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color::FromHSB(rainbow, 1.f, 1.f));
		Render::Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color::FromHSB(rainbow, 1.f, 1.f));
	}
}

void CEsp::DrawArmor(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox ArmorBar = size;
	if (Menu::Window.VisualsTab.OptionsArmur.GetState() == 1)
		ArmorBar.y += (ArmorBar.h + 6);
	else
		ArmorBar.y += (ArmorBar.h + 0);

	ArmorBar.h = 4;

	float ArmorValue = pEntity->ArmorValue();
	float ArmorPerc = ArmorValue / 100.f;
	float Width = (size.w * ArmorPerc);
	ArmorBar.w = Width;

	//  Main Bar  //
	Vertex_t Verts[4];
	Verts[0].Init(Vector2D(ArmorBar.x, ArmorBar.y));
	Verts[1].Init(Vector2D(ArmorBar.x + size.w, ArmorBar.y));
	Verts[2].Init(Vector2D(ArmorBar.x + size.w, ArmorBar.y + 5));
	Verts[3].Init(Vector2D(ArmorBar.x, ArmorBar.y + 5));

	Render::PolygonOutline(4, Verts, Color(0, 0, 0, 255), Color(0, 0, 0, 255));

	Vertex_t Verts2[4];
	Verts2[0].Init(Vector2D(ArmorBar.x + 1, ArmorBar.y + 1));
	Verts2[1].Init(Vector2D(ArmorBar.x + ArmorBar.w, ArmorBar.y + 1));
	Verts2[2].Init(Vector2D(ArmorBar.x + ArmorBar.w, ArmorBar.y + 5));
	Verts2[3].Init(Vector2D(ArmorBar.x, ArmorBar.y + 5));

	Color c = Color(0, 205, 247, 255);
	Render::Polygon(4, Verts2, c);

	Verts2[0].Init(Vector2D(ArmorBar.x + 1, ArmorBar.y + 1));
	Verts2[1].Init(Vector2D(ArmorBar.x + ArmorBar.w, ArmorBar.y + 1));
	Verts2[2].Init(Vector2D(ArmorBar.x + ArmorBar.w, ArmorBar.y + 2));
	Verts2[3].Init(Vector2D(ArmorBar.x, ArmorBar.y + 2));

	Render::Polygon(4, Verts2, Color(200, 0, 200, 255));
}



// Unicode Conversions
static wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size / 4, text, size);
	return wa;
}

// Player name
void CEsp::DrawName(player_info_t pinfo, CEsp::ESPBox size)
{
	if (strlen(pinfo.name) > 16)
	{
		pinfo.name[16] = 0;
		strcat(pinfo.name, "...");
		puts(pinfo.name);
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
	}
	else
	{
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
	}
}

// Draw a health bar. For Tf2 when a bar is bigger than max health a second bar is displayed
void CEsp::DrawHealth(IClientEntity* pEntity, CEsp::ESPBox size)
{
	if (Menu::Window.VisualsTab.OptionsHealth.GetIndex() == 0)
	{
		ESPBox HealthBar = size;
		HealthBar.y += (HealthBar.h + 6);
		HealthBar.h = 4;

		float HealthValue = pEntity->GetHealth();
		float HealthPerc = HealthValue / 100.f;
		float Width = (size.w * HealthPerc);
		HealthBar.w = Width;

		int health = pEntity->GetHealth();

		if (health > 100)
			health = 100;

		int r = 255 - health * 2.55;
		int g = health * 2.55;

		int healthBar = size.h / 100 * health;
		int healthBarDelta = size.h - healthBar;

		//int iClampedHealth = pEntity->GetHealth();
		//	if (iClampedHealth >= 100)
		//	iClampedHealth = 100;

		Render::Outline(size.x - 4, size.y + 1, 1, size.h * 0.01 * health, Color(r, g, 0, 255));
		Render::Outline(size.x - 5, size.y - 1, 3, size.h + 2, Color(0, 0, 0, 150));
	}
	if (Menu::Window.VisualsTab.OptionsHealth.GetIndex() == 1)
	{
		int HPEnemy = 100;
		HPEnemy = pEntity->GetHealth();
		char nameBuffer[512];
		sprintf(nameBuffer, "%d", HPEnemy);


		float h = (size.h);
		float offset = (h / 4.f) + 5;
		float w = h / 64.f;
		float health = pEntity->GetHealth();
		UINT hp = h - (UINT)((h * health) / 100);

		int Red = 255 - (health*2.55);
		int Green = health*2.55;

		Render::DrawOutlinedRect((size.x - 6) - 1, size.y - 1, 3, h + 2, Color(0, 0, 0, 180));

		Render::DrawLine((size.x - 6), size.y + hp, (size.x - 6), size.y + h, Color(Red, Green, 0, 180));

		if (health < 100) {

			Render::Text(size.x - 9, size.y + hp, Color(255, 255, 255, 255), Render::Fonts::ESP, nameBuffer);
		}
	}
}

// Cleans the internal class name up to something human readable and nice
std::string CleanItemName(std::string name)
{
	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}



void CEsp::Info(IClientEntity* pEntity, CEsp::ESPBox size)
{
	std::vector<std::string> Info;

	if (Menu::Window.VisualsTab.HasDefuser.GetState() && pEntity->HasDefuser())
	{
		Info.push_back("Has Defuser");
	}

	if (Menu::Window.VisualsTab.IsScoped.GetState() && pEntity->IsScoped())
	{
		Info.push_back("Scoped");
	}

	static RECT Size = Render::GetTextSize(Render::Fonts::ESP, "Hi");
	int i = 0;
	for (auto Text : Info)
	{
		Render::Text(size.x + size.w + 3, size.y + (i*(Size.bottom + 2)), Color(255, 255, 255, 255), Render::Fonts::ESP, Text.c_str());
		i++;
	}
}

// Anything else: weapons, class state? idk
void CEsp::DrawInfo(IClientEntity* pEntity, CEsp::ESPBox size)
{
	std::vector<std::string> Info;

	// Player Weapon ESP Text
	IClientEntity* pWeapon = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (Menu::Window.VisualsTab.OptionsWeapon.GetState() && pWeapon)
	{
		ClientClass* cClass = (ClientClass*)pWeapon->GetClientClass();
		if (cClass)
		{
			// Draw it
			Info.push_back(CleanItemName(cClass->m_pNetworkName));
		}
	}




	// Bomb Carrier
	if (Menu::Window.VisualsTab.OptionsInfo.GetState() && pEntity == BombCarrier)
	{
		Info.push_back("Bomb Carrier");
	}

	static RECT Size = Render::GetTextSize(Render::Fonts::Default, "Hi");
	int i = 0;
	for (auto Text : Info)

	{
		Render::Text(size.x + size.w + 3, size.y + (i*(Size.bottom + 2)), Color(255, 255, 255, 255), Render::Fonts::ESP, Text.c_str());
		i++;
	}
}



void CEsp::GrenadeTrace()
{
	if (Menu::Window.VisualsTab.GrenadeTrace.GetState())
	{
		auto granade = Interfaces::CVar->FindVar("sv_grenade_trajectory");
		auto granadespoof = new SpoofedConvar(granade);
		granadespoof->SetInt(1);
	}
}

// Little circle on their heads
void CEsp::DrawCross(IClientEntity* pEntity)
{

	Vector cross = pEntity->GetHeadPos(), screen;
	static int Scale = 2;
	if (Render::WorldToScreen(cross, screen))
	{

		Render::DrawCircle(screen.x - Scale, screen.y - (Scale * 2), (Scale * 2), (Scale * 4), Color(255, 255, 255, 255));
	}

}

void CEsp::Junk() {
	float pJunkcode = 5113278119111453;
	pJunkcode = 13411111111189242;
	if (pJunkcode = 171111132425)
		pJunkcode = 183211111111425;
	pJunkcode = 173200000000425;
	pJunkcode = 11117898911;
	if (pJunkcode = 3492511173490534);
	pJunkcode = 2304892193748943;
	pJunkcode = 43900000000453;
	if (pJunkcode = 171389425)
		pJunkcode = 18111138912425;
	pJunkcode = 17324111111895;
	pJunkcode = 43578195346534;
	if (pJunkcode = 3492123473490534);
	pJunkcode = 23048921374118943;
	pJunkcode = 43907513945113;
	if (pJunkcode = 1732189342425)
		pJunkcode = 183543254113425;
	pJunkcode = 1732423198465;
	pJunkcode = 43222222216534;
	if (pJunkcode = 34934555555474);
	pJunkcode = 23333333333343346;
	pJunkcode = 4390437910453;
}

void AngleVectors123(const Vector &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}

// Draws a dropped CS:GO Item
void CEsp::DrawDrop(IClientEntity* pEntity, ClientClass* cClass)
{
	Vector Box;
	CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)pEntity;
	IClientEntity* plr = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)Weapon->GetOwnerHandle());
	if (!plr && Render::WorldToScreen(Weapon->GetOrigin(), Box))
	{
		if (Menu::Window.VisualsTab.OptionsBox.GetState())
		{
			Render::Outline(Box.x - 2, Box.y - 2, 4, 4, Color(255, 255, 255, 255));
			Render::Outline(Box.x - 3, Box.y - 3, 6, 6, Color(10, 10, 10, 150));
		}

		if (Menu::Window.VisualsTab.OptionsInfo.GetState())
		{
			std::string ItemName = CleanItemName(cClass->m_pNetworkName);
			RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, ItemName.c_str());
			Render::Text(Box.x - (TextSize.right / 2), Box.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, ItemName.c_str());
		}
	}
}

void CEsp::lbyup(IClientEntity* pEntity, CEsp::ESPBox size)
{
	if (pEntity->GetFlags() & FL_ONGROUND && pEntity->GetVelocity().Length2D() != 1)
	{
		RECT defSize = Render::GetTextSize(Render::Fonts::ESP, "");
		Render::Text(size.x + size.w + 3, size.y + (0.3*(defSize.bottom + 15)),
			Color(0, 255, 0, 255), Render::Fonts::ESP, "LBY");
	}

	if (pEntity->GetFlags() & FL_ONGROUND && pEntity->GetVelocity().Length2D() != 0)
	{
		RECT defSize = Render::GetTextSize(Render::Fonts::ESP, "");
		Render::Text(size.x + size.w + 3, size.y + (0.3*(defSize.bottom + 15)),
			Color(255, 0, 0, 255), Render::Fonts::ESP, "LBY");
	}
}

// Draws a chicken
void CEsp::DrawChicken(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Chicken");
		if (Menu::Window.VisualsTab.OptionsBox.GetState())
			DrawBox(Box, Color(255, 255, 255, 255));

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box);
	}
}

// Draw the planted bomb and timer
void CEsp::DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;

	if (Render::WorldToScreen(vOrig, vScreen))
	{
		float flBlow = Bomb->GetC4BlowTime();
		float TimeRemaining = flBlow - (Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
		char buffer[64];
		sprintf(buffer, "Bomb explodes in %.1f", TimeRemaining);
		Render::Text(vScreen.x, vScreen.y, Color(255, 255, 255, 255), Render::Fonts::ESP, buffer);
	}
}

void CEsp::Fill(CEsp::ESPBox size, Color color)
{

	if (Menu::Window.VisualsTab.OptionsFill.GetState())
	{
		Render::Clear(size.x, size.y, size.w, size.h, Color(20, 20, 20, 120));
	}
	else {

	}
}//+

 // Draw the bomb if it's dropped, or store the player who's carrying 
void CEsp::DrawBomb(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	bool adopted = true;
	HANDLE parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		IClientEntity* pParentEnt = (Interfaces::EntList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}

	if (adopted)
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			Render::Text(vScreen.x, vScreen.y, Color(112, 230, 20, 255), Render::Fonts::ESP, "Bomb");
		}
	}
}

void DrawBoneArray(int* boneNumbers, int amount, IClientEntity* pEntity, Color color)
{
	Vector LastBoneScreen;
	for (int i = 0; i < amount; i++)
	{
		Vector Bone = pEntity->GetBonePos(boneNumbers[i]);
		Vector BoneScreen;

		if (Render::WorldToScreen(Bone, BoneScreen))
		{
			if (i>0)
			{
				Render::Line(LastBoneScreen.x, LastBoneScreen.y, BoneScreen.x, BoneScreen.y, color);
			}
		}
		LastBoneScreen = BoneScreen;
	}
}

void DrawBoneTest(IClientEntity *pEntity)
{
	for (int i = 0; i < 127; i++)
	{
		Vector BoneLoc = pEntity->GetBonePos(i);
		Vector BoneScreen;
		if (Render::WorldToScreen(BoneLoc, BoneScreen))
		{
			char buf[10];
			_itoa_s(i, buf, 10);
			Render::Text(BoneScreen.x, BoneScreen.y, Color(255, 255, 255, 180), Render::Fonts::ESP, buf);
		}
	}
}

void CEsp::DrawSkeleton(IClientEntity* pEntity)
{
	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			if (Render::WorldToScreen(vParent, sParent) && Render::WorldToScreen(vChild, sChild))
			{
				Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 255, 255, 255));
			}
		}
	}
}


void CEsp::DrawGlow()
{
	int GlowR = Menu::Window.GUITab.GlowR.GetValue();
	int GlowG = Menu::Window.GUITab.GlowG.GetValue();
	int GlowB = Menu::Window.GUITab.GlowB.GetValue();
	int GlowZ = Menu::Window.GUITab.GlowZ.GetValue();

	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	for (int i = 0; i < GlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		IClientEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{
		case 35:
			if (Menu::Window.VisualsTab.OptionsGlow.GetState())
			{
				if (!Menu::Window.VisualsTab.FiltersPlayers.GetState() && !(Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
					break;
				if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
					break;

				if (GameUtils::IsVisible(hackManager.pLocal(), Entity, 0))
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(GlowR, GlowG, GlowB, GlowZ) : Color(GlowR, GlowG, GlowB, GlowZ));
				}

				else
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(GlowR, GlowG, GlowB, GlowZ) : Color(GlowR, GlowG, GlowB, GlowZ));
				}
			}
		}
	}
}

void CEsp::EntityGlow()
{
	int GlowR = Menu::Window.GUITab.GlowR.GetValue();
	int GlowG = Menu::Window.GUITab.GlowG.GetValue();
	int GlowB = Menu::Window.GUITab.GlowB.GetValue();
	int GlowZ = Menu::Window.GUITab.GlowZ.GetValue();

	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	for (int i = 0; i < GlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		IClientEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{
		case 1:
			if (Menu::Window.VisualsTab.OptionsGlow.GetState())
			{
				if (Menu::Window.VisualsTab.OptionsGlow.GetState())
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 29:
			if (Menu::Window.VisualsTab.OptionsGlow.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 39:
			if (Menu::Window.VisualsTab.OptionsGlow.GetState())
			{
				if (Menu::Window.VisualsTab.FiltersC4.GetState())
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 41:
			if (Menu::Window.VisualsTab.OptionsGlow.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 66:
			if (Menu::Window.VisualsTab.OptionsGlow.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}

		case 108:
			if (Menu::Window.VisualsTab.FiltersC4.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		default:
			if (Menu::Window.VisualsTab.OptionsGlow.GetState())
			{
				if (strstr(Entity->GetClientClass()->m_pNetworkName, "Weapon"))
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		}
	}
}

void CEsp::Junk2() {
	float pJunkcode = 5111231231231231453;
	pJunkcode = 13477777777789242;
	if (pJunkcode = 171111132425)
		pJunkcode = 183211111111425;
	pJunkcode = 173200000000425;
	pJunkcode = 11117898911;
	if (pJunkcode = 3492511173490534);
	pJunkcode = 2304892193748943;
	pJunkcode = 43900000000453;
	if (pJunkcode = 171389425)
		pJunkcode = 18111138912425;
	pJunkcode = 17324111111895;
	pJunkcode = 43578195346534;
	if (pJunkcode = 3492123473490534);
	pJunkcode = 23048921374118943;
	pJunkcode = 43907513945113;
	if (pJunkcode = 1732189342425)
		pJunkcode = 18309807987987425;
	pJunkcode = 1732423198465;
	pJunkcode = 43222222216534;
	if (pJunkcode = 349098709870474);
	pJunkcode = 23879978798798346;
	pJunkcode = 430987098710453;
}

void CEsp::DrawMolotov(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Fire");

		if (Menu::Window.VisualsTab.FiltersNades.GetState())
			DrawName(pinfo, Box);
	}
}

void CEsp::DrawSmoke(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Smoke");

		if (Menu::Window.VisualsTab.FiltersNades.GetState() == 1)
			DrawName(pinfo, Box);
	}
}

void CEsp::DrawDecoy(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Decoy");

		if (Menu::Window.VisualsTab.FiltersNades.GetState())
			DrawName(pinfo, Box);
	}
}

void CEsp::DrawHE(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "HE or Flash");

		if (Menu::Window.VisualsTab.FiltersNades.GetState())
			DrawName(pinfo, Box);
	}
}
