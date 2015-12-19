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


BOOL __stdcall WardenPreInit()
{
	if(!InitializeCriticalSectionAndSpinCount(&hWarden.WardenLock, 4000))
	{
		LogNoLock("No memory to allocate critical section!");
		return FALSE;
	}

	if(!InitializeCriticalSectionAndSpinCount(&LOG_CS, 4000))
	{
		DeleteCriticalSection(&hWarden.WardenLock);
		LogNoLock("No memory to allocate critical section!");
		return FALSE;
	}

	SetupD2Vars();
	SetupD2Pointers();
	SetupD2Funcs();
	

	Warden_Init();
	if (Warden_Enable == false)
	{
		DeleteCriticalSection(&hWarden.WardenLock);
		DeleteCriticalSection(&LOG_CS);
		LogNoLock("Error during initialization. Warden is turned off");
		return FALSE;
	}

	Log("Available memory can handle %d clients.", hWarden.Clients.max_size());
	Log("Warden initialized successfully.");
	WardenUpTime = GetTickCount();

	return TRUE; 
}


DWORD WINAPI DllMain(HMODULE hModule, int dwReason, void* lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{

			if(!WardenPreInit())
			{

				LogNoLock("Failed to init the Warden :(");
				return FALSE;
			}

		}
		break;
		case DLL_PROCESS_DETACH:
			{
			LogNoLock("Closing threads...");
			if (DumpHandle)
			{
				WaitForSingleObject(DumpHandle, 5000);
				CloseHandle(DumpHandle);
			}

			delete[] MOD_Enrypted; 

			hWarden.Clients.clear();
	
			DeleteCriticalSection(&LOG_CS);
			DeleteCriticalSection(&hWarden.WardenLock);
			DumpHandle = 0;
			LogNoLock("Done!");
		}
		break;
	}
	return TRUE;
}