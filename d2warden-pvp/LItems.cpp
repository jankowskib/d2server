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
#include "LItems.h"

UnitAny* __stdcall ITEMS_AddKillerId(Game *pGame, CreateItem *srCreateItem, int a5)
{
UnitAny* ptItem = D2Funcs.D2GAME_CreateItemEx(pGame,srCreateItem,0);
if(ptItem)
{
		StatList* ptList = ptItem->pStatsEx->pMyLastList;
		if(ptList)
		{
		if(ptList->dwOwnerId!=ptItem->dwUnitId)
		{
		for(;ptList ; ptList=ptList->pPrevLink) if(ptList->dwOwnerId==ptItem->dwUnitId) break;
		}

		D2Funcs.D2COMMON_AddStatToStatList(ptList,184,srCreateItem->pOwner->dwClassId,0); 
		}
}

return ptItem;

}

bool ParseItemsCmds(UnitAny* pUnit, char* str, char *t)
{
int ClientID = pUnit->pPlayerData->pClientData->ClientID;

	if(_stricmp(str,"#icreate")==0)
		{
		if(!isAnAdmin(pUnit->pPlayerData->pClientData->AccountName))  return TRUE; 
		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"#icreate <icode> <iquality [4]> <count [1]>"); return false;}
		char StrCode[5] = {0};
		bool isGold = false;
		strncpy_s(StrCode,5,str,4);
		int ICode = TransCode(StrCode);
		if(ICode == ' dlg') isGold = true;
		int Idx = 0;
		int iQual = 2;
		int iCount = 1;
		if(!D2Funcs.D2COMMON_GetItemIdx(ICode,&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); return false;}
		str = strtok_s(NULL," ",&t);
		if(str) iQual = atoi(str); //1 = cracked 2 = normal 3 = superior 4 = magic 5 = set 6 = rare 7 = unique 8 = crafted
		str = strtok_s(NULL," ",&t);
		if(str) iCount = atoi(str);
		UnitAny* ptItem = D2ASMFuncs::D2GAME_CreateItem(99, 1, 1, pUnit, Idx, pUnit->pGame, 3, iQual, 1, 0, 0);
		CreateItem sets;
		memset(&sets,0,sizeof(sets));

		sets.pOwner=pUnit;
		sets.wItemFormat=pUnit->pGame->ItemFormat;
		sets.pGame=pUnit->pGame;
		sets.iMode=ITEM_MODE_ON_CURSOR;
		sets.iQuality=iQual;
		sets.iIdx=Idx;
		sets.dwItemFlags.bIdentified=1; 
		sets.dwItemFlags.bRepaired=1;
		sets.wCreateFlags=1; 
		sets.iLvl=99;
		for(int i = 0; i<iCount; i++) {

		UnitAny* ptItem = D2Funcs.D2GAME_CreateItemEx(pUnit->pGame,&sets,0);

			if(ptItem)
			{
			if(isGold) D2Funcs.D2COMMON_SetStat(ptItem,STAT_GOLD,1000000,0);
			POINT Pos = {pUnit->pPath->xPos,pUnit->pPath->yPos};
			POINT Out = {0,0};
			Room1* aRoom =	D2ASMFuncs::D2GAME_FindFreeCoords(&Pos,pUnit->pPath->pRoom1,&Out,1);
			if(!aRoom) {SendMsgToClient(pUnit->pPlayerData->pClientData,"FindFreeCoords failed!"); return false;}
			D2ASMFuncs::D2GAME_DropItem(ptItem,aRoom,pUnit->pGame,pUnit,Out.x,Out.y);
			}
		}
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Item(s) created, dropping to ground...");
		return false;
		}
		if(_stricmp(str,"#idupe")==0)
		{
		UnitAny* ptItem = D2Funcs.D2COMMON_GetCursorItem(pUnit->pInventory);
		if(!ptItem) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Put Item On The Cursor!");  return false;}

		if(!isAnAdmin(pUnit->pPlayerData->pClientData->AccountName))  return TRUE; 

		//D2ASMFuncs::D2GAME_DropItem(ptItem,pUnit->pPath->pRoom1,pUnit->pGame,pUnit,pUnit->pPath->xPos,pUnit->pPath->yPos);
		//D2Funcs.D2COMMON_SetCursorItem(pUnit->pInventory,0);
		UnitAny* pNew = D2ASMFuncs::D2GAME_DupeItem(ptItem->pGame, ptItem, 1);
		if(pNew)
		{
			SendMsgToClient(pUnit->pPlayerData->pClientData,"Item created, dropping to ground...");
		/*	pNew->pItemData->dwItemFlags.bInit=1;
			pNew->pItemData->CommandFlags&=~1;*/
			D2ASMFuncs::D2GAME_DropItem(pNew,pUnit->pPath->pRoom1,pUnit->pGame,pUnit,pUnit->pPath->xPos,pUnit->pPath->yPos);
		}

		return false;
		}
		if(_stricmp(str,"#iperson")==0)
		{
		UnitAny* ptItem = D2Funcs.D2COMMON_GetCursorItem(pUnit->pInventory);
		if(!ptItem) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Put Item On The Cursor!");  return false;}

		WardenClient_i ptCurrentClient = GetClientByID(ClientID);
		if(ptCurrentClient  == hWarden.Clients.end()) return TRUE;

		if(!isAnAdmin(ptCurrentClient->AccountName)) { UNLOCK return TRUE; }

		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"#iperson <charname>"); return false;}
		if(strlen(str)>15)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Max 15 chars!"); return false;}
		strcpy_s(ptItem->pItemData->szPlayerName,16,str);
		ptItem->pItemData->dwItemFlags.bPersonalized=1;
		ptItem->pItemData->dwItemFlags.bNamed=1;
		D2Funcs.D2COMMON_SetItemFlag(ptItem,ITEMFLAG_NEWITEM,1);
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Ok.");
		UNLOCK
		return false;
		}
		if(_stricmp(str,"#iunperson")==0)
		{
		UnitAny* ptItem = D2Funcs.D2COMMON_GetCursorItem(pUnit->pInventory);
		if(!ptItem) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Put Item On The Cursor!");  return false;}

		WardenClient_i ptCurrentClient = GetClientByID(ClientID);
		if(ptCurrentClient  == hWarden.Clients.end()) return TRUE;

		if(!isAnAdmin(ptCurrentClient->AccountName)) { UNLOCK return TRUE; }

		strcpy_s(ptItem->pItemData->szPlayerName,16,"");
		ptItem->pItemData->dwItemFlags.bPersonalized=0;
		ptItem->pItemData->dwItemFlags.bNamed=0;
		D2Funcs.D2COMMON_SetItemFlag(ptItem,ITEMFLAG_NEWITEM,1);
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Ok.");
		UNLOCK
		return false;
		}
		if(_stricmp(str,"#iq")==0)
		{
		UnitAny* ptItem = D2Funcs.D2COMMON_GetCursorItem(pUnit->pInventory);
		if(!ptItem) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Put Item On The Cursor!");  return false;}

		if(!isAnAdmin(pUnit->pPlayerData->pClientData->AccountName))  return TRUE; 
		
		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"1 = cracked 2 = normal 3 = superior 4 = magic 5 = set 6 = rare 7 = unique 8 = crafted"); return false;}
		int No = atoi(str);
		if(No>9 || !No)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"1 = cracked 2 = normal 3 = superior 4 = magic 5 = set 6 = rare 7 = unique 8 = crafted"); return false;}
		D2Funcs.D2COMMON_SetItemQuality(ptItem,No);
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Ok.");
		return false;
		}
		if(_stricmp(str,"#irem")==0)
		{
		WardenClient_i ptCurrentClient = GetClientByID(ClientID);
		if(ptCurrentClient  == hWarden.Clients.end()) return TRUE;

		if(!isAnAdmin(ptCurrentClient->AccountName)) { UNLOCK return TRUE; }

		str = strtok_s(NULL," ",&t);

		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"#irem <charname> <itemcode> <itemquality>"); UNLOCK return false;}

		UNLOCK
		WardenClient_i ptFound = GetClientByName(str);

		if(ptFound  == hWarden.Clients.end())  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Player isn't in game"); return false;}
		if(!ptFound->ptPlayer) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Player isn't inited"); UNLOCK return false;}

		char * code = strtok_s(NULL," ",&t);
		if(!code) { SendMsgToClient(pUnit->pPlayerData->pClientData,"#irem <charname> <itemcode>"); UNLOCK return false;}
		char StrCode[5] = {0};
		strncpy_s(StrCode,5,str,4);
		int ICode = TransCode(StrCode);
		int Idx = 0;
		if(!D2Funcs.D2COMMON_GetItemIdx(ICode,&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); UNLOCK return false;}
		char* szq = strtok_s(NULL," ",&t);
		int iQual = ITEM_QUALITY_INVALID;
		if(szq) iQual = atoi(szq);
		if(iQual>9 || iQual<0)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"1 = cracked 2 = normal 3 = superior 4 = magic 5 = set 6 = rare 7 = unique 8 = crafted"); UNLOCK return false;}
		
		Inventory * ptInv = ptFound->ptPlayer->pInventory;
		
		if(!ptInv)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot find inventory!"); UNLOCK return false;}
		for(UnitAny* i = D2Funcs.D2COMMON_GetFirstItem(ptInv); i; i = D2Funcs.D2COMMON_GetNextItem(i))
		{
		if(!D2Funcs.D2COMMON_UnitIsItem(i)) continue;
		DWORD dwItemCode = D2Funcs.D2COMMON_GetItemCode(i);
		if(dwItemCode == ICode)  
		if(!iQual || iQual == i->pItemData->QualityNo)
		{ 
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Deleted an item '%s from %s'",TransCode(dwItemCode),str);
		BYTE aPacket[] = {0x0A,0x04,0,0,0,0}; *(DWORD*)&aPacket[2]=i->dwUnitId;
		if(ptFound->ptClientData->InitStatus) D2ASMFuncs::D2GAME_SendPacket(ptFound->ptClientData,aPacket,6);
		D2Funcs.D2COMMON_FlushItem(ptInv,i);
		D2Funcs.D2GAME_DeleteUnit(ptFound->ptGame,i);
		//D2ASMFuncs::D2GAME_RemoveItem(ptFound->ptGame,ptFound->ptPlayer,i);
	    i =  D2Funcs.D2COMMON_GetFirstItem(ptInv);
		}
		if(!i) break;
		}
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Ok.");
		UNLOCK
		return false;
		}
		if(_stricmp(str,"#ic")==0)
		{
		UnitAny* ptItem = D2Funcs.D2COMMON_GetCursorItem(pUnit->pInventory);
		if(!ptItem) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Put Item On The Cursor!");  return false;}

		if(!isAnAdmin(pUnit->pPlayerData->pClientData->AccountName))  return TRUE; 

		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type item code (3 chars aqv = arrows)"); return false;}
		char StrCode[5] = {0};
		strncpy_s(StrCode,5,str,4);
		int ICode = TransCode(StrCode);
		int Idx = 0;
		int iQual = 2;
		if(!D2Funcs.D2COMMON_GetItemIdx(ICode,&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); return false;}
		ptItem->dwClassId=Idx;		
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Ok.");
		return false;
		}
		if(_stricmp(str,"#ipre")==0)
		{
		UnitAny* ptItem = D2Funcs.D2COMMON_GetCursorItem(pUnit->pInventory);
		if(!ptItem) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Put Item On The Cursor!");  return false;}

		if(!isAnAdmin(pUnit->pPlayerData->pClientData->AccountName))  return TRUE; 

		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type item prefix"); return false;}
		int Idx = atoi(str);
		ptItem->pItemData->MagicPrefix[0]=Idx;
		D2Funcs.D2COMMON_SetItemFlag(ptItem,ITEMFLAG_NEWITEM,1);
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Ok.");
		return false;
		}
		if(_stricmp(str,"#isut")==0)
		{
		UnitAny* ptItem = D2Funcs.D2COMMON_GetCursorItem(pUnit->pInventory);
		if(!ptItem) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Put Item On The Cursor!");  return false;}

		if(!isAnAdmin(pUnit->pPlayerData->pClientData->AccountName))  return TRUE; 

		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type item sutfix"); return false;}
		int Idx = atoi(str);
		ptItem->pItemData->MagicSutfix[0]=Idx;
		D2Funcs.D2COMMON_SetItemFlag(ptItem,ITEMFLAG_NEWITEM,1);
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Ok.");
		return false;
		}
		if(_stricmp(str,"#iuni")==0)
		{
		UnitAny* ptItem = D2Funcs.D2COMMON_GetCursorItem(pUnit->pInventory);
		if(!ptItem) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Put Item On The Cursor!");  return false;}

		if(!isAnAdmin(pUnit->pPlayerData->pClientData->AccountName))  return TRUE; 

		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-400"); return false;}
		int No = atoi(str);
		if(No>400)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-400"); return false;}
		if(ptItem->pItemData->QualityNo!=ITEM_QUALITY_UNIQUE)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Item must be unique!"); return false;}
		ptItem->pItemData->FileIndex=No;
		D2Funcs.D2COMMON_SetItemFlag(ptItem,ITEMFLAG_NEWITEM,1);
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Ok.");
		return false;
		}
		if(_stricmp(str,"#iadd")==0)
		{
		UnitAny* ptItem = D2Funcs.D2COMMON_GetCursorItem(pUnit->pInventory);
		if(!ptItem) {SendMsgToClient(pUnit->pPlayerData->pClientData,"Put Item On The Cursor!");  return false;}

		if(!isAnAdmin(pUnit->pPlayerData->pClientData->AccountName))  return TRUE; 
		
		int Val2 = 0; 
		int Val = 0;
		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-358, ItemStatCost.Txt"); return false;}
		int No = atoi(str);
		if(No>358)  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type number 0-358, ItemStatCost.Txt"); return false;}
		str = strtok_s(NULL," ",&t);
		if(!str) { SendMsgToClient(pUnit->pPlayerData->pClientData,"Type value"); return false;}
		Val = atoi(str);
		str = strtok_s(NULL," ",&t);
		if(str) Val2 = atoi(str);
		StatList* ptList = ptItem->pStatsEx->pMyLastList;

		if(ptList->dwOwnerId!=ptItem->dwUnitId)
		{
		for(;ptList ; ptList=ptList->pPrevLink) if(ptList->dwOwnerId==ptItem->dwUnitId) break;
		}
		D2Funcs.D2COMMON_AddStatToStatList(ptList,No,Val,Val2);
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Added Stat No %d, value %d, %d to an item", No, Val, Val2); 
		D2Funcs.D2COMMON_SetItemFlag(ptItem,ITEMFLAG_NEWITEM,1);
		D2Funcs.D2COMMON_AddToContainer(pUnit->pInventory,ptItem);
		D2Funcs.D2COMMON_SetBeginFlag(pUnit,1);
		SendMsgToClient(pUnit->pPlayerData->pClientData,"Ok.");
		return false;
		}

		return true;
}

void CreateFFAItems(UnitAny *pUnit)
{
	
		UnitAny* pNPC = FindUnitByClassId(pUnit->pGame,1,148); //AKARA
		if(pNPC)
		{
		if(!pNPC->pInventory)	D2Funcs.D2COMMON_AllocInventory(pNPC->pMemPool,pNPC);

		CreateItem sets;
		memset(&sets,0,sizeof(sets));
		sets.pOwner=pUnit;
		sets.wItemFormat=pUnit->pGame->ItemFormat;
		sets.pGame=pUnit->pGame;
		sets.iMode=ITEM_MODE_ON_CURSOR;
		sets.iQuality=ITEM_QUALITY_SUPERIOR;
		sets.dwItemFlags.bIdentified=1;
		sets.dwItemFlags.bInStore=1;
		sets.wCreateFlags=1; 
		sets.iLvl=99;
		//sets._26=8;//8- allow eth 2- allow socket

		//BUG BELT
		if(!isItemBelongToUnit(pNPC,' cvu',ITEM_QUALITY_TEMPERED))
		{
		int Idx = 0;
		if(!D2Funcs.D2COMMON_GetItemIdx(' cvu',&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); return;}	

		sets.iIdx=Idx;
		UnitAny* ptItem = D2Funcs.D2GAME_CreateItemEx(pUnit->pGame,&sets,0);
		if(ptItem)
		{
		ptItem->pItemData->dwItemFlags.bEthereal=false;
		int aP = D2Funcs.D2COMMON_GetInvPage(ptItem);
		D2Funcs.D2COMMON_SetInvPage(ptItem,aP);

		D2Funcs.D2GAME_AddItemToNPC(__FILE__,__LINE__,pUnit->pGame,pNPC,ptItem->dwUnitId,0,0,1,1,0);

		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,16,0,0); //ac% remove
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,75,0,0); //dura% remove
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,127,1,0);// +1 All skills
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,93,10,0);// 10% ias
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,156,1,0);// Piercing Attack
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,60,5,0); // 5% ll
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,62,5,0); // 5% ml
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,150,10,0);// %10 slow
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,0,15,0);// 15+ str
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,2,25,0);// 20+ dex
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,39,20,0);// 20+ fire
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,43,20,0);// 20+ cold
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,41,20,0);// 20+ lgith
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,45,20,0);// 20+ psn
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,36,20,0);//  20% dr
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,138,2,0);// +2 Mana /kill
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,89,-3,0);// -3 Light Radious
		ptItem->pItemData->QualityNo=9;
		ptItem->pItemData->RarePrefix=166;
		ptItem->pItemData->RareSutfix=22;

		D2Funcs.D2COMMON_SetBeginFlag(pNPC,1);
		D2Funcs.D2COMMON_AddToContainer(pNPC->pInventory,ptItem);

		}
		}
		//MANA POTION
		if(!isItemBelongToUnit(pNPC,' 5pm',ITEM_QUALITY_TEMPERED))
		{
		int Idx = 0;
		if(!D2Funcs.D2COMMON_GetItemIdx(' 5pm',&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); return;}	
		sets.iIdx=Idx;
		//sets.iQuality=ITEM_QUALITY_NORMAL;
		UnitAny* ptItem = D2Funcs.D2GAME_CreateItemEx(pUnit->pGame,&sets,0);
		if(ptItem)
		{
		int aP = D2Funcs.D2COMMON_GetInvPage(ptItem);
		D2Funcs.D2COMMON_SetInvPage(ptItem,aP);

		D2Funcs.D2GAME_AddItemToNPC(__FILE__,__LINE__,pUnit->pGame,pNPC,ptItem->dwUnitId,0,0,1,1,0);
		ptItem->pItemData->QualityNo=9;
		D2Funcs.D2COMMON_SetBeginFlag(pNPC,1);
		D2Funcs.D2COMMON_AddToContainer(pNPC->pInventory,ptItem);
		}
		}
		//451 psn sc
		if(!isItemBelongToUnit(pNPC,' 1mc',ITEM_QUALITY_TEMPERED))
		{
		int Idx = 0;
		if(!D2Funcs.D2COMMON_GetItemIdx(' 1mc',&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); return;}	
		sets.iIdx=Idx;
		UnitAny* ptItem = D2Funcs.D2GAME_CreateItemEx(pUnit->pGame,&sets,0);
		if(ptItem)
		{
		int aP = D2Funcs.D2COMMON_GetInvPage(ptItem);
		D2Funcs.D2COMMON_SetInvPage(ptItem,aP);

		D2Funcs.D2GAME_AddItemToNPC(__FILE__,__LINE__,pUnit->pGame,pNPC,ptItem->dwUnitId,0,0,1,1,0);
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,57,385,0); //+ 451 psn
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,58,385,0); //+ 451 psn max
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,59,300,0); //12 sec
		ptItem->pItemData->QualityNo=9;
		D2Funcs.D2COMMON_SetBeginFlag(pNPC,1);
		D2Funcs.D2COMMON_AddToContainer(pNPC->pInventory,ptItem);
		}
		}
		//BUG TAL	
		if(!isItemBelongToUnit(pNPC,' htu',ITEM_QUALITY_TEMPERED))
		{
		int Idx = 0;
		if(!D2Funcs.D2COMMON_GetItemIdx(' htu',&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); return;}	
		sets.iIdx=Idx;
		//sets.iQuality=ITEM_QUALITY_SET;
		sets.dwItemFlags.bSocketed=1;
		UnitAny* ptItem = D2Funcs.D2GAME_CreateItemEx(pUnit->pGame,&sets,0);
		if(ptItem)
		{
		ptItem->pItemData->dwItemFlags.bEthereal=false;
		int aP = D2Funcs.D2COMMON_GetInvPage(ptItem);
		D2Funcs.D2COMMON_SetInvPage(ptItem,aP);
		D2Funcs.D2GAME_AddItemToNPC(__FILE__,__LINE__,pUnit->pGame,pNPC,ptItem->dwUnitId,0,0,1,1,0);
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,16,0,0); //ac% remove
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,75,0,0); //dura% remove
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,127,2,0);//+2 All skills
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,31,400,0);//+400 Defense
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,96,25,0);//+25 frw
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,39,40,0);// 40+ fire
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,43,40,0);// 40+ cold
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,41,40,0);// 40+ lgith
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,45,40,0);// 40+ psn
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,36,25,0);//  25% dr
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,35,15,0);//  15 mdr
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,91,-60,0);//  -60 req
		D2Funcs.D2COMMON_SetStat(ptItem,194,1,0);									// (1) Socket
		ptItem->pItemData->QualityNo=9;
		ptItem->pItemData->RarePrefix=13;
		ptItem->pItemData->RareSutfix=37;

		D2Funcs.D2COMMON_SetBeginFlag(pNPC,1);
		D2Funcs.D2COMMON_AddToContainer(pNPC->pInventory,ptItem);
		}
		}
		//VALK 08
		if(!isItemBelongToUnit(pNPC,' mhx',ITEM_QUALITY_TEMPERED))
		{
		int Idx = 0;
		if(!D2Funcs.D2COMMON_GetItemIdx(' mhx',&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); return;}	
		sets.iIdx=Idx;
		UnitAny* ptItem = D2Funcs.D2GAME_CreateItemEx(pUnit->pGame,&sets,0);
		if(ptItem)
		{
		ptItem->pItemData->dwItemFlags.bEthereal=false;
		int aP = D2Funcs.D2COMMON_GetInvPage(ptItem);
		D2Funcs.D2COMMON_SetInvPage(ptItem,aP);
		D2Funcs.D2GAME_AddItemToNPC(__FILE__,__LINE__,pUnit->pGame,pNPC,ptItem->dwUnitId,0,0,1,1,0);

		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,16,0,0); //ac% remove
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,75,0,0); //dura% remove

		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,31,35,0);//+35 Defense
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,96,30,0);//+30 frw
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,93,30,0);//+30 ias
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,105,30,0);//+30 fcr
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,99,30,0);//+30 frw
		D2Funcs.D2COMMON_SetStat(ptItem,194,1,0);									// (1) Socket
		ptItem->pItemData->QualityNo=9;
		ptItem->pItemData->RarePrefix=77;
		ptItem->pItemData->RareSutfix=87;

		D2Funcs.D2COMMON_SetBeginFlag(pNPC,1);
		D2Funcs.D2COMMON_AddToContainer(pNPC->pInventory,ptItem);
		}
		}
		//Faith
		if(!isItemBelongToUnit(pNPC,' cma',ITEM_QUALITY_TEMPERED))
		{
		int Idx = 0;
		if(!D2Funcs.D2COMMON_GetItemIdx(' cma',&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); return;}	
		sets.iIdx=Idx;
		sets.dwItemFlags.bSocketed=0;
		UnitAny* ptItem = D2Funcs.D2GAME_CreateItemEx(pUnit->pGame,&sets,0);
		if(ptItem)
		{
		ptItem->pItemData->dwItemFlags.bEthereal=0;
		ptItem->pItemData->dwItemFlags.bSocketed=0;
		int aP = D2Funcs.D2COMMON_GetInvPage(ptItem);
		D2Funcs.D2COMMON_SetInvPage(ptItem,aP);
		D2Funcs.D2GAME_AddItemToNPC(__FILE__,__LINE__,pUnit->pGame,pNPC,ptItem->dwUnitId,0,0,1,1,0);

		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,17,345,0); // 345% ed
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,18,345,0); // 345% ed
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,19,3,0); // 3 ar
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,188,3,0);// 3+ bow (tab no 0)

		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,151,15,122);//15 lvl fana (122 skill Id)
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,127,2,0);//+2 All skills
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,115,1,0);//ignore tgac
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,119,300,0);//+300% ar
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,48,120,0);//+120 fire dmg
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,49,120,0);//+120 fire dmg
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,39,15,0);// 15+ fire
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,43,15,0);// 15+ cold
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,41,15,0);// 15+ lgith
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,45,15,0);// 15+ psn
		D2Funcs.D2COMMON_SetStat(ptItem,194,0,0);
		
		ptItem->pItemData->QualityNo=9;
		ptItem->pItemData->RarePrefix=23;
		ptItem->pItemData->RareSutfix=54;

		D2Funcs.D2COMMON_SetBeginFlag(pNPC,1);
		D2Funcs.D2COMMON_AddToContainer(pNPC->pInventory,ptItem);
		}
		}
		//Faith MB
		if(!isItemBelongToUnit(pNPC,' bma',ITEM_QUALITY_TEMPERED))
		{
		int Idx = 0;
		if(!D2Funcs.D2COMMON_GetItemIdx(' bma',&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); return;}	
		sets.iIdx=Idx;
		sets.dwItemFlags.bSocketed=0;
		UnitAny* ptItem = D2Funcs.D2GAME_CreateItemEx(pUnit->pGame,&sets,0);
		if(ptItem)
		{
		ptItem->pItemData->dwItemFlags.bEthereal=0;
		ptItem->pItemData->dwItemFlags.bSocketed=0;
		int aP = D2Funcs.D2COMMON_GetInvPage(ptItem);
		D2Funcs.D2COMMON_SetInvPage(ptItem,aP);
		D2Funcs.D2GAME_AddItemToNPC(__FILE__,__LINE__,pUnit->pGame,pNPC,ptItem->dwUnitId,0,0,1,1,0);

		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,17,345,0); // 345% ed
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,18,345,0); // 345% ed
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,19,3,0); // 3 ar
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,188,3,0);// 3+ bow (tab no 0)

		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,151,15,122);//15 lvl fana (122 skill Id)
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,127,2,0);//+2 All skills
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,115,1,0);//ignore tgac
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,119,300,0);//+300% ar
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,48,120,0);//+120 fire dmg
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,49,120,0);//+120 fire dmg
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,39,15,0);// 15+ fire
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,43,15,0);// 15+ cold
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,41,15,0);// 15+ lgith
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,45,15,0);// 15+ psn
		D2Funcs.D2COMMON_SetStat(ptItem,194,0,0);
		
		ptItem->pItemData->QualityNo=9;
		ptItem->pItemData->RarePrefix=23;
		ptItem->pItemData->RareSutfix=54;

		D2Funcs.D2COMMON_SetBeginFlag(pNPC,1);
		D2Funcs.D2COMMON_AddToContainer(pNPC->pInventory,ptItem);
		}
		}
		//Beast
		if(!isItemBelongToUnit(pNPC,' aw7',ITEM_QUALITY_TEMPERED))
		{
		int Idx = 0;
		if(!D2Funcs.D2COMMON_GetItemIdx(' aw7',&Idx))  { SendMsgToClient(pUnit->pPlayerData->pClientData,"Cannot get item idx"); return;}	
		sets.iIdx=Idx;
		sets.dwItemFlags.bSocketed=0;
		UnitAny* ptItem = D2Funcs.D2GAME_CreateItemEx(pUnit->pGame,&sets,0);
		if(ptItem)
		{
		ptItem->pItemData->dwItemFlags.bEthereal=0;
		ptItem->pItemData->dwItemFlags.bSocketed=0;
		int aP = D2Funcs.D2COMMON_GetInvPage(ptItem);
		D2Funcs.D2COMMON_SetInvPage(ptItem,aP);
		D2Funcs.D2GAME_AddItemToNPC(__FILE__,__LINE__,pUnit->pGame,pNPC,ptItem->dwUnitId,0,0,1,1,0);

		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,17,285,0); // 285% ed
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,18,285,0); // 285% ed
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,19,3,0); // 3 ar

		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,151,9,122);//9 lvl fana (122 skill Id)
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,93,40,0);//+40 ias
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,136,20,0);// +20 cb
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,135,25,0);// +25 ow
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,0,40,0);// +40 str
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,1,10,0);// +10 ene
		D2Funcs.D2COMMON_AddStatToStatList(ptItem->pStatsEx->pMyLastList,138,2,0);// +2 Mana /kill
		D2Funcs.D2COMMON_SetStat(ptItem,194,0,0);
		ptItem->pItemData->QualityNo=9;
		ptItem->pItemData->RarePrefix=23;
		ptItem->pItemData->RareSutfix=57;

		D2Funcs.D2COMMON_SetBeginFlag(pNPC,1);
		D2Funcs.D2COMMON_AddToContainer(pNPC->pInventory,ptItem);
		}
		}
		}
}