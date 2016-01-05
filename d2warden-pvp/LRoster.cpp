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

#include "LRoster.h"
#include "LEvents.h"
#include "LSpectator.h"

void __stdcall OnCreateDamage(UnitAny* pDefender, Damage* pDamage, UnitAny* pMissile)
{
	if(!pDefender) return;
	if(pDefender->dwType != UNIT_PLAYER) return;
	if(!pDefender->pPlayerData) return;
	if(!pMissile) return;
	pDefender->pPlayerData->LastHitSkillId = D2Funcs.D2COMMON_GetMissileSkillId(pMissile);
	
}
 
void COMBAT_Free(Game* pGame, UnitAny* pUnit)
{
	Combat* pCombat = pUnit->ptCombat;
	if (!pCombat)
		return;

	Combat * pDiffCombat = 0;
	Combat * pNext;
	do
	{
	  pNext = pCombat->ptNext;
	  if ( pCombat->dwAttackerType != pUnit->dwType || pCombat->dwAttackerId != pUnit->dwUnitId )
	  {
		pDiffCombat = pCombat;
	  }
	  else
	  {
		if ( pDiffCombat )
		  pDiffCombat->ptNext = pNext;
		else
		  pUnit->ptCombat = pNext;
		D2Funcs.FOG_FreeServerMemory(pGame->pMemPool, pCombat, __FILE__, __LINE__, 0);
	  }
	  pCombat = pNext;
	}
	while ( pNext );
}

Combat* COMBAT_Find(UnitAny* pAttacker, UnitAny* pDefender)
{
	if(!pAttacker || !pDefender) return 0;
	for(Combat* pCombat = pDefender->ptCombat; pCombat; pCombat = pCombat->ptNext)
	{ 
		if(pCombat->dwAttackerId == pAttacker->dwUnitId && pCombat->dwAttackerType == pAttacker->dwType) 
			return pCombat;
	}
return 0; 
}

void __fastcall PLAYERMODES_0_Death(Game *pGame, UnitAny *pVictim, int nMode, UnitAny *pKiller)  //_6FC62F20
{
	ASSERT(pGame)

	if(pVictim) {
		if(!D2Funcs.D2COMMON_isInShapeForm(pVictim) || !(pVictim->dwType == UNIT_PLAYER && pVictim->dwClassId == 5))
		{
			COMBAT_Free(pGame,pVictim);
			D2Funcs.D2COMMON_ChangeCurrentMode(pVictim, PLAYER_MODE_DEATH);

			ClientData* pClient = 0;
			if(pVictim->dwType == UNIT_PLAYER) 
				pClient = pVictim->pPlayerData->pClientData;

			if(pClient) 
				pClient->PlayerStatus |= 8;

			D2ASMFuncs::D2GAME_StopSequence(pVictim);
			D2ASMFuncs::D2GAME_RemoveBuffs(pGame, pVictim);
			D2ASMFuncs::D2GAME_DeleteTimers(pGame, pVictim);
			D2ASMFuncs::D2GAME_ResetTimers(pGame, pVictim);

			if(pKiller)
			{ 
				OnDeath(pKiller, pVictim, pGame);
			}
		
			if(!D2Funcs.D2COMMON_GetUnitState(pVictim,playerbody))  
				pVictim->dwFlags &= ~(UNITFLAG_SELECTABLE|UNITFLAG_OPERATED);
	
			D2ASMFuncs::D2GAME_RemoveInteraction(pGame, pVictim);
			return;
		}

		if(!pVictim->pInventory || !D2Funcs.D2COMMON_GetCursorItem(pVictim->pInventory)) 
		{
			int eMode = PLAYER_MODE_STAND_OUTTOWN;
			Room1* pRoom = D2Funcs.D2COMMON_GetUnitRoom(pVictim);
			if (pRoom && D2Funcs.D2COMMON_isRoomInTown(pRoom))
				eMode = PLAYER_MODE_STAND_INTOWN;

			D2ASMFuncs::D2GAME_DeleteTimers(pGame, pVictim);
			COMBAT_Free(pGame,pVictim);
			D2Funcs.D2COMMON_ChangeCurrentMode(pVictim,eMode);
			D2Funcs.D2COMMON_SetCurrentSkill(pVictim, 0);
		}
	}
}

void __fastcall OnGameDestroy(Game* ptGame)
{
	DEBUGMSG("Closing game %s", ptGame->GameName);
	LRoster::Clear(ptGame);
	LSpectator *l = ptGame->pLSpectator;
	while (l)
	{
		LSpectator *d = l;
		l = l->pNext;
		D2Funcs.FOG_FreeServerMemory(ptGame->pMemPool, d, __FILE__, __LINE__, NULL);
	}
	ptGame->pLSpectator = 0;
}

/*
	Sends a roster of all players to given client [ptClient]
*/
void LRoster::SyncClient(Game *ptGame, ClientData* ptClient) 
{
	if(!ptGame || !ptClient)
		return;
	if(!(ptClient->InitStatus & 4))
		return;

	for(ClientData * pClientList = ptGame->pClientList; pClientList; pClientList = pClientList->ptPrevious)
	{
		if(pClientList->InitStatus & 4) {
			LRosterData * pRoster = LRoster::Find(ptGame,pClientList->CharName);
			if(!pRoster) continue;

			RosterPacket pVKInfo = {0x66,pClientList->pPlayerUnit->dwUnitId,(BYTE)1,(BYTE)pRoster->Kills};
			RosterPacket pVDInfo = {0x66,pClientList->pPlayerUnit->dwUnitId,(BYTE)2,(BYTE)pRoster->Deaths};
			RosterPacket pVAInfo = {0x66,pClientList->pPlayerUnit->dwUnitId,(BYTE)3,(BYTE)pRoster->Assists};
			RosterPacket pVGInfo = { 0x66, pClientList->pPlayerUnit->dwUnitId, (BYTE)4, (BYTE)pRoster->Giveups };

			D2ASMFuncs::D2GAME_SendPacket(ptClient, (BYTE*)&pVKInfo,7);
			D2ASMFuncs::D2GAME_SendPacket(ptClient, (BYTE*)&pVDInfo,7);
			D2ASMFuncs::D2GAME_SendPacket(ptClient, (BYTE*)&pVAInfo, 7);
			D2ASMFuncs::D2GAME_SendPacket(ptClient, (BYTE*)&pVGInfo, 7);
		}
	}
}

/*
	Sends a roster of a player with [UnitId] to all clients
*/
void LRoster::SyncClient(Game *ptGame, DWORD UnitId, LRosterData* pRoster)
{
	if(!ptGame || !pRoster) return;
		RosterPacket pVKInfo = {0x66,UnitId,(BYTE)1,(BYTE)pRoster->Kills};
		RosterPacket pVDInfo = {0x66,UnitId,(BYTE)2,(BYTE)pRoster->Deaths};
		RosterPacket pVAInfo = { 0x66, UnitId, (BYTE)3, (BYTE)pRoster->Assists };
		RosterPacket pVGInfo = { 0x66, UnitId, (BYTE)4, (BYTE)pRoster->Giveups };
		//TODO:Assysty

	for(ClientData * pClientList = ptGame->pClientList; pClientList; pClientList = pClientList->ptPrevious)
	{
	if(pClientList->InitStatus & 4) {
		D2ASMFuncs::D2GAME_SendPacket(pClientList,(BYTE*)&pVKInfo,7);
		D2ASMFuncs::D2GAME_SendPacket(pClientList,(BYTE*)&pVDInfo,7);
		D2ASMFuncs::D2GAME_SendPacket(pClientList, (BYTE*)&pVAInfo, 7);
		D2ASMFuncs::D2GAME_SendPacket(pClientList, (BYTE*)&pVGInfo, 7);
		}
	}
}

/*
	Sends a runaway roster of a player with [UnitId] to all clients
*/
void LRoster::SyncClientRunaways(Game *ptGame, DWORD UnitId, LRosterData* pRoster)
{
	if (!ptGame || !pRoster) return;
	RosterPacket pVGInfo = { 0x66, UnitId, (BYTE)4, (BYTE)pRoster->Giveups };

	for (ClientData * pClientList = ptGame->pClientList; pClientList; pClientList = pClientList->ptPrevious)
	{
		if (pClientList->InitStatus & 4)
			D2ASMFuncs::D2GAME_SendPacket(pClientList, (BYTE*)&pVGInfo, 7);
	}
}

LRosterData * LRoster::Find(Game * ptGame, char* szName)
{
	for(LRosterData * LR = ptGame->pLRosterData; LR; LR = LR->ptNext)
	{
		if(_stricmp(LR->szName,szName)==0) return LR;
	}
	return 0;
}

void LRoster::UpdateRoster(Game* ptGame, char * szName, BYTE Type)
{
	if(!ptGame) return;

	LRosterData *Player = LRoster::Find(ptGame,szName);
	if(!Player)
	{
		Player = (LRosterData*) D2Funcs.FOG_AllocServerMemory(ptGame->pMemPool, sizeof(LRosterData), __FILE__, __LINE__, NULL);
		memset(Player,0,sizeof(LRosterData));
		Player->ptNext = ptGame->pLRosterData;
		ptGame->pLRosterData=Player;
		strcpy_s(Player->szName,16,szName);
	}
	switch(Type)
	{
		case 4: {++Player->Giveups; break; }
		case 3: {++Player->Assists; break;}
		case 2: {++Player->Deaths; break;}
		case 1:	{++Player->Kills;break;}
	}
}

void LRoster::Clear(Game * ptGame)
{
	LRosterData * LR = ptGame->pLRosterData; 
	while(LR)
	{
		LRosterData * Next = LR->ptNext;
		D2Funcs.FOG_FreeServerMemory(ptGame->pMemPool, LR, __FILE__, __LINE__, NULL);
		LR = Next;
	}
	ptGame->pLRosterData = 0;
}