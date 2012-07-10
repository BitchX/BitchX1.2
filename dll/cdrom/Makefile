SHELL = /bin/sh

srcdir = .

top_srcdir = ../..
topdir = /home/dan/BitchX
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

local_dir = $(HOME)

# Where the BitchX binary will be installed.
# "make install" will compile and install the program.
INSTALL_IRC = ${bindir}/$(_VERSION_)$(EXEEXT)

# Where the BitchX library will be. Generally this is the place that
# you put the scripts, help pages and translation tables. It is
# very important that you set this correctly.
IRCLIB = ${libdir}/bx

CC = gcc
DEFS = -I. -I$(topdir)/include -I$(top_srcdir)/include -I$(srcdir) -I$(srcdir)/include
LIBS =  -ldl -lncurses -lssl -lcrypto -lresolv -lm -lcrypt

# Tcl library.
TCL_LIBS = 

# These are for Tcl support.
TCL_OBJS = 
# You don't have the following, so you'll want to leave this blank.
TCL_SRCS = 

# Set this to -g if you want to be able to debug the client, otherwise
# use -O to have the compiler do some optimization instead.
CFLAGS =  -g -O2 -fno-strict-aliasing -Wall

# Set this to -s if you want the binary to be stripped.
LDFLAGS = 

# These are for the cd device player.
CD_SRCS = 
CD_OBJS = 

# This is the executable suffix for the target operating system.
EXEEXT = 

# Extra files.
DEFAULT_CTOOLZ_DIR = ~/.BitchX
DEFAULT_MSGLOGFILE = BitchX.away
DEFAULT_BITCHX_HELP_FILE = BitchX.help
DEFAULT_SCRIPT_HELP_FILE = BitchX.help
DEFAULT_BITCHX_KICK_FILE = BitchX.kick
DEFAULT_BITCHX_QUIT_FILE = BitchX.quit
DEFAULT_BITCHX_IRCNAME_FILE = BitchX.ircnames

# Full path of the directory for BitchX help files.
HELPDIR = $(IRCLIB)/help

# Full path of the directory for the BitchX scripts.
INSTALL_SCRIPT = $(IRCLIB)/script

# Default setting for IRCPATH where BitchX will look for
# its script files if the environment variable is undefined.
# Usually, this should contain the same path as used for INSTALL_SCRIPT in
# the Makefile, but it can contain multiple path elements
# separated by colons. The path MUST lead to an existing directory,
# because the 'global' script is expected to be found there.
IRCPATH = $(DEFAULT_CTOOLZ_DIR):$(DEFAULT_CTOOLZ_DIR)/plugins:.:$(PLUGINDIR):$(INSTALL_SCRIPT):$(IRCLIB)

# Path for TRANSLATION variable.
TRANSLATION_PATH = $(IRCLIB)/translation

# This is where the optional plugins will be copied to.
PLUGINDIR = $(IRCLIB)/plugins

# Plugin flags.
SHLIB_LD = gcc -shared
SHLIB_CFLAGS = -fPIC
SHLIB_SUFFIX = .so

# This command will be used to install the BitchX files on Win32/OS2EMX
# systems.
WINNT_INSTALL = 

# This program allows you to use screen/xterm's to put new BitchX windows
# on new screen/xterm windows.
INSTALL_WSERV = $(IRCLIB)/wserv$(EXEEXT)

# This program allows you to screen BitchX and reattach to it later.
INSTALL_SCRBX = ${bindir}/scr-bx$(EXEEXT)

# Set gzip and bzip2 options.
GZIP_ENV = --best
BZIP2 = -9 --repetitive-best

# Standard programs.
RM = rm -f
LN = ln -s
CP = cp -f
MV = mv -f

INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644

VERSION = BitchX-1.2c01-svn
_VERSION_ = BitchX

MAKE_BIN = gmake
MAKE = $(MAKE_BIN) $(MFLAGS)
MFLAGS = \
	'local_dir=$(HOME)'			\
	'INSTALL_IRC=$(INSTALL_IRC)'		\
	'IRCLIB=$(IRCLIB)'			\
	'CC=$(CC)'				\
	'CFLAGS=$(CFLAGS)'			\
	'HELPDIR=$(HELPDIR)'			\
        'INSTALL_WSERV=$(INSTALL_WSERV)'	\
	'IRCPATH=$(IRCPATH)'			\
	'TRANSLATION_PATH=$(TRANSLATION_PATH)'	\
	'LDFLAGS=$(LDFLAGS)'			\
	'LIBS=$(LIBS)'				\
	'LN=$(LN)'				\
	'RM=$(RM)'				\
	'TCL_SRCS=$(TCL_SRCS)'			\
	'TCL_OBJS=$(TCL_OBJS)'			\
	'CD_PLAY=$(CD_PLAY)'			\
	'CD_SRCS=$(CD_SRCS)'			\
	'CD_OBJS=$(CD_OBJS)'			\
	'TCL_LIBS=$(TCL_LIBS)'			\
	'PLUGINDIR=$(PLUGINDIR)'		\
	'_VERSION_=$(_VERSION_)'		\
	'VERSION=$(VERSION)'			\
	'INSTALL_DATA=$(INSTALL_DATA)'		\
	'INSTALL_SCRIPT=$(INSTALL_SCRIPT)'	\
	'EXEEXT=$(EXEEXT)'			\
	'SHLIB_CFLAGS=$(SHLIB_CFLAGS)'		\
	'SHLIB_SUFFIX=$(SHLIB_SUFFIX)'

## Makefile starts here.

PLUGIN_NAME = cdrom

all: Makefile cdrom$(SHLIB_SUFFIX)

Makefile: Makefile.in
	cd $(topdir) \
	  && ./config.status

cdrom.o: $(srcdir)/cdrom.c
	$(CC) $(DEFS) $(SHLIB_CFLAGS) $(CFLAGS) -c $(srcdir)/cdrom.c

cdrom$(SHLIB_SUFFIX): cdrom.o ../dllinit.o
	$(SHLIB_LD) cdrom.o ../dllinit.o ${SHLIB_CFLAGS} -o cdrom$(SHLIB_SUFFIX)

clean:
	$(RM) *~ *.o *.a *.dll cdrom$(SHLIB_SUFFIX) *.def .#*

distclean: clean
	$(RM) Makefile

install:
	$(INSTALL) $(PLUGIN_NAME)$(SHLIB_SUFFIX) $(DESTDIR)$(PLUGINDIR)
