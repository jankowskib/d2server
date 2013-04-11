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
#define _ENGLISH_LOGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace std;

#include <string>
#include <vector>
#include <sstream>

#include <list>
#include <map>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/random.hpp>
#include <shellapi.h>
#include <urlmon.h>
#include <math.h>

#include "D2Structs.h"
#include "D2Stubs.h"
#include "Vars.h"

#include "WardenMisc.h"
#include "D2Ptrs.h"
#include "Offset.h"

#include "RC4.h"

