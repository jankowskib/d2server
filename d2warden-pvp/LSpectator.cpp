/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2013 lolet
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


unsigned int __stdcall SpecThread(void* Params)
{
Spec * Data = (Spec*)Params;
UnitAny *MyUnit;
UnitAny *SpecUnit;
if(!Data || !Data->ptGame || !Data->RequesterID || !Data->SpecID) 
{
	return 0;
}

srand(time(0));
do
{
	EnterCriticalSection(Data->ptGame->ptLock);

	MyUnit = D2Funcs::D2GAME_FindUnit(Data->ptGame, Data->RequesterID, UNIT_PLAYER);
	SpecUnit = D2Funcs::D2GAME_FindUnit(Data->ptGame, Data->SpecID, UNIT_PLAYER);
	if(!MyUnit) { LeaveCriticalSection(Data->ptGame->ptLock); return 0;}

	//D2Funcs::D2COMMON_ChangeCurrentMode(MyUnit,PLAYER_MODE_DEAD);
	D2Funcs::D2GAME_KillPlayer(Data->ptGame,MyUnit,PLAYER_MODE_DEAD,0);
	D2Funcs::D2COMMON_SetGfxState(MyUnit, D2States::invis,1);
	MyUnit->pPlayerData->isSpecing = 1;

	if(!SpecUnit) {LeaveCriticalSection(Data->ptGame->ptLock); break; }
	//Sleep(50);

	Room1* aRoom = D2Funcs::D2COMMON_GetUnitRoom(SpecUnit);
	Room1* mRoom = D2Funcs::D2COMMON_GetUnitRoom(MyUnit);

	if(!aRoom || !mRoom) {	LeaveCriticalSection(Data->ptGame->ptLock); break;}
	if(!MyUnit->pPath || !SpecUnit->pPath){ LeaveCriticalSection(Data->ptGame->ptLock); break;}

	int aX = D2Funcs::D2COMMON_GetPathX(SpecUnit->pPath);
	int aY = D2Funcs::D2COMMON_GetPathY(SpecUnit->pPath);


	//BYTE p[5];
	//::memset(&p,0,5);
	//*(BYTE*)&p[0] = 0x7E;
	//*(WORD*)&p[1]=SpecUnit->MouseXPosition;
	//*(WORD*)&p[3]=SpecUnit->MouseYPosition;
	//D2Funcs::D2GAME_SendPacket(ptRequestClient->ptClientData,(BYTE*)&p,5);

	POINT Pos = {aX, aY};
	POINT Out = {0, 0};

	for(int i = 0; i<10; ++i)
	{
		aRoom =	D2Funcs::D2GAME_FindFreeCoords(&Pos,aRoom,&Out,0);
		if(aRoom && Out.x && Out.y) break;

		if(i % 2) Pos.x = Pos.x + (rand() % 4); else  Pos.x = Pos.x - (rand() % 4);
		if(i % 2) Pos.y = Pos.y + (rand() % 4); else  Pos.y = Pos.y - (rand() % 4);
	}

	if(!Out.x && !Out.y) {	LeaveCriticalSection(Data->ptGame->ptLock); continue; }

	if(!aRoom) { LeaveCriticalSection(Data->ptGame->ptLock); continue; }
	//if(D2Funcs::D2COMMON_ValidateCoord(aRoom,aX,aY,1)) continue;
	if(aRoom==mRoom)
	{
		int mX = D2Funcs::D2COMMON_GetPathX(MyUnit->pPath);
		int mY = D2Funcs::D2COMMON_GetPathY(MyUnit->pPath);
		int	aRange = ((aX-mX)*(aX-mX)) + ((aY-mY)*(aY-mY));
		if(aRange>100) D2Funcs::D2GAME_TeleportUnit(Out.x, Out.y,0, MyUnit->pGame, MyUnit);
	}
	else
		D2Funcs::D2GAME_TeleportUnit(Out.x, Out.y, aRoom, MyUnit->pGame, MyUnit);
	LeaveCriticalSection(Data->ptGame->ptLock); 
} while((MyUnit->dwMode == PLAYER_MODE_DEATH || MyUnit->dwMode == PLAYER_MODE_DEAD) && (SpecUnit->dwMode != PLAYER_MODE_DEATH || SpecUnit->dwMode != PLAYER_MODE_DEAD));

EnterCriticalSection(Data->ptGame->ptLock);

D2Funcs::D2COMMON_SetGfxState(MyUnit,D2States::invis,0);
MyUnit->pPlayerData->isSpecing = 0;

int aLevel = D2Funcs::D2COMMON_GetTownLevel(MyUnit->dwAct);
int aCurrLevel = D2Funcs::D2COMMON_GetLevelNoByRoom(MyUnit->pPath->pRoom1);
if(aCurrLevel!=aLevel)	D2Funcs::D2GAME_MoveUnitToLevelId(MyUnit,aLevel,Data->ptGame);
LeaveCriticalSection(Data->ptGame->ptLock); 
return 0;
}
