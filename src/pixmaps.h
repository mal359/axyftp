/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef PIXMAPS_H
#define PIXMAPS_H

#define NUMTYPES 4

#include<X11/xpm.h>
extern Pixmap pixmap[NUMTYPES];
extern Pixmap mask[NUMTYPES];
extern Pixmap portret;
extern Pixmap portmask;

void create_pixmaps();

#endif /*PIXMAPS_H*/

