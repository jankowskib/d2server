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
		if(pCombat->dwAttackerId == pAttacker->dwUnitId && pCombat->dwAttackerType == pAttacker->dwType) return pCombat;
	}
return 0; 
}

void __fastcall PLAYERMODES_0_Death(Game *pGame, UnitAny *pVictim, int nMode, UnitAny *pKiller)  //_6FC62F20
{
	bool bDruid = false;

	ASSERT(pGame)
		if(pVictim)
			if(pVictim->dwType == UNIT_PLAYER && pVictim->dwClassId == 5) bDruid = true;
	if(pVictim)
	if(!D2Funcs.D2COMMON_isInShapeForm(pVictim) || !bDruid) {
	COMBAT_Free(pGame,pVictim);
	D2Funcs.D2COMMON_ChangeCurrentMode(pVictim,PLAYER_MODE_DEATH);

	ClientData* pClient = 0;
	if(pVictim)
		if(pVictim->dwType == UNIT_PLAYER) pClient = pVictim->pPlayerData->pClientData;
	if(pClient) pClient->PlayerStatus |= 8;
	D2ASMFuncs::D2GAME_StopSequence(pVictim);
	D2ASMFuncs::D2GAME_RemoveBuffs(pGame, pVictim);
	D2ASMFuncs::D2GAME_DeleteTimers(pGame, pVictim);
	D2ASMFuncs::D2GAME_ResetTimers(pGame, pVictim);

		if(pKiller && pVictim) { 
		OnDeath(pKiller,pVictim,pGame);
		}
		
		if(!D2Funcs.D2COMMON_GetUnitState(pVictim,playerbody))  pVictim->dwFlags &= -(UNITFLAG_SELECTABLE|UNITFLAG_OPERATED);
		D2ASMFuncs::D2GAME_RemoveInteraction(pGame, pVictim);
		return;
	}

	if(pVictim)
	if(!pVictim->pInventory || !D2Funcs.D2COMMON_GetCursorItem(pVictim->pInventory)) {
		int eMode = 1;
		Room1* pRoom = D2Funcs.D2COMMON_GetUnitRoom(pVictim);
		if(pRoom) D2Funcs.D2COMMON_isRoomInTown(pRoom) ? eMode = PLAYER_MODE_STAND_INTOWN : eMode = PLAYER_MODE_STAND_OUTTOWN;
		D2ASMFuncs::D2GAME_DeleteTimers(pGame, pVictim);
		COMBAT_Free(pGame,pVictim);
		D2Funcs.D2COMMON_ChangeCurrentMode(pVictim,eMode);
		D2Funcs.D2COMMON_ResetFlag(pVictim, 0);
	}
}

void __fastcall OnGameDestroy(Game* ptGame)
{
	DEBUGMSG("Zamykam gre %s", ptGame->GameName);
	LRost::Clear(ptGame);
}

void __stdcall OnDeath(UnitAny* ptKiller, UnitAny * ptVictim, Game * ptGame)
{
	if(!ptVictim || !ptKiller) return;
	if(ptVictim->dwType) return;

	EventPacket aPacket = {0};
	aPacket.P_5A=0x5A;
	aPacket.Color=4;
	aPacket.MsgType=6;

	if(ptKiller->dwType==UNIT_MONSTER)
	{
		UnitAny* ptParent = D2ASMFuncs::D2GAME_FindUnit(ptGame,ptKiller->pMonsterData->pAiGeneral->OwnerGUIDEq,ptKiller->pMonsterData->pAiGeneral->eOwnerTypeEq);
		if(ptParent)
			if(ptParent->dwType==UNIT_PLAYER)
				ptKiller = ptParent;
	}

	if(ptKiller->dwType==0) 
		LRost::UpdateRoster(ptGame,ptKiller->pPlayerData->szName,1);
	LRost::UpdateRoster(ptGame,ptVictim->pPlayerData->szName,2);

	*(DWORD*)&aPacket.Param1=ptKiller->dwClassId;

	strcpy_s(aPacket.Name1,16,ptVictim->pPlayerData->szName);
	if(ptKiller->dwType==0) 
		strcpy_s(aPacket.Name2,16,ptKiller->pPlayerData->szName);
	else if(ptKiller->dwType==1)
	{
		aPacket.Param2=1;
		if(ptKiller->pMonsterData->fSuperUniq) *(WORD*)&aPacket.Name2=ptKiller->pMonsterData->wUniqueNo;
	}
	//Invisible hit
	if(ptKiller->dwType==UNIT_PLAYER && ptVictim->dwType==UNIT_PLAYER) 
	{
	int KillCount = 0;
	bool check = false;
			if(!ptKiller->pPlayerData->FirstKillTick) {	ptKiller->pPlayerData->FirstKillTick = GetTickCount();	}	
			if(GetTickCount() - ptKiller->pPlayerData->FirstKillTick < 1500) {ptKiller->pPlayerData->KillCount++; KillCount = ptKiller->pPlayerData->KillCount;}
			if(KillCount>1) {	ptKiller->pPlayerData->FirstKillTick = 0; ptKiller->pPlayerData->KillCount = 0; check = true;}

		if(check)  {
		switch(KillCount)
			{
			case 2: {SendExEvent(ptKiller->pPlayerData->pClientData, COL_YELLOW, D2EX_DOUBLEKILL, 3, -1, 150, "PODWOJNE ZABÓJSTWO!", "DOUBLE KILL!"); 	BroadcastExEvent(ptKiller->pGame,COL_WHITE,ptKiller->dwUnitId,1,"data\\D2Ex\\Blobs");} break;
			case 3: {SendExEvent(ptKiller->pPlayerData->pClientData, COL_YELLOW, D2EX_TRIPLEKILL, 3, -1, 150, "POTROJNE ZABÓJSTWO!", "TRIPPLE KILL!"); BroadcastExEvent(ptKiller->pGame,COL_GOLD,ptKiller->dwUnitId,1,"data\\D2Ex\\Blobs");} break;
			case 5:
			case 4: {SendExEvent(ptKiller->pPlayerData->pClientData, COL_YELLOW, D2EX_DOMINATION, 3, -1, 150, "ZABOJCA DRU¯YNY!", "TEAM KILL!"); BroadcastExEvent(ptKiller->pGame,COL_RED,ptKiller->dwUnitId,1,"data\\D2Ex\\Blobs");} break;
			}
		}

		int InRange = D2ASMFuncs::D2GAME_isUnitInRange(ptGame, ptVictim->dwUnitId, UNIT_PLAYER, ptKiller, 51);

		if(InRange) {  
			SendExEvent(ptKiller->pPlayerData->pClientData, COL_YELLOW, D2EX_IMPRESSIVE, 3, -1, 150, "SNAJPER!", "SNIPER!");
			BroadcastExEvent(ptKiller->pGame,COL_WHITE,ptKiller->dwUnitId,4,"data\\D2Ex\\Blobs");
			//BroadcastMsg(ptKiller->pGame,"%s zdobywa osiagniecie 'Snajper'",ptKiller->pPlayerData->pClientData->AccountName);
		}
	int SkillId = 0;
	Skill* pSkill = D2Funcs.D2COMMON_GetCurrentSkill(ptKiller);
	if(pSkill) SkillId = isMeleeSkill(pSkill->pTxt->wSkillId) ? pSkill->pTxt->wSkillId : 0;
		if(SkillId)
		{
			if(SkillId == D2S_JAB || SkillId == D2S_STRAFE)
			{
				SendExEvent(ptKiller->pPlayerData->pClientData, COL_PURPLE, D2EX_HUMILATION, 3, -1, 150, "LOL!", "LOL!");
				BroadcastExEvent(ptKiller->pGame,COL_WHITE,ptKiller->dwUnitId,3,"data\\D2Ex\\Blobs");
			}
		}
	}

		
		if(ptKiller->dwType==UNIT_PLAYER && ptVictim->dwType==UNIT_PLAYER)
		{
		if(ptGame->bFestivalMode == 1) {
			ptVictim->pPlayerData->CanAttack = 0;
			ptVictim->pPlayerData->SaidGO = 0;
			
			DoRoundEndStuff(ptGame, ptVictim);					
			
			ptVictim->pPlayerData->isPlaying = 0;
			ptVictim->pGame->nPlaying--;
			if(ptVictim->pGame->nPlaying<0) ptVictim->pGame->nPlaying = 0;
		}

		//int SkillId = 0;
		//Skill* pSkill = D2Funcs.D2COMMON_GetCurrentSkill(ptKiller);
		//if(pSkill) SkillId = isMeleeSkill(pSkill->pTxt->wSkillId) ? pSkill->pTxt->wSkillId : 0;

		//if(!SkillId)
	 //   { 
		//SkillId = ptVictim->pPlayerData->LastHitSkillId;
		//ptVictim->pPlayerData->LastHitSkillId = 0;
		//}
		//if(!SkillId)
		//{
		//Skill* pSkill = D2Funcs.D2COMMON_GetCurrentSkill(ptKiller);
		//if(pSkill) SkillId =  pSkill->pTxt->wSkillId;
		//}
		//ostringstream MsgEng; MsgEng << ptVictim->pPlayerData->szName  << " was slain by " << ptKiller->pPlayerData->szName << " (" << ConvertSkill(SkillId).c_str() << ')';
		//ostringstream MsgPol; MsgPol << ptVictim->pPlayerData->szName  << " - zabija cie " << ptKiller->pPlayerData->szName << " (" << ConvertSkill(SkillId).c_str() << ')';
		//BroadcastEventMsgEx(ptGame,COL_DARK_GOLD,MsgEng.str(),MsgPol.str());

		LRoster * VictimRoster = LRost::Find(ptGame,ptVictim->pPlayerData->szName);
		LRoster * KillerRoster = LRost::Find(ptGame,ptKiller->pPlayerData->szName);
		if(VictimRoster) LRost::SyncClient(ptGame,ptVictim->dwUnitId,VictimRoster);
		if(KillerRoster) LRost::SyncClient(ptGame,ptKiller->dwUnitId,KillerRoster);
		ptGame->LastKiller = ptKiller->pPlayerData->pClientData->ClientID;
		ptGame->LastVictim = ptVictim->pPlayerData->pClientData->ClientID;
		}
		BroadcastPacket(ptGame,(BYTE*)&aPacket,40);
}

void LRost::SyncClient(Game *ptGame, ClientData* ptClient) // Wysyla roster wszystkich klientow do ServerHashMapgo gracza
{
	if(!ptGame || !ptClient) return;
	if(ptClient->InitStatus != 4) return;

	for(ClientData * pClientList = ptGame->pClientList; pClientList; pClientList = pClientList->ptPrevious)
	{
	if(pClientList->InitStatus) {
		LRoster * pRoster = LRost::Find(ptGame,pClientList->CharName);
		if(!pRoster) continue;

		RosterPacket pVKInfo = {0x66,pClientList->pPlayerUnit->dwUnitId,(BYTE)1,(BYTE)pRoster->Kills};
		RosterPacket pVDInfo = {0x66,pClientList->pPlayerUnit->dwUnitId,(BYTE)2,(BYTE)pRoster->Deaths};
		RosterPacket pVAInfo = {0x66,pClientList->pPlayerUnit->dwUnitId,(BYTE)3,(BYTE)pRoster->Assists};

		D2ASMFuncs::D2GAME_SendPacket(ptClient,(BYTE*)&pVKInfo,7);
		D2ASMFuncs::D2GAME_SendPacket(ptClient,(BYTE*)&pVDInfo,7);
		D2ASMFuncs::D2GAME_SendPacket(ptClient,(BYTE*)&pVAInfo,7);
		}
	}
}

void LRost::SyncClient(Game *ptGame, DWORD UnitId, LRoster* pRoster) // Wysyla roster ServerHashMapgo gracza do wszystkich klientow
{
	if(!ptGame || !pRoster) return;
		RosterPacket pVKInfo = {0x66,UnitId,(BYTE)1,(BYTE)pRoster->Kills};
		RosterPacket pVDInfo = {0x66,UnitId,(BYTE)2,(BYTE)pRoster->Deaths};
		RosterPacket pVAInfo = {0x66,UnitId,(BYTE)3,(BYTE)pRoster->Assists};
		//TODO:Assysty

	for(ClientData * pClientList = ptGame->pClientList; pClientList; pClientList = pClientList->ptPrevious)
	{
	if(pClientList->InitStatus == 4) {
		D2ASMFuncs::D2GAME_SendPacket(pClientList,(BYTE*)&pVKInfo,7);
		D2ASMFuncs::D2GAME_SendPacket(pClientList,(BYTE*)&pVDInfo,7);
		D2ASMFuncs::D2GAME_SendPacket(pClientList,(BYTE*)&pVAInfo,7);
		}
	}
}


LRoster * LRost::Find(Game * ptGame, char* szName)
{
	for(LRoster * LR = ptGame->ptLRoster; LR; LR = LR->ptNext)
	{
		if(_stricmp(LR->szName,szName)==0) return LR;
	}
	return 0;
}

void LRost::UpdateRoster(Game* ptGame, char * szName, BYTE Type)
{
	if(!ptGame) return;

	LRoster *Player = LRost::Find(ptGame,szName);
		if(!Player)
		{
		Player = new LRoster;
		memset(Player,0,sizeof(LRoster));
		Player->ptNext=ptGame->ptLRoster;
		ptGame->ptLRoster=Player;
		strcpy_s(Player->szName,16,szName);
		}
		switch(Type)
		{
			case 3: {Player->Assists++; break;}
			case 2: {Player->Deaths++; break;}
			case 1:	{Player->Kills++;break;}
		}
}

void LRost::Clear(Game * ptGame)
{
	LRoster * LR = ptGame->ptLRoster; 
	while(LR)
	{
		LRoster * Next = LR ->ptNext;
		delete LR;
		LR = Next;
	}
	ptGame->ptLRoster = 0;
}