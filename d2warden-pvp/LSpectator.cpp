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

//
//DWORD WINAPI SpecThread(void* Params)
//{
//Spec * Data = (Spec*)Params;
//
//while(Data->MyUnit->dwMode==PLAYER_MODE_DEATH || Data->MyUnit->dwMode==PLAYER_MODE_DEAD)
//{
//Sleep(50);
//		WardenClient *ptRequestClient = GetClientByID(Data->RequesterID);
//		WardenClient *ptSpecClient = GetClientByID(Data->SpecID);
//		
//		if(!ptSpecClient && !ptRequestClient) { delete Data; return 0;}
//		if(!ptSpecClient && ptRequestClient) { ptRequestClient->Specing=false; break; }
//		if(!ptRequestClient) { delete Data; return 0;}
//
//		Room1* aRoom = D2Funcs::D2COMMON_GetUnitRoom(Data->SpecUnit);
//		Room1* mRoom = D2Funcs::D2COMMON_GetUnitRoom(Data->MyUnit);
//
//		if(!aRoom || !mRoom) { delete Data; return 0;}
//		if(!Data->MyUnit->pPath || !Data->SpecUnit->pPath){ delete Data; return 0;}
//
//		int aX = D2Funcs::D2COMMON_GetPathX(Data->SpecUnit->pPath);
//		int aY = D2Funcs::D2COMMON_GetPathY(Data->SpecUnit->pPath);
//
//		BYTE p[5];
//		::memset(&p,0,5);
//		*(BYTE*)&p[0] = 0x7E;
//		*(WORD*)&p[1]=ptSpecClient->MouseXPosition;
//		*(WORD*)&p[3]=ptSpecClient->MouseYPosition;
//		D2Funcs::D2GAME_SendPacket(ptRequestClient->ptClientData,(BYTE*)&p,5);
//
//		POINT Pos = {aX, aY};
//		POINT Out = {0,0};
//
//		for(int i = 0; i<10; i++)
//		{
//		aRoom =	D2Funcs::D2GAME_FindFreeCoords(&Pos,aRoom,&Out,0);
//		if(aRoom && Out.x && Out.y) break;
//
//		if(i % 2) Pos.x = Pos.x + (rand() % 4); else  Pos.x = Pos.x - (rand() % 4);
//		if(i % 2) Pos.y = Pos.y + (rand() % 4); else  Pos.y = Pos.y - (rand() % 4);
//		}
//		
//		if(!Out.x && !Out.y) continue;
//		
//		if(!aRoom) continue;
//		//if(D2Funcs::D2COMMON_ValidateCoord(aRoom,aX,aY,1)) continue;
//		if(aRoom==mRoom)
//		{
//		int mX = D2Funcs::D2COMMON_GetPathX(Data->MyUnit->pPath);
//		int mY = D2Funcs::D2COMMON_GetPathY(Data->MyUnit->pPath);
//		int	aRange = ((aX-mX)*(aX-mX)) + ((aY-mY)*(aY-mY));
//		if(aRange>100) D2Funcs::D2GAME_TeleportUnit(Out.x, Out.y,0, Data->MyUnit->pGame, Data->MyUnit);
//		}
//		else
//		D2Funcs::D2GAME_TeleportUnit(Out.x, Out.y, aRoom, Data->MyUnit->pGame, Data->MyUnit);
//}
//			WardenClient *ptRequestClient = GetClientByID(Data->RequesterID);
//			if(ptRequestClient) ptRequestClient->Specing=false;
//			int aLevel = D2Funcs::D2COMMON_GetTownLevel(Data->MyUnit->dwAct);
//			int aCurrLevel = D2Funcs::D2COMMON_GetLevelNoByRoom(Data->MyUnit->pPath->pRoom1);
//			if(aCurrLevel!=aLevel)	D2Funcs::D2GAME_MoveUnitToLevelId(Data->MyUnit,aLevel,Data->MyUnit->pGame);
//
//delete Data;
//return 0;
//}
