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
#include "LServer.h"


namespace LServer {
	

	/*
		Patch no 18 <- not needed
		Patch location: D2GAME.0xD53C6
		Original code: call		ParseIncomingPacket_6FC873A0
		New code:	   call		D2GS_OnPacketRecv_STUB
	*/
	/*
		Patch no 19 <- not needed
		Patch location D2GAME.0xBFEEF
		Original code: call		GAME_ParseCreatePackets_6FCF53E0
		New code:      call		D2GS_ParseCreatePackets_STUB
	*/


}