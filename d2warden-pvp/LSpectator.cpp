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


DWORD __fastcall OnRunToLocation(Game* ptGame, UnitAny* ptPlayer, SkillPacket *ptPacket, DWORD PacketLen)
{
	bool InRange = false;
	if(PacketLen!=5) return 3; // zwroc hack
	if(!ptGame) return 3;
	if(!ptPlayer || ptPlayer->dwType != UNIT_PLAYER) return 3;

	WORD UnitX = D2Funcs.D2GAME_GetUnitX(ptPlayer);
	WORD UnitY = D2Funcs.D2GAME_GetUnitY(ptPlayer);

	int xOffset = UnitX - ptPacket->xPos;
	if(xOffset<0) xOffset = -xOffset;
	int yOffset = UnitY - ptPacket->yPos;
	if(yOffset<0) yOffset = -yOffset;

	if(yOffset < 50 && xOffset < 50) InRange = true;

	PlayerData* pPlayerData = ptPlayer->pPlayerData;
	if(!pPlayerData) {
		Log("Didn't find a PlayerData, function %s, %d",__FUNCTION__,__LINE__);
		return 2;
	}

	if(InRange)
	{
		for (map<DWORD, DWORD>::const_iterator i = Specers.begin(); i != Specers.end(); ++i)
		{
			if((*i).second == ptPlayer->dwUnitId) 
			{
				UnitAny * ptSpecee = D2ASMFuncs::D2GAME_FindUnit(ptGame, (*i).first, UNIT_PLAYER);
				if(!ptSpecee) continue;

				Room1* aRoom = D2Funcs.D2COMMON_GetUnitRoom(ptPlayer);
				Room1* mRoom = D2Funcs.D2COMMON_GetUnitRoom(ptSpecee);
				if(!mRoom || !aRoom) continue;

				POINT Pos = {(ptPacket->xPos + UnitX) / 2, (ptPacket->yPos + UnitY) / 2};
				POINT Out = {0, 0};
				
				aRoom = D2ASMFuncs::D2GAME_FindFreeCoords(&Pos, aRoom, &Out, 0);
				if(aRoom && Out.x && Out.y)
				{
					if(aRoom == mRoom)
					{
						int mX = D2Funcs.D2COMMON_GetPathX(ptSpecee->pPath);
						int mY = D2Funcs.D2COMMON_GetPathY(ptSpecee->pPath);
						int	aRange = ((Pos.x-mX)*(Pos.x-mX)) + ((Pos.y-mY)*(Pos.y-mY));
						if (aRange>50) D2ASMFuncs::D2GAME_TeleportUnit(Out.x, Out.y, 0, ptGame, ptSpecee);
					}
					else
						D2ASMFuncs::D2GAME_TeleportUnit(Out.x, Out.y, aRoom, ptGame, ptSpecee);
				}

			}
		}

		pPlayerData->GameFrame = ptGame->GameFrame;		
		D2Funcs.D2GAME_SetUnitMode(ptGame,ptPlayer,0, PLAYER_MODE_RUN, ptPacket->xPos, ptPacket->yPos, 0);
		return 0;
	}
	else
	{
		if((signed int)(ptGame->GameFrame - pPlayerData->GameFrame) > 25)
		{
			ClientData * pClient = pPlayerData->pClientData;
			if(pClient)
			{
				ReassignPacket hReassign = {0};
				hReassign.Header= 0x15;
				hReassign.UnitId = ptPlayer->dwUnitId;
				hReassign.xPos = UnitX;
				hReassign.yPos = UnitY;
				hReassign.Reassign = 1;

				D2ASMFuncs::D2GAME_SendPacket(pClient,(BYTE*)&hReassign,11);
			}
		}
		return 1;
	}
	return 3;
}

unsigned int __stdcall SpecThread(void* Params)
{
Spec * Data = *(Spec**)Params;

if(!Data || !Data->ptGame || !Data->RequesterID || !Data->SpecID) 
{
	if(Data) delete Data;
	return 0;
}
	EnterCriticalSection(Data->ptGame->ptLock);

	UnitAny * MyUnit = D2ASMFuncs::D2GAME_FindUnit(Data->ptGame, Data->RequesterID, UNIT_PLAYER);
	UnitAny * SpecUnit = D2ASMFuncs::D2GAME_FindUnit(Data->ptGame, Data->SpecID, UNIT_PLAYER);

	if(!MyUnit) 
	{ 
		LeaveCriticalSection(Data->ptGame->ptLock);
		delete Data; 
		return 0;
	}

	//D2Funcs.D2GAME_SetPlayerUnitMode(Data->ptGame, MyUnit, 0, PLAYER_MODE_DEAD, 6, -1, 0);
	D2Funcs.D2COMMON_SetGfxState(MyUnit, D2States::invis, 1);
	D2Funcs.D2GAME_KillPlayer(Data->ptGame, MyUnit, PLAYER_MODE_DEAD, 0);
	MyUnit->pPlayerData->isSpecing = 1;
	Specers[Data->RequesterID] = Data->SpecID;

	LeaveCriticalSection(Data->ptGame->ptLock); 
do
{
	EnterCriticalSection(Data->ptGame->ptLock);

	MyUnit = D2ASMFuncs::D2GAME_FindUnit(Data->ptGame, Data->RequesterID, UNIT_PLAYER);
	SpecUnit = D2ASMFuncs::D2GAME_FindUnit(Data->ptGame, Data->SpecID, UNIT_PLAYER);
	if(!MyUnit) { LeaveCriticalSection(Data->ptGame->ptLock); delete Data; return 0;}
	if(!SpecUnit) { LeaveCriticalSection(Data->ptGame->ptLock); break; }
	Sleep(50);
	LeaveCriticalSection(Data->ptGame->ptLock); 
} while((MyUnit->dwMode == PLAYER_MODE_DEATH || MyUnit->dwMode == PLAYER_MODE_DEAD) && (SpecUnit->dwMode != PLAYER_MODE_DEATH || SpecUnit->dwMode != PLAYER_MODE_DEAD));

EnterCriticalSection(Data->ptGame->ptLock);
SendMsgToClient(MyUnit->pPlayerData->pClientData,"Quiting spectator mode..."); 
D2Funcs.D2COMMON_SetGfxState(MyUnit, D2States::invis, 0);
MyUnit->pPlayerData->isSpecing = 0;
Specers.erase(Data->RequesterID);

int aLevel = D2Funcs.D2COMMON_GetTownLevel(MyUnit->dwAct);
int aCurrLevel = D2Funcs.D2COMMON_GetLevelNoByRoom(MyUnit->pPath->pRoom1);
if (aCurrLevel != aLevel) D2ASMFuncs::D2GAME_MoveUnitToLevelId(MyUnit, aLevel, Data->ptGame);
LeaveCriticalSection(Data->ptGame->ptLock); 
return 0;
}
