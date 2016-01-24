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
#include "LMonsters.h"


UnitAny* __fastcall OnMonsterSpawn(PresetMonster * pSetup)
{
	UnitAny* pMonster =  D2Funcs.D2GAME_SpawnPresetMonster(pSetup);
	if (pMonster)
	{
		DEBUGMSG("Created monster %d on [%d, %d]!", pMonster->dwClassId, D2Funcs.D2GAME_GetUnitX(pMonster), D2Funcs.D2GAME_GetUnitY(pMonster))
	}
	return pMonster;
}

bool ParseMonCmds(UnitAny* pUnit, char* str, char *t)
{
	int ClientID = pUnit->pPlayerData->pClientData->ClientID;


	if (_stricmp(str, "#mspawntest") == 0) 
	{
		if (!isAnAdmin(pUnit->pPlayerData->pClientData->AccountName))  return TRUE;
		DWORD nMonsters = (*D2Vars.D2COMMON_sgptDataTables)->dwMonStatsRecs;

		str = strtok_s(NULL, " ", &t);
		if (!str) {
			SendMsgToClient(pUnit->pPlayerData->pClientData, "Type monster index 0-%d", nMonsters);
			return false;
		}

		DWORD monIdx = atoi(str);
		if (!str) {
			SendMsgToClient(pUnit->pPlayerData->pClientData, "Type monster index 0-%d", nMonsters);
			return false;
		}

		Room1* pRoom = D2Funcs.D2COMMON_GetUnitRoom(pUnit);
		if (pRoom) {
			DWORD xPos = pUnit->pPath->xPos;
			DWORD yPos = pUnit->pPath->yPos;

			CoordsInfo* pCoords = D2Funcs.D2COMMON_GetCoordsInfo(pRoom, xPos, yPos);
			if (D2Funcs.D2GAME_FindMonsterRoom(pUnit->pGame, pRoom, pCoords, monIdx, &xPos, &yPos, FALSE)) {

				if (D2Funcs.D2GAME_SpawnMonster(monIdx, 1, pUnit->pGame, pRoom, xPos, yPos, -1, 0))
				{
					SendMsgToClient(pUnit->pPlayerData->pClientData, "Done!");
				}
			}
			else
			{
				SendMsgToClient(pUnit->pPlayerData->pClientData, "D2GAME_FindMonsterRoom() failed!");
			}
		}

		return false;
	}
	if(_stricmp(str,"#mspawn")==0)
	{
		WardenClient_i ptCurrentClient = Warden::getInstance().findClientById(ClientID);
		if (ptCurrentClient == Warden::getInstance().getInvalidClient()) return TRUE;

		if (!isAnAdmin(ptCurrentClient->AccountName)) { return TRUE; }


		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-733"); return false;}
		int No = atoi(str);
		if(No>733)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-733"); return false;}

		int count = 1;
		WardenClient_i psUnit = ptCurrentClient;

		Room1* aRoom = pUnit->pPath->pRoom1;
		int xPos = pUnit->pPath->xPos;
		int yPos = pUnit->pPath->yPos;

		str = strtok_s(NULL," ",&t);
		if(str) 
		{
		count = atoi(str);
		str = strtok_s(NULL," ",&t);
		if(str)
		{
			WardenClient_i psUnit = Warden::getInstance().findClientByName(str);
			if (psUnit != Warden::getInstance().getInvalidClient())
			{
			SendMsgToClient(pUnit->pPlayerData->pClientData,"Spawning monster on %s", psUnit->CharName);
			aRoom = psUnit->ptPlayer->pPath->pRoom1;
			xPos = psUnit->ptPlayer->pPath->xPos;
			yPos = psUnit->ptPlayer->pPath->yPos;
			}
		}
		}

		
		D2POINT Pos = { xPos, yPos };
		D2POINT Out = { 0, 0 };

		UnitAny* ptMonster = 0;

		for(int z= 0; z<count; z++)
		{
		for(int i = 0; i<10; i++)
		{

		aRoom =	D2ASMFuncs::D2GAME_FindFreeCoords(&Pos,aRoom,&Out,1);
		if(!aRoom) {SendMsgToClient(pUnit->pPlayerData->pClientData,"FindFreeCoords failed!"); break;}

		ptMonster = D2Funcs.D2GAME_SpawnMonster(No,1,psUnit->ptPlayer->pGame,aRoom,Out.x,Out.y,-1,0);
		if(ptMonster) break;

		if(i % 2) Pos.x = Out.x + (rand() % 4); else  Pos.x = Out.x - (rand() % 4);
		if(i % 2) Pos.y = Out.y + (rand() % 4); else  Pos.y = Out.y - (rand() % 4);
		}
		if(!ptMonster) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Error limit exceeded!"); break;}

		SendMsgToClient(pUnit->pPlayerData->pClientData,"#%d Monster spawned, HP = %d",z+1, D2Funcs.D2COMMON_GetUnitMaxLife(ptMonster) >> 8);	
		}
		
		return false;
		}

	return true;
}