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

volatile uintptr_t Hook_StunFistSuccess_Return;
volatile uintptr_t Hook_StunFistFailure_Return;

volatile CNWSCreature *Hook_Stunner;
volatile CNWSCreature *Hook_Stunnee;

__attribute__((noinline))
int Hook_GetScriptEffectType(CGameEffect *eff)
{
    asm("leave");
    asm("mov $0x0817efb8, %eax");
    asm("jmp *%eax");
}

__attribute__((noinline))
int Hook_IsTargetStunned(CNWSCreature *target)
{
    CNWSObject *obj = &target->obj;
    for (int i = 0; i < obj->obj_effects_len; i++) {
        // 29 is EFFECT_TYPE_STUNNED
        if (Hook_GetScriptEffectType(obj->obj_effects[i]) == 29) {
            return 1;
        }
    }
    return 0;
}

void Hook_StunFistUse(void)
{
    uintptr_t return_address;
    asm("leave");
    /* The creature being stunned is in $sp + 0x34 */
    asm("mov 0x30(%esp),%edx");
    asm("pushl %edx");
    asm("popl Hook_Stunner");
    asm("mov 0x34(%esp),%edx");
    asm("pushl %edx");
    asm("popl Hook_Stunnee");

    if (!Hook_IsTargetStunned((CNWSCreature *)Hook_Stunnee)) {
        /* Duplicate original work */
        asm("sub $0x8,%esp");
        asm("push $0x27");
        asm("mov -0x10(%ebp),%edx");
        asm("pushl 0xc68(%edx)");

        /* Return to decrement feat uses */
        asm("pushl Hook_StunFistSuccess_Return");
        asm("ret");
    } else {
        nwn_ExecuteScript("m_nwnx_stunfail", Hook_Stunner->obj.obj_id);
        /* Return to - maybe - normal melee attack */
        asm("pushl Hook_StunFistFailure_Return");
        asm("ret");
    }

}

/* vim: set sw=4: */
