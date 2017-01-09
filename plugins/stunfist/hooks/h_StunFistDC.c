
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

volatile uintptr_t Hook_StunFistMakeSave_Return;
volatile CNWSCreatureStats *Hook_Stunner;

__attribute__((noinline))
int Hook_GetStunFistDC(CNWSCreatureStats *stunner)
{
    int wis = stunner->cs_wis_mod;
    int hd = 0;
    for (int i = 0; i < 3; i++) {
        hd += stunner->cs_classes[i].cl_level;
    }
    int feats = 10;
    while (feats) {
      if (CNWSCreatureStats__HasFeat(stunner, FEAT_EPIC_IMPROVED_STUNNING_FIST_1 + feats - 1)) break;
      feats--;
    }
    return 10 + hd / 2 + feats + wis;
}

void HookStunFistDC(void)
{
    asm("leave");
    /* The creature stunning is in %esp + 0x28 */
    asm("mov 0x28(%esp),%edx");
    asm("pushl 0xc68(%edx)");
    asm("popl Hook_Stunner");

    /* the result is in %eax*/
    Hook_GetStunFistDC((CNWSCreatureStats *)Hook_Stunner);

    /* Duplicate original work */
    asm("push   $0x1");
    asm("push   $0x0");
    asm("push   $0x1");
    asm("mov    -0x10(%ebp), %ecx");
    asm("pushl  0x4(%ecx)");
    asm("push   $0x0");
    // Result already in %eax, so skip this.
    // asm("movzwl %di,%eax");
    asm("push   %eax");
    asm("push   $0x1");
    asm("pushl  -0x1c(%ebp)");

    asm("pushl Hook_StunFistMakeSave_Return");
    asm("ret");
}

/* vim: set sw=4: */
