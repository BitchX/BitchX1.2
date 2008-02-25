/* Define short BitchX version here */
#undef _VERSION_

/* Define long BitchX version here */
#undef VERSION

/* Define BitchX version number here */
#undef VERSION_NUMBER

/* Define this if you have a crypt implementation in -lcrypt */
#undef HAVE_CRYPT

/* Define this if you have the resolv library */
#undef HAVE_RESOLV

/* Define this if tparm is declared in an included header */
#undef TPARM_DECLARED

/* Define this if you have the tparm function in an included lib */
#undef HAVE_TPARM

/* Define this if tputs is declared in an included header */
#undef TPUTS_DECLARED

/* Define this if you have the tputs function in an included lib */
#undef HAVE_TPUTS

/* Define this if you have terminfo support */
#undef HAVE_TERMINFO

/* define this if an unsigned long is 32 bits */
#undef UNSIGNED_LONG32

/* define this if an unsigned int is 32 bits */
#undef UNSIGNED_INT32

/* define this if you are unsure what is is 32 bits */
#undef UNKNOWN_32INT

/* Define this if stpcpy is declared in unistd.h */
#undef STPCPY_DECLARED

/* Define this if getpgid is declared in unistd.h */
#undef GETPGID_DECLARED

/* Define this if killpg is declared in signal.h */
#undef KILLPG_DECLARED

/* Define this if getpass is declared */
#undef GETPASS_DECLARED

/* Define this if errno is declared */
#undef ERRNO_DECLARED

/* Define this if struct linger is declared in sys/socket.h */
#undef STRUCT_LINGER_DECLARED

/* Define this if sun_len is declared in sys/un.h */
#undef HAVE_SUN_LEN

/* Define this if you want QMAIL support */
#undef HAVE_QMAIL

/* Define this if inet_aton is in the system */
#undef HAVE_INET_ATON

/* Define this if bcopy is declared in string.h */
#undef BCOPY_DECLARED

/* Define your maildir here */
#undef UNIX_MAIL

/* Define a list of default servers here */
#undef DEFAULT_SERVER

/* Define this if you want Tcl support */
#undef WANT_TCL

/* Define this if you have tcl.h */
#undef HAVE_TCL_H

/* Define this is the system has SSL support */
#undef HAVE_SSL

/* Define this if you want IPV6 support */
#undef IPV6

/* Define this if you want SOCKS support */
#undef SOCKS

/* Non-blocking type should be one of these */
#undef NBLOCK_POSIX
#undef NBLOCK_BSD
#undef NBLOCK_SYSV

/*
 * Are we doing non-blocking connects? Note: SOCKS support precludes us from
 * using this feature.
 */
#if (defined(NBLOCK_POSIX) || defined(NBLOCK_BSD) || defined(NBLOCK_SYSV)) && !defined(SOCKS)
#define NON_BLOCKING_CONNECTS
#endif

/* Define this if you want CD-ROM support */
#undef WANT_CD

/* Define this if you want GUI support (PM or GTK) */
#undef GUI

/* Defined if Win32 GUI */
#if defined(GUI) && defined(WINNT)
#define WIN32 1
#endif

/* Define this is you want OS/2 PM support */
#undef __EMXPM__

/* Define this if you want GTK support */
#undef GTK

/* Define this if you want imlib support */
#undef USE_IMLIB

/* Define this if you want GNOME support */
#undef USE_GNOME

/* Define this if your ZVT is newer than 1.0.10 */
#undef HAVE_NEW_ZVT

/* Define this if you want ZVT support */
#undef USE_ZVT

/* Define this is you want sound support */
#undef SOUND

/* Define this if you want loadable module support */
#undef HAVE_DLLIB

/*
 * Define this if you have shlib support and want plugin support in BitchX
 * Note: Not all systems support this.
 */
#ifdef HAVE_DLLIB
#define WANT_DLL
#endif

/* The Extra Files */
#undef DEFAULT_CTOOLZ_DIR
#undef DEFAULT_MSGLOGFILE
#undef DEFAULT_BITCHX_HELP_FILE
#undef DEFAULT_SCRIPT_HELP_FILE
#undef DEFAULT_BITCHX_KICK_FILE
#undef DEFAULT_BITCHX_QUIT_FILE
#undef DEFAULT_BITCHX_IRCNAME_FILE

/* Needed defines */
#undef DEFAULT_HELP_PATH
#undef IRCLIB
#undef IRCPATH
#undef SHLIB_SUFFIX
#undef PLUGINDIR
#undef SCRIPT_PATH
#undef WSERV_PATH
#undef TRANSLATION_PATH
