/*
 * By: Q-bert][@Efnet qbert@clark.net (Rich Schuler)
 * this can be under whatever bX's license is
 */

#include <stdio.h>
#include <stdlib.h>
#include "irc.h"
#include "cset.h"
#include "ircaux.h"
#include "status.h"
#include "screen.h"
#include "vars.h"
#include "misc.h"
#include "output.h"
#include "module.h"
#include "hash.h"
#include "hash2.h"
#define INIT_MODULE
#include "modval.h"

void sort_scan (IrcCommandDll *, char *, char *, char *);

void sort_scan (IrcCommandDll *this_not_used, char *called, char *args, char *subargs)
{
	int numberofpeople = 0, server = -1;
	char *channel = NULL, *qbert = NULL;
	ChannelList *chanlist;
	NickList *anick, *ops = NULL, *nops = NULL, *voice = NULL, *tmp = NULL;
	
	if (!(chanlist = prepare_command (&server, channel, NO_OP)) )
		return;

	for (anick = next_nicklist (chanlist, NULL); anick; anick = next_nicklist (chanlist, anick))
	{
		if (!nick_isop(anick) && !nick_isvoice(anick))
		{
			tmp = (NickList *)new_malloc (sizeof (NickList));
			memcpy (tmp, anick, sizeof (NickList));
			tmp -> next = NULL;
			add_to_list ((List **)&nops, (List *)tmp);
		}
		else if (nick_isvoice(anick) && !nick_isop(anick))
		{
			tmp = (NickList *)new_malloc (sizeof (NickList));
			memcpy (tmp, anick, sizeof (NickList));
			tmp -> next = NULL;
			add_to_list ((List **)&voice, (List *)tmp);
		}
		else if (nick_isop(anick))
		{
			tmp = (NickList *)new_malloc (sizeof (NickList));
			memcpy (tmp, anick, sizeof (NickList));
			tmp -> next = NULL;
			add_to_list ((List **)&ops, (List *)tmp);
		}
		numberofpeople++;
	}

	put_it ("%s", convert_output_format (fget_string_var (FORMAT_NAMES_FSET), "%s %s %d %s", update_clock (GET_TIME), chanlist -> channel, numberofpeople, space));
	
	numberofpeople = 0;
	for (anick = ops; anick; anick = anick -> next)
	{
		malloc_strcat (&qbert, convert_output_format (fget_string_var (FORMAT_NAMES_OPCOLOR_FSET), "@ %s", anick -> nick));
		malloc_strcat (&qbert, space);
		if (numberofpeople++ == 4)
		{
			if (fget_string_var (FORMAT_NAMES_BANNER_FSET))
				put_it ("%s%s", convert_output_format (fget_string_var(FORMAT_NAMES_BANNER_FSET), NULL, NULL), qbert);
			else
				put_it ("%s", qbert);
			new_free (&qbert);
			numberofpeople = 0;
		}
	}
	for (anick = voice; anick; anick = anick -> next)
	{
		malloc_strcat (&qbert, convert_output_format (fget_string_var(FORMAT_NAMES_VOICECOLOR_FSET), "+ %s", anick -> nick));
		malloc_strcat (&qbert, space);
		if (numberofpeople++ == 4)
		{
			if (fget_string_var (FORMAT_NAMES_BANNER_FSET))
				put_it ("%s%s", convert_output_format (fget_string_var(FORMAT_NAMES_BANNER_FSET), NULL, NULL), qbert);
			else
				put_it ("%s", qbert);
			new_free (&qbert);
			numberofpeople = 0;
		}
	}
	for (anick = nops; anick; anick = anick -> next)
	{
		malloc_strcat (&qbert, convert_output_format (fget_string_var(FORMAT_NAMES_NICKCOLOR_FSET), "$ %s", anick -> nick));
		malloc_strcat (&qbert, space);
		if (numberofpeople++ == 4)
		{
			if (fget_string_var (FORMAT_NAMES_BANNER_FSET))
				put_it ("%s%s", convert_output_format (fget_string_var(FORMAT_NAMES_BANNER_FSET), NULL, NULL), qbert);
			else
				put_it ("%s", qbert);
			new_free (&qbert);
			numberofpeople = 0;
		}
	}
	if (numberofpeople && qbert)
	{
		if (fget_string_var (FORMAT_NAMES_BANNER_FSET))
			put_it ("%s%s", convert_output_format (fget_string_var (FORMAT_NAMES_BANNER_FSET), NULL, NULL), qbert);
		else
			put_it ("%s", qbert);
	}
	new_free (&qbert);

	if (fget_string_var (FORMAT_NAMES_FOOTER_FSET))
		put_it ("%s", convert_output_format (fget_string_var (FORMAT_NAMES_FOOTER_FSET), NULL, NULL));
	clear_sorted_nicklist (&ops);
	clear_sorted_nicklist (&nops);
	clear_sorted_nicklist (&voice);
}

int Scan_Init (IrcCommandDll **intp, Function_ptr *global_func)
{	
	set_global_func(global_func);
	set_dll_name("scan");
	add_module_proc (COMMAND_PROC, "scan", "SCAN", "This is just like /scan except it goes +o +v -o",
     	 0, 0, sort_scan, NULL);
	return 0;
}
