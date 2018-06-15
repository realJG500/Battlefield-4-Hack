#include "Util.h"
#include "Function Prototypes.h"

float getDistance3D(const D3DXVECTOR3& myLocation, const D3DXVECTOR3& enemyLocation)
{
	float x = enemyLocation.x - myLocation.x;
	float y = enemyLocation.y - myLocation.y;
	float z = enemyLocation.z - myLocation.z;
	return sqrt(x*x + y*y + z*z);
}

float getDistance2D(const D3DXVECTOR3& from, const D3DXVECTOR3& to)
{
	float xDiff = to.x - from.x;
	float zDiff = to.z - from.z;
	return sqrtf(xDiff*xDiff + zDiff*zDiff);
}

float getScreenDistance(const D3DXVECTOR3& worldPos)
{
	D3DXVECTOR3 screenpos;
	WorldToScreen(worldPos, &screenpos);
	float midX = DxRenderer::GetInstance()->m_pScreen->m_Width / 2;
	float midY = DxRenderer::GetInstance()->m_pScreen->m_Height / 2;
	float diffX = fabsf(screenpos.x - midX);
	float diffY = fabsf(screenpos.y - midY);
	float screenDist = sqrtf(diffX*diffX + diffY*diffY);
	return screenDist;
}

bool WorldToScreen(D3DXVECTOR3& vLocVec4)
{
	GameRenderer* pGameRenderer = GameRenderer::GetInstance();
	D3DXMATRIX vProjMatrix = pGameRenderer->m_pRenderView->m_ViewProj;

	float width = DxRenderer::GetInstance()->m_pScreen->m_Width;
	float height = DxRenderer::GetInstance()->m_pScreen->m_Height;

	float w =
		vProjMatrix(0, 3) * vLocVec4.x +
		vProjMatrix(1, 3) * vLocVec4.y +
		vProjMatrix(2, 3) * vLocVec4.z +
		vProjMatrix(3, 3);

	if (w < 0.65f)
	{
		vLocVec4.z = w;
		return false;
	}

	float x =
		vProjMatrix(0, 0) * vLocVec4.x +
		vProjMatrix(1, 0) * vLocVec4.y +
		vProjMatrix(2, 0) * vLocVec4.z +
		vProjMatrix(3, 0);

	float y =
		vProjMatrix(0, 1) * vLocVec4.x +
		vProjMatrix(1, 1) * vLocVec4.y +
		vProjMatrix(2, 1) * vLocVec4.z +
		vProjMatrix(3, 1);

	vLocVec4.x = (width*0.5) + (width*0.5) * x / w;
	vLocVec4.y = (height*0.5) - (height*0.5) * y / w;
	vLocVec4.z = w;
	return true;
}

bool WorldToScreen(const D3DXVECTOR3& WorldPos, D3DXVECTOR3* ScreenPos)
{
	D3DXMATRIX viewProj = GameRenderer::GetInstance()->m_pRenderView->m_ViewProj;

	float mX = DxRenderer::GetInstance()->m_pScreen->m_Width * 0.5f;
	float mY = DxRenderer::GetInstance()->m_pScreen->m_Height * 0.5f;

	float w =
		viewProj(0, 3) * WorldPos.x +
		viewProj(1, 3) * WorldPos.y +
		viewProj(2, 3) * WorldPos.z +
		viewProj(3, 3);

	if (w < 0.65f)
	{
		ScreenPos->z = w;
		return false;
	}

	float x =
		viewProj(0, 0) * WorldPos.x +
		viewProj(1, 0) * WorldPos.y +
		viewProj(2, 0) * WorldPos.z +
		viewProj(3, 0);

	float y =
		viewProj(0, 1) * WorldPos.x +
		viewProj(1, 1) * WorldPos.y +
		viewProj(2, 1) * WorldPos.z +
		viewProj(3, 1);

	ScreenPos->x = mX * (1 + x / w);
	ScreenPos->y = mY * (1 - y / w);
	ScreenPos->z = w;

	return true;
}

bool copyMem(void* Dst, void* Src, int num)
{
	size_t num_of_bytes;
	void* source;
	void* destination;

	num_of_bytes = num;
	source = Src;
	destination = Dst;

	DWORD oldProtect;
	VirtualProtect(Dst, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(destination, source, num_of_bytes);
	return VirtualProtect(destination, 8, oldProtect, &oldProtect);
}

PBYTE HookVTableFunction(PDWORD64* ppVTable, PBYTE pHook, SIZE_T iIndex)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)((*ppVTable) + iIndex), sizeof(PDWORD64), PAGE_EXECUTE_READWRITE, &dwOld);

	PBYTE pOrig = ((PBYTE)(*ppVTable)[iIndex]);
	(*ppVTable)[iIndex] = (DWORD64)pHook;

	VirtualProtect((void*)((*ppVTable) + iIndex), sizeof(PDWORD64), dwOld, &dwOld);

	return pOrig;
}

void* DetourVTable(void** VTable, int Function, void* Detour)
{
	if (!VTable)
		return NULL;

	if (!Detour)
		return NULL;

	void *ret = VTable[Function];

	DWORD old;
	VirtualProtect(&(VTable[Function]), sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
	VTable[Function] = Detour;
	VirtualProtect(&(VTable[Function]), sizeof(void*), old, &old);

	return ret;
}

float VectorLength2D(D3DXVECTOR3* pV)
{
	return sqrtf(pV->x * pV->x + pV->z * pV->z);
}

bool GetBone(ClientSoldierEntity* pEnt, D3DXVECTOR3 *vOut, UpdatePoseResultData::BONES BoneId)
{
	if (!IsValidPTR(pEnt))
		return false;

	ClientRagDollComponent* pRag = pEnt->m_ragdollComponent;
	if (!IsValidPTR(pRag))
		return false;

	UpdatePoseResultData PoseResult = pRag->m_ragdollTransforms;
	if (PoseResult.m_ValidTransforms)
	{
		UpdatePoseResultData::QuatTransform* pQuat = PoseResult.m_ActiveWorldTransforms;
		if (!IsValidPTR(pQuat))
			return false;

		D3DXVECTOR3 Temp = pQuat[BoneId].m_TransAndScale;

		vOut->x = Temp.x;
		vOut->y = Temp.y;
		vOut->z = Temp.z;
		return true;
	}

	else
	{
		return false;
	}
}

bool IsInFOV(D3DXVECTOR3 forward, D3DXVECTOR3 EnemyPos, D3DXVECTOR3 LocalPos, float fov)
{
	D3DXVECTOR3 vec1, vec2;
	D3DXVec3Normalize(&vec1, &forward);
	D3DXVec3Normalize(&vec2, &(EnemyPos - LocalPos));
	float angle = acos(D3DXVec3Dot(&vec1, &vec2));
	angle = D3DXToDegree(angle);

	if (angle > (fov / 2))
		return false;

	return true;
}

ClientPlayer* getClosestTargetbyDistance(int fov)
{
	int targetFOV = fov;

	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return NULL;

	ClientPlayerManager* pPlayerMgr = pGameContext->m_pPlayerManager;
	if (!pPlayerMgr)
		return NULL;

	ClientPlayer* pLocalPlyr = pPlayerMgr->m_pLocalPlayer;
	if (!pLocalPlyr)
		return NULL;

	ClientSoldierEntity* pLocalSoldier = pLocalPlyr->m_pControlledControllable;
	if (!pLocalSoldier)
		return NULL;

	D3DXMATRIX tran;
	tran = GameRenderer::GetInstance()->m_pRenderView->m_viewMatrixInverse;
	D3DXVECTOR3 localPos = tran.m[3];
	D3DXVECTOR3 cameraForward = D3DXVECTOR3(-tran._31, -tran._32, -tran._33);

	float closestDist = 1000.0f;
	ClientPlayer* closestPlayer = NULL;
	ClientSoldierEntity* closestSoldier = NULL;

	for (int i = 0; i < 70; i++)
	{
		ClientPlayer* pEnemyPlyr = pPlayerMgr->GetPlayerById(i);
		if (!IsValidPTR(pEnemyPlyr))
			continue;

		if (pEnemyPlyr->m_teamID == pLocalPlyr->m_teamID)
			continue;

		ClientSoldierEntity* pEnemySoldier = pEnemyPlyr->m_soldier.GetData();
		if (!IsValidPTR(pEnemySoldier))
			continue;

		if (stricmp(pEnemySoldier->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity")) // typeinfo checking, do not remove
			continue;

		D3DXVECTOR3 enemyVec, enemyVecScreen;
		if (!GetBone(pEnemySoldier, &enemyVec, UpdatePoseResultData::BONE_SPINE))
			continue;

		if (!WorldToScreen(enemyVec, &enemyVecScreen))
			continue;

		if (enemyVecScreen.x > 1920 || enemyVecScreen.y > 1080)
			continue;

		if (enemyVecScreen.x < 2.00f && enemyVecScreen.y < 2.00f)
			continue;

		if (!IsInFOV(cameraForward, enemyVec, localPos, targetFOV))
			continue;

		float diffX = fabsf(enemyVec.x - localPos.x);
		float diffY = fabsf(enemyVec.y - localPos.y);
		float diffZ = fabsf(enemyVec.z - localPos.z);
		float dist = sqrtf(diffX*diffX + diffY*diffY + diffZ*diffZ);

		if (dist < closestDist)
		{
			closestDist = dist;
			closestPlayer = pEnemyPlyr;
			closestSoldier = pEnemySoldier;

			return closestPlayer;
		}
	}

	return NULL;
}

ClientPlayer* getClosestTargetbyCrosshair(bool visibleCheck, int fov)
{
	int targetFOV = fov;

	ClientGameContext* pGameContext = ClientGameContext::GetInstance();
	if (!pGameContext)
		return NULL;

	ClientPlayerManager* pPlayerMgr = pGameContext->m_pPlayerManager;
	if (!pPlayerMgr)
		return NULL;

	ClientPlayer* pLocalPlyr = pPlayerMgr->m_pLocalPlayer;
	if (!pLocalPlyr)
		return NULL;

	ClientSoldierEntity* pLocalSoldier = pLocalPlyr->m_pControlledControllable;
	if (!pLocalSoldier)
		return NULL;

	D3DXMATRIX tran;
	tran = GameRenderer::GetInstance()->m_pRenderView->m_viewMatrixInverse;
	D3DXVECTOR3 localPos = tran.m[3];
	D3DXVECTOR3 cameraForward = D3DXVECTOR3(-tran._31, -tran._32, -tran._33);

	float closestdist = 1000.0f;
	ClientPlayer* closestPlayer = NULL;
	ClientSoldierEntity* closestSoldier = NULL;

	for (int i = 0; i < 70; i++)
	{
		ClientPlayer* pEnemyPlyr = pPlayerMgr->GetPlayerById(i);
		if (!IsValidPTR(pEnemyPlyr))
			continue;

		if (pEnemyPlyr->m_teamID == pLocalPlyr->m_teamID)
			continue;

		ClientSoldierEntity* pEnemySoldier = pEnemyPlyr->m_soldier.GetData();
		if (!IsValidPTR(pEnemySoldier))
			continue;

		if (stricmp(pEnemySoldier->GetTypeInfo()->m_infoData->name, "ClientSoldierEntity")) // typeinfo checking, do not remove
			continue;

		if (visibleCheck)
		{
			bool visible = !pEnemySoldier->m_occluded;
			if (!visible)
				continue;
		}

		D3DXVECTOR3 enemyVec, enemyVecScreen;
		if (!GetBone(pEnemySoldier, &enemyVec, UpdatePoseResultData::BONE_NECK))
			continue;

		if (!WorldToScreen(enemyVec, &enemyVecScreen))
			continue;

		if (enemyVecScreen.x > 1920 || enemyVecScreen.y > 1080)
			continue;

		if (enemyVecScreen.x < 2.00f && enemyVecScreen.y < 2.00f)
			continue;

		if (!IsInFOV(cameraForward, enemyVec, localPos, targetFOV))
			continue;

		float screenX = DxRenderer::GetInstance()->m_pScreen->m_Width / 2;
		float screenY = DxRenderer::GetInstance()->m_pScreen->m_Height / 2;

		float posX = fabs(enemyVecScreen.x - screenX);
		float posY = fabs(enemyVecScreen.y - screenY);

		float flScreenDist = fabs(sqrtf(posX * posX + posY * posY));

		if (flScreenDist < closestdist)
		{
			closestdist = flScreenDist;
			closestPlayer = pEnemyPlyr;
			closestSoldier = pEnemySoldier;

			return closestPlayer;
		}
	}

	return NULL;
}

static std::stack<ClientPlayer*> players;

std::stack<ClientPlayer*> targetList()
{
	// = new std::stack<ClientPlayer*>;

	auto pGameContext = ClientGameContext::GetInstance();
	if (!IsValidPTR(pGameContext))
		return players;

	auto pPlayerMgr = pGameContext->m_pPlayerManager;
	if (!IsValidPTR(pPlayerMgr))
		return players;

	auto pLocalPlayer = pPlayerMgr->m_pLocalPlayer;
	if (!IsValidPTR(pLocalPlayer))
		return players;

	auto pLocalSoldier = pLocalPlayer->m_soldier.GetData();
	if (!IsValidPTR(pLocalSoldier))
		return players;

	D3DXMATRIX myTrans;
	pLocalSoldier->GetTransform(&myTrans);
	D3DXVECTOR3 myPos = myTrans.m[3];
	D3DXVECTOR3 myForward = myTrans.m[2];

	for (int i = 0; i < pPlayerMgr->getPlayers()->size(); i++)
	{
		auto pPlayer = pPlayerMgr->getPlayers()->at(i);
		if(!IsValidPTR(pPlayer))
			continue;

		if(pPlayer->m_teamID == pLocalPlayer->m_teamID)
			continue;

		auto pSoldier = pPlayer->m_soldier.GetData();
		if(!IsValidPTR(pSoldier))
			continue;

		D3DXMATRIX trans;
		pSoldier->GetTransform(&trans);
		D3DXVECTOR3 pos = trans.m[3];
		D3DXVECTOR3 forward = trans.m[2];
		D3DXVECTOR3 screen;

		if(!WorldToScreen(pos, &screen))
			continue;

		if (IsInFOV(myForward, pos, myPos, 15))
			players.push(pPlayer);
	}

	return players;
}