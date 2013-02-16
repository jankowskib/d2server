#ifndef LEVENTS_H__
#define LEVENTS_H__

#include "D2Structs.h"

void __stdcall OnBroadcastEvent(Game* pGame, EventPacket * pEvent);
void DoRoundEndStuff(Game* pGame, UnitAny* pUnit);
#endif