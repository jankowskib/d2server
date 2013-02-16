#ifndef __LQUESTS_H__
#define __LQUESTS_H__


#include <cstdio>

#include "D2Structs.h"


int __fastcall DYES_Prepare(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId);
int __fastcall DYES_Colorize(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId);
BYTE * __stdcall DYES_DrawItem(UnitAny *ptPlayer, UnitAny* ptItem, BYTE* out, BOOL a4);

void QUESTS_UpdateUnit(UnitAny* pUnit, WORD UpdateType, UnitAny*pUpdateUnit);
BOOL __stdcall QUESTS_CowLevelOpenPortal(Game *pGame, UnitAny *pUnit);
bool __stdcall QUESTS_OnUseItem(Game* pGame, UnitAny* pUnit, UnitAny* pItem, DWORD dwItemCode);

#endif