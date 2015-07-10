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
#include "LCube.h"
#include "LQuests.h"
#include "LUberQuest.h"

/*
	Wrapper in D2Game.0x6FCB70FC
*/
DWORD __stdcall CUBE_OnCustomFunc(Game* pGame, UnitAny* pPlayer, CubeOutputItem *pCubeOutput)
{
	if (pCubeOutput->Type == CUBEOUTPUT_PORTAL) { // Portal
		if (pCubeOutput->nLevel >= (*D2Vars.D2COMMON_sgptDataTables)->dwLevelsRecs) // Invalid level has set
			return FALSE;
		Room1* pRoom = D2Funcs.D2COMMON_GetUnitRoom(pPlayer);
		ASSERT(pRoom)

		if (pCubeOutput->nAct != pRoom->pAct->dwAct) {
			QUESTS_UpdateUnit(pPlayer, 19, pPlayer);
			return FALSE;
		}

		return QUESTS_OpenPortal(pGame, pPlayer, pCubeOutput->nLevel);
	}
	else if (pCubeOutput->Type == CUBEOUTPUT_COWPORTAL) {
		return QUESTS_CowLevelOpenPortal(pGame, pPlayer);
	}
	else if (pCubeOutput->Type == CUBEOUTPUT_UBERQUEST) { // Uber quest key portals
		if (!UBERQUEST_OpenPandemoniumPortal(pGame, pPlayer)) {
			QUESTS_UpdateUnit(pPlayer, 19, pPlayer);
			return FALSE;
		}
		return TRUE;
	}
	else if (pCubeOutput->Type == CUBEOUTPUT_UBERQUEST_FINAL) { // Uber quest tristram
		if(!UBERQUEST_OpenPandemoniumFinalPortal(pGame, pPlayer)) {
			QUESTS_UpdateUnit(pPlayer, 19, pPlayer);
			return FALSE;
		}
		return TRUE;
	}
	return -1; // Invalid function
}

