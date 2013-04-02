#include "stdafx.h"
#include "WardenThread.h"

#define _DEFINE_PTRS
#include "D2Ptrs.h"
#undef _DEFINE_PTRS

#include "D2Warden.h"
#include "WardenMisc.h"
#include "Vars.h"
#include "process.h"

static HANDLE WardenThread = 0;
static unsigned Id = 0;

unsigned __stdcall d2warden_thread(void *lpParameter)
{
	if(!InitializeCriticalSectionAndSpinCount(&hWarden.WardenLock, 4000))
	{
		LogNoLock("Brak pamieci na alokacje sekcji krytycznej!");
		return -1;
	}
	if(!InitializeCriticalSectionAndSpinCount(&LOG_CS, 4000))
	{
		DeleteCriticalSection(&hWarden.WardenLock);
		LogNoLock("Brak pamieci na alokacje sekcji krytycznej!");
		return -1;
	}


	DefineOffsets();

	Warden_Init();
	if (Warden_Enable == false)
	{
		DeleteCriticalSection(&LOG_CS);
		DeleteCriticalSection(&hWarden.WardenLock);
		LogNoLock("Blad podczas inicjalizacji. Warden wylaczony.");
		return -1;
	}
	Log("Dostepna pamiec wystarczy na %d klientow.", hWarden.Clients.max_size());

	Log("Warden pomyslnie zainicjowany.");
	WardenUpTime = GetTickCount();

	while (WaitForSingleObject(hEvent, 0) != WAIT_OBJECT_0) 
	{
		WardenLoop();
		Sleep(1);
	}

	delete[] MOD_Enrypted; 
	LOCK
	hWarden.Clients.clear();
	UNLOCK
	LogNoLock("Koniec watku glownego!");
	DeleteCriticalSection(&LOG_CS);
	return 0; 
}

DWORD WINAPI DllMain(HMODULE hModule, int dwReason, void* lpReserved)
{
switch (dwReason)
{
case DLL_PROCESS_ATTACH:
	{
	hEvent = CreateEvent(NULL, TRUE, FALSE, "WARDEN_END");
	if(!hEvent)
		{ 
		LogNoLock("Nie moge zainicjowac eventu. Blad %d!", errno); 
		return FALSE;
		}

	if(!WardenThread) WardenThread = (HANDLE)_beginthreadex(0,0,&d2warden_thread,0,0,&Id);
	if(!WardenThread)
		{ 
		LogNoLock("Nie moge zainicjowac watku. Blad %d!", errno); 
		return FALSE;
		}
	}
break;
case DLL_PROCESS_DETACH:
	{
	LogNoLock("Trwa zamykanie watkow...");
	SetEvent(hEvent);
	WaitForSingleObject(WardenThread,5000);
	WaitForSingleObject(DumpHandle,5000);
	CloseHandle(hEvent);
	CloseHandle(WardenThread);
	CloseHandle(DumpHandle);

	DeleteCriticalSection(&hWarden.WardenLock);
	hEvent = 0;
	WardenThread = 0;
	DumpHandle = 0;
	}
break;
}
return TRUE;
}