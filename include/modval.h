#ifndef _MODVAL_

/* include this so we have the enum table just in case someone forgets. */

#include "module.h"

/* 
 * this is a method first used in eggdrop modules.. 
 * A global table of functions is passed into the init function of module,
 * which is then assigned to the value global. This table is then indexed,
 * to access the various functions. What this means to us, is that we no 
 * longer require -rdynamic on the LDFLAGS line, which reduces the size 
 * of the client. This also makes this less compiler/environment dependant,
 * allowing modules to work on more platforms.
 * A Function_ptr *global is required in the module. The second arg to 
 * the init function is used to initialize this table. The table itself is
 * initialized in modules.c. This file should only be included once in the
 * module and also should be the last file included. It should never be 
 * included in the source itself. 
 * As long as we add new functions to the END of the list in module.h then
 * currently compiled modules will continue to function fine. If we change
 * the order of the list however, then BAD things will occur.
 * Copyright Colten Edwards July 1998.
 */

#define _MODVAL_
#ifndef BUILT_IN_DLL
#define BUILT_IN_DLL(x) \
	void x (IrcCommandDll *intp, char *command, char *args, char *subargs, char *helparg)
#endif

#if defined(WTERM_C) || defined(STERM_C)
/* If we are building wserv or scr-bx we can't use
 * the global table so we forward to the actual
 * functions instead of throught the global table.
 */
#define set_non_blocking(x) BX_set_non_blocking(x)
#define set_blocking(x) BX_set_blocking(x)
#define ip_to_host(x) BX_ip_to_host(x)
#define host_to_ip(x) BX_host_to_ip(x)
#define connect_by_number(a, b, c, d, e) BX_connect_by_number(a, b, c, d, e)
#ifndef __vars_h_
enum VAR_TYPES { unused };
#endif
int  get_int_var (enum VAR_TYPES);
void ircpanic (char *, ...);
char *my_ltoa (long);
#else

/*
 * need to undefine these particular defines. Otherwise we can't include
 * them in the table.
 */
#undef new_malloc
#undef new_free
#undef RESIZE
#undef malloc_strcat
#undef malloc_strcpy
#undef m_strdup
#undef m_strcat_ues
#undef m_strndup

#undef MODULENAME

#ifdef MAIN_SOURCE
void init_global_functions(void);
#ifndef __modules_c
extern char *_modname_;
extern Function_ptr *global;
#endif
#else

#ifdef INIT_MODULE
/* only in the first c file do we #define INIT_MODULE */
char *_modname_ = NULL;
Function_ptr *global = NULL;
#undef INIT_MODULE
#else
extern char *_modname_;
extern Function_ptr *global;
#endif

#endif /* MAIN_SOURCE */

#define MODULENAME _modname_

#define check_module_version(x) ((int) (global[MODULE_VERSION_CHECK]((unsigned long)x)))
#define set_dll_name(x) malloc_strcpy(&_modname_, x)
#define set_global_func(x) global = x;
#define initialize_module(x) { \
		global = global_table; \
		malloc_strcpy(&_modname_, x); \
		if (!check_module_version(MODULE_VERSION)) \
			return INVALID_MODVERSION; \
}

	
#ifndef MAIN_SOURCE
#define empty_string ""
#define space " "
#endif

#ifndef HAVE_VSNPRINTF
#define vsnprintf ((char * (*)())global[VSNPRINTF])
#endif
#ifndef HAVE_SNPRINTF
#define snprintf ((char * (*)())global[SNPRINTF])
#endif

/* ircaux.c */
#define new_malloc(x) ((void *)(global[NEW_MALLOC]((x),MODULENAME, __FILE__,__LINE__)))
#define new_free(x) ((void *)(global[NEW_FREE]((x),MODULENAME, __FILE__,__LINE__)))
#define RESIZE(x, y, z) ((void *)(global[NEW_REALLOC]((void **)& (x), sizeof(y) * (z), MODULENAME, __FILE__, __LINE__)))
#define malloc_strcpy(x, y) ((char *)(global[MALLOC_STRCPY]((char **)x, (char *)y, MODULENAME, __FILE__, __LINE__)))
#define malloc_strcat(x, y) ((char *)(global[MALLOC_STRCAT]((char **)x, (char *)y, MODULENAME, __FILE__, __LINE__)))
#define malloc_str2cpy(x, y, z) ((char *)(global[MALLOC_STR2CPY]((char **)x, (char *)y, (char *)z)))
#define m_3dup(x, y, z) ((char *) (global[M_3DUP]((char *)x, (char *)y, (char *)z)))
#define m_opendup ((char * (*)())global[M_OPENDUP])
#define m_s3cat(x, y, z) ((char *)(global[M_S3CAT]((char **)x, (char *)y, (char *)z)))
#define m_s3cat_s(x, y, z) ((char *)(global[M_S3CAT_S]((char **)x, (char *)y, (char *)z)))
#define m_3cat(x, y, z) ((char *) (global[M_3CAT]((char **)x, (char *)y, (char *)z)))
#define m_2dup(x, y) ((char *)(global[M_2DUP]((char *)x, (char *)y)))
#define m_e3cat(x, y, z) ((char *)(global[M_E3CAT]((char **)x, (char *)y, (char *)z)))

#define my_stricmp(x, y) ((int)(global[MY_STRICMP]((const unsigned char *)x, (const unsigned char *)y)))
#define my_strnicmp(x, y, n) ((int)(global[MY_STRNICMP]((const unsigned char *)x, (const unsigned char *)y, (int)n)))

#define my_strnstr(x, y, z) ((int) (global[MY_STRNSTR]((const unsigned char *)x, (const unsigned char *)y, (size_t)z)))
#define chop(x, n) ((char *) (global[CHOP]((char *)x, (int)n)))
#define strmcpy(x, y, n) ((char *) (global[STRMCPY]((char *)x, (const char *)y, (int)n)))
#define strmcat(x, y, n) ((char *) (global[STRMCAT]((char *)x, (const char *)y, (int)n)))
#define scanstr(x, y) ((int) (global[SCANSTR]((char *)x, (char *)y)))
#define m_dupchar(c) ((char *) (global[M_DUPCHAR]((int)c)))
#define streq(x, y) ((size_t) (global[STREQ]((const char *)x, (const char *)y)))
#define strieq(x, y) ((size_t) (global[STRIEQ]((const char *)x, (const char *)y)))
#define strmopencat ((char * (*)())global[STRMOPENCAT])
#define ov_strcpy(x, y) ((char *) (global[OV_STRCPY]((char *)x, (const char *)y)))
#define upper(x) ((char *) (global[UPPER]((char *)x)))
#define lower(x) ((char *) (global[LOWER]((char *)x)))
#define stristr(x, y) ((char *) (global[STRISTR]((const char *)x, (char *)y)))
#define rstristr(x, y) ((char *) (global[RSTRISTR]((char *)x, (char *)y)))
#define word_count(x) ((int) (global[WORD_COUNT]((char *)x)))
#define remove_trailing_spaces(x) ((char *) (global[REMOVE_TRAILING_SPACES]((char *)x)))
#define expand_twiddle(x) ((char *) (global[EXPAND_TWIDDLE]((char *)x)))
#define check_nickname(x) ((char *) (global[CHECK_NICKNAME]((char *)x)))
#define sindex(x, y) ((char *) (global[SINDEX](( char *)x, (char *)y)))
#define rsindex(x, y, z, a) ((char *) (global[RSINDEX](( char *)x, (char *)y, (char *)z, (int)a)))
#define is_number(x) ((int) (global[ISNUMBER]((char *)x)))
#define rfgets(x, n, y) ((char *) (global[RFGETS]((char *)x, (int)n, (FILE *)y)))
#define path_search(x, y) ((char *) (global[PATH_SEARCH]((char *)x, (char *)y)))
#define double_quote(x, y, z) ((char *) (global[DOUBLE_QUOTE]((const char *)x, (const char *)y, (char *)z)))
#define ircpanic (global[IRCPANIC])
#define end_strcmp(x, y, n) ((int) (global[END_STRCMP]((const char *)x, (const char *)y, (int)n)))
#define beep_em(x) ((void) (global[BEEP_EM]((int)x)))
#define uzfopen(x, y, n) ((FILE *) (global[UZFOPEN]((char **)x, (char *)y, (int)n)))
#define get_time(x) ((global[FUNC_GET_TIME]((struct timeval *)x)))
#define time_diff(x, y) ((double) (global[TIME_DIFF]((struct timeval)x, (struct timeval)y)))
#define time_to_next_minute (int (*)(void)global[TIME_TO_NEXT_MINUTE])
#define plural(x) ((char *) (global[PLURAL]((int)x)))
#define my_ctime(x) ((char *) (global[MY_CTIME]((time_t)x)))
#define ccspan(x, y) ((size_t) (global[CCSPAN]((char *)x, (int)y)))

/* If we are in a module, undefine the previous define from ltoa to my_ltoa */
#ifdef ltoa
#undef ltoa
#endif
#define ltoa(x) ((char *) (global[LTOA]((long)x)))
#define strformat(x, y, n, z) ((char *) (global[STRFORMAT]((char *)x, (char *)y, (int)n, (char)z)))
#define MatchingBracket(x, y, z) ((char *) (global[MATCHINGBRACKET]((char *)x, ( char)y, (char)z)))
#define parse_number(x) ((int) (global[PARSE_NUMBER]((char **)x)))
#define splitw(x, y) ((int) (global[SPLITW]((char *)x, (char ***)y)))
#define unsplitw(x, y) ((char *) (global[UNSPLITW]((char ***)x, (char *)y)))
#define check_val(x) ((int) (global[CHECK_VAL]((char *)x)))
#define on_off(x) ((char *) (global[ON_OFF]((int)x)))
#define strextend(x, y, n) ((char *) (global[STREXTEND]((char *)x, (char)y, (int)n)))
#define strfill(x, n) ((const char *) (global[STRFILL]((char)x, (int)n)))
#define empty(x) ((int) (global[EMPTY_FUNC]((const char *)x)))
#define remove_brackets(x, y, n) ((char *) (global[REMOVE_BRACKETS]((char *)x, (char *)y, (int *)n)))
#define my_atol(x) ((long) (global[MY_ATOL]((char *)x)))
#define strip_control(x, y) ((void) (global[STRIP_CONTROL]((const char *)x, (char *)y)))
#define figure_out_address(a, b, c, d, e, f) ((int) (global[FIGURE_OUT_ADDRESS]((char *)a, (char **)b, (char **)c, (char **)d, (char **)e, (int *)f)))
#define strnrchr(x, y, n) ((char *) (global[STRNRCHR]((char *)x, (char)y, (int)n)))
#define mask_digits(x) ((void) (global[MASK_DIGITS]((char **)x)))
#define ccscpan(x, n) ((size_t) (global[CCSPAN]((const char *)x, (int)n)))
#define charcount(x, y) ((int) (global[CHARCOUNT]((const char *)x, (char)y)))
#define strpcat ((char *) (global[STRPCAT]))
#define strcpy_nocolorcodes(x, y) ((u_char *) (global[STRCPY_NOCOLORCODES]((u_char *)x, (const u_char *)y)))
#define cryptit(x) ((char *) (global[CRYPTIT]((const char *)x)))
#define stripdev(x) ((char *) (global[STRIPDEV]((char *)x)))
#define mangle_line(x, y, z) ((size_t) (global[MANGLE_LINE]((char *)x, (int)y, (size_t)z)))
#define m_strdup(x) ((char *)(global[M_STRDUP]((char *)x, MODULENAME, __FILE__, __LINE__)))
#define m_strcat_ues(x, y, z) ((char *)(global[M_STRCAT_UES](x, y, z, MODULENAME, __FILE__, __LINE__)))
#define m_strndup(x, y) ((char *)(global[M_STRNDUP](x, y, MODULENAME, __FILE__, __LINE__)))
#define malloc_sprintf ((char * (*)())global[MALLOC_SPRINTF])
#define m_sprintf ((char * (*)())global[M_SPRINTF])
#define next_arg(x, y) ((char *)(global[NEXT_ARG]((char *)x, (char **)y)))
#define new_next_arg(x, y) ((char *)(global[NEW_NEXT_ARG]((char *)x, (char **)y)))
#define new_new_next_arg(x, y, z) ((char *)(global[NEW_NEW_NEXT_ARG]((char *)x, (char **)y, (char *)z)))
#define last_arg(x) ((char *)(global[LAST_ARG]((char **)x)))
#define next_in_comma_list(x, y) ((char *)(global[NEXT_IN_COMMA_LIST]((char *)x, (char **)y)))
#define random_number(x) ((unsigned long)(global[RANDOM_NUMBER]((unsigned long)x)))


/* words.c reg.c */
#define search(x, y, z, n) ((char *) (global[SEARCH]((char *)x, (char **)y, (char *)z, (int)n)))
#define move_to_abs_word(x, y, n) ((char *) (global[MOVE_TO_ABS_WORD]((char *)x, (char **)y, (int)n)))
#define move_word_rel(x, y, n) ((char *) (global[MOVE_WORD_REL]((char *)x, (char **)y, (int)n)))
#define extract(x, y, n) ((char *) (global[EXTRACT]((char *)x, (int)y, (int)n)))
#define extract2(x, y, n) ((char *) (global[EXTRACT2]((char *)x, (int)y, (int)n)))
#define wild_match(x, y) ((int) (global[WILD_MATCH]((const unsigned char *)x, (const unsigned char *)y)))

/* network.c */
#define connect_by_number(a, b, c, d, e) ((int) (global[CONNECT_BY_NUMBER]((char *)a, (unsigned short *)b, (int)c, (int)d, (int)e)))
#define lookup_host(x) ((struct sockaddr_foobar *) (global[LOOKUP_HOST]((const char *)x)))
#define resolv(x) ((struct sockaddr_foobar *) (global[LOOKUP_HOST]((const char *)x)))
#define host_to_ip(x) ((char *) (global[HOST_TO_IP]((const char *)x)))
#define ip_to_host(x) ((char *) (global[IP_TO_HOST]((const char *)x)))
#define one_to_another(x) ((char *) (global[ONE_TO_ANOTHER]((const char *)x)))
#define set_blocking(x) ((int) (global[SET_BLOCKING]((int)x)))
#define set_non_blocking(x) ((int) (global[SET_NON_BLOCKING]((int)x)))


/* list.c */
#define add_to_list(x, y) ((void)(global[ADD_TO_LIST]((List **)x, (List *)y)))
#define add_to_list_ext(x, y, f) ((void) (global[ADD_TO_LIST_EXT]((List **)x, (List *)y, (int (*)(List *, List *))f )))
#define	find_in_list(x, y, z) ((List *) (global[FIND_IN_LIST]((List **)x, (char *)y, (int)z)))
#define	find_in_list_ext(x, y, n, f) ((List *) (global[FIND_IN_LIST_EXT]((List **)x, (char *)y, (int)n, (int (*)(List *, char *))f )))
#define	remove_from_list(x, y) ((List *) (global[REMOVE_FROM_LIST_]((List **)x, (char *)y)))
#define	remove_from_list_ext(x, y, f) ((List *) (global[REMOVE_FROM_LIST_EXT]((List **)x, (char *)y, (int (*)(List *, char *))f)))
#define	removewild_from_list(x, y) ((List *) (global[REMOVEWILD_FROM_LIST]((List **)x, (char *)y)))
#define	list_lookup(x, y, z, n)  ((List *) (global[LIST_LOOKUP]((List **)x, (char *)y, (int)z, (int)n)))
#define	list_lookup_ext(x, y, z, n, f) ((List *) (global[LIST_LOOKUP_EXT]((List **)x, (char *)y, (int)z, (int)n, (int (*)(List *, char *))f)))

/* alist.c */
#define add_to_array(x, y) ((Array_item *) (global[ADD_TO_ARRAY]((Array *)x, (Array_item *)y)))
#define remove_from_array(x, y) ((Array_item *) (global[REMOVE_FROM_ARRAY]((Array *)x, (char *)y)))
#define array_pop(x, y) ((Array_item *) (global[ARRAY_POP]((Array *)x, (int)y)))

#define remove_all_from_array(x, y) ((Array_item *) (global[REMOVE_ALL_FROM_ARRAY]((Array *)x, (char *)y)))
#define array_lookup(x, y, z, a) ((Array_item *) (global[ARRAY_LOOKUP]((Array *)x, (char *)y, (int)z, (int)a)))
#define find_array_item(x, y, z, a) ((Array_item *) (global[FIND_ARRAY_ITEM]((Array *)x, (char *)y, (int *)z, (int *)a)))

#define find_fixed_array_item(a, b, c, d, e, f) ((Array_item *) (global[FIND_FIXED_ARRAY_ITEM]((void *)a, (size_t)b, (int)c, (char *)d, (int *)e, (int *)f)))

/* output.c */
#define put_it	((void (*)())global[PUT_IT])
#define bitchsay ((void (*)())global[BITCHSAY])
#define yell ((void (*)())global[YELL])
#define add_to_screen(x) ((void) (global[ADD_TO_SCREEN]((unsigned char *)x)))
#define add_to_log(x, y, z, a) ((void) (global[ADD_TO_LOG]((FILE *)x, (time_t)y, (const char *)z, (int)a)))

#define bsd_glob(x, y, z, a) ((int) (global[BSD_GLOB]((char *)x, (int)y, (void *)z, (glob_t *)a)))
#define bsd_globfree(x) ((void) (global[BSD_GLOBFREE]((glob_t *)x)))

/* misc commands */
#define my_encrypt(x, y, z) ((void) (global[MY_ENCRYPT]((char *)x, (int)y, (char *)z)))
#define my_decrypt(x, y, z) ((void) (global[MY_DECRYPT]((char *)x, (int)y, (char *)z)))
#define prepare_command(x, y, z) ((ChannelList *)(global[PREPARE_COMMAND](x, y, z)))
#define convert_output_format ((char * (*)())global[CONVERT_OUTPUT_FORMAT])
#define userage(x, y) ((void) (global[USERAGE]((char *)x, (char *)y)))
#define send_text(x, y, z, a, b) ((void) (global[SEND_TEXT]((char *)x, (const char *)y, (char *)z, (int)a, (int)b)))
/* this needs to be worked out. it's passed in the IrcVariable * to _Init */
#define load(a, b, c, d) ((void) (global[FUNC_LOAD]((char *)a, (char *)b, (char *)c, (char *)d)))
#define update_clock(x) ((char *)(global[UPDATE_CLOCK](x)))
#define PasteArgs(x, n) ((char *) (global[PASTEARGS]((char **)x, (int)n)))
#define BreakArgs(x, y, z, n) ((int) (global[BREAKARGS]((char *)x, (char **)y, (char **)z, (int)n)))

#define set_lastlog_msg_level(x) ((unsigned long) (global[SET_LASTLOG_MSG_LEVEL]((unsigned long)x)))
#define split_CTCP(x, y, z) ((void) (global[SPLIT_CTCP]((char *)x, (char *)y, (char *)z)))
#define random_str(x, y) ((char *) (global[RANDOM_STR]((int)x, (int)y)))
#define dcc_printf ((int (*)())global[DCC_PRINTF])

/* screen.c */
#define prepare_display(x, y, z, a) ((unsigned char **) (global[PREPARE_DISPLAY]((const unsigned char *)x, (int)y, (int *)z, (int)a)))
#define add_to_window(x, y) ((void) (global[ADD_TO_WINDOW]((Window *)x, (const unsigned char *)y)))
#define skip_incoming_mirc(x) ((unsigned char *) (global[SKIP_INCOMING_MIRC]((unsigned char *)x)))
#define add_to_screen(x) ((void) (global[ADD_TO_SCREEN]((unsigned char *)x)))
#define split_up_line(x, y) ((unsigned char **) (global[SPLIT_UP_LINE]((const unsigned char *)x, (int)y)))
#define output_line(x) ((int) (global[OUTPUT_LINE]((const unsigned char *)x)))
#define output_with_count(x, y, z) ((int) (global[OUTPUT_WITH_COUNT]((const unsigned char *)x, (int)y, (int)z)))
#define scroll_window(x) ((void) (global[SCROLL_WINDOW]((Window *)x)))
#define cursor_not_in_display(x) ((void) (global[CURSOR_IN_DISPLAY]((Screen *)x)))
#define cursor_in_display(x) ((void) (global[CURSOR_IN_DISPLAY]((Screen *)x)))
#define is_cursor_in_display(x) ((int) (global[IS_CURSOR_IN_DISPLAY]((Screen *)x)))
#define repaint_window(x, y, z) ((void) (global[REPAINT_WINDOW]((Window *)x, (int)y, (int)z)))

#define kill_screen(x) ((void) (global[KILL_SCREEN]((Screen *)x)))
#define xterm_settitle ((void (*)(void)) global[XTERM_SETTITLE])
#define add_wait_prompt(a, b, c, d, e) ((void) (global[ADD_WAIT_PROMPT]((char *)a,(void (*) (char *, char *))b, (char *)c, (int)d, (int)e )))
#define skip_ctl_c_seq(a, b, c, d) ((const unsigned char *) (global[SKIP_CTL_C_SEQ]((const unsigned char *)a, (int *)b, (int *)c, (int)d)))
#define strip_ansi(x) ((unsigned char *) (global[STRIP_ANSI]((const unsigned char *)x)))
#define create_new_screen ((Screen * (*)(void))global[CREATE_NEW_SCREEN])
#define create_additional_screen ((Window * (*)(void))global[CREATE_ADDITIONAL_SCREEN])


/* window.c */
#define free_formats(x) ((void) (global[FREE_FORMATS]((Window *)x)))
#define set_screens_current_window(x, y) ((void) (global[SET_SCREENS_CURRENT_WINDOW]((Screen *)x, (Window *)y)))
#define new_window(x) ((Window *) (global[NEW_WINDOW]((Screen *)x)))
#define delete_window(x) ((void) (global[DELETE_WINDOW]((Window *)x)))
#define traverse_all_windows(x) ((int) (global[TRAVERSE_ALL_WINDOWS]((Window **)x)))
#define add_to_invisible_list(x) ((void) (global[ADD_TO_INVISIBLE_LIST]((Window *)x)))
#define remove_window_from_screen(x) ((void) (global[REMOVE_WINDOW_FROM_SCREEN]((Window *)x)))
#define recalculate_window_positions(x) ((void) (global[RECALCULATE_WINDOW_POSITIONS]((Screen *)x)))
#define move_window(x, y) ((void) (global[MOVE_WINDOW]((Window *)x, (int)y)))
#define resize_window(x, y, z) ((void) (global[RESIZE_WINDOW]((int)x, (Window *)y, (int)z)))
#define redraw_all_windows ((void (*)(void)) global[REDRAW_ALL_WINDOWS])
#define rebalance_windows(x) ((void) (global[REBALANCE_WINDOWS]((Screen *)x)))
#define recalculate_windows(x) ((void) (global[RECALCULATE_WINDOWS]((Screen *)x)))

#define update_all_windows ((void (*)(void))global[UPDATE_ALL_WINDOWS])

#define goto_window(x, y) ((void) (global[GOTO_WINDOW]((Screen *)x, (int)y)))
#define hide_window(x) ((void) (global[HIDE_BX_WINDOW]((Window *)x)))
#define swap_last_window(x, y) ((void) (global[FUNC_SWAP_LAST_WINDOW]((char)x, (char *)y)))
#define swap_next_window(x, y) ((void) (global[FUNC_SWAP_NEXT_WINDOW]((char)x, (char *)y)))
#define swap_previous_window(x, y) ((void) (global[FUNC_SWAP_PREVIOUS_WINDOW]((char)x, (char
#define show_window(x) ((void) (global[SHOW_WINDOW]((Window *)x)))
#define get_status_by_refnum(x, y) ((char *) (global[GET_STATUS_BY_REFNUM]((unsigned int)x, (unsigned int)y)))
#define get_visible_by_refnum(x) ((int) (global[GET_VISIBLE_BY_REFNUM]((char *)x)))
#define get_window_by_desc(x) ((Window *) (global[GET_WINDOW_BY_DESC]((unsigned int)x)))
#define get_window_by_refnum(x) ((Window *) (global[GET_WINDOW_BY_REFNUM]((unsigned int)x)))
#define get_window_by_name(x) ((Window *) (global[GET_WINDOW_BY_NAME]((char *)x)))
#define next_window(x, y) ((void) (global[FUNC_NEXT_WINDOW]((char)x, (char *)y)))
#define previous_window(x, y) ((void) (global[FUNC_PREVIOUS_WINDOW]((char)x, (char *)y)))
#define update_window_status(x, y) ((void) (global[UPDATE_WINDOW_STATUS]((Window *)x, (int)y)))
#define update_all_status(x, y, z) ((void) (global[UPDATE_ALL_STATUS]((Window *)x, (char *)y, (int) z)))
#define update_window_status_all ((void (*)())global[UPDATE_WINDOW_STATUS_ALL])
#define status_update(x) ((int) (global[STATUS_UPDATE]((int)x)))

#define set_prompt_by_refnum(x, y) ((void) (global[SET_PROMPT_BY_REFNUM]((unsigned int)x, (char *)y)))
#define get_prompt_by_refnum(x) ((char *) (global[GET_PROMPT_BY_REFNUM]((unsigned int)x)))
#define query_nick() ((char *)(global[QUERY_NICK])())
#define query_host() ((char *)(global[QUERY_HOST])())
#define query_cmd()  ((char *)(global[QUERY_CMD])())
#define get_target_by_refnum(x) ((char *) (global[GET_TARGET_BY_REFNUM]((unsigned int)x)))
#define get_target_cmd_by_refnum(x) ((char *) (global[GET_TARGET_CMD_BY_REFNUM]((unsigned int)x)))
#define get_window_target_by_desc(x) ((Window *) (global[GET_WINDOW_TARGET_BY_DESC]((char *)x)))
#define is_current_channel(x, y, z) ((int) (global[IS_CURRENT_CHANNEL]((char *)x, (int)y, (int)z)))
#define set_current_channel_by_refnum(x, y) ((char *) (global[SET_CURRENT_CHANNEL_BY_REFNUM]((int)x, (char *)y)))
#define get_current_channel_by_refnum(x) ((char *) (global[GET_CURRENT_CHANNEL_BY_REFNUM]((int)x)))
#define get_refnum_by_window(x) ((char *) (global[GET_REFNUM_BY_WINDOW]((Window *)x)))
#define is_bound_to_window(x, y) ((int) (global[IS_BOUND_TO_WINDOW]((Window *)x, (char *)y)))
#define get_window_bound_channel(x) ((Window *) (global[GET_WINDOW_BOUND_CHANNEL]((char *)x)))
#define is_bound_anywhere(x) ((int) (global[IS_BOUND_ANYWHERE]((char *)x)))
#define is_bound(x, y) ((int) (global[IS_BOUND]((char *)x, (int)y)))
#define unbind_channel(x, y) ((void) (global[UNBIND_CHANNEL]((char *)x, (int)y)))
#define get_bound_channel(x) ((char *) (global[GET_BOUND_CHANNEL]((Window *)x)))
#define get_window_server(x) ((int) (global[GET_WINDOW_SERVER]((unsigned int)x)))
#define set_window_server(x, y, z) ((void) (global[SET_WINDOW_SERVER]((int)x, (int)y, (int)z)))
#define window_check_servers ((void (*)(int))global[WINDOW_CHECK_SERVERS])
#define window_close_server ((void (*)(int))global[WINDOW_CHECK_SERVERS])
#define change_window_server(x, y) ((void) (global[CHANGE_WINDOW_SERVER]((int)x, (int)y))) 
#define set_level_by_refnum(x, y) ((void) (global[SET_LEVEL_BY_REFNUM]((unsigned int)x, (int)y)))
#define message_to(x) ((void) (global[MESSAGE_TO]((unsigned int)x)))
#define clear_window(x) ((void) (global[CLEAR_WINDOW]((Window *)x)))
#define clear_all_windows(x, y) ((void) (global[CLEAR_ALL_WINDOWS]((int)x, (int)y)))
#define clear_window_by_refnum(x) ((void) (global[CLEAR_WINDOW_BY_REFNUM]((unsigned int)x)))
#define unclear_window_by_refnum(x) ((void) (global[UNCLEAR_WINDOW_BY_REFNUM]((unsigned int)x)))
#define set_scroll_lines(x, y, z) ((void) (global[SET_SCROLL_LINES]((Window *)x, (char *)y, (int)y)))
#define set_continued_lines(x, y, z) ((void) (global[SET_CONTINUED_LINES]((Window *)x, (char *)y, (int)y)))
#define current_refnum ((int (*)())global[CURRENT_REFNUM])
#define number_of_windows_on_screen(x) ((int) (global[NUMBER_OF_WINDOWS_ON_SCREEN]((Window *)x)))
#define set_scrollback_size(x, y, z) ((void) (global[SET_SCROLLBACK_SIZE]((Window *)x, (char *)y, (int)y)))
#define is_window_name_unique(x) ((int) (global[IS_WINDOW_NAME_UNIQUE]((char *)x)))
#define get_nicklist_by_window(x) ((char *) (global[GET_NICKLIST_BY_WINDOW]((Window *)x)))
#define scrollback_backwards_lines(x) ((void) (global[SCROLLBACK_BACKWARDS_LINES]((int)x)))
#define scrollback_forwards_lines(x) ((void) (global[SCROLLBACK_FORWARDS_LINES]((int)x)))
#define scrollback_forwards(x, y) ((void) (global[SCROLLBACK_FORWARDS]((char)x, (char *)y)))
#define scrollback_backwards(x, y) ((void) (global[SCROLLBACK_BACKWARDS]((char)x, (char *)y)))
#define scrollback_end(x, y) ((void) (global[SCROLLBACK_END]((char)x, (char *)y)))
#define scrollback_start(x, y) ((void) (global[SCROLLBACK_START]((char)x, (char *)y)))
#define hold_mode(x, y, z) ((void) (global[HOLD_MODE]((Window *)x, (int)y, (int)z)))
#define unhold_windows ((int (*)())global[UNHOLD_WINDOWS])
#define unstop_all_windows(x, y) ((void) (global[FUNC_UNSTOP_ALL_WINDOWS]((char)x, (char *)y)))
#define reset_line_cnt(x, y, z) ((void) (global[RESET_LINE_CNT]((Window *)x, (char *)y, (int)z)))
#define toggle_stop_screen(x, y) ((void) (global[TOGGLE_STOP_SCREEN]((char)x, (char *)y)))
#define flush_everything_being_held(x) ((void) (global[FLUSH_EVERYTHING_BEING_HELD]((Window *)x)))
#define unhold_a_window(x) ((int) (global[UNHOLD_A_WINDOW]((Window *)x)))
#define recalculate_window_cursor(x) ((void) (global[RECALCULATE_WINDOW_CURSOR]((Screen *)x)))
#define make_window_current(x) ((void) (global[MAKE_WINDOW_CURRENT]((Window *)x)))
#define clear_scrollback(x) ((void) (global[CLEAR_SCROLLBACK]((Window *)x)))

#define set_display_target(x, y) ((void) (global[SET_DISPLAY_TARGET]((const char *)x,(unsigned long)y))) 
#define save_display_target(x, y) ((void) (global[SAVE_DISPLAY_TARGET]((const char **)x, (unsigned long *)y)))
#define restore_display_target(x, y) ((void) (global[RESTORE_DISPLAY_TARGET]((const char *)x, (unsigned long)y)))
#define reset_display_target ((void (*)())global[RESET_DISPLAY_TARGET])

#define build_status(x, y, z) ((void) (global[BUILD_STATUS]((Window *)x, (char *)y, (int)z)))



#define do_hook ((int (*)())global[HOOK])

/* input.c */
#define update_input(x) ((void) (global[FUNC_UPDATE_INPUT]((int)x)))
#define cursor_to_input ((void (*)(void)) global[CURSOR_TO_INPUT])
#define set_input(x) ((void) (global[SET_INPUT]((char *)x)))
#define get_input ((char * (*)(void)) (global[GET_INPUT]))
#define get_input_prompt ((char * (*)(void)) (global[GET_INPUT_PROMPT]))
#define set_input_prompt(x, y, z) ((void) (global[SET_INPUT_PROMPT]((Window *)x, (char *)y, (int)z)))
#define addtabkey(x, y, z) ((void) (global[ADDTABKEY]((char *)x, (char *)y, (int)z)))
#define gettabkey(x, y, z) ((NickTab *) (global[GETTABKEY]((int)x, (int)y, (char *)z)))
#define getnextnick(x, y, z, a) ((NickTab *) (global[GETNEXTNICK]((int)x, (char *)y, (char *)z, (char *)a)))
#define getchannick(x, y) ((char *) (global[GETCHANNICK]((char *)x, (char *)y)))
#define lookup_nickcompletion(x, y) ((NickList *) (global[LOOKUP_NICKCOMPLETION]((ChannelList *)x, (char *)y)))
#define add_completion_type(x, y, z) ((int) (global[ADD_COMPLETION_TYPE]((char *)x, (int)y, (enum completion)z)))

/* names.c */
#define is_channel(x) ((int) (global[IS_CHANNEL]((char *)x)))
#define make_channel(x) ((char *) (global[MAKE_CHANNEL]((char *)x)))
#define is_chanop(x, y) ((int) (global[IS_CHANOP]((char *)x, (char *)y)))
#define im_on_channel(x, y) ((int) (global[IM_ON_CHANNEL]((char *)x, (int)y)))
#define is_on_channel(x, y, z) ((int) (global[IS_ON_CHANNEL]((char *)x, (int)y, (char *)z)))
#define add_channel(x, y, z) ((ChannelList *) (global[ADD_CHANNEL]((char *)x, (int)y, (int)z)))
#define add_to_channel(a, b, c, d, e, f, g, h, i, j) ((ChannelList *) (global[ADD_TO_CHANNEL]((char *)a, (char *)b, (int)c, (int)d, (int)e, (char *)f, (char *)g, (char *)h, (int)i, (int)j)))
#define get_channel_key(x, y) ((char *) (global[GET_CHANNEL_KEY]((char *)x, (int)y)))
#define recreate_mode(x) ((char *) (global[FUNC_RECREATE_MODE]((ChannelList *)x)))
#define compress_modes(x, y, z, a) ((char *) (global[FUNC_COMPRESS_MODES]((ChannelList *)x, (int)y, (char *)z, (char *)a)))
#define got_ops(x, y) ((int) (global[FUNC_GOT_OPS]((int)x, (ChannelList *)y)))
#define get_channel_bans(x, y, z) ((char *) (global[GET_CHANNEL_BANS]((char *)x, (int)y, (int)z)))
#define get_channel_mode(x, y) ((char *) (global[GET_CHANNEL_MODE]((char *)x, (int)y)))
#define clear_bans(x) ((void) (global[CLEAR_BANS]((ChannelList *)x)))
#define remove_channel(x, y) ((void) (global[REMOVE_CHANNEL]((char *)x, (int)y)))
#define remove_from_channel(a, b, c, d, e) ((void) (global[REMOVE_FROM_CHANNEL]((char *)a, (char *)b, (int)c, (int)d, (char *)e)))
#define rename_nick(x, y, z) ((void) (global[RENAME_NICK]((char *)x, (char *)y, (int)z)))
#define get_channel_oper(x, y) ((int) (global[GET_CHANNEL_OPER]((char *)x, (int)y)))
#define get_channel_halfop(x, y) ((int) (global[GET_CHANNEL_HALFOP]((char *)x, (int)y)))
#define get_channel_voice(x, y) ((int) (global[GET_CHANNEL_VOICE]((char *)x, (int)y)))
#define fetch_userhost(x, y) ((char *) (global[FETCH_USERHOST]((int)x, (char *)y)))
#define create_channel_list(x) ((char *) (global[CREATE_CHANNEL_LIST]((Window *)x)))
#define flush_channel_stats ((void (*)(void)) (global[FLUSH_CHANNEL_STATS]))
#define lookup_channel(x, y, z) ((ChannelList *) (global[LOOKUP_CHANNEL]((char *)x, (int)y, (int)z)))

/* hash.c */
#define find_nicklist_in_channellist(x, y, z) ((NickList *) (global[FIND_NICKLIST_IN_CHANNELLIST]((char *)x, (ChannelList *)y, (int)z)))
#define add_nicklist_to_channellist(x, y) ((void) (global[ADD_NICKLIST_TO_CHANNELLIST]((NickList *)x, (ChannelList *)y)))
#define next_nicklist(x, y) ((NickList *) (global[NEXT_NICKLIST] ((ChannelList *)x, (NickList *)y)))
#define next_namelist(x, y, z) ((List *) (global[NEXT_NAMELIST]((HashEntry *)x, (List *)y, (unsigned int)z)))
#define add_name_to_genericlist(x, y, z) ((void) (global[ADD_NAME_TO_GENERICLIST]((char *)x, (HashEntry *)y, (unsigned int)z)))
#define find_name_in_genericlist(x, y, z, a) ((List *) (global[FIND_NAME_IN_GENERICLIST]((char *)x, (HashEntry *)y, (unsigned int)z, (int)a)))
#define add_whowas_userhost_channel(x, y) ((void) (global[ADD_WHOWAS_USERHOST_CHANNEL]((WhowasList *)x, (WhowasWrapList *)y)))
#define find_userhost_channel(x, y, z, a) ((WhowasList *) (global[FIND_USERHOST_CHANNEL]((char *)x, (char *)y, (int)z, (WhowasWrapList *)a)))
#define next_userhost(x, y) ((WhowasList *) (global[NEXT_USERHOST]((WhowasWrapList *)x, (WhowasList *)y)))
#define sorted_nicklist(x, y) ((NickList *) (global[SORTED_NICKLIST]((ChannelList *)x, (int)y)))
#define clear_sorted_nicklist(x) ((void)(global[CLEAR_SORTED_NICKLIST](x)))
#define add_name_to_floodlist(a, b, c, d, e) ((Flooding *) (global[ADD_NAME_TO_FLOODLIST]((char *)a, (char *)b, (char *)c, (HashEntry *)d, (unsigned int)e)))
#define find_name_in_floodlist(a, b, c, d, e) ((Flooding *) (global[FIND_NAME_IN_FLOODLIST]((char *)a, (char *)b, (HashEntry *)c, (unsigned int)d, (int)e)))

#define remove_oldest_whowas_hashlist(x, y, z) ((int) (global[REMOVE_OLDEST_WHOWAS_HASHLIST]((WhowasWrapList *)x, (time_t)y, (int)z)))



/* cset.c fset.c vars.c set string and set int ops */
#define fget_string_var(x) ((char *)(global[FGET_STRING_VAR]((int)x))) 
#define fset_string_var(x, y) ((void) (global[FSET_STRING_VAR]((int)x, (char *)y)))
#define get_wset_string_var(x, y) ((char *) (global[GET_WSET_STRING_VAR]((WSet *)x, (char *)y)))
#define set_wset_string_var(x, y, z) ((void) (global[SET_WSET_STRING_VAR]((WSet *)x, (int)y, (char *)z)))
#define get_cset_int_var(x, y) ((int) (global[GET_CSET_INT_VAR]((CSetList *)x, (int)y)))
#define set_cset_int_var(x, y, z) ((void) (global[SET_CSET_INT_VAR]((CSetList *)x, (int)y, (int)z)))
#define get_cset_str_var(x, y) ((char *) (global[GET_CSET_STR_VAR]((CSetList *)x, (int)y)))
#define set_cset_str_var(x, y, z) ((void) (global[SET_CSET_STR_VAR]((CSetList *)x, (int)y, (char *)z)))

#define get_dllint_var(x) ((int) (global[GET_DLLINT_VAR]((char *)x)))
#define set_dllint_var(x, y) ((void) (global[SET_DLLINT_VAR]((char *)x, (unsigned int)y)))
#define get_dllstring_var(x) ((char *) (global[GET_DLLSTRING_VAR]((char *)x)))
#define set_dllstring_var(x, y) ((void) (global[SET_DLLSTRING_VAR]((char *)x, (char *)y)))

#define get_int_var(x) ((int) (global[GET_INT_VAR]((int)x)))
#define set_int_var(x, y) ((void) (global[SET_INT_VAR]((int)x, (int)y)))
#define get_string_var(x) ((char *) (global[GET_STRING_VAR]((int)x)))
#define set_string_var(x, y) ((void) (global[SET_STRING_VAR]((int)x, (char *)y)))


/* module.c */
#define add_module_proc(x, y, z, a, b, c, d, e) ((int)(global[ADD_MODULE_PROC](x, y, z, a, b, c, d, e)))
#define remove_module_proc(a, b, c, d) ((int) (global[REMOVE_MODULE_PROC]((int)a, (char *)b, (char *)c, (char *)d)))


/* timer.c */
#define add_timer(x, y, z, n, f, a, b, w, c) ((char *) (global[ADD_TIMER]((int)x, (char *)y, (double)z, (long)n, (int (*) (void *))f, (char *)a, (char *)b, (Window *)w, (char *)c)))
#define delete_timer(x) ((int) (global[DELETE_TIMER]((char *)x)))
#define delete_all_timers ((int (*)(void)) (global[DELETE_ALL_TIMERS]))



/* server.c */
#define send_to_server ((void (*)()) global[SEND_TO_SERVER])
#define queue_send_to_server ((void (*)()) global[QUEUE_SEND_TO_SERVER])
#define my_send_to_server ((void (*)()) global[MY_SEND_TO_SERVER])
#define get_connected(x, y) ((void) (global[GET_CONNECTED]((int)x, (int)y)))
#define connect_to_server_by_refnum(x, y) ((int) (global[CONNECT_TO_SERVER_BY_REFNUM]((int)x, (int)y)))
#define close_server(x, y) ((void) (global[CLOSE_SERVER]((int)x, (char *)y)))
#define is_server_connected(x) ((int) (global[IS_SERVER_CONNECTED]((int)x)))
#define flush_server ((void (*)(void)) global[FLUSH_SERVER])
#define server_is_connected(x, y) ((int) (global[SERVER_IS_CONNECTED]((int)x, (int)y)))
#define is_server_open(x) ((int) (global[IS_SERVER_OPEN]((int)x)))
#define close_all_server ((void (*)(void)) global[CLOSE_ALL_SERVER])

#define read_server_file(x) ((int) (global[READ_SERVER_FILE]((char *)x)))
#define add_to_server_list(a, b, c, d, e, f, g) ((void) (global[ADD_TO_SERVER_LIST]((char *)a, (int)b, (char *)c, (char *)d, (char *)e, (int)f, (int)g)))
#define build_server_list(x) ((int) (global[BUILD_SERVER_LIST]((char *)x)))
#define display_server_list() ((void) (global[DISPLAY_SERVER_LIST]()))
#define create_server_list(x) ((char *) (global[CREATE_SERVER_LIST]((char *)x)))
#define parse_server_info(a, b, c, d, e) ((void) (global[PARSE_SERVER_INFO]((char *)a, (char **)b, (char **)c, (char **)d, (char **)e)))
#define server_list_size ((int (*)(void)) global[SERVER_LIST_SIZE])

#define find_server_refnum(x, y) ((int) (global[FIND_SERVER_REFNUM]((char *)x, (char **)y)))
#define find_in_server_list(x, y) ((int) (global[FIND_IN_SERVER_LIST]((char *)x, (int)y)))
#define parse_server_index(x) ((int) (global[PARSE_SERVER_INDEX]((char *)x)))

#define set_server_redirect(x, y) ((void) (global[SET_SERVER_REDIRECT]((int)x, (char *)y)))
#define get_server_redirect(x) ((char *) (global[GET_SERVER_REDIRECT]((int)x)))
#define check_server_redirect(x) ((int) (global[CHECK_SERVER_REDIRECT]((char *)x)))
#define fudge_nickname(x, y) ((void) (global[FUDGE_NICKNAME]((int)x, (int)y)))
#define reset_nickname(x) ((void) (global[RESET_NICKNAME]((int)x)))

#define set_server_cookie(x, y) ((void) (global[SET_SERVER_COOKIE]((int)x, (char *)y)))
#define set_server_flag(x, y, z) ((void) (global[SET_SERVER_FLAG]((int)x, (int)y, (int)z)))
#define set_server_motd(x, y) ((void) (global[SET_SERVER_MOTD]((int)x, (int)y)))
#define set_server_operator(x, y) ((void) (global[SET_SERVER_OPERATOR]((int)x, (int)y)))
#define set_server_itsname(x, y) ((void) (global[SET_SERVER_ITSNAME]((int)x, (char *)y)))
#define set_server_version(x, y) ((void) (global[SET_SERVER_VERSION]((int)x, (int)y)))
#define set_server_lag(x, y) ((void) (global[SET_SERVER_LAG]((int)x, (int)y)))
#define set_server_password(x, y) ((char *) (global[SET_SERVER_PASSWORD]((int)x, (char *)y)))
#define set_server_nickname(x, y) ((void) (global[SET_SERVER_NICKNAME]((int)x, (char *)y)))
#define set_server2_8(x, y) ((void) (global[SET_SERVER2_8]((int)x, (int)y)))
#define set_server_away(x, y, z) ((void) (global[SET_SERVER_AWAY]((int)x, (char *)y, (int)z)))

#define get_server_cookie(x) ((char *) (global[GET_SERVER_COOKIE]((int)x)))
#define get_server_nickname(x) ((char *) (global[GET_SERVER_NICKNAME]((int)x)))
#define get_server_name(x) ((char *) (global[GET_SERVER_NAME]((int)x)))
#define get_server_itsname(x) ((char *) (global[GET_SERVER_ITSNAME]((int)x)))
#define get_server_motd(x) ((int) (global[GET_SERVER_MOTD]((int)x)))
#define get_server_operator(x) ((int) (global[GET_SERVER_OPERATOR]((int)x)))
#define get_server_version(x) ((int) (global[GET_SERVER_VERSION]((int)x)))
#define get_server_flag(x, y) ((int) (global[GET_SERVER_FLAG]((int)x, (int)y)))
#define get_possible_umodes(x) ((char *) (global[GET_POSSIBLE_UMODES]((int)x)))
#define get_server_port(x) ((int) (global[GET_SERVER_PORT]((int)x)))
#define get_server_lag(x) ((int) (global[GET_SERVER_LAG]((int)x)))
#define get_server2_8(x) ((int) (global[GET_SERVER2_8]((int)x)))
#define get_umode(x) ((char *) (global[GET_UMODE]((int)x)))
#define get_server_away(x) ((char *) (global[GET_SERVER_AWAY]((int)x)))
#define get_server_network(x) ((char *) (global[GET_SERVER_NETWORK]((int)x)))
#define get_pending_nickname(x) ((char *) (global[GET_PENDING_NICKNAME]((int)x)))
#define server_disconnect(x, y) ((void) (global[SERVER_DISCONNECT]((int)x, (char *)y)))

#define get_server_list ((Server * (*)(void)) global[GET_SERVER_LIST])
#define get_server_channels(x) ((ChannelList *) (global[GET_SERVER_CHANNELS]((int) x)))

#define set_server_last_ctcp_time(x, y) ((void) (global[SET_SERVER_LAST_CTCP_TIME]((int)x, (time_t)y)))
#define get_server_last_ctcp_time(x) ((time_t) (global[GET_SERVER_LAST_CTCP_TIME]((int)x)))
#define set_server_trace_flag(x, y) ((void) (global[SET_SERVER_TRACE_FLAG]((int)x, (int)y)))
#define get_server_trace_flag(x) ((int) (global[GET_SERVER_TRACE_FLAG]((int)x)))
#define get_server_read(x) ((int) (global[GET_SERVER_READ]((int)x)))
#define get_server_linklook(x) ((int) (global[GET_SERVER_LINKLOOK]((int)x)))
#define set_server_linklook(x, y) ((void) (global[SET_SERVER_LINKLOOK]((int)x, (int)y)))
#define get_server_stat_flag(x) ((int) (global[GET_SERVER_STAT_FLAG]((int)x)))
#define set_server_stat_flag(x, y) ((void) (global[SET_SERVER_STAT_FLAG]((int)x, (int)y)))
#define get_server_linklook_time(x) ((time_t) (global[GET_SERVER_LINKLOOK_TIME]((int)x)))
#define set_server_linklook_time(x, y) ((void) (global[SET_SERVER_LINKLOOK_TIME]((int)x, (time_t)y)))
#define get_server_trace_kill(x) ((int) (global[GET_SERVER_TRACE_KILL]((int)x)))
#define set_server_trace_kill(x, y) ((void) (global[SET_SERVER_TRACE_KILL]((int)x, (int)y)))
#define add_server_channels(x, y) ((void) (global[ADD_SERVER_CHANNELS]((int)x, (ChannelList *)y)))
#define set_server_channels(x, y) ((void) (global[SET_SERVER_CHANNELS]((int)x, (ChannelList *)y)))
#define send_msg_to_channels(x, y, z) ((void) (global[SEND_MSG_TO_CHANNELS]((ChannelList *)x, (int)y, (char *)z)))
#define send_msg_to_nicks(x, y, z) ((void) (global[SEND_MSG_TO_CHANNELS]((NickList *)x, (int)y, (char *)z)))
#define is_server_queue() ((int) (global[IS_SERVER_QUEUE]()))


/* sockets */
#define add_socketread(a, b, c, d, x, y) ((int) (global[ADD_SOCKETREAD]((int)a, (int)b, (unsigned long)c, (char *)d, (void *)x, (void *)y)))
#define add_sockettimeout(x, y, z) ((int) (global[ADD_SOCKETTIMEOUT]((int)x, (time_t)y, (void *)z)))
#define close_socketread(x) ((void) (global[CLOSE_SOCKETREAD]((int)x)))
#define get_socket(x) ((SocketList *) (global[GET_SOCKET]((int)x)))
#define set_socketflags(x, y) ((void) (global[SET_SOCKETFLAGS]((int)x, (unsigned long)y)))
#define get_socketflags(x) ((unsigned long) (global[GET_SOCKETFLAGS]((int)x)))
#define check_socket(x) ((int) (global[CHECK_SOCKET]((int)x)))
#define read_sockets(x, y, z) ((int) (global[READ_SOCKETS]((int)x, (char *)y, (int)z)))
#define write_sockets(x, y, z, a) ((int) (global[WRITE_SOCKETS]((int)x, (char *)y, (int)z, (int)a)))
#define get_max_fd() ((int) (global[GET_MAX_FD]()))
#define new_close(x) ((int) (global[NEW_CLOSE]((int)x)))
#define new_open(x) ((int) (global[NEW_OPEN]((int)x)))
#define dgets(x, y, z, l, m) ((int) (global[DGETS]((char *)x, (int)y, (int)z, (int)l, (void *)m)))
#define get_socketinfo(a) ((void *) (global[GET_SOCKETINFO]((int)a)))
#define set_socketinfo(a, b) ((void) (global[SET_SOCKETINFO]((int)a, (void *)b)))
#define set_socket_write(a) ((int) (global[SET_SOCKETWRITE]((int)a)))


/* flood.c */
#define is_other_flood(x, y, z, a) ((int) (global[IS_OTHER_FLOOD]((ChannelList *)x, (NickList *)y, (int)z, (int *)a)))
#define check_flooding(x, y, z, a) ((int) (global[CHECK_FLOODING]((char *)x, (int)y, (char *)z, (char *)a)))
#define flood_prot(x, y, z, a, b, c) ((int) (global[FLOOD_PROT]((char *)x, (char *)y, (char *)z, (int)a, (int)b, (char *)c)))

/* expr.c */
#define next_unit(a, b, c, d) ((char *) (global[NEXT_UNIT]((char *)a, (char *)b, (int *)c, (int)d)))
#define parse_inline(x, y, z) ((char *) (global[PARSE_INLINE]((char *)x, (char *)y, (int *)z)))
#define expand_alias(a, b, c, d) ((char *) (global[EXPAND_ALIAS]((char *)a, (char *)b, (int *)c, (char **)d)))
#define alias_special_char(a, b, c, d, e) ((char *) (global[ALIAS_SPECIAL_CHAR]((char **)a, (char *)b, (char *)c, (char *)d, (int *)e))) 
#define parse_line(a, b, c, e, f, g) ((void) (global[PARSE_LINE]((char *)a, (char *)b, (char *)c, (int)e, (int)f, (int)g)))
#define parse_command(a, b, c) ((void) (global[PARSE_COMMAND_FUNC]((char *)a, (int)b, (char *)c)))
#define make_local_stack(a) ((void) (global[MAKE_LOCAL_STACK]((char *)a)))
#define destroy_local_stack() ((void (*)(void)) (global[DESTROY_LOCAL_STACK]))()


/* dcc.c */
#define dcc_create(a, b, c, d, e, f, g, h) ((DCC_int *) (global[DCC_CREATE_FUNC]((char *)a, (char *)b, (char *)c, (unsigned long)d, (int)e, (int)f, (unsigned long)g, (void (*) (int))h)))
#define find_dcc(a, b, c, d, e, f, g) ((SocketList *) (global[FIND_DCC_FUNC]((char *)a, (char *)b, (char *)c, (int)d, (int)e, (int)f, (int)g)))
#define erase_dcc_info ((void (*)()) global[ERASE_DCC_INFO])
#define add_dcc_bind(a, x, b, c, d, e, f) ((int) (global[ADD_DCC_BIND]((char *)a, (char *)x, (void *)b,(void *)c,(void *)d,(void *)e,(void *)f)))
#define remove_dcc_bind(x, y) ((int) (global[REMOVE_DCC_BIND]((char *)x, (int)y)))
#define remove_all_dcc_binds(x) ((int) (global[REMOVE_ALL_DCC_BINDS])((char *)x))
#define get_active_count ((int (*)(void)) (global[GET_ACTIVE_COUNT]))
#define get_num_queue ((int (*)(void)) (global[GET_NUM_QUEUE]))
#define add_to_queue(x, y, z) ((int) (global[ADD_TO_QUEUE]((char *)x, (char *)y, (pack *)z)))
#define dcc_filesend(x, y) ((void) (global[DCC_FILESEND]((char *)x, (char *)y)))
#define dcc_resend(x, y) ((void) (global[DCC_RESEND]((char *)x, (char *)y)))

/* irc.c */
#define irc_exit ((void (*)())  (global[IRC_EXIT_FUNC]))
#define io(x) ((void) (global[IRC_IO_FUNC]((char *)x)))

/* commands.c */
#define find_command(x, y) ( (IrcCommand *)(global[FIND_COMMAND_FUNC]((char *)x, (int *)y)))

#define lock_stack_frame() ((void (*)(void)) (global[LOCK_STACK_FRAME]))()
#define unlock_stack_frame() ((void (*) (void)) (global[UNLOCK_STACK_FRAME]))()

/* who.c */
#define userhostbase ((void (*)())global[USERHOSTBASE])
#define isonbase ((void (*)())global[ISONBASE])
#define whobase ((void (*)())global[WHOBASE])

#define add_to_window_list(x, y) ((int) (global[ADD_TO_WINDOW_LIST]((struct ScreenStru *)x, (Window *)y)))

/*
 * Rest of these are all variables of various sorts.
 */

#ifndef MAIN_SOURCE

#define nickname ((char *) *global[NICKNAME])
#define irc_version ((char *) *global[IRC_VERSION])

#define from_server (*(int *)global[FROM_SERVER])
#define connected_to_server ((int) *((int *)global[CONNECTED_TO_SERVER]))
#define primary_server ((int) *((int *)global[PRIMARY_SERVER]))
#define parsing_server_index ((int) *((int *)global[PARSING_SERVER_INDEX]))
#define now ((time_t) *((time_t *)global[NOW]))
#define start_time ((time_t) *((time_t *)global[START_TIME]))
#define idle_time() ((time_t) *((time_t *)global[IDLE_TIME]()))

#define loading_global ((int) *((int *)global[LOADING_GLOBAL]))
#define target_window (*((Window **)global[TARGET_WINDOW]))
#define current_window (*((Window **)global[CURRENT_WINDOW]))
#define invisible_list (*((Window **)global[INVISIBLE_LIST]))
#define main_screen (*((Screen **)global[MAIN_SCREEN]))
#define last_input_screen (*((Screen **)global[LAST_INPUT_SCREEN]))
#define output_screen (*((Screen **)global[OUTPUT_SCREEN]))
#define screen_list (*((Screen **)global[SCREEN_LIST]))
#define irclog_fp (*((FILE **)global[IRCLOG_FP]))
#define dll_functions (*((BuiltInDllFunctions **)global[DLL_FUNCTIONS]))
#define dll_numeric (*((NumericFunction **)global[DLL_NUMERIC]))
#define dll_commands (*((IrcCommandDll **)global[DLL_COMMANDS]))
#define dll_variable (*((IrcVariableDll **)global[DLL_VARIABLE]))
#define dll_ctcp (*((CtcpEntryDll **)global[DLL_CTCP]))
#define dll_window (*((WindowDll **)global[DLL_WINDOW]))
#define window_display ((int) *((int *)global[WINDOW_DISPLAY]))
#define status_update_flag ((int) *((int *)global[STATUS_UPDATE_FLAG]))
#define tabkey_array (*((NickTab **)global[TABKEY_ARRAY]))
#define autoreply_array (*((NickTab *)global[AUTOREPLY_ARRAY]))
#define identd ((int) *((int *)global[IDENTD_SOCKET]))
#define doing_notice ((int) *((int *)global[DOING_NOTICE]))
#define last_sent_msg_body (*((char **)global[LAST_SENT_MSG_BODY]))
#define sent_nick (*((char **)global[SENT_NICK]))

#define default_output_function (*(void (**)(char *))global[DEFAULT_OUTPUT_FUNCTION])

#define serv_open_func (*(int (**)(int, unsigned long, int))global[SERV_OPEN_FUNC])
#define serv_input_func (*(int (**)(int, char *, int, int, int))global[SERV_INPUT_FUNC])
#define serv_output_func (*(int (**)(int, int, char *, int))global[SERV_OUTPUT_FUNC])
#define serv_close_func (*(int (**)(int, unsigned long, int))global[SERV_CLOSE_FUNC])
#if 0
#define check_ext_mail_status (*(int (**)()) global[CHECK_EXT_MAIL_STATUS])
#define check_ext_mail (*(char *(**)())global[CHECK_EXT_MAIL])
#endif

#ifdef WANT_TCL
#define tcl_interp ((Tcl_Interp *)((Tcl_Interp **)(global[VAR_TCL_INTERP])))
#else
#define tcl_interp NULL
#endif
#else
#undef get_time
#define get_time(a) BX_get_time(a)
#endif /* MAIN_SOURCE */

#ifdef GUI
#ifndef MAIN_SOURCE
#define lastclicklinedata ((char *) *global[LASTCLICKLINEDATA])
#define contextx ((int) *((int *)global[CONTEXTX]))
#define contexty ((int) *((int *)global[CONTEXTY]))
#define guiipc ((int) *((int *)global[GUIIPC]))
#endif
#define gui_mutex_lock() ((void (*)(void)) global[GUI_MUTEX_LOCK])()
#define gui_mutex_unlock() ((void (*)(void))global[GUI_MUTEX_UNLOCK])()
#endif

#endif /* WTERM_C || STERM_C */
#endif
