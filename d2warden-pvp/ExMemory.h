/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2013 lolet
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

#ifndef __EXMEMORY_H__
#define __EXMEMORY_H__
#include <cstdio>

using namespace std;

namespace ExMemory
{
	int GetPoolsCount();
	int GetMemUsage();
	void Release();
	void LogLeaks();

	void* __fastcall Alloc_STUB(int MemSize, int* MemPool, char* szFile, int Line);
	void* __stdcall Realloc_STUB(int NewSize);
	void __fastcall Free_STUB(int* Pool, int* MemPool, char* szFile, int Line);

	void* __fastcall Alloc(int MemSize, char* szFile, int Line, int aNull);
	void* __stdcall Realloc(void* MemPool, void *pMem, int MemSize, char* szFile, int Line);
	void __fastcall  Free(void* MemPool, char* szFile, int Line, int aNull);
	
//	void* __fastcall AllocPool(void* pMemPool, int MemSize, char* szFile, int Line, int aNull);
//	void __fastcall  FreePool (void* pMemPool, void* Memory, char* szFile, int Line, int aNull);
}

#endif