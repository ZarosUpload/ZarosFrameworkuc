#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"

// Any init here
void CVisuals::Init()
{
	// Idk
}

// Don't really need to do anything in here
void CVisuals::Move(CUserCmd *pCmd, bool &bSendPacket) {}

// Main ESP Drawing loop
void CVisuals::Draw()
{
	NoScopeBorder();

	// Crosshair
	DrawCrosshair();

	// Recoil Crosshair
	DrawRecoilCrosshair();
}

// Draw a basic crosshair
void CVisuals::DrawCrosshair()
{
	if (Menu::Window.VisualsTab.OtherCrosshair.GetIndex() == 1)
	{
		static float rainbow;
		rainbow += 0.005f;
		if (rainbow > 1.f) rainbow = 0.f;

		RECT View = Render::GetViewport();
		int MidX = View.right / 2;
		int MidY = View.bottom / 2;
		Render::Line(MidX - 5, MidY - 5, MidX + 5, MidY + 5, Color::FromHSB(rainbow, 1.f, 1.f));
		Render::Line(MidX + 5, MidY - 5, MidX - 5, MidY + 5, Color::FromHSB(rainbow, 1.f, 1.f));
	}
	else if (Menu::Window.VisualsTab.OtherCrosshair.GetIndex() == 2)
	{
		// Crosshair 2
		RECT View = Render::GetViewport();
		int MidX = View.right / 2;
		int MidY = View.bottom / 2;
		Render::Line(MidX - 5, MidY - 5, MidX + 5, MidY + 5, Color(0, 0, 255, 200));
		Render::Line(MidX + 5, MidY - 5, MidX - 5, MidY + 5, Color(0, 0, 255, 200));
		Render::Line(MidX - 5, MidY - 5, MidX + 5, MidY + 5, Color(0, 0, 255, 255));
		Render::Line(MidX + 5, MidY - 5, MidX - 5, MidY + 5, Color(0, 0, 255, 255));
	}

}

void CVisuals::Junk() {
	float pJunkcode = 5113278119111453;
	pJunkcode = 13411111111189242;
	if (pJunkcode = 171111132425)
		pJunkcode = 183211111111425;
	pJunkcode = 1732111515111425;
	pJunkcode = 11117895558911;
	if (pJunkcode = 34925115173490534);
	pJunkcode = 2304892193748943;
	pJunkcode = 439075153590453;
	if (pJunkcode = 1711153559425)
		pJunkcode = 18111138912425;
	pJunkcode = 17324115111189;
	pJunkcode = 43578194556534;
	if (pJunkcode = 34921123473490534);
	pJunkcode = 230489251374118943;
	pJunkcode = 439075139145113;
	if (pJunkcode = 1732189342425)
		pJunkcode = 1834113254113425;
	pJunkcode = 182321673854372;
	pJunkcode = 435789513416534;
	if (pJunkcode = 34934617349053474);
	pJunkcode = 230489214189143346;
	pJunkcode = 439043557910453;

	Junk2();
}



void CVisuals::Junk2() {
	float pJunkcode = 5113278119111453;
	pJunkcode = 134111111189242;
	if (pJunkcode = 171111132425)
		pJunkcode = 18311111111425;
	pJunkcode = 1732111515111425;
	pJunkcode = 11117895558911;
	if (pJunkcode = 3492515173490534);
	pJunkcode = 2304892193748943;
	pJunkcode = 43907515359453;
	if (pJunkcode = 171115359425)
		pJunkcode = 18111138912425;
	pJunkcode = 173241151111895;
	pJunkcode = 4357819455634;
	if (pJunkcode = 34921123473490534);
	pJunkcode = 23048925137118943;
	pJunkcode = 439075139145113;
	if (pJunkcode = 173218932425)
		pJunkcode = 1834113254113425;
	pJunkcode = 173242355198465;
	pJunkcode = 435789513416534;
	if (pJunkcode = 3493417349053474);
	pJunkcode = 2304892141143346;
	pJunkcode = 4390435579453;

	Junk3();
}

// Recoil crosshair
void CVisuals::DrawRecoilCrosshair()
{
	if (Menu::Window.VisualsTab.OtherRecoilCrosshair.GetIndex() == 0)
	{
		// No recoil crosshair selected in menu.
	}

	else if (Menu::Window.VisualsTab.OtherRecoilCrosshair.GetIndex() == 2)
	{
		// Recoil Crosshair 1

		IClientEntity *pLocal = hackManager.pLocal();

		// Get the view with the recoil
		Vector ViewAngles;
		Interfaces::Engine->GetViewAngles(ViewAngles);
		ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

		// Build a ray going fowards at that angle
		Vector fowardVec;
		AngleVectors(ViewAngles, &fowardVec);
		fowardVec *= 10000;

		// Get ray start / end
		Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
		Vector end = start + fowardVec, endScreen;

		if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
		{
			Render::Line(endScreen.x - 4, endScreen.y - 4, endScreen.x + 4, endScreen.y + 4, Color(0, 0, 255, 255));
			Render::Line(endScreen.x + 4, endScreen.y - 4, endScreen.x - 4, endScreen.y + 4, Color(0, 0, 255, 255));
			Render::Line(endScreen.x - 2, endScreen.y - 2, endScreen.x + 2, endScreen.y + 2, Color(0, 0, 20, 200));
			Render::Line(endScreen.x + 2, endScreen.y - 2, endScreen.x - 2, endScreen.y + 2, Color(0, 0, 20, 200));
		}
	}
	else if (Menu::Window.VisualsTab.OtherRecoilCrosshair.GetIndex() == 1)
	{
		// Recoil Crosshair 1

		IClientEntity *pLocal = hackManager.pLocal();

		// Get the view with the recoil
		Vector ViewAngles;
		Interfaces::Engine->GetViewAngles(ViewAngles);
		ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

		// Build a ray going fowards at that angle
		Vector fowardVec;
		AngleVectors(ViewAngles, &fowardVec);
		fowardVec *= 10000;

		// Get ray start / end
		Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
		Vector end = start + fowardVec, endScreen;

		//Rainbow

		static float rainbow;
		rainbow += 0.005f;
		if (rainbow > 1.f) rainbow = 0.f;

		RECT View = Render::GetViewport();
		int MidX = View.right / 2;
		int MidY = View.bottom / 2;

		if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
		{

			Render::Line(MidX - 5, MidY - 5, MidX + 5, MidY + 5, Color::FromHSB(rainbow, 1.f, 1.f));
			Render::Line(MidX + 5, MidY - 5, MidX - 5, MidY + 5, Color::FromHSB(rainbow, 1.f, 1.f));
		}
	}
}


void CVisuals::Junk3() {
	float pJunkcode = 5113278119199999;
	pJunkcode = 134111111189242;
	if (pJunkcode = 17111132425)
		pJunkcode = 18311111111425;
	pJunkcode = 173211515111425;
	pJunkcode = 11117895558911;
	if (pJunkcode = 3925115173490534);
	pJunkcode = 230492193748943;
	pJunkcode = 4375153590453;
	if (pJunkcode = 711153559425)
		pJunkcode = 18111138912425;
	pJunkcode = 17341151111895;
	pJunkcode = 4357194556534;
	if (pJunkcode = 567456745674566);
	pJunkcode = 230489251374118333;
	pJunkcode = 43905139145222;
	if (pJunkcode = 1732189342425)
		pJunkcode = 1834113254113425;
	pJunkcode = 17322355198465;
	pJunkcode = 435789513416534;
	if (pJunkcode = 3934617349053474);
	pJunkcode = 2309214189143346;
	pJunkcode = 439043557910453;
}

void CVisuals::NoScopeBorder()
{
	if (Menu::Window.VisualsTab.OtherNoScope.GetState())
	{
		if (hackManager.pLocal()->IsScoped() && hackManager.pLocal()->IsAlive())
		{
			int Width;
			int Height;
			Interfaces::Engine->GetScreenSize(Width, Height);

			Color cColor = Color(87, 75, 172, 255);
			Render::Line(Width / 2, 0, Width / 2, Height, cColor);
			Render::Line(0, Height / 2, Width, Height / 2, cColor);


		}
		else
		{

		}
	}
}

