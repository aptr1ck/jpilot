/*******************************************************************************
 * address.c
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

/********************************* Includes ***********************************/
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <pi-source.h>
#include <pi-socket.h>
#include <pi-address.h>
#include <pi-dlp.h>

#include "address.h"
#include "i18n.h"
#include "utils.h"
#include "log.h"
#include "prefs.h"
#include "libplugin.h"
#include "password.h"

/********************************* Constants **********************************/
#define SORT_JAPANESE 8
#define SORT_JOS 16

/******************************* Global vars **********************************/
static int glob_sort_rule;
int addr_sort_order;

/****************************** Main Code *************************************/
static int address_compare(const void *v1, const void *v2)
{
   AddressList **al1, **al2;
   struct Address *a1, *a2;
   char *str1 = NULL, *str2 = NULL;
   int last_cmp1, last_cmp2;
   int sort_idx[4];
   int i, j, r;

   al1=(AddressList **)v1;
   al2=(AddressList **)v2;

   a1=&((*al1)->maddr.addr);
   a2=&((*al2)->maddr.addr);

   switch (glob_sort_rule & 0x7) {
    case SORT_BY_FNAME:
      sort_idx[1] = entryFirstname;
      sort_idx[2] = entryLastname;
      sort_idx[3] = entryCompany;
      break;
    case SORT_BY_LNAME:
    default:
      sort_idx[1] = entryLastname;
      sort_idx[2] = entryFirstname;
      sort_idx[3] = entryCompany;
      break;
    case SORT_BY_COMPANY:
      sort_idx[1] = entryCompany;
      sort_idx[2] = entryFirstname;
      sort_idx[3] = entryLastname;
      break;
   } 

   last_cmp1=last_cmp2=0;

   if (!(glob_sort_rule & SORT_JAPANESE) || (glob_sort_rule & SORT_JOS)) { /* normal */
      while (last_cmp1 < 3 && last_cmp2 < 3) {
         str1=str2=NULL;
         /* Find the next non-blank field to use for sorting */
         for (i=last_cmp1+1; i<=3; i++) {
            if (a1->entry[sort_idx[i]]) {
               if ((str1 = malloc(strlen(a1->entry[sort_idx[i]])+1)) == NULL) {
                  return 0;
               }
               strcpy(str1, a1->entry[sort_idx[i]]);
               /* Convert string to lower case for more accurate comparison */
               for (j=strlen(str1)-1; j >= 0; j--) {
                  str1[j] = tolower(str1[j]);
               }
               break;
            }
         }
         last_cmp1 = i;

         if (!str1) return -1;

         for (i=last_cmp2+1; i<=3; i++) {
            if (a2->entry[sort_idx[i]]) {
               if ((str2 = malloc(strlen(a2->entry[sort_idx[i]])+1)) == NULL) {
                  return 0;
               }
               strcpy(str2, a2->entry[sort_idx[i]]);
               for (j=strlen(str2)-1; j >= 0; j--) {
                  str2[j] = tolower(str2[j]);
               }
               break;
            }
         }
         last_cmp2 = i;

         if (!str2) {
            free(str1);
            return 1;
         }

         r = strcoll(str1, str2);

         if (str1) free(str1);
         if (str2) free(str2);

         if (r != 0) return r;

         /* Comparisons between unequal fields, such as last name and company
          * must assume that the other fields are blank.  This matches
          * Palm sort ordering. */
         if (last_cmp1 != last_cmp2) {
            return (last_cmp2 - last_cmp1);
         }
      }   /* end of while loop to search over 3 fields */

      /* Compared all search fields and no difference found */
      return 0;

   } else if ((glob_sort_rule & SORT_JAPANESE) && !(glob_sort_rule & SORT_JOS)){
      /* Japanese sorting has not been updated to fix Bug 1814 because no test
       * platform is available for Western programmers maintaining Jpilot. */
      int sort1, sort2, sort3;
      char *tmp_p1, *tmp_p2, *tmp_p3;
      sort1 = sort_idx[1];
      sort2 = sort_idx[2];
      sort3 = sort_idx[3];

      if (a1->entry[sort1] || a1->entry[sort2]) {
         if (a1->entry[sort1] && a1->entry[sort2]) {
            if (!(tmp_p1 = strchr(a1->entry[sort1],'\1'))) tmp_p1=a1->entry[sort1]+1;
            if (!(tmp_p2 = strchr(a1->entry[sort2],'\1'))) tmp_p2=a1->entry[sort2]+1;
            if ((str1 = malloc(strlen(tmp_p1)+strlen(tmp_p2)+1)) == NULL) {
               return 0;
            }
            strcpy(str1, tmp_p1);
            strcat(str1, tmp_p2);
         }
         if (a1->entry[sort1] && (!a1->entry[sort2])) {
            if (!(tmp_p1 = strchr(a1->entry[sort1],'\1'))) tmp_p1=a1->entry[sort1]+1;
            if ((str1 = malloc(strlen(tmp_p1)+1)) == NULL) {
               return 0;
            }
            strcpy(str1, tmp_p1);
         }
         if ((!a1->entry[sort1]) && a1->entry[sort2]) {
            if (!(tmp_p2 = strchr(a1->entry[sort2],'\1'))) tmp_p2=a1->entry[sort2]+1;
            if ((str1 = malloc(strlen(tmp_p2)+1)) == NULL) {
               return 0;
            }
            strcpy(str1, tmp_p2);
         }
      } else if (a1->entry[sort3]) {
         if (!(tmp_p3 = strchr(a1->entry[sort3],'\1'))) tmp_p3=a1->entry[sort3]+1;
         if ((str1 = malloc(strlen(tmp_p3)+1)) == NULL) {
            return 0;
         }
         strcpy(str1, tmp_p3);
      } else {
         return 1;
      }

      if (a2->entry[sort1] || a2->entry[sort2]) {
         if (a2->entry[sort1] && a2->entry[sort2]) {
            if (!(tmp_p1 = strchr(a2->entry[sort1],'\1'))) tmp_p1=a2->entry[sort1]+1;
            if (!(tmp_p2 = strchr(a2->entry[sort2],'\1'))) tmp_p2=a2->entry[sort2]+1;
            if ((str2 = malloc(strlen(tmp_p1)+strlen(tmp_p2)+1)) == NULL) {
               return 0;
            }
            strcpy(str2, tmp_p1);
            strcat(str2, tmp_p2);
         }
         if (a2->entry[sort1] && (!a2->entry[sort2])) {
            if (!(tmp_p1 = strchr(a2->entry[sort1],'\1'))) tmp_p1=a2->entry[sort1]+1;
            if ((str2 = malloc(strlen(tmp_p1)+1)) == NULL) {
               return 0;
            }
            strcpy(str2, tmp_p1);
         }
         if ((!a2->entry[sort1]) && a2->entry[sort2]) {
            if (!(tmp_p2 = strchr(a2->entry[sort2],'\1'))) tmp_p2=a2->entry[sort2]+1;
            if ((str2 = malloc(strlen(tmp_p2)+1)) == NULL) {
               return 0;
            }
            strcpy(str2, tmp_p2);
         }
      } else if (a2->entry[sort3]) {
         if (!(tmp_p3 = strchr(a2->entry[sort3],'\1'))) tmp_p3=a2->entry[sort3]+1;
         if ((str2 = malloc(strlen(tmp_p3)+1)) == NULL) {
            return 0;
         }
         strcpy(str2, tmp_p3);
      } else {
         free(str1);
         return -1;
      }

      /* lower case the strings for a better compare */
      for (i=strlen(str1)-1; i >= 0; i--) {
         str1[i] = tolower(str1[i]);
      }
      for (i=strlen(str2)-1; i >= 0; i--) {
         str2[i] = tolower(str2[i]);
      }

      i = strcoll(str1, str2);
      if (str1) free(str1);
      if (str2) free(str2);

      return i;
   }

   /* Should never be reached.  Assume records are unsortable if reached */
   return 0;
}

/* sort_order: SORT_ASCENDING | SORT_DESCENDING */
static int address_sort(AddressList **al, int sort_order)
{
   AddressList *temp_al;
   AddressList **sort_al;
   int count, i;
   long use_jos, char_set;

   /* Count the entries in the list */
   for (count=0, temp_al=*al; temp_al; temp_al=temp_al->next, count++) {}

   if (count<2) {
      /* No need to sort 0 or 1 items */
      return EXIT_SUCCESS;
   }

   glob_sort_rule = addr_sort_order;

   get_pref(PREF_CHAR_SET, &char_set, NULL);
   if (char_set == CHAR_SET_JAPANESE || char_set == CHAR_SET_SJIS_UTF) {
      glob_sort_rule = glob_sort_rule | SORT_JAPANESE;
   } else {
      glob_sort_rule = glob_sort_rule & (SORT_JAPANESE-1);
   }
   get_pref(PREF_USE_JOS, &use_jos, NULL);
   if (use_jos) {
      glob_sort_rule = glob_sort_rule | SORT_JOS;
   } else {
      glob_sort_rule = glob_sort_rule & (SORT_JOS-1);
   }

   /* Allocate an array to be qsorted */
   sort_al = calloc(count, sizeof(AddressList *));
   if (!sort_al) {
      jp_logf(JP_LOG_WARN, "address_sort(): %s\n", _("Out of memory"));
      return EXIT_FAILURE;
   }

   /* Set our array to be a list of pointers to the nodes in the linked list */
   for (i=0, temp_al=*al; temp_al; temp_al=temp_al->next, i++) {
      sort_al[i] = temp_al;
   }

   qsort(sort_al, count, sizeof(AddressList *), address_compare);

   /* Put the linked list in the order of the array */
   if (sort_order==SORT_ASCENDING) {
      sort_al[count-1]->next = NULL;
      for (i=count-1; i>0; i--) {
         sort_al[i-1]->next=sort_al[i];
      }
      *al = sort_al[0];
   } else {
      /* Descending order */
      for (i=count-1; i>0; i--) {
         sort_al[i]->next=sort_al[i-1];
      }
      sort_al[0]->next = NULL;
      *al = sort_al[count-1];
   }

   free(sort_al);

   return EXIT_SUCCESS;
}

void free_AddressList(AddressList **al)
{
   AddressList *temp_al, *temp_al_next;

   for (temp_al = *al; temp_al; temp_al=temp_al_next) {
      free_Address(&(temp_al->maddr.addr));
      temp_al_next = temp_al->next;
      free(temp_al);
   }
   *al = NULL;
}

int get_address_app_info(struct AddressAppInfo *ai)
{
   int num, r;
   int rec_size;
   unsigned char *buf;

   memset(ai, 0, sizeof(*ai));
   /* Put at least one entry in there */
   strcpy(ai->category.name[0], "Unfiled");

   r = jp_get_app_info("AddressDB", &buf, &rec_size);
   if ((r != EXIT_SUCCESS) || (rec_size<=0)) {
      jp_logf(JP_LOG_WARN, _("%s:%d Error reading application info %s\n"), __FILE__, __LINE__, "AddressDB");
      if (buf) {
         free(buf);
      }
      return EXIT_FAILURE;
   }

   num = unpack_AddressAppInfo(ai, buf, rec_size);
   if (buf) {
      free(buf);
   }
   if ((num<0) || (rec_size<=0)) {
      jp_logf(JP_LOG_WARN, _("Error reading file: %s\n"), "AddressDB.pdb");
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}

int get_addresses(AddressList **address_list, int sort_order)
{
   return get_addresses2(address_list, sort_order, 1, 1, 1, CATEGORY_ALL);
}
/*
 * sort_order: SORT_ASCENDING | SORT_DESCENDING
 * modified, deleted and private, 0 for no, 1 for yes, 2 for use prefs
 */
int get_addresses2(AddressList **address_list, int sort_order,
                   int modified, int deleted, int privates, int category)
{
   GList *records;
   GList *temp_list;
   int recs_returned, i, num;
   struct Address addr;
   AddressList *temp_a_list;
   long keep_modified, keep_deleted;
   int keep_priv;
   long char_set;
   buf_rec *br;
   char *buf;
   pi_buffer_t *RecordBuffer;

   jp_logf(JP_LOG_DEBUG, "get_addresses2()\n");
   if (modified==2) {
      get_pref(PREF_SHOW_MODIFIED, &keep_modified, NULL);
   } else {
      keep_modified = modified;
   }
   if (deleted==2) {
      get_pref(PREF_SHOW_DELETED, &keep_deleted, NULL);
   } else {
      keep_deleted = deleted;
   }
   if (privates==2) {
      keep_priv = show_privates(GET_PRIVATES);
   } else {
      keep_priv = privates;
   }
   get_pref(PREF_CHAR_SET, &char_set, NULL);

   *address_list=NULL;
   recs_returned = 0;

   num = jp_read_DB_files("AddressDB", &records);
   if (-1 == num)
      return 0;

   for (temp_list = records; temp_list; temp_list = temp_list->next) {
      if (temp_list->data) {
         br=temp_list->data;
      } else {
         continue;
      }
      if (!br->buf) {
         continue;
      }

      if ( ((br->rt==DELETED_PALM_REC)  && (!keep_deleted)) ||
           ((br->rt==DELETED_PC_REC)    && (!keep_deleted)) ||
           ((br->rt==MODIFIED_PALM_REC) && (!keep_modified)) ) {
         continue;
      }
      if ((keep_priv != SHOW_PRIVATES) &&
          (br->attrib & dlpRecAttrSecret)) {
         continue;
      }

      RecordBuffer = pi_buffer_new(br->size);
      memcpy(RecordBuffer->data, br->buf, br->size);
      RecordBuffer->used = br->size;

      if (unpack_Address(&addr, RecordBuffer, address_v1) == -1) {
         pi_buffer_free(RecordBuffer);
         continue;
      }
      pi_buffer_free(RecordBuffer);

      if ( ((br->attrib & 0x0F) != category) && category != CATEGORY_ALL) {
         free_Address(&addr);
         continue;
      }
      buf = NULL;
      if (char_set != CHAR_SET_LATIN1) {
         for (i = 0; i < 19; i++) {
            if ((addr.entry[i] != NULL) && (addr.entry[i][0] != '\0')) {
               buf = charset_p2newj(addr.entry[i], -1, char_set);
               if (buf) {
                  free(addr.entry[i]);
                  addr.entry[i] = buf;
               }
            }
         }
      }

      temp_a_list = malloc(sizeof(AddressList));
      if (!temp_a_list) {
         jp_logf(JP_LOG_WARN, "get_addresses2(): %s\n", _("Out of memory"));
         break;
      }
      memcpy(&(temp_a_list->maddr.addr), &addr, sizeof(struct Address));
      temp_a_list->app_type = ADDRESS;
      temp_a_list->maddr.rt = br->rt;
      temp_a_list->maddr.attrib = br->attrib;
      temp_a_list->maddr.unique_id = br->unique_id;
      temp_a_list->next = *address_list;
      *address_list = temp_a_list;
      recs_returned++;
   }

   jp_free_DB_records(&records);

#ifdef JPILOT_DEBUG
   print_address_list(address_list);
#endif
   address_sort(address_list, sort_order);

   jp_logf(JP_LOG_DEBUG, "Leaving get_addresses2()\n");

   return recs_returned;
}

int pc_address_write(struct Address *addr, PCRecType rt, unsigned char attrib,
                     unsigned int *unique_id)
{
   pi_buffer_t *RecordBuffer;
   int i;
   buf_rec br;
   long char_set;

   get_pref(PREF_CHAR_SET, &char_set, NULL);
   if (char_set != CHAR_SET_LATIN1) {
      for (i = 0; i < 19; i++) {
         if (addr->entry[i]) 
            charset_j2p(addr->entry[i], strlen(addr->entry[i])+1, char_set);
      }
   }

   RecordBuffer = pi_buffer_new(0);
   if (pack_Address(addr, RecordBuffer, address_v1) == -1) {
      PRINT_FILE_LINE;
      jp_logf(JP_LOG_WARN, "pack_Address %s\n", _("error"));
      return EXIT_FAILURE;
   }
   br.rt=rt;
   br.attrib = attrib;
   br.buf = RecordBuffer->data;
   br.size = RecordBuffer->used;
   /* Keep unique ID intact */
   if (unique_id) {
      br.unique_id = *unique_id;
   } else {
      br.unique_id = 0;
   }

   jp_pc_write("AddressDB", &br);
   if (unique_id) {
      *unique_id = br.unique_id;
   }

   pi_buffer_free(RecordBuffer);

   return EXIT_SUCCESS;
}

#ifdef JPILOT_DEBUG
static void print_address_list(AddressList **al)
{
   AddressList *temp_al, *prev_al;

   for (prev_al=NULL, temp_al=*al; 
        temp_al;
        prev_al=temp_al, temp_al=temp_al->next) {
      jp_logf(JP_LOG_FILE | JP_LOG_STDOUT, "entry[0]=[%s]\n", temp_al->maddr.addr.entry[0]);
   }
}
#endif

