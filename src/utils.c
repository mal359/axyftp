/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "axyftp.h"
#include "utils.h"

char* WXnewstring(char* orig){
  char* ret;

  if(orig==NULL)return NULL;

  if((ret=strdup(orig))==NULL){
    perror("AxY FTP");
    exit(1);
  }
  return ret;
}

#include<Xm/Xm.h>
#include<X11/cursorfont.h>

void WXsetLabel(WXwidget label,char* string){
  XmString xms;

  xms=XmStringCreateLocalized(string);
  XtVaSetValues(label,XmNlabelString,xms,NULL);
  XmStringFree(xms);
}

void beep(){
  XBell(XtDisplay(toplevel),100);
}

void busy_cursor(int busy){
  Cursor cursor;
  XSetWindowAttributes at;

  if(busy){
    cursor=XCreateFontCursor(XtDisplay(toplevel),XC_watch);
    at.cursor=cursor;
  } else {
    at.cursor=None;
  }
  XChangeWindowAttributes(XtDisplay(toplevel),XtWindow(toplevel),CWCursor,&at);
  XFlush(XtDisplay(toplevel));
}

void process_events(){
  XEvent e;
/*  XFlush(XtDisplay(toplevel));*/
  XmUpdateDisplay(toplevel);
  while(XEventsQueued(XtDisplay(toplevel),QueuedAfterFlush)){
    XNextEvent(XtDisplay(toplevel),&e);
    XtDispatchEvent(&e);
  }
/*  while(XCheckMaskEvent(XtDisplay(toplevel), ButtonPressMask | 
      ButtonReleaseMask | ButtonMotionMask | 
      PointerMotionMask | KeyPressMask, &e)){
    XtDispatchEvent(&e);
  }
*/   
}
