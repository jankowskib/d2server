/* ==========================================================
* d2warden
* https://github.com/lolet/d2warden
* ==========================================================
* Copyright 2011-2015 Bartosz Jankowski
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
#include "LUberQuest.h"
#include "LQuests.h"

#include <vector>

using namespace std;

void __stdcall UBERQUEST_SpawnMonsters(Game* pGame)
{
	if (!pGame->bOpenedLilithPortal && !pGame->bOpenedDurielPortal && !pGame->bOpenedIzualPortal && !pGame->bOpenedTristramPortal) {
		D2ASMFuncs::D2GAME_UpdateRoomUnits(pGame);
		return;
	}
	if (pGame->bSpawnedLilith && pGame->bSpawnedDuriel && pGame->bSpawnedIzual) {
		D2ASMFuncs::D2GAME_UpdateRoomUnits(pGame);
		return;
	}

	Act* pAct = pGame->pDrlgAct[D2ACT_V];
	if (pAct) {
		for (Room1* pRoom = pAct->pRoom1; pRoom; pRoom = pRoom->pRoomNext) {
			if (!(pRoom->nUnknown & 1))
				continue;
			int lvl = D2Funcs.D2COMMON_GetLevelNoByRoom(pRoom);
			switch (lvl) {
			case PANDEMONIUM_RUN_1:
			{
				if (!pGame->bSpawnedLilith) {

					D2POINT Out = { 0, 0 };
					BOOL found = D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, 0, UBERANDARIEL, (DWORD*)&Out.x, (DWORD*)&Out.y, 1);
					if (!found)
						continue;
					if (D2Funcs.D2GAME_SpawnMonster(UBERANDARIEL, 1, pGame, pRoom, Out.x, Out.y, -1, 0))
						pGame->bSpawnedLilith = true;
					else continue;
				}
				break;
			}
			case PANDEMONIUM_RUN_2:
			{
				if (!pGame->bSpawnedDuriel) {
					D2POINT Out = { 0, 0 };
					BOOL found = D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, 0, UBERDURIEL, (DWORD*)&Out.x, (DWORD*)&Out.y, 1);
					if (!found)
						continue;

					if (D2Funcs.D2GAME_SpawnMonster(UBERDURIEL, 1, pGame, pRoom, Out.x, Out.y, -1, 0))
						pGame->bSpawnedDuriel = true;
					else continue;
				}
				break;
			}
			case PANDEMONIUM_RUN_3:
			{
				if (!pGame->bSpawnedIzual) {
					D2POINT Out = { 0, 0 };
					BOOL found = D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, 0, UBERIZUAL, (DWORD*)&Out.x, (DWORD*)&Out.y, 1);
					if (!found)
						continue;


					if (D2Funcs.D2GAME_SpawnMonster(UBERIZUAL, 1, pGame, pRoom, Out.x, Out.y, -1, 0))
						pGame->bSpawnedIzual = true;
					else continue;
				}
				break;
			}
			case UBER_TRISTRAM:
			{
				if (!pGame->bSpawnedBaal) {
					D2POINT Out = { 0, 0 };
					BOOL found = D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, 0, UBERBAAL, (DWORD*)&Out.x, (DWORD*)&Out.y, 1);
					if (!found)
						continue;


					if (D2Funcs.D2GAME_SpawnMonster(UBERBAAL, 1, pGame, pRoom, Out.x, Out.y, -1, 0))
						pGame->bSpawnedBaal = true;
					else continue;
				}
				if (!pGame->bSpawnedMephisto) {
					D2POINT Out = { 0, 0 };
					BOOL found = D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, 0, UBERMEPHISTO, (DWORD*)&Out.x, (DWORD*)&Out.y, 1);
					if (!found)
						continue;


					if (D2Funcs.D2GAME_SpawnMonster(UBERMEPHISTO, 1, pGame, pRoom, Out.x, Out.y, -1, 0))
						pGame->bSpawnedMephisto = true;
					else continue;
				}
				if (!pGame->bSpawnedDiablo) {
					D2POINT Out = { 0, 0 };
					BOOL found = D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, 0, UBERDIABLO, (DWORD*)&Out.x, (DWORD*)&Out.y, 1);
					if (!found)
						continue;


					if (D2Funcs.D2GAME_SpawnMonster(UBERDIABLO, 1, pGame, pRoom, Out.x, Out.y, -1, 0))
						pGame->bSpawnedDiablo = true;
					else continue;
				}
				break;
			}
			}
		}
	}

	D2ASMFuncs::D2GAME_UpdateRoomUnits(pGame);
}

BOOL UBERQUEST_OpenPandemoniumPortal(Game *pGame, UnitAny *pPlayer)
{
	if (pGame->DifficultyLevel != DIFF_HELL) // Open only in hell
		return FALSE;
	Room1* pRoom = D2Funcs.D2COMMON_GetUnitRoom(pPlayer);
	if (pRoom)
	{
		DWORD aLvl = D2Funcs.D2COMMON_GetLevelNoByRoom(pRoom);
		if (aLvl != HARROGATH)
			return FALSE;

		vector<DWORD> quest_levels;
		if (!pGame->bOpenedLilithPortal)
			quest_levels.push_back(PANDEMONIUM_RUN_1);
		if (!pGame->bOpenedDurielPortal)
			quest_levels.push_back(PANDEMONIUM_RUN_2);
		if (!pGame->bOpenedIzualPortal)
			quest_levels.push_back(PANDEMONIUM_RUN_3);

		if (quest_levels.size() == 0)
			return FALSE;

		DWORD levelToSpawn = quest_levels[rand() % quest_levels.size()];

		if (!QUESTS_OpenPortal(pGame, pPlayer, levelToSpawn))
			return FALSE;


		if (levelToSpawn == PANDEMONIUM_RUN_1) {
			pGame->bOpenedLilithPortal = TRUE;
		}
		else if (levelToSpawn == PANDEMONIUM_RUN_2)
			pGame->bOpenedDurielPortal = TRUE;
		else if (levelToSpawn == PANDEMONIUM_RUN_3)
			pGame->bOpenedIzualPortal = TRUE;

		
		if (levelToSpawn)
		
		return TRUE;
	}

	return FALSE;
}

BOOL UBERQUEST_OpenPandemoniumFinalPortal(Game *pGame, UnitAny *pPlayer)
{
	if (pGame->DifficultyLevel != DIFF_HELL || pGame->bOpenedTristramPortal) // Open only in hell
		return FALSE;

	Room1* pRoom = D2Funcs.D2COMMON_GetUnitRoom(pPlayer);
	if (pRoom)
	{
		DWORD aLvl = D2Funcs.D2COMMON_GetLevelNoByRoom(pRoom);
		if (aLvl != HARROGATH)
			return FALSE;

		if (!QUESTS_OpenPortal(pGame, pPlayer, UBER_TRISTRAM))
			return FALSE;

		pGame->bOpenedTristramPortal = TRUE;
		return TRUE;
	}
	return FALSE;
}

