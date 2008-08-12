
/***************************************************************************
    NWNXFuncs.cpp - Implementation of the CNWNXFuncs class.
    Copyright (C) 2007 Doug Swarin (zac@intertex.net)

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

#include "NWNXFuncs.h"


void Func_GetGroundHeight (CGameObject *ob, char *value) {
    Vector vec;
    CNWSArea *area;
    nwn_objid_t areaid;

    if (sscanf(value, "%x�%f�%f�%f", &areaid, &(vec.x), &(vec.y), &(vec.z)) != 4) {
        snprintf(value, strlen(value), "-65535.0");
        return;
    }


    area = CServerExoAppInternal__GetAreaByGameObjectID((*NWN_AppManager)->app_server->srv_internal, areaid);
    if (area == NULL) {
        snprintf(value, strlen(value), "65535.0");
        return;
    }

    snprintf(value, strlen(value), "%f", CNWSArea__ComputeHeight(area, vec));
}


/* vim: set sw=4: */
