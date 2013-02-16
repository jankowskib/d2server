#include "stdafx.h"
#include "LMonsters.h"
#include "D2Ptrs.h"
#include "D2Stubs.h"
#include "Vars.h"
#include "WardenMisc.h"

bool ParseMonCmds(UnitAny* pUnit, char* str, char *t)
{
	int ClientID = pUnit->pPlayerData->pClientData->ClientID;

	if(_stricmp(str,"#mspawn")==0)
		{
		list<WardenClient>::iterator ptCurrentClient = GetClientByID(ClientID);
		if(ptCurrentClient == hWarden.Clients.end()) return TRUE;

		if(!isAnAdmin(ptCurrentClient->AccountName)) { UNLOCK return TRUE; }

		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-733"); UNLOCK return false;}
		int No = atoi(str);
		if(No>733)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-733"); UNLOCK return false;}

		int count = 1;
		list<WardenClient>::iterator psUnit = ptCurrentClient;

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
		list<WardenClient>::iterator psUnit = GetClientByName(str);
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

		aRoom =	D2GAME_FindFreeCoords(&Pos,aRoom,&Out,1);
		if(!aRoom) {SendMsgToClient(pUnit->pPlayerData->pClientData,"FindFreeCoords failed!"); break;}

		ptMonster = D2GAME_SpawnMonster(No,1,psUnit->ptPlayer->pGame,aRoom,Out.x,Out.y,-1,0);
		if(ptMonster) break;

		if(i % 2) Pos.x = Out.x + (rand() % 4); else  Pos.x = Out.x - (rand() % 4);
		if(i % 2) Pos.y = Out.y + (rand() % 4); else  Pos.y = Out.y - (rand() % 4);
		}
		if(!ptMonster) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Error limit exceeded!"); break;}

		SendMsgToClient(pUnit->pPlayerData->pClientData,"#%d Monster spawned, HP = %d",z+1, D2COMMON_GetUnitMaxLife(ptMonster) >> 8);	
		}
		UNLOCK
		return false;
		}

		if(_stricmp(str,"#mskill")==0)
		{
		if(!ptMonster) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Spawn monster first (#spawn)");  return false;}

		list<WardenClient>::iterator ptCurrentClient = GetClientByID(ClientID);
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
		

		D2COMMON_AddSkillToUnit(ptMonster, SkillNo, 99, 1 , __FILE__, __LINE__);
		D2COMMON_RefreshSkills(ptMonster);
		D2GAME_SetMonSkill(ptMonster,0,SkillNo,-1);
		UNLOCK
		return false;
		}

	return true;
}