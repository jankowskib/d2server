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

#include "LWorldEvent.h"

BOOL __stdcall WardenPreInit()
{
	SetupD2Vars();
	SetupD2Pointers();
	SetupD2Funcs();
	

	if (!Warden::getInstance(__FUNCTION__).isInited())
	{
		Log("Error during initialization. Stopping...");
		return FALSE;
	}

	if (!Warden::getInstance(__FUNCTION__).NextDC)
		WE_GenerateNextDC();

	Log("Warden initialized successfully.");

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
				Log("Failed to init the Warden :(");
				return FALSE;
			}

		}
		break;
		case DLL_PROCESS_DETACH:
		{
			Log("Server shutting down");
		}
		break;
	}
	return TRUE;
}