# It's 2024. Screw Autotools.

LIBDIR = /usr/local/lib
INCLUDEDIR= /usr/local/include/XmAxy

MAKEDEPEND = gcc -M

CC = gcc
CFLAGS = -O2 -g -fPIC
	 
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

X_LIB=-lX11 -lXt

SOURCES = List.c Notebook.c Util.c

HEADERS = List.h Notebook.h Util.h 

PHEADERS = $(HEADERS) ListP.h NotebookP.h version.h

OBJECTS = List.o Notebook.o Util.o

LIB_SHARED = libXmAxy.so.0
SYMLINK = libXmAxy.so

LIB_STATIC = libXmAxy.a

DEMO_SRC = demo.c

DEMO_OBJ = demo.o


all:		shared static

shared:		$(OBJECTS)
		$(CC) $(CFLAGS) -shared -Wl,-soname,$(LIB_SHARED) -o $(LIB_SHARED) $(OBJECTS) $(X_LIB) $(XPM_LIB) $(MOTIF_LIB) 
		ln -sf $(LIB_SHARED) $(SYMLINK)

static: 	$(OBJECTS)
		ar rcs $(LIB_STATIC) $(OBJECTS)

install:	cp $(LIB_SHARED) $(LIBDIR)
		cp $(SYMLINK) $(LIBDIR)
		cp $(LIB_STATIC) $(LIBDIR)
		cp $(HEADERS) $(INCLUDEDIRDIR)

clean:		
		rm -f *~ *.o *.bak *.so* *.a a.out demo core

