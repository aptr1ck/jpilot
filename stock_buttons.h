/* $Id: stock_buttons.h,v 1.1 2005/10/16 09:42:25 rousseau Exp $ */

/*******************************************************************************
 * stock_buttons.h
 * A module of J-Pilot http://jpilot.org
 *
 * Copyright (C) 2005 by Ludovic Rosuseau
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

#include "config.h"

extern GtkTooltips *glob_tooltips;

#ifdef DISABLE_STOCK_BUTTONS

/* old behavior */
#include "gdk/gdkkeysyms.h"

#define CREATE_BUTTON(widget, text, stock, tooltip, shortcut_key, shortcut_mask, shortcut_text) \
   widget = gtk_button_new_with_label(text); \
   if (shortcut_key) \
   { \
	  char str[100]; \
      gtk_widget_add_accelerator(widget, "clicked", accel_group, shortcut_key, shortcut_mask, GTK_ACCEL_VISIBLE); \
	  sprintf(str, "%s   %s", tooltip, shortcut_text); \
      gtk_tooltips_set_tip(glob_tooltips, widget, str, NULL); \
   } \
   else \
      gtk_tooltips_set_tip(glob_tooltips, widget, tooltip, NULL);\
   gtk_box_pack_start(GTK_BOX(hbox_temp), widget, TRUE, TRUE, 0);
#else

/* GTK+ stock buttons only if GTK2 is used */
#ifdef ENABLE_GTK2

#define CREATE_BUTTON(widget, text, stock, tooltip, shortcut_key, shortcut_mask, shortcut_text) \
   widget = gtk_button_new_from_stock(GTK_STOCK_ ## stock); \
   gtk_tooltips_set_tip(glob_tooltips, widget, tooltip, NULL); \
   gtk_box_pack_start(GTK_BOX(hbox_temp), widget, TRUE, TRUE, 0);

#else
#error "ENABLE_STOCK_BUTTONS can't be used without ENABLE_GTK2"
#endif

#endif

