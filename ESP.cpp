#include "ESP.h"
#include "Function Prototypes.h"

void addToShitList(std::string name)
{
	blacklist.push_back(name);
}

void ESP(bool bDrawNames)
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!IsValidPTR(pGameContext))
		return;

	ClientPlayerManager* playerManager = pGameContext->m_pPlayerManager;
	if (!IsValidPTR(playerManager))
		return;

	ClientPlayer* pLocalPlayer = playerManager->m_pLocalPlayer;
	if (!IsValidPTR(pLocalPlayer))
		return;

	ClientSoldierEntity* pLocalSoldier = pLocalPlayer->m_pControlledControllable;
	if (!IsValidPTR(pLocalSoldier))
		return;

	for (int i = 0; i < 70; i++)
	{
		ClientPlayer* pPlayer = playerManager->GetPlayerById(i);
		if (!IsValidPTR(pPlayer))
			continue;

		if (pPlayer->InVehicle() && pPlayer->m_teamID != pLocalPlayer->m_teamID)
		{
			ClientVehicleEntity* pVehicle = pPlayer->GetClientVehicleEntity();
			if (!IsValidPTR(pVehicle))
				continue;

			ClientSoldierEntity* pEnemySoldier = pPlayer->m_soldier.GetData();
			if (!IsValidPTR(pEnemySoldier))
				continue;

			if (stricmp(pEnemySoldier->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
				continue;

			D3DXMATRIX tran;
			pVehicle->GetTransform(&tran);
			D3DXVECTOR3 loc = tran.m[3];

			if (!WorldToScreen(loc))
				continue;

			if (loc.x > DxRenderer::GetInstance()->m_pScreen->m_Width || loc.y > DxRenderer::GetInstance()->m_pScreen->m_Height)
				continue;

			if (loc.x < 2.00f && loc.y < 2.00f)
				continue;

			bool alreadyDrawn = false;

			for (int k = 0; k < blacklist.size(); k++)
			{
				if (strstr(pPlayer->szName, blacklist.at(k).c_str()) != NULL) // we have a match
				{
					DebugRenderer2::Singleton()->drawLine2d(DxRenderer::GetInstance()->m_pScreen->m_Width / 2,
						DxRenderer::GetInstance()->m_pScreen->m_Height - 10,
						loc.x,
						loc.y,
						Color32(0, 255, 255, 100));
					DebugRenderer2::Singleton()->drawText(loc.x, loc.y, Color32(0, 255, 255, 100), pPlayer->szName, 2.0f);
					DrawVehicleAABB(pPlayer);
					DrawPlayerAABB(pEnemySoldier, 0, 255, 255);
					alreadyDrawn = true;
				}
			}

			if (!alreadyDrawn)
			{
				// no need to render twice
				DebugRenderer2::Singleton()->drawText(loc.x, loc.y, Color32(200, 0, 200, 100), pPlayer->szName, 1.0f);
				DrawVehicleAABB(pPlayer);
				DrawPlayerAABB(pEnemySoldier, 255, 255, 51);
				continue;
			}
		}

		else if (pPlayer->m_teamID != pLocalPlayer->m_teamID)
		{
			ClientSoldierEntity* pEnemySoldier = pPlayer->m_soldier.GetData();
			if (!IsValidPTR(pEnemySoldier))
				continue;

			if (stricmp(pEnemySoldier->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity") != 0)
				continue;

			D3DXMATRIX trans;
			pEnemySoldier->GetTransform(&trans);
			D3DXVECTOR3 loc = trans.m[3];
			D3DXVECTOR3 screen;

			if (!WorldToScreen(loc, &screen))
				continue;

			if (screen.x > DxRenderer::GetInstance()->m_pScreen->m_Width || screen.y > DxRenderer::GetInstance()->m_pScreen->m_Height)
				continue;

			if (screen.x < 2.00f && screen.y < 2.00f)
				continue;

			bool visible = pEnemySoldier->isVisible();
			bool alreadyDrawn = false;

			int r = 0, g = 0, b = 0, a = 100;

			for (int k = 0; k < blacklist.size(); k++)
			{
				if (strstr(pPlayer->szName, blacklist.at(k).c_str()) != NULL) // we have a match
				{
					DebugRenderer2::Singleton()->drawLine2d(DxRenderer::GetInstance()->m_pScreen->m_Width / 2,
						DxRenderer::GetInstance()->m_pScreen->m_Height - 10,
						screen.x,
						screen.y,
						Color32(0, 255, 255, 100));
					DebugRenderer2::Singleton()->drawText(screen.x, screen.y, Color32(0, 255, 255, 100), pPlayer->szName, 2.0f);
					DrawPlayerAABB(pEnemySoldier, 0, 255, 255);
					alreadyDrawn = true;
				}
			}

			if (visible && !alreadyDrawn)
			{
				r = 51; g = 255; b = 51;
				DebugRenderer2::Singleton()->drawText(screen.x, screen.y, Color32(r, g, b, 100), pPlayer->szName, 1.0f);
				DrawPlayerAABB(pEnemySoldier, r, g, b);
			}


			else if (!alreadyDrawn)
			{
				r = 255; g = 0; b = 0;
				DebugRenderer2::Singleton()->drawText(screen.x, screen.y, Color32(r, g, b, 100), pPlayer->szName, 1.0f);
				DrawPlayerAABB(pEnemySoldier, r, g, b);
			}
		}
	}
}

void DrawPlayerAABB(ClientSoldierEntity* pSoldier, int r, int g, int b)
{
	if (!IsValidPTR(pSoldier))
		return;

	int a = 255;
	D3DXMATRIX trans;

	pSoldier->GetTransform(&trans);
	D3DXVECTOR3 loc = trans.m[3];
	TransformAABBStruct transAABB;
	pSoldier->GetAABB(&transAABB);
	AxisAlignedBox aabb = transAABB.AABB;

	D3DXVECTOR3 min = D3DXVECTOR3(aabb.min.x, aabb.min.y, aabb.min.z);
	D3DXVECTOR3 max = D3DXVECTOR3(aabb.max.x, aabb.max.y, aabb.max.z);
	D3DXVECTOR3 crnr2 = D3DXVECTOR3(max.x, min.y, min.z);
	D3DXVECTOR3 crnr3 = D3DXVECTOR3(max.x, min.y, max.z);
	D3DXVECTOR3 crnr4 = D3DXVECTOR3(min.x, min.y, max.z);
	D3DXVECTOR3 crnr5 = D3DXVECTOR3(min.x, max.y, max.z);
	D3DXVECTOR3 crnr6 = D3DXVECTOR3(min.x, max.y, min.z);
	D3DXVECTOR3 crnr7 = D3DXVECTOR3(max.x, max.y, min.z);

	D3DXVECTOR3 pMin, pMax, pCrnr2, pCrnr3, pCrnr4, pCrnr5, pCrnr6, pCrnr7;
	D3DXVec3TransformNormal(&pMin, &min, &trans);
	D3DXVec3TransformNormal(&pMax, &max, &trans);
	D3DXVec3TransformNormal(&pCrnr2, &crnr2, &trans);
	D3DXVec3TransformNormal(&pCrnr3, &crnr3, &trans);
	D3DXVec3TransformNormal(&pCrnr4, &crnr4, &trans);
	D3DXVec3TransformNormal(&pCrnr5, &crnr5, &trans);
	D3DXVec3TransformNormal(&pCrnr6, &crnr6, &trans);
	D3DXVec3TransformNormal(&pCrnr7, &crnr7, &trans);

	pMin += loc;
	pMax += loc;
	pCrnr2 += loc;
	pCrnr3 += loc;
	pCrnr4 += loc;
	pCrnr5 += loc;
	pCrnr6 += loc;
	pCrnr7 += loc;

	D3DXVECTOR3 cornerDiff = pMin - pCrnr2;
	if (sqrtf(cornerDiff.x*cornerDiff.x + cornerDiff.y*cornerDiff.y + cornerDiff.z*cornerDiff.z) > 1.33f)
		return;

	if (WorldToScreen(pMin) && WorldToScreen(pMax) && WorldToScreen(pCrnr2) && WorldToScreen(pCrnr3) && WorldToScreen(pCrnr4) && WorldToScreen(pCrnr5) && WorldToScreen(pCrnr6) && WorldToScreen(pCrnr7))
	{
		DebugRenderer2::Singleton()->drawLine2d(pMin.x, pMin.y, pCrnr2.x, pCrnr2.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pMin.x, pMin.y, pCrnr4.x, pCrnr4.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pMin.x, pMin.y, pCrnr6.x, pCrnr6.y, Color32(r, g, b, a));

		DebugRenderer2::Singleton()->drawLine2d(pMax.x, pMax.y, pCrnr5.x, pCrnr5.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pMax.x, pMax.y, pCrnr7.x, pCrnr7.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pMax.x, pMax.y, pCrnr3.x, pCrnr3.y, Color32(r, g, b, a));

		DebugRenderer2::Singleton()->drawLine2d(pCrnr2.x, pCrnr2.y, pCrnr7.x, pCrnr7.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pCrnr2.x, pCrnr2.y, pCrnr3.x, pCrnr3.y, Color32(r, g, b, a));

		DebugRenderer2::Singleton()->drawLine2d(pCrnr4.x, pCrnr4.y, pCrnr5.x, pCrnr5.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pCrnr4.x, pCrnr4.y, pCrnr3.x, pCrnr3.y, Color32(r, g, b, a));

		DebugRenderer2::Singleton()->drawLine2d(pCrnr6.x, pCrnr6.y, pCrnr5.x, pCrnr5.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pCrnr6.x, pCrnr6.y, pCrnr7.x, pCrnr7.y, Color32(r, g, b, a));

		/*D3DDrawLine(pMin.x, pMin.y, pCrnr2.x, pCrnr2.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pMin.x, pMin.y, pCrnr4.x, pCrnr4.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pMin.x, pMin.y, pCrnr6.x, pCrnr6.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);

		D3DDrawLine(pMax.x, pMax.y, pCrnr5.x, pCrnr5.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pMax.x, pMax.y, pCrnr7.x, pCrnr7.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pMax.x, pMax.y, pCrnr3.x, pCrnr3.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);

		D3DDrawLine(pCrnr2.x, pCrnr2.y, pCrnr7.x, pCrnr7.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pCrnr2.x, pCrnr2.y, pCrnr3.x, pCrnr3.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);

		D3DDrawLine(pCrnr4.x, pCrnr4.y, pCrnr5.x, pCrnr5.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pCrnr4.x, pCrnr4.y, pCrnr3.x, pCrnr3.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);

		D3DDrawLine(pCrnr6.x, pCrnr6.y, pCrnr5.x, pCrnr5.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pCrnr6.x, pCrnr6.y, pCrnr7.x, pCrnr7.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);*/
	}
}

void DrawVehicleAABB(ClientPlayer* player)
{
	int r = 200, g = 0, b = 200, a = 255;
	D3DXMATRIX trans;

	ClientVehicleEntity* pVehicle = player->GetClientVehicleEntity();
	if (!IsValidPTR(pVehicle))
		return;

	TransformAABBStruct transAABB;
	pVehicle->GetAABB(&transAABB);
	pVehicle->GetTransform(&trans);
	D3DXVECTOR3 loc = D3DXVECTOR3(trans._41, trans._42, trans._43);
	AxisAlignedBox aabb = transAABB.AABB;

	D3DXVECTOR3 min = D3DXVECTOR3(aabb.min.x, aabb.min.y, aabb.min.z);
	D3DXVECTOR3 max = D3DXVECTOR3(aabb.max.x, aabb.max.y, aabb.max.z);
	D3DXVECTOR3 crnr2 = D3DXVECTOR3(max.x, min.y, min.z);
	D3DXVECTOR3 crnr3 = D3DXVECTOR3(max.x, min.y, max.z);
	D3DXVECTOR3 crnr4 = D3DXVECTOR3(min.x, min.y, max.z);
	D3DXVECTOR3 crnr5 = D3DXVECTOR3(min.x, max.y, max.z);
	D3DXVECTOR3 crnr6 = D3DXVECTOR3(min.x, max.y, min.z);
	D3DXVECTOR3 crnr7 = D3DXVECTOR3(max.x, max.y, min.z);

	D3DXVECTOR3 pMin, pMax, pCrnr2, pCrnr3, pCrnr4, pCrnr5, pCrnr6, pCrnr7;
	D3DXVec3TransformNormal(&pMin, &min, &trans);
	D3DXVec3TransformNormal(&pMax, &max, &trans);
	D3DXVec3TransformNormal(&pCrnr2, &crnr2, &trans);
	D3DXVec3TransformNormal(&pCrnr3, &crnr3, &trans);
	D3DXVec3TransformNormal(&pCrnr4, &crnr4, &trans);
	D3DXVec3TransformNormal(&pCrnr5, &crnr5, &trans);
	D3DXVec3TransformNormal(&pCrnr6, &crnr6, &trans);
	D3DXVec3TransformNormal(&pCrnr7, &crnr7, &trans);

	pMin += loc;
	pMax += loc;
	pCrnr2 += loc;
	pCrnr3 += loc;
	pCrnr4 += loc;
	pCrnr5 += loc;
	pCrnr6 += loc;
	pCrnr7 += loc;

	if (WorldToScreen(pMin) && WorldToScreen(pMax) && WorldToScreen(pCrnr2) && WorldToScreen(pCrnr3) && WorldToScreen(pCrnr4) && WorldToScreen(pCrnr5) && WorldToScreen(pCrnr6) && WorldToScreen(pCrnr7))
	{
		DebugRenderer2::Singleton()->drawLine2d(pMin.x, pMin.y, pCrnr2.x, pCrnr2.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pMin.x, pMin.y, pCrnr4.x, pCrnr4.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pMin.x, pMin.y, pCrnr6.x, pCrnr6.y, Color32(r, g, b, a));

		DebugRenderer2::Singleton()->drawLine2d(pMax.x, pMax.y, pCrnr5.x, pCrnr5.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pMax.x, pMax.y, pCrnr7.x, pCrnr7.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pMax.x, pMax.y, pCrnr3.x, pCrnr3.y, Color32(r, g, b, a));

		DebugRenderer2::Singleton()->drawLine2d(pCrnr2.x, pCrnr2.y, pCrnr7.x, pCrnr7.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pCrnr2.x, pCrnr2.y, pCrnr3.x, pCrnr3.y, Color32(r, g, b, a));

		DebugRenderer2::Singleton()->drawLine2d(pCrnr4.x, pCrnr4.y, pCrnr5.x, pCrnr5.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pCrnr4.x, pCrnr4.y, pCrnr3.x, pCrnr3.y, Color32(r, g, b, a));

		DebugRenderer2::Singleton()->drawLine2d(pCrnr6.x, pCrnr6.y, pCrnr5.x, pCrnr5.y, Color32(r, g, b, a));
		DebugRenderer2::Singleton()->drawLine2d(pCrnr6.x, pCrnr6.y, pCrnr7.x, pCrnr7.y, Color32(r, g, b, a));

		/*D3DDrawLine(pMin.x, pMin.y, pCrnr2.x, pCrnr2.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pMin.x, pMin.y, pCrnr4.x, pCrnr4.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pMin.x, pMin.y, pCrnr6.x, pCrnr6.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);*/

		/*D3DDrawLine(pMax.x, pMax.y, pCrnr5.x, pCrnr5.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pMax.x, pMax.y, pCrnr7.x, pCrnr7.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pMax.x, pMax.y, pCrnr3.x, pCrnr3.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);*/

		/*D3DDrawLine(pCrnr2.x, pCrnr2.y, pCrnr7.x, pCrnr7.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pCrnr2.x, pCrnr2.y, pCrnr3.x, pCrnr3.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);*/

		/*D3DDrawLine(pCrnr4.x, pCrnr4.y, pCrnr5.x, pCrnr5.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pCrnr4.x, pCrnr4.y, pCrnr3.x, pCrnr3.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);*/

		/*D3DDrawLine(pCrnr6.x, pCrnr6.y, pCrnr5.x, pCrnr5.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);
		D3DDrawLine(pCrnr6.x, pCrnr6.y, pCrnr7.x, pCrnr7.y, D3DCOLOR_RGBA(r, g, b, a), dx_Device);*/
	}
}

void showChams()
{
	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return;

	ClientPlayerManager* pPlayerMngr = pGameContext->m_pPlayerManager;
	if (!pPlayerMngr)
		return;

	ClientPlayer* pLocalPlayer = pPlayerMngr->m_pLocalPlayer;
	if (!pLocalPlayer)
		return;

	ClientSoldierEntity* pLocalSoldier = pLocalPlayer->m_pControlledControllable;
	if (!pLocalSoldier)
		return;

	for (int i = 0; i < 70; i++)
	{
		ClientPlayer* pPlayer = pPlayerMngr->GetPlayerById(i);
		if (!pPlayer)
			continue;

		ClientSoldierEntity* pSoldierEntity = pPlayer->m_soldier.GetData();
		if (!pSoldierEntity)
			continue;

		if (stricmp(pSoldierEntity->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity"))
			continue;

		if (pSoldierEntity->m_pHealthComp->m_Health <= 0.1f)
		{
			ClientSoldierEntity* pCorpse = pPlayer->m_corpse.GetData();
			if (!pCorpse)
				return;

			pCorpse->m_EngineChams = 64;
		}

		if (pPlayer->m_teamID != pLocalPlayer->m_teamID)
		{
			pSoldierEntity->m_EngineChams = 4;
			pSoldierEntity->m_RenderChamsBehindBigWall = 159;
		}

		else
		{
			pSoldierEntity->m_EngineChams = 64;
		}
	}
}

void minimap()
{
	for (unsigned int i = 0; i < 70; i++)
	{
		ClientGameContext* CGC = ClientGameContext::GetInstance();
		if (!CGC)
			continue;

		ClientPlayerManager* playerManager = CGC->m_pPlayerManager;
		if (!playerManager)
			continue;

		ClientPlayer* player = playerManager->m_ppPlayers[i];
		if (!player)
			continue;

		ClientPlayer* localPlayer = playerManager->m_pLocalPlayer;
		if (!localPlayer)
			continue;

		if (player != localPlayer)
		{
			int playerTeamID = player->m_teamID;
			if (!playerTeamID)
				continue;

			if (playerTeamID != localPlayer->m_teamID)
			{
				ClientControllableEntity* player_entity = (ClientControllableEntity*)player->m_pControlledControllable;

				if (!player_entity)
					continue;

				typedef ClientSpottingTargetComponent* (*getFirstComponentOfType_t)(ClientControllableEntity* thisptr, int entity);
				getFirstComponentOfType_t getSpottingComponent = (getFirstComponentOfType_t)(OFFSET_GETCLIENTSPOTTINGTARGETCOMPONENT);

				ClientSpottingTargetComponent* CSTC = getSpottingComponent(player_entity, 0);

				if (!CSTC)
					continue;

				if (CSTC->m_spotType != ClientSpottingTargetComponent::SpotType_Active)
					CSTC->m_spotType = ClientSpottingTargetComponent::SpotType_Active;
			}
		}
	}
}