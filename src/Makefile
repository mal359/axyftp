# It's 2024. Screw Autotools.

MALLOC = -ljemalloc

AXYFTP_HELP_DIR = /usr/local/share/axyftp/help/

MAKEDEPEND = gcc -M

CC = gcc 

XMAXY_INCLUDE = -I../xmaxy
XMAXY_LIB = ../xmaxy/XmAxy/libXmAxy.a

#DT_INCLUDE = -I../dtwidget
DT_INCLUDE = -I/usr/dt

#DT_LIB = ../dtwidget/Dt/libDtWidget.a
DT_LIB = -L/usr/dt/lib -lDtWidget

#XPM_INCLUDE = -I/usr/X11R6/include
XPM_INCLUDE = -I/usr/include

#XPM_LIB = -L/usr/X11R6/lib -lXpm
XPM_LIB = -lXpm

OTHER_LIB = 
#OTHER_LIB = -lsocket
#OTHER_LIB = -lsocket -lnsl

MOTIF_INCLUDE=-I/usr/include/Xm
#MOTIF_INCLUDE=-I/usr/dt/include

MOTIF_LIB= -lXm
#MOTIF_LIB=-L/usr/dt/lib -lXm

#MOTIF_LIB_STATIC = -L/opt/motif12/lib -Wl,-Bstatic -lXm -Wl,-Bdynamic
#MOTIF_LIB_STATIC = -L/usr/dt/lib -Wl,-Bstatic -lXm -Wl,-Bdynamic

X_INCLUDE=-I/usr/include/X11

X_LIB=-L/usr/lib/x86_64-linux-gnu -lXt -lX11

DEFINES = -DLINUX -DMOTIF -D_GNU_SOURCE


CFLAGS = -O2 -g -fcommon $(DEFINES) $(XMAXY_INCLUDE) $(DT_INCLUDE) \
         $(MOTIF_INCLUDE) $(X_INCLUDE) $(XPM_INCLUDE) \
	 -DAXYFTP_HELP_DIR=\"$(AXYFTP_HELP_DIR)\"


OBJECTS = axyftp.o main_frame.o buttonbar.o arrows.o status.o proto.o panel.o \
          buttons.o dirname.o dirlist.o session_data.o ftp.o utils.o \
	  read_init.o functions.o fileinfo.o dirinfo.o pixmaps.o ftp_xfer.o \
	  dialogs.o session_dialog.o session_general.o session_startup.o \
	  about_dialog.o progress_dialog.o little_dialogs.o prompt_dialogs.o \
	  viewer.o session_advanced.o options_dialog.o options_general.o \
	  options_display.o options_sound.o options_data.o multi.o \
	  options_advanced.o

SOURCES = axyftp.c main_frame.c buttonbar.c arrows.c status.c proto.c panel.c \
          buttons.c dirname.c dirlist.c session_data.c ftp.c utils.c \
	  read_init.c \
	  functions.c fileinfo.c dirinfo.c pixmaps.c ftp_xfer.c dialogs.c \
	  session_dialog.c session_general.c session_startup.c about_dialog.c \
	  progress_dialog.c little_dialogs.c prompt_dialogs.c viewer.c \
	  session_advanced.c options_dialog.c options_general.c \
	  options_display.c options_sound.c options_data.c multi.c \
	  options_advanced.c

HEADERS = axyftp.h main_frame.h buttonbar.h arrows.h status.h proto.h proto.h \
          buttons.h dirname.h dirlist.h session_data.h ftp.h utils.h \
	  read_init.h \
          functions.h fileinfo.h dirinfo.h pixmaps.h ftp_xfer.h dialogs.h \
	  session_dialog.h session_general.h session_startup.h about_dialog.h \
	  progress_dialog.h little_dialogs.h prompt_dialogs.h viewer.h \
	  session_advanced.h options_dialog.h options_general.h \
	  options_display.h options_sound.h options_data.h multi.h \
	  options_advanced.h


all:	$(OBJECTS) 
		$(CC) $(CFLAGS) -o axyftp $(OBJECTS) $(XMAXY_LIB) $(MOTIF_LIB) $(X_LIB) $(XPM_LIB) $(OTHER_LIB) $(MALLOC)


clean:		
		rm -f *~ *.o *.bak core

distclean:	clean 
		rm axyftp
