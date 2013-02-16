#ifndef D2NODS_H__
#define D2NODS_H__



#include "D2Structs.h"

void __fastcall NODES_Free(Game *ptGame);
void __fastcall NODES_AddAsParentNode(Game *ptGame, UnitAny *ptUnit, UnitNode *ptNode, int NodeIdx);
void __fastcall NODES_SetUnitNode(Game *ptGame, UnitAny *ptUnit, UnitNode* ptNode, int NodeIdx);
UnitNode *__fastcall NODES_GetUnitNode(UnitAny *ptUnit, Game *ptGame);
void __fastcall NODES_AssignNode(Game *ptGame, UnitAny *ptUnit, UnitNode* pChild);
void __fastcall NODES_FreeChildNode(Game *ptGame, UnitAny *ptUnit);
void __fastcall NODES_FreeUnitNode(Game* ptGame, UnitAny* ptUnit);
UnitAny *__stdcall NODES_BaalCheck(Game *ptGame, UnitAny *ptUnit, int *xPos, int *yPos, int aZero, int (__fastcall *pfnCallback)(UnitAny*, UnitAny*));  //0x6FC4A790
UnitAny *__stdcall NODES_NormalCheck(Game *ptGame, UnitAny *ptUnit, int *xPos, int *yPos, int aZero, int (__fastcall *pfnCallback)(UnitAny*, UnitAny*));  //0x6FC4A790


#endif