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
extern int in_cparse;
int in_chelp = 0;

typedef struct _chelp_struct {
	char *title;
	char **contents;
	char *relates;
} Chelp;

Chelp **help_index = NULL;
Chelp **script_help = NULL;

char *get_help_topic(char *args, int helpfunc)
{
char *new_comm = NULL;
int found = 0, i;
char *others = NULL;

	new_comm = LOCAL_COPY(args);

	for (i = 0; helpfunc ? script_help[i] : help_index[i]; i++)
	{
		if (!my_strnicmp(helpfunc?script_help[i]->title:help_index[i]->title, new_comm, strlen(new_comm)))
		{
			int j;
			char *text = NULL;
			if (found++)
			{
				m_s3cat(&others, " , ", helpfunc?script_help[i]->title:help_index[i]->title);
				continue;
			}
			if (args && *args && do_hook(HELPTOPIC_LIST, "%s", args))
				put_it("%s",convert_output_format("$G \002$0\002: Help on Topic: \002$1\002", version, args));
			for (j = 0; ; j++)
			{
				if (helpfunc && (script_help[i] && script_help[i]->contents[j]))
					text = script_help[i]->contents[j];
				else if (!helpfunc && (help_index[i] && help_index[i]->contents[j]))
					text = help_index[i]->contents[j];
				else 
					break;

				if (text && do_hook(HELPSUBJECT_LIST, "%s %s", new_comm, text))
				{
					in_chelp++;
					put_it("%s", convert_output_format(text, NULL));
					in_chelp--;
				}
			}		
			text = helpfunc ?script_help[i]->relates:help_index[i]->relates;
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
		if (help_index)
		{
			for (i = 0; help_index[i]; i++)
			{
				if (help_index[i]->contents)
				{
					for (j =0; help_index[i]->contents[j]; j++)
						new_free(&help_index[i]->contents[j]);
				}
				new_free(&help_index[i]->contents);
				new_free(&help_index[i]->title);
				new_free(&help_index[i]->relates);
				new_free(&help_index[i]);
			}
			new_free(&help_index);
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
char *topic = NULL;
char *subject = NULL;
int item_number = 0;
int topics = 0;
	fgets(line, sizeof(line)-1, help_file);
	if (line)
		line[strlen(line)-1] = '\0';
	while (!feof(help_file))
	{
		if (!line || !*line || *line == '#')
		{
			fgets(line, sizeof(line)-1, help_file);
			continue;
		}
		else if (*line && (*line != ' ')) /* we got a topic copy to topic */
		{
			topics++;
			item_number = 0;
			if (!my_strnicmp(line, "-RELATED", 7))
			{
				if (topic)
				{
					if (helpfunc)
						script_help[topics-1]->relates = m_strdup(line+8);
					else
						help_index[topics-1]->relates = m_strdup(line+8);
				}
			}
			else
			{	
				new_free(&topic); new_free(&subject);
				malloc_strcpy(&topic, line);
				if (helpfunc)
				{
					RESIZE(script_help, Chelp, topics+1);
					script_help[topics-1] = new_malloc(sizeof(Chelp));
					script_help[topics-1]->title = m_strdup(line);
				}
				else
				{
					RESIZE(help_index, Chelp, topics+1);
					help_index[topics-1] = new_malloc(sizeof(Chelp));
					help_index[topics-1]->title = m_strdup(line);
				}
			}
			fgets(line, sizeof(line)-1, help_file);
			if (line)
				line[strlen(line)-1] = '\0';
		}
		else if (topic && *topic)
		{ /* we found the subject material */
			do {
				if (!line || (line && *line != ' '))
					break;
				if (helpfunc)
				{
					RESIZE(script_help[topics-1]->contents, char **, ++item_number);
					script_help[topics-1]->contents[item_number-1] = m_strdup(line);
				}
				else
				{
					RESIZE(help_index[topics-1]->contents, char **, ++item_number);
					help_index[topics-1]->contents[item_number-1] = m_strdup(line);
				}
				fgets(line, sizeof(line)-1, help_file);
				if (line)
					line[strlen(line)-1] = '\0';
			} while (!feof(help_file));
		}
	}

	return 0;
}
#endif
