#include "stdafx.h"

#include "Votekick.h"
#include "D2Ptrs.h"
#include "D2Warden.h"
#include "WardenMisc.h"
#include "D2Structs.h"
#include "Vars.h"
//
//DWORD WINAPI VoteThread(void* Params)
//{
//if(!sVote) return 0;
//WardenClient * pUnit = sVote->pVoter;
//if(!pUnit) return 0;
//BroadcastMsg(pUnit->ptClientData->pGame,"Votekick for ÿc9%sÿce type #yes/#no (15 seconds)",pUnit->CharName);
//while(GetTickCount()<sVote->Time+15000)
//{
//if(sVote->Votes==pUnit->ptClientData->pGame->nClients)
//{
//	if((sVote->Score*100)/sVote->Votes<=50) break;
//	else
//	{
//	BroadcastMsg(pUnit->ptClientData->pGame,"Votekick passed (%d%%)",(sVote->Score*100)/sVote->Votes);
//	KickPlayer(pUnit->ptClientData);
//	delete sVote;
//	sVote=0;
//	return 0;
//	}
//}
//Sleep(100);
//}
//if((sVote->Score*100)/sVote->Votes>50)
//{
//	BroadcastMsg(pUnit->ptClientData->pGame,"Votekick passed (%d%%)",(sVote->Score*100)/sVote->Votes);
//	KickPlayer(pUnit->ptClientData);
//	delete sVote;
//	sVote=0;
//	return 0;
//}
//else
//{
//BroadcastMsg(pUnit->ptClientData->pGame,"Votekick failed (%d%%)",(sVote->Score*100)/sVote->Votes);
//delete sVote;
//sVote=0;
//return 0;
//}
//}