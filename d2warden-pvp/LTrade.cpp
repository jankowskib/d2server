#include "stdafx.h"

#include "LTrade.h"
#include "D2Ptrs.h"
#include "D2Structs.h"
#include "D2Stubs.h"
#include "WardenMisc.h"

#include "ARTransferGoldBinding.nsmap"
#include "soapARQuerySaldoBindingProxy.h"
#include "soapARTransferGoldBindingProxy.h"

#include <sstream>

void TRADE_UpdateUnit(UnitAny* pUnit, WORD UpdateType, UnitAny*pUpdateUnit) //UType = 19 - 'Nie mogê!', 2 - 'Lvl Up Sound'
{
	ASSERT(pUnit)
	pUnit->UpdateType=UpdateType;
	pUnit->pUpdateUnit=pUpdateUnit;
	D2COMMON_UpdateOverlay(pUnit);
	pUnit->dwFlags|= 0x400;
}

BOOL __fastcall TRADE_OnARGoldPass(Game *pGame, UnitAny *pPlayerUnit, GoldPacket *aPacket, int aLen)
{
if(aLen != 18) return 3;
if(!pPlayerUnit->bInteracting) return 3;
if(!pPlayerUnit->pPlayerData->pTrade) {Log("HACK: (*%s) <Proba exploitu> - Wyslano haslo AR Gold bedac poza trade!",pPlayerUnit->pPlayerData->pClientData->AccountName); return 3; }

pPlayerUnit->pPlayerData->pTrade->GoldPass = aPacket->szPass;

return 0;
}

int TRADE_GetARGoldSaldo(string szAcc)
{
	//if(szAcc == "") szAcc = "LOLET";
	ARQuerySaldoBindingProxy hQuery;
	int nGold;

	if(hQuery.ARQuerySaldo(szAcc,nGold) != SOAP_OK) 
	{
		ostringstream sperror;
		hQuery.soap_stream_fault(sperror);
		Log("ARGoldQuerySaldo: Wystapil blad podczas pobierania salda gracza (*%s), blad : '%s'",szAcc.c_str(),sperror.str().c_str());
		return -1;
	}
//	else Log("Pobrano saldo gracza (*%s), %d",szAcc.c_str(),nGold);
	return nGold;
}

bool TRADE_TransferARGold(UnitAny* pSender, UnitAny* pReceiver, int Amount)
{
 ARTransferGoldBindingProxy hQuery;

 string szSender(pSender->pPlayerData->pClientData->AccountName);
 string szReceiver(pReceiver->pPlayerData->pClientData->AccountName);
 ostringstream szDesc;
 szDesc << pSender->pGame->GameName << " / " << pSender->pGame->GamePass;
 //szSender = "LOLET";
 //szReceiver = "LOLET";
 string szPass(pSender->pPlayerData->pTrade->GoldPass);
 string wynik;

 if(hQuery.ARTransferGold(szSender,szReceiver,Amount,szDesc.str(),"/UNDONE/","/UNDONE/",szPass,"ardupa098gold",wynik) != SOAP_OK)
 {
		Log("ARTransferGoldBinding: Wystapil blad podczas transferu gracza (*%s), blad : '%s'",szSender.c_str(),hQuery.soap_fault_string());
		return false;
 }

 if(wynik=="OK") return true;
 else Log("ARTransfer FAIL: Kod bledu %s", wynik.c_str());

 return false;
}

BYTE __fastcall TRADE_AllocTradeStrc(Game* pGame, UnitAny* pUnit)
{
	if(!pUnit) return 0;
	PlayerData* pData = pUnit->pPlayerData;
	if(pData->pTrade) return 0;

	PlayerTrade* pTrade = new PlayerTrade;
	BYTE *pItemBuffer = new BYTE[8192];
	pTrade->pSaveBuffer = pItemBuffer;
	pTrade->nSaveLen = D2GAME_SaveItems(pItemBuffer,0,pGame,pUnit,pUnit->pInventory,8192,0,0,0);
	ASSERT(pTrade->nSaveLen>0)
	pTrade->pContainer = 0;
	pTrade->nOfferedGold = 0;
	pTrade->nGreenCheckTick = 0;
	pTrade->ARGold = 0;
	pTrade->GoldPass.reserve(16);
	pTrade->GoldNo = D2COMMON_GetStatSigned(pUnit,STAT_GOLD,0);

	int ARGold = TRADE_GetARGoldSaldo(pData->pClientData->AccountName);
	if( ARGold != -1)
	{
	pTrade->ARGold = ARGold;
	D2COMMON_SetStat(pUnit,STAT_GOLD,ARGold,0);
	}
	else
	return 0;

	pData->pTrade = pTrade;

	return 1;
}

void TRADE_FreeTradeStrc(Game* pGame, UnitAny* pPlayer)
{
	PlayerData * pData = pPlayer->pPlayerData;
	ASSERT(pData);
	if(!pData->pTrade) return;
	delete[] pData->pTrade->pSaveBuffer;
	for(TradeContainer * i = pData->pTrade->pContainer; i; i = i->pNext) FOG_FreeServerMemory(pGame->pMemPool,i,__FILE__,__LINE__,0);
	pData->pTrade->pContainer = 0;
	delete pData->pTrade;
	pData->pTrade = 0;
}

void TRADE_UpdateClientUI(ClientData* pClient, UpdateClient How)
{
	BYTE aPacket[2];
	aPacket[0] = 0x77;
	aPacket[1] = How;

	D2GAME_SendPacket(pClient,aPacket,2);
}

void TRADE_SendTradeAccepted(ClientData* pClient, DWORD UnitId, char* szName)
{
	TradeData aPacket;
	aPacket.Header = 0x78;
	aPacket.UnitId = UnitId;
	strcpy_s(aPacket.szName,16,szName);

	D2GAME_SendPacket(pClient,(BYTE*)&aPacket,21);
}

void TRADE_SendOfferedGold(ClientData* pClient, bool bMyUnit, DWORD nGold)
{
	TradeGold aPacket;
	aPacket.Header = 0x79;
	aPacket.bMyUnit = bMyUnit ? 1 : 0;
	aPacket.nGold = nGold;

	D2GAME_SendPacket(pClient,(BYTE*)&aPacket,sizeof(TradeGold));
}

void TRADE_StashAction(UnitAny* pPlayer, Game *pGame, short ButtonId, int nGold)
{

	UnitAny* pObject = D2GAME_FindUnit(pGame,pPlayer->dwInteractId,UNIT_OBJECT);
	if(!pObject) return;
	if(pObject->dwClassId!=267) return; //BANK

	Room1* pRoom = D2COMMON_GetUnitRoom(pPlayer);
	if(!pRoom) return;
	if(!D2COMMON_isRoomInTown(pRoom)) return;
	Room1* pObjRoom = D2COMMON_GetUnitRoom(pObject);
	if(!pObjRoom) return;
	if(!D2COMMON_isRoomInTown(pObjRoom)) return;

	switch(ButtonId)
	{
		case TRADE_CLOSE_STASH:
			{
				TRADE_RemoveInteraction(pPlayer);
				D2GAME_UpdateQuantity(pPlayer);
			}
		break;
		case TRADE_STASH_WITHDRAW:
			{
				int nGoldBank = D2COMMON_GetStatSigned(pPlayer,STAT_GOLDBANK,0);

				if(nGold<=0) break;
				if(nGold>nGoldBank) break;

				if((nGold+D2COMMON_GetStatSigned(pPlayer,STAT_GOLD,0)) > D2COMMON_GetGoldLimit(pPlayer)) TRADE_UpdateUnit(pPlayer,19,pPlayer);
				else
				{
				D2GAME_AddGold(pPlayer,pGame,nGold);
				int BankNew = nGoldBank -nGold;
				if(BankNew<0 || BankNew > D2COMMON_GetBankLimit(pPlayer))	D2COMMON_SetStat(pPlayer,STAT_GOLDBANK,0,0);
				else
				D2COMMON_AddStat(pPlayer,STAT_GOLDBANK,-nGold,0);
				}		
			}
		break;
		case TRADE_STASH_DEPOSIT:
			{
			int nCharGold = D2COMMON_GetStatSigned(pPlayer,STAT_GOLD,0);

			if(nGold<=0) break;
			if(nGold>nCharGold) break;
			
			int nBankLimit = D2COMMON_GetBankLimit(pPlayer);
			int nGoldBank= D2COMMON_GetStatSigned(pPlayer,STAT_GOLDBANK,0);
			

			if(nGoldBank + nGold <= nBankLimit)
				{
				int GoldNew = nCharGold - nGold;
				if(GoldNew<0 || GoldNew > D2COMMON_GetGoldLimit(pPlayer))	D2COMMON_SetStat(pPlayer,STAT_GOLD,0,0);
				else
				D2COMMON_AddStat(pPlayer,STAT_GOLD,-nGold,0);

				int BankNew = nGoldBank + nGold;
				if(BankNew<0 || BankNew > D2COMMON_GetBankLimit(pPlayer))	D2COMMON_SetStat(pPlayer,STAT_GOLDBANK,0,0);
				else
				D2COMMON_AddStat(pPlayer,STAT_GOLDBANK,nGold,0);

				}
			else
			if(nGoldBank < nBankLimit)
				{
				int BankNew = nGoldBank + (nBankLimit - nGoldBank);
				if(BankNew<0 || BankNew > D2COMMON_GetBankLimit(pPlayer))	D2COMMON_SetStat(pPlayer,STAT_GOLDBANK,0,0);
				else
				D2COMMON_AddStat(pPlayer,STAT_GOLDBANK, (nBankLimit - nGoldBank) ,0);

				nGoldBank= D2COMMON_GetStatSigned(pPlayer,STAT_GOLDBANK,0);

				int GoldNew = nCharGold + (nGoldBank - nBankLimit);
				if(GoldNew<0 || GoldNew > D2COMMON_GetGoldLimit(pPlayer))	D2COMMON_SetStat(pPlayer,STAT_GOLD,0,0);
				else
				D2COMMON_AddStat(pPlayer,STAT_GOLD,(nGoldBank - nBankLimit),0);	

				}

			}
		break;
	}

}

bool TRADE_ValidateContainers(Game* pGame, UnitAny* pPlayer)
{
	if(pPlayer->dwFlags2 & 0x400000 || pPlayer->dwFlags2 & 0x800000) return true;

	for(ItemCon* pItemCon = pPlayer->pInventory->pFirstCointainer; pItemCon; pItemCon = pItemCon->pNext)
	{
		int ItemId = pItemCon->ItemId;
		UnitAny* pItem = pGame->pUnitList[3][ItemId & 127];
		if(!pItem) continue;
		if(pItem->dwUnitId != ItemId) return true;
	}
	return false;
}

void TRADE_RemoveInteraction(UnitAny* pUnit)
{
	pUnit->dwInteractId = -1;
	pUnit->dwInteractType = 6;
	pUnit->bInteracting = 0;
}


void TRADE_RestoreItemsAndGold(Game* pGame, UnitAny* pPlayer)
{
	PlayerTrade * pTrade = pPlayer->pPlayerData->pTrade;
	if(!pTrade) return;

	int Len;
	D2GAME_RestoreItems(pGame,pTrade->pSaveBuffer,pPlayer,96,8192,0,&Len);
	//int nGold = D2COMMON_GetStatSigned(pPlayer,STAT_GOLD,0);
	//if(nGold<0) return;
	if(pPlayer->dwMode != PLAYER_MODE_DEATH && pPlayer->dwMode != PLAYER_MODE_DEAD)
	{
		D2COMMON_SetStat(pPlayer,STAT_GOLD,0,0);

		int GoldNew = pTrade->GoldNo;
		if(GoldNew<0 || GoldNew > D2COMMON_GetGoldLimit(pPlayer))	D2COMMON_SetStat(pPlayer,STAT_GOLD,0,0);
		else
		D2COMMON_AddStat(pPlayer,STAT_GOLD,GoldNew,0);

	}
}


void TRADE_RestorePreTradeState(Game* pGame, UnitAny* pPlayer)
{
PostTradePacket aPacket;

	if(!pPlayer->pPlayerData->pTrade) return;
	aPacket.Header = 0x92;
	aPacket.UnitType = 0;
	aPacket.UnitId = pPlayer->dwUnitId;
	D2GAME_SendPacket(pPlayer->pPlayerData->pClientData,(BYTE*)&aPacket,sizeof(PostTradePacket));
	D2GAME_ForEach(pGame,(void (__fastcall *)(Game *,UnitAny *,void *))D2GAME_Send0X92Packet,pPlayer);

	for(UnitAny* pItem = pPlayer->pInventory->pFirstItem; pItem; pItem = pItem->pItemData->pNextItem)
	{
		if(pItem->dwType != UNIT_ITEM) continue;
		if(pItem->dwMode == ITEM_MODE_INV_STASH_CUBE_STORE) D2GAME_UpdateItemQuantity(pPlayer,pItem);
		D2COMMON_AddToContainer(pPlayer->pInventory,pItem);
		D2COMMON_SetItemCMDFlag(pItem,1,1);

	}

	UnitAny* pCursor = D2COMMON_GetCursorItem(pPlayer->pInventory);
	if(pCursor)
	if(pCursor->dwType == UNIT_ITEM)
	{
		D2COMMON_AddToContainer(pPlayer->pInventory,pCursor);
		D2COMMON_SetItemCMDFlag(pCursor,1,1);
	}
	D2GAME_RemoveBonuses(pGame,pPlayer);
	TRADE_RestoreItemsAndGold(pGame,pPlayer);
}

void TRADE_ResetTrade(Game* pGame, UnitAny* pPlayer, UnitAny* pTrader)
{
	D2GAME_UpdateQuantity(pPlayer);
	D2GAME_UpdateQuantity(pTrader);

	D2COMMON_SetBeginFlag(pPlayer,1);
	D2COMMON_SetBeginFlag(pTrader,1);

	pPlayer->dwFlags2 |= 0x800000;
	pTrader->dwFlags2 |= 0x800000;

	pTrader->pPlayerData->pTrade->nOfferedGold = pPlayer->pPlayerData->pTrade->nOfferedGold = 0;

	TRADE_RestorePreTradeState(pGame,pPlayer);
	TRADE_RestorePreTradeState(pGame,pTrader);

	D2GAME_UpdateClientInventory(pPlayer->pPlayerData->pClientData,pPlayer);
	D2GAME_UpdateClientInventory(pTrader->pPlayerData->pClientData,pTrader);

	D2COMMON_SetStat(pPlayer,STAT_GOLD,0,0);
	D2COMMON_SetStat(pTrader,STAT_GOLD,0,0);

	int ARGold = TRADE_GetARGoldSaldo(pPlayer->pPlayerData->pClientData->AccountName);
	if( ARGold != -1)
	{
	pPlayer->pPlayerData->pTrade->ARGold = ARGold;
	D2COMMON_SetStat(pPlayer,STAT_GOLD,ARGold,0);
	}

	ARGold = TRADE_GetARGoldSaldo(pTrader->pPlayerData->pClientData->AccountName);
	if( ARGold != -1)
	{
	pTrader->pPlayerData->pTrade->ARGold = ARGold;
	D2COMMON_SetStat(pTrader,STAT_GOLD,ARGold,0);
	}
		
	TRADE_SendOfferedGold(pPlayer->pPlayerData->pClientData,0,0);
	TRADE_SendOfferedGold(pPlayer->pPlayerData->pClientData,1,0);
	TRADE_SendOfferedGold(pTrader->pPlayerData->pClientData,0,0);
	TRADE_SendOfferedGold(pTrader->pPlayerData->pClientData,1,0);

	TRADE_UpdateClientUI(pPlayer->pPlayerData->pClientData,UPDATE_OPENTRADE);
	TRADE_UpdateClientUI(pTrader->pPlayerData->pClientData,UPDATE_OPENTRADE);

	pPlayer->pPlayerData->nTradeState = pTrader->pPlayerData->nTradeState = 5;

	D2GAME_UpdateQuantity(pPlayer);
	D2GAME_UpdateQuantity(pTrader);

	D2GAME_InitTimer(pGame, pPlayer, 13, pGame->GameFrame +1, 0, 0, 0);
	D2GAME_InitTimer(pGame, pTrader, 13, pGame->GameFrame +1, 0, 0, 0);
}

bool TRADE_TransferItems(Game* pGame, UnitAny* pPlayer, UnitAny* pTrader)
{
	if(pPlayer->pPlayerData->pTrade->nOfferedGold>0)
	{
	int nSafeGold = TRADE_GetARGoldSaldo(pPlayer->pPlayerData->pClientData->AccountName);
	if(nSafeGold == -1 || nSafeGold != pPlayer->pPlayerData->pTrade->ARGold || pPlayer->pPlayerData->pTrade->nOfferedGold>nSafeGold) 
		{
			Log("TRADE: Uwaga! (*%s) cos kombinuje! nSafeGold (%d) != pPlayer->pPlayerData->pTrade->ARGold (%d) || pPlayer->pPlayerData->pTrade->nOfferedGold (%d) >nSafeGold (%d)",pPlayer->pPlayerData->pClientData->AccountName,nSafeGold,pPlayer->pPlayerData->pTrade->ARGold,pPlayer->pPlayerData->pTrade->nOfferedGold, nSafeGold); 
			return false;
		}
	}

	if(pTrader->pPlayerData->pTrade->nOfferedGold>0)
	{
	int nSafeGold = TRADE_GetARGoldSaldo(pTrader->pPlayerData->pClientData->AccountName);
	if(nSafeGold == -1 || nSafeGold != pTrader->pPlayerData->pTrade->ARGold || pTrader->pPlayerData->pTrade->nOfferedGold>nSafeGold) 
		{
			Log("TRADE: Uwaga! (*%s) cos kombinuje! nSafeGold (%d) != pTrader->pPlayerData->pTrade->ARGold (%d) || pTrader->pPlayerData->pTrade->nOfferedGold (%d) >nSafeGold (%d)",pTrader->pPlayerData->pClientData->AccountName,nSafeGold,pTrader->pPlayerData->pTrade->ARGold,pTrader->pPlayerData->pTrade->nOfferedGold, nSafeGold); 
			return false;
		}
	}

int bBuyerNoRoom;

if(!D2COMMON_AllocTradeInventory(pGame->pMemPool,pPlayer,pTrader,&bBuyerNoRoom))
{
pTrader->pPlayerData->pTrade->nOfferedGold = pPlayer->pPlayerData->pTrade->nOfferedGold = 0;
	if(bBuyerNoRoom)
	{
		TRADE_UpdateClientUI(pPlayer->pPlayerData->pClientData,UPDATE_SELLER_NOROOM);
		TRADE_UpdateClientUI(pTrader->pPlayerData->pClientData,UPDATE_BUYER_NOROOM);
		return false;
	}
	else
	{
		TRADE_UpdateClientUI(pPlayer->pPlayerData->pClientData,UPDATE_BUYER_NOROOM);
		TRADE_UpdateClientUI(pTrader->pPlayerData->pClientData,UPDATE_SELLER_NOROOM);
		return false;
	}
}

pPlayer->dwFlags2 |= 0x800000;
pTrader->dwFlags2 |= 0x800000;

pPlayer->pPlayerData->nTradeState = pTrader->pPlayerData->nTradeState = 9;

D2GAME_InitTimer(pGame, pPlayer, 13, pGame->GameFrame +1, 0, 0, 0);
D2GAME_InitTimer(pGame, pTrader, 13, pGame->GameFrame +1, 0, 0, 0);

if(!D2GAME_MoveItems(pGame,pTrader,pPlayer)) return false;
if(!D2GAME_MoveItems(pGame,pPlayer,pTrader)) return false;

if(pPlayer->pPlayerData->pTrade->nOfferedGold)
if(!TRADE_TransferARGold(pPlayer,pTrader,pPlayer->pPlayerData->pTrade->nOfferedGold)) return false;

if(pTrader->pPlayerData->pTrade->nOfferedGold)
if(!TRADE_TransferARGold(pTrader,pPlayer,pTrader->pPlayerData->pTrade->nOfferedGold)) return false;

if(pPlayer->dwMode != PLAYER_MODE_DEATH && pPlayer->dwMode != PLAYER_MODE_DEAD)
{
		D2COMMON_SetStat(pPlayer,STAT_GOLD,0,0);

		int GoldNew = pPlayer->pPlayerData->pTrade->GoldNo;
		if(GoldNew<0 || GoldNew > D2COMMON_GetGoldLimit(pPlayer))	D2COMMON_SetStat(pPlayer,STAT_GOLD,0,0);
		else
		D2COMMON_AddStat(pPlayer,STAT_GOLD,GoldNew,0);

}
if(pTrader->dwMode != PLAYER_MODE_DEATH && pTrader->dwMode != PLAYER_MODE_DEAD)
{
		D2COMMON_SetStat(pTrader,STAT_GOLD,0,0);

		int GoldNew = pTrader->pPlayerData->pTrade->GoldNo;
		if(GoldNew<0 || GoldNew > D2COMMON_GetGoldLimit(pTrader))	D2COMMON_SetStat(pTrader,STAT_GOLD,0,0);
		else
		D2COMMON_AddStat(pTrader,STAT_GOLD,GoldNew,0);

}

TRADE_RemoveInteraction(pPlayer);
TRADE_RemoveInteraction(pTrader);

TRADE_FreeTradeStrc(pGame,pPlayer);
TRADE_FreeTradeStrc(pGame,pTrader);

D2GAME_UpdateQuantity(pPlayer);
D2GAME_UpdateQuantity(pTrader);

TRADE_UpdateClientUI(pPlayer->pPlayerData->pClientData,UPDATE_TRADE_DONE);
TRADE_UpdateClientUI(pTrader->pPlayerData->pClientData,UPDATE_TRADE_DONE);

return true;
}

void TRADE_CloseTrade(Game* pGame, UnitAny* pPlayer, UnitAny* pTrader, short TradeState)
{
D2COMMON_SetBeginFlag(pPlayer,1);
D2COMMON_SetBeginFlag(pTrader,1);

pPlayer->dwFlags2 |= 0x800000;
pTrader->dwFlags2 |= 0x800000;

TRADE_RemoveInteraction(pPlayer);
TRADE_RemoveInteraction(pTrader);

TRADE_RestorePreTradeState(pGame,pPlayer);
TRADE_RestorePreTradeState(pGame,pTrader);

TRADE_FreeTradeStrc(pGame,pPlayer);
TRADE_FreeTradeStrc(pGame,pTrader);

pPlayer->pPlayerData->nTradeState = TradeState;
pTrader->pPlayerData->nTradeState = TradeState;

D2GAME_UpdateQuantity(pPlayer);
D2GAME_UpdateQuantity(pTrader);

TRADE_UpdateClientUI(pPlayer->pPlayerData->pClientData,UPDATE_CLOSE_TRADE);
TRADE_UpdateClientUI(pTrader->pPlayerData->pClientData,UPDATE_CLOSE_TRADE);

for(Timer * pTimer = pPlayer->pTimer; pTimer; pTimer = pTimer->pNextTimerOnUnit)
{
	if(pTimer->nTimerIdx == 13) D2GAME_DeleteTimer(pGame,pTimer);
}

for(Timer * pTimer = pTrader->pTimer; pTimer; pTimer = pTimer->pNextTimerOnUnit)
{
	if(pTimer->nTimerIdx == 13) D2GAME_DeleteTimer(pGame,pTimer);
}

D2GAME_InitTimer(pGame, pPlayer, 13, pGame->GameFrame +1, 0, 0, 0);
D2GAME_InitTimer(pGame, pTrader, 13, pGame->GameFrame +1, 0, 0, 0);
}

BOOL __fastcall TRADE_OnTradeAction(Game *pGame, UnitAny *pPlayerUnit, TradePacket *aPacket, int aLen)
{
	if(aLen!=7) return 3;
	if(!pGame) return 3;
	if(!pPlayerUnit) return 1;
	if(pPlayerUnit->dwType) return 3;

	if(!pPlayerUnit->bInteracting)
	{
		TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_CLOSE_TRADE);
		return 0;
	}
	int Argument = aPacket->Arg2 | (aPacket->Arg1 << 16);

	switch(aPacket->ButtonId)
	{
		case TRADE_CLOSE_STASH:
		case TRADE_STASH_DEPOSIT:
		case TRADE_STASH_WITHDRAW:
			{
				if(pPlayerUnit->dwInteractType != UNIT_OBJECT) return 1;
				TRADE_StashAction(pPlayerUnit,pGame,aPacket->ButtonId,Argument);
				return 0;
			}
			break;
		case TRADE_CLOSE_CUBE:
			{
			if(pPlayerUnit->dwInteractType != UNIT_ITEM) return 3;

			TRADE_RemoveInteraction(pPlayerUnit);
			D2GAME_UpdateQuantity(pPlayerUnit);
			return 0;
			}
			break;
		case TRADE_TRANSMUTE:
			{
			if(pPlayerUnit->dwInteractType != UNIT_ITEM) return 3;
			D2GAME_Transmute(pGame,pPlayerUnit);
			return 0;
			}
			break;
		case TRADE_PERFORM:
			{
			if(pPlayerUnit->dwInteractType != UNIT_PLAYER) 
				{
				TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_TRADE_DONE);
				return 3;
				}
			UnitAny* pTradingUnit = D2GAME_FindUnit(pGame, pPlayerUnit->dwInteractId, UNIT_PLAYER);
			if(!pTradingUnit)
				{
					if(TRADE_ValidateContainers(pGame,pPlayerUnit)) return 0;
					else
					{
					TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_CLOSE_TRADE);
					return 0;
					}
				}

			if(TRADE_ValidateContainers(pGame,pPlayerUnit) || TRADE_ValidateContainers(pGame,pTradingUnit))
				{
				if(pPlayerUnit->pPlayerData->nTradeState == 7) { pPlayerUnit->pPlayerData->nTradeState = 14; return 0; }
				if(pPlayerUnit->pPlayerData->nTradeState == 8) { pPlayerUnit->pPlayerData->nTradeState = 15; return 0; }
				TRADE_CloseTrade(pGame,pPlayerUnit,pTradingUnit,12);
				return 0;
				}
			if(pPlayerUnit->pPlayerData->nTradeState !=2 || pTradingUnit->pPlayerData->nTradeState != 1)
				{
				TRADE_CloseTrade(pGame,pPlayerUnit,pTradingUnit,12);
				return 0;
				}
			TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_OPENTRADE);
			TRADE_UpdateClientUI(pTradingUnit->pPlayerData->pClientData,UPDATE_OPENTRADE);
			pPlayerUnit->pPlayerData->nTradeState = 3;
			pTradingUnit->pPlayerData->nTradeState = 3;
			if( TRADE_AllocTradeStrc(pGame,pPlayerUnit) && TRADE_AllocTradeStrc(pGame,pTradingUnit))
				{
					TRADE_SendTradeAccepted(pPlayerUnit->pPlayerData->pClientData,pTradingUnit->dwUnitId,pTradingUnit->pPlayerData->szName);
					TRADE_SendTradeAccepted(pTradingUnit->pPlayerData->pClientData,pPlayerUnit->dwUnitId,pPlayerUnit->pPlayerData->szName);
					return 0;
				}
			else
				{
					TRADE_CloseTrade(pGame,pPlayerUnit,pTradingUnit,12);
					return 0;
				}

			}
			break;
		case TRADE_CANCEL:
			{
				if(pPlayerUnit->dwInteractType != UNIT_PLAYER) 
				{
				TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_TRADE_DONE);
				Log("TRADE: pPlayerUnit->dwInteractType != UNIT_PLAYER",0);
				return 3;
				}
			UnitAny* pTradingUnit = D2GAME_FindUnit(pGame, pPlayerUnit->dwInteractId, UNIT_PLAYER);
			if(!pTradingUnit)
				{
					if(TRADE_ValidateContainers(pGame,pPlayerUnit)) return 0;
					else
					{
					TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_CLOSE_TRADE);
					return 0;
					}
				}
			
			if((pPlayerUnit->pPlayerData->nTradeState == 2 || pPlayerUnit->pPlayerData->nTradeState == 1) && (pTradingUnit->pPlayerData->nTradeState == 2 || pTradingUnit->pPlayerData->nTradeState == 1))
			{
			TRADE_CloseTrade(pGame,pPlayerUnit,pTradingUnit,9);
			pTradingUnit->pPlayerData->dwAcceptTradeTick = pPlayerUnit->pPlayerData->dwAcceptTradeTick = GetTickCount();
			return 0;
			}
			if(TRADE_ValidateContainers(pGame,pPlayerUnit) || TRADE_ValidateContainers(pGame,pTradingUnit)) return 0;
			TRADE_CloseTrade(pGame,pPlayerUnit,pTradingUnit,9);
			pTradingUnit->pPlayerData->dwAcceptTradeTick = pPlayerUnit->pPlayerData->dwAcceptTradeTick = GetTickCount();
			return 0;
			}
			break;
		case TRADE_SENDGOLD:
			{
				if(pPlayerUnit->dwInteractType != UNIT_PLAYER) 
				{
					TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_TRADE_DONE);
					Log("TRADE: pPlayerUnit->dwInteractType != UNIT_PLAYER",0);
					return 3;
				}
			UnitAny* pTradingUnit = D2GAME_FindUnit(pGame, pPlayerUnit->dwInteractId, UNIT_PLAYER);
			if(!pTradingUnit)
				{
					if(TRADE_ValidateContainers(pGame,pPlayerUnit)) return 0;
					else
					{
						TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_CLOSE_TRADE);
						return 0;
					}
				}
			if(pPlayerUnit->pPlayerData->pTrade->ARGold>=Argument)
				{
					int nSafeGold = TRADE_GetARGoldSaldo(pPlayerUnit->pPlayerData->pClientData->AccountName);
					if(nSafeGold == -1 || pPlayerUnit->pPlayerData->pTrade->ARGold != nSafeGold)
					{
					Log("TRADE: (*%s) : pTrade->ARGold != TRADE_GetARGoldSaldo || TRADE_GetARGoldSaldo == FAIL",pPlayerUnit->pPlayerData->pClientData->AccountName);
					TRADE_CloseTrade(pGame,pPlayerUnit,pTradingUnit,9);
					return 0;
					}
					pPlayerUnit->pPlayerData->pTrade->nOfferedGold = Argument;
					TRADE_SendOfferedGold(pTradingUnit->pPlayerData->pClientData,0,Argument);
					TRADE_SendOfferedGold(pPlayerUnit->pPlayerData->pClientData,1,Argument);
					return 0;
				}
			else
				{
					if(TRADE_ValidateContainers(pGame,pPlayerUnit) || TRADE_ValidateContainers(pGame,pTradingUnit)) return 0;
					TRADE_CloseTrade(pGame,pPlayerUnit,pTradingUnit,9);
					return 0;
				}

			}
		break;
		case TRADE_ACCEPT:
		{
			if(pPlayerUnit->dwInteractType != UNIT_PLAYER) 
				{
					TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_TRADE_DONE);
					Log("TRADE: pPlayerUnit->dwInteractType != UNIT_PLAYER",0);
					return 3;
				}
			UnitAny* pTradingUnit = D2GAME_FindUnit(pGame, pPlayerUnit->dwInteractId, UNIT_PLAYER);
			if(!pTradingUnit)
				{
					if(TRADE_ValidateContainers(pGame,pPlayerUnit)) return 0;
					else
					{
						TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_CLOSE_TRADE);
						return 0;
					}
				}

			if(TRADE_ValidateContainers(pGame,pPlayerUnit) || TRADE_ValidateContainers(pGame,pTradingUnit) || pPlayerUnit->pPlayerData->nTradeState != 3 || D2COMMON_GetCursorItem(pPlayerUnit->pInventory))
				{
				TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_OPENTRADE);
				return 0;
				}

			TRADE_UpdateClientUI(pTradingUnit->pPlayerData->pClientData,UPDATE_ACCEPT_TRADE);

			if(pPlayerUnit->pPlayerData->nTradeState != 3 || (pPlayerUnit->pPlayerData->nTradeState = 4, pTradingUnit->pPlayerData->nTradeState != 4)) return 0;
			if(!TRADE_TransferItems(pGame, pPlayerUnit,pTradingUnit)) TRADE_ResetTrade(pGame,pPlayerUnit,pTradingUnit); 
			pTradingUnit->pPlayerData->dwAcceptTradeTick = pPlayerUnit->pPlayerData->dwAcceptTradeTick = GetTickCount();
			return 0;
		}
		break;
		case TRADE_GREEN_UNCHECK:
		{
			if(pPlayerUnit->dwInteractType != UNIT_PLAYER) 
				{
					TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_TRADE_DONE);
					Log("TRADE: pPlayerUnit->dwInteractType != UNIT_PLAYER",0);
					return 3;
				}
			UnitAny* pTradingUnit = D2GAME_FindUnit(pGame, pPlayerUnit->dwInteractId, UNIT_PLAYER);
			if(!pTradingUnit)
				{
					if(TRADE_ValidateContainers(pGame,pPlayerUnit)) return 0;
					else
					{
						TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_CLOSE_TRADE);
						return 0;
					}
				}
			if(pPlayerUnit->pPlayerData->nTradeState == 4) pPlayerUnit->pPlayerData->nTradeState = 3;
			if(pTradingUnit->pPlayerData->nTradeState == 4) pTradingUnit->pPlayerData->nTradeState = 3;
			TRADE_UpdateClientUI(pPlayerUnit->pPlayerData->pClientData,UPDATE_OPENTRADE);
			TRADE_UpdateClientUI(pTradingUnit->pPlayerData->pClientData,UPDATE_OPENTRADE);
			return 0;
		}
		break;
	}

	return 3;
}