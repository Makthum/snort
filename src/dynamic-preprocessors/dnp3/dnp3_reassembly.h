/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation.  You may not use, modify or
 * distribute this program under any other version of the GNU General
 * Public License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) 2011-2012 Sourcefire, Inc.
 *
 * Author: Ryan Jordan
 *
 * Dynamic preprocessor for the DNP3 protocol
 *
 */

#ifndef DNP3_REASSEMBLY__H
#define DNP3_REASSEMBLY__H

#include "sf_types.h"
#include "sf_snort_packet.h"
#include "spp_dnp3.h"


int DNP3FullReassembly(dnp3_config_t *config, dnp3_session_data_t *session, SFSnortPacket *packet, uint8_t *pdu_start, uint16_t pdu_length);

#endif /* DNP3_REASSEMBLY__H */
