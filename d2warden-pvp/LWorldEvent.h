#ifndef D2WE_H__
#define D2WE_H__

#include "D2Structs.h"
#include <sstream>

bool WE_isKey(UnitAny* ptItem);
void WE_UpdateCounter(int Value);
void WE_GenerateNextDC();
void WE_CreateDCKey(UnitAny* pUnit);
void WE_Inform(Game* ptGame);
void WE_Inform2(Game* ptGame);
int __fastcall WE_Spawn(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId);

#endif
