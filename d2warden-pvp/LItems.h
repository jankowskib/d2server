#ifndef D2LITEMS_H__
#define D2LITEMS_H__

#include <cstdio>
#include "D2Structs.h"

UnitAny* __stdcall ITEMS_AddKillerId(Game *pGame, CreateItem *srCreateItem, int a5);
void CreateFFAItems(UnitAny* pUnit);
bool ParseItemsCmds(UnitAny* pUnit, char* str, char *t);

#endif