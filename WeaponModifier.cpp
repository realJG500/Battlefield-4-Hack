#include "WeaponModifier.h"
#include "Function Prototypes.h"

typedef void(__fastcall* tGetRecoil)(void* thisptr, void* a2);
tGetRecoil oGetRecoil;
void __fastcall hk__GetRecoil(void* thisptr, void* a2)
{
	*(float*)(a2) = 1;
	*((float*)a2 + 1) = 0;
	*((float*)a2 + 2) = 0;
	*((float*)a2 + 3) = 0;
	*((float*)a2 + 4) = 0;
	*((float*)a2 + 5) = 1;
	*((float*)a2 + 6) = 0;
	*((float*)a2 + 7) = 0;
	*((float*)a2 + 8) = 0;
	*((float*)a2 + 9) = 0;
	*((float*)a2 + 10) = 1;
	*((float*)a2 + 11) = 0;
	*((float*)a2 + 12) = 0;
	*((float*)a2 + 13) = 0;
	*((float*)a2 + 14) = 0;
	*((float*)a2 + 15) = 1;
}

void unhookRecoil()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!IsValidPTR(pGameContext))
		return;

	ClientPlayerManager* pPlayerMgnr = pGameContext->m_pPlayerManager;
	if (!IsValidPTR(pPlayerMgnr))
		return;

	ClientPlayer* pLocalPlayer = pPlayerMgnr->m_pLocalPlayer;
	if (!IsValidPTR(pLocalPlayer))
		return;

	ClientSoldierEntity* pSoldierEntity = pLocalPlayer->m_pControlledControllable;
	if (!IsValidPTR(pSoldierEntity))
		return;

	if (stricmp(pSoldierEntity->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
		return;

	ClientSoldierWeapon* CSW = pSoldierEntity->GetActiveWeapon();
	if (!IsValidPTR(CSW))
		return;

	WeaponFiring* wepFiring = CSW->m_pPrimary;
	if (!IsValidPTR(wepFiring))
		return;

	WeaponSway* wepSway = wepFiring->m_pSway;
	if (!IsValidPTR(wepSway))
		return;

	HookVTableFunction((PDWORD64*)wepSway, orig26, 26);
	HookVTableFunction((PDWORD64*)wepSway, orig27, 27);
	HookVTableFunction((PDWORD64*)wepSway, orig28, 28);
	HookVTableFunction((PDWORD64*)wepSway, orig29, 29);
	printf("Unhooking WeaponSway vtable...\n");
	printf("...Done\n");
}

void noBreath()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* m_PlayerManager = pGameContext->m_pPlayerManager;
	if (!m_PlayerManager)
		return;

	ClientPlayer* pLocalPlayer = m_PlayerManager->m_pLocalPlayer;
	if (!pLocalPlayer)
		return;

	if (pLocalPlayer->InVehicle())
		return;

	ClientSoldierEntity* pSoldierEntity = pLocalPlayer->m_pControlledControllable;
	if (!pSoldierEntity)
		return;

	if (stricmp(pSoldierEntity->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
		return;

	ClientSoldierEntity::BreathControlHandler* pBreath = pSoldierEntity->m_breathControlHandler;
	if (!pBreath)
		return;

	pBreath->m_breathControlTimer = 0.0f;
	pBreath->m_breathControlMultiplier = 0.0f;
	pBreath->m_breathControlPenaltyTimer = 0.0f;
	pBreath->m_breathControlpenaltyMultiplier = 0.0f;
	pBreath->m_Enabled = 0.0f;
}

void noRecoil()
{
	//ClientGameContext* gameContext = ClientGameContext::GetInstance();
	//if (!gameContext)
	//	return;

	//ClientPlayerManager* playerManager = gameContext->m_pPlayerManager;
	//if (!playerManager)
	//	return;

	//ClientPlayer* pLocalPlyr = playerManager->m_pLocalPlayer;
	//if (!pLocalPlyr)
	//	return;

	//if (pLocalPlyr->InVehicle())
	//	return;

	//ClientSoldierEntity* pLocalSoldier = pLocalPlyr->m_pControlledControllable;
	//if (!pLocalSoldier)
	//	return;

	//if (stricmp(pLocalSoldier->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
	//	return;

	//ClientSoldierWeapon* CSW = pLocalSoldier->GetActiveWeapon();
	//if (!CSW)
	//	return;

	//WeaponFiring* pWepFiring = CSW->m_pPrimary;
	//if (!pWepFiring)
	//	return;

	//WeaponSway* pWepSway = pWepFiring->m_pSway;
	//if (!pWepSway)
	//	return;

	//GunSwayData* pSwayData = pWepSway->m_data;
	//if (!pSwayData)
	//	return;

	//ClientSoldierAimingSimulation* pAimingSim = CSW->m_authorativeAiming;
	//if (!pAimingSim)
	//	return;

	//SoldierAimingSimulationData* pAimingSimData = pAimingSim->m_data;
	//if (!pAimingSimData)
	//	return;

	///*ZoomLevelData* pZoomData1 = pAimingSimData->m_ZoomLevels[0];
	//if (pZoomData1)
	//{
	//	pZoomData1->m_SupportedSwayPitchMagnitude = 0.0f;
	//	pZoomData1->m_SupportedSwayYawMagnitude = 0.0f;
	//	pZoomData1->m_SwayPitchMagnitude = 0.0f;
	//	pZoomData1->m_SwayYawMagnitude = 0.0f;
	//	pZoomData1->m_SuppressedSwayPitchMagnitude = 0.0f;
	//	pZoomData1->m_SuppressedSwayYawMagnitude = 0.0f;
	//}

	//ZoomLevelData* pZoomData2 = pAimingSimData->m_ZoomLevels[2];
	//if (pZoomData2)
	//{
	//	pZoomData2->m_SupportedSwayPitchMagnitude = 0.0f;
	//	pZoomData2->m_SupportedSwayYawMagnitude = 0.0f;
	//	pZoomData2->m_SwayPitchMagnitude = 0.0f;
	//	pZoomData2->m_SwayYawMagnitude = 0.0f;
	//	pZoomData2->m_SuppressedSwayPitchMagnitude = 0.0f;
	//	pZoomData2->m_SuppressedSwayYawMagnitude = 0.0f;
	//}*/

	////no recoil
	//pSwayData->m_DeviationScaleFactorZoom = 0.0f;
	//pSwayData->m_DeviationScaleFactorNoZoom = 0.0f;
	//pSwayData->m_GameplayDeviationScaleFactorZoom = 0.0f;
	//pSwayData->m_GameplayDeviationScaleFactorNoZoom = 0.0f;
	//pSwayData->m_FirstShotRecoilMultiplier = 0.0f;
	//pSwayData->m_ShootingRecoilDecreaseScale = 100.0f;

	//no animation
	DWORD oldProtect;
	/*VirtualProtect((LPVOID)OFFSET_GUN_ANIMATION_0, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((BYTE *)(OFFSET_GUN_ANIMATION_0 + 0)) = 0x90;
	*((BYTE *)(OFFSET_GUN_ANIMATION_0 + 1)) = 0x90;
	*((BYTE *)(OFFSET_GUN_ANIMATION_0 + 2)) = 0x90;
	*((BYTE *)(OFFSET_GUN_ANIMATION_0 + 3)) = 0x90;
	DWORD temp;
	VirtualProtect((LPVOID)OFFSET_GUN_ANIMATION_0, 4, oldProtect, &temp);

	VirtualProtect((LPVOID)OFFSET_GUN_ANIMATION_1, 2, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((BYTE *)(OFFSET_GUN_ANIMATION_1 + 0)) = 0x90;
	*((BYTE *)(OFFSET_GUN_ANIMATION_1 + 1)) = 0x90;
	VirtualProtect((LPVOID)OFFSET_GUN_ANIMATION_1, 2, oldProtect, &temp);*/
}

void noSpread()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!IsValidPTR(pGameContext))
		return;

	ClientPlayerManager* pPlayerMgnr = pGameContext->m_pPlayerManager;
	if (!IsValidPTR(pPlayerMgnr))
		return;

	ClientPlayer* pLocalPlayer = pPlayerMgnr->m_pLocalPlayer;
	if (!IsValidPTR(pLocalPlayer))
		return;

	ClientSoldierEntity* pSoldierEntity = pLocalPlayer->m_pControlledControllable;
	if (!IsValidPTR(pSoldierEntity))
		return;

	if (stricmp(pSoldierEntity->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
		return;

	ClientSoldierWeapon* CSW = pSoldierEntity->GetActiveWeapon();
	if (!IsValidPTR(CSW))
		return;

	WeaponFiring* wepFiring = CSW->m_pPrimary;
	if (!IsValidPTR(wepFiring))
		return;

	WeaponSway* wepSway = wepFiring->m_pSway;
	if (!IsValidPTR(wepSway))
		return;

	GunSwayData* pSwayData = wepSway->m_pGunSwayData;
	if (!IsValidPTR(pSwayData))
		return;

	pSwayData->m_DeviationScaleFactorNoZoom = 0.0f;
	pSwayData->m_DeviationScaleFactorZoom = 0.0f;
	pSwayData->m_FirstShotRecoilMultiplier = 0.0f;
	pSwayData->m_ShootingRecoilDecreaseScale = 100.0f;

	//RemoveDispersion(pSwayData->m_Stand);
	//RemoveDispersion(pSwayData->m_Stand.m_Moving);
	//RemoveDispersion(pSwayData->m_Stand.m_Jumping);
	//RemoveDispersion(pSwayData->m_Stand.m_Sprinting);
	//RemoveDispersion(pSwayData->m_Stand.m_VaultingSmallObject);
	//RemoveDispersion(pSwayData->m_Stand.m_VaultingMediumObject);

	//// Remove Standing Zoom Dispersion
	//RemoveDispersion(pSwayData->m_Stand.m_Zoom);
	//RemoveDispersion(pSwayData->m_Stand.m_Zoom.m_Moving);
	//RemoveDispersion(pSwayData->m_Stand.m_Zoom.m_Sprinting);
	//RemoveDispersion(pSwayData->m_Stand.m_Zoom.m_Jumping);
	//RemoveDispersion(pSwayData->m_Stand.m_Zoom.m_VaultingSmallObject);
	//RemoveDispersion(pSwayData->m_Stand.m_Zoom.m_VaultingMediumObject);

	//// Remove Crouch No Zoom Dispersion
	//RemoveDispersion(pSwayData->m_Crouch);
	//RemoveDispersion(pSwayData->m_Crouch.m_Moving);

	//// Remove Crouch Zoom Dispersion
	//RemoveDispersion(pSwayData->m_Crouch.m_Zoom);
	//RemoveDispersion(pSwayData->m_Crouch.m_Zoom.m_Moving);

	//// Remove Prone No Zoom
	//RemoveDispersion(pSwayData->m_Prone);
	//RemoveDispersion(pSwayData->m_Prone.m_Moving);

	//// Remove Prone Zoom
	//RemoveDispersion(pSwayData->m_Prone.m_Zoom);
	//RemoveDispersion(pSwayData->m_Prone.m_Zoom.m_Moving);
	bool hooked = false;

	if (!hooked)
	{
		//bf1: 29,30,31,32
		//bf4: 26,27,28,29
		orig26 = HookVTableFunction((PDWORD64*)wepSway, (PBYTE)&hk__GetRecoil, 26);
		orig27 = HookVTableFunction((PDWORD64*)wepSway, (PBYTE)&hk__GetRecoil, 27);
		orig28 = HookVTableFunction((PDWORD64*)wepSway, (PBYTE)&hk__GetRecoil, 28);
		orig29 = HookVTableFunction((PDWORD64*)wepSway, (PBYTE)&hk__GetRecoil, 29);
		hooked = true;
	}
}

void noGravity()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* pPlayerMgnr = pGameContext->m_pPlayerManager;
	if (!pPlayerMgnr)
		return;

	ClientPlayer* localPlayer = pPlayerMgnr->m_pLocalPlayer;
	if (!localPlayer)
		return;

	if (localPlayer->InVehicle())
		return;

	ClientSoldierEntity* pSoldierEntity = localPlayer->m_pControlledControllable;
	if (!pSoldierEntity)
		return;

	if (stricmp(pSoldierEntity->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
		return;

	ClientSoldierWeapon* CSW = pSoldierEntity->GetActiveWeapon();
	if (!CSW)
		return;

	WeaponFiring* wepFiring = CSW->m_pPrimary;
	if (!wepFiring)
		return;

	PrimaryFire* pPrimaryFire = wepFiring->m_pPrimaryFire;
	if (!pPrimaryFire)
		return;

	ShotConfigData* shotData = pPrimaryFire->m_primary;
	if (!shotData)
		return;

	BulletEntityData* bulletData = shotData->m_pProjectileData;
	if (!bulletData)
		return;

	shotData->m_initialSpeed = 9999.0f;
	bulletData->m_Gravity = -0.0f;
}

void damageReducer()
{
	ClientGameContext* GameContext = ClientGameContext::GetInstance();
	if (!GameContext)
		return;

	ClientPlayerManager* playerManager = GameContext->m_pPlayerManager;
	if (!playerManager)
		return;

	ClientPlayer* localPlayer = playerManager->m_pLocalPlayer;
	if (!localPlayer)
		return;

	ClientSoldierEntity* pSoldierEntity = localPlayer->m_pControlledControllable;
	if (!pSoldierEntity)
		return;

	ClientSoldierWeapon* CSW = pSoldierEntity->GetActiveWeapon();
	if (!CSW)
		return;

	WeaponFiring* wepFiring = CSW->m_pPrimary;
	if (!wepFiring)
		return;

	PrimaryFire* pFire = wepFiring->m_pPrimaryFire;
	if (!pFire)
		return;

	ShotConfigData* shotData = pFire->m_primary;
	if (!shotData)
		return;

	BulletEntityData* bulletData = shotData->m_pProjectileData;
	if (!bulletData)
		return;

	bulletData->m_StartDamage = 0.001f;
}