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

#include "LWorldEvent.h"
#include "LQuests.h"

#ifdef VER_113D

#endif

bool WE_isKey(UnitAny* ptItem)
{

	DWORD iCode = D2Funcs::D2COMMON_GetItemCode(ptItem);

	for(int i = 0; i<20; i++)
	{
		if(WItem.ItemCode[i] == 0) return false;

		//Log("ICode %s",TransCode(WItem.ItemCode[i]));
			if(iCode == WItem.ItemCode[i])
			{
				for(int q = 0; q<7; q++)
				{
					if(WItem.ItemQuality[q]==-1) break;
			//		Log("IQuality %d",WItem.ItemQuality[q]);
					if(WItem.ItemQuality[q] == 0)
					{
						SellCount++;
						WE_UpdateCounter(SellCount);
						if(SellCount % InfoDelay == 0) WE_Inform(ptItem->pGame);
							return true;
					}
					for(int x = 0; x<20 ; x++)
					{
				//		Log("Idx %d",WItem.FileIdx[x]);
						if(ptItem->pItemData->QualityNo == WItem.ItemQuality[q] && WItem.FileIdx[x] == -1)
						{
							SellCount++;
							WE_UpdateCounter(SellCount);
							if(SellCount % InfoDelay == 0) WE_Inform(ptItem->pGame);
							return true;
						}
					if(WItem.FileIdx[x] == -1) 
						break;
					if(ptItem->pItemData->QualityNo == WItem.ItemQuality[q] && WItem.FileIdx[x] == ptItem->pItemData->FileIndex)
					{
						SellCount++;
						WE_UpdateCounter(SellCount);
						if(SellCount % InfoDelay == 0) WE_Inform(ptItem->pGame);
						return true;
					}
				}
			}

			}
	}
	return false;

}

void WE_GenerateNextDC()
{
	static boost::mt19937 rng;
	rng.seed(static_cast<unsigned> (std::time(0)));
	boost::uniform_int<int> dist(MinSell, MaxSell);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<int>>random(rng, dist);

	int NxtDC = NextDC + random();

	ostringstream str;
	str << NxtDC;
	NextDC = NxtDC;
	WritePrivateProfileString("World Event","NextDC",str.str().c_str(),ConfigFile.c_str());
}

void WE_CreateDCKey(UnitAny* pUnit)
{
		CreateItem sets;
		memset(&sets,0,sizeof(sets));
		int Idx;
		D2Funcs::D2COMMON_GetItemIdx(' kew',&Idx);

		sets.pOwner=pUnit;
		sets.wItemFormat=pUnit->pGame->ItemFormat;
		sets.pGame=pUnit->pGame;
		sets.iMode=ITEM_MODE_ON_CURSOR;
		sets.iQuality=ITEM_QUALITY_NORMAL;
		sets.iIdx=Idx;
		sets.dwItemFlags.bIdentified=1; 
		sets.wCreateFlags=1; 
		sets.iLvl=99;
		UnitAny* ptItem = D2Funcs::D2GAME_CreateItemEx(pUnit->pGame,&sets,0);

			if(ptItem)
			{
				POINT Pos = {pUnit->pPath->xPos,pUnit->pPath->yPos};
				POINT Out = {0,0};
				Room1* aRoom =	D2Funcs::D2GAME_FindFreeCoords(&Pos,pUnit->pPath->pRoom1,&Out,1);
				if(!aRoom) return;
				D2Funcs::D2GAME_DropItem(ptItem,aRoom,pUnit->pGame,pUnit,Out.x,Out.y);
				BroadcastEventMsg(pUnit->pGame,COL_RED,"The World Event Key has been crafted");
			}
}

int __fastcall WE_Spawn(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId)
{
	if(pGame->bSpawnedClone) 
	{
		QUESTS_UpdateUnit(pUnit,20,pUnit);
		return 0;
	}

	Room1* pRoom = D2Funcs::D2COMMON_GetUnitRoom(pUnit);

	if(pRoom)
	{
		int aLvl = D2Funcs::D2COMMON_GetLevelNoByRoom(pRoom);
		if(aLvl==39)
		{

			POINT Pos = {pUnit->pPath->xPos,pUnit->pPath->yPos};
			POINT Out = {0,0};
			Room1* aRoom =	D2Funcs::D2GAME_FindFreeCoords(&Pos,pUnit->pPath->pRoom1,&Out,1);

			if(aRoom)
			{
				ptMonster = D2Funcs::D2GAME_SpawnMonster(333,1,pGame,aRoom,Out.x,Out.y,-1,0);
				if(ptMonster)
				{

					pGame->bSpawnedClone = TRUE;
					WE_Inform2(pGame);
					return 1;
				}
			}
		}
	}

		QUESTS_UpdateUnit(pUnit,20,pUnit);
		return 0;
}

void WE_UpdateCounter(int Value)
{
	ostringstream str;
	str << Value;
	LOCK
	WritePrivateProfileString("World Event", "SellCount", str.str().c_str(),ConfigFile.c_str());
	UNLOCK
}


void WE_Inform(Game* ptGame)
{

		EventPacket aPacket;
		memset(&aPacket,0,40);
		aPacket.P_5A=0x5A;
		aPacket.Color=9;
		aPacket.MsgType=17;
		*(DWORD*)&aPacket.Param1 = SellCount;

		BroadcastPacket(ptGame,(BYTE*)&aPacket, 40);
}

void WE_Inform2(Game* ptGame)
{

		EventPacket aPacket;
		memset(&aPacket,0,40);
		aPacket.P_5A=0x5A;
		aPacket.Color=9;
		aPacket.MsgType=18;

		BroadcastPacket(ptGame,(BYTE*)&aPacket,40);
}
