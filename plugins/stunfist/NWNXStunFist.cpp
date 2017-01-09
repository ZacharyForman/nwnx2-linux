
/***************************************************************************
    NWNXStunFist.cpp - Implementation of the CNWNXStunFist class.

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

#include "NWNXStunFist.h"

//////////////////////////////////////////////////////////////////////
// Function Signatures
//////////////////////////////////////////////////////////////////////

#define NWNX_STUNFIST_SIG(NAME, SIG) { #NAME, &NAME, SIG }

static unsigned char *Ref_StunFistMakeSave;

static struct StunFistSignatureTable {
    const char         *name;
    void               *ref;
    const char         *sig;
} Table_StunFistSignatures[] = {
    { NULL, NULL,                               NULL },

    NWNX_STUNFIST_SIG(Ref_StunFistMakeSave,     "6a 01 6a 00 6a 01 8b 4d f0 ff 71 04 6a 00 0f b7 c7 50 6a 01 ff 75 e4"),

    { NULL, NULL,                               NULL },
};


static void StunFistSearchCallback(int id, void *addr)
{
    nx_log(NX_LOG_NOTICE, 0, "%s (%d) found at %p%s",
           Table_StunFistSignatures[id].name, id, addr,
           (*(void **)Table_StunFistSignatures[id].ref == NULL ? "" : " (duplicate)"));

    *(void **)(Table_StunFistSignatures[id].ref) = addr;
}

static void StunFistSearchSignatures(void)
{
    int i;

    nx_sig_search_t *sig = nx_sig_search_create(StunFistSearchCallback);

    for (i = 1; Table_StunFistSignatures[i].sig != NULL; i++)
        nx_sig_search_add_signature(sig, i, Table_StunFistSignatures[i].sig);

    nx_sig_search_run(sig);
    nx_sig_search_destroy(sig);


    for (i = 1; Table_StunFistSignatures[i].sig != NULL; i++) {
        if (*(void **)Table_StunFistSignatures[i].ref == NULL)
            nx_log(NX_LOG_NOTICE, 0, "%s (%d) not found", Table_StunFistSignatures[i].name, i);
    }
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNWNXStunFist::CNWNXStunFist()
{
    confKey = strdup("STUNFIST");
}


CNWNXStunFist::~CNWNXStunFist()
{
}


char *CNWNXStunFist::OnRequest(char *gameObject, char *Request, char *Parameters)
{
    // No requests.
    return NULL;
}


unsigned long CNWNXStunFist::OnRequestObject(char *gameObject, char *Request)
{
    // No requests.
    return 0;
}


bool CNWNXStunFist::OnCreate(gline *config, const char *LogDir)
{
    char log[128];

    sprintf(log, "%s/nwnx_stunfist.txt", LogDir);

    /* call the base class create function */
    if (!CNWNXBase::OnCreate(config, log))
        return false;

    /* find hook signatures */
    StunFistSearchSignatures();

    /* critical confirmation roll hook */
    if (Ref_StunFistMakeSave != NULL) {
        extern volatile uintptr_t Hook_StunFistMakeSave_Return;
        nx_hook_function(Ref_StunFistMakeSave, (void *)HookStunFistDC, 5, NX_HOOK_DIRECT | NX_HOOK_RETCODE);
        Hook_StunFistMakeSave_Return = (uintptr_t) Ref_StunFistMakeSave + 23;
    }

    return true;
}

/* vim: set sw=4: */
