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
#include "LQuests.h"

int GetDyeRealCol(int Col)
{
	switch(Col)
	{
	case 1: return 20;
	case 2: return 3;
	case 3: return 6;
	case 4: return 16;
	case 5: return 12;
	case 6: return 19;
	case 7: return 17;
	case 8: return 9;
	case 60: return 0;
	}
return 0;
}

BYTE * __stdcall DYES_DrawItem(UnitAny *ptPlayer, UnitAny* ptItem, BYTE* out, BOOL a4)
{
	int col = D2Funcs.D2COMMON_GetStatSigned(ptItem,183,0);

	if(col)
	{
		ItemsTxt* pTxt = D2Funcs.D2COMMON_GetItemTxt(ptItem->dwClassId);
		*out= GetDyeRealCol(col);
		return D2Funcs.D2CMP_MixPalette(a4 ? pTxt->bInvTrans : pTxt->bTransform,GetDyeRealCol(col));
	}

	return D2Funcs.D2COMMON_GetItemColor(ptPlayer,ptItem,out,a4);
}

void QUESTS_UpdateUnit(UnitAny* pUnit, WORD UpdateType, UnitAny*pUpdateUnit) //UType = 19 - 'Nie mogê!', 2 - 'Lvl Up Sound'
{
	ASSERT(pUnit)
	pUnit->UpdateType=UpdateType;
	pUnit->pUpdateUnit=pUpdateUnit;
	D2Funcs.D2COMMON_UpdateOverlay(pUnit);
	pUnit->dwFlags|= 0x400;
}

void RESPEC_RestoreSkills(UnitAny* pUnit)
{
	ASSERT(pUnit)
	if(pUnit->dwType) return;
	int sCount = D2Funcs.D2COMMON_GetPlayerSkillCount(pUnit->dwClassId); 
	int SkillPoints = 0;

	ClientData* pClient = pUnit->pPlayerData->pClientData;

	for(int i = 0; i<sCount; i++)
	{
		int SkillId = D2Funcs.D2COMMON_GetPlayerSkillIdFromList(pUnit->dwClassId,i);
		Skill* pSkill = D2Funcs.D2COMMON_GetSkillById(pUnit,SkillId,-1);
		if(!pSkill) continue;
		SkillPoints+= D2Funcs.D2COMMON_GetSkillLevel(pUnit,pSkill,0);
		D2ASMFuncs::D2GAME_SetSkills(SkillId,pUnit,0,1);
		D2ASMFuncs::D2GAME_Send0x21_UpdateSkills(pClient, SkillId, pUnit, 0, 1);
	}

	D2Funcs.D2COMMON_AddStat(pUnit,5,SkillPoints,0);
	D2ASMFuncs::D2GAME_UpdateBonuses(pUnit);

	D2Funcs.D2COMMON_SetStartFlags(pUnit,1);
}

void RESPEC_RestoreStats(UnitAny* pUnit)
{
	ASSERT(pUnit)
	if(pUnit->dwType) return;
	signed int ClassId = pUnit->dwClassId;
	if(ClassId>=0)
	{
		if(ClassId< (signed int) (* D2Vars.D2COMMON_sgptDataTables)->dwCharsStatsRecs)
		{
			CharStatsTxt* pTxt = &(* D2Vars.D2COMMON_sgptDataTables)->pCharStatsTxt[ClassId];
			if(!pTxt) return;
			signed int BaseStr = D2Funcs.D2COMMON_GetBaseStatSigned(pUnit,STAT_STRENGTH,0);
			signed int rStr = pTxt->bStr - BaseStr;
			if(pTxt->bStr != BaseStr) //Respec Str
			{
				D2Funcs.D2COMMON_AddStat(pUnit,4,-rStr,0); //Stats Points
				D2Funcs.D2COMMON_AddStat(pUnit,STAT_STRENGTH, rStr, 0);
				D2Funcs.D2COMMON_UpdateOverlay(pUnit);
			}

			signed int BaseDex = D2Funcs.D2COMMON_GetBaseStatSigned(pUnit,STAT_DEXTERITY,0);
			signed int rDex = pTxt->bDex - BaseDex;

			if(pTxt->bDex != BaseDex) //Respec Dex
			{
				D2Funcs.D2COMMON_AddStat(pUnit,4,-rDex,0); //Stats Points
				D2Funcs.D2COMMON_AddStat(pUnit,STAT_DEXTERITY, rDex, 0);
				D2Funcs.D2COMMON_UpdateOverlay(pUnit);
			}

			signed int BaseVit = D2Funcs.D2COMMON_GetBaseStatSigned(pUnit,STAT_VITALITY,0);
			signed int rVit = pTxt->bVita - BaseVit;

			if(rVit)		//Respec Vitality
			{
				D2Funcs.D2COMMON_AddStat(pUnit,4,-rVit,0); //Stats Points
				D2Funcs.D2COMMON_AddStat(pUnit,STAT_VITALITY, rVit, 0);
				D2Funcs.D2COMMON_AddStat(pUnit,STAT_MAXHP, rVit * pTxt->bLifePerVitality << 6, 0);  // Divide by 64
				if(rVit>0) D2Funcs.D2COMMON_AddStat(pUnit,STAT_HP, rVit * pTxt->bLifePerVitality << 6, 0);  // Divide by 64
				int MaxHP = D2Funcs.D2COMMON_GetStatSigned(pUnit,STAT_MAXHP,0);
				int HP = D2Funcs.D2COMMON_GetStatSigned(pUnit,STAT_HP,0);
				if(HP > MaxHP) D2Funcs.D2COMMON_SetStat(pUnit,STAT_HP,MaxHP,0);

				D2Funcs.D2COMMON_AddStat(pUnit,STAT_MAXSTAMINA, rVit * pTxt->bStaminaPerVitality << 6, 0);  // Divide by 64
				if(rVit>0) D2Funcs.D2COMMON_AddStat(pUnit,STAT_STAMINA, rVit * pTxt->bStaminaPerVitality << 6, 0);  // Divide by 64
				int MaxSt = D2Funcs.D2COMMON_GetStatSigned(pUnit,STAT_MAXSTAMINA,0);
				int St = D2Funcs.D2COMMON_GetStatSigned(pUnit,STAT_STAMINA,0);
				if(St > MaxSt) D2Funcs.D2COMMON_SetStat(pUnit,STAT_STAMINA,MaxSt,0);

			}

			signed int BaseEne = D2Funcs.D2COMMON_GetBaseStatSigned(pUnit,STAT_ENERGY,0);
			signed int rEne = pTxt->bEne - BaseEne;

			if(rEne)		//Respec Vitality
			{
				D2Funcs.D2COMMON_AddStat(pUnit,4,-rEne,0); //Stats Points
				D2Funcs.D2COMMON_AddStat(pUnit,STAT_ENERGY, rEne, 0);
				D2Funcs.D2COMMON_AddStat(pUnit,STAT_MAXMANA, rEne * pTxt->bManaPerMagic << 6, 0);  // Divide by 64

				if(rEne>0) D2Funcs.D2COMMON_AddStat(pUnit,STAT_MANA, rEne * pTxt->bManaPerMagic << 6, 0);  // Divide by 64
				int MaxMP = D2Funcs.D2COMMON_GetStatSigned(pUnit,STAT_MAXMANA,0);
				int MP = D2Funcs.D2COMMON_GetStatSigned(pUnit,STAT_MANA,0);
				if(MP > MaxMP) D2Funcs.D2COMMON_SetStat(pUnit,STAT_MANA,MaxMP,0);

			}


		}
	}
}

bool __stdcall QUESTS_OnUseItem(Game* pGame, UnitAny* pUnit, UnitAny* pItem, DWORD dwItemCode)
{
	if(dwItemCode == ' aot') 
	{
	D2Funcs.D2GAME_RemoveFromPickedUp(pUnit);

	RESPEC_RestoreSkills(pUnit);
	RESPEC_RestoreStats(pUnit);

	D2ASMFuncs::D2GAME_RemoveItem(pGame,pUnit,pItem);
	QUESTS_UpdateUnit(pUnit,2,pUnit);
	LogToFile("Respec.log",true,"%s (*%s) zresetowal swoja postac!",pUnit->pPlayerData->szName,pUnit->pPlayerData->pClientData->AccountName);
	return true;
	}

	return false;
}

int __fastcall DYES_Prepare(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId)
{

	D2Funcs.D2COMMON_SetItemFlag(pScroll,4,1);
	ClientData* pClient = pUnit->pPlayerData->pClientData;

	BYTE Packet[8];
	::memset(Packet,0,8);
	Packet[0] = 0x3F;
	Packet[1] = 0x07; //Spell Icon
	*(DWORD*)&Packet[2] = pScroll->dwUnitId;
	*(WORD*)&Packet[6] = SkillId;


	D2ASMFuncs::D2GAME_SendPacket(pClient,Packet,8);

	return 1;
}

int __fastcall DYES_Colorize(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId)
{
	
	DWORD iCode = D2Funcs.D2COMMON_GetItemCode(pScroll);
	StatList* ptList = ptItem->pStatsEx->pMyLastList;
	if(ptList->dwOwnerId!=ptItem->dwUnitId)
	{
	for(; ptList ; ptList=ptList->pPrevLink) if(ptList->dwOwnerId==ptItem->dwUnitId) break;
	}
	switch(iCode)
	{
		case ' 1yd':	//White Dye
		D2Funcs.D2COMMON_AddStatToStatList(ptList,183,1,0);
		break;
		case ' 2yd':	//Black Dye
		D2Funcs.D2COMMON_AddStatToStatList(ptList,183,2,0);
		break;
		case ' 3yd':	//Blue Dye
		D2Funcs.D2COMMON_AddStatToStatList(ptList,183,3,0);
		break;
		case ' 4yd':	//Gold Dye
		D2Funcs.D2COMMON_AddStatToStatList(ptList,183,4,0);
		break;
		case ' 5yd':	//Green Dye
		D2Funcs.D2COMMON_AddStatToStatList(ptList,183,5,0);
		break;
		case ' 6yd':	//Orange Dye
		D2Funcs.D2COMMON_AddStatToStatList(ptList,183,6,0);
		break;
		case ' 7yd':	//Purple Dye
		D2Funcs.D2COMMON_AddStatToStatList(ptList,183,7,0);
		break;
		case ' 8yd':	//Red Dye
		D2Funcs.D2COMMON_AddStatToStatList(ptList,183,8,0);
		break;
		case ' clb':	//Bleach
		D2Funcs.D2COMMON_AddStatToStatList(ptList,183,0,0);
		break;
		case ' 2lb':	//Super Bleach
		D2Funcs.D2COMMON_AddStatToStatList(ptList,183,60,0);
		break;
	}
	strcpy_s(ptItem->pItemData->szPlayerName,16,iCode == ' clb' ? "" : pUnit->pPlayerData->pClientData->AccountName);
	ptItem->pItemData->dwItemFlags.bPersonalized=iCode == ' clb' ? 0 : 1;
	ptItem->pItemData->dwItemFlags.bNamed=iCode == ' clb' ? 0 : 1;
	D2Funcs.D2COMMON_SetItemFlag(ptItem,ITEMFLAG_NEWITEM,1);
	D2Funcs.D2COMMON_AddToContainer(pUnit->pInventory,ptItem);
	D2Funcs.D2COMMON_SetBeginFlag(pUnit,1);
	QUESTS_UpdateUnit(pUnit,6,pUnit);
	return 1;
}

BOOL __stdcall QUESTS_CowLevelOpenPortal(Game *pGame, UnitAny *pUnit)
{
ASSERT(pUnit)
ASSERT(pGame->pQuestControl)
ASSERT(pGame->pQuestControl->bPickedSet)

Room1* pRoom = D2Funcs.D2COMMON_GetUnitRoom(pUnit);
if(pRoom)
{
int aLvl = D2Funcs.D2COMMON_GetLevelNoByRoom(pRoom);
if(aLvl==1)
{

	POINT Pos = {pUnit->pPath->xPos,pUnit->pPath->yPos};
	POINT Out = {0,0};
	Room1* aRoom =	D2ASMFuncs::D2GAME_FindFreeCoords(&Pos,pUnit->pPath->pRoom1,&Out,1);

	if(aRoom) 
	{

	UnitAny* pTown = D2Funcs.D2GAME_CreateUnit(UNIT_OBJECT,60,Out.x,Out.y,pGame,aRoom,1,1,0);
	D2Funcs.D2COMMON_ChangeCurrentMode(pTown,1);
	UnitAny* pCow = D2Funcs.D2GAME_CopyPortal(pGame,pTown,39,Out);
		if(pCow)
		{
			DWORD Flags = D2Funcs.D2COMMON_GetPortalFlags(pCow) | 3;
			D2Funcs.D2COMMON_SetPortalFlags(pCow,Flags);
			D2Funcs.D2COMMON_UpdateRoomWithPortal(aRoom,0);
			Room1* bRoom = D2Funcs.D2COMMON_GetUnitRoom(pCow);
			D2Funcs.D2COMMON_UpdateRoomWithPortal(bRoom,0);
			return 1;
		}
	}
}
}
QUESTS_UpdateUnit(pUnit,20,pUnit);

return 0;
}