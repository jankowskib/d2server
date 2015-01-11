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

	if (_stricmp(str, "#mspawnxy") == 0)
	{
		str = strtok_s(NULL, " ", &t);
		if (!str) { SendMsgToClient(pUnit->pPlayerData->pClientData, "Type number 0-733"); UNLOCK return false; }
		int No = atoi(str);
		if (No>733)  { SendMsgToClient(pUnit->pPlayerData->pClientData, "Type number 0-733"); UNLOCK return false; }

		str = strtok_s(NULL, " ", &t);
		if (!str) { SendMsgToClient(pUnit->pPlayerData->pClientData, "#mspawnxy <id> <x> <y>"); return false; }
		int nX = atoi(str);
		str = strtok_s(NULL, " ", &t);
		if (!str) { SendMsgToClient(pUnit->pPlayerData->pClientData, "#mspawnxy <id> <x> <y>"); return false; }
		int nY = atoi(str);

		POINT Pos = { nX, nY };
		POINT Out = { 0, 0 };



		ptMonster = 0;

		for (int i = 0; i<10; i++)
		{
			Room1 * mRoom = 0;

			for (Room1* dRoom = pUnit->pAct->pRoom1; dRoom; dRoom = dRoom->pRoomNext)
			{
				if (dRoom->dwXStart < Pos.x && (dRoom->dwXStart + dRoom->dwXSize) >= Pos.x &&
					dRoom->dwYStart < Pos.y && (dRoom->dwYStart + dRoom->dwYSize) >= Pos.y)
				{
					SendMsgToClient(pUnit->pPlayerData->pClientData, "Find suitable room!");
					mRoom = dRoom;
					break;
				}
			}

			if (!mRoom) { SendMsgToClient(pUnit->pPlayerData->pClientData, "FindFreeCoords failed!"); break; }

			ptMonster = D2Funcs.D2GAME_SpawnMonster(No, 5, pUnit->pGame, mRoom, Pos.x, Pos.y, -1, 0);
			if (ptMonster) break;

			if (i % 2) Pos.x = Pos.x + (rand() % 4); else  Pos.x = Pos.x - (rand() % 4);
			if (i % 2) Pos.y = Pos.y + (rand() % 4); else  Pos.y = Pos.y - (rand() % 4);
		}
		if (!ptMonster) { SendMsgToClient(pUnit->pPlayerData->pClientData, "Error limit exceeded!"); return false; }

		SendMsgToClient(pUnit->pPlayerData->pClientData, "Monster spawned!");

		return false;
	}
	if(_stricmp(str,"#mspawn")==0)
		{
		WardenClient_i ptCurrentClient = GetClientByID(ClientID);
		if(ptCurrentClient == hWarden.Clients.end()) return TRUE;

		if(!isAnAdmin(ptCurrentClient->AccountName)) { UNLOCK return TRUE; }

		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-733"); UNLOCK return false;}
		int No = atoi(str);
		if(No>733)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-733"); UNLOCK return false;}

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
		UNLOCK
		WardenClient_i psUnit = GetClientByName(str);
			if(psUnit != hWarden.Clients.end()) 
			{
			SendMsgToClient(pUnit->pPlayerData->pClientData,"Spawning monster on %s", psUnit->CharName);
			aRoom = psUnit->ptPlayer->pPath->pRoom1;
			xPos = psUnit->ptPlayer->pPath->xPos;
			yPos = psUnit->ptPlayer->pPath->yPos;
			}
		}
		}

		
		POINT Pos = {xPos, yPos};
		POINT Out = {0,0};

		ptMonster = 0;

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
		UNLOCK
		return false;
		}

		if(_stricmp(str,"#mskill")==0)
		{
		if(!ptMonster) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Spawn monster first (#spawn)");  return false;}

		WardenClient_i ptCurrentClient = GetClientByID(ClientID);
		if(ptCurrentClient == hWarden.Clients.end()) return TRUE;

		if(!isAnAdmin(ptCurrentClient->AccountName)) { UNLOCK return TRUE;}

		if(ptMonster->dwMode==NPC_MODE_DEATH || ptMonster->dwMode==NPC_MODE_DEAD) 
		{
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Monster is dead!");
		UNLOCK
		return false;
		}
		
		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-356"); UNLOCK return false;}
		int SkillNo = atoi(str);
		if(SkillNo>356)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-356"); UNLOCK return false;}
		

		D2Funcs.D2COMMON_AddSkillToUnit(ptMonster, SkillNo, 99, 1 , __FILE__, __LINE__);
		D2Funcs.D2COMMON_RefreshSkills(ptMonster);
		D2Funcs.D2GAME_SetMonSkill(ptMonster,0,SkillNo,-1);
		UNLOCK
		return false;
		}

	return true;
}