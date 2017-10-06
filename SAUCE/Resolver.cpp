#include "Resolver.h"
#include "Ragebot.h"
#include "Hooks.h"

void LowerBodyYawFix(IClientEntity* pEntity)
{
	if (Menu::Window.RageBotTab.LowerbodyFix.GetState())
	{
		if (!pEntity) return;
		if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer) return;
		if (!pEntity->IsAlive() || !pEntity->GetActiveWeaponHandle()) return;
		if (Interfaces::Engine->GetLocalPlayer()) return;

		auto EyeAngles = pEntity->GetEyeAnglesXY();
		if (pEntity->GetVelocity().Length() > 1 && (pEntity->GetFlags() & (int)pEntity->GetFlags() & FL_ONGROUND))
			EyeAngles->y = pEntity->GetLowerBodyYaw();
	}
}



void ResolverSetup::Resolve(IClientEntity* pEntity)
{
	bool MeetsLBYReq;
	if (pEntity->GetFlags() & FL_ONGROUND)
		MeetsLBYReq = true;
	else
		MeetsLBYReq = false;

	bool IsMoving;
	if (pEntity->GetVelocity().Length2D() >= 0.5)
		IsMoving = true;
	else
		IsMoving = false;

	ResolverSetup::NewANgles[pEntity->GetIndex()] = *pEntity->GetEyeAnglesXY();
	ResolverSetup::newlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::newsimtime = pEntity->GetSimulationTime();
	ResolverSetup::newdelta[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::newlbydelta[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::finaldelta[pEntity->GetIndex()] = ResolverSetup::newdelta[pEntity->GetIndex()] - ResolverSetup::storeddelta[pEntity->GetIndex()];
	ResolverSetup::finallbydelta[pEntity->GetIndex()] = ResolverSetup::newlbydelta[pEntity->GetIndex()] - ResolverSetup::storedlbydelta[pEntity->GetIndex()];
	if (newlby == storedlby)
		ResolverSetup::lbyupdated = false;
	else
		ResolverSetup::lbyupdated = true;

	if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 0)
	{

	}
	else if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 1)//level 1
	{
		if (MeetsLBYReq && lbyupdated)
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
		else if (!MeetsLBYReq && lbyupdated)
		{
			switch (Globals::Shots % 4)
			{
			case 1:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
				break;
			case 2:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 40;
				break;
			case 3:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 15;
				break;
			case 4:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
				break;
			}
		}
		else if (MeetsLBYReq && !lbyupdated)
		{
			switch (Globals::Shots % 4)
			{
			case 1:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 15;
				break;
			case 2:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y + 40;
				break;
			case 3:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y + 15;
				break;
			case 4:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 40;
				break;
			}
		}
		else
			pEntity->GetEyeAnglesXY()->y = rand() % 180 - rand() % 30;
	}
	else if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 2) //level 2 
	{
		if (Resolver::didhitHS)
		{
			if (MeetsLBYReq && lbyupdated)
			{
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
			}
			switch (Globals::Shots % 6)
			{
			case 1:
				*pEntity->GetEyeAnglesXY() = StoredAngles[pEntity->GetIndex()];
				break;
			case 2:
				*pEntity->GetEyeAnglesXY() = StoredAngles[pEntity->GetIndex()];
				break;
			case 3:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
				break;
			case 4:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y + 40;
				break;
			case 5:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y + 15;
				break;
			case 6:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 40;
				break;
			}
		}
		else if (MeetsLBYReq && lbyupdated && !Resolver::didhitHS)
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
		}
		else if (!MeetsLBYReq || !lbyupdated && !Resolver::didhitHS)
		{
			pEntity->GetEyeAnglesXY()->y = rand() % 180 - rand() % 30;
		}
		else
			pEntity->GetEyeAnglesXY()->y = rand() % 180;
	}
	else if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 3)//level 3
	{
		if (Globals::missedshots > 3 && Globals::missedshots < 21)
		{
			if (MeetsLBYReq && lbyupdated)
			{
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
			}
			else if (!MeetsLBYReq && lbyupdated)
			{
				switch (Globals::Shots % 6)
				{
				case 1:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
					break;
				case 2:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 40;
					break;
				case 3:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 15;
					break;
				case 4:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
					break;
				case 5:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 70;
					break;
				case 6:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 70;
					break;

				}
			}
			else
				pEntity->GetEyeAnglesXY()->y = rand() % 180 - rand() % 30;
		}

		else if (Globals::missedshots >= 2 && Globals::missedshots <= 3)
		{
			if (MeetsLBYReq && lbyupdated)
			{
				pEntity->GetEyeAnglesXY()->y = ResolverSetup::finallbydelta[pEntity->GetIndex()];
			}
			else
				pEntity->GetEyeAnglesXY()->y = ResolverSetup::finaldelta[pEntity->GetIndex()];
		}
		else
		{
			if (MeetsLBYReq && lbyupdated)
			{
				bool timer;
				if (timer)
					pEntity->GetEyeAnglesXY()->y = ResolverSetup::finallbydelta[pEntity->GetIndex()] + rand() % 30;
				else
					pEntity->GetEyeAnglesXY()->y = ResolverSetup::finallbydelta[pEntity->GetIndex()] - rand() % 30;
				timer = !timer;
			}
			else
			{
				bool timer;
				if (timer)
					pEntity->GetEyeAnglesXY()->y = ResolverSetup::finaldelta[pEntity->GetIndex()] + rand() % 30;
				else
					pEntity->GetEyeAnglesXY()->y = ResolverSetup::finaldelta[pEntity->GetIndex()] - rand() % 30;
				timer = !timer;
			}
		}
	}
	LowerBodyYawFix(pEntity);
	
}

void ResolverSetup::StoreFGE(IClientEntity* pEntity)
{
	ResolverSetup::storedanglesFGE = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::storedlbyFGE = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtimeFGE = pEntity->GetSimulationTime();
}

void ResolverSetup::StoreThings(IClientEntity* pEntity)
{
	ResolverSetup::StoredAngles[pEntity->GetIndex()] = *pEntity->GetEyeAnglesXY();
	ResolverSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtime = pEntity->GetSimulationTime();
	ResolverSetup::storeddelta[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
}

void ResolverSetup::CM(IClientEntity* pEntity)
{
	for (int x = 1; x < Interfaces::Engine->GetMaxClients(); x++)
	{

		pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(x);

		if (!pEntity
			|| pEntity == hackManager.pLocal()
			|| pEntity->IsDormant()
			|| !pEntity->IsAlive())
			continue;

		ResolverSetup::StoreThings(pEntity);
	}
}

void ResolverSetup::FSN(IClientEntity* pEntity, ClientFrameStage_t stage)
{
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (int i = 1; i < Interfaces::Engine->GetMaxClients(); i++)
		{

			pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

			if (!pEntity
				|| pEntity == hackManager.pLocal()
				|| pEntity->IsDormant()
				|| !pEntity->IsAlive())
				continue;

			ResolverSetup::Resolve(pEntity);
		}
	}
}