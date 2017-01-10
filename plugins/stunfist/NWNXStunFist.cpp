
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

const uintptr_t kStunMakeSave = 0x080ea513;
const uintptr_t kStunStart = 0x080ea2ec;

volatile CNWSCreature *stun_hook_stunner;
volatile CNWSCreature *stun_hook_stunnee;

volatile uintptr_t happy_return = 0x80ea2fa;
volatile uintptr_t unhappy_return = 0x80e9b29;

#ifdef __cplusplus
extern "C" {
#endif

extern void nwn_ExecuteScript(char *s, unsigned long id);

#ifdef __cplusplus
}
#endif

bool IsStunned(CNWSCreature *stunnee)
{
    for (unsigned i = 0; i < stunnee->EffectsLength; i++) {
        /* EFFECT_TYPE_STUNNED = 29 */
        if (stunnee->Effects[i]->GetScriptEffectType() == 29) {
            return true;
        }
    }
    return false;
}

void StunFistUsageOverride(void)
{
    asm("leave");
    /* The creature stunning is in $sp + 0x30 */
    asm("mov 0x30(%esp),%edx");
    asm("pushl %edx");
    asm("popl stun_hook_stunner");
    /* The creature being stunned is in $sp + 0x34 */
    asm("mov 0x34(%esp),%edx");
    asm("pushl %edx");
    asm("popl stun_hook_stunnee");

    if (!IsStunned((CNWSCreature*)stun_hook_stunnee)) {
        /* Duplicate original work */
        asm("sub $0x8,%esp");
        asm("push $0x27");
        asm("mov -0x10(%ebp),%edx");
        asm("pushl 0xc68(%edx)");

        /* Return to decrement feat uses */
        asm("pushl happy_return");
        asm("ret");
    } else {
        nwn_ExecuteScript("m_nwnx_stunfail", stun_hook_stunner->ObjectID);
        /* Return to normal melee attack */
        asm("pushl unhappy_return");
        asm("ret");
    }
}

unsigned char SavingThrowRollOverride(CNWSCreature *stunnee,
                                      unsigned char type, unsigned short dc,
                                      unsigned char savetype, unsigned long stunner_id,
                                      int print, unsigned short feat, int queuefeedback)
{
    CNWSCreature *stunner = g_pAppManager->ServerExoApp->GetCreatureByGameObjectID(stunner_id);
    if (!stunner) {
        return stunnee->SavingThrowRoll(type, dc, savetype, stunner_id, print, feat, queuefeedback);
    }
    CNWSCreatureStats *stats = stunner->GetStats();
    int wis = (stats->GetWISStat() - 10) / 2;
    int hd = stats->GetLevel(true);
    int feats = 10;
    while (feats) {
      if (stats->HasFeat(FEAT_EPIC_IMPROVED_STUNNING_FIST_1 + feats - 1)) break;
      feats--;
    }
    return stunnee->SavingThrowRoll(type, 10+wis+hd/2+feats, savetype, stunner_id, print, feat, queuefeedback);
}

CNWNXStunFist::CNWNXStunFist()
{
    confKey = strdup("STUNFIST");
}

char *CNWNXStunFist::OnRequest(char *gameObject, char *Request, char *Parameters)
{
    return NULL;
}

unsigned long CNWNXStunFist::OnRequestObject(char *gameObject, char *Request)
{
    return 0;
}

bool CNWNXStunFist::OnCreate(gline *config, const char *LogDir)
{
    char log[128];

    sprintf(log, "%s/nwnx_stunfist.txt", LogDir);

    /* call the base class create function */
    if (!CNWNXBase::OnCreate(config, log))
        return false;

    /* stun start hook */
    nx_hook_function((void*)kStunStart, (void*)StunFistUsageOverride, 5, NX_HOOK_DIRECT | NX_HOOK_RETCODE);

    /* stun DC hook */
    NX_HOOK_CALL(kStunMakeSave, SavingThrowRollOverride);

    return true;
}

/* vim: set sw=4: */
