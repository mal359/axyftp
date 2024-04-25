/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef UTILS_H
#define UTILS_H

#include"axyftp.h"
void WXsetLabel(WXwidget,char*);
void busy_cursor(int);
void beep();
void process_events();
char* WXnewstring(char*);

#include<Xm/Xm.h>
#define WXmalloc(x) XtMalloc(x)
#define WXfree(x) XtFree((char*)(x));
#define WXrealloc(x,y) XtRealloc(x,y)
#define LOOP() XtAppMainLoop(appcontext)

#endif /* UTILS_H */
