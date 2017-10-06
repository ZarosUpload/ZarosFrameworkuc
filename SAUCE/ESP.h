/*
Syn's AyyWare Framework 2015
*/

#pragma once

#include "Hacks.h"



class CEsp : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:

	// Other shit
	IClientEntity *BombCarrier;

	
	struct ESPBox
	{
		int x, y, w, h;
	};

	// Draw a player
	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo);


	void Junk4();

	// Get player info
	Color GetPlayerColor(IClientEntity* pEntity);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);


	// Draw shit about player
	void SpecList();
	void DrawBox(ESPBox size, Color color);
	void Info(IClientEntity * pEntity, CEsp::ESPBox size);
	void DrawWeapon(IClientEntity * pEntity, CEsp::ESPBox size);
	void DrawIcon(IClientEntity * pEntity, CEsp::ESPBox size);
	void DrawName(player_info_t pinfo, ESPBox size);
	void DrawHealth(IClientEntity* pEntity, ESPBox size);
	void Fill(CEsp::ESPBox size, Color color2);
	void DrawArmor(IClientEntity * pEntity, CEsp::ESPBox size);
	void lbyup(IClientEntity * pEntity, CEsp::ESPBox size);
	void DrawInfo(IClientEntity* pEntity, CEsp::ESPBox size);
	void DrawCross(IClientEntity* pEntity);
	void DrawSkeleton(IClientEntity* pEntity);
	void DrawGlow();
	void EntityGlow();
	void GrenadeTrace();
	void DrawChicken(IClientEntity* pEntity, ClientClass* cClass);
	void DrawDrop(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
	void Junk();
	void Junk2();
	void DrawMolotov(IClientEntity * pEntity, ClientClass * cClass);
	void DrawSmoke(IClientEntity * pEntity, ClientClass * cClass);
	void DrawDecoy(IClientEntity * pEntity, ClientClass * cClass);
	void DrawHE(IClientEntity * pEntity, ClientClass * cClass);
};

class CGlowObjectManager
{
public:
	class GlowObjectDefinition_t
	{
	public:
		void set(Color color)
		{
			m_vGlowColor = Vector(color.rBase(), color.gBase(), color.bBase());
			m_flGlowAlpha = color.aBase();
			m_bRenderWhenOccluded = true;
			m_bRenderWhenUnoccluded = false;
			m_flBloomAmount = 1.f;
		}

		IClientEntity* getEntity()
		{
			return m_hEntity;
		}

		bool IsEmpty() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

	public:
		IClientEntity*		m_hEntity;
		Vector				m_vGlowColor;
		float				m_flGlowAlpha;

		char				unknown[4];
		float				flUnk;
		float				m_flBloomAmount;
		float				localplayeriszeropoint3;


		bool				m_bRenderWhenOccluded;
		bool				m_bRenderWhenUnoccluded;
		bool				m_bFullBloomRender;
		char				unknown1[1];


		int					m_nFullBloomStencilTestValue;
		int					iUnk;
		int					m_nSplitScreenSlot;
		int					m_nNextFreeSlot;

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	GlowObjectDefinition_t* m_GlowObjectDefinitions;
	int		max_size;
	int		pad;
	int		size;
	GlowObjectDefinition_t* m_GlowObjectDefinitions2;
	int		currentObjects;
};
