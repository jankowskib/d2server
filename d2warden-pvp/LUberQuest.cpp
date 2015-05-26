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

DWORD GetXYRange(DWORD x1, DWORD y1, DWORD x2, DWORD y2)
{
	return (DWORD)sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}


void __stdcall UBERQUEST_SpawnMonsters(Game* pGame)
{
	if (!pGame->bUberQuestFlags.bOpenedLilithPortal && !pGame->bUberQuestFlags.bOpenedDurielPortal &&
		!pGame->bUberQuestFlags.bOpenedIzualPortal && !pGame->bUberQuestFlags.bOpenedTristramPortal) {
		D2ASMFuncs::D2GAME_UpdateRoomUnits(pGame);
		return;
	}
	if (pGame->bUberQuestFlags.bSpawnedLilith && pGame->bUberQuestFlags.bSpawnedDuriel && pGame->bUberQuestFlags.bSpawnedIzual
		&& pGame->bUberQuestFlags.bSpawnedDiablo && pGame->bUberQuestFlags.bSpawnedMephisto && pGame->bUberQuestFlags.bSpawnedBaal) {
		D2ASMFuncs::D2GAME_UpdateRoomUnits(pGame);
		return;
	}

	Act* pAct = pGame->pDrlgAct[D2ACT_V];
	static D2POINT hSpawnDuriel = { 0, 0 };
	if (pAct) {
		for (Room1* pRoom = pAct->pRoom1; pRoom; pRoom = pRoom->pRoomNext) {
			if (!(pRoom->nUnknown & 1))
				continue;
			int lvl = D2Funcs.D2COMMON_GetLevelNoByRoom(pRoom);
			switch (lvl) {
			case PANDEMONIUM_RUN_1:
			{
				if (!pGame->bUberQuestFlags.bSpawnedLilith) {

					for (PresetUnit* p = pRoom->pRoom2->pPreset; p; p = p->pPresetNext) {

						if (p->dwType == UNIT_OBJECT && p->dwClassId == OBJ_SPARKLYCHEST) { // Spawn Lilith near Golden Chest automap blob
							DWORD xPos;
							DWORD yPos;

							CoordsInfo* pCoords = D2Funcs.D2COMMON_GetCoordsInfo(pRoom, pRoom->hCoords.dwXStart + p->dwPosX, pRoom->hCoords.dwYStart + p->dwPosY);
							if (D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, pCoords, UBERANDARIEL, &xPos, &yPos, FALSE)) {

								if (D2Funcs.D2GAME_SpawnMonster(UBERANDARIEL, 1, pGame, pRoom, xPos, yPos, -1, 0)) {
									pGame->bUberQuestFlags.bSpawnedLilith = true;
									DEBUGMSG("Spawned Uber Andy!")
								}
							}
							break;
						}

					}
				}
				break;
			}
			case PANDEMONIUM_RUN_2:
			{
				if (!pGame->bUberQuestFlags.bSpawnedDuriel) {
					for (PresetUnit* p = pRoom->pRoom2->pPreset; p; p = p->pPresetNext) {

						if (!hSpawnDuriel.x && p->dwType == UNIT_OBJECT && p->dwClassId == OBJ_WAYPOINT_VALLEYWAYPOINT) { // Spawn Duriel near Waypoint, but not on it
							hSpawnDuriel.x = pRoom->hCoords.dwXStart + p->dwPosX;
							hSpawnDuriel.y = pRoom->hCoords.dwYStart + p->dwPosY;
							continue;
						}
					}
					if (hSpawnDuriel.x && hSpawnDuriel.y && (GetXYRange(hSpawnDuriel.x, hSpawnDuriel.y, pRoom->hCoords.dwXStart, pRoom->hCoords.dwYStart) > 50))
					{
						DWORD xPos;
						DWORD yPos;

						CoordsInfo* pCoords = D2Funcs.D2COMMON_GetCoordsInfo(pRoom, (pRoom->hCoords.dwXStart + pRoom->hCoords.dwYSize / 2), (pRoom->hCoords.dwYStart + pRoom->hCoords.dwYSize) / 2);
						if (D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, pCoords, UBERDURIEL, &xPos, &yPos, FALSE)) {

							if (D2Funcs.D2GAME_SpawnMonster(UBERDURIEL, 1, pGame, pRoom, xPos, yPos, -1, 0)) {
								pGame->bUberQuestFlags.bSpawnedDuriel = true;
								DEBUGMSG("Spawned Uber Duriel!")
							}
						}
						break;
					}


				}
				break;
			}
			case PANDEMONIUM_RUN_3:
			{
				if (!pGame->bUberQuestFlags.bSpawnedIzual) {
					for (PresetUnit* p = pRoom->pRoom2->pPreset; p; p = p->pPresetNext) {

						if (p->dwType == UNIT_OBJECT && p->dwClassId == OBJ_SPARKLYCHEST) { // Spawn Izual near Golden Chest automap blob
							DWORD xPos;
							DWORD yPos;

							CoordsInfo* pCoords = D2Funcs.D2COMMON_GetCoordsInfo(pRoom, pRoom->hCoords.dwXStart + p->dwPosX, pRoom->hCoords.dwYStart + p->dwPosY);
							if (D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, pCoords, UBERIZUAL, &xPos, &yPos, FALSE)) {

								if (D2Funcs.D2GAME_SpawnMonster(UBERIZUAL, 1, pGame, pRoom, xPos, yPos, -1, 0)) {
									pGame->bUberQuestFlags.bSpawnedIzual = true;
									DEBUGMSG("Spawned Uber Izual!")
								}
							}
							break;
						}

					}
				}
				break;
			}
			case UBER_TRISTRAM:
			{
				if (!pGame->bUberQuestFlags.bSpawnedBaal) {
					D2POINT Out = { 0, 0 };
					BOOL found = D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, 0, UBERBAAL, (DWORD*)&Out.x, (DWORD*)&Out.y, 1);
					if (!found)
						continue;


					if (D2Funcs.D2GAME_SpawnMonster(UBERBAAL, 1, pGame, pRoom, Out.x, Out.y, -1, 0)) {
						pGame->bUberQuestFlags.bSpawnedBaal = true;
						DEBUGMSG("Spawned Uber Baal!")
					}
					else continue;
				}
				if (!pGame->bUberQuestFlags.bSpawnedMephisto) {
					D2POINT Out = { 0, 0 };
					BOOL found = D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, 0, UBERMEPHISTO, (DWORD*)&Out.x, (DWORD*)&Out.y, 1);
					if (!found)
						continue;


					if (D2Funcs.D2GAME_SpawnMonster(UBERMEPHISTO, 1, pGame, pRoom, Out.x, Out.y, -1, 0)) {
						pGame->bUberQuestFlags.bSpawnedMephisto = true;
						DEBUGMSG("Spawned Uber Meph!")
					}
					else continue;
				}
				if (!pGame->bUberQuestFlags.bSpawnedDiablo) {
					D2POINT Out = { 0, 0 };
					BOOL found = D2Funcs.D2GAME_FindMonsterRoom(pGame, pRoom, 0, UBERDIABLO, (DWORD*)&Out.x, (DWORD*)&Out.y, 1);
					if (!found)
						continue;


					if (D2Funcs.D2GAME_SpawnMonster(UBERDIABLO, 1, pGame, pRoom, Out.x, Out.y, -1, 0)) {
						pGame->bUberQuestFlags.bSpawnedDiablo = true;
						DEBUGMSG("Spawned Uber D2!")
					}
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
		if (!pGame->bUberQuestFlags.bOpenedLilithPortal)
			quest_levels.push_back(PANDEMONIUM_RUN_1);
		if (!pGame->bUberQuestFlags.bOpenedDurielPortal)
			quest_levels.push_back(PANDEMONIUM_RUN_2);
		if (!pGame->bUberQuestFlags.bOpenedIzualPortal)
			quest_levels.push_back(PANDEMONIUM_RUN_3);

		if (quest_levels.size() == 0)
			return FALSE;

		DWORD levelToSpawn = quest_levels[rand() % quest_levels.size()];

		if (!QUESTS_OpenPortal(pGame, pPlayer, levelToSpawn))
			return FALSE;


		if (levelToSpawn == PANDEMONIUM_RUN_1) {
			pGame->bUberQuestFlags.bOpenedLilithPortal = TRUE;
		}
		else if (levelToSpawn == PANDEMONIUM_RUN_2)
			pGame->bUberQuestFlags.bOpenedDurielPortal = TRUE;
		else if (levelToSpawn == PANDEMONIUM_RUN_3)
			pGame->bUberQuestFlags.bOpenedIzualPortal = TRUE;

		
		if (levelToSpawn)
		
		return TRUE;
	}

	return FALSE;
}

BOOL UBERQUEST_OpenPandemoniumFinalPortal(Game *pGame, UnitAny *pPlayer)
{
	if (pGame->DifficultyLevel != DIFF_HELL || pGame->bUberQuestFlags.bOpenedTristramPortal) // Open only in hell
		return FALSE;

	Room1* pRoom = D2Funcs.D2COMMON_GetUnitRoom(pPlayer);
	if (pRoom)
	{
		DWORD aLvl = D2Funcs.D2COMMON_GetLevelNoByRoom(pRoom);
		if (aLvl != HARROGATH)
			return FALSE;

		if (!QUESTS_OpenPortal(pGame, pPlayer, UBER_TRISTRAM))
			return FALSE;

		pGame->bUberQuestFlags.bOpenedTristramPortal = TRUE;
		return TRUE;
	}
	return FALSE;
}

