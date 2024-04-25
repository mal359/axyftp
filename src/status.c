/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"status.h"

#include<Xm/Xm.h>
#include<Xm/List.h>
#include<Xm/ScrolledW.h>
#include<Xm/Text.h>
#include<Xm/Label.h>
#include<Xm/Frame.h>
#include<string.h>

void append_status(char* start){
  int len;

  if(start==NULL) return;
  len=strlen(start);
  if(len && start[--len]=='\r'){
    start[len]='\n';
    start[len+1]='\0';
  }
  if(len && start[--len]=='\r'){
    start[len]='\n';
    start[len+1]='\0';
  }

  len=XmTextGetLastPosition(appdata.status);
  XmTextInsert(appdata.status,len,start);
  len+=strlen(start);
  XmTextSetInsertionPosition(appdata.status,len);
  XmTextShowPosition(appdata.status,len);
  XmUpdateDisplay(toplevel);
  return;
}

Widget create_status(Widget parent){
  Arg args[10];
  Cardinal n;
  Widget status,text;
  Pixel bg;

  n=0;
  XtSetArg(args[n],XmNshadowType,XmSHADOW_ETCHED_OUT);n++;
  XtSetArg(args[n],XmNmarginWidth,3);n++;
  XtSetArg(args[n],XmNmarginHeight,3);n++;
  status=XmCreateFrame(parent,"_status",args,n);
  XtManageChild(status);

  XtVaGetValues(status,XmNbackground,&bg,NULL);

  n=0;
  XtSetArg(args[n],XmNchildType,XmFRAME_WORKAREA_CHILD);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNeditable,False);n++;
  XtSetArg(args[n],XmNscrollHorizontal,False);n++;
  XtSetArg(args[n],XmNcursorPositionVisible,False);n++;
  XtSetArg(args[n],XmNeditMode,XmMULTI_LINE_EDIT);n++;
  XtSetArg(args[n],XmNverifyBell,False);n++;
  XtSetArg(args[n],XmNwordWrap,True);n++;
  XtSetArg(args[n],XmNrows,3);n++;
  text=XmCreateScrolledText(status,"status",args,n);
  XtManageChild(text);

  XtVaSetValues(text,XmNshadowThickness,0,XmNbackground,bg,NULL);

  appdata.status=text;

  return status;

}
