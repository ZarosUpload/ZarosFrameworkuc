#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" 
#include "Interfaces.h"
#include "CRC32.h"

//test1337 1

// Menu Window Size
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 557

AyyWareWindow Menu::Window;

// Save Config to CSGO directory
void SaveCallbk()
{
	switch (Menu::Window.ConfigBox.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Menu::Window, "WHLegit1.cfg");
		break;
	case 1:
		GUI.SaveWindowState(&Menu::Window, "WHLegit2.cfg");
		break;
	case 2:
		GUI.SaveWindowState(&Menu::Window, "WHRage1.cfg");
		break;
	case 3:
		GUI.SaveWindowState(&Menu::Window, "WHRage2.cfg");
		break;
	case 4:
		GUI.SaveWindowState(&Menu::Window, "WHHvH1.cfg");
		break;
	case 5:
		GUI.SaveWindowState(&Menu::Window, "WHHvH2.cfg");
		break;
	case 6:
		GUI.SaveWindowState(&Menu::Window, "WHOther.cfg");
		break;
	}
}

// Load Config from CSGO directory
void LoadCallbk()
{
	switch (Menu::Window.ConfigBox.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Menu::Window, "WHLegit1.cfg");
		break;
	case 1:
		GUI.LoadWindowState(&Menu::Window, "WHLegit2.cfg");
		break;
	case 2:
		GUI.LoadWindowState(&Menu::Window, "WHRage1.cfg");
		break;
	case 3:
		GUI.LoadWindowState(&Menu::Window, "WHRage2.cfg");
		break;
	case 4:
		GUI.LoadWindowState(&Menu::Window, "WHHvH1.cfg");
		break;
	case 5:
		GUI.LoadWindowState(&Menu::Window, "WHHvH2.cfg");
		break;
	case 6:
		GUI.LoadWindowState(&Menu::Window, "WHOther.cfg");
		break;
	}
}

// Unload Function
void UnLoadCallbk()
{
	DoUnload = true;
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
}

void GlovesApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
	bGlovesNeedUpdate = true;
}

void AyyWareWindow::Setup()
{
	SetPosition(150, 85);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	RegisterTab(&RageBotTab);
	
	RegisterTab(&VisualsTab);
	RegisterTab(&SkinTab);
	RegisterTab(&MiscTab);
	RegisterTab(&GUITab);

	RECT Client = GetClientArea();
	Client.bottom -= 29;

	RageBotTab.Setup();
	
	VisualsTab.Setup();
	SkinTab.Setup();
	MiscTab.Setup();
	GUITab.Setup();


#pragma region Bottom Buttons

	ConfigBox.SetFileId("cfg_box");
	ConfigBox.AddItem("Legit 1");
	ConfigBox.AddItem("Legit 2");
	ConfigBox.AddItem("Rage 1");
	ConfigBox.AddItem("Rage 2");
	ConfigBox.AddItem("HvH 1");
	ConfigBox.AddItem("HvH 2");
	ConfigBox.AddItem("Other");
	ConfigBox.SetSize(112, 350);
	ConfigBox.SetPosition(405, Client.bottom - 200);

	SaveButton.SetText("Save");
	SaveButton.SetCallback(SaveCallbk);
	SaveButton.SetSize(112, 350);
	SaveButton.SetPosition(285, Client.bottom - 170);

	LoadButton.SetText("Load");
	LoadButton.SetCallback(LoadCallbk);
	LoadButton.SetSize(112, 350);
	LoadButton.SetPosition(285, Client.bottom - 200);

	MiscTab.RegisterControl(&SaveButton);
	MiscTab.RegisterControl(&LoadButton);

	MiscTab.RegisterControl(&ConfigBox);

#pragma endregion Setting up the settings buttons
}

void CRageBotTab::Setup()
{
	SetTitle("A");

	ActiveLabel.SetPosition(15, 5);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(65, 5);
	RegisterControl(&Active);

#pragma region Aimbot

	AimbotGroup.SetPosition(20, 20);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(376, 270);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.SetValue(39.f);
	AimbotGroup.PlaceLabledControl("FOV Range", this, &AimbotFov);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl("Silent Aim", this, &AimbotSilentAim);

	AWPAtBody.SetFileId("aim_awpatbody");
	AimbotGroup.PlaceLabledControl("AWP at Body", this, &AWPAtBody);

	AutoRevolver.SetFileId("aim_autorevolver");
	AimbotGroup.PlaceLabledControl("Auto Revolver", this, &AutoRevolver);

	AutoPistol.SetFileId("aim_autopistol");
	AimbotGroup.PlaceLabledControl("Auto Pistol", this, &AutoPistol);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key Press", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key", this, &AimbotKeyBind);

	AimbotStopKey.SetFileId("aim_stop");
	AimbotGroup.PlaceLabledControl("Stop Aim", this, &AimbotStopKey);
#pragma endregion Aimbot Controls Get Setup in here

#pragma region Accuracy
	AccuracyGroup.SetPosition(20, 297);
	AccuracyGroup.SetText("Accuracy");
	AccuracyGroup.SetSize(376, 310);
	RegisterControl(&AccuracyGroup);

	AccuracyRecoil.SetFileId("acc_norecoil");
	AccuracyGroup.PlaceLabledControl("Remove Recoil", this, &AccuracyRecoil);

	WeaponCheck.SetFileId("Indv_Hitchance");
	AccuracyGroup.PlaceLabledControl("Weaponcheck", this, &WeaponCheck);

	AccuracyAutoWall.SetFileId("acc_awall");
	AccuracyGroup.PlaceLabledControl("Auto Wall", this, &AccuracyAutoWall);

	AccuracyMinimumDamage.SetFileId("acc_mindmg");
	AccuracyMinimumDamage.SetBoundaries(1.f, 99.f);
	AccuracyMinimumDamage.SetValue(10.f);
	AccuracyGroup.PlaceLabledControl("Autowall Damage", this, &AccuracyMinimumDamage);

	AccuracyAutoScope.SetFileId("acc_scope");
	AccuracyGroup.PlaceLabledControl("Auto Scope", this, &AccuracyAutoScope);

	LowerbodyFix.SetFileId("acc_lowerbodyfix");
	AccuracyGroup.PlaceLabledControl("LBY Correction", this, &LowerbodyFix);

	AimbotResolver.SetFileId("acc_aaa");
	AimbotResolver.AddItem("Off");
	AimbotResolver.AddItem("LBY");
	AimbotResolver.AddItem("Bruteforce");
	AimbotResolver.AddItem("Delta");
	AccuracyGroup.PlaceLabledControl("Resolver", this, &AimbotResolver);

	AccuracyHitchance.SetFileId("acc_chance");
	AccuracyHitchance.SetBoundaries(0, 100);
	AccuracyHitchance.SetValue(0);
	AccuracyGroup.PlaceLabledControl("Hit Chance", this, &AccuracyHitchance);

	BaimIfUnderXHealth.SetFileId("acc_BaimIfUnderXHealth");
	BaimIfUnderXHealth.SetBoundaries(0, 100);
	BaimIfUnderXHealth.SetValue(0);
	AccuracyGroup.PlaceLabledControl("Body-Aim if HP is lower than X", this, &BaimIfUnderXHealth);

	Accuracylagcomp.SetFileId("acc_lagcomp");
	AccuracyGroup.PlaceLabledControl("Lag Compensation", this, &Accuracylagcomp);

	Accuracydisableinterp.SetFileId("acc_disableinterp");
	AccuracyGroup.PlaceLabledControl("Disable Interpolation", this, &Accuracydisableinterp);


#pragma endregion Accuracy controls 

#pragma region Target
	TargetGroup.SetPosition(406, 20);
	TargetGroup.SetText("Target");
	TargetGroup.SetSize(360, 270);
	RegisterControl(&TargetGroup);

	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem("Closest To Crosshair");
	TargetSelection.AddItem("Distance");
	TargetSelection.AddItem("Lowest Health");
	TargetSelection.AddItem("Threat");
	TargetSelection.AddItem("Next Shot");
	TargetGroup.PlaceLabledControl("Selection", this, &TargetSelection);

	TargetFriendlyFire.SetFileId("tgt_friendlyfire");
	TargetGroup.PlaceLabledControl("Friendly Fire", this, &TargetFriendlyFire);

	TargetHitbox.SetFileId("tgt_hitbox");
	TargetHitbox.AddItem("Head");
	TargetHitbox.AddItem("Neck");
	TargetHitbox.AddItem("Chest");
	TargetHitbox.AddItem("Stomach");
	TargetHitbox.AddItem("Foots");
	TargetGroup.PlaceLabledControl("Hitbox", this, &TargetHitbox);

	TargetHitscan.SetFileId("tgt_hitscan");
	TargetHitscan.AddItem("Off");
	TargetHitscan.AddItem("Low");
	TargetHitscan.AddItem("Medium");
	TargetHitscan.AddItem("High");
	TargetGroup.PlaceLabledControl("Hitscan", this, &TargetHitscan);

	PVSFix.SetFileId("tgt_pvsfix");
	TargetGroup.PlaceLabledControl("PVS Fix", this, &PVSFix);

	EnginePrediction.SetFileId("tgt_enginepred");
	TargetGroup.PlaceLabledControl("Engine Prediction (!)", this, &EnginePrediction);

	TargetMultipoint.SetFileId("tgt_multipoint");
	TargetGroup.PlaceLabledControl("Multipoint", this, &TargetMultipoint);

	TargetPointscale.SetFileId("tgt_pointscale");
	TargetPointscale.SetBoundaries(0.f, 10.f);
	TargetPointscale.SetValue(8.f);
	TargetGroup.PlaceLabledControl("Pointscale", this, &TargetPointscale);



#pragma endregion  Target controls get Setup in here

#pragma region AntiAim
	AntiAimGroup.SetPosition(406, 297);
	AntiAimGroup.SetText("Anti Aim");
	AntiAimGroup.SetSize(360, 310);
	RegisterControl(&AntiAimGroup);

	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl("Enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("Off");
	AntiAimPitch.AddItem("Halfdown");
	AntiAimPitch.AddItem("Jitter");
	AntiAimPitch.AddItem("Emotion");
	AntiAimPitch.AddItem("Up");
	AntiAimPitch.AddItem("Zero");
	AntiAimPitch.AddItem("Custom");
	AntiAimGroup.PlaceLabledControl("Pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem("Off");
	AntiAimYaw.AddItem("Fast Spin");
	AntiAimYaw.AddItem("Slow Spin");
	AntiAimYaw.AddItem("Jitter");
	AntiAimYaw.AddItem("180 Jitter");
	AntiAimYaw.AddItem("Backwards");
	AntiAimYaw.AddItem("Backwards Jitter");
	AntiAimYaw.AddItem("Sideways Right");
	AntiAimYaw.AddItem("Sideways Left");
	AntiAimYaw.AddItem("LBY");
	AntiAimYaw.AddItem("LBY Jitter");
	AntiAimYaw.AddItem("LBY Sideways");
	AntiAimYaw.AddItem("LBY Spin");
	AntiAimYaw.AddItem("LBY Break");
	AntiAimGroup.PlaceLabledControl("Real Yaw", this, &AntiAimYaw);

	FakeYaw.SetFileId("fakeaa");
	FakeYaw.AddItem("Off");
	FakeYaw.AddItem("Fast Spin");
	FakeYaw.AddItem("Slow Spin");
	FakeYaw.AddItem("Jitter");
	FakeYaw.AddItem("180 Jitter");
	FakeYaw.AddItem("Backwards");
	FakeYaw.AddItem("Backwards Jitter");
	FakeYaw.AddItem("Sideways Right");
	FakeYaw.AddItem("Sideways Left");
	FakeYaw.AddItem("LBY");
	FakeYaw.AddItem("LBY Jitter");
	FakeYaw.AddItem("LBY Sideways");
	FakeYaw.AddItem("LBY Spin");
	FakeYaw.AddItem("LBY Break");
	AntiAimGroup.PlaceLabledControl("Fake Yaw", this, &FakeYaw);

	CustomPitch.SetFileId("aa_offset");
	CustomPitch.SetBoundaries(0, 89);
	CustomPitch.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Custom Pitch", this, &CustomPitch);

	AntiAimOffset.SetFileId("aa_offset");
	AntiAimOffset.SetBoundaries(0, 360);
	AntiAimOffset.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Add Real Yaw", this, &AntiAimOffset);

	AddFakeYaw.SetFileId("aa_offset");
	AddFakeYaw.SetBoundaries(0, 360);
	AddFakeYaw.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Add Fake Yaw", this, &AddFakeYaw);

	LBY.SetFileId("aa_lby");
	AntiAimGroup.PlaceLabledControl("Anti Resolver", this, &LBY);

	FlipAA.SetFileId("aa_flip");
	AntiAimGroup.PlaceLabledControl("Flip AA on Hit", this, &FlipAA);

	AntiAimEdge.SetFileId("aa_edge");
	AntiAimGroup.PlaceLabledControl("Wall Detection", this, &AntiAimEdge);

	AntiAimKnife.SetFileId("aa_knife");
	AntiAimGroup.PlaceLabledControl("Knife/Nade Anti Aim", this, &AntiAimKnife);

	AntiAimTarget.SetFileId("aa_target");
	AntiAimGroup.PlaceLabledControl("At Targets", this, &AntiAimTarget);
#pragma endregion  AntiAim controls get setup in here

#pragma region Hitchances
	wpnhitchanceGroup.SetPosition(775, 20);
	wpnhitchanceGroup.SetSize(360, 190);
	wpnhitchanceGroup.SetText("Weapon Hitchances");
	RegisterControl(&wpnhitchanceGroup);

	scoutChance.SetFileId("scout_chance");
	scoutChance.SetBoundaries(0, 100);
	scoutChance.SetValue(0);
	wpnhitchanceGroup.PlaceLabledControl("Scout", this, &scoutChance);

	AWPChance.SetFileId("awp_chance");
	AWPChance.SetBoundaries(0, 100);
	AWPChance.SetValue(0);
	wpnhitchanceGroup.PlaceLabledControl("AWP", this, &AWPChance);

	AutoChance.SetFileId("auto_chance");
	AutoChance.SetBoundaries(0, 100);
	AutoChance.SetValue(0);
	wpnhitchanceGroup.PlaceLabledControl("Auto", this, &AutoChance);

	RifleChance.SetFileId("rifle_chance");
	RifleChance.SetBoundaries(0, 100);
	RifleChance.SetValue(0);
	wpnhitchanceGroup.PlaceLabledControl("Rifle", this, &RifleChance);

	MPChance.SetFileId("mp_chance");
	MPChance.SetBoundaries(0, 100);
	MPChance.SetValue(0);
	wpnhitchanceGroup.PlaceLabledControl("MPS", this, &MPChance);

	PistolChance.SetFileId("pistol_chance");
	PistolChance.SetBoundaries(0, 100);
	PistolChance.SetValue(0);
	wpnhitchanceGroup.PlaceLabledControl("Pistols", this, &PistolChance);

	R8Chance.SetFileId("R8_chance");
	R8Chance.SetBoundaries(0, 100);
	R8Chance.SetValue(0);
	wpnhitchanceGroup.PlaceLabledControl("Revolver", this, &R8Chance);

	wpnmindmgGroup.SetPosition(775, 220);
	wpnmindmgGroup.SetSize(360, 190);
	wpnmindmgGroup.SetText("Weapon Min Damage");
	RegisterControl(&wpnmindmgGroup);

	scoutmindmg.SetFileId("scout_mindmg");
	scoutmindmg.SetBoundaries(0, 100);
	scoutmindmg.SetValue(0);
	wpnmindmgGroup.PlaceLabledControl("Scout", this, &scoutmindmg);

	AWPmindmg.SetFileId("awp_mindmg");
	AWPmindmg.SetBoundaries(0, 100);
	AWPmindmg.SetValue(0);
	wpnmindmgGroup.PlaceLabledControl("AWP", this, &AWPmindmg);

	Automindmg.SetFileId("auto_mindmg");
	Automindmg.SetBoundaries(0, 100);
	Automindmg.SetValue(0);
	wpnmindmgGroup.PlaceLabledControl("Auto", this, &Automindmg);

	Riflemindmg.SetFileId("rifle_mindmg");
	Riflemindmg.SetBoundaries(0, 100);
	Riflemindmg.SetValue(0);
	wpnmindmgGroup.PlaceLabledControl("Rifle", this, &Riflemindmg);

	MPmindmg.SetFileId("mp_mindmg");
	MPmindmg.SetBoundaries(0, 100);
	MPmindmg.SetValue(0);
	wpnmindmgGroup.PlaceLabledControl("MPS", this, &MPmindmg);

	Pistolmindmg.SetFileId("pistol_mindmg");
	Pistolmindmg.SetBoundaries(0, 100);
	Pistolmindmg.SetValue(0);
	wpnmindmgGroup.PlaceLabledControl("Pistols", this, &Pistolmindmg);

	R8mindmg.SetFileId("R8_mindmg");
	R8mindmg.SetBoundaries(0, 100);
	R8mindmg.SetValue(0);
	wpnmindmgGroup.PlaceLabledControl("Revolver", this, &R8mindmg);
#pragma endregion Hitchances
}



void CVisualTab::Setup()
{
	SetTitle("D");

	ActiveLabel.SetPosition(10, 8);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(55, 8);
	RegisterControl(&Active);

#pragma region Options
	OptionsGroup.SetText("Options");
	OptionsGroup.SetPosition(10, 35);
	OptionsGroup.SetSize(185, 430);
	RegisterControl(&OptionsGroup);

	OptionsBox.SetFileId("opt_box");
	OptionsGroup.PlaceLabledControl("Box", this, &OptionsBox);

	BoxDesign.SetFileId("opt_boxtype");
	BoxDesign.AddItem("Corner");
	BoxDesign.AddItem("Full");
	BoxDesign.AddItem("Rainbow");
	OptionsGroup.PlaceLabledControl("Box Type", this, &BoxDesign);

	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem("Off");
	OptionsChams.AddItem("Normal");
	OptionsChams.AddItem("Flat");
	OptionsGroup.PlaceLabledControl("Chams", this, &OptionsChams);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl("Name", this, &OptionsName);

	OptionHealthEnable.SetFileId("opt_health");
	OptionsGroup.PlaceLabledControl("Health", this, &OptionHealthEnable);

	OptionsHealth.SetFileId("opt_hp");
	OptionsHealth.AddItem("Simple");
	OptionsHealth.AddItem("Advanced");
	OptionsGroup.PlaceLabledControl("Health", this, &OptionsHealth);

	OptionsArmur.SetFileId("opt_armor");
	OptionsGroup.PlaceLabledControl("Armor", this, &OptionsArmur);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsGroup.PlaceLabledControl("Weapon", this, &OptionsWeapon);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl("Skeleton", this, &OptionsSkeleton);

	OptionsGlow.SetFileId("opt_glow");
	OptionsGroup.PlaceLabledControl("Glow", this, &OptionsGlow);

	OptionsAimSpot.SetFileId("opt_aimspot");
	OptionsGroup.PlaceLabledControl("Head Cross", this, &OptionsAimSpot);

	OptionsFill.SetFileId("opt_fill");
	OptionsGroup.PlaceLabledControl("Fill Box", this, &OptionsFill);

	LBY.SetFileId("opt_LBY");
	OptionsGroup.PlaceLabledControl("LBY", this, &LBY);

#pragma endregion Setting up the Options controls

#pragma region Filters
	FiltersGroup.SetText("Filters");
	FiltersGroup.SetPosition(205, 35);
	FiltersGroup.SetSize(150, 350);
	RegisterControl(&FiltersGroup);

	FiltersAll.SetFileId("ftr_all");
	FiltersGroup.PlaceLabledControl("All", this, &FiltersAll);

	FiltersPlayers.SetFileId("ftr_players");
	FiltersGroup.PlaceLabledControl("Players", this, &FiltersPlayers);

	FiltersEnemiesOnly.SetFileId("ftr_enemyonly");
	FiltersGroup.PlaceLabledControl("Enemies Only", this, &FiltersEnemiesOnly);

	FiltersWeapons.SetFileId("ftr_weaps");
	FiltersGroup.PlaceLabledControl("Weapons", this, &FiltersWeapons);

	FiltersC4.SetFileId("ftr_c4");
	FiltersGroup.PlaceLabledControl("C4", this, &FiltersC4);

	FiltersNades.SetFileId("ftr_Nades");
	FiltersGroup.PlaceLabledControl("Nades", this, &FiltersNades);

#pragma endregion Setting up the Filters controls

#pragma region Other
	OtherGroup.SetText("Other");
	OtherGroup.SetPosition(365, 35);
	OtherGroup.SetSize(290, 435);
	RegisterControl(&OtherGroup);

	OtherCrosshair.SetFileId("otr_xhair");
	OtherCrosshair.AddItem("Off");
	OtherCrosshair.AddItem("Rainbow X");
	OtherCrosshair.AddItem("Normal");
	OtherGroup.PlaceLabledControl("Crosshair", this, &OtherCrosshair);

	OtherRecoilCrosshair.SetFileId("otr_recoilhair");
	OtherRecoilCrosshair.AddItem("Off");
	OtherRecoilCrosshair.AddItem("Rainbow");
	OtherRecoilCrosshair.AddItem("Normal");
	OtherGroup.PlaceLabledControl("Recoil Crosshair", this, &OtherRecoilCrosshair);

	OtherRadar.SetFileId("otr_radar");
	OtherGroup.PlaceLabledControl("Radar", this, &OtherRadar);

	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	OtherGroup.PlaceLabledControl("No Visual Recoil", this, &OtherNoVisualRecoil);

	OtherNoFlash.SetFileId("otr_noflash");
	OtherGroup.PlaceLabledControl("No Flash", this, &OtherNoFlash);

	OtherNoFlashAlpha.SetFileId("otr_NoFlashAlpha");
	OtherNoFlashAlpha.SetBoundaries(0.f, 255.f);
	OtherNoFlashAlpha.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Flash Alpha", this, &OtherNoFlashAlpha);

	OtherNoSmoke.SetFileId("otr_nosmoke");
	OtherGroup.PlaceLabledControl("No Smoke", this, &OtherNoSmoke);

	OtherNoScope.SetFileId("otr_noscope");
	OtherGroup.PlaceLabledControl("Noscope", this, &OtherNoScope);

	OtherThirdperson.SetFileId("aa_thirdpsr");
	OtherGroup.PlaceLabledControl("Thirdperson", this, &OtherThirdperson);

	OtherThirdpersonRange.SetFileId("otr_thirdpersonfov");
	OtherThirdpersonRange.SetBoundaries(0.f, 360.f);
	OtherThirdpersonRange.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Thirdperson Range", this, &OtherThirdpersonRange);

	OtherThirdpersonKey.SetFileId("aa_thirdpsrkey");
	OtherGroup.PlaceLabledControl("Thirdperson Key", this, &OtherThirdpersonKey);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 130.f);
	OtherFOV.SetValue(90.f);
	OtherGroup.PlaceLabledControl("FOV", this, &OtherFOV);

	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(0.f, 200.f);
	OtherViewmodelFOV.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Viewmodel FOV", this, &OtherViewmodelFOV);

	Clock.SetFileId("Clock");
	OtherGroup.PlaceLabledControl("Clock", this, &Clock);

	OptionsCompRank.SetFileId("opt_comprank");
	OptionsGroup.PlaceLabledControl("Tab Rank Reveal", this, &OptionsCompRank);

	OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceLabledControl("Info", this, &OptionsInfo);

	IsScoped.SetFileId("opt_isscoped");
	OptionsGroup.PlaceLabledControl("Is Scoped", this, &IsScoped);

	HasDefuser.SetFileId("opt_hasdefuser");
	OptionsGroup.PlaceLabledControl("Has Defuser", this, &HasDefuser);

	GrenadeTrace.SetFileId("otr_grenadetrace");
	OtherGroup.PlaceLabledControl("Grenade Trajectory", this, &GrenadeTrace);

	DisablePostProcess.SetFileId("otr_disablepostprocess");
	OtherGroup.PlaceLabledControl("Disable Post Process", this, &DisablePostProcess);
#pragma endregion

}

void CSkinTab::Setup()
{
	SetTitle("B");

	SkinActive.SetPosition(15, 5);
	SkinActive.SetText("Active");
	RegisterControl(&SkinActive);

	SkinEnable.SetFileId("skin_enable");
	SkinEnable.SetPosition(65, 5);
	RegisterControl(&SkinEnable);

#pragma region Knife
	KnifeGroup.SetPosition(16, 21);
	KnifeGroup.SetText("Knife");
	KnifeGroup.SetSize(376, 80);
	RegisterControl(&KnifeGroup);

	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem("Bayonet");
	KnifeModel.AddItem("Bowie Knife");
	KnifeModel.AddItem("Butterfly Knife");
	KnifeModel.AddItem("Falchion Knife");
	KnifeModel.AddItem("Flip Knife");
	KnifeModel.AddItem("Gut Knife");
	KnifeModel.AddItem("Huntsman Knife");
	KnifeModel.AddItem("Karambit");
	KnifeModel.AddItem("M9 Bayonet");
	KnifeModel.AddItem("Shadow Daggers");
	KnifeGroup.PlaceLabledControl("Knife", this, &KnifeModel);

	KnifeSkin.SetFileId("knife_skin");
	KnifeGroup.PlaceLabledControl("Skin ID", this, &KnifeSkin);
#pragma endregion

#pragma region Machineguns
	MachinegunsGroup.SetPosition(408, 21);
	MachinegunsGroup.SetText("Heavy");
	MachinegunsGroup.SetSize(360, 80);
	RegisterControl(&MachinegunsGroup);

	NEGEVSkin.SetFileId("negev_skin");
	NEGEVSkin.AddItem("Anodized Navy");
	NEGEVSkin.AddItem("Man-o'-war");
	NEGEVSkin.AddItem("Bratatat");
	NEGEVSkin.AddItem("Desert-Strike");
	NEGEVSkin.AddItem("Nuclear Waste");
	NEGEVSkin.AddItem("Loudmouth");
	NEGEVSkin.AddItem("Power Loader");
	MachinegunsGroup.PlaceLabledControl("Negev", this, &NEGEVSkin);

	M249Skin.SetFileId("m249_skin");
	M249Skin.AddItem("System Lock");
	M249Skin.AddItem("Shipping Forecast");
	M249Skin.AddItem("Impact Drill");
	M249Skin.AddItem("Nebula Crusader");
	M249Skin.AddItem("Spectre");
	MachinegunsGroup.PlaceLabledControl("M249", this, &M249Skin);

#pragma endregion

#pragma region Snipers
	Snipergroup.SetPosition(16, 108);
	Snipergroup.SetText("Snipers");
	Snipergroup.SetSize(376, 125);
	RegisterControl(&Snipergroup);

	AWPSkin.SetFileId("awp_skin");
	AWPSkin.AddItem("BOOM");
	AWPSkin.AddItem("Dragon Lore");
	AWPSkin.AddItem("Pink DDPAT");
	AWPSkin.AddItem("Fever Dream");
	AWPSkin.AddItem("Lightning Strike");
	AWPSkin.AddItem("Corticera");
	AWPSkin.AddItem("Redline");
	AWPSkin.AddItem("Man-o'-war");
	AWPSkin.AddItem("Graphite");
	AWPSkin.AddItem("Electric Hive");
	AWPSkin.AddItem("Pit Viper");
	AWPSkin.AddItem("Asiimov");
	AWPSkin.AddItem("Oni Taiji");
	AWPSkin.AddItem("Medusa");
	AWPSkin.AddItem("Sun in Leo");
	AWPSkin.AddItem("Hyper Beast");
	AWPSkin.AddItem("Elite Build");
	Snipergroup.PlaceLabledControl("AWP", this, &AWPSkin);

	SSG08Skin.SetFileId("sgg08_skin");
	SSG08Skin.AddItem("Dragonfire");
	SSG08Skin.AddItem("Blood in the Water");
	SSG08Skin.AddItem("Ghost Crusader");
	SSG08Skin.AddItem("Detour");
	SSG08Skin.AddItem("Abyss");
	SSG08Skin.AddItem("Big Iron");
	Snipergroup.PlaceLabledControl("SGG 08", this, &SSG08Skin);

	SCAR20Skin.SetFileId("scar20_skin");
	SCAR20Skin.AddItem("Splash Jam");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Crimson Web");
	SCAR20Skin.AddItem("Cardiac");
	SCAR20Skin.AddItem("Bloodsport");
	SCAR20Skin.AddItem("Cyrex");
	SCAR20Skin.AddItem("Grotto");
	Snipergroup.PlaceLabledControl("SCAR-20", this, &SCAR20Skin);

	G3SG1Skin.SetFileId("g3sg1_skin");
	G3SG1Skin.AddItem("Demeter");
	G3SG1Skin.AddItem("Azure Zebra");
	G3SG1Skin.AddItem("Green Apple");
	G3SG1Skin.AddItem("Orange Kimono");
	G3SG1Skin.AddItem("Neon Kimono");
	G3SG1Skin.AddItem("Murky");
	G3SG1Skin.AddItem("Chronos");
	G3SG1Skin.AddItem("Flux");
	G3SG1Skin.AddItem("The Executioner");
	Snipergroup.PlaceLabledControl("G3SG1", this, &G3SG1Skin);
#pragma endregion

#pragma region Shotguns
	Shotgungroup.SetPosition(408, 108);
	Shotgungroup.SetText("Shotguns");
	Shotgungroup.SetSize(360, 125);
	RegisterControl(&Shotgungroup);

	MAG7Skin.SetFileId("mag7_skin");
	MAG7Skin.AddItem("Counter Terrace");
	MAG7Skin.AddItem("Bulldozer");
	MAG7Skin.AddItem("Heat");
	Shotgungroup.PlaceLabledControl("Mag-7", this, &MAG7Skin);

	XM1014Skin.SetFileId("xm1014_skin");
	XM1014Skin.AddItem("Tranquality");
	XM1014Skin.AddItem("Teclu Burner");
	XM1014Skin.AddItem("Scumbria");
	XM1014Skin.AddItem("Heaven Guard");
	XM1014Skin.AddItem("Quicksilver");
	Shotgungroup.PlaceLabledControl("XM1014", this, &XM1014Skin);

	SAWEDOFFSkin.SetFileId("sawedoff_skin");
	SAWEDOFFSkin.AddItem("Serenity");
	SAWEDOFFSkin.AddItem("Orange DDPAT");
	SAWEDOFFSkin.AddItem("Fade");
	SAWEDOFFSkin.AddItem("The Kraken");
	SAWEDOFFSkin.AddItem("Wasteland Princess");
	SAWEDOFFSkin.AddItem("Highwayman");
	Shotgungroup.PlaceLabledControl("Sawed-Off", this, &SAWEDOFFSkin);

	NOVASkin.SetFileId("nova_skin");
	NOVASkin.AddItem("Candy Apple");
	NOVASkin.AddItem("Blaze Orange");
	NOVASkin.AddItem("Modern Hunter");
	NOVASkin.AddItem("Forest Leaves");
	NOVASkin.AddItem("Bloomstick");
	NOVASkin.AddItem("Sand Dune");
	NOVASkin.AddItem("Polar Mesh");
	NOVASkin.AddItem("Walnut");
	NOVASkin.AddItem("Predator");
	NOVASkin.AddItem("Tempest");
	NOVASkin.AddItem("Graphite");
	NOVASkin.AddItem("Ghost Camo");
	NOVASkin.AddItem("Rising Skull");
	NOVASkin.AddItem("Antique");
	NOVASkin.AddItem("Green Apple");
	NOVASkin.AddItem("Caged Steel");
	NOVASkin.AddItem("Koi");
	NOVASkin.AddItem("Moon in Libra");
	NOVASkin.AddItem("Ranger");
	NOVASkin.AddItem("HyperBeast");
	Shotgungroup.PlaceLabledControl("Nova", this, &NOVASkin);
#pragma endregion

#pragma region Rifles
	Riflegroup.SetPosition(16, 243);
	Riflegroup.SetText("Rifles");
	Riflegroup.SetSize(376, 200);
	RegisterControl(&Riflegroup);

	AK47Skin.SetFileId("ak47_skin");
	AK47Skin.AddItem("First Class");
	AK47Skin.AddItem("Red Laminate");
	AK47Skin.AddItem("Case Hardened");
	AK47Skin.AddItem("Black Laminate");
	AK47Skin.AddItem("Fire Serpent");
	AK47Skin.AddItem("Cartel");
	AK47Skin.AddItem("Emerald Pinstripe");
	AK47Skin.AddItem("Blue Laminate");
	AK47Skin.AddItem("Redline");
	AK47Skin.AddItem("Vulcan");
	AK47Skin.AddItem("Jaguar");
	AK47Skin.AddItem("Jet Set");
	AK47Skin.AddItem("Wasteland Rebel");
	AK47Skin.AddItem("Orbit Mk01");
	AK47Skin.AddItem("Hydroponic");
	AK47Skin.AddItem("Aquamarine Revenge");
	AK47Skin.AddItem("Frontside Misty");
	AK47Skin.AddItem("Point Disarray");
	AK47Skin.AddItem("Fuel Injector");
	AK47Skin.AddItem("Neon Revolution");
	AK47Skin.AddItem("Bloodsport");
	Riflegroup.PlaceLabledControl("AK-47", this, &AK47Skin);

	M41SSkin.SetFileId("m4a1s_skin");
	M41SSkin.AddItem("Dark Water");
	M41SSkin.AddItem("Hyper Beast");
	M41SSkin.AddItem("Decimator");
	M41SSkin.AddItem("VariCamo");
	M41SSkin.AddItem("Nitro");
	M41SSkin.AddItem("Bright Water");
	M41SSkin.AddItem("Atomic Alloy");
	M41SSkin.AddItem("Blood Tiger");
	M41SSkin.AddItem("Guardian");
	M41SSkin.AddItem("Master Piece");
	M41SSkin.AddItem("Knight");
	M41SSkin.AddItem("Cyrex");
	M41SSkin.AddItem("Basilisk");
	M41SSkin.AddItem("Icarus Fell");
	M41SSkin.AddItem("Hot Rod");
	M41SSkin.AddItem("Golden Coi");
	M41SSkin.AddItem("Chantico's Fire");
	M41SSkin.AddItem("Mecha Industries");
	M41SSkin.AddItem("Flashback");
	Riflegroup.PlaceLabledControl("M4A1-S", this, &M41SSkin);

	M4A4Skin.SetFileId("m4a4_skin");
	M4A4Skin.AddItem("Bullet Rain");
	M4A4Skin.AddItem("Zirka");
	M4A4Skin.AddItem("Asiimov");
	M4A4Skin.AddItem("Howl");
	M4A4Skin.AddItem("X-Ray");
	M4A4Skin.AddItem("Desert-Strike");
	M4A4Skin.AddItem("Griffin");
	M4A4Skin.AddItem("Dragon King");
	M4A4Skin.AddItem("Poseidon");
	M4A4Skin.AddItem("Daybreak");
	M4A4Skin.AddItem("Evil Daimyo");
	M4A4Skin.AddItem("Royal Paladin");
	M4A4Skin.AddItem("The BattleStar");
	M4A4Skin.AddItem("Desolate Space");
	M4A4Skin.AddItem("Buzz Kill");
	M4A4Skin.AddItem("Hellfire");
	Riflegroup.PlaceLabledControl("M4A4", this, &M4A4Skin);

	AUGSkin.SetFileId("aug_skin");
	AUGSkin.AddItem("Bengal Tiger");
	AUGSkin.AddItem("Hot Rod");
	AUGSkin.AddItem("Chameleon");
	AUGSkin.AddItem("Torque");
	AUGSkin.AddItem("Radiation Hazard");
	AUGSkin.AddItem("Asterion");
	AUGSkin.AddItem("Daedalus");
	AUGSkin.AddItem("Akihabara Accept");
	AUGSkin.AddItem("Ricochet");
	AUGSkin.AddItem("Fleet Flock");
	AUGSkin.AddItem("Syd Mead");
	Riflegroup.PlaceLabledControl("AUG", this, &AUGSkin);

	FAMASSkin.SetFileId("famas_skin");
	FAMASSkin.AddItem("Djinn");
	FAMASSkin.AddItem("Afterimage");
	FAMASSkin.AddItem("Doomkitty");
	FAMASSkin.AddItem("Spitfire");
	FAMASSkin.AddItem("Teardown");
	FAMASSkin.AddItem("Hexane");
	FAMASSkin.AddItem("Pulse");
	FAMASSkin.AddItem("Sergeant");
	FAMASSkin.AddItem("Styx");
	FAMASSkin.AddItem("Neural Net");
	FAMASSkin.AddItem("Survivor");
	FAMASSkin.AddItem("Valence");
	FAMASSkin.AddItem("Roll Cage");
	FAMASSkin.AddItem("Mecha Industries");
	Riflegroup.PlaceLabledControl("FAMAS", this, &FAMASSkin);

	GALILSkin.SetFileId("galil_skin");
	GALILSkin.AddItem("Orange DDPAT");
	GALILSkin.AddItem("Eco");
	GALILSkin.AddItem("Stone Cold");
	GALILSkin.AddItem("Cerberus");
	GALILSkin.AddItem("Aqua Terrace");
	GALILSkin.AddItem("Chatterbox");
	GALILSkin.AddItem("Firefight");
	GALILSkin.AddItem("Rocket Pop");
	GALILSkin.AddItem("Sugar Rush");
	Riflegroup.PlaceLabledControl("GALIL", this, &GALILSkin);

	SG553Skin.SetFileId("sg552_skin");
	SG553Skin.AddItem("Bulldozer");
	SG553Skin.AddItem("Ultraviolet");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Pulse");
	SG553Skin.AddItem("Army Sheen");
	SG553Skin.AddItem("Traveler");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Cyrex");
	SG553Skin.AddItem("Tiger Moth");
	SG553Skin.AddItem("Atlas");
	Riflegroup.PlaceLabledControl("SG553", this, &SG553Skin);
#pragma endregion

#pragma region MPs
	MPGroup.SetPosition(16, 453);
	MPGroup.SetText("MPs");
	MPGroup.SetSize(376, 173);
	RegisterControl(&MPGroup);

	MAC10Skin.SetFileId("mac10_skin");
	MAC10Skin.AddItem("Fade");
	MAC10Skin.AddItem("Neon Rider");
	MAC10Skin.AddItem("Ultraviolet");
	MAC10Skin.AddItem("Palm");
	MAC10Skin.AddItem("Graven");
	MAC10Skin.AddItem("Tatter");
	MAC10Skin.AddItem("Amber Fade");
	MAC10Skin.AddItem("Heat");
	MAC10Skin.AddItem("Curse");
	MAC10Skin.AddItem("Indigo");
	MAC10Skin.AddItem("Commuter");
	MAC10Skin.AddItem("Nuclear Garden");
	MAC10Skin.AddItem("Malachite");
	MAC10Skin.AddItem("Rangeen");
	MAC10Skin.AddItem("Lapis Gator");
	MPGroup.PlaceLabledControl("MAC-10", this, &MAC10Skin);

	P90Skin.SetFileId("p90_skin");
	P90Skin.AddItem("Cold Blooded");
	P90Skin.AddItem("Death by Kitty");
	P90Skin.AddItem("Shapewood");
	P90Skin.AddItem("Emerald Dragon");
	P90Skin.AddItem("Asiimov");
	P90Skin.AddItem("Virus");
	MPGroup.PlaceLabledControl("P90", this, &P90Skin);

	UMP45Skin.SetFileId("ump45_skin");
	UMP45Skin.AddItem("Blaze");
	UMP45Skin.AddItem("Primal Saber");
	UMP45Skin.AddItem("Minotaurs Labyrinth");
	UMP45Skin.AddItem("Grand Prix");
	MPGroup.PlaceLabledControl("UMP-45", this, &UMP45Skin);

	BIZONSkin.SetFileId("bizon_skin");
	BIZONSkin.AddItem("Blue Streak");
	BIZONSkin.AddItem("Antique");
	BIZONSkin.AddItem("Judgement of Anubis");
	BIZONSkin.AddItem("Osiris");
	BIZONSkin.AddItem("Fuel Rod");
	MPGroup.PlaceLabledControl("PP-Bizon", this, &BIZONSkin);

	MP7Skin.SetFileId("mp7_skin");
	MP7Skin.AddItem("Whiteout");
	MP7Skin.AddItem("Nemesis");
	MP7Skin.AddItem("Impire");
	MPGroup.PlaceLabledControl("MP7", this, &MP7Skin);

	MP9Skin.SetFileId("mp9_skin");
	MP9Skin.AddItem("Ruby Poison Dart");
	MP9Skin.AddItem("Hot Rod");
	MPGroup.PlaceLabledControl("MP9", this, &MP9Skin);

#pragma endregion

#pragma region Pistols
	PistolGroup.SetPosition(408, 243);
	PistolGroup.SetText("Pistols");
	PistolGroup.SetSize(360, 250);
	RegisterControl(&PistolGroup);

	GLOCKSkin.SetFileId("glock_skin");
	GLOCKSkin.AddItem("Wasteland Rebel");
	GLOCKSkin.AddItem("Twillight Galaxy");
	GLOCKSkin.AddItem("Fade");
	GLOCKSkin.AddItem("Dragon Tattoo");
	GLOCKSkin.AddItem("Water Elemental");
	GLOCKSkin.AddItem("Royal Legion");
	GLOCKSkin.AddItem("Grinder");
	GLOCKSkin.AddItem("Reactor");
	GLOCKSkin.AddItem("Weasel");
	PistolGroup.PlaceLabledControl("Glock", this, &GLOCKSkin);

	USPSSkin.SetFileId("usps_skin");
	USPSSkin.AddItem("Dark Water");;
	USPSSkin.AddItem("Overgrowth");
	USPSSkin.AddItem("Caiman");
	USPSSkin.AddItem("Blood Tiger");
	USPSSkin.AddItem("Serum");
	USPSSkin.AddItem("Neo-Noir");
	USPSSkin.AddItem("Stainless");
	USPSSkin.AddItem("Guardian");
	USPSSkin.AddItem("Orion");
	USPSSkin.AddItem("Road Rash");
	USPSSkin.AddItem("Royal Blue");
	USPSSkin.AddItem("Business Class");
	USPSSkin.AddItem("Para Green");
	USPSSkin.AddItem("Torque");
	USPSSkin.AddItem("Kill Confirmed");
	USPSSkin.AddItem("Lead Conduit");
	USPSSkin.AddItem("Cyrex");
	PistolGroup.PlaceLabledControl("USP-S", this, &USPSSkin);

	DEAGLESkin.SetFileId("deagle_skin");
	DEAGLESkin.AddItem("Blaze");
	DEAGLESkin.AddItem("Oxide Blaze");
	DEAGLESkin.AddItem("Kumicho Dragon");
	DEAGLESkin.AddItem("Sunset Storm");
	PistolGroup.PlaceLabledControl("Deagle", this, &DEAGLESkin);

	DUALSSkin.SetFileId("duals_skin");
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Duelist");
	DUALSSkin.AddItem("Hemoglobin");
	DUALSSkin.AddItem("Marina");
	DUALSSkin.AddItem("Urban Shock");
	PistolGroup.PlaceLabledControl("Duals", this, &DUALSSkin);

	RevolverSkin.SetFileId("revolver_skin");
	RevolverSkin.AddItem("Fade");
	RevolverSkin.AddItem("Crimson Web");
	RevolverSkin.AddItem("Amber Fade");
	RevolverSkin.AddItem("Reboot");
	PistolGroup.PlaceLabledControl("Revolver", this, &RevolverSkin);

	FIVESEVENSkin.SetFileId("fiveseven_skin");
	FIVESEVENSkin.AddItem("Case Hardened");
	FIVESEVENSkin.AddItem("Fowl Play");
	FIVESEVENSkin.AddItem("Retrobution");
	FIVESEVENSkin.AddItem("Triumvirate");
	FIVESEVENSkin.AddItem("Neon Kimono");
	FIVESEVENSkin.AddItem("Monkey Business");
	FIVESEVENSkin.AddItem("Copper Galaxy");
	FIVESEVENSkin.AddItem("Hyper Beast");
	PistolGroup.PlaceLabledControl("Five-Seven", this, &FIVESEVENSkin);

	TECNINESkin.SetFileId("tec9_skin");
	TECNINESkin.AddItem("Terrace");
	TECNINESkin.AddItem("Isaac");
	TECNINESkin.AddItem("Red Quartz");
	TECNINESkin.AddItem("Avalanche");
	TECNINESkin.AddItem("Toxic");
	TECNINESkin.AddItem("Fuel Injector");
	TECNINESkin.AddItem("Re-Entry");
	TECNINESkin.AddItem("Bamboo Forest");
	TECNINESkin.AddItem("Nuclear Threat");
	PistolGroup.PlaceLabledControl("Tec-9", this, &TECNINESkin);

	P2000Skin.SetFileId("p2000_skin");
	P2000Skin.AddItem("Handgun");
	P2000Skin.AddItem("Corticera");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Fire Elemental");
	P2000Skin.AddItem("Imperial Dragon");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Amber Fade");
	PistolGroup.PlaceLabledControl("P2000", this, &P2000Skin);

	P250Skin.SetFileId("p250_skin");
	P250Skin.AddItem("Whiteout");
	P250Skin.AddItem("Nuclear Threat");
	P250Skin.AddItem("Splash");
	P250Skin.AddItem("Mehndi");
	P250Skin.AddItem("Asiimov");
	P250Skin.AddItem("Undertow");
	P250Skin.AddItem("Franklin");
	P250Skin.AddItem("Supernova");
	P250Skin.AddItem("Cartel");
	PistolGroup.PlaceLabledControl("P250", this, &P250Skin);

#pragma endregion

#pragma region Skinsettings
	SkinsettingsGroup.SetPosition(408, 506);
	SkinsettingsGroup.SetText("Customisation");
	SkinsettingsGroup.SetSize(360, 118);
	RegisterControl(&SkinsettingsGroup);

	StatTrakEnable.SetFileId("skin_stattrack");
	SkinsettingsGroup.PlaceLabledControl("Stat Trak", this, &StatTrakEnable);

	StatTrackAmount.SetFileId("skin_stamount");
	SkinsettingsGroup.PlaceLabledControl("Value", this, &StatTrackAmount);

	KnifeName.SetFileId("knife_name");
	SkinsettingsGroup.PlaceLabledControl("Knife Name", this, &KnifeName);

	SkinName.SetFileId("skin_name");
	SkinsettingsGroup.PlaceLabledControl("Skin Name", this, &SkinName);

	SkinApply.SetText("Apply Changes");
	SkinApply.SetCallback(KnifeApplyCallbk);
	SkinApply.SetPosition(408, 635);
	SkinApply.SetSize(360, 106);
	RegisterControl(&SkinApply);

#pragma endregion

#pragma region Glovechanger

	Gloves.SetPosition(778, 21);
	Gloves.SetText("Override Gloves");
	Gloves.SetSize(376, 100);
	RegisterControl(&Gloves);

	EnableGloves.SetFileId("gloves_enable");
	Gloves.PlaceLabledControl("Enable Glovechanger", this, &EnableGloves);

	GloveModel.SetFileId("glove_model");
	GloveModel.AddItem("Off");
	GloveModel.AddItem("Bloodhound");
	GloveModel.AddItem("Handwrap");
	GloveModel.AddItem("Driver");
	GloveModel.AddItem("Sport");
	GloveModel.AddItem("Motorcycle");
	GloveModel.AddItem("Specialist");
	Gloves.PlaceLabledControl("Glove Model", this, &GloveModel);

	GloveSkin.SetFileId("glove_skin");
	GloveSkin.AddItem("Off");
	GloveSkin.AddItem("Bloodhound - Snakebite");
	GloveSkin.AddItem("Bloodhound - Charred");
	GloveSkin.AddItem("Bloodhound - Guerrilla");
	GloveSkin.AddItem("Bloodhound - Bronzed");
	GloveSkin.AddItem("Handwrap - Slaughter");
	GloveSkin.AddItem("Handwrap - Badlands");
	GloveSkin.AddItem("Handwrap - Leather");
	GloveSkin.AddItem("Handwrap - Spruce DDPAT");
	GloveSkin.AddItem("Driver - Crimson Weave");
	GloveSkin.AddItem("Driver - Lunar Weave");
	GloveSkin.AddItem("Driver - Diamondback");
	GloveSkin.AddItem("Driver - Convoy");
	GloveSkin.AddItem("Sport - Pandoras Box");
	GloveSkin.AddItem("Sport - Hedge Naze");
	GloveSkin.AddItem("Sport - Superconductor");
	GloveSkin.AddItem("Sport - Arid");
	GloveSkin.AddItem("Motorcycle - Spearmint");
	GloveSkin.AddItem("Motorcycle - Cool Mint");
	GloveSkin.AddItem("Motorcycle - Boom");
	GloveSkin.AddItem("Motorcycle - Eclipse");
	GloveSkin.AddItem("Specialist - Crimson Kimono");
	GloveSkin.AddItem("Specialist - Emerald Web");
	GloveSkin.AddItem("Specialist - Foundation");
	GloveSkin.AddItem("Specialist - Forest DDPAT");
	Gloves.PlaceLabledControl("Glove Skin", this, &GloveSkin);

	GlovesApply.SetText("Apply Gloves");
	GlovesApply.SetCallback(GlovesApplyCallbk);
	GlovesApply.SetPosition(778, 131);
	GlovesApply.SetSize(360, 106);
	RegisterControl(&GlovesApply);

#pragma endregion
}

void CMiscTab::Setup()
{
	SetTitle("G");

#pragma region Other
	OtherGroup.SetPosition(270, 10);
	OtherGroup.SetSize(270, 380);
	OtherGroup.SetText("Misc");
	RegisterControl(&OtherGroup);

	OtherChatSpam.SetFileId("otr_spam");
	OtherChatSpam.AddItem("Off");
	OtherChatSpam.AddItem("Regular");
	OtherChatSpam.AddItem("Disperse Name");
	OtherChatSpam.AddItem("Namespam");
	OtherGroup.PlaceLabledControl("Chat Spam", this, &OtherChatSpam);

	OtherChatDelay.SetFileId("otr_chatdelay");
	OtherChatDelay.SetBoundaries(0.1, 3.0);
	OtherChatDelay.SetValue(2.0);
	OtherGroup.PlaceLabledControl("Spam Delay", this, &OtherChatDelay);

	OtherClantag.SetFileId("otr_spam");
	OtherClantag.AddItem("Off");
	OtherClantag.AddItem("Zaros");
	OtherClantag.AddItem("Meme");
	OtherClantag.AddItem("Decryption");
	OtherClantag.AddItem("Local Time");
	OtherGroup.PlaceLabledControl("Custom Clantag", this, &OtherClantag);

	OtherClantagspeed.SetFileId("otr_clantagspeed");
	OtherClantagspeed.SetBoundaries(0, 10);
	OtherClantagspeed.SetValue(3);
	OtherGroup.PlaceLabledControl("Clantag Speed", this, &OtherClantagspeed);

	OtherAirStuck.SetFileId("otr_astuck");
	OtherGroup.PlaceLabledControl("Air Stuck", this, &OtherAirStuck);

	OtherSpectators.SetFileId("otr_speclist");
	OtherGroup.PlaceLabledControl("Spectators List", this, &OtherSpectators);

	WaterMark.SetFileId("otr_wtrmrk");
	WaterMark.SetState(true);
	OtherGroup.PlaceLabledControl("Watermark", this, &WaterMark);

	Info.SetFileId("otr_info");
	Info.SetState(true);
	OtherGroup.PlaceLabledControl("Info", this, &Info);

	OtherHitmarker.SetFileId("otr_hitmarker");
	OtherGroup.PlaceLabledControl("Hitmarker", this, &OtherHitmarker);

	HitmarkerSound.SetFileId("otr_hitmarkersound");
	OtherGroup.PlaceLabledControl("Hitmarker Sound", this, &HitmarkerSound);

	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	OtherGroup.PlaceLabledControl("Anti Untrusted", this, &OtherSafeMode);	

#pragma endregion other random options

#pragma region Strafing
	StrafingGroup.SetPosition(10, 10);
	StrafingGroup.SetSize(250, 250);
	StrafingGroup.SetText("Movement");
	RegisterControl(&StrafingGroup);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherAutoJump.AddItem("Off");
	OtherAutoJump.AddItem("Normal");
	StrafingGroup.PlaceLabledControl("Auto Jump", this, &OtherAutoJump);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherAutoStrafe.AddItem("Off");
	OtherAutoStrafe.AddItem("Legit");
	OtherAutoStrafe.AddItem("Rage");
	StrafingGroup.PlaceLabledControl("Auto Strafer", this, &OtherAutoStrafe);

	OtherStrafeSpeed.SetFileId("acc_strafespeed");
	OtherStrafeSpeed.SetBoundaries(0.f, 10.f);
	OtherStrafeSpeed.SetValue(5.f);
	StrafingGroup.PlaceLabledControl("Strafe Speed", this, &OtherStrafeSpeed);

	OtherCircle.SetFileId("otr_circle");
	StrafingGroup.PlaceLabledControl("Circle Strafe", this, &OtherCircle);

	OtherCircleKey.SetFileId("otr_circlekey");
	StrafingGroup.PlaceLabledControl("Circle Key", this, &OtherCircleKey);

	FakeWalk.SetFileId("otr_fakewalk");
	StrafingGroup.PlaceLabledControl("Fake Walk Key", this, &FakeWalk);

#pragma endregion

}

void CGUITab::Setup()
{
	SetTitle("H");

#pragma region ESPColorBox
	ESPColorBox.SetText("CT ESP");
	ESPColorBox.SetPosition(10, 10);
	ESPColorBox.SetSize(190, 220);
	RegisterControl(&ESPColorBox);

	RBoxCT.SetFileId("colorbox_rmma");
	RBoxCT.SetBoundaries(0, 255);
	RBoxCT.SetValue(0);
	ESPColorBox.PlaceLabledControl("R CT", this, &RBoxCT);

	GBoxCT.SetFileId("colorbox_gmma");
	GBoxCT.SetBoundaries(0, 255);
	GBoxCT.SetValue(0);
	ESPColorBox.PlaceLabledControl("G CT", this, &GBoxCT);

	BBoxCT.SetFileId("colorbox_bmma");
	BBoxCT.SetBoundaries(0, 255);
	BBoxCT.SetValue(255);
	ESPColorBox.PlaceLabledControl("B CT", this, &BBoxCT);

	ABoxCT.SetFileId("colorbox_amma");
	ABoxCT.SetBoundaries(0, 255);
	ABoxCT.SetValue(255);
	ESPColorBox.PlaceLabledControl("A CT", this, &ABoxCT);

	RBoxCTV.SetFileId("colorbox_rmma");
	RBoxCTV.SetBoundaries(0, 255);
	RBoxCTV.SetValue(0);
	ESPColorBox.PlaceLabledControl("R CT Visible", this, &RBoxCTV);

	GBoxCTV.SetFileId("colorbox_gmma");
	GBoxCTV.SetBoundaries(0, 255);
	GBoxCTV.SetValue(255);
	ESPColorBox.PlaceLabledControl("G CT Visible", this, &GBoxCTV);

	BBoxCTV.SetFileId("colorbox_bmma");
	BBoxCTV.SetBoundaries(0, 255);
	BBoxCTV.SetValue(0);
	ESPColorBox.PlaceLabledControl("B CT Visible", this, &BBoxCTV);

	ABoxCTV.SetFileId("colorbox_amma");
	ABoxCTV.SetBoundaries(0, 255);
	ABoxCTV.SetValue(255);
	ESPColorBox.PlaceLabledControl("A CT Visible", this, &ABoxCTV);


#pragma endregion

#pragma region ESPColorBox2
	ESPColorBox2.SetText("T ESP");
	ESPColorBox2.SetPosition(210, 10);
	ESPColorBox2.SetSize(190, 220);
	RegisterControl(&ESPColorBox2);

	RBoxT.SetFileId("colorbox_rmma");
	RBoxT.SetBoundaries(0, 255);
	RBoxT.SetValue(255);
	ESPColorBox2.PlaceLabledControl("R T", this, &RBoxT);

	GBoxT.SetFileId("colorbox_gmma");
	GBoxT.SetBoundaries(0, 255);
	GBoxT.SetValue(0);
	ESPColorBox2.PlaceLabledControl("G T", this, &GBoxT);

	BBoxT.SetFileId("colorbox_bmma");
	BBoxT.SetBoundaries(0, 255);
	BBoxT.SetValue(0);
	ESPColorBox2.PlaceLabledControl("B T", this, &BBoxT);

	ABoxT.SetFileId("colorbox_amma");
	ABoxT.SetBoundaries(0, 255);
	ABoxT.SetValue(255);
	ESPColorBox2.PlaceLabledControl("A T", this, &ABoxT);

	RBoxTV.SetFileId("colorbox_rmma");
	RBoxTV.SetBoundaries(0, 255);
	RBoxTV.SetValue(255);
	ESPColorBox2.PlaceLabledControl("R T Visible", this, &RBoxTV);

	GBoxTV.SetFileId("colorbox_gmma");
	GBoxTV.SetBoundaries(0, 255);
	GBoxTV.SetValue(255);
	ESPColorBox2.PlaceLabledControl("G T Visible", this, &GBoxTV);

	BBoxTV.SetFileId("colorbox_bmma");
	BBoxTV.SetBoundaries(0, 255);
	BBoxTV.SetValue(0);
	ESPColorBox2.PlaceLabledControl("B T Visible", this, &BBoxTV);

	ABoxTV.SetFileId("colorbox_amma");
	ABoxTV.SetBoundaries(0, 255);
	ABoxTV.SetValue(255);
	ESPColorBox2.PlaceLabledControl("A T Visible", this, &ABoxTV);

#pragma endregion

#pragma region GLOWCOLOR
	GLOWColorbox.SetText("Glow Color");
	GLOWColorbox.SetPosition(10, 240);
	GLOWColorbox.SetSize(190, 130);
	RegisterControl(&GLOWColorbox);

	GlowR.SetFileId("GlowR");
	GlowR.SetBoundaries(0.f, 255.f);
	GlowR.SetValue(230.f);
	GLOWColorbox.PlaceLabledControl("Red", this, &GlowR);

	GlowG.SetFileId("GlowG");
	GlowG.SetBoundaries(0.f, 255.f);
	GlowG.SetValue(0.f);
	GLOWColorbox.PlaceLabledControl("Green", this, &GlowG);

	GlowB.SetFileId("GlowB");
	GlowB.SetBoundaries(0.f, 255.f);
	GlowB.SetValue(0.f);
	GLOWColorbox.PlaceLabledControl("Blue", this, &GlowB);

	GlowZ.SetFileId("opt_glowz");
	GlowZ.SetBoundaries(0.f, 255.f);
	GlowZ.SetValue(255);
	GLOWColorbox.PlaceLabledControl("Alpha", this, &GlowZ);

#pragma endregion

#pragma region Chams CT

	ChamsCT.SetText("CT Chams");
	ChamsCT.SetPosition(410, 10);
	ChamsCT.SetSize(190, 105);
	RegisterControl(&ChamsCT);

	CTChamsR.SetFileId("ctchamsr");
	CTChamsR.SetBoundaries(0.f, 255.f);
	CTChamsR.SetValue(40.f);
	ChamsCT.PlaceLabledControl("Red", this, &CTChamsR);

	CTChamsG.SetFileId("ctchamsg");
	CTChamsG.SetBoundaries(0.f, 255.f);
	CTChamsG.SetValue(120.f);
	ChamsCT.PlaceLabledControl("Green", this, &CTChamsG);

	CTChamsB.SetFileId("ctchamsb");
	CTChamsB.SetBoundaries(0.f, 255.f);
	CTChamsB.SetValue(170.f);
	ChamsCT.PlaceLabledControl("Blue", this, &CTChamsB);
#pragma endregion Chams CT

#pragma region T Chams

	ChamsT.SetText("T Chams");
	ChamsT.SetPosition(410, 125);
	ChamsT.SetSize(190, 105);
	RegisterControl(&ChamsT);

	TChamsR.SetFileId("tchamsr");
	TChamsR.SetBoundaries(0.f, 255.f);
	TChamsR.SetValue(210.f);
	ChamsT.PlaceLabledControl("Red", this, &TChamsR);

	TChamsG.SetFileId("tchamsg");
	TChamsG.SetBoundaries(0.f, 255.f);
	TChamsG.SetValue(20.f);
	ChamsT.PlaceLabledControl("Green", this, &TChamsG);

	TChamsB.SetFileId("tchamsb");
	TChamsB.SetBoundaries(0.f, 255.f);
	TChamsB.SetValue(70.f);
	ChamsT.PlaceLabledControl("Blue", this, &TChamsB);

#pragma endregion T Chams
};

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoUIFrame()
{
	// General Processing

	// If the "all filter is selected tick all the others
	if (Window.VisualsTab.FiltersAll.GetState())
	{
		Window.VisualsTab.FiltersC4.SetState(true);
		Window.VisualsTab.FiltersChickens.SetState(true);
		Window.VisualsTab.FiltersPlayers.SetState(true);
		Window.VisualsTab.FiltersWeapons.SetState(true);
		Window.VisualsTab.FiltersNades.SetState(true);
		Window.VisualsTab.FiltersEnemiesOnly.SetState(true);
	}

	GUI.Update();
	GUI.Draw();

}