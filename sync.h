/*******************************************************************************
 * sync.h
 * A module of J-Pilot http://jpilot.org
 * 
 * Copyright (C) 1999-2014 by Judd Montgomery
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ******************************************************************************/

#ifndef _SYNC_H__
#define _SYNC_H__

#include <pi-dlp.h>

/* Bitmasks for backup */
#define SYNC_FULL_BACKUP   0x01
#define SYNC_NO_PLUGINS    0x02
#define SYNC_OVERRIDE_USER 0x04
#define SYNC_NO_FORK       0x08
#define SYNC_RESTORE       0x10
#define SYNC_INSTALL_USER  0x20

#define SYNC_ERROR_BIND            -10
#define SYNC_ERROR_LISTEN          -11
#define SYNC_ERROR_OPEN_CONDUIT    -12
#define SYNC_ERROR_PI_ACCEPT       -13
#define SYNC_ERROR_READSYSINFO     -14
#define SYNC_ERROR_PI_CONNECT      -15
#define SYNC_ERROR_NOT_SAME_USER   -20
#define SYNC_ERROR_NOT_SAME_USERID -21
#define SYNC_ERROR_NULL_USERID     -22

struct my_sync_info {
   unsigned int sync_over_ride;
   char port[128];
   unsigned int flags;
   unsigned int num_backups;

   long userID;
   unsigned long viewerID;
   long PC_ID;
   char username[128];
};

int sync_once(struct my_sync_info *sync_info);
int sync_loop(const char *port, unsigned int flags, const int num_backups);

#endif
