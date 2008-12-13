/***************************************************************************
    Events plugin for NWNX  - hooks implementation
    (c) 2006 virusman (virusman@virusman.ru)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***************************************************************************/

#include <sys/types.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <stdarg.h>

#include <limits.h>		/* for PAGESIZE */
#ifndef PAGESIZE
#define PAGESIZE 4096
#endif

#include "HookFunc.h"
#include "NWNXAreas.h"
#include "AssemblyHelper.cpp"

extern CNWNXAreas areas;
AssemblyHelper asmhelp;


void (*CNWSArea__CNWSArea)(void *pArea, CResRef res, int a3, dword ObjID);
void (*CNWSArea__LoadArea)(void *pArea, int flag);
void (*CExoArrayList__Add)(void *pArray, dword nObjID);
void (*CExoArrayList__Remove)(void *pArray, dword nObjID);
void (*CNWSArea__Destructor)(void *pArea, int flag = 3);
void *(*GetAreaByGameObjectID)(void *pServerExoAppInternal, dword nObjID);


dword pServThis = 0; //g_pAppManager
dword pScriptThis = 0;
dword pServInternal = 0;

CResRef *CResRef____as(CResRef *res, char *str)
{
	memset(res->ResRef, 0, 0x10);
	strncpy(res->ResRef, str, 0x10);
	return res;
}

void NWNXCreateArea(void *pModule, char *sResRef)
{
	CResRef res;
	CResRef____as(&res, sResRef);
	void *pArea = malloc(0x210);
	areas.Log(0, "Creating area '%s'\n", sResRef);
	CNWSArea__CNWSArea(pArea, res, 0, OBJECT_INVALID);
	areas.Log(0, "Loading area '%s'\n", sResRef);
	CNWSArea__LoadArea(pArea, 0);
	dword nAreaID = *((dword *)pArea+0x32);
	areas.Log(0, "AreaID=%08lX\n", nAreaID);
	void *pArray = ((dword *)pModule+0x7);
	CExoArrayList__Add(pArray, nAreaID);
	areas.nLastAreaID = nAreaID;
}

void NWNXDestroyArea(void *pModule, dword nAreaID)
{
	if(!nAreaID || nAreaID == OBJECT_INVALID)
		return;
	if(!pServInternal)
		InitConstants();
	areas.Log(0, "Unregistering area %08lX\n", nAreaID);
	void *pArray = ((dword *)pModule+0x7);
	CExoArrayList__Remove(pArray, nAreaID);
	areas.Log(0, "Destroying area %08lX\n", nAreaID);
	void *pArea = GetAreaByGameObjectID((void *)pServInternal, nAreaID);
	CNWSArea__Destructor(pArea);
}

int HookFunctions()
{
	pServThis = 0x0832F1F4;
	pScriptThis = pServThis - 8;

	*(dword*)&CNWSArea__CNWSArea = 0x080CBD30;
	*(dword*)&CNWSArea__LoadArea = 0x080CDFDC;
	*(dword*)&CExoArrayList__Add = 0x0805EEE0;
	*(dword*)&CNWSArea__Destructor = 0x080CC244;
	*(dword*)&GetAreaByGameObjectID = 0x080B0484;
	*(dword*)&CExoArrayList__Remove = 0x0805EE88;

    areas.Log(0, "pServThis = %08lX\n", pServThis);

	return (pServThis && pScriptThis);
}

void InitConstants()
{
	pServInternal = *(*(*(dword***)pServThis + 1) + 1);
}
