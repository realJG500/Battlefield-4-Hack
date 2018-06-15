#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "Hack Functions.h"
#include "VMProtectSDK.h"
#include "detours.h"
#include "Aimbot.h"
#include "AutoSpot.h"
#include "ChatSpammer.h"
#include "LagSwitch.h"
#include "Util.h"
#include "ESP.h"
#include "DamageStream.h"
#include "Loadout.h"
#include "WeaponModifier.h"

#pragma comment (lib, "detours.lib")

static bool uninject = false;
static HMODULE hModule;
static bool menu = true;
static bool visible = true;

typedef int(*tPreFrameUpdate)(float dt);
tPreFrameUpdate oPreFrame;

typedef int(*tPresent)(void*, UINT, UINT);
tPresent oPresent;

int hkPreFrame(float DeltaTime)
{
	float rValue = 0.0f; // oPreFrame(DeltaTime);

	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) vehicleAimbot();
	if (GetAsyncKeyState(0xA4) & 0x8000) Aimbot(visible);
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) magicBullet(0);
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) autoHeal();

	/*if (m.getMenuObjectbyIndex(21).enabled && GetAsyncKeyState(0x54)) teleport();
	if (m.getMenuObjectbyIndex(22).enabled && GetAsyncKeyState(VK_MBUTTON)) superJump();
	else if (!GetAsyncKeyState(VK_MBUTTON) && !GetAsyncKeyState(0x54) && !GetAsyncKeyState(0x20)) unpatchVaultCheck();*/

	noSpread();
	noBreath();

	return rValue;
}

int hkPresent(void* pSwapChain, UINT a1, UINT a2)
{
	DebugRenderer2::Singleton()->drawText(DxRenderer::GetInstance()->m_pScreen->m_Width / 2 - 110, 10, Color32(255, 0, 0, 100), "Battlefield 4 Hax", 2.0f);

	ESP(true);
	showChams();

	char* output = new char[30];
	snprintf(output, 30, "Aimbot Visible Check: %d", visible);
	
	DebugRenderer2::Singleton()->drawText(DxRenderer::GetInstance()->m_pScreen->m_Width - 400,
		DxRenderer::GetInstance()->m_pScreen->m_Height / 2,
		Color32(255, 255, 255, 100),
		const_cast<char*>(output),
		2.0f);

	WorldRenderSettings* pWRS = WorldRenderSettings::Singleton();
	if (IsValidPTR(pWRS))
	{
		pWRS->m_DrawVeggies = false;
		pWRS->m_LensFlaresEnable = false;
		pWRS->m_MotionBlurEnable = false;
	}

	return oPresent(pSwapChain, a1, a2);
}

void pingSpoofThread()
{
	for (;;)
	{
		pingSpoof();
		Sleep(2000);
	}
}

void spottingThread()
{
	while (1)
	{
		if (uninject)
			return;

		autoSpot();
		Sleep(2800);
	}
}

void Thread2()
{
	for (;;)
	{
		createSquadMessage();
		Sleep(5);
	}
}

void InputThread()
{
	printf("Johnny Sins MULTIHACK\n");
	printf("DO NOT DISTRIBUTE\n");

	for (int i = 0; i < 80; i++)
		printf("-");
	printf("\n\n");

	//disableHardcore();
	srand((unsigned)time(NULL));

	for (;;)
	{
		if (GetAsyncKeyState(VK_END))
		{
			menu = !menu;
			Sleep(200);
		}

		/*if (GetAsyncKeyState(VK_NUMPAD6))
		{
			showMouse = !showMouse;
			printf("Cursor mode changed\n");
			Sleep(175);
		}

		if (showMouse)
		{
			auto pBorderInput = BorderInputNode::GetInstance();
			if (!IsValidPTR(pBorderInput))
				return;

			auto pMouse = pBorderInput->m_pMouse;
			if (!IsValidPTR(pMouse))
				return;

			pMouse->m_pDevice->m_ShowCursor = true;
			pMouse->m_pDevice->m_CursorMode = true;
			pMouse->m_pDevice->m_UIOwnsInput = true;
		}

		else if (!showMouse)
		{
			auto pBorderInput = BorderInputNode::GetInstance();
			if (!IsValidPTR(pBorderInput))
				return;

			auto pMouse = pBorderInput->m_pMouse;
			if (!IsValidPTR(pMouse))
				return;

			pMouse->m_pDevice->m_ShowCursor = false;
			pMouse->m_pDevice->m_CursorMode = false;
			pMouse->m_pDevice->m_UIOwnsInput = false;
		}*/

		if (GetAsyncKeyState(VK_NUMPAD1))
		{
			votekickSpam();
			Sleep(200);
		}

		if (GetAsyncKeyState(VK_F10))
		{
			HookVTableFunction((PDWORD64*)BorderInputNode::GetInstance()->vtable_class_40, (PBYTE)oPreFrame, 3);
			HookVTableFunction((PDWORD64*)DxRenderer::GetInstance()->m_pScreen->m_pSwapChain, (PBYTE)oPresent, 8);
			unhookRecoil();

			printf("\nUninjecting in 6 second(s)...\n");
			uninject = true;
			Sleep(6000);
			FreeConsole();
			FreeLibraryAndExitThread(hModule, 0);
			uninject = false;
		}

		if (GetAsyncKeyState(VK_F5))
		{
			Beep(750, 350);
			printf("[BLACKLIST] Enter player name:\t");
			char* input = new char[30];
			scanf("%s", input);
			addToShitList(input);
			delete input;
		}

		if (GetAsyncKeyState(VK_XBUTTON2))
		{
			visible = !visible;
			Sleep(200);
		}

		if (GetAsyncKeyState(VK_NUMPAD3))
		{
			applyMBT();
			applyIFV();
			applyMobileAA();
			Sleep(200);
		}

		if (GetAsyncKeyState(VK_NUMPAD4))
		{
			applyRorsche();
			applyInvisibility();
			Sleep(200);
		}

		if (GetAsyncKeyState(VK_NUMPAD5))
		{
			applyMKV();
			Sleep(200);
		}

		if (GetAsyncKeyState(VK_F12))
		{
			ruleSpam();
			Sleep(200);
		}

		Sleep(5);
	}
}

BOOL WINAPI DllMain(HINSTANCE HMODULE, DWORD dwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(HMODULE);
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hModule = HMODULE;
		/*char name[MAX_PATH];
		GetModuleFileNameA(HMODULE, name, MAX_PATH);

		strcpy(strstr(name, ".dll"), ".key");
		FILE* f = fopen(name, "r");

		char key[512];
		ZeroMemory(key, 512);
		char* pos = key;
		int c;

		while ((c = getc(f)) != EOF)
		{
			if (iscntrl(c))
				continue;
			*pos = c;
			pos++;
		}
		fclose(f);

		VMProtectSetSerialNumber(key);
		VMProtectSerialNumberData data;
		if (VMProtectGetSerialNumberData(&data, sizeof(VMProtectSerialNumberData)))
		{
			wprintf(L"Authorized as %s\n\n", data.wUserName);
		}*/

		/*DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)oVeniceNetworkRequestHardwareIdMessage, &hkVeniceNetworkRequestHardwareIdMessage);
		if (DetourTransactionCommit() != NO_ERROR) MessageBox(0, "Error: Couldn't install hook (1)", "Error", 0);

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)oTakeScreenshotByDxRenderer, &hkTakeScreenshotByDxRenderer);
		if (DetourTransactionCommit() != NO_ERROR) MessageBox(0, "Error: Couldn't install hook (2)", "Error", 0);

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)oTakeScreenshotByHwnd, &hkTakeScreenshotByHwnd);
		if (DetourTransactionCommit() != NO_ERROR) MessageBox(0, "Error: Couldn't install hook (3)", "Error", 0);

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)VeniceNetworkSetPingMessage, &hk__VeniceNetworkSetPingMessage);
		if (DetourTransactionCommit() != NO_ERROR) MessageBox(0, "Error: Couldn't install hook (4)", "Error", 0);*/

		/*DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttachEx(&(PVOID&)fb__transmitPacket, &hk__fb__transmitPacket, (PDETOUR_TRAMPOLINE*)&tmp_transmitPacket, 0, 0);
		DetourTransactionCommit();
		printf("[+] Detoured: Transmit Packet\n");*/

		/*DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttachEx(&(PVOID&)fb__WriteMoves, &hk_fb__WriteMoves, (PDETOUR_TRAMPOLINE*)&trampoline__WriteMoves, 0, 0);
		DetourTransactionCommit();
		printf("[+] WriteMoves detoured\n");*/

		/*DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)sub_140243FC0, &hk_sub_140243FC0);
		DetourTransactionCommit();*/

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InputThread, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)spottingThread, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pingSpoofThread, 0, 0, 0);
		oPreFrame = (tPreFrameUpdate)HookVTableFunction((PDWORD64*)BorderInputNode::GetInstance()->vtable_class_40, (PBYTE)&hkPreFrame, 3);
		oPresent = (tPresent)HookVTableFunction((PDWORD64*)DxRenderer::GetInstance()->m_pScreen->m_pSwapChain, (PBYTE)&hkPresent, 8);
		*(DWORD_PTR*)(SSMODULE) = 0;
	}
	return TRUE;
}