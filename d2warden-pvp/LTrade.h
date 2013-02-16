#ifndef D2TRADE_H__
#define D2TRADE_H__

#include "D2Structs.h"

BOOL __fastcall TRADE_OnARGoldPass(Game *pGame, UnitAny *pPlayerUnit, GoldPacket *aPacket, int aLen);
BYTE __fastcall TRADE_AllocTradeStrc(Game* pGame, UnitAny* pUnit);
void TRADE_RemoveInteraction(UnitAny* pUnit);
BOOL __fastcall TRADE_OnTradeAction(Game *pGame, UnitAny *pPlayerUnit, TradePacket *aPacket, int aLen);


enum UpdateClient
{
	UPDATE_ASK_TO_TRADE = 0,
	UPDATE_ASKED_TO_TRADE,
	UPDATE_ACCEPT_TRADE = 5,
	UPDATE_OPENTRADE = 6,
	UPDATE_SELLER_NOROOM = 9,
	UPDATE_BUYER_NOROOM = 0xA,
	UPDATE_CLOSE_TRADE = 0x0C,
	UPDATE_TRADE_DONE = 0xD,
	UPADTE_RED_CHECKBOX = 0xE,
	UPDATE_NORM_CHECKBOX = 0xF,
	UPADTE_OPEN_STASH = 0x10,
	UPDATE_OPEN_CUBE = 0x15
};

enum TradeButtons
{
	TRADE_CANCEL = 2,				//ok
	TRADE_PERFORM = 3,				//ok
	TRADE_ACCEPT = 4,			
	TRADE_GREEN_UNCHECK = 7,		//ok
	TRADE_SENDGOLD,					//ok	
	TRADE_CLOSE_STASH = 0x12,		//ok
	TRADE_STASH_WITHDRAW,			//ok
	TRADE_STASH_DEPOSIT,			//ok
	TRADE_CLOSE_CUBE = 0x17,		//ok
	TRADE_TRANSMUTE					//ok
};

enum TradeStates
{	
	TRADE_STATE_OPENED = 3,
	TRADE_STATE_GREEN_PRESSED = 4,
	TRADE_RESETED = 5,
	TRADE_STATE_TRADE_DONE  = 9,
	TRADE_STATE_FAIL	= 12
};

#endif