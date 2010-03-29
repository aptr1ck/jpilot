/* $Id: print_headers.c,v 1.11 2010/03/29 05:44:31 rikster5 Exp $ */

/*******************************************************************************
 * print_headers.c
 * A module of J-Pilot http://jpilot.org
 *
 * Copyright (C) 2001 by Colin Brough
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
 ******************************************************************************/

/********************************* Includes ***********************************/
#include <stdio.h>
#include <string.h>

#include "print_headers.h"

/****************************** Main Code *************************************/
void print_common_prolog(FILE *f)
{
   char *common_prolog =
      "/Recode {\n"
      "  exch\n"
      "  findfont\n"
      "  dup length dict\n"
      "  begin\n"
      "    { def } forall\n"
      "    /Encoding ISOLatin1Encoding def\n"
      "    currentdict\n"
      "  end\n"
      "  definefont pop\n"
      "} bind def\n\n"
      "/inch {72 mul} def\n"
      "/edef { exch def } def\n"
      "/esub { exch sub } def\n"
      "/l    { lineto } def\n"
      "/m    { moveto } def\n"
      "/rm   { rmoveto } def\n"
      "/rl   { rlineto } def\n"
      "/strw { stringwidth } def\n"
      "/nstr 10 string def\n"
      "/slw  { setlinewidth } def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "/CenterText {\n"
      "    dup strw pop 2 div 5 2 roll 3 -1 roll dup 3 1 roll sub 2 div\n"
      "    add 3 -1 roll sub exch m show\n"
      "} def\n"
      "/RightText { dup strw pop 4 -1 roll exch sub 3 -1 roll m show } def\n"
      "%------------------------------------------------------------------\n"
      "% Should leave the minimum of 2 values on the stack\n"
      "\n"
      "/Min { 2 copy gt { exch } if pop } def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% This is an array of paper sizes lifted from ghostview; the A4\n"
      "% definitions are here because we scale everything else relative to\n"
      "% an A4 page.\n"
      "\n"
      "/A4X 842 def\n"
      "/A4Y 595 def\n"
      "\n"
      "/PaperSizes [\n"
      "        [ (Letter)       612     792 10 ]\n"
      "        [ (Legal)        612    1008 10 ]\n"
      "        [ (Statement)    396     612 10 ]\n"
      "        [ (Tabloid)      792    1224 10 ]\n"
      "        [ (Ledger)      1224     792 10 ]\n"
      "        [ (Folio)        612     936 10 ]\n"
      "        [ (Quarto)       610     780 10 ]\n"
      "        [ (7x9)          504     648 10 ]\n"
      "        [ (9x11)         648     792 10 ]\n"
      "        [ (9x12)         648     864 10 ]\n"
      "        [ (10x13)        720     936 10 ]\n"
      "        [ (10x14)        720    1008 10 ]\n"
      "        [ (Executive)    540     720 10 ]\n"
      "        [ (A0)          2384    3370 10 ]\n"
      "        [ (A1)          1684    2384 10 ]\n"
      "        [ (A2)          1191    1684 10 ]\n"
      "        [ (A3)           842    1191  9 ]\n"
      "        [ (A4)           595     842 10 ]\n"
      "        [ (A5)           420     595 12 ]\n"
      "        [ (A6)           297     420 10 ]\n"
      "        [ (A7)           210     297 10 ]\n"
      "        [ (A8)           148     210 10 ]\n"
      "        [ (A9)           105     148 10 ]\n"
      "        [ (A10)           73     105 10 ]\n"
      "        [ (B0)          2920    4127 10 ]\n"
      "        [ (B1)          2064    2920 10 ]\n"
      "        [ (B2)          1460    2064 10 ]\n"
      "        [ (B3)          1032    1460 10 ]\n"
      "        [ (B4)           729    1032 10 ]\n"
      "        [ (B5)           516     729 10 ]\n"
      "        [ (B6)           363     516 10 ]\n"
      "        [ (B7)           258     363 10 ]\n"
      "        [ (B8)           181     258 10 ]\n"
      "        [ (B9)           127     181 10 ]\n"
      "        [ (B10)           91     127 10 ]\n"
      "        [ (ISOB0)       2835    4008 10 ]\n"
      "        [ (ISOB1)       2004    2835 10 ]\n"
      "        [ (ISOB2)       1417    2004 10 ]\n"
      "        [ (ISOB3)       1001    1417 10 ]\n"
      "        [ (ISOB4)        709    1001 10 ]\n"
      "        [ (ISOB5)        499     709 10 ]\n"
      "        [ (ISOB6)        354     499 10 ]\n"
      "        [ (ISOB7)        249     354 10 ]\n"
      "        [ (ISOB8)        176     249 10 ]\n"
      "        [ (ISOB9)        125     176 10 ]\n"
      "        [ (ISOB10)        88     125 10 ]\n"
      "        [ (C0)          2599    3676 10 ]\n"
      "        [ (C1)          1837    2599 10 ]\n"
      "        [ (C2)          1298    1837 10 ]\n"
      "        [ (C3)           918    1296 10 ]\n"
      "        [ (C4)           649     918 10 ]\n"
      "        [ (C5)           459     649 10 ]\n"
      "        [ (C6)           323     459 10 ]\n"
      "        [ (C7)           230     323 10 ]\n"
      "        [ (DL)           312     624 10 ]\n"
      "        [ (Filo)         270     487 12 ]\n"
      "] def\n"
      "\n";

    fputs(common_prolog, f);
}

void print_common_setup(FILE *f)
{
   char *common_setup =
      "%------------------------------------------------------------------\n"
      "/Times-Roman      /Times-Roman-ISOLatin1 Recode\n"
      "/Times-Bold       /Times-Bold-ISOLatin1 Recode\n"
      "/Courier          /Courier-ISOLatin1 Recode\n"
      "/Courier-Bold     /Courier-Bold-ISOLatin1 Recode\n"
      "/Helvetica        /Helvetica-ISOLatin1 Recode\n"
      "%------------------------------------------------------------------\n"
      "% Based on the paper size, work out the scaling and fontsize for\n"
      "% the page.\n"
      "\n"
      "PaperSizes {\n"
      "        /PaperArray edef\n"
      "        PaperArray 0 get PageSize eq {\n"
      "                /ScaleX PaperArray 2 get A4X div def\n"
      "                /ScaleY PaperArray 1 get A4Y div def\n"
      "                /fontsize PaperArray 3 get def\n"
      "                /linespacing fontsize 1.1 mul def\n"
      "        } if\n"
      "} forall\n";

    fputs(common_setup, f);
}

/*----------------------------------------------------------------------
 * print_week_header    Routine to print the week plan view PostScript
 *                      header.
 *----------------------------------------------------------------------*/

void print_week_header(FILE *f)
{
   char *week_header =
      "/TM 560 def     % top margin\n"
      "/BM  30 def     % bottom margin\n"
      "/LM  30 def     % left margin\n"
      "/RM 810 def     % right margin\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "\n"
      "/itemfont       /Helvetica-ISOLatin1    def\n"
      "/datefont       /Helvetica-Bold         def\n"
      "/FirstHour       9 def  % Default beginning and end of the plan\n"
      "/LastHour       22 def\n"
      "\n"
      "/usefont {findfont fontsize scalefont setfont} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% Rotate page round so it is printed A4 landscape\n"
      "\n"
      "/A4landscape {\n"
      "        initgraphics\n"
      "        A4Y ScaleY mul 0 translate\n"
      "        90 rotate\n"
      "        ScaleX ScaleY Min dup scale\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "%       'concat' - routine to concatenate two strings from the\n"
      "%        stack, leaving result on the stack. Does it by stack\n"
      "%       manipulation, without needing to 'def' anything...\n"
      "\n"
      "/concat {\n"
      "        2 copy\n"
      "        length exch length add string dup dup\n"
      "        0 5 index putinterval\n"
      "        3 index length 3 index putinterval\n"
      "        exch pop exch pop\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% This does all the initial set up for a planner sheet, setting up\n"
      "% page, printing the times down left hand edge of sheet, printing\n"
      "% date and time of printing at the top, etc.\n"
      "\n"
      "/startprinting {\n"
      "        dup LastHour  gt { /LastHour  edef } { pop } ifelse\n"
      "        dup FirstHour lt { /FirstHour edef } { pop } ifelse\n"
      "        /Year edef\n"
      "        /Month edef\n"
      "        /Day edef\n"
      "        A4landscape\n"
      "        gsave\n"
      "        /Helvetica-Oblique findfont 8 scalefont setfont\n"
      "        LM TM 5 add m (Printed on: ) show\n"
      "          Day nstr cvs show (/) show\n"
      "        Month nstr cvs show (/) show\n"
      "         Year nstr cvs show\n"
      "        gsave\n"
      "        %----------------------------------------------------------\n"
      "        % Put the times, in 24 hour format, down the left hand edge\n"
      "        % of the page...\n"
      "        datefont usefont\n"
      "        /HourHeight TopOfDay BotOfDay sub LastHour FirstHour sub\n"
      "                    div def\n"
      "        FirstHour 1 LastHour {\n"
      "                dup dup 0 TimeToY fontsize 0.7 mul sub LM exch m\n"
      "                10 lt { (0) show } if\n"
      "                nstr cvs show (00) show\n"
      "        } for\n"
      "        grestore\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% Brief counterpart to startprinting: just showpages and restores\n"
      "% the graphics state...\n"
      "\n"
      "/finishprinting {\n"
      "        showpage\n"
      "        grestore\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% Give TimeToY a 24 hour clock time (hours, minutes) and it'll work\n"
      "% out a Y offset...\n"
      "\n"
      "/TimeColWidth 40 def\n"
      "/DayWidth RM LM sub TimeColWidth sub 7 div def\n"
      "/TopOfDay TM linespacing 2 mul sub def\n"
      "/BotOfDay BM linespacing 8 mul add def\n"
      "\n"
      "/TimeToY {\n"
      "        60 div exch FirstHour sub add HourHeight mul TopOfDay esub\n"
      "} def\n"
      "\n"
      "/TimeToY2 {\n"
      "        FirstHour sub HourHeight mul TopOfDay esub\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% Some constants for the the 'StringToTime' routine below\n"
      "\n"
      "/PM     (p) 0 get def\n"
      "/AM     (a) 0 get def\n"
      "/Colon  (:) 0 get def\n"
      "/Zero   (0) 0 get def\n"
      "/Nine   (9) 0 get def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% Give this a string containing a from/to time pair, like:\n"
      "%       (6:30pm to 7:30pm)\n"
      "% it will leave the two times, in 24-hour format, with the minutes\n"
      "% as a decimal fraction of the hour, on the stack.\n"
      "\n"
      "/StringToTimes {\n"
      "        /String edef\n"
      "\n"
      "        %----------------------------------------------------------\n"
      "        % This stuff extracts the from time from the start of the\n"
      "        % string.\n"
      "\n"
      "        String 1 get Colon eq { /Len1 1 def }{ /Len1 2 def } ifelse\n"
      "        /Hour1  String 0 Len1 getinterval cvi def\n"
      "        /Min1   String Len1 1 add 2 getinterval cvi def\n"
      "        Hour1 12 eq { /Hour1 0 def } if\n"
      "        String Len1 3 add get PM eq { /Hour1 Hour1 12 add def } if\n"
      "\n"
      "        Hour1 Min1 60 div add\n"
      "\n"
      "        %----------------------------------------------------------\n"
      "        /Len3 9 Len1 add def\n"
      "        String Len3 1 add get Colon eq { /Len2 1 def }\n"
      "                                       { /Len2 2 def } ifelse\n"
      "        /Hour2  String Len3 Len2 getinterval cvi def\n"
      "        /Min2   String Len3 Len2 1 add add 2 getinterval cvi def\n"
      "        Hour2 12 eq { /Hour2 0 def } if\n"
      "        String Len3 Len2 3 add add get PM eq\n"
      "            { /Hour2 Hour2 12 add def } if\n"
      "\n"
      "        Hour2 Min2 60 div add\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "\n"
      "/startday {\n"
      "        /Today edef\n"
      "        /TodayLM DayWidth Today mul TimeColWidth add LM add def\n"
      "        grestore gsave\n"
      "        gsave newpath\n"
      "        TodayLM TM m TodayLM DayWidth add TM l\n"
      "        TodayLM DayWidth add BM l TodayLM BM l\n"
      "        closepath 0 slw 0 setgray stroke\n"
      "        TodayLM TopOfDay 3 add m DayWidth 0 rlineto stroke\n"
      "        2 slw\n"
      "        TodayLM BotOfDay m DayWidth 0 rlineto stroke\n"
      "        Dashes {\n"
      "            0 slw [ 1 5 ] 1 setdash\n"
      "            FirstHour 1 add 1 LastHour 1 sub {\n"
      "                TodayLM exch 0 TimeToY m DayWidth 0 rlineto stroke\n"
      "            } for\n"
      "        } if\n"
      "        grestore\n"
      "        newpath\n"
      "        TodayLM TM m TodayLM DayWidth add TM l\n"
      "        TodayLM DayWidth add BM l TodayLM BM l\n"
      "        closepath clip\n"
      "        /BotItemY BotOfDay 3 sub def\n"
      "} def\n"
      "\n"
      "/dateline {\n"
      "        datefont usefont\n"
      "        TodayLM 5 add TM linespacing sub m show\n"
      "        TodayLM 5 add TM 100 sub m\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% 'spacify' routine to replace line-feeds with spaces in strings\n"
      "%  passed in - places string back on stack where it came from.\n"
      "\n"
      "/spacify {\n"
      "        /LF 10 def\n"
      "        /SP 32 def\n"
      "\n"
      "        /String edef\n"
      "\n"
      "        0 1 String length 1 sub {\n"
      "                /I edef\n"
      "                String I get LF eq {\n"
      "                        String I SP put\n"
      "                } if\n"
      "        } for\n"
      "        String\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% Move onto the next line\n"
      "\n"
      "/incY { /X LMpara def /Y Y linespacing sub def } def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% This routine will do automatic line-breaking of the item text, so\n"
      "% that as much of it as possible is visible.\n"
      "%\n"
      "% Arguments are: 'Paragraph Y para'\n"
      "\n"
      "/para {\n"
      "    /RMpara TodayLM DayWidth add def\n"
      "    /LMpara TodayLM 3 add def\n"
      "    /Y edef\n"
      "    spacify\n"
      "    /Paragraph edef\n"
      "    /SpaceW ( ) stringwidth pop def\n"
      "    RMpara LMpara sub Paragraph stringwidth pop gt\n"
      "    {\n"
      "        %----------------------------------------------\n"
      "        % The whole paragraph fits on one line - so\n"
      "        % just print it!\n"
      "        %----------------------------------------------\n"
      "        LMpara Y m Paragraph show\n"
      "    } {\n"
      "        %----------------------------------------------\n"
      "        % Have to do our clever formatting\n"
      "        %----------------------------------------------\n"
      "        /X LMpara def\n"
      "        {\n"
      "            Paragraph ( ) search\n"
      "            {\n"
      "                %--------------------------------------------\n"
      "                % Found a space - so check where to print it.\n"
      "                %--------------------------------------------\n"
      "                dup\n"
      "                stringwidth pop /SW edef\n"
      "                SW RMpara X sub gt { incY } if\n"
      "                X Y m show\n"
      "                pop\n"
      "                /X X SW SpaceW add add def\n"
      "                /Paragraph edef\n"
      "            } {\n"
      "                %----------------------------------------\n"
      "                % No space. Print rest of string and exit\n"
      "                %----------------------------------------\n"
      "                RMpara X sub Paragraph stringwidth pop lt\n"
      "                { incY } if\n"
      "                X Y m Paragraph show\n"
      "                pop\n"
      "                exit\n"
      "            } ifelse\n"
      "        } loop\n"
      "    } ifelse\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% Takes either one or two arguments - one if no time component, two\n"
      "% if there is also a time for the appointment. Prints time in left\n"
      "% margin, and actual item text offset by 65 point. Long lines can\n"
      "% generate extra strings, which should print on a new line, but at\n"
      "% the moment, on basis that space is critical and in Filofax I am\n"
      "% likely to get the gist - I can always check the details at home!\n"
      "\n"
      "/itemline {\n"
      "    itemfont usefont\n"
      "    count 0 ne {\n"
      "        count 1 eq {\n"
      "            /BotItemY BotItemY linespacing sub def\n"
      "            TodayLM 3 add BotItemY m show\n"
      "        } {\n"
      "            { count 3 gt { concat } { exit } ifelse } loop\n"
      "            /Details        edef\n"
      "            /HourTo         edef\n"
      "            /HourFrom       edef\n"
      "            %----------------------------------------\n"
      "            % Having got the times extracted, draw the\n"
      "            % shaded box round the blocked out time,\n"
      "            % and set this box to be a clipping path...\n"
      "            gsave\n"
      "            newpath\n"
      "            TodayLM                 HourFrom TimeToY2 m\n"
      "            TodayLM DayWidth add    HourFrom TimeToY2 l\n"
      "            TodayLM DayWidth add    HourTo   TimeToY2 l\n"
      "            TodayLM                 HourTo   TimeToY2 l\n"
      "            closepath gsave 0.9 setgray fill\n"
      "            grestore gsave 0 setgray stroke\n"
      "            grestore clip\n"
      "\n"
      "            %----------------------------------------\n"
      "            % Now fill in the text of the details\n"
      "            Details HourFrom TimeToY2 linespacing sub para\n"
      "            grestore\n"
      "        } ifelse\n"
      "    } if\n"
      "} def\n"
      "\n"
      "%-------------------------------------------------------------\n\n\n";

    fputs(week_header, f);
}

/*----------------------------------------------------------------------
 * print_month_header   Routine to print the month view PostScript
 *                      header.
 *----------------------------------------------------------------------*/

void print_month_header(FILE *f)
{
   char *month_header =
      "/Row 0 def\n"
      "/Col 0 def\n"
      "/DayNumber 0 def\n"
      "/LM  60 def\n"
      "/RM 780 def\n"
      "/TM 550 def\n"
      "/BM  50 def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% Utility routines are up here\n"
      "\n"
      "/Rectangle {\n"
      "        newpath 4 2 roll m exch dup 3 1 roll 0 rl 0 exch rl neg 0\n"
      "        rl closepath\n"
      "} def\n"
      "/DrawBox { slw Rectangle stroke 1 slw } def\n"
      "/ColToX { BoxWidth mul LM add } def\n"
      "/RowToY { 4 exch sub BoxHeight mul BM add } def\n"
      "/DaysOfWeek {\n"
      "        FDOW 0 eq {\n"
      "                [(Sun) (Mon) (Tue) (Wed) (Thu) (Fri) (Sat)]\n"
      "        } {\n"
      "                [(Mon) (Tue) (Wed) (Thu) (Fri) (Sat) (Sun)]\n"
      "        } ifelse\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "\n"
      "/InitialisePage {\n"
      "        /FDOW edef          % First day of week; 1 = Mon, 0 = Sun\n"
      "        /JpilotVersion edef % Jpilot version string\n"
      "        /TodaysDate edef    % String for date of printing\n"
      "        /FirstDay edef      % Day of 1st of month (0=Sun, 1=Mon...)\n"
      "        /MonthName edef     % Month/year string\n"
      "        %----------------------------------------------------------\n"
      "        % Orient page and scale to fit paper size\n"
      "        initgraphics\n"
      "        A4Y ScaleY mul 0 translate\n"
      "        90 rotate\n"
      "        ScaleX ScaleY Min dup scale\n"
      "\n"
      "        %----------------------------------------------------------\n"
      "        % Draw various headings\n"
      "        /Helvetica-Bold findfont fontsize 3 mul scalefont setfont\n"
      "        LM TM 30 sub RM MonthName CenterText\n"
      "        /Helvetica findfont fontsize 0.8 mul scalefont setfont\n"
      "        LM TM 30 sub m (Printed on: ) show TodaysDate show\n"
      "        RM TM 30 sub JpilotVersion RightText\n"
      "\n"
      "        %----------------------------------------------------------\n"
      "        % Now draw the seven days of week across top\n"
      "        /X LM def\n"
      "        /Y TM 60 sub def\n"
      "        /BoxWidth RM LM sub 7 div def\n"
      "        /BoxHeight Y BM sub 5 div def\n"
      "        /Helvetica-Bold findfont 16 scalefont setfont\n"
      "        DaysOfWeek\n"
      "        {\n"
      "            /DayName edef\n"
      "            0.5 setgray X Y BoxWidth 20 Rectangle fill\n"
      "            0   setgray X Y BoxWidth 20 1 DrawBox\n"
      "            1 setgray X Y 3 add X BoxWidth add DayName CenterText\n"
      "            /X X BoxWidth add def\n"
      "        } forall\n"
      "        0 setgray\n"
      "\n"
      "        %----------------------------------------------------------\n"
      "        % Now draw all the empty, slightly gray boxes that are the\n"
      "        % backdrop\n"
      "\n"
      "        0 1 6 {\n"
      "            /Col edef\n"
      "            0 1 4 {\n"
      "                /Row edef\n"
      "                Col ColToX Row RowToY BoxWidth BoxHeight Rectangle\n"
      "                gsave 0.95 setgray fill grestore 0 setgray stroke\n"
      "            } for\n"
      "        } for\n"
      "        /Col FirstDay FDOW 1 add sub def\n"
      "        Col -2 eq { /Col 5 def } if\n"
      "        /Row 0 def\n"
      "        gsave\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "\n"
      "/NextDay {\n"
      "        grestore gsave  % This copes with a changing clipping path\n"
      "        /DayNumber DayNumber 1 add def\n"
      "        /Col Col 1 add def\n"
      "        Col 7 eq { \n"
      "                /Row Row 1 add def /Col 0 def \n"
      "                Row 5 eq { /Row 0 def } if\n"
      "        } if\n"
      "        %----------------------------------------------------------\n"
      "        % First box the thing off in white, and set a clipping path\n"
      "        Col ColToX Row RowToY BoxWidth BoxHeight Rectangle\n"
      "        gsave 1 setgray fill grestore 0 setgray stroke\n"
      "\n"
      "        Col ColToX 1 add Row RowToY 1 add BoxWidth 2 sub BoxHeight\n"
      "        2 sub Rectangle clip\n"
      "\n"
      "        %----------------------------------------------------------\n"
      "        % Then put up the large number for the day...\n"
      "        /Helvetica-Bold findfont fontsize 2 mul scalefont setfont\n"
      "        Col ColToX 3 add\n"
      "        Row RowToY BoxHeight fontsize 2 mul 1 add sub add m\n"
      "        DayNumber nstr cvs show\n"
      "\n"
      "        %----------------------------------------------------------\n"
      "        % Finally, set variables for the items\n"
      "        /X Col ColToX 3 add def\n"
      "        /Y Row RowToY BoxHeight fontsize sub 2 sub add def\n"
      "        /Ytop2 Row RowToY BoxHeight fontsize 3 mul sub add def\n"
      "        /fsize fontsize 0.8 mul def\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% 'spacify' routine to replace line-feeds with spaces in strings\n"
      "%  passed in - places string back on stack where it came from.\n"
      "\n"
      "/spacify {\n"
      "        /LF 10 def\n"
      "        /SP 32 def\n"
      "\n"
      "        /String edef\n"
      "\n"
      "        0 1 String length 1 sub {\n"
      "                /I edef\n"
      "                String I get LF eq {\n"
      "                        String I SP put\n"
      "                } if\n"
      "        } for\n"
      "        String\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% Move onto the next line\n"
      "\n"
      "/incY { /X X_LM def /Y Y fsize sub def } def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% This routine will do automatic line-breaking of the item text, so\n"
      "% that as much of it as possible is visible.\n"
      "%\n"
      "% Arguments are: 'Paragraph Y para'\n"
      "\n"
      "/para {\n"
      "    /RMpara X PrintBoxWidth add def\n"
      "    /LMpara X def\n"
      "    /Y edef\n"
      "    spacify\n"
      "    /Paragraph edef\n"
      "    /SpaceW ( ) stringwidth pop def\n"
      "    RMpara LMpara sub Paragraph stringwidth pop gt\n"
      "    {\n"
      "        %----------------------------------------------\n"
      "        % The whole paragraph fits on one line - so\n"
      "        % just print it!\n"
      "        %----------------------------------------------\n"
      "        LMpara Y m Paragraph show\n"
      "    } {\n"
      "        %----------------------------------------------\n"
      "        % Have to do our clever formatting\n"
      "        %----------------------------------------------\n"
      "        /X LMpara def\n"
      "        /I 0 def\n"
      "        {\n"
      "            Paragraph ( ) search\n"
      "            {\n"
      "                %--------------------------------------------\n"
      "                % Found a space - so check where to print it.\n"
      "                %--------------------------------------------\n"
      "                dup\n"
      "                stringwidth pop /SW edef\n"
      "                SW RMpara X sub gt { /I I 1 add def I 2 eq { exit } if incY } if\n"
      "                X Y m show\n"
      "                pop\n"
      "                /X X SW SpaceW add add def\n"
      "                /Paragraph edef\n"
      "            } {\n"
      "                %----------------------------------------\n"
      "                % No space. Print rest of string and exit\n"
      "                %----------------------------------------\n"
      "                RMpara X sub Paragraph stringwidth pop lt\n"
      "                { /I I 1 add def I 2 eq { exit } if incY } if\n"
      "                X Y m Paragraph show\n"
      "                pop\n"
      "                exit\n"
      "            } ifelse\n"
      "        } loop\n"
      "    } ifelse\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% This procedure prints an item\n"
      "\n"
      "/TimedItem {\n"
      "        /ApptText edef\n"
      "        Y Ytop2 gt { /Y Ytop2 def } if\n"
      "        /Helvetica-Oblique findfont fsize scalefont setfont\n"
      "        X Y m show\n"
      "        /Helvetica-ISOLatin1 findfont fsize scalefont setfont\n"
      "        /X_LM X def\n"
      "        /X X 4.2 fsize mul add def\n"
      "        /PrintBoxWidth BoxWidth X X_LM sub sub 3 sub def\n"
      "        ApptText Y para\n"
      "        incY\n"
      "} def\n"
      "\n"
      "/UntimedItem {\n"
      "        /ApptText edef\n"
      "        /Helvetica-ISOLatin1 findfont fsize scalefont setfont\n"
      "        /X_LM_save X def\n"
      "        /X X 3 fsize mul add def\n"
      "        /X_LM X def\n"
      "        /PrintBoxWidth BoxWidth X X_LM_save sub sub 3 sub def\n"
      "        ApptText Y para\n"
      "        /X_LM X_LM_save def\n"
      "        incY\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "\n"
      "/SmallMonth {\n"
      "        grestore gsave  % This copes with a changing clipping path\n"
      "        /S_FDOM    edef\n"
      "        /NumDays   edef\n"
      "        /MonthName edef\n"
      "        /S_row     edef\n"
      "        /S_col     edef\n"
      "\n"
      "        /X1 S_col ColToX def\n"
      "        /X2 S_col 1 add ColToX def\n"
      "        /Y1 S_row RowToY def\n"
      "        /Y2 S_row 1 sub RowToY def\n"
      "        /SmallBoxWidth .125 BoxWidth mul def\n"
      "        /SmallLeftMargin X1 .16 BoxWidth mul add def\n"
      "        /Helvetica-Bold findfont 10 scalefont setfont \n"
      "        X1 Y2 12 sub X2 MonthName CenterText\n"
      "        /Helvetica-Bold findfont 8 scalefont setfont \n"
      "        /X SmallLeftMargin def\n"
      "        /DayName 1 string def\n"
      "        DaysOfWeek\n"
      "        {\n"
      "                0 get DayName 0 3 2 roll put\n"
      "                X S_row 1 sub RowToY 24 sub DayName RightText\n"
      "                /X X SmallBoxWidth add def\n"
      "        } forall\n"
      "        /Helvetica findfont 8 scalefont setfont \n"
      "        /SmallRow 0 def\n"
      "        /SmallCol S_FDOM FDOW sub def\n"
      "        SmallCol FDOW lt { /SmallCol SmallCol 7 add 7 mod def } if\n"
      "        1 1 NumDays {\n"
      "                /Day edef\n"
      "                SmallLeftMargin SmallCol SmallBoxWidth mul add\n"
      "                Y1 4 SmallRow sub 11 mul 10 add add\n"
      "                Day nstr cvs RightText\n"
      "                /SmallCol SmallCol 1 add def\n"
      "                SmallCol 7 ge {\n"
      "                        /SmallCol 0 def\n"
      "                        /SmallRow SmallRow 1 add def\n"
      "                        SmallRow 4 gt { /SmallRow 0 def } if\n"
      "                } if\n"
      "        } for\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "\n"
      "/SmallMonths {\n"
      "        /NumDays2       edef\n"
      "        /FDOM2          edef\n"
      "        /MonthName2     edef\n"
      "        /NumDays1       edef\n"
      "        /FDOM1          edef\n"
      "        /MonthName1     edef\n"
      "\n"
      "        Col 5 lt Row 1 gt or {\n"
      "            Row 0 ne {\n"
      "                5 4 MonthName1 NumDays1 FDOM1 SmallMonth\n"
      "                6 4 MonthName2 NumDays2 FDOM2 SmallMonth\n"
      "            } {\n"
      "                Col 2 add 0 MonthName1 NumDays1 FDOM1 SmallMonth\n"
      "                Col 3 add 0 MonthName2 NumDays2 FDOM2 SmallMonth\n"
      "            } ifelse\n"
      "        } {\n"
      "            0 0 MonthName1 NumDays1 FDOM1 SmallMonth\n"
      "            1 0 MonthName2 NumDays2 FDOM2 SmallMonth\n"
      "        } ifelse\n"
      "} def\n"
      "\n"
      "%------------------------------------------------------------------\n"
      "% This is the end of the header\n"
      "%------------------------------------------------------------------\n"
      "\n";

    fputs(month_header, f);
}

/*----------------------------------------------------------------------
 * print_day_header     Routine to print the day view PostScript
 *                      header.
 *----------------------------------------------------------------------*/

void print_day_header(FILE *f)
{
    char *day_header =
        "ScaleY ScaleX Min dup scale\n"
        "%----------------------------------------\n"
        "% First the gray background area\n"
        "newpath\n"
        "0 slw\n"
        " 0.5  inch  3.5 inch m\n"
        " 0          7.0 inch rl\n"
        " 7.5  inch  0        rl\n"
        " 0         -1   inch rl\n"
        "-3    inch  0        rl\n"
        " 0         -6   inch rl\n"
        "-0.75 inch  0        rl\n"
        " 0          6   inch rl\n"
        "-3    inch  0        rl\n"
        " 0         -6   inch rl\n"
        "closepath 0.85 setgray fill stroke\n"
        "%----------------------------------------\n"
        "% Next highlighting lines round title\n"
        "0 setgray\n"
        "1 slw newpath 0.5 inch 10.5    inch m 7.5 inch 0 rl stroke\n"
        "      newpath 0.5 inch  9.5625 inch m 7.5 inch 0 rl stroke\n"
        "2 slw newpath 0.5 inch  9.5    inch m 7.5 inch 0 rl stroke\n\n"
        "/Times-Roman-ISOLatin1 findfont 14 scalefont setfont\n"
        "0 slw\n"
        "0 1 11 {\n"
        "       /I edef\n"
        "       %----------------------------------------\n"
        "       % The hours\n"
        "       0.5 inch 9.15 inch I 0.5 inch mul sub m\n"
        "       I 10 lt { (0) show } if I nstr cvs show (:00) show\n"
        "       4.25 inch 9.15 inch I 0.5 inch mul sub m\n"
        "       I 12 add nstr cvs show (:00) show\n"
        "       %----------------------------------------\n"
        "       % The horizontal lines between hours\n"
        "       1.25 inch 9 inch I 0.5 inch mul sub m 3 inch 0 rl\n"
        "       5    inch 9 inch I 0.5 inch mul sub m 3 inch 0 rl\n"
        "} for\n"
        "stroke\n";
    fputs(day_header, f);
}

/*----------------------------------------------------------------------
 * print_todo_header
 *----------------------------------------------------------------------*/

void print_todo_header(FILE *f)
{
   char *todo_header =
      "%------------------------------------------------------------------\n"
      "() CategoryName eq {\n"
      "        /CategoryName (All Categories) def\n"
      "}\n"
      "/Ytitle 785 def\n"
      "/Ytop Ytitle 20 sub def\n"
      "/PageNo 0 def\n"
      "\n"
      "/IndentDone 30 def\n"
      "/IndentPriority IndentDone fontsize 1.5 mul add def\n"
      "/IndentDate     IndentDone fontsize 3   mul add def\n"
      "/IndentText     IndentDone fontsize 9.5 mul add def\n"
      "/fsize fontsize 1.2 mul def\n"
      "\n"
      "%-----------------------------------------\n"
      "\n"
      "/NewPage {\n"
      "        PageNo 0 ne { showpage } if\n"
      "        /PageNo PageNo 1 add def\n"
      "        ScaleY ScaleX Min dup scale\n"
      "        /Y Ytop def\n"
      /*        "        /Helvetica-Bold findfont fsize 1.5 mul scalefont setfont \n" */
      "        /Times-Roman-ISOLatin1 findfont fsize 1.5 mul scalefont setfont \n"
      "         0 Ytitle A4Y CategoryName CenterText\n"
      /*        "        /Helvetica findfont fsize scalefont setfont \n" */
      "        /Times-Roman-ISOLatin1 findfont fsize scalefont setfont \n"
      "        IndentDone Ytitle m (To Do List) show\n"
      "        /PageString 8 string def\n"
      "        (Page: ) PageString copy\n"
      "        PageString 5 PageNo nstr cvs putinterval\n"
      "        A4Y IndentDone sub Ytitle PageString RightText\n"
      "        2 slw\n"
      "        IndentDone Ytitle 3 sub m A4Y IndentDone sub Ytitle 3 sub l"
      " stroke\n"
      "        0 slw\n"
      "} def\n"
      "\n"
      "%-----------------------------------------\n"
      "\n"
      "/incY { /Y Y linespacing 1.2 mul sub def Y 30 lt {NewPage} if } def\n"
      "\n"
      "%-----------------------------------------\n"
      "\n"
      "/Todo {\n"
      "        /Note edef\n"
      "        /Text edef\n"
      "        /Date edef\n"
      "        /Priority edef\n"
      "        /Done edef\n"
      "        PageNo 0 eq { NewPage } { incY } ifelse\n"
      "        IndentDone Y 0.9 fsize mul dup\n"
      "        Done { rectfill } { rectstroke } ifelse\n"
      /*        "        /Helvetica-Bold findfont fsize scalefont setfont \n" */
      "        /Courier-Bold-ISOLatin1 findfont fsize scalefont setfont \n"
      "        IndentPriority Y m Priority nstr cvs show\n"
      /*        "        /Helvetica findfont fsize scalefont setfont \n" */
      "        /Courier-ISOLatin1 findfont fsize scalefont setfont \n"
      "        IndentDate Y m Date show\n"
      "        IndentText Y m Text show\n"
      "        () Note ne {\n"
      "                incY\n"
      /*        "                /Helvetica-Oblique findfont fsize scalefont setfont\n" */
      "                /Courier-ISOLatin1 findfont fsize scalefont setfont \n"
      "                gsave\n"
      "                IndentText 2 sub Y 2 sub A4Y IndentText IndentDone\n"
      "                add sub linespacing 1.2 mul rectclip\n"
      "                IndentText Y m Note show\n"
      "                grestore\n"
      "        } if\n"
      "} def\n";

    fputs(todo_header, f);
}

