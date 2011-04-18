/*
 * Copyright Colten Edwards (c) 1996
 * BitchX help file system. 
 * When Chelp is called the help file is loaded from 
 * BitchX.help and saved. This file is never loaded from disk after this.
 * Information from the help file is loaded into an array as 0-Topic.
 * $help() also calls the same routines except this information is loaded 
 * differantly as 1-Topic. this allows us to distingush between them 
 * internally. 
 */
 
#include "irc.h"
static char cvsrevision[] = "$Id$";
CVS_REVISION(chelp_c)
#include "struct.h"
#include "ircaux.h"
#include "chelp.h"
#include "output.h"
#include "hook.h"
#include "misc.h"
#include "vars.h"
#include "window.h"
#define MAIN_SOURCE
#include "modval.h"

#ifdef WANT_CHELP
int read_file (FILE *help_file, int helpfunc);
int in_chelp = 0;

struct chelp_entry {
	char *title;
	char **contents;
	char *relates;
};

struct chelp_index {
	int size;
	struct chelp_entry *entries;
};

struct chelp_index bitchx_help;
struct chelp_index script_help;

char *get_help_topic(char *args, int helpfunc)
{
char *new_comm = NULL;
int found = 0, i;
char *others = NULL;
	struct chelp_index *index = helpfunc ? &script_help : &bitchx_help;
	new_comm = LOCAL_COPY(args);

	for (i = 0; i < index->size; i++)
	{
		if (!my_strnicmp(index->entries[i].title, new_comm, strlen(new_comm)))
		{
			int j;
			char *text = NULL;
			if (found++)
			{
				m_s3cat(&others, " , ", index->entries[i].title);
				continue;
			}
			if (args && *args && do_hook(HELPTOPIC_LIST, "%s", args))
				put_it("%s",convert_output_format("$G \002$0\002: Help on Topic: \002$1\002", "%s %s", version, args));
			for (j = 0; (text = index->entries[i].contents[j]) != NULL; j++)
			{
				if (do_hook(HELPSUBJECT_LIST, "%s %s", new_comm, text))
				{
					in_chelp++;
					put_it("%s", convert_output_format(text, NULL));
					in_chelp--;
				}
			}		
			text = index->entries[i].relates;
			if (text && do_hook(HELPTOPIC_LIST, "%s", text))
				put_it("%s", convert_output_format(text, NULL));
		}
		else if (found)
			break;
	}
	if (!found)
	{
		if (do_hook(HELPTOPIC_LIST, "%s", args))
			bitchsay("No help on %s", args);
	}

	if (others && found)
	{
		if (do_hook(HELPTOPIC_LIST, "%d %s", found, others))
			put_it("Other %d subjects: %s", found - 1, others);
	}
	new_free(&others);
	if (helpfunc)
		return m_strdup(empty_string);
	return NULL;
}

BUILT_IN_COMMAND(chelp)
{
static int first_time = 1;
	reset_display_target();
	if (args && *args == '-' && !my_strnicmp(args, "-dump", 4))
	{
		int i, j;
		next_arg(args, &args);
		first_time = 1;
		if (bitchx_help.entries)
		{
			for (i = 0; i < bitchx_help.size; i++)
			{
				if (bitchx_help.entries[i].contents)
				{
					for (j =0; bitchx_help.entries[i].contents[j]; j++)
						new_free(&bitchx_help.entries[i].contents[j]);
				}
				new_free(&bitchx_help.entries[i].contents);
				new_free(&bitchx_help.entries[i].title);
				new_free(&bitchx_help.entries[i].relates);
			}
			new_free(&bitchx_help.entries);
			bitchx_help.size = 0;
		}
	}
	if (first_time)
	{
		char *help_dir = NULL;
		FILE *help_file;
#ifdef PUBLIC_SYSTEM
		malloc_sprintf(&help_dir, "%s", DEFAULT_BITCHX_HELP_FILE);
#else
		malloc_sprintf(&help_dir, "%s", get_string_var(BITCHX_HELP_VAR));
#endif
		if (!(help_file = uzfopen(&help_dir, get_string_var(LOAD_PATH_VAR), 1)))
		{
			new_free(&help_dir);
			return;
		}
		new_free(&help_dir);
		first_time = 0;
		read_file(help_file, 0);
		fclose(help_file);
	}	
	if (!args || !*args)
	{
		userage(command, helparg);
		return;
	}
	get_help_topic(args, 0);
}

int read_file(FILE *help_file, int helpfunc)
{
	char line[BIG_BUFFER_SIZE + 1];
	int item_number = 0;
	int topic = -1;
	struct chelp_index *index = helpfunc ? &script_help : &bitchx_help;

	while (fgets(line, sizeof line, help_file))
	{
		size_t len = strlen(line);
		if (line[len - 1] == '\n')
			line[len - 1] = '\0';

		if (!*line || *line == '#')
			continue;

		if (*line != ' ') /* we got a topic copy to topic */
		{
			if (!my_strnicmp(line, "-RELATED", 7))
			{
				if (topic > -1)
				{
					index->entries[topic].relates = m_strdup(line+8);
				}
			}
			else
			{	
				topic++;
				item_number = 0;
				RESIZE(index->entries, index->entries[0], topic + 1);

				index->entries[topic].title = m_strdup(line);
				index->entries[topic].contents = new_malloc(sizeof(char *));
				index->entries[topic].contents[0] = NULL;
				index->entries[topic].relates = NULL;
			}
		}
		else if (topic > -1)
		{ /* we found the subject material */
			item_number++;
			RESIZE(index->entries[topic].contents, char *, item_number + 1);

			index->entries[topic].contents[item_number-1] = m_strdup(line);
			index->entries[topic].contents[item_number] = NULL;
		}
	}

	index->size = topic + 1;

	return 0;
}
#endif
