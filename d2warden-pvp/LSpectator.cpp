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

#include "D2Warden.h"


//Valid for 0x06*, 0x07, 0x09*, 0x0A,  || 0x0D*, 0x0E, 0x10*, 0x11
DWORD __fastcall OnClickUnit(Game* pGame, UnitAny* pPlayer, SkillTargetPacket *ptPacket, DWORD PacketLen)
{
	int InRange = 0;
	if (PacketLen != 9) return 3;
	if (!pGame) return 3;
	if (pPlayer->dwType != UNIT_PLAYER) return 3;
	if (ptPacket->UnitType > 5) return 3;

	InRange = D2ASMFuncs::D2GAME_isUnitInRange(pGame, ptPacket->UnitId, ptPacket->UnitType, pPlayer, 50);
	if (InRange == 2)
	{
		DEBUGMSG("OnClickUnit: Over the range (2)")
			return 2;
	}
	if (InRange == 3)
	{
		DEBUGMSG("OnClickUnit: Over the range (3)");
		return 3;
	}
	Skill * ptSkill = (ptPacket->Header == 6 || ptPacket->Header == 7 || ptPacket->Header == 9 || ptPacket->Header == 0xA) ? D2Funcs.D2COMMON_GetLeftSkill(pPlayer) : D2Funcs.D2COMMON_GetRightSkill(pPlayer);
	if (!ptSkill)
	{
		DEBUGMSG("%s: ptSkill not found!. Packet id is = %d", __FUNCTION__, ptPacket->Header);
		return 3;
	}
	int SkillId = D2Funcs.D2COMMON_GetSkillId(ptSkill, __FILE__, __LINE__);

	PlayerData* pPlayerData = pPlayer->pPlayerData;
	if (!pPlayerData) return 2;

	if (TeleChars[pPlayer->dwClassId] == FALSE && pPlayer->pGame->dwGameState == 0 && SkillId == 0x36)
	{
		SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Teleport nie jest dozwolony dla tej klasy!" : "Teleport Is Not Allowed For This Character");
		return 0;
	}

	if (SkillId == 0x65 && !wcfgAllowHB)
	{
		SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Swiety pocisk jest zabroniony na tym serwerze" : "Holy Bolt Is Not Allowed On This Server");
		return 0;
	}
	if (SkillId == 151 && !wcfgAllowNLWW)
	{
		SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "NLWW jest zabronione na tym serwerze" : "NLWW Is Not Allowed On This Server");
		return 0;
	}

	static int AttackCount;

	if (pGame->bFestivalMode == 1 && pPlayerData->CanAttack == 0 && (!isSafeSkill(SkillId) && SkillId != D2S_CHARGE))
	{
		if (AttackCount == 0)
		{
			SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Wpisz #go aby rozpoczac runde!" : "Type #go to start round");
		}
		AttackCount++;
		if (AttackCount > 4) AttackCount = 0;
		return 0;
	}


	int nPierceIdx = D2Funcs.D2COMMON_GetBaseStatSigned(pPlayer, 328, 0);
	D2Funcs.D2COMMON_SetStat(pPlayer, 328, nPierceIdx + 1, 0);

	D2ASMFuncs::D2GAME_CastSkillOnUnit(pPlayer, ptSkill, pGame, ptPacket->UnitType, ptPacket->UnitId, (ptPacket->Header == 6 || ptPacket->Header == 9 || ptPacket->Header == 0xD || ptPacket->Header == 0x10) ? 1 : 0);

	WORD UnitX = D2Funcs.D2GAME_GetUnitX(pPlayer);
	WORD UnitY = D2Funcs.D2GAME_GetUnitY(pPlayer);

	for (LSpectator* l = pGame->pLSpectator; l; l = l->pNext)
	{
		if (l->UnitUID == pPlayer->dwUnitId)
		{
			UnitAny * pWatcher = D2ASMFuncs::D2GAME_FindUnit(pGame, l->WatcherUID, UNIT_PLAYER);
			if (!pWatcher) continue;

			Room1* aRoom = D2Funcs.D2COMMON_GetUnitRoom(pPlayer);
			Room1* mRoom = D2Funcs.D2COMMON_GetUnitRoom(pWatcher);
			if (!mRoom || !aRoom) continue;

			POINT Pos = { UnitX ,  UnitY};
			POINT Out = { 0, 0 };

			aRoom = D2ASMFuncs::D2GAME_FindFreeCoords(&Pos, aRoom, &Out, 0);
			if (aRoom && Out.x && Out.y)
			{
				int mX = D2Funcs.D2COMMON_GetPathX(pWatcher->pPath);
				int mY = D2Funcs.D2COMMON_GetPathY(pWatcher->pPath);
				if (aRoom == mRoom)
				{
					int	aRange = ((Pos.x - mX)*(Pos.x - mX)) + ((Pos.y - mY)*(Pos.y - mY));
					if (aRange > (35 * 35))
					{
						DEBUGMSG("[SPEC] Moving w/o Room to [%d,%d], range = %d", Out.x, Out.y, aRange)
							D2ASMFuncs::D2GAME_TeleportUnit(Out.x, Out.y, 0, pGame, pWatcher);
					}
				}
				else
				{
					int	aRange = ((Pos.x - mX)*(Pos.x - mX)) + ((Pos.y - mY)*(Pos.y - mY));
					if (aRange > (35 * 35))
					{
						DEBUGMSG("[SPEC] Moving with Room to [%d,%d]", Out.x, Out.y)
							D2ASMFuncs::D2GAME_TeleportUnit(Out.x, Out.y, aRoom, pGame, pWatcher);
					}
				}
			}

		}
	}
	return 0;
}


DWORD __fastcall OnClickLocation(Game* pGame, UnitAny* pPlayer, SkillPacket *ptPacket, DWORD PacketLen)
{
	bool InRange = false;
	static int AttackCount;
	if (PacketLen != 5) return 3; // zwroc hack
	if (!pGame) return 3;
	if (pPlayer->dwType != UNIT_PLAYER) return 3;

	WORD UnitX = D2Funcs.D2GAME_GetUnitX(pPlayer);
	WORD UnitY = D2Funcs.D2GAME_GetUnitY(pPlayer);

	int xOffset = UnitX - ptPacket->xPos;
	if (xOffset < 0) xOffset = -xOffset;
	int yOffset = UnitY - ptPacket->yPos;
	if (yOffset < 0) yOffset = -yOffset;

	if (yOffset < 50 && xOffset < 50) InRange = true;

	PlayerData* pPlayerData = pPlayer->pPlayerData;
	if (!pPlayerData)
	{
		DEBUGMSG("Didn't find a PlayerData, function %s, %d", __FUNCTION__, __LINE__);
		return 2;
	}

	if (InRange)
	{
		pPlayerData->GameFrame = pGame->GameFrame;
		Skill * ptSkill = (ptPacket->Header == 5 || ptPacket->Header == 8) ? D2Funcs.D2COMMON_GetLeftSkill(pPlayer) : D2Funcs.D2COMMON_GetRightSkill(pPlayer);
		if (!ptSkill) return 3;
		int SkillId = D2Funcs.D2COMMON_GetSkillId(ptSkill, __FILE__, __LINE__);

		if (TeleChars[pPlayer->dwClassId] == FALSE && pPlayer->pGame->dwGameState == 0 && SkillId == 0x36)
		{
			SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Teleport nie jest dozwolony dla tej klasy!" : "Teleport Is Not Allowed For This Character");
			return 0;
		}

		if (SkillId == 0x65 && !wcfgAllowHB) {
			SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Swiety pocisk jest zabroniony na tym serwerze" : "Holy Bolt Is Not Allowed On This Server");
			return 0;
		}

		int nPierceIdx = D2Funcs.D2COMMON_GetBaseStatSigned(pPlayer, 328, 0);
		D2Funcs.D2COMMON_SetStat(pPlayer, 328, nPierceIdx + 1, 0);

		if (pGame->bFestivalMode && !pPlayerData->CanAttack && !isSafeSkill(SkillId))
		{
			if (AttackCount == 0)
			{
				SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Wpisz #go aby rozpoczac runde!" : "Type #go to start round");
			}
			AttackCount++;
			if (AttackCount > 4)
				AttackCount = 0;
			return 0;
		}

		D2ASMFuncs::D2GAME_CastSkill(pPlayer, ptSkill, pGame, ptPacket->xPos, ptPacket->yPos);

		for (LSpectator* l = pGame->pLSpectator; l; l = l->pNext)
		{
			if (l->UnitUID == pPlayer->dwUnitId)
			{
				UnitAny * pWatcher = D2ASMFuncs::D2GAME_FindUnit(pGame, l->WatcherUID, UNIT_PLAYER);
				if (!pWatcher) continue;

				Room1* aRoom = D2Funcs.D2COMMON_GetUnitRoom(pPlayer);
				Room1* mRoom = D2Funcs.D2COMMON_GetUnitRoom(pWatcher);
				if (!mRoom || !aRoom) continue;

				POINT Pos = { (ptPacket->xPos + UnitX) / 2, (ptPacket->yPos + UnitY) / 2 };
				POINT Out = { 0, 0 };

				aRoom = D2ASMFuncs::D2GAME_FindFreeCoords(&Pos, aRoom, &Out, 0);
				if (aRoom && Out.x && Out.y)
				{
					int mX = D2Funcs.D2COMMON_GetPathX(pWatcher->pPath);
					int mY = D2Funcs.D2COMMON_GetPathY(pWatcher->pPath);
					if (aRoom == mRoom)
					{
						int	aRange = ((Pos.x - mX)*(Pos.x - mX)) + ((Pos.y - mY)*(Pos.y - mY));
						if (aRange > (35 * 35))
						{
							DEBUGMSG("[SPEC] Moving w/o Room to [%d,%d], range = %d", Out.x, Out.y, aRange)
								D2ASMFuncs::D2GAME_TeleportUnit(Out.x, Out.y, 0, pGame, pWatcher);
						}
					}
					else
					{
						int	aRange = ((Pos.x - mX)*(Pos.x - mX)) + ((Pos.y - mY)*(Pos.y - mY));
						if (aRange > (35 * 35))
						{
							DEBUGMSG("[SPEC] Moving with Room to [%d,%d]", Out.x, Out.y)
								D2ASMFuncs::D2GAME_TeleportUnit(Out.x, Out.y, aRoom, pGame, pWatcher);
						}
					}
				}

			}
		}

		if (!wcfgDetectTrick)
			return 0;

		WardenClient_i ptWardenClient = GetClientByID(pPlayerData->pClientData->ClientID);
		if (ptWardenClient == hWarden.Clients.end()) return 0;

		if (GetTickCount() > ptWardenClient->UIModesTime + 500) { UNLOCK return 0; }

		if ((ptWardenClient->UIModes[UI_CHARACTER] || ptWardenClient->UIModes[UI_QUEST]) && (ptWardenClient->MouseXPosition >= 0 && ptWardenClient->MouseXPosition <= 200) && (ptWardenClient->MouseYPosition >= 0 && ptWardenClient->MouseYPosition <= 550))
		{
			if (ptWardenClient->DebugTrick)
				SendMsgToClient(ptWardenClient->ptClientData, "Trick (Left window) X=%d Y=%d, LAG= %d ms", ptWardenClient->MouseXPosition, ptWardenClient->MouseYPosition, GetTickCount() - ptWardenClient->UIModesTime);
			Log("HACK: %s (*%s) used Polish GA Trick [%s]!, skill : %s XY=[%d,%d]", ptWardenClient->CharName.c_str(), ptWardenClient->AccountName.c_str(), ptWardenClient->UIModes[UI_CHARACTER] ? "Character Stats" : "Quests", ConvertSkill(SkillId).c_str(), ptWardenClient->MouseXPosition, ptWardenClient->MouseYPosition);
		}
		else
			if ((ptWardenClient->UIModes[UI_INVENTORY] || ptWardenClient->UIModes[UI_SKILL]) && (ptWardenClient->MouseXPosition >= 600 && ptWardenClient->MouseXPosition <= 800) && (ptWardenClient->MouseYPosition >= 0 && ptWardenClient->MouseYPosition <= 550))
			{
				if (ptWardenClient->DebugTrick)
					SendMsgToClient(ptWardenClient->ptClientData, "Trick (Right window) X=%d Y=%d, LAG = %d ms", ptWardenClient->MouseXPosition, ptWardenClient->MouseYPosition, GetTickCount() - ptWardenClient->UIModesTime);
				Log("HACK: %s (*%s) used Polish GA Trick [%s]!, skill : %s XY=[%d,%d]", ptWardenClient->CharName.c_str(), ptWardenClient->AccountName.c_str(), ptWardenClient->UIModes[UI_INVENTORY] ? "Inventory" : "Skill Tree", ConvertSkill(SkillId).c_str(), ptWardenClient->MouseXPosition, ptWardenClient->MouseYPosition);
			}
		UNLOCK
			return 0;
	}
	else
	{
		if ((signed int)(pGame->GameFrame - pPlayerData->GameFrame) > 25)
		{
			ClientData * pClient = pPlayerData->pClientData;
			if (pClient)
			{
				ReassignPacket hReassign = { 0 };
				hReassign.Header = 0x15;
				hReassign.UnitId = pPlayer->dwUnitId;
				hReassign.xPos = UnitX;
				hReassign.yPos = UnitY;
				hReassign.Reassign = 1;

				D2ASMFuncs::D2GAME_SendPacket(pClient, (BYTE*)&hReassign, 11);
			}
		}
		return 1;
	}
	return 3;
}

DWORD __fastcall OnRunToLocation(Game* pGame, UnitAny* pPlayer, SkillPacket *ptPacket, DWORD PacketLen)
{
	bool InRange = false;
	if (PacketLen != 5) return 3; // zwroc hack
	if (!pGame) return 3;
	if (!pPlayer || pPlayer->dwType != UNIT_PLAYER) return 3;

	WORD UnitX = D2Funcs.D2GAME_GetUnitX(pPlayer);
	WORD UnitY = D2Funcs.D2GAME_GetUnitY(pPlayer);

	int xOffset = UnitX - ptPacket->xPos;
	if (xOffset < 0) xOffset = -xOffset;
	int yOffset = UnitY - ptPacket->yPos;
	if (yOffset < 0) yOffset = -yOffset;

	if (yOffset < 50 && xOffset < 50) InRange = true;

	PlayerData* pPlayerData = pPlayer->pPlayerData;
	if (!pPlayerData) {
		Log("Didn't find a PlayerData, function %s, %d", __FUNCTION__, __LINE__);
		return 2;
	}

	if (InRange)
	{
		for (LSpectator* l = pGame->pLSpectator; l; l = l->pNext)
		{
			if (l->UnitUID == pPlayer->dwUnitId)
			{
				UnitAny * pWatcher = D2ASMFuncs::D2GAME_FindUnit(pGame, l->WatcherUID, UNIT_PLAYER);
				if (!pWatcher) continue;

				Room1* aRoom = D2Funcs.D2COMMON_GetUnitRoom(pPlayer);
				Room1* mRoom = D2Funcs.D2COMMON_GetUnitRoom(pWatcher);
				if (!mRoom || !aRoom) continue;

				POINT Pos = { (ptPacket->xPos + UnitX) / 2, (ptPacket->yPos + UnitY) / 2 };
				POINT Out = { 0, 0 };

				aRoom = D2ASMFuncs::D2GAME_FindFreeCoords(&Pos, aRoom, &Out, 0);
				if (aRoom && Out.x && Out.y)
				{
					int mX = D2Funcs.D2COMMON_GetPathX(pWatcher->pPath);
					int mY = D2Funcs.D2COMMON_GetPathY(pWatcher->pPath);
					if (aRoom == mRoom)
					{
						int	aRange = ((Pos.x - mX)*(Pos.x - mX)) + ((Pos.y - mY)*(Pos.y - mY));
						if (aRange > (35 * 35))
						{
							DEBUGMSG("[SPEC] Moving w/o Room to [%d,%d], range = %d", Out.x, Out.y, aRange)
							D2ASMFuncs::D2GAME_TeleportUnit(Out.x, Out.y, 0, pGame, pWatcher);
						}
					}
					else
					{
						int	aRange = ((Pos.x - mX)*(Pos.x - mX)) + ((Pos.y - mY)*(Pos.y - mY));
						if (aRange > (35 * 35))
						{
							DEBUGMSG("[SPEC] Moving with Room to [%d,%d]", Out.x, Out.y)
							D2ASMFuncs::D2GAME_TeleportUnit(Out.x, Out.y, aRoom, pGame, pWatcher);
						}
					}
				}

			}
		}

		pPlayerData->GameFrame = pGame->GameFrame;
		D2Funcs.D2GAME_SetUnitMode(pGame, pPlayer, 0, PLAYER_MODE_RUN, ptPacket->xPos, ptPacket->yPos, 0);
		return 0;
	}
	else
	{
		if ((signed int)(pGame->GameFrame - pPlayerData->GameFrame) > 25)
		{
			ClientData * pClient = pPlayerData->pClientData;
			if (pClient)
			{
				ReassignPacket hReassign = { 0 };
				hReassign.Header = 0x15;
				hReassign.UnitId = pPlayer->dwUnitId;
				hReassign.xPos = UnitX;
				hReassign.yPos = UnitY;
				hReassign.Reassign = 1;

				D2ASMFuncs::D2GAME_SendPacket(pClient, (BYTE*)&hReassign, 11);
			}
		}
		return 1;
	}
	return 3;
}

DWORD OnClickSpecate(Game *pGame, UnitAny* pWatcher, UnitAny* pUnit)
{
	if (pWatcher->pPlayerData->isSpecing && pWatcher == pUnit)
	{
		LSpectator *l = pGame->pLSpectator;
		while (l)
		{
			if (l->UnitUID == pWatcher->pPlayerData->pClientData->UnitId || l->WatcherUID == pWatcher->pPlayerData->pClientData->UnitId)
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
		return 0;
	}
	else if (pWatcher->pPlayerData->isSpecing)
	{
		SendMsgToClient(pWatcher->pPlayerData->pClientData, "You already are watching someone!");
		return 0;
	}

	if (pWatcher->dwUnitId == pUnit->dwUnitId)
	{
		SendMsgToClient(pWatcher->pPlayerData->pClientData, "You cannot watch yourself!");
		return 3;
	}
	if (pUnit->dwMode == PLAYER_MODE_DEAD || pUnit->dwMode == PLAYER_MODE_DEATH)
	{
		SendMsgToClient(pWatcher->pPlayerData->pClientData, "You cannot watch a dead player!");
		return 0;
	}
	LSpectator * pSpec = new LSpectator;
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
	D2Funcs.D2COMMON_SetGfxState(pWatcher, D2States::invis, 1);
	D2Funcs.D2COMMON_SetGfxState(pWatcher, D2States::uninterruptable, 1);
	pWatcher->dwFlags &= -(UNITFLAG_SELECTABLE | UNITFLAG_OPERATED);
	//D2Funcs.D2GAME_KillPlayer(pGame, pWatcher, PLAYER_MODE_DEAD, 0);
	ExEventSpecatorStart msg;
	msg.P_A6 = 0xA6;
	msg.MsgType = EXEVENT_SPECTATOR_START;
	msg.PacketLen = sizeof(ExEventSpecatorStart);
	msg.UnitId = pUnit->dwUnitId;
	D2ASMFuncs::D2GAME_SendPacket(pWatcher->pPlayerData->pClientData, (BYTE*)&msg, sizeof(ExEventSpecatorStart));

	return 0;
}
