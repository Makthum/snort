/* $Id$ */
/*
 ** Copyright (C) 2011-2012 Sourcefire, Inc.
 **
 ** This program is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License Version 2 as
 ** published by the Free Software Foundation.  You may not use, modify or
 ** distribute this program under any other version of the GNU General
 ** Public License.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
/**
 * @file   idle_processing.c
 * @author Ron Dempster <rdempster@sourcefire.com>
 * @date   Tue Jun 17 17:09:59 2003
 *
 * @brief  Allow functions to be registered to be called when packet
 *         processing is idle.
 *
 */

#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "idle_processing_funcs.h"
#include "util.h"

typedef struct _IDLE_HANDLER_ELEMENT
{
    struct _IDLE_HANDLER_ELEMENT *next;
    IdleProcessingHandler handler;
} IdleHandlerElement;

static IdleHandlerElement *idle_handlers;

int IdleProcessingRegisterHandler(IdleProcessingHandler func)
{
    IdleHandlerElement *e;

    if ((e = calloc(1, sizeof(*e))) == NULL)
    {
        WarningMessage("%s\n", "Failed to allocate an idle handler element");
        return -1;
    }
    e->handler = func;
    e->next = idle_handlers;
    idle_handlers = e;
    return 0;
}

void IdleProcessingExecute(void)
{
    IdleHandlerElement *e;

    for (e = idle_handlers; e; e = e->next)
        e->handler();
}

void IdleProcessingCleanUp(void)
{
    IdleHandlerElement *e;

    while ((e = idle_handlers))
    {
        idle_handlers = e->next;
        free(e);
    }
}

