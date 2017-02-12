
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

#ifndef NWNX_DETECT_H
#define NWNX_DETECT_H


#include "NWNXApi.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}

#include "NWNXBase.h"

class CNWNXDetect: public CNWNXBase
{
public:
    CNWNXDetect();
    virtual ~CNWNXDetect() = default;
    bool OnCreate(gline *nwnxConfig, const char *LogDir = NULL);
    char *OnRequest(char *gameObject, char *Request, char *Parameters);
    unsigned long OnRequestObject(char *gameObject, char *Request);
};

#endif

#endif /* NWNX_DETECT_H */

/* vim: set sw=4: */
