#ifndef LROSTER_H__
#define LROSTER_H__

#include "D2Structs.h"

void __stdcall OnCreateDamage(UnitAny* pDefender, Damage* pDamage, UnitAny* pMissile);
void __stdcall OnDeath(UnitAny* ptKiller, UnitAny * ptVictim, Game * ptGame);
void __fastcall OnGameDestroy(Game* ptGame);
void __fastcall PLAYERMODES_0_Death(Game *pGame, UnitAny *pVictim, int nMode, UnitAny *pKiller);

namespace LRost
{
	void SyncClient(Game *ptGame, ClientData* ptClient);
	void SyncClient(Game *ptGame, DWORD UnitId, LRoster* pRoster);
	LRoster* Find(Game * ptGame, char* szName);
	void UpdateRoster(Game * ptGame,char * szName, BYTE Type);
	void Clear(Game * ptGame);
}

#endif