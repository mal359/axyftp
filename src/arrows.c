/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <stdio.h>
#include <stdlib.h>
#ifdef USE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif

#include "axyftp.h"
#include "utils.h"
#include "multi.h"
#include "functions.h"
#include "dirlist.h"
#include "arrows.h"
#include "proto.h"
#include "little_dialogs.h"

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>

void download_cb(Widget,XtPointer,XtPointer);
void upload_cb(Widget,XtPointer,XtPointer);

void upload_cb(Widget w,XtPointer app,XtPointer call){
  int *selrow;
  int ret;
  char* mask;

  if(appdata.job)return;
  if(!appdata.connected){
    (void)popup_warning_dialog(toplevel,"Not connected");
    return;
  }

  selrow=get_selected_rows(appdata.local.table);
  if(selrow==NULL){
    (void)popup_warning_dialog(toplevel,"Nothing selected");
    return;
  }
  appdata.job=4;
  busy_cursor(True);
  XmUpdateDisplay(toplevel);
  ret=put_files(selrow,get_proto_state(appdata.proto));
  mask=XmTextFieldGetString(appdata.remote.text);
  if(!ret)update_remote(mask);
  free(mask);
  busy_cursor(False);
  appdata.job=0;
  return;
}

void download_cb(Widget w,XtPointer app,XtPointer call){
  int *selrow;
  char* mask;

  if(appdata.job)return;
  if(!appdata.connected){
    (void)popup_warning_dialog(toplevel,"Not connected");
    return;
  }

  selrow=get_selected_rows(appdata.remote.table);
  if(selrow==NULL){
    (void)popup_warning_dialog(toplevel,"Nothing selected");
    return;
  }
  appdata.job=3;
  busy_cursor(TRUE);
  if(!get_files(selrow,get_proto_state(appdata.proto))){
    mask=XmTextFieldGetString(appdata.local.text);
    update_local(mask);
    free(mask);
  }
  free((char*)selrow);
  busy_cursor(FALSE);
  appdata.job=0;
  return;
  /*
  selrow=get_selected_row(appdata.remote.table);
  switch(selrow){
    case MANY:
    case NONE:
      (void)popup_warning_dialog(toplevel,
	  "Exactly one remote file should be selected");
      return;
    default:
      appdata.job=3;
      busy_cursor(True);
      XmUpdateDisplay(toplevel);
      ret=download_file(selrow,get_proto_state(appdata.proto));
      mask=XmTextFieldGetString(appdata.local.text);
      if(!ret)update_local(mask);
      free(mask);
      busy_cursor(False);
      appdata.job=0;
      return;
  }
  */
}

Widget create_arrows(Widget parent){
  Widget arrows;
  Arg args[10];
  Cardinal n;
  Widget left,right;
  XmString label;
  Dimension d;
  Pixel b;

  XtVaGetValues(parent,XmNbackground,&b,NULL);


  n=0;
  XtSetArg(args[n],XmNhorizontalSpacing,4);
  arrows=XmCreateForm(parent,"arrows",args,n);
  XtManageChild(arrows);

  label=XmStringCreateLocalized(" <- ");
  n=0;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNhighlightColor,b);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,49);n++;
  left=XmCreatePushButton(arrows,"<-",args,n);
  XtManageChild(left);
  XmStringFree(label);
  XtVaGetValues(left,XmNwidth,&d,NULL);
  XtVaSetValues(left,XmNheight,d,NULL);

  label=XmStringCreateLocalized(" -> ");
  n=0;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNhighlightColor,b);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,51);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  right=XmCreatePushButton(arrows,"->",args,n);
  XtManageChild(right);
  XmStringFree(label);
  XtVaSetValues(right,XmNheight,d,NULL);

  XtAddCallback(left,XmNactivateCallback,download_cb,NULL);
  XtAddCallback(right,XmNactivateCallback,upload_cb,NULL);

  return arrows;
}
