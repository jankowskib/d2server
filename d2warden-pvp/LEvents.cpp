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

#include "LEvents.h"
#include "LRoster.h"
#include "D2Warden.h"


DWORD __fastcall OnResurrect(Game *pGame, UnitAny *pPlayer, BYTE *aPacket, int PacketSize) // Replacement for 0x41 parse (1.11b: D2Game.6FC51F60)
{
	ClientData *pClientData = 0;
	DEBUGMSG(__FUNCTION__)
	if (PacketSize != 1)
		return 3;
	
	if (!pPlayer || pPlayer->dwMode != PLAYER_MODE_DEAD)
		return 0;

	
		if (pPlayer->dwType == UNIT_PLAYER)
			pClientData = pPlayer->pPlayerData->pClientData;
		if (!pClientData)
			return 3;
		if (pClientData->PlayerStatus & 4)
		{
			D2ASMFuncs::D2GAME_BroadcastLeavingEvent(pClientData, pGame, EVENT_LEFT);
			return 0;
		}
		
		if ((GetTickCount() - pPlayer->pPlayerData->tDeathTime) <= (wcfgRespawnTimer * 1000))
			return 0;

		if (pPlayer->pSkills)
		{
			for (Skill* s = pPlayer->pSkills->pFirstSkill; s; s = s->pNextSkill)
			{
				WORD nSkill = D2Funcs.D2COMMON_GetSkillId(s, __FILE__, __LINE__);
				int nState = D2Funcs.D2COMMON_GetStateNoBySkillId(nSkill);
				if (nState > 0)
				{
					D2Funcs.D2COMMON_SetGfxState(pPlayer, nState, 1);
					D2Funcs.D2COMMON_RefreshAura(pPlayer, nSkill);
				}
			}
		}
		
		int dwMaxLife = D2Funcs.D2COMMON_GetUnitMaxLife(pPlayer);
		int dwMaxMana = D2Funcs.D2COMMON_GetUnitMaxMana(pPlayer);
		int dwMaxStamina = D2Funcs.D2COMMON_GetStatSigned(pPlayer, STAT_MAXSTAMINA, 0);

		D2Funcs.D2COMMON_SetStat(pPlayer, STAT_HP, dwMaxLife, 0);
		D2Funcs.D2COMMON_SetStat(pPlayer, STAT_MANA, dwMaxMana, 0);
		D2Funcs.D2COMMON_SetStat(pPlayer, STAT_STAMINA, dwMaxStamina, 0);

		D2Funcs.D2GAME_UpdatePlayerStats(pPlayer, STAT_HP, dwMaxLife, pPlayer);
		D2Funcs.D2GAME_UpdatePlayerStats(pPlayer, STAT_MANA, dwMaxMana, pPlayer);
		D2Funcs.D2GAME_UpdatePlayerStats(pPlayer, STAT_STAMINA, dwMaxStamina, pPlayer);
		pPlayer->dwFlags |= UNITFLAG_SELECTABLE;				  // Make unit selectable

		D2Funcs.D2COMMON_SetGfxState(pPlayer, uninterruptable, 1);   // Uninterrupable
		D2Funcs.D2COMMON_SetGfxState(pPlayer, uninterruptable, 0);

		int aLevel = D2Funcs.D2COMMON_GetTownLevel(pPlayer->dwAct);

		//Room1* aRoom = D2Funcs.D2COMMON_GetRoomXYByLevel(pRoom->pAct, aLevel, 0, &aX, &aY, 2);
		//D2ASMFuncs::D2GAME_TeleportUnit(aX, aY, aRoom, pGame, pUnit);

		D2ASMFuncs::D2GAME_MoveUnitToLevelId(pPlayer, aLevel, pGame);
		D2Funcs.D2GAME_SetUnitMode(pGame, pPlayer, 0, PLAYER_MODE_STAND_OUTTOWN, 0, 0, 1);

		Skill* pLeftSkill = D2Funcs.D2COMMON_GetLeftSkill(pPlayer);
		Skill* pRightSkill = D2Funcs.D2COMMON_GetRightSkill(pPlayer);
		if (pLeftSkill)
		{
			DWORD nFlags = pLeftSkill->dwFlags;
			WORD nSkill = D2Funcs.D2COMMON_GetSkillId(pLeftSkill, __FILE__, __LINE__);

			D2Funcs.D2GAME_SetMonSkill(pPlayer, 1, nSkill, nFlags);
		}

		if(pRightSkill)
		{
			DWORD nFlags = pRightSkill->dwFlags;
			WORD nSkill = D2Funcs.D2COMMON_GetSkillId(pRightSkill, __FILE__, __LINE__);
			D2Funcs.D2GAME_SetMonSkill(pPlayer, 0, nSkill, nFlags);
		}

	return 0;
}

/*
0x00 - "%Name1(%Name2) dropped due to time out."
0x01 - "%Name1(%Name2) dropped due to errors."
0x02 - "%Name1(%Name2) joined our world. Diablo's minions grow stronger."
0x03 - "%Name1(%Name2) left our world. Diablo's minions weaken."
0x04 - "%Name1 is not in the game."
0x05 - "%Name1 is not logged in."
0x06 - "%Name1 was Slain by %Name2" 
0x07 - Player Relations (Bottom Left Text)
0x08 - "%Name1 is busy."
0x09 - "You must wait a short time to trade with that player."
0x0d - "%Name1 is not listening to you."
0x0e - Received on 'Not enough mana' speech.
0x0f - "Realm going down in %Param1 minutes."
0x10 - "You must wait a short time to declare hostility with that player."
0x11 - "%Param1 Stones of Jordan Sold to Merchants"
0x12 - "Diablo Walks the Earth"
*/

void __stdcall OnBroadcastEvent(Game* pGame, EventPacket * pEvent)
{
	DEBUGMSG("OnBroadcastEvent %d",pEvent->MsgType);
	if(pGame) 
	{
		if(pGame->bFestivalMode == 1)
			if(pEvent->MsgType == 0 || pEvent->MsgType == 1 || pEvent->MsgType == 3) // Leave msgs 
			{
				DEBUGMSG("DEBUG: Szukam struktury ClientData z nazwa %s",pEvent->Name1);

				ClientData* pClient = FindClientDataByName(pGame,pEvent->Name1);
				if(pClient)
				{
					if(pClient->pPlayerUnit->pPlayerData->isPlaying == 0)  return;
					DoRoundEndStuff(pGame, pClient->pPlayerUnit);
				}
				else 
					DEBUGMSG("Nie znalazlem struktury WardenClient w %s",__FUNCTION__);
			}
			if(pEvent->MsgType == EVENT_JOINED) // Join packet
			{
				ClientData* pClient = FindClientDataByName(pGame,pEvent->Name1);
				if(pClient) 
				{
					LRost::SyncClient(pGame,pClient);
					LRoster* pRoster = LRost::Find(pGame,pEvent->Name1);
					if(pRoster && pClient->pPlayerUnit) {
						LRost::SyncClient(pGame,pClient->pPlayerUnit->dwUnitId,pRoster);
					}
				}
			}
	BroadcastPacket(pGame,(BYTE*)pEvent,40);
	}
	switch(pEvent->MsgType)
	{
		case EVENT_TIMEOUT:
		case EVENT_LEFT: 
		case EVENT_DROPPED:
		{
			DEBUGMSG("[REMOVECLIENT] Removing (*%s) %s from WardenQueue", pEvent->Name2, pEvent->Name1);
			WardenClient_i pWardenClient = GetClientByName(pEvent->Name1);
			if(pWardenClient == hWarden.Clients.end()) 
			{
				DEBUGMSG("[REMOVECLIENT] Failed to find WardenClient!");
				return;
			}
			LSpectator *l = pGame->pLSpectator;
			while (l)
			{
				if (l->UnitUID == pWardenClient->ptClientData->UnitId || l->WatcherUID == pWardenClient->ptClientData->UnitId)
				{
					UnitAny *pWatcher = D2ASMFuncs::D2GAME_FindUnit(pGame, l->WatcherUID, UNIT_PLAYER);
					if (pWatcher)
					{
						DEBUGMSG("Cleaning the specer")
						pWatcher->dwFlags |= UNITFLAG_SELECTABLE;
						D2Funcs.D2COMMON_SetGfxState(pWatcher, D2States::invis, 0);
						D2Funcs.D2COMMON_SetGfxState(pWatcher, D2States::uninterruptable, 0);
						int aLevel = D2Funcs.D2COMMON_GetTownLevel(pWatcher->dwAct);
						int aCurrLevel = D2Funcs.D2COMMON_GetLevelNoByRoom(pWatcher->pPath->pRoom1);
						if (aCurrLevel != aLevel) D2ASMFuncs::D2GAME_MoveUnitToLevelId(pWatcher, aLevel, pGame);

						ExEventSpecatorEnd msg;
						msg.P_A6 = 0xA6;
						msg.MsgType = EXEVENT_SPECTATOR_END;
						msg.PacketLen = sizeof(ExEventSpecatorEnd);
						D2ASMFuncs::D2GAME_SendPacket(pWatcher->pPlayerData->pClientData, (BYTE*)&msg, sizeof(ExEventSpecatorEnd));
						pWatcher->pPlayerData->isSpecing = 0;

					}
					LSpectator *d = l;
					if (l->pPrev)
					{
						DEBUGMSG("Removing neighbour spec node...")
							l->pPrev->pNext = l->pNext;
						l = l->pNext;
						delete d;
						continue;
					}
					else
					{
						DEBUGMSG("Removing single spec node...")
							pGame->pLSpectator = l->pNext;
						l = l->pNext;
						if (l)
							l->pPrev = 0;
						delete d;
						continue;
					}
				}
				l = l->pNext;
			}
			RemoveWardenPacket(pWardenClient);
			pWardenClient = hWarden.Clients.erase(pWardenClient);
			UNLOCK
		}
		break;
	}

}


void DoRoundEndStuff(Game* pGame, UnitAny* pUnit) //Sprawdz czy wskaznik jest ok przed wywolaniem, WardenClient = Victim
{
	DEBUGMSG("DEBUG: Doing DoRoundEndStuff");
	if(!pGame || !pUnit) return;

	ostringstream str; 
	string wynik;

	short P1Out = 0,P2Out = 0;
	int P1ID = 0, P2ID = 0;
	int vPID = D2ASMFuncs::D2GAME_GetPartyID(pUnit);

	if(pGame->nPlaying == 0) return;

	for(ClientData* pClient = pGame->pClientList; pClient; pClient = pClient->ptPrevious) {
		if(!pClient->pPlayerUnit) continue;
		if(pClient->InitStatus != 4) continue;
		
		if(pClient->pPlayerUnit->pPlayerData->isPlaying == 1) {
			Room1* pRoom = D2Funcs.D2COMMON_GetUnitRoom(pClient->pPlayerUnit);
			if(!pRoom) continue;
			if(D2Funcs.D2COMMON_GetLevelNoByRoom(pRoom) != D2Funcs.D2COMMON_GetTownLevel(pClient->pPlayerUnit->dwAct))
			{
			int pId = D2ASMFuncs::D2GAME_GetPartyID(pClient->pPlayerUnit);
			if(pId != -1 && P1ID == 0) P1ID = pId;
			else if(pId != -1 && P1ID && pId != P1ID && P2ID == 0) P2ID = pId;

			if(pId != -1 && pClient->pPlayerUnit->dwMode != PLAYER_MODE_DEAD && pClient->pPlayerUnit->dwMode != PLAYER_MODE_DEATH) {
				if(pId == P1ID) P1Out++;
				else if(pId == P2ID) P2Out++;
			}
		}
		}
	}

	DEBUGMSG("Party %d Out = %d, Party %d Out = %d,  Victim Party Id = %d ", P1ID, P1Out,  P2ID, P2Out,  vPID);

	if((P1ID == 0) || (P1Out == 0 || P2Out == 0)) {
		for(ClientData* pClient = pGame->pClientList; pClient; pClient = pClient->ptPrevious)
		{
			if(!pClient->pPlayerUnit) continue;
			if(pClient->InitStatus != 4) continue;

			if(pClient->pPlayerUnit->pPlayerData->isPlaying == 1)
			if(pClient->pPlayerUnit->dwMode != PLAYER_MODE_DEAD && pClient->pPlayerUnit->dwMode != PLAYER_MODE_DEATH)
			{
				int aLevel = D2Funcs.D2COMMON_GetTownLevel(pClient->pPlayerUnit->dwAct);
				int aCurrLevel = D2Funcs.D2COMMON_GetLevelNoByRoom(pClient->pPlayerUnit->pPath->pRoom1);
				if(aCurrLevel!=aLevel)
					D2ASMFuncs::D2GAME_MoveUnitToLevelId(pClient->pPlayerUnit,aLevel,pGame);
			}

		}
		pGame->nPlaying = 0;
		ClearSayGoFlag(pGame);
		ClearCanAttackFlag(pGame);
		ClearIsPlayingFlag(pGame);
		BroadcastExEvent(pGame,1,0,3,-1,200, "Runda zakonczona!", "Round end!");
	}

}