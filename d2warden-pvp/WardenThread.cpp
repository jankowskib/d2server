/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2011-2013 Bartosz Jankowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================== */

#include "stdafx.h"

#define __DEFINE_EXPTRS
#ifdef VER_113D
#include "ExPointers_113D.h"
#else
#include "ExPointers_111B.h"
#endif

#include "D2Warden.h"
#include "Misc.h"
#include "process.h"

static HANDLE WardenThread = 0;
static unsigned Id = 0;
static volatile bool gWardenRunning = true;

unsigned __stdcall d2warden_thread(void *lpParameter)
{
	if(!InitializeCriticalSectionAndSpinCount(&hWarden.WardenLock, 4000))
	{
#ifdef _ENGLISH_LOGS
		LogNoLock("No memory to allocate critical section!");
#else
		LogNoLock("Brak pamieci na alokacje sekcji krytycznej!");
#endif
		return -1;
	}
	if(!InitializeCriticalSectionAndSpinCount(&LOG_CS, 4000))
	{
		DeleteCriticalSection(&hWarden.WardenLock);
#ifdef _ENGLISH_LOGS
		LogNoLock("No memory to allocate critical section!");
#else
		LogNoLock("Brak pamieci na alokacje sekcji krytycznej!");
#endif
		return -1;
	}


	SetupD2Vars();
	SetupD2Pointers();
	SetupD2Funcs();
	
	Warden_Init();
	if (Warden_Enable == false)
	{
		DeleteCriticalSection(&LOG_CS);
#ifdef _ENGLISH_LOGS
		LogNoLock("Error during initialization. Warden is turned off");
#else
		LogNoLock("Blad podczas inicjalizacji. Warden wylaczony.");
#endif
		return -1;
	}
#ifdef _ENGLISH_LOGS
	Log("Available memory can handle %d clients.", hWarden.Clients.max_size());
	Log("Warden initialized successfully.");
#else
	Log("Dostepna pamiec wystarczy na %d klientow.", hWarden.Clients.max_size());
	Log("Warden pomyslnie zainicjowany.");
#endif


	WardenUpTime = GetTickCount();
	//while (WaitForSingleObject(hEvent, 0) != WAIT_OBJECT_0) 
	while(gWardenRunning)
	{
		if(!isWardenQueueEmpty()) 
			WaitForSingleObject(hWardenCheckEvent, WardenLoop());
	}
#ifdef _ENGLISH_LOGS
	LogNoLock("End of main thread!");
#else
	LogNoLock("Koniec watku glownego!");
#endif
	return 0; 
}


DWORD WINAPI DllMain(HMODULE hModule, int dwReason, void* lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
		//hEvent = CreateEvent(NULL, TRUE, FALSE, "WARDEN_END");
			hWardenCheckEvent = CreateEvent(NULL, FALSE, FALSE, "WARDEN_CHECK");

			if (!hWardenCheckEvent)
				{ 
		#ifdef _ENGLISH_LOGS
				LogNoLock("Couldn't initialize events. Error id: %d!", errno); 
		#else
				LogNoLock("Nie moge zainicjowac eventu. Blad %d!", errno); 
		#endif
				return FALSE;
				}

			if(!WardenThread) WardenThread = (HANDLE)_beginthreadex(0,0,&d2warden_thread,0,0,&Id);
			if(!WardenThread)
				{ 
		#ifdef _ENGLISH_LOGS
				LogNoLock("Couldn't initialize thread. Error id: %d!", errno);
		#else
				LogNoLock("Nie moge zainicjowac thread. Blad %d!", errno);
		#endif
				return FALSE;
				}
			}
		break;
		case DLL_PROCESS_DETACH:
			{
		#ifdef _ENGLISH_LOGS
			LogNoLock("Closing threads...");
		#else
			LogNoLock("Trwa zamykanie watkow...");
		#endif
			LOCK
			gWardenRunning = false;
			SetEvent(hWardenCheckEvent);
			UNLOCK
			//SetEvent(hEvent);
			WaitForSingleObject(WardenThread,5000);
			if (DumpHandle)
				WaitForSingleObject(DumpHandle,5000);
			if (hEvent)
				CloseHandle(hEvent);
			CloseHandle(WardenThread);
			CloseHandle(hWardenCheckEvent);
			if (DumpHandle)
				CloseHandle(DumpHandle);

			delete[] MOD_Enrypted; 

			hWarden.Clients.clear();
	
			DeleteCriticalSection(&LOG_CS);
			DeleteCriticalSection(&hWarden.WardenLock);
			hEvent = 0;
			hWardenCheckEvent = 0;
			WardenThread = 0;
			DumpHandle = 0;
			LogNoLock("Done!");
		}
		break;
	}
	return TRUE;
}