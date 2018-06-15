#include "DamageStream.h"

void magicBullet(int killfeedmode)
{
	UnlockAssetBase* pWeapon;

	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!IsValidPTR(pGameContext))
		return;

	ClientPlayerManager* pPlayerMgr = pGameContext->m_pPlayerManager;
	if (!IsValidPTR(pPlayerMgr))
		return;

	ClientPlayer* pLocalPlyr = pPlayerMgr->m_pLocalPlayer;
	if (!IsValidPTR(pLocalPlyr))
		return;

	ClientSoldierEntity* pLocalSoldier = pLocalPlyr->m_soldier.GetData();
	if (!IsValidPTR(pLocalSoldier))
		return;

	ClientSoldierEntity* pControllable = pLocalPlyr->m_pControlledControllable;
	if (!IsValidPTR(pControllable))
		return;

	if (!stricmp(pControllable->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity"))
	{
		ClientSoldierWeapon* pSoldierWep = ClientSoldierWeapon::GetInstance();
		if (!IsValidPTR(pSoldierWep))
			return;

		ClientWeapon* pClientWep = pSoldierWep->m_pWeapon;
		if (!IsValidPTR(pClientWep))
			return;

		WeaponModifier* pWepModifier = pClientWep->m_pModifier;
		if (!IsValidPTR(pWepModifier))
			return;

		UnlockAssetBase* pUnlockAsset = pWepModifier->m_weaponUnlockAsset;
		if (!IsValidPTR(pUnlockAsset))
			return;

		if (killfeedmode == 0)
			pWeapon = 0;
		else
			pWeapon = pUnlockAsset;
	}

	else if (!stricmp(pControllable->GetTypeInfo()->m_infoData->name, "ClientVehicleEntity"))
	{
		pWeapon = 0;
	}

	ClientSoldierPrediction* pPred = pLocalSoldier->m_pPredictedController;
	if (!IsValidPTR(pPred))
		return;

	D3DXMATRIX invMat = GameRenderer::GetInstance()->m_pRenderView->m_viewMatrixInverse;
	D3DXVECTOR3 forward = D3DXVECTOR3(-invMat._31, -invMat._32, -invMat._33);
	D3DXVECTOR3 pos = invMat.m[3];

	ClientPlayer* pPlayer = getClosestTargetbyCrosshair(false, 5);
	if (!IsValidPTR(pPlayer))
		return;

	OnlineManager* pOnlineMngr = pGameContext->m_pOnlineManager;
	if (!IsValidPTR(pOnlineMngr))
		return;

	ClientConnection* pClientConnection = pOnlineMngr->m_pClientConnection;
	if (!IsValidPTR(pClientConnection))
		return;

	ClientDamageStream* pDamageStream = pClientConnection->m_pDamageStream;
	if (!pDamageStream)
		return;

	DamageData dmgData;

	void(__fastcall* const initializeDamageData)(DamageData* pDamageData) = (void(__fastcall* const)(DamageData*)) (0x1409A0EE0);
	initializeDamageData(&dmgData);

	DamageData* const pDamageData = &dmgData;

	/*if (*(DWORD64*)pDamageData != 0x141BED5E0)
	{
	*(DWORD64*)pDamageData = 0x141BED5E0;
	*(DWORD64*)(pDamageData + 0xC0) = 0x141BED5E0;
	*(DWORD64*)(pDamageData + 8) = 0x14242AB10;
	*(DWORD64*)(pDamageData + 0x10) = 0x141A37C18;
	*(DWORD64*)(pDamageData + 0xD0) = 0x141A37C18;
	}*/

	/*typedef void(*sendDamage)(ClientConnection* pConnection, DamageData* pDamageData);
	sendDamage g__sendDamage = (sendDamage)(0x140885DA0);*/

	void(*const addDamage) (ClientConnection*, DamageData*) = (void(*) (ClientConnection*, DamageData*))(OFFSET_ADDDAMAGE);

	int r;
	r = rand() % 6;
	if (r == 1)
		r = 0;

	pDamageData->m_VictimPlayerID = pPlayer->getId();
	pDamageData->m_VictimInstanceID = 0;
	pDamageData->m_InflictorPlayerID = pLocalPlyr->getId();
	pDamageData->HitReactionType = r; // headshot if 1	
	pDamageData->m_unlockAsset = pWeapon;
	pDamageData->m_damage = 110.0f;
	pDamageData->m_ClientAIKilled = 0; // *(BYTE*)(pPlayer + 0x13C8); has to be 0

	addDamage(pClientConnection, pDamageData);
}

void autoHeal()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!IsValidPTR(pGameContext))
		return;

	ClientPlayerManager* pPlayerMgr = pGameContext->m_pPlayerManager;
	if (!IsValidPTR(pPlayerMgr))
		return;

	ClientPlayer* pLocalPlyr = pPlayerMgr->m_pLocalPlayer;
	if (!IsValidPTR(pLocalPlyr))
		return;

	if (pLocalPlyr->InVehicle())
		return;

	ClientSoldierEntity* pLocalSoldier = pLocalPlyr->m_pControlledControllable;
	if (!IsValidPTR(pLocalSoldier))
		return;

	if (stricmp(pLocalSoldier->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
		return;

	OnlineManager* pOnlineMngr = pGameContext->m_pOnlineManager;
	if (!IsValidPTR(pOnlineMngr))
		return;

	ClientConnection* pv13 = pOnlineMngr->m_pClientConnection;
	if (!IsValidPTR(pv13))
		return;

	ClientDamageStream* pv14 = pv13->m_pDamageStream;
	if (!IsValidPTR(pv14))
		return;

	DamageData dmgData;

	void(__fastcall* const initializeDamageData)(DamageData* pDamageData) = (void(__fastcall*)(DamageData*)) (0x1409A0EE0);
	initializeDamageData(&dmgData);

	DamageData* const pDamageData = &dmgData;

	/*if (*(DWORD64*)pDamageData != 0x141BED5E0)
	{
	*(DWORD64*)pDamageData = 0x141BED5E0;
	*(DWORD64*)(pDamageData + 0xC0) = 0x141BED5E0;
	*(DWORD64*)(pDamageData + 8) = 0x14242AB10;
	*(DWORD64*)(pDamageData + 0x10) = 0x141A37C18;
	*(DWORD64*)(pDamageData + 0xD0) = 0x141A37C18;
	}*/

	int r;
	r = rand() % 6;
	if (r == 1)
		r = 0;

	pDamageData->m_VictimInstanceID = 0;
	pDamageData->m_VictimPlayerID = pLocalPlyr->getId();
	pDamageData->m_InflictorPlayerID = pLocalPlyr->getId();
	pDamageData->HitReactionType = r;
	pDamageData->m_damage = -100.0f;
	pDamageData->m_unlockAsset = 0;
	pDamageData->m_ClientAIKilled = 0;

	void(*const addDamage)(ClientConnection*, DamageData*) = (void(*) (ClientConnection*, DamageData*)) (OFFSET_ADDDAMAGE);
	addDamage(pv13, pDamageData);
}
