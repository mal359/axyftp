/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include "axyftp.h"
#include "pixmaps.h"

#include "xpms/photo16.xpm"
#include "xpms/up.xpm"
#include "xpms/folder.xpm"
#include "xpms/doc.xpm"
#include "xpms/link.xpm"

Pixmap pixmap[NUMTYPES];
Pixmap mask[NUMTYPES];

Pixmap portret;
Pixmap portmask;

int get_pixmap(char** data,Pixmap *pix,Pixmap *mas){
  XpmAttributes attr;
  int result;

  attr.valuemask=XpmExactColors|XpmCloseness;
  attr.exactColors=FALSE;
  attr.closeness=10000;
  result=XpmCreatePixmapFromData(XtDisplay(appdata.status),
                          DefaultRootWindow(XtDisplay(appdata.status)),
			  data,pix,mas,&attr);

  return result;
}


void create_pixmaps(){
  get_pixmap(up_p,&pixmap[0],&mask[0]);
  get_pixmap(folder_p,&pixmap[1],&mask[1]);
  get_pixmap(doc_p,&pixmap[2],&mask[2]);
  get_pixmap(link_p,&pixmap[3],&mask[3]);
  get_pixmap(photo,&portret,&portmask);
}
