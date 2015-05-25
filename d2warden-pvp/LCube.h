/* ==========================================================
* d2warden
* https://github.com/lolet/d2warden
* ==========================================================
* Copyright 2011-2015 Bartosz Jankowski
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

#ifndef LCUBE_H__
#define LCUBE_H__


enum CubeOutputTypes {
	CUBEOUTPUT_USETYPE = -1,
	CUBEOUTPUT_USEITEM = -2,
	CUBEOUTPUT_NORMALITEM = -3,
	CUBEOUTPUT_UNIQUESETITEM = -4,
	CUBEOUTPUT_PORTAL = 1,
	CUBEOUTPUT_UBERQUEST,
	CUBEOUTPUT_UBERQUEST_FINAL,
	CUBEOUTPUT_COWPORTAL,
};


enum CubeMainOpCodes {
	CUBEMAIN_OP_DAYOFMONTH = 1,		 // DayOfMonth is less than Param or greater than value 
	CUBEMAIN_OP_DAYOFWEEK = 2,		 // DayOfWeek != value(1 = Sunday, ...)
	CUBEMAIN_OP_STAT_HIGHER = 3,
	CUBEMAIN_OP_STAT_LOWER = 4, 
	CUBEMAIN_OP_STAT_EQ = 5,	
	CUBEMAIN_OP_STAT_NOT_EQ = 6,
	// ...
};

DWORD __stdcall CUBE_OnCustomFunc(Game* pGame, UnitAny* pPlayer, CubeOutputItem *pCubeOutput);

#endif