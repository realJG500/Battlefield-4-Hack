#include "Aimbot.h"
#include "FB Classes.h"
#include "Util.h"
#include <stdlib.h>

void Aimbot(bool bVisibleCheck)
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

	ClientSoldierEntity* pControllable = pLocalPlyr->m_pControlledControllable;
	if (!IsValidPTR(pControllable))
		return;

	ClientSoldierEntity* pLocalSoldier = pLocalPlyr->m_soldier.GetData();
	if (!IsValidPTR(pLocalSoldier))
		return;

	if (stricmp(pControllable->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
		return;

	if (stricmp(pLocalSoldier->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
		return;

	ClientSoldierWeaponsComponent* pWepComp = pLocalSoldier->m_soldierWeaponsComponent;
	if (!IsValidPTR(pWepComp))
		return;

	ClientSoldierWeaponsComponent::ClientAnimatedSoldierWeaponHandler* pAnimatedWepHandler = pWepComp->m_handler;
	if (!IsValidPTR(pAnimatedWepHandler))
		return;

	ClientSoldierWeapon* pSoldierWep = pAnimatedWepHandler->m_WeaponList[pWepComp->m_activeSlot];
	if (!IsValidPTR(pSoldierWep) || pSoldierWep == NULL)
		return;

	ClientSoldierAimingSimulation* pCSAS = pSoldierWep->m_authorativeAiming;
	if (!IsValidPTR(pCSAS))
		return;

	AimAssist* pAimAssist = pCSAS->m_fpsAimer;
	if (!IsValidPTR(pAimAssist))
		return;

	WeaponFiring* pWepFiring = pSoldierWep->m_pPrimary;
	if (!IsValidPTR(pWepFiring))
		return;

	PrimaryFire* pWepFiringData = pWepFiring->m_pPrimaryFire;
	if (!IsValidPTR(pWepFiringData))
		return;

	ShotConfigData* pFiringData = pWepFiringData->m_primary;
	if (!IsValidPTR(pFiringData))
		return;

	BulletEntityData* pBulletData = pFiringData->m_pProjectileData;
	if (!IsValidPTR(pBulletData))
		return;

	D3DXMATRIX tran;
	tran = GameRenderer::GetInstance()->m_pRenderView->m_viewMatrixInverse;
	D3DXVECTOR3 pos = D3DXVECTOR3(tran._41, tran._42, tran._43);
	D3DXVECTOR3 cameraForward = D3DXVECTOR3(-tran._31, -tran._32, -tran._33);

	float closestdist = 1000.0f;
	ClientPlayer* closestPlayer = NULL;
	ClientSoldierEntity* closestSoldier = NULL;
	D3DXVECTOR3 enemyAimVec;

	for (int i = 0; i < pPlayerMgr->getPlayers()->size(); i++)
	{
		ClientPlayer* pEnemyPlyr = pEnemyPlyr = pPlayerMgr->getPlayers()->at(i);
		if (!IsValidPTR(pEnemyPlyr))
			continue;

		if (pEnemyPlyr->m_teamID == pLocalPlyr->m_teamID)
			continue;

		ClientSoldierEntity* pEnemySoldier = pEnemyPlyr->m_soldier.GetData();
		if (!IsValidPTR(pEnemySoldier))
			continue;

		if (stricmp(pEnemySoldier->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
			continue;

		D3DXVECTOR3 enemyVec, enemyVecScreen;
		const UpdatePoseResultData::BONES bonearray[]
		{ UpdatePoseResultData::BONE_SPINE,UpdatePoseResultData::BONE_SPINE1,UpdatePoseResultData::BONE_SPINE2,
			UpdatePoseResultData::BONE_LEFTSHOULDER,UpdatePoseResultData::BONE_RIGHTSHOULDER,
			UpdatePoseResultData::BONE_SPINE1,UpdatePoseResultData::BONE_SPINE2,UpdatePoseResultData::BONE_HEAD };

		if (!GetBone(pEnemySoldier, &enemyVec, bonearray[rand() % 8]))
			continue;

		if (!WorldToScreen(enemyVec, &enemyVecScreen))
			continue;

		if (!IsInFOV(cameraForward, enemyVec, pos, 20.0f))
			continue;

		const float screenX = DxRenderer::GetInstance()->m_pScreen->m_Width / 2;
		const float screenY = DxRenderer::GetInstance()->m_pScreen->m_Height / 2;
		float diffX = fabs(enemyVecScreen.x - screenX);
		float diffY = fabs(enemyVecScreen.y - screenY);
		float screenDist = fabs(sqrt(diffX * diffX + diffY * diffY));

		if (screenDist < closestdist)
		{
			closestPlayer = pEnemyPlyr;
			closestSoldier = pEnemySoldier;
			closestdist = screenDist;
			enemyAimVec = enemyVec;
		}

		if (bVisibleCheck) {
			if (!pEnemyPlyr->InVehicle() && !pEnemySoldier->isVisible())
				continue;
		}

		float distance = 0.0f, gravity = 0.0f;
		gravity = pBulletData->m_Gravity;
		distance = getDistance2D(pos, enemyAimVec);
		D3DXVECTOR3 localVelocity;
		D3DXVECTOR3 enemyVelocity;

		if (pEnemyPlyr->InVehicle())
		{
			ClientVehicleEntity* pEnemyVeh = pEnemyPlyr->GetClientVehicleEntity();
			if (!IsValidPTR(pEnemyVeh))
				continue;

			if (stricmp(pEnemyVeh->GetTypeInfo()->m_infoData->name, "ClientVehicleEntity") != 0)
				continue;

			ClientChassisComponent* pChassis = pEnemyVeh->m_Chassis;
			if (!IsValidPTR(pChassis))
				continue;

			enemyVelocity = pChassis->m_Velocity;
		}

		else
		{
			if (!IsValidPTR(closestSoldier))
				continue;

			auto pPrediction = closestSoldier->m_pPredictedController;
			if (!IsValidPTR(pPrediction))
				continue;

			enemyVelocity = pPrediction->m_Velocity;
		}

		D3DXVECTOR3 newAimVec;
		if (pFiringData->m_initialSpeed < 2000.0f)
			newAimVec = AimCorrection(localVelocity, enemyVelocity, enemyAimVec, distance, pFiringData->m_initialSpeed, gravity);
		else
			newAimVec = enemyAimVec;

		D3DXVECTOR3 temp = newAimVec - D3DXVECTOR3(tran._41, tran._42, tran._43);
		D3DXVECTOR3 aimAngle;
		D3DXVec3Normalize(&aimAngle, &temp);

		float flYaw = -atan2(aimAngle.x, aimAngle.z);
		float flPitch = atan2(aimAngle.y, VectorLength2D(&aimAngle));

		float midpointY = pCSAS->m_sway.y;
		float midpointX = pCSAS->m_sway.x;

		float yawDiff = flYaw - midpointX;
		float pitchDiff = flPitch - midpointY;

		pAimAssist->m_yaw = yawDiff;
		pAimAssist->m_pitch = pitchDiff;
	}
}

void vehicleAimbot()
{
	BorderInputNode* pBorderInput = BorderInputNode::GetInstance();
	if (!IsValidPTR(pBorderInput))
		return;

	Mouse* pMouse = pBorderInput->m_pMouse;
	if (!IsValidPTR(pMouse))
		return;

	Mouse::MouseDevice* pMouseDevice = pMouse->m_pDevice;
	if (!IsValidPTR(pMouseDevice))
		return;

	pMouseDevice->m_UseRawMouseInput = 1;

	float bulletGrav = 0.0f;
	float bulletSpeed = 0.0f;

	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!IsValidPTR(pGameContext))
		return;

	ClientPlayerManager* pPlayerMgr = pGameContext->m_pPlayerManager;
	if (!IsValidPTR(pPlayerMgr))
		return;

	ClientPlayer* pLocalPlayer = pPlayerMgr->m_pLocalPlayer;
	if (!IsValidPTR(pLocalPlayer))
		return;

	ClientSoldierEntity* pControllable = pLocalPlayer->m_pControlledControllable;
	if (!IsValidPTR(pControllable))
		return;

	if (stricmp(pControllable->GetTypeInfo()->m_infoData->name, "ClientVehicleEntity") != 0)
		return;

	ClientVehicleEntity* pLocalVehicle = pLocalPlayer->GetClientVehicleEntity();
	if (!IsValidPTR(pLocalVehicle))
		return;

	ClientChassisComponent* pLocalChassis = pLocalVehicle->m_Chassis;
	if (!IsValidPTR(pLocalChassis))
		return;

	EntryComponent* pEntry = pLocalPlayer->GetEntryComponent();
	if (!IsValidPTR(pEntry))
		return;

	int stance = pEntry->GetActiveStance(); // 0 if primary weapon selected, 1 if secondary weapon selected
	int weaponIndex = 0;

	for (int i = 0; i < pEntry->weapons.size(); i++)
	{
		if (stance == 0) // we have our primary selected, or in a vehicle with only one weapon
		{
			if (pEntry->weapons.size() == 3 || pEntry->weapons.size() == 1) // we are a gunner, or in a vehicle with only one weapon (Jeep, Transport Heli, etc.)
			{
				weaponIndex = 0;
			}

			else
			{
				EntryComponent::FiringCallbacks* pFiringCallbacks = pEntry->weapons.at(i);
				if (!IsValidPTR(pFiringCallbacks))
					continue;

				EntryComponent::WeaponInfo* pWepInfo = pFiringCallbacks->weapon_info;
				if (!IsValidPTR(pWepInfo))
					continue;

				ClientWeaponComponent* pWepComp = pWepInfo->m_pWeaponComponent;
				if (!IsValidPTR(pWepComp))
					continue;

				WeaponComponentData* pWepCompData = pWepComp->m_pComponentData;
				if (!IsValidPTR(pWepCompData))
					continue;

				if (!stricmp(pWepCompData->m_pDamageGiverName, "HE Shell Weapon") ||
					!stricmp(pWepCompData->m_pDamageGiverName, "AAV AutoCannon") ||
					!stricmp(pWepCompData->m_pDamageGiverName, "LAV25 Cannon"))
				{
					weaponIndex = i;
				}
			}
		}

		else if (stance == 1) // we have our secondary selected
		{
			EntryComponent::FiringCallbacks* pFiringCallbacks = pEntry->weapons.at(i);
			if (!IsValidPTR(pFiringCallbacks))
				continue;

			EntryComponent::WeaponInfo* pWepInfo = pFiringCallbacks->weapon_info;
			if (!IsValidPTR(pWepInfo))
				continue;

			ClientWeaponComponent* pWepComp = pWepInfo->m_pWeaponComponent;
			if (!IsValidPTR(pWepComp))
				continue;

			WeaponComponentData* pWepCompData = pWepComp->m_pComponentData;
			if (!IsValidPTR(pWepCompData))
				continue;

			if (!stricmp(pWepCompData->m_pDamageGiverName, "M1A2 Coax LMG") || !stricmp(pWepCompData->m_pDamageGiverName, "IFV Coax"))
			{
				weaponIndex = i;
			}
		}
	}

	EntryComponent::FiringCallbacks* pFiringCallbacks = pEntry->weapons[weaponIndex];
	if (!IsValidPTR(pFiringCallbacks))
		return;

	EntryComponent::WeaponInfo* pWepInfo = pFiringCallbacks->weapon_info;
	if (!IsValidPTR(pWepInfo))
		return;

	ClientWeapon* pClientWep = pWepInfo->GetWeapon();
	if (!IsValidPTR(pClientWep))
		return;

	PrimaryFire* pPrimaryFire = pClientWep->m_pWeaponFiring;
	if (!IsValidPTR(pPrimaryFire))
		return;

	ShotConfigData* pShotData = pPrimaryFire->m_primary;
	if (!IsValidPTR(pShotData))
		return;

	BulletEntityData* pBulletEntity = pShotData->m_pProjectileData;
	if (!IsValidPTR(pBulletEntity))
		return;

	bulletSpeed = pShotData->m_initialSpeed;
	bulletGrav = pBulletEntity->m_Gravity;

	float dist = 1000.0f;
	ClientPlayer* closestPlayer = NULL;
	ClientSoldierEntity* closestSoldier = NULL;
	D3DXVECTOR3 enemyBone;

	for (int i = 0; i < 70; i++)
	{
		ClientPlayer* pEnemyPlayer = getClosestTargetbyCrosshair(true, 15);
		if (pEnemyPlayer == NULL)
		{
			pEnemyPlayer = getClosestTargetbyCrosshair(false, 15);
		}

		if (!IsValidPTR(pEnemyPlayer))
			continue;

		if (pEnemyPlayer->m_teamID == pLocalPlayer->m_teamID)
			continue;

		ClientSoldierEntity* pEnemySoldier = pEnemyPlayer->m_soldier.GetData();
		if (!IsValidPTR(pEnemySoldier))
			continue;

		if (stricmp(pEnemySoldier->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0) // aim at soldiers only, for now
			continue;

		UpdatePoseResultData::BONES bonearray[] = { UpdatePoseResultData::BONE_NECK, UpdatePoseResultData::BONE_HEAD, UpdatePoseResultData::BONE_SPINE, UpdatePoseResultData::BONE_SPINE1, UpdatePoseResultData::BONE_SPINE2 };
		int bone = rand() % 5;

		if (!GetBone(pEnemySoldier, &enemyBone, bonearray[bone]))
			continue;

		D3DXVECTOR3 myVelocity = D3DXVECTOR3(0, 0, 0);// pLocalChassis->m_Velocity;
		D3DXMATRIX trans;
		pLocalVehicle->GetTransform(&trans);

		D3DXMATRIX invMat;
		invMat = GameRenderer::GetInstance()->m_pRenderView->m_viewMatrixInverse;

		D3DXVECTOR3 vehicleLoc = D3DXVECTOR3(invMat._41, invMat._42, invMat._43);
		D3DXVECTOR3 correctedEnemyVec;
		D3DXVECTOR3 enemyVelocity;

		if (pEnemyPlayer->InVehicle())
		{
			ClientVehicleEntity* pEnemyVeh = pEnemyPlayer->GetClientVehicleEntity();
			if (!pEnemyVeh)
				continue;

			ClientChassisComponent* pChassis = pEnemyVeh->m_Chassis;
			if (!pChassis)
				continue;

			enemyVelocity = pChassis->m_Velocity;
		}

		else
		{
			enemyVelocity = pEnemySoldier->m_pPredictedController->m_Velocity;
		}

		D3DXVECTOR3 aimPos = AimCorrection(myVelocity, enemyVelocity, enemyBone, getDistance2D(vehicleLoc, enemyBone), bulletSpeed, bulletGrav);
		D3DXVECTOR3 screenPos;
		if (!WorldToScreen(aimPos, &screenPos)) // new check
			continue;

		float midX;
		float midY;
		float deltaX;
		float deltaY;

		if (pEntry->weapons.size() == 12 || pEntry->weapons.size() == 13 || pEntry->weapons.size() == 10) // we are a tank, LAV, or AA
		{
			Current_Weapon* pCurWep = Current_Weapon::GetInstance();
			if (!IsValidPTR(pCurWep))
				continue;

			D3DXMATRIX shootspace = pCurWep->matrice1;
			D3DXVECTOR3 forward = shootspace.m[2];
			D3DXVECTOR3 pos = shootspace.m[3];
			D3DXVECTOR3 midpoint;
			if (!WorldToScreen(forward + pos, &midpoint)) // new check
				continue;

			midX = midpoint.x;
			midY = midpoint.y;
			deltaX = screenPos.x - midX;
			deltaY = screenPos.y - midY;
		}

		else
		{
			midX = DxRenderer::GetInstance()->m_pScreen->m_Width / 2;
			midY = DxRenderer::GetInstance()->m_pScreen->m_Height / 2;
			deltaX = screenPos.x - midX;
			deltaY = screenPos.y - midY;
		}

		if (deltaX > 5.0f || deltaX < -5.0f)
		{
			pMouseDevice->m_Buffer.x = deltaX / 2.0f;
		}
		else
		{
			pMouseDevice->m_Buffer.x = deltaX;
		}

		if (deltaY > 5.0f || deltaY < -5.0f)
		{
			pMouseDevice->m_Buffer.y = deltaY / 2.0f;
		}
		else
		{
			pMouseDevice->m_Buffer.y = deltaY;
		}

		/*Vec3 left, up, orig;
		left = Vec3(invMat._11, invMat._12, invMat._13);
		up = Vec3(invMat._21, invMat._22, invMat._23);
		orig = Vec3(invMat._41, invMat._42, invMat._43);

		Vec3 finalenemyVec = Vec3(correctedEnemyVec.x, correctedEnemyVec.y, correctedEnemyVec.z);
		Vec3 dt = Vec3(finalenemyVec.m_x - orig.m_x, finalenemyVec.m_y - orig.m_y, finalenemyVec.m_z - orig.m_z);
		dt.Normalize();

		D3DXVECTOR3 enemyVecWorld;
		WorldToScreen(correctedEnemyVec, &enemyVecWorld);
		float screenX = DxRenderer::GetInstance()->m_pScreen->m_Width / 2;
		float screenY = DxRenderer::GetInstance()->m_pScreen->m_Height / 2;
		float diffX = abs(enemyVecWorld.x - screenX);
		float diffY = abs(enemyVecWorld.y - screenY);
		float screenDist = sqrtf(diffX*diffX + diffY*diffY);

		float flYaw, flPitch, flRoll, flVehicleSens = 0.0f;

		if (screenDist < 150.0f && screenDist > 75.0f)
		flVehicleSens = 1.75f;
		else if (screenDist < 75.0f && screenDist > 50.0f)
		flVehicleSens = 1.20f;
		else if (screenDist < 50.0f && screenDist > 15.0f)
		flVehicleSens = 1.00f;
		else
		flVehicleSens = 0.90f;

		flYaw = left.AimFloat(dt, GameRenderer::GetInstance()->m_pRenderView->fovX / 2.0f);
		flPitch = up.AimFloat(dt, GameRenderer::GetInstance()->m_pRenderView->fovX / 2.0f);
		flRoll = flYaw;

		BorderInputNode* pBorderInput = BorderInputNode::GetInstance();
		if (!pBorderInput)
		return;

		for (int i = 0; i < 234; i++)
		{
		pBorderInput->m_inputCache->flInputBuffer[i] += g_pInputBuffers[i];
		g_pInputBuffers[i] = 0.0f;
		}

		pBorderInput->m_inputCache->flInputBuffer[ConceptPitch] = flPitch * flVehicleSens;
		pBorderInput->m_inputCache->flInputBuffer[ConceptYaw] = flYaw * flVehicleSens;
		pBorderInput->m_inputCache->flInputBuffer[ConceptRoll] = flRoll * flVehicleSens;*/
	}
}

void NormalizeAngle(D3DXVECTOR2 &Angle)
{
	while (Angle.x <= -D3DX_PI)
		Angle.x += 2 * D3DX_PI;
	while (Angle.x > D3DX_PI)
		Angle.x -= 2 * D3DX_PI;
	while (Angle.y <= -D3DX_PI)
		Angle.y += 2 * D3DX_PI;
	while (Angle.y > D3DX_PI)
		Angle.y -= 2 * D3DX_PI;
}

void SmoothAngle(D3DXVECTOR2& From, D3DXVECTOR2& To, float Percent)
{
	D3DXVECTOR2 VecDelta = From - To;
	NormalizeAngle(VecDelta);
	VecDelta.x *= Percent;
	VecDelta.y *= Percent;
	To = From - VecDelta;
}

D3DXVECTOR3 AimCorrection(const D3DXVECTOR3& Velocity, const D3DXVECTOR3& EnemyVelocity, /*__OUT__*/ D3DXVECTOR3 InVec, float Distance, float Speed, float Gravity)
{
	InVec = InVec + EnemyVelocity * (Distance / fabs(Speed));
	// InVec = InVec - Velocity * (Distance / (float)abs(Speed));
	float m_grav = fabs(Gravity);
	float m_dist = Distance / fabs(Speed);
	InVec.y += 0.5f * m_grav * m_dist * m_dist;
	return InVec;
}