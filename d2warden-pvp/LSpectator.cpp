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
#include "LSpectator.h"
#include "LRoster.h"
#include "LPackets.h"
#include "D2Warden.h"

/*
	Updates position on every player that observes WatchedPlayer
*/
void SPECTATOR_UpdatePositions(Game* pGame, UnitAny* pWatchedPlayer, int xPos, int yPos)
{
	WORD UnitX = D2Funcs.D2GAME_GetUnitX(pWatchedPlayer);
	WORD UnitY = D2Funcs.D2GAME_GetUnitY(pWatchedPlayer);

	for (LSpectator* l = pGame->pLSpectator; l; l = l->pNext)
	{
		if (l->UnitUID == pWatchedPlayer->dwUnitId)
		{
			UnitAny * pWatcher = D2ASMFuncs::D2GAME_FindUnit(pGame, l->WatcherUID, UNIT_PLAYER);
			if (!pWatcher) continue;

			Room1* aRoom = D2Funcs.D2COMMON_GetUnitRoom(pWatchedPlayer);
			Room1* mRoom = D2Funcs.D2COMMON_GetUnitRoom(pWatcher);
			if (!mRoom || !aRoom) continue;

			//POINT Pos = { (xPos + UnitX) / 2, (yPos + UnitY) / 2 };
			D2POINT Pos = { xPos, yPos };
			D2POINT Out = { 0, 0 };

			aRoom = D2ASMFuncs::D2GAME_FindFreeCoords(&Pos, aRoom, &Out, 0);
			if (aRoom && Out.x && Out.y)
			{
				int mX = D2Funcs.D2COMMON_GetPathX(pWatcher->pPath);
				int mY = D2Funcs.D2COMMON_GetPathY(pWatcher->pPath);
				if (aRoom == mRoom)
				{
					int	aRange = ((Pos.x - mX)*(Pos.x - mX)) + ((Pos.y - mY)*(Pos.y - mY));
					if (aRange > (25 * 25))
					{
						DEBUGMSG("[SPEC] Moving w/o Room to [%d,%d], range = %d", Out.x, Out.y, aRange)
							D2ASMFuncs::D2GAME_TeleportUnit(Out.x, Out.y, 0, pGame, pWatcher);
					}
				}
				else
				{
					int	aRange = ((Pos.x - mX)*(Pos.x - mX)) + ((Pos.y - mY)*(Pos.y - mY));
					if (aRange > (25 * 25))
					{
						DEBUGMSG("[SPEC] Moving with Room to [%d,%d]", Out.x, Out.y)
							D2ASMFuncs::D2GAME_TeleportUnit(Out.x, Out.y, aRoom, pGame, pWatcher);
					}
				}
			}

		}
	}

}

/*
	Removes pWatcher from LSpectator targets or requesters
*/
void SPECTATOR_RemoveFromQueue(Game* pGame, DWORD QuitterId)
{
	LSpectator *l = pGame->pLSpectator;
	while (l)
	{
		if (l->UnitUID == QuitterId || l->WatcherUID == QuitterId)
		{
			UnitAny *pWatcher = D2ASMFuncs::D2GAME_FindUnit(pGame, l->WatcherUID, UNIT_PLAYER);
			if (pWatcher)
			{
				DEBUGMSG("Cleaning the specer")
				pWatcher->dwFlags |= UNITFLAG_SELECTABLE;
				pWatcher->dwFlags &= ~(UNITFLAG_DEATHVANISH | UNITFLAG_DEAD | UNITFLAG_NEUTRAL);
				D2Funcs.D2COMMON_SetGfxState(pWatcher, D2EX_SPECTATOR_STATE, 0);
				D2Funcs.D2COMMON_SetGfxState(pWatcher, D2States::uninterruptable, 0);
				pWatcher->pPath->dwCollisionFlag = 0x1C09;
				pWatcher->pPath->_5[0] = 1;
				pWatcher->pPath->_5[1] = 128;
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
				D2Funcs.FOG_FreeServerMemory(pGame->pMemPool, d, __FILE__, __LINE__, NULL);
				continue;
			}
			else
			{
				DEBUGMSG("Removing single spec node...")
					pGame->pLSpectator = l->pNext;
				l = l->pNext;
				if (l)
					l->pPrev = 0;
				D2Funcs.FOG_FreeServerMemory(pGame->pMemPool, d, __FILE__, __LINE__, NULL);
				continue;
			}
		}
		l = l->pNext;
	}
}

DWORD SPECTATOR_OnClickSpecate(Game *pGame, UnitAny* pWatcher, UnitAny* pUnit)
{
	if (pWatcher->pPlayerData->isSpecing && pWatcher == pUnit)
	{
		SPECTATOR_RemoveFromQueue(pGame, pWatcher->dwUnitId);
		return 0;
	}

	if (pWatcher->pPlayerData->isSpecing)
	{
		SendMsgToClient(pWatcher->pPlayerData->pClientData, "You already are watching someone!");
		return 0;
	}

	if (pUnit->pPlayerData->isSpecing)
	{
		SendMsgToClient(pWatcher->pPlayerData->pClientData, "You cannot watch a spectator!");
		return 0;
	}

	if (pWatcher->dwUnitId == pUnit->dwUnitId)
	{
		SendMsgToClient(pWatcher->pPlayerData->pClientData, "You cannot watch yourself!");
		return 3;
	}
	if (pUnit->dwMode == PLAYER_MODE_DEAD || pUnit->dwMode == PLAYER_MODE_DEATH || pWatcher->dwMode == PLAYER_MODE_DEAD || pWatcher->dwMode == PLAYER_MODE_DEATH)
	{
		SendMsgToClient(pWatcher->pPlayerData->pClientData, "You cannot watch a dead player or being a dead!");
		return 0;
	}
	LSpectator * pSpec = (LSpectator*)D2Funcs.FOG_AllocServerMemory(pGame->pMemPool, sizeof(LSpectator), __FILE__, __LINE__, NULL);
	pSpec->UnitUID = pUnit->dwUnitId;
	pSpec->WatcherUID = pWatcher->dwUnitId;

	LSpectator *l = pGame->pLSpectator;
	for (; l; l = l->pNext)
	{
		if (!l->pNext)
			break;
	}

	if (l)
	{
		pSpec->pPrev = l;
		pSpec->pNext = 0;
		l->pNext = pSpec;
	}
	else
	{
		pSpec->pPrev = 0;
		pSpec->pNext = 0;
		pGame->pLSpectator = pSpec;
	}
	
	pWatcher->pPlayerData->isSpecing = 1;

	COMBAT_Free(pGame, pWatcher);

	D2ASMFuncs::D2GAME_RemovePets(pGame, pWatcher);
	D2ASMFuncs::D2GAME_StopSequence(pWatcher);
	D2ASMFuncs::D2GAME_RemoveBuffs(pGame, pWatcher);
	D2ASMFuncs::D2GAME_DeleteTimers(pGame, pWatcher);
	D2ASMFuncs::D2GAME_ResetTimers(pGame, pWatcher);
	D2ASMFuncs::D2GAME_RemoveInteraction(pGame, pWatcher);

	D2Funcs.D2COMMON_SetGfxState(pWatcher, D2States::uninterruptable, 1);
	D2Funcs.D2COMMON_SetGfxState(pWatcher, D2EX_SPECTATOR_STATE, 1);

	pWatcher->dwFlags &= ~(UNITFLAG_SELECTABLE);
	pWatcher->dwFlags |=  (UNITFLAG_NEUTRAL);
	pWatcher->pPath->dwCollisionFlag = 0;
	pWatcher->pPath->_5[0] = 0;
	pWatcher->pPath->_5[1] = 0;
	//pWatcher->dwFlags2 |= (0x00040000 | 0x00010000);
	//D2Funcs.D2COMMON_ChangeCurrentMode(pWatcher, PLAYER_MODE_DEATH);

	//D2Funcs.D2GAME_KillPlayer(pGame, pWatcher, PLAYER_MODE_DEAD, 0);
	ExEventSpecatorStart msg;
	msg.P_A6 = 0xA6;
	msg.MsgType = EXEVENT_SPECTATOR_START;
	msg.PacketLen = sizeof(ExEventSpecatorStart);
	msg.UnitId = pUnit->dwUnitId;
	D2ASMFuncs::D2GAME_SendPacket(pWatcher->pPlayerData->pClientData, (BYTE*)&msg, sizeof(ExEventSpecatorStart));

	return 0;
}

