/* search_gui.c
 *
 * Copyright (C) 1999 by Judd Montgomery
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"
#include "prefs.h"
#include "log.h"
#include "datebook.h"
#include "address.h"
#include "todo.h"
#include "memo.h"
#include <pi-datebook.h>
#include <pi-address.h>
#include <pi-todo.h>
#include <pi-memo.h>

static AnyRecordList *search_rl = NULL;
static GtkWidget *case_sense_checkbox;


const char *jpilot_strstr(const char *haystack, const char *needle, int case_sense)
{
   char *needle2;
   char *haystack2;
   register char *Ps2;
   register const char *Ps1;
   char *r;
   
   if (case_sense) {
      return strstr(haystack, needle);
   } else {
      if (!haystack) {
	 return NULL;
      }
      if (!needle) {
	 return haystack;
      }
      //I'm going to take the whimpy way out of this one for now
      needle2 = malloc(strlen(needle)+2);
      haystack2 = malloc(strlen(haystack)+2);
      
      Ps1 = needle;
      Ps2 = needle2;
      while (Ps1[0]) {
	 Ps2[0] = tolower(Ps1[0]);
	 Ps1++;
	 Ps2++;
      }
      Ps2[0]='\0';
      
      Ps1 = haystack;
      Ps2 = haystack2;
      while (Ps1[0]) {
	 Ps2[0] = tolower(Ps1[0]);
	 Ps1++;
	 Ps2++;
      }
      Ps2[0]='\0';

      r = strstr(haystack2, needle2);
      if (r) {
	 r = (char *)((r-haystack2)+haystack);
      }
      free(needle2);
      free(haystack2);
      return r;
   }
}

      
static int
  search_datebook(char *needle, GtkWidget *clist)
{
   gchar *empty_line[] = { "","" };
   AppointmentList *a_list;
   AppointmentList *temp_al;
   int found, count;
   char str[200];
   char date_str[50];
   char datef[50];
   const char *svalue1;
   int ivalue;
   AnyRecordList *new_arl;
   
   //Search Appointments
   a_list = NULL;
   
   get_days_appointments(&a_list, NULL);

   if (a_list==NULL) {
      return 0;
   }

   count = 0;
   for (temp_al = a_list; temp_al; temp_al=temp_al->next) {
      if (temp_al->ma.rt == MODIFIED_PALM_REC) {
	 //todo - this will be in preferences as to whether you want to 
	 //see deleted records, or not.
	 continue;
      }
      found = 0;
      if ( (temp_al->ma.a.description) &&
	  (temp_al->ma.a.description[0]) ) {
	 if ( jpilot_strstr(temp_al->ma.a.description, needle,
	      GTK_TOGGLE_BUTTON(case_sense_checkbox)->active) ) {
	    found = 1;
	 }
      }
      if ( (temp_al->ma.a.note) &&
	  (temp_al->ma.a.note[0]) ) {
	 if ( jpilot_strstr(temp_al->ma.a.note, needle,
	      GTK_TOGGLE_BUTTON(case_sense_checkbox)->active) ) {
	    found = 2;
	 }
      }
      if (found) {
	 gtk_clist_prepend(GTK_CLIST(clist), empty_line);
	 gtk_clist_set_text(GTK_CLIST(clist), 0, 0, "datebook");

	 //Add to the search list
	 new_arl = malloc(sizeof(AnyRecordList));
	 new_arl->app_type = DATEBOOK;
	 memcpy(&(new_arl->any.mappo), &temp_al->ma, sizeof(MyAppointment));
	 //This is to prevent appointment from being freed later
	 memset(&(temp_al->ma.a), 0, sizeof(struct Appointment));
	 new_arl->next = search_rl;
	 search_rl = new_arl;
/*
 //Add to the search list
	 new_al = (AppointmentList *)malloc(sizeof(AppointmentList));
	 memcpy(new_al, temp_al, sizeof(AppointmentList));
	 //This is to prevent appointment from being freed later
	 memset(&(temp_al->ma.a), 0, sizeof(struct Appointment));
	 new_al->next = (AppointmentList *)search_rl;
	 search_rl = (AnyRecordList *)new_al;
*/	 
	 gtk_clist_set_row_data(GTK_CLIST(clist), 0, new_arl);
	 count++;

	 //get the date
	 get_pref(PREF_SHORTDATE, &ivalue, &svalue1);
	 if (svalue1 == NULL) {
	    strcpy(datef, "%x");
	 } else {
	    strncpy(datef, svalue1, 50);
	 }
	 strftime(date_str, 30, datef, &new_arl->any.mappo.a.begin);
	    
	 if (found == 1) {
	    g_snprintf(str, 200, "%s  %s",
//		       new_arl->any.mappo.a.begin.tm_mon + 1,
//		       new_arl->any.mappo.a.begin.tm_mday,
//		       new_arl->any.mappo.a.begin.tm_year + 1900,
		       date_str,
		       new_arl->any.mappo.a.description);
	    str[199] = '\0';
	    gtk_clist_set_text(GTK_CLIST(clist), 0, 1, str);
	 }
	 if (found == 2) {
	    g_snprintf(str, 200, "%s %s",
//		       new_arl->any.mappo.a.begin.tm_mon + 1,
//		       new_arl->any.mappo.a.begin.tm_mday,
//		       new_arl->any.mappo.a.begin.tm_year + 1900,
		       date_str,
		       new_arl->any.mappo.a.note);
	    str[199] = '\0';
	    gtk_clist_set_text(GTK_CLIST(clist), 0, 1, str);
	 }
      }
   }
   jpilot_logf(LOG_DEBUG, "calling free_AppointmentList\n");
   free_AppointmentList(&a_list);
   a_list = NULL;
   return count;
}

static int
  search_address(char *needle, GtkWidget *clist)
{
   gchar *empty_line[] = { "","" };
   AddressList *a_list;
   AddressList *temp_al;
   AnyRecordList *new_arl;
   int i, count;
   
   //Search Addresses
   a_list = NULL;

   get_addresses(&a_list);

   if (a_list==NULL) {
      return 0;
   }

   count = 0;
   for (temp_al = a_list; temp_al; temp_al=temp_al->next) {
      if (temp_al->ma.rt == MODIFIED_PALM_REC) {
	 //todo - this will be in preferences as to whether you want to 
	 //see deleted records, or not.
	 continue;
      }
      for (i=0; i<19; i++) {
	 if (temp_al->ma.a.entry[i]) {
	    if ( jpilot_strstr(temp_al->ma.a.entry[i], needle,
			       GTK_TOGGLE_BUTTON(case_sense_checkbox)->active) ) {
	       gtk_clist_prepend(GTK_CLIST(clist), empty_line);
	       gtk_clist_set_text(GTK_CLIST(clist), 0, 0, "address");
	       gtk_clist_set_text(GTK_CLIST(clist), 0, 1, temp_al->ma.a.entry[i]);

	       //Add to the search list
	       new_arl = malloc(sizeof(AnyRecordList));
	       new_arl->app_type = ADDRESS;
	       memcpy(&(new_arl->any.maddr), &temp_al->ma, sizeof(MyAddress));
	       //This is to prevent appointment from being freed later
	       memset(&(temp_al->ma.a), 0, sizeof(struct Address));
	       new_arl->next = search_rl;
	       search_rl = new_arl;
	       
	       gtk_clist_set_row_data(GTK_CLIST(clist), 0, new_arl);
	       count++;

	       break;
	    }
	 }
      }
   }
   jpilot_logf(LOG_DEBUG, "calling free_AddressList\n");
   free_AddressList(&a_list);
   a_list = NULL;
   return count;
}

static int
  search_todo(char *needle, GtkWidget *clist)
{
   gchar *empty_line[] = { "","" };
   ToDoList *todo_list;
   ToDoList *temp_todo;
   AnyRecordList *new_arl;
   int found, count;
   
   //Search Appointments
   todo_list = NULL;
   
   get_todos(&todo_list);

   if (todo_list==NULL) {
      return 0;
   }

   count = 0;
   for (temp_todo = todo_list; temp_todo; temp_todo=temp_todo->next) {
      if (temp_todo->mtodo.rt == MODIFIED_PALM_REC) {
	 //todo - this will be in preferences as to whether you want to 
	 //see deleted records, or not.
	 continue;
      }
      found = 0;
      if ( (temp_todo->mtodo.todo.description) &&
	  (temp_todo->mtodo.todo.description[0]) ) {
	 if ( jpilot_strstr(temp_todo->mtodo.todo.description, needle,
			    GTK_TOGGLE_BUTTON(case_sense_checkbox)->active) ) {
	    found = 1;
	 }
      }
      if ( (temp_todo->mtodo.todo.note) &&
	  (temp_todo->mtodo.todo.note[0]) ) {
	 if ( jpilot_strstr(temp_todo->mtodo.todo.note, needle,
			    GTK_TOGGLE_BUTTON(case_sense_checkbox)->active) ) {
	    found = 2;
	 }
      }
      if (found) {
	 gtk_clist_prepend(GTK_CLIST(clist), empty_line);
	 gtk_clist_set_text(GTK_CLIST(clist), 0, 0, "ToDo");
	 
	 //Add to the search list
	 new_arl = malloc(sizeof(AnyRecordList));
	 new_arl->app_type = TODO;
	 memcpy(&(new_arl->any.mtodo), &temp_todo->mtodo, sizeof(MyToDo));
	 //This is to prevent appointment from being freed later
	 memset(&(temp_todo->mtodo.todo), 0, sizeof(struct ToDo));
	 new_arl->next = search_rl;
	 search_rl = new_arl;

	 gtk_clist_set_row_data(GTK_CLIST(clist), 0, new_arl);
	 count++;

	 if (found == 1) {
	    gtk_clist_set_text(GTK_CLIST(clist), 0, 1,
			       new_arl->any.mtodo.todo.description);
	 }
	 if (found == 2) {
	    gtk_clist_set_text(GTK_CLIST(clist), 0, 1,
			       new_arl->any.mtodo.todo.note);
	 }
      }
   }
   jpilot_logf(LOG_DEBUG, "calling free_ToDoList\n");
   free_ToDoList(&todo_list);
   todo_list = NULL;
   return count;
}

static int
  search_memo(char *needle, GtkWidget *clist)
{
   gchar *empty_line[] = { "","" };
   MemoList *memo_list;
   MemoList *temp_memo;
   AnyRecordList *new_arl;
   int count;
   
   //Search Memos
   memo_list = NULL;

   get_memos(&memo_list);

   if (memo_list==NULL) {
      return 0;
   }

   count = 0;
   for (temp_memo = memo_list; temp_memo; temp_memo=temp_memo->next) {
      if (temp_memo->mmemo.rt == MODIFIED_PALM_REC) {
	 //todo - this will be in preferences as to whether you want to 
	 //see deleted records, or not.
	 continue;
      }
      if (jpilot_strstr(temp_memo->mmemo.memo.text, needle,
		 GTK_TOGGLE_BUTTON(case_sense_checkbox)->active) ) {
	 gtk_clist_prepend(GTK_CLIST(clist), empty_line);
	 gtk_clist_set_text(GTK_CLIST(clist), 0, 0, "memo");
	 if (temp_memo->mmemo.memo.text) {
	    gtk_clist_set_text(GTK_CLIST(clist), 0, 1, temp_memo->mmemo.memo.text);
	 }

	 //Add to the search list
	 new_arl = malloc(sizeof(AnyRecordList));
	 new_arl->app_type = MEMO;
	 memcpy(&(new_arl->any.mmemo), &(temp_memo->mmemo), sizeof(MyMemo));
	 //This is to prevent appointment from being freed later
	 memset(&(temp_memo->mmemo.memo), 0, sizeof(struct Memo));
	 new_arl->next = search_rl;
	 search_rl = new_arl;

	 gtk_clist_set_row_data(GTK_CLIST(clist), 0, new_arl);
	 count++;
      }
   }
   jpilot_logf(LOG_DEBUG, "calling free_MemoList\n");
   free_MemoList(&memo_list);
   memo_list = NULL;
   return count;
}


static gboolean cb_destroy(GtkWidget *widget)
{
   if (search_rl) {
      free_AnyRecordList(&search_rl);
      search_rl = NULL;
   }
   return FALSE;
}

static void
  cb_quit(GtkWidget *widget,
	   gpointer   data)
{
   gtk_widget_destroy(data);
}

static void
  cb_entry(GtkWidget *widget,
	   gpointer   data)
{
   gchar *empty_line[] = { "","" };
   GtkWidget *clist;
   char *entry_text;
   int count;
   
   jpilot_logf(LOG_DEBUG, "enter cb_entry\n");
   
   clist = data;
   entry_text = gtk_entry_get_text(GTK_ENTRY(widget));
   if (!entry_text || !strlen(entry_text)) {
      return;
   }

   jpilot_logf(LOG_DEBUG, "entry text = %s\n", entry_text);

   gtk_clist_clear(GTK_CLIST(clist));

   count = search_datebook(entry_text, clist);
   count += search_address(entry_text, clist);
   count += search_todo(entry_text, clist);
   count += search_memo(entry_text, clist);

   if (count == 0) {
      gtk_clist_prepend(GTK_CLIST(clist), empty_line);
      gtk_clist_set_text(GTK_CLIST(clist), 0, 1, "No records found");
   }
   
   return;
}

static void
  cb_clist_select(GtkWidget      *clist,
		  gint           row,
		  gint           column,
		  GdkEventButton *event,
		  gpointer       data)
{

   AnyRecordList *arl;

   if (!event) return;

   arl = gtk_clist_get_row_data(GTK_CLIST(clist), row);
   if (arl == NULL) {
      return;
   }
   switch (arl->app_type) {
    case DATEBOOK:
      glob_find_id = arl->any.mappo.unique_id;
      glob_find_mon = arl->any.mappo.a.begin.tm_mon;
      glob_find_day = arl->any.mappo.a.begin.tm_mday;
      glob_find_year = arl->any.mappo.a.begin.tm_year;
      cb_app_button(NULL, GINT_TO_POINTER(DATEBOOK));
      break;
    case ADDRESS:
      glob_find_id = arl->any.maddr.unique_id;
      cb_app_button(NULL, GINT_TO_POINTER(ADDRESS));
      break;
    case TODO:
      glob_find_id = arl->any.mtodo.unique_id;
      cb_app_button(NULL, GINT_TO_POINTER(TODO));
      break;
    case MEMO:
      glob_find_id = arl->any.mmemo.unique_id;
      cb_app_button(NULL, GINT_TO_POINTER(MEMO));
      break;
   }
}

void cb_search_gui(GtkWidget *widget, gpointer data)
{
   static GtkWidget *window;
   GtkWidget *entry;
   GtkWidget *scrolled_window;
   GtkWidget *clist;
   GtkWidget *label;
   GtkWidget *button;
   GtkWidget *vbox, *hbox;
   
   if (GTK_IS_WIDGET(window)) {
      return;
   }

   if (search_rl) {
      free_AnyRecordList(&search_rl);
      search_rl = NULL;
   }

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_title(GTK_WINDOW(window), PN" Search");

   gtk_signal_connect(GTK_OBJECT(window), "destroy",
                      GTK_SIGNAL_FUNC(cb_destroy), window);

   vbox = gtk_vbox_new(FALSE, 0);
   gtk_container_add(GTK_CONTAINER(window), vbox);

   hbox = gtk_hbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

   label = gtk_label_new("Search for: ");
   gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

   entry = gtk_entry_new();
   gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);
   gtk_widget_grab_focus(GTK_WIDGET(entry));
   
   case_sense_checkbox = gtk_check_button_new_with_label("Case Sensitive");
   gtk_box_pack_start(GTK_BOX(hbox), case_sense_checkbox, FALSE, FALSE, 0);
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(case_sense_checkbox),
				FALSE);
   //gtk_signal_connect_object(GTK_OBJECT(todo_hide_completed_checkbox), 
//			     "clicked", GTK_SIGNAL_FUNC(cb_hide_completed),
//			     NULL);

   //Put the scrolled window up
   scrolled_window = gtk_scrolled_window_new(NULL, NULL);
   gtk_container_set_border_width(GTK_CONTAINER(scrolled_window), 0);
   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

   clist = gtk_clist_new(2);
   gtk_signal_connect(GTK_OBJECT(clist), "select_row",
		      GTK_SIGNAL_FUNC(cb_clist_select),
		      NULL);
   gtk_clist_set_shadow_type(GTK_CLIST(clist), SHADOW);
   gtk_clist_set_selection_mode(GTK_CLIST(clist), GTK_SELECTION_BROWSE);
   gtk_clist_set_column_width(GTK_CLIST(clist), 0, 50);
   //gtk_clist_set_column_width(GTK_CLIST(clist), 1, 300);
   gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW
					 (scrolled_window), clist);

   gtk_signal_connect(GTK_OBJECT(entry), "activate",
		      GTK_SIGNAL_FUNC(cb_entry),
		      clist);

   // Create a "Quit" button
   button = gtk_button_new_with_label("Done");
   gtk_signal_connect(GTK_OBJECT(button), "clicked",
		      GTK_SIGNAL_FUNC(cb_quit), window);
   gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

   gtk_widget_show_all(window);
}