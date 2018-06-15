#include "Loadout.h"

void applyInvisibility()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!IsValidPTR(pGameContext))
		return;

	ClientPlayerManager* pPlayerManager = pGameContext->m_pPlayerManager;
	if (!IsValidPTR(pPlayerManager))
		return;

	ClientPlayer* pLocalPlayer = pPlayerManager->m_pLocalPlayer;
	if (!IsValidPTR(pLocalPlayer))
		return;

	ResourceManager* rManager = ResourceManager::GetInstance();
	UnlockAssetBase* camoUnlockAsset;
	UnlockAssetBase* noCamoAsset;

	for (int i = 0; i < 50; i++)
	{
		if (!IsValidPTR(rManager))
			continue;

		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!pComp)
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			if (!strcmp(pComp->m_objects[x]->getType()->m_infoData->name, "CamoUnlockAsset"))
			{
				camoUnlockAsset = (UnlockAssetBase*)pComp->m_objects[x];

				if (!strcmp(camoUnlockAsset->m_name, "Gameplay/Camo/U_NoCamo"))
				{
					noCamoAsset = (UnlockAssetBase*)pComp->m_objects[x];
				}
			}
		}
	}

	for (int i = 0; i < 50; i++)
	{
		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!IsValidPTR(pComp))
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			ITypedObject* typedObj = pComp->m_objects.at(x);
			TeamData* pTeamData = (TeamData*)typedObj;

			if (!strcmp(typedObj->getType()->m_infoData->name, "TeamData") &&
				(!strcmp(pTeamData->m_name, "Gameplay/Teams/US_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/RU_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/Ch_Large")))
			{
				Array<VeniceSoldierCustomizationAsset*> pSoldier = pTeamData->m_soldierCustomization;
				if (!pSoldier.m_firstElement)
					continue;

				VeniceSoldierCustomizationAsset *pAssault, *pEngineer, *pSupport, *pRecon;
				pAssault = pSoldier[0];
				pEngineer = pSoldier[1];
				pSupport = pSoldier[2];
				pRecon = pSoldier[3];

				if (!pAssault || !pEngineer || !pSupport || !pRecon)
					continue;
				/*ClientGameContext* pGameContext = ClientGameContext::GetInstance();
				if (!pGameContext)
				return;

				ClientPlayerManager* pPlayerMgr = pGameContext->m_pPlayerManager;
				if (!pPlayerMgr)
				return;

				ClientPlayer* localPlyr = pPlayerMgr->m_pLocalPlayer;
				if (!localPlyr)
				return;

				VeniceSoldierCustomizationAsset* pCustomizationAsset = localPlyr->m_pCustomization;
				if (!pCustomizationAsset)
				return;*/

				CustomizationTable* assaultAppearance = pAssault->m_pAppearance;
				CustomizationTable* engineerAppearance = pEngineer->m_pAppearance;
				CustomizationTable* supportAppearance = pSupport->m_pAppearance;
				CustomizationTable* reconAppearance = pRecon->m_pAppearance;

				if (!assaultAppearance || !engineerAppearance || !supportAppearance || !reconAppearance)
					continue;

				Array<CustomizationUnlockParts*> assault_slots = assaultAppearance->m_unlockParts;
				Array<CustomizationUnlockParts*> engineer_slots = engineerAppearance->m_unlockParts;
				Array<CustomizationUnlockParts*> support_slots = supportAppearance->m_unlockParts;
				Array<CustomizationUnlockParts*> recon_slots = reconAppearance->m_unlockParts;
				if (!assault_slots.m_firstElement || !engineer_slots.m_firstElement || !support_slots.m_firstElement || !recon_slots.m_firstElement)
					continue;

				//printf("%p\n", customizationParts[0]); // camo slot
				//printf("%p\n", customizationParts[1]); // parachute slot

				CustomizationUnlockParts* assault_camo_slot = assault_slots[0];
				CustomizationUnlockParts* engineer_camo_slot = engineer_slots[0];
				CustomizationUnlockParts* support_camo_slot = support_slots[0];
				CustomizationUnlockParts* recon_camo_slot = recon_slots[0];

				if (!assault_camo_slot || !engineer_camo_slot || !support_camo_slot || !recon_camo_slot)
					continue;

				*(assault_camo_slot->m_SelectableUnlocks.m_firstElement + 0x8) = (DWORD_PTR)noCamoAsset;
				*(engineer_camo_slot->m_SelectableUnlocks.m_firstElement + 0x8) = (DWORD_PTR)noCamoAsset;
				*(support_camo_slot->m_SelectableUnlocks.m_firstElement + 0x8) = (DWORD_PTR)noCamoAsset;
				*(recon_camo_slot->m_SelectableUnlocks.m_firstElement + 0x8) = (DWORD_PTR)noCamoAsset;
				//camoMenu->m_SelectableUnlocks.m_firstElement = (DWORD_PTR*)noCamoAsset;
				printf("Invisibility enabled\n");
			}
		}
	}
}

void applyIcicle()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* pPlayerManager = pGameContext->m_pPlayerManager;
	if (!pPlayerManager)
		return;

	ClientPlayer* pLocalPlayer = pPlayerManager->m_pLocalPlayer;
	if (!pLocalPlayer)
		return;

	ResourceManager* rManager = ResourceManager::GetInstance();
	SoldierWeaponUnlockAsset* weaponAsset;
	SoldierWeaponUnlockAsset* weapon_to_find;

	for (int i = 0; i < 50; i++)
	{
		if (!rManager)
			continue;

		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!pComp)
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			if (!strcmp(pComp->m_objects[x]->getType()->m_infoData->name, "SoldierWeaponUnlockAsset"))
			{
				//printf("%p\n", pComp->m_objects[x]);
				weaponAsset = (SoldierWeaponUnlockAsset*)pComp->m_objects[x];

				if (!strcmp(weaponAsset->m_name, "XP4/Gameplay/Weapons/icicle/U_Icicle"))
				{
					weapon_to_find = (SoldierWeaponUnlockAsset*)pComp->m_objects[x];
					printf("Icicle found\n");
				}
			}
		}
	}

	for (int i = 0; i < 50; i++)
	{
		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!pComp)
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			ITypedObject* typedObj = pComp->m_objects.at(x);
			TeamData* pTeamData = (TeamData*)typedObj;

			if (!strcmp(typedObj->getType()->m_infoData->name, "TeamData") &&
				(!strcmp(pTeamData->m_name, "Gameplay/Teams/US_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/RU_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/Ch_Large")))
			{
				Array<VeniceSoldierCustomizationAsset*> pSoldier = pTeamData->m_soldierCustomization;
				VeniceSoldierCustomizationAsset *pAssault, *pEngineer, *pSupport, *pRecon;
				CustomizationTable *pTableAssault, *pTableEngineer, *pTableSupport, *pTableRecon;

				if (pSoldier.m_firstElement == NULL)
					return;

				pAssault = pSoldier.At(0);
				pEngineer = pSoldier.At(1);
				pSupport = pSoldier.At(2);
				pRecon = pSoldier.At(3);

				if (!pAssault || !pEngineer || !pSupport || !pRecon)
					return;

				pTableAssault = pAssault->m_weaponTable;
				pTableEngineer = pEngineer->m_weaponTable;
				pTableSupport = pSupport->m_weaponTable;
				pTableRecon = pRecon->m_weaponTable;

				if (!pTableAssault || !pTableEngineer || !pTableSupport || !pTableRecon)
					return;

				if (pTableAssault->m_unlockParts.m_firstElement == NULL || pTableEngineer->m_unlockParts.m_firstElement == NULL ||
					pTableSupport->m_unlockParts.m_firstElement == NULL || pTableRecon->m_unlockParts.m_firstElement == NULL)
					return;

				Array<CustomizationUnlockParts*> assaultUnlockSlots = pTableAssault->m_unlockParts;
				Array<CustomizationUnlockParts*> engineerUnlockSlots = pTableEngineer->m_unlockParts;
				Array<CustomizationUnlockParts*> supportUnlockSlots = pTableSupport->m_unlockParts;
				Array<CustomizationUnlockParts*> reconUnlockSlots = pTableRecon->m_unlockParts;
				if (assaultUnlockSlots.m_firstElement == NULL || engineerUnlockSlots.m_firstElement == NULL ||
					supportUnlockSlots.m_firstElement == NULL || reconUnlockSlots.m_firstElement == NULL)
					return;

				CustomizationUnlockParts* assault_knife = assaultUnlockSlots.At(7);
				CustomizationUnlockParts* engineer_knife = engineerUnlockSlots.At(7);
				CustomizationUnlockParts* support_knife = supportUnlockSlots.At(7);
				CustomizationUnlockParts* recon_knife = reconUnlockSlots.At(7);

				if (!assault_knife || !engineer_knife || !support_knife || !recon_knife)
					return;

				*(assault_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)weapon_to_find;
				*(engineer_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)weapon_to_find;
				*(support_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)weapon_to_find;
				*(recon_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)weapon_to_find;
				printf("Icicle enabled in Knife Slot\n", pTeamData->m_name);
			}
		}
	}
}

void applyRAWR()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* pPlayerManager = pGameContext->m_pPlayerManager;
	if (!pPlayerManager)
		return;

	ClientPlayer* pLocalPlayer = pPlayerManager->m_pLocalPlayer;
	if (!pLocalPlayer)
		return;

	UnlockAssetBase* pRAWR = GetAssetByName("SoldierWeaponUnlockAsset", "XP3/Gameplay/Vehicles/MAARS_Robot/U_MAARSBot");
	ResourceManager* rManager = ResourceManager::GetInstance();

	if (!pRAWR) return;

	for (int i = 0; i < 50; i++)
	{
		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!pComp)
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			ITypedObject* typedObj = pComp->m_objects.at(x);
			TeamData* pTeamData = (TeamData*)typedObj;

			if (!strcmp(typedObj->getType()->m_infoData->name, "TeamData") &&
				(!strcmp(pTeamData->m_name, "Gameplay/Teams/US_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/RU_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/Ch_Large")))
			{
				Array<VeniceSoldierCustomizationAsset*> pSoldier = pTeamData->m_soldierCustomization;
				VeniceSoldierCustomizationAsset *pAssault, *pEngineer, *pSupport, *pRecon;
				CustomizationTable *pTableAssault, *pTableEngineer, *pTableSupport, *pTableRecon;

				if (pSoldier.m_firstElement == NULL)
					return;

				pAssault = pSoldier.At(0);
				pEngineer = pSoldier.At(1);
				pSupport = pSoldier.At(2);
				pRecon = pSoldier.At(3);

				if (!pAssault || !pEngineer || !pSupport || !pRecon)
					return;

				pTableAssault = pAssault->m_weaponTable;
				pTableEngineer = pEngineer->m_weaponTable;
				pTableSupport = pSupport->m_weaponTable;
				pTableRecon = pRecon->m_weaponTable;

				if (!pTableAssault || !pTableEngineer || !pTableSupport || !pTableRecon)
					return;

				if (pTableAssault->m_unlockParts.m_firstElement == NULL || pTableEngineer->m_unlockParts.m_firstElement == NULL ||
					pTableSupport->m_unlockParts.m_firstElement == NULL || pTableRecon->m_unlockParts.m_firstElement == NULL)
					return;

				Array<CustomizationUnlockParts*> assaultUnlockSlots = pTableAssault->m_unlockParts;
				Array<CustomizationUnlockParts*> engineerUnlockSlots = pTableEngineer->m_unlockParts;
				Array<CustomizationUnlockParts*> supportUnlockSlots = pTableSupport->m_unlockParts;
				Array<CustomizationUnlockParts*> reconUnlockSlots = pTableRecon->m_unlockParts;
				if (assaultUnlockSlots.m_firstElement == NULL || engineerUnlockSlots.m_firstElement == NULL ||
					supportUnlockSlots.m_firstElement == NULL || reconUnlockSlots.m_firstElement == NULL)
					return;

				CustomizationUnlockParts* assault_gadget_1 = assaultUnlockSlots.At(2);
				CustomizationUnlockParts* assault_gadget_2 = assaultUnlockSlots.At(5);
				CustomizationUnlockParts* assault_grenade = assaultUnlockSlots.At(6);
				CustomizationUnlockParts* assault_knife = assaultUnlockSlots.At(7);

				CustomizationUnlockParts* engineer_gadget_1 = engineerUnlockSlots.At(2);
				CustomizationUnlockParts* engineer_gadget_2 = engineerUnlockSlots.At(5);
				CustomizationUnlockParts* engineer_grenade = engineerUnlockSlots.At(6);
				CustomizationUnlockParts* engineer_knife = engineerUnlockSlots.At(7);

				CustomizationUnlockParts* support_gadget_1 = supportUnlockSlots.At(2);
				CustomizationUnlockParts* support_gadget_2 = supportUnlockSlots.At(5);
				CustomizationUnlockParts* support_grenade = supportUnlockSlots.At(6);
				CustomizationUnlockParts* support_knife = supportUnlockSlots.At(7);

				CustomizationUnlockParts* recon_gadget_1 = reconUnlockSlots.At(2);
				CustomizationUnlockParts* recon_gadget_2 = reconUnlockSlots.At(5);
				CustomizationUnlockParts* recon_grenade = reconUnlockSlots.At(6);
				CustomizationUnlockParts* recon_knife = reconUnlockSlots.At(7);

				if (!assault_gadget_1 || !assault_gadget_2 || !assault_grenade || !assault_knife ||
					!engineer_gadget_1 || !engineer_gadget_2 || !engineer_grenade || !engineer_knife ||
					!support_gadget_1 || !support_gadget_2 || !support_grenade || !support_knife ||
					!recon_gadget_1 || !recon_gadget_2 || !recon_grenade || !recon_knife)
					return;

				// swap the first slot in gadget 1 with a RAWR	
				*(assault_gadget_2->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;
				*(assault_grenade->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;
				*(assault_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;

				*(engineer_gadget_2->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;
				*(engineer_grenade->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;
				*(engineer_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;

				*(support_gadget_2->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;
				*(support_grenade->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;
				*(support_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;

				*(recon_gadget_2->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;
				*(recon_grenade->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;
				*(recon_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pRAWR;
			}
		}
	}
	printf("RAWR enabled\n");
}

void applyRorsche()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* pPlayerManager = pGameContext->m_pPlayerManager;
	if (!pPlayerManager)
		return;

	ClientPlayer* pLocalPlayer = pPlayerManager->m_pLocalPlayer;
	if (!pLocalPlayer)
		return;

	UnlockAssetBase* pUnlock = GetAssetByName("SoldierWeaponUnlockAsset", "XP4/Gameplay/Weapons/Railgun/U_Railgun");
	UnlockAssetBase* pPrimaryWep = GetAssetByName("SoldierWeaponUnlockAsset", "Gameplay/Weapons/Type88/spec/U_Type88_SP_Squad");
	UnlockAssetBase* pAT4 = GetAssetByName("SoldierWeaponUnlockAsset", "Gameplay/Gadgets/AT4/U_SP_AT4");
	ResourceManager* rManager = ResourceManager::GetInstance();

	if (!pUnlock) return;

	for (int i = 0; i < 50; i++)
	{
		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!pComp)
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			ITypedObject* typedObj = pComp->m_objects.at(x);
			TeamData* pTeamData = (TeamData*)typedObj;

			if (!strcmp(typedObj->getType()->m_infoData->name, "TeamData") &&
				(!strcmp(pTeamData->m_name, "Gameplay/Teams/US_Large") ||
					!strcmp(pTeamData->m_name, "Gameplay/Teams/RU_Large") ||
					!strcmp(pTeamData->m_name, "Gameplay/Teams/Ch_Large")))
			{
				Array<VeniceSoldierCustomizationAsset*> pSoldier = pTeamData->m_soldierCustomization;
				VeniceSoldierCustomizationAsset *pAssault, *pEngineer, *pSupport, *pRecon;
				CustomizationTable *pTableAssault, *pTableEngineer, *pTableSupport, *pTableRecon;

				if (pSoldier.m_firstElement == NULL)
					return;

				pAssault = pSoldier.At(0);
				pEngineer = pSoldier.At(1);
				pSupport = pSoldier.At(2);
				pRecon = pSoldier.At(3);

				if (!pAssault || !pEngineer || !pSupport || !pRecon)
					return;

				pTableAssault = pAssault->m_weaponTable;
				pTableEngineer = pEngineer->m_weaponTable;
				pTableSupport = pSupport->m_weaponTable;
				pTableRecon = pRecon->m_weaponTable;

				if (!pTableAssault || !pTableEngineer || !pTableSupport || !pTableRecon)
					return;

				if (pTableAssault->m_unlockParts.m_firstElement == NULL || pTableEngineer->m_unlockParts.m_firstElement == NULL ||
					pTableSupport->m_unlockParts.m_firstElement == NULL || pTableRecon->m_unlockParts.m_firstElement == NULL)
					return;

				Array<CustomizationUnlockParts*> assaultUnlockSlots = pTableAssault->m_unlockParts;
				Array<CustomizationUnlockParts*> engineerUnlockSlots = pTableEngineer->m_unlockParts;
				Array<CustomizationUnlockParts*> supportUnlockSlots = pTableSupport->m_unlockParts;
				Array<CustomizationUnlockParts*> reconUnlockSlots = pTableRecon->m_unlockParts;
				if (assaultUnlockSlots.m_firstElement == NULL || engineerUnlockSlots.m_firstElement == NULL ||
					supportUnlockSlots.m_firstElement == NULL || reconUnlockSlots.m_firstElement == NULL)
					return;

				CustomizationUnlockParts* assault_primary = assaultUnlockSlots.At(0);
				CustomizationUnlockParts* assault_secondary = assaultUnlockSlots.At(1);
				CustomizationUnlockParts* assault_gadget_1 = assaultUnlockSlots.At(2);
				CustomizationUnlockParts* assault_gadget_2 = assaultUnlockSlots.At(5);
				CustomizationUnlockParts* assault_grenade = assaultUnlockSlots.At(6);
				CustomizationUnlockParts* assault_knife = assaultUnlockSlots.At(7);

				CustomizationUnlockParts* engineer_primary = engineerUnlockSlots.At(0);
				CustomizationUnlockParts* engineer_secondary = engineerUnlockSlots.At(1);
				CustomizationUnlockParts* engineer_gadget_1 = engineerUnlockSlots.At(2);
				CustomizationUnlockParts* engineer_gadget_2 = engineerUnlockSlots.At(5);
				CustomizationUnlockParts* engineer_grenade = engineerUnlockSlots.At(6);
				CustomizationUnlockParts* engineer_knife = engineerUnlockSlots.At(7);

				CustomizationUnlockParts* support_primary = supportUnlockSlots.At(0);
				CustomizationUnlockParts* support_secondary = supportUnlockSlots.At(1);
				CustomizationUnlockParts* support_gadget_1 = supportUnlockSlots.At(2);
				CustomizationUnlockParts* support_gadget_2 = supportUnlockSlots.At(5);
				CustomizationUnlockParts* support_grenade = supportUnlockSlots.At(6);
				CustomizationUnlockParts* support_knife = supportUnlockSlots.At(7);

				CustomizationUnlockParts* recon_primary = reconUnlockSlots.At(0);
				CustomizationUnlockParts* recon_secondary = reconUnlockSlots.At(1);
				CustomizationUnlockParts* recon_gadget_1 = reconUnlockSlots.At(2);
				CustomizationUnlockParts* recon_gadget_2 = reconUnlockSlots.At(5);
				CustomizationUnlockParts* recon_grenade = reconUnlockSlots.At(6);
				CustomizationUnlockParts* recon_knife = reconUnlockSlots.At(7);

				if (!assault_gadget_1 || !assault_gadget_2 || !assault_grenade || !assault_knife ||
					!engineer_gadget_1 || !engineer_gadget_2 || !engineer_grenade || !engineer_knife ||
					!support_gadget_1 || !support_gadget_2 || !support_grenade || !support_knife ||
					!recon_gadget_1 || !recon_gadget_2 || !recon_grenade || !recon_knife)
					return;

				*(assault_primary->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pPrimaryWep;
				*(engineer_primary->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pPrimaryWep;
				*(support_primary->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pPrimaryWep;
				*(recon_primary->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pPrimaryWep;
				*(engineer_gadget_1->m_SelectableUnlocks.m_firstElement) = reinterpret_cast<DWORD_PTR>(pAT4);

				*(assault_secondary->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pPrimaryWep;
				*(engineer_secondary->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pPrimaryWep;
				*(support_secondary->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pPrimaryWep;
				*(recon_secondary->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pPrimaryWep;

				// swap the first slot in gadget 1 with a railgun				
				*(assault_grenade->m_SelectableUnlocks.m_firstElement + 1) = (DWORD_PTR)pUnlock;
				*(assault_knife->m_SelectableUnlocks.m_firstElement + 1) = (DWORD_PTR)pUnlock;

				*(engineer_grenade->m_SelectableUnlocks.m_firstElement + 1) = (DWORD_PTR)pUnlock;
				*(engineer_knife->m_SelectableUnlocks.m_firstElement + 1) = (DWORD_PTR)pUnlock;

				*(support_grenade->m_SelectableUnlocks.m_firstElement + 1) = (DWORD_PTR)pUnlock;
				*(support_knife->m_SelectableUnlocks.m_firstElement + 1) = (DWORD_PTR)pUnlock;

				*(recon_grenade->m_SelectableUnlocks.m_firstElement + 1) = (DWORD_PTR)pUnlock;
				*(recon_knife->m_SelectableUnlocks.m_firstElement + 1) = (DWORD_PTR)pUnlock;
			}
		}
	}
	printf("Railgun and TYPE 88 enabled\n");
}

void applyMKV()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* pPlayerManager = pGameContext->m_pPlayerManager;
	if (!pPlayerManager)
		return;

	ClientPlayer* pLocalPlayer = pPlayerManager->m_pLocalPlayer;
	if (!pLocalPlayer)
		return;

	UnlockAssetBase* pUnlock = GetAssetByName("SoldierWeaponUnlockAsset", "XP4/Gameplay/Gadgets/MKV/U_MKV");
	ResourceManager* rManager = ResourceManager::GetInstance();

	if (!pUnlock) return;

	for (int i = 0; i < 50; i++)
	{
		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!pComp)
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			ITypedObject* typedObj = pComp->m_objects.at(x);
			TeamData* pTeamData = (TeamData*)typedObj;

			if (!strcmp(typedObj->getType()->m_infoData->name, "TeamData") &&
				(!strcmp(pTeamData->m_name, "Gameplay/Teams/US_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/RU_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/Ch_Large")))
			{
				Array<VeniceSoldierCustomizationAsset*> pSoldier = pTeamData->m_soldierCustomization;
				VeniceSoldierCustomizationAsset *pAssault, *pEngineer, *pSupport, *pRecon;
				CustomizationTable *pTableAssault, *pTableEngineer, *pTableSupport, *pTableRecon;

				if (pSoldier.m_firstElement == NULL)
					return;

				pAssault = pSoldier.At(0);
				pEngineer = pSoldier.At(1);
				pSupport = pSoldier.At(2);
				pRecon = pSoldier.At(3);

				if (!pAssault || !pEngineer || !pSupport || !pRecon)
					return;

				pTableAssault = pAssault->m_weaponTable;
				pTableEngineer = pEngineer->m_weaponTable;
				pTableSupport = pSupport->m_weaponTable;
				pTableRecon = pRecon->m_weaponTable;

				if (!pTableAssault || !pTableEngineer || !pTableSupport || !pTableRecon)
					return;

				if (pTableAssault->m_unlockParts.m_firstElement == NULL || pTableEngineer->m_unlockParts.m_firstElement == NULL ||
					pTableSupport->m_unlockParts.m_firstElement == NULL || pTableRecon->m_unlockParts.m_firstElement == NULL)
					return;

				Array<CustomizationUnlockParts*> assaultUnlockSlots = pTableAssault->m_unlockParts;
				Array<CustomizationUnlockParts*> engineerUnlockSlots = pTableEngineer->m_unlockParts;
				Array<CustomizationUnlockParts*> supportUnlockSlots = pTableSupport->m_unlockParts;
				Array<CustomizationUnlockParts*> reconUnlockSlots = pTableRecon->m_unlockParts;
				if (assaultUnlockSlots.m_firstElement == NULL || engineerUnlockSlots.m_firstElement == NULL ||
					supportUnlockSlots.m_firstElement == NULL || reconUnlockSlots.m_firstElement == NULL)
					return;

				CustomizationUnlockParts* assault_gadget_1 = assaultUnlockSlots.At(2);
				CustomizationUnlockParts* assault_gadget_2 = assaultUnlockSlots.At(5);
				CustomizationUnlockParts* assault_grenade = assaultUnlockSlots.At(6);
				CustomizationUnlockParts* assault_knife = assaultUnlockSlots.At(7);

				CustomizationUnlockParts* engineer_gadget_1 = engineerUnlockSlots.At(2);
				CustomizationUnlockParts* engineer_gadget_2 = engineerUnlockSlots.At(5);
				CustomizationUnlockParts* engineer_grenade = engineerUnlockSlots.At(6);
				CustomizationUnlockParts* engineer_knife = engineerUnlockSlots.At(7);

				CustomizationUnlockParts* support_gadget_1 = supportUnlockSlots.At(2);
				CustomizationUnlockParts* support_gadget_2 = supportUnlockSlots.At(5);
				CustomizationUnlockParts* support_grenade = supportUnlockSlots.At(6);
				CustomizationUnlockParts* support_knife = supportUnlockSlots.At(7);

				CustomizationUnlockParts* recon_gadget_1 = reconUnlockSlots.At(2);
				CustomizationUnlockParts* recon_gadget_2 = reconUnlockSlots.At(5);
				CustomizationUnlockParts* recon_grenade = reconUnlockSlots.At(6);
				CustomizationUnlockParts* recon_knife = reconUnlockSlots.At(7);

				if (!assault_gadget_1 || !assault_gadget_2 || !assault_grenade || !assault_knife ||
					!engineer_gadget_1 || !engineer_gadget_2 || !engineer_grenade || !engineer_knife ||
					!support_gadget_1 || !support_gadget_2 || !support_grenade || !support_knife ||
					!recon_gadget_1 || !recon_gadget_2 || !recon_grenade || !recon_knife)
					return;

				*(assault_gadget_2->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;
				*(assault_grenade->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;
				*(assault_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;

				*(engineer_gadget_2->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;
				*(engineer_grenade->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;
				*(engineer_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;

				*(support_gadget_2->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;
				*(support_grenade->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;
				*(support_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;

				*(recon_gadget_2->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;
				*(recon_grenade->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;
				*(recon_knife->m_SelectableUnlocks.m_firstElement) = (DWORD_PTR)pUnlock;
			}
		}
	}
	printf("XD-1 enabled\n");

}

void applyStealthJet()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* pPlayerManager = pGameContext->m_pPlayerManager;
	if (!pPlayerManager)
		return;

	ClientPlayer* pLocalPlayer = pPlayerManager->m_pLocalPlayer;
	if (!pLocalPlayer)
		return;

	printf("Please wait...\n");
	ResourceManager* rManager = ResourceManager::GetInstance();

	for (int i = 0; i < 50; i++)
	{
		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!pComp)
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			ITypedObject* typedObj = pComp->m_objects.at(x);
			TeamData* pTeamData = (TeamData*)typedObj;

			if (!strcmp(typedObj->getType()->m_infoData->name, "TeamData") &&
				(!strcmp(pTeamData->m_name, "Gameplay/Teams/US_Large") ||
					!strcmp(pTeamData->m_name, "Gameplay/Teams/RU_Large") ||
					!strcmp(pTeamData->m_name, "Gameplay/Teams/Ch_Large")))
			{
				Array<VehicleCustomizationAsset*> pVehicles;
				VehicleCustomizationAsset* JET;
				CustomizationTable* pTable;

				pVehicles = pTeamData->m_VehicleCustomization;
				if (pVehicles.m_firstElement == NULL)
					return;

				JET = pVehicles.At(4); // JET index
				if (!JET)
					return;

				pTable = JET->m_Customization;
				if (!pTable)
					return;

				if (pTable->m_unlockParts.m_firstElement == NULL)
					return;

				Array<CustomizationUnlockParts*> pUnlockParts = pTable->m_unlockParts;
				if (pUnlockParts.m_firstElement == NULL)
					return;

				CustomizationUnlockParts* primary_slot = pUnlockParts.At(0);
				CustomizationUnlockParts* secondary_slot = pUnlockParts.At(1);
				CustomizationUnlockParts* paint_slot = pUnlockParts.At(4);

				if (!primary_slot || !secondary_slot || !paint_slot)
					return;

				auto twenty_mm = primary_slot->m_SelectableUnlocks[0];
				auto twentyfive_mm = primary_slot->m_SelectableUnlocks[1];
				auto thirty_mm = primary_slot->m_SelectableUnlocks[2];
				*(secondary_slot->m_SelectableUnlocks.m_firstElement) = twentyfive_mm;
				*(paint_slot->m_SelectableUnlocks.m_firstElement) = thirty_mm;

				printf("JET upgrade enabled for Team: %s\n", pTeamData->m_name);
			}
		}
	}
}

void applyMBT()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* pPlayerManager = pGameContext->m_pPlayerManager;
	if (!pPlayerManager)
		return;

	ClientPlayer* pLocalPlayer = pPlayerManager->m_pLocalPlayer;
	if (!pLocalPlayer)
		return;

	ResourceManager* rManager = ResourceManager::GetInstance();
	for (int i = 0; i < 50; i++)
	{
		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!pComp)
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			ITypedObject* typedObj = pComp->m_objects.at(x);
			TeamData* pTeamData = (TeamData*)typedObj;

			if (!strcmp(typedObj->getType()->m_infoData->name, "TeamData") &&
				(!strcmp(pTeamData->m_name, "Gameplay/Teams/US_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/RU_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/Ch_Large")))
			{
				Array<VehicleCustomizationAsset*> pVehicles;
				VehicleCustomizationAsset* MBT;
				CustomizationTable* pTable;

				pVehicles = pTeamData->m_VehicleCustomization;
				if (pVehicles.m_firstElement == NULL)
					return;

				MBT = pVehicles.At(0); // MBT index
				if (!MBT)
					return;

				pTable = MBT->m_Customization;
				if (!pTable)
					return;

				if (pTable->m_unlockParts.m_firstElement == NULL)
					return;

				Array<CustomizationUnlockParts*> pUnlockParts = pTable->m_unlockParts;
				if (pUnlockParts.m_firstElement == NULL)
					return;

				CustomizationUnlockParts* primary_slot = pUnlockParts.At(0);
				CustomizationUnlockParts* secondary_slot = pUnlockParts.At(1);
				//CustomizationUnlockParts* optics_slot = pUnlockParts.At(3); // can't touch, won't allow 1st person camera
				CustomizationUnlockParts* upgrade_slot = pUnlockParts.At(4);
				CustomizationUnlockParts* paint_slot = pUnlockParts.At(5);
				CustomizationUnlockParts* gunner_upgrade = pUnlockParts.At(7);

				if (!primary_slot || !secondary_slot || !upgrade_slot || !paint_slot || !gunner_upgrade)
					return;

				// swap the secondary slot unlock with a primary unlock
				auto apShell = primary_slot->m_SelectableUnlocks[0];
				auto heShell = primary_slot->m_SelectableUnlocks[1];
				auto sabotShell = primary_slot->m_SelectableUnlocks[2];

				auto coaxLMG = secondary_slot->m_SelectableUnlocks[0];
				auto coaxHMG = secondary_slot->m_SelectableUnlocks[2];

				*(paint_slot->m_SelectableUnlocks.m_firstElement) = heShell;
				*(upgrade_slot->m_SelectableUnlocks.m_firstElement) = sabotShell;
				*(gunner_upgrade->m_SelectableUnlocks.m_firstElement) = coaxHMG;

				printf("MBT upgrade enabled for Team: %s\n", pTeamData->m_name);
			}
		}
	}
}

void applyIFV()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* pPlayerManager = pGameContext->m_pPlayerManager;
	if (!pPlayerManager)
		return;

	ClientPlayer* pLocalPlayer = pPlayerManager->m_pLocalPlayer;
	if (!pLocalPlayer)
		return;

	ResourceManager* rManager = ResourceManager::GetInstance();
	bool skip = true;

	for (int i = 0; i < 50; i++)
	{
		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!pComp)
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			ITypedObject* typedObj = pComp->m_objects.at(x);

			/*if(skip)
			{
			skip = false;
			continue;
			}*/

			TeamData* pTeamData = (TeamData*)typedObj;

			if (!strcmp(typedObj->getType()->m_infoData->name, "TeamData") &&
				(!strcmp(pTeamData->m_name, "Gameplay/Teams/US_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/RU_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/Ch_Large")))
			{
				Array<VehicleCustomizationAsset*> pVehicles;
				VehicleCustomizationAsset* IFV;
				CustomizationTable* pTable;

				pVehicles = pTeamData->m_VehicleCustomization;
				if (pVehicles.m_firstElement == NULL)
					return;

				IFV = pVehicles.At(1); // MBT index
				if (!IFV)
					return;

				pTable = IFV->m_Customization;
				if (!pTable)
					return;

				if (pTable->m_unlockParts.m_firstElement == NULL)
					return;

				Array<CustomizationUnlockParts*> pUnlockParts = pTable->m_unlockParts;
				if (pUnlockParts.m_firstElement == NULL)
					return;

				CustomizationUnlockParts* primary_slot = pUnlockParts.At(0);
				CustomizationUnlockParts* paint_slot = pUnlockParts.At(5);

				if (!primary_slot || !paint_slot)
					return;

				// swap the secondary slot unlock with a primary unlock
				auto HE_SHELL = primary_slot->m_SelectableUnlocks[0];
				auto AP_SHELL = primary_slot->m_SelectableUnlocks[1];
				*(paint_slot->m_SelectableUnlocks.m_firstElement) = AP_SHELL;

				printf("LAV upgrade enabled for Team: %s\n", pTeamData->m_name);
			}
		}
	}
}

void applyMobileAA()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* pPlayerManager = pGameContext->m_pPlayerManager;
	if (!pPlayerManager)
		return;

	ClientPlayer* pLocalPlayer = pPlayerManager->m_pLocalPlayer;
	if (!pLocalPlayer)
		return;

	ResourceManager* rManager = ResourceManager::GetInstance();
	bool skip = true;

	for (int i = 0; i < 50; i++)
	{
		ResourceManager::Compartment* pComp = rManager->m_compartments[i];
		if (!pComp)
			continue;

		for (int x = 0; x < pComp->m_objects.size(); x++)
		{
			ITypedObject* typedObj = pComp->m_objects.at(x);

			/*if(skip)
			{
			skip = false;
			continue;
			}*/

			TeamData* pTeamData = (TeamData*)typedObj;

			if (!strcmp(typedObj->getType()->m_infoData->name, "TeamData") &&
				(!strcmp(pTeamData->m_name, "Gameplay/Teams/US_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/RU_Large") || !strcmp(pTeamData->m_name, "Gameplay/Teams/Ch_Large")))
			{
				Array<VehicleCustomizationAsset*> pVehicles;
				VehicleCustomizationAsset* AA;
				CustomizationTable* pTable;

				pVehicles = pTeamData->m_VehicleCustomization;
				if (pVehicles.m_firstElement == NULL)
					return;

				AA = pVehicles.At(2); // MBT index
				if (!AA)
					return;

				pTable = AA->m_Customization;
				if (!pTable)
					return;

				if (pTable->m_unlockParts.m_firstElement == NULL)
					return;

				Array<CustomizationUnlockParts*> pUnlockParts = pTable->m_unlockParts;
				if (pUnlockParts.m_firstElement == NULL)
					return;

				CustomizationUnlockParts* primary_slot = pUnlockParts.At(0);
				CustomizationUnlockParts* paint_slot = pUnlockParts.At(5);

				if (!primary_slot || !paint_slot)
					return;

				// swap the secondary slot unlock with a primary unlock
				auto cannon25 = primary_slot->m_SelectableUnlocks[0];
				auto cannon30 = primary_slot->m_SelectableUnlocks[1];
				*(paint_slot->m_SelectableUnlocks.m_firstElement) = cannon30;

				printf("AA upgrade enabled for Team: %s\n", pTeamData->m_name);
			}
		}
	}
}
