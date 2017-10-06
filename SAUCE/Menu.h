#pragma once

#include "GUI.h"
#include "Controls.h"

class CRageBotTab : public CTab
{
public:
	void Setup();

	CLabel ActiveLabel;
	CCheckBox Active;

	CCheckBox Accuracydisableinterp;
	CCheckBox Accuracylagcomp;


	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CSlider1  AimbotFov;
	CCheckBox AimbotSilentAim;
	CCheckBox PVSFix;
	CComboBox AimbotResolver;
	CCheckBox AdvancedResolver;
	CCheckBox AutoRevolver;
	CCheckBox AutoPistol;
	CSlider1 AddFakeYaw;
	CSlider1 CustomPitch;
	CCheckBox LBY;
	CComboBox FakeYaw;
	CCheckBox LowerbodyFix;
	CCheckBox PreferBodyAim;
	CCheckBox PreferHeadAim;
	CCheckBox AWPAtBody;
	CSlider1 BaimIfUnderXHealth;
	CSlider1 XShots;
	CCheckBox EnginePrediction;
	CCheckBox AimbotAimStep;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CKeyBind  AimbotStopKey;

	CGroupBox TargetGroup;
	CComboBox TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBox TargetHitbox;
	CComboBox TargetHitscan;
	CCheckBox TargetMultipoint;
	CSlider   TargetPointscale;
	CSlider AccuracySmart;

	CGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;
	CCheckBox AccuracyAutoWall;
	CSlider1 AccuracyMinimumDamage;
	CCheckBox AccuracyAutoStop;
	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CSlider1   AccuracyHitchance;
	CComboBox AccuracyResolver;

	CGroupBox AntiAimGroup;
	CCheckBox AntiAimEnable;
	CComboBox AntiAimPitch;
	CComboBox AntiAimYaw;
	CCheckBox FlipAA;
	CCheckBox AntiAimEdge;
	CSlider1 AntiAimOffset;
	CCheckBox AntiAimKnife;
	CCheckBox AntiAimTarget;
	CCheckBox WeaponCheck;
	CGroupBox wpnhitchanceGroup;
	CSlider  scoutChance;
	CSlider  AWPChance;
	CSlider  AutoChance;
	CSlider  RifleChance;
	CSlider  MPChance;
	CSlider  PistolChance;
	CSlider  R8Chance;

	CGroupBox wpnmindmgGroup;
	CSlider  scoutmindmg;
	CSlider  AWPmindmg;
	CSlider  Automindmg;
	CSlider  Riflemindmg;
	CSlider  MPmindmg;
	CSlider  Pistolmindmg;
	CSlider  R8mindmg;
};

class CVisualTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Options Settings
	CGroupBox OptionsGroup;
	CCheckBox OptionsBox;
	CComboBox BoxDesign;
	CCheckBox OptionsName;
	
	CCheckBox OptionHealthEnable;
	CComboBox OptionsHealth;
	CCheckBox OptionsArmur;
	CCheckBox OptionsWeapon;
	CCheckBox OptionsInfo;
	CCheckBox OptionsHelmet;
	CComboBox OtherWorld;
	CCheckBox OptionsKit;
	CCheckBox OptionsDefuse;
	//CCheckBox OptionsGlow;
	CCheckBox OptionsSkeleton;
	CCheckBox OptionsAimSpot;
	CCheckBox OptionsCompRank;

	// Skeleton ESP Colour
	CGroupBox ESPColorSkeleton;
	CSlider   RSkele;
	CSlider   GSkele;
	CSlider   BSkele;
	CSlider   ASkele;

	CCheckBox FiltersNades;
	CCheckBox EntityGlow;

	// Box ESP Colour
	CGroupBox ESPColorBox;
	CGroupBox ESPColorBox2;
	CSlider   RBoxCT;
	CSlider   GBoxCT;
	CSlider   BBoxCT;
	CSlider   ABoxCT;
	CSlider   RBoxCTV;
	CSlider   GBoxCTV;
	CSlider   BBoxCTV;
	CSlider   ABoxCTV;
	CSlider   RBoxT;
	CSlider   GBoxT;
	CSlider   BBoxT;
	CSlider   ABoxT;
	CSlider   RBoxTV;
	CSlider   GBoxTV;
	CSlider   BBoxTV;
	CSlider   ABoxTV;

	// Filters Settings
	CGroupBox FiltersGroup;
	CCheckBox FiltersAll;
	CCheckBox FiltersPlayers;
	CCheckBox FiltersEnemiesOnly;
	CCheckBox FiltersWeapons;
	CCheckBox FiltersChickens;
	CCheckBox FiltersC4;
	CSlider1  Filterspos;
	// Other Settings
	CGroupBox OtherGroup;
	CComboBox OtherCrosshair;
	CComboBox OtherRecoilCrosshair;
	CCheckBox OtherRadar;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox OtherNoSky;
	CCheckBox OtherNoFlash;
	CSlider1 OtherNoFlashAlpha;
	CCheckBox OtherNoSmoke;
	CCheckBox OtherAsusWalls;
	CComboBox OtherNoHands;
	CCheckBox OtherThirdperson;
	CSlider1 OtherThirdpersonRange;
	CKeyBind OtherThirdpersonKey;
	CCheckBox IsScoped;
	CCheckBox IsReloading;
	CSlider1  OtherViewmodelFOV;
	CSlider1  OtherFOV;
	CCheckBox GrenadeTrace;
	CCheckBox CustomSky;
	CCheckBox OtherNoScope;
	CCheckBox OptionsFill;
	CCheckBox HasDefuser;
	CCheckBox OptionsGlow;
	CComboBox OptionsChams;
	CCheckBox LBY;
	CSlider   TraceLength;
	CCheckBox Clock;
	CCheckBox DisablePostProcess;
};

class CMiscTab : public CTab
{
public:
	void Setup();

	// Other Settings
	CGroupBox OtherGroup;
	CCheckBox OtherSafeMode;
	CComboBox OtherChatSpam;
	CCheckBox OtherTeamChat;
	CSlider	  OtherChatDelay;
	CKeyBind  OtherAirStuck;
	CKeyBind FakeWalk;
	CKeyBind  OtherLagSwitch;
	CCheckBox OtherSpectators;
	CCheckBox HitmarkerSound;
	CCheckBox OtherHitmarker;
	CCheckBox OtherThirdperson;
	CCheckBox AutoAccept;

	//CCheckBox OtherAutoAccept;
	CCheckBox OtherWalkbot;
	CSlider   Distance;
	CSlider   Edgeam;

	// Clan Tag Settings
	CComboBox OtherClantag;
	CSlider1  OtherClantagspeed;
	

	// Strafing
	CGroupBox StrafingGroup;
	CComboBox OtherAutoJump;
	CComboBox OtherAutoStrafe;
	CSlider   OtherStrafeSpeed;
	CCheckBox OtherEdgeJump;
	CCheckBox OtherCircle;
	CKeyBind OtherCircleKey;




	// Rank Reveal
	CGroupBox RankGroup;
	CCheckBox WaterMark;
	CCheckBox Info;
	CCheckBox OtherSlowmo;
	CCheckBox OtherLBY;
};

class CSkinTab : public CTab
{
public:
	void Setup();

	CLabel SkinActive;
	CCheckBox SkinEnable;
	CButton   SkinApply;

	CGroupBox KnifeGroup;
	CComboBox KnifeModel;
	CTextField KnifeSkin;

	CGroupBox PistolGroup;
	CComboBox GLOCKSkin;
	CComboBox USPSSkin;
	CComboBox DEAGLESkin;
	CComboBox MAGNUMSkin;
	CComboBox DUALSSkin;
	CComboBox RevolverSkin;
	CComboBox FIVESEVENSkin;
	CComboBox TECNINESkin;
	CComboBox P2000Skin;
	CComboBox P250Skin;

	CGroupBox MPGroup;
	CComboBox MAC10Skin;
	CComboBox P90Skin;
	CComboBox UMP45Skin;
	CComboBox BIZONSkin;
	CComboBox MP7Skin;
	CComboBox MP9Skin;

	CGroupBox Riflegroup;
	CComboBox M41SSkin;
	CComboBox M4A4Skin;
	CComboBox AK47Skin;
	CComboBox AUGSkin;
	CComboBox FAMASSkin;
	CComboBox GALILSkin;
	CComboBox SG553Skin;

	CGroupBox MachinegunsGroup;
	CComboBox NEGEVSkin;
	CComboBox M249Skin;

	CGroupBox Snipergroup;
	CComboBox SCAR20Skin;
	CComboBox G3SG1Skin;
	CComboBox SSG08Skin;
	CComboBox AWPSkin;

	CGroupBox Shotgungroup;
	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;
	CComboBox NOVASkin;

	CGroupBox SkinsettingsGroup;
	CCheckBox StatTrakEnable;
	CTextField StatTrackAmount;
	CTextField SkinName;
	CTextField KnifeName;

	CGroupBox Gloves;
	CCheckBox EnableGloves;
	CComboBox GloveSkin;
	CComboBox GloveModel;
	CButton GlovesApply;
};

class CGUITab : public CTab
{
public:
	void Setup();

	// Skeleton ESP Colour
	CGroupBox ESPColorSkeleton;
	CSlider   RSkele;
	CSlider   GSkele;
	CSlider   BSkele;
	CSlider   ASkele;

	CGroupBox ChamsCT;
	CGroupBox ChamsT;

	// Box ESP Colour
	CGroupBox ESPColorBox;
	CGroupBox ESPColorBox2;
	CSlider   RBoxCT;
	CSlider   GBoxCT;
	CSlider   BBoxCT;
	CSlider   ABoxCT;
	CSlider   RBoxCTV;
	CSlider   GBoxCTV;
	CSlider   BBoxCTV;
	CSlider   ABoxCTV;
	CSlider   RBoxT;
	CSlider   GBoxT;
	CSlider   BBoxT;
	CSlider   ABoxT;
	CSlider   RBoxTV;
	CSlider   GBoxTV;
	CSlider   BBoxTV;
	CSlider   ABoxTV;

	CSlider TChams;
	CSlider CTChams;
	CSlider CTChamsR;
	CSlider CTChamsG;
	CSlider CTChamsB;
	CSlider TChamsR;
	CSlider TChamsB;
	CSlider TChamsG;

	// Main Menu
	CGroupBox ColorMain;
	CSlider   RMainMenuA;
	CSlider   GMainMenuA;
	CSlider   BMainMenuA;
	CSlider   AMainMenuA;
	CSlider   RMainMenuB;
	CSlider   GMainMenuB;
	CSlider   BMainMenuB;
	CSlider   AMainMenuB;

	// Menu Text
	CGroupBox ColorText;
	CSlider   RMenuText;
	CSlider   GMenuText;
	CSlider   BMenuText;

	// Inner Window
	CGroupBox ColorInner;
	CSlider   RInner;
	CSlider   GInner;
	CSlider   BInner;
	CSlider   AInner;

	// Tabs
	CGroupBox ColorTabs;
	CSlider   RTabs;
	CSlider   GTabs;
	CSlider   BTabs;

	CGroupBox RGBGroup;
	CSlider1   MenuR;
	CSlider1   MenuG;
	CSlider1   MenuB;


	CGroupBox ColorsGroup;
	CSlider1 CTColorVisR;
	CSlider1 CTColorVisG;
	CSlider1 CTColorVisB;

	CSlider1 CTColorNoVisR;
	CSlider1 CTColorNoVisG;
	CSlider1 CTColorNoVisB;

	CLabel ColorSpacer;

	CSlider1 TColorVisR;
	CSlider1 TColorVisG;
	CSlider1 TColorVisB;

	CSlider1 TColorNoVisR;
	CSlider1 TColorNoVisG;
	CSlider1 TColorNoVisB;


	CGroupBox GLOWColorbox;
	CSlider1 GlowR;
	CSlider1 GlowG;
	CSlider1 GlowB;
	CSlider1 GlowZ;


};

// Elements that can be placed anywhere in any tabs, should be decleared here.
class AyyWareWindow : public CWindow
{
public:
	void Setup();

	CRageBotTab RageBotTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;
	CSkinTab SkinTab;
	CGUITab GUITab;

	CButton SaveButton;
	CButton LoadButton;
	CComboBox ConfigBox;
};

namespace Menu
{
	void SetupMenu();
	void DoUIFrame();

	extern AyyWareWindow Window;
};

