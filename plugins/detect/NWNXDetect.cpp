
/***************************************************************************
    NWNXDetect.cpp - Implementation of the NWNXDetect class.

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

#include "NWNXDetect.h"
#include <cstring>

const uintptr_t kSpotRoll = 0x8126853;
const uintptr_t kListenRoll = 0x8126453;
const uintptr_t kTwoByteNop = 0x6690;

CNWNXDetect::CNWNXDetect()
{
    confKey = strdup("STUNFIST");
}

char *CNWNXDetect::OnRequest(char *gameObject, char *Request, char *Parameters)
{
    return NULL;
}

unsigned long CNWNXDetect::OnRequestObject(char *gameObject, char *Request)
{
    return 0;
}

bool CNWNXDetect::OnCreate(gline *config, const char *LogDir)
{
    char log[128];

    sprintf(log, "%s/nwnx_stunfist.txt", LogDir);

    /* call the base class create function */
    if (!CNWNXBase::OnCreate(config, log))
        return false;

    nx_hook_enable_write((void*) kSpotRoll, 2);
    nx_hook_enable_write((void*) kListenRoll, 2);
    memset((void*) kSpotRoll, kTwoByteNop, 2);
    memset((void*) kListenRoll, kTwoByteNop, 2);

    return true;
}

/* vim: set sw=4: */
