/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <stdio.h>
#include <stdlib.h>
#ifdef USE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif

#include "axyftp.h"
#include "multi.h"
#include "utils.h"
#include "pixmaps.h"
#include "functions.h"

#include <Xm/Xm.h>
#include <Xm/TextF.h>
#include <Xm/SelectioB.h>
#include <Xm/MessageB.h>

static void help_cb(Widget w,XtPointer app,XtPointer call){
  show_help((int)app);
}

static void close_xfer_cb(Widget w,XtPointer app,XtPointer call){
  XtUnmanageChild((Widget)app);
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,3);
}

static void xfer_cb(Widget w,XtPointer app,XtPointer call){
  XtUnmanageChild(w);
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,(int)app);
}

void init_xfer_dialog(Widget dialog,char* file){
  char* p;
  XmString xms;

  p=malloc(strlen(file)+50);
  snprintf(p, sizeof(p), "%s\nalready exists",file);
  xms=XmStringCreateLocalized(p);

  XtVaSetValues(dialog,XmNmessageString,xms,NULL);
  free(p);
  XmStringFree(xms);

  XtManageChild(dialog);
}

Widget create_xfer_dialog(Widget parent){
  Widget dialog,shell;
  XmString ok,cancel,help;
  Cardinal n;
  Arg args[10];

  ok=XmStringCreateLocalized("Overwrite");
  cancel=XmStringCreateLocalized("Resume");
  help=XmStringCreateLocalized("Skip");

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP transfer confirm");n++;
  XtSetArg(args[n],XmNokLabelString,ok);n++;
  XtSetArg(args[n],XmNcancelLabelString,cancel);n++;
  XtSetArg(args[n],XmNhelpLabelString,help);n++;
  XtSetArg(args[n],XmNmessageAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreateQuestionDialog(parent,"xfer",args,n);

  XtAddCallback(dialog,XmNokCallback,xfer_cb,(XtPointer)1);
  XtAddCallback(dialog,XmNcancelCallback,xfer_cb,(XtPointer)2);
  XtAddCallback(dialog,XmNhelpCallback,xfer_cb,(XtPointer)3);

  shell=dialog;
  while(!XtIsWMShell(shell))shell=XtParent(shell);

  XtVaSetValues(shell,XmNdeleteResponse,XmDO_NOTHING,
      XmNmwmFunctions,0,NULL);

  XtAddCallback(shell,XmNdestroyCallback,close_xfer_cb,(XtPointer)dialog);

  XmStringFree(ok);
  XmStringFree(cancel);
  XmStringFree(help);

  return dialog;
}

static void close_delete_cb(Widget w,XtPointer app,XtPointer call){
  XtUnmanageChild((Widget)app);
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,3);
}

static void delete_cb(Widget w,XtPointer app,XtPointer call){
  XtUnmanageChild(w);
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,(int)app);
}

void init_delete_dialog(Widget dialog,char* file){
  char* p;
  XmString xms;

  p=malloc(strlen(file)+50);
  snprintf(p, sizeof(p), "Delete\n%s",file);
  xms=XmStringCreateLocalized(p);

  XtVaSetValues(dialog,XmNmessageString,xms,NULL);
  free(p);
  XmStringFree(xms);

  XtManageChild(dialog);
}
  

Widget create_delete_dialog(Widget parent){
  Widget dialog,shell;
  XmString ok,cancel,help;
  Cardinal n;
  Arg args[10];

  ok=XmStringCreateLocalized("Yes");
  cancel=XmStringCreateLocalized("Yes to all");
  help=XmStringCreateLocalized("No");

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP delete confirm");n++;
  XtSetArg(args[n],XmNokLabelString,ok);n++;
  XtSetArg(args[n],XmNcancelLabelString,cancel);n++;
  XtSetArg(args[n],XmNhelpLabelString,help);n++;
  XtSetArg(args[n],XmNmessageAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreateQuestionDialog(parent,"delete",args,n);

  XtAddCallback(dialog,XmNokCallback,delete_cb,(XtPointer)1);
  XtAddCallback(dialog,XmNcancelCallback,delete_cb,(XtPointer)2);
  XtAddCallback(dialog,XmNhelpCallback,delete_cb,(XtPointer)3);

  shell=dialog;
  while(!XtIsWMShell(shell))shell=XtParent(shell);

  XtVaSetValues(shell,XmNdeleteResponse,XmDO_NOTHING,
      XmNmwmFunctions,0,NULL);

  XtAddCallback(shell,XmNdestroyCallback,close_delete_cb,(XtPointer)dialog);

  XmStringFree(ok);
  XmStringFree(cancel);
  XmStringFree(help);

  return dialog;
}

static void rename_cb(Widget w,XtPointer app,XtPointer call){
  int which;
  XmSelectionBoxCallbackStruct *cbs;
  char* text;
  char* orig;

  which=(int)app;
  cbs=(XmSelectionBoxCallbackStruct*)call;


  if(appdata.job && which==REMOTE){
    XtUnmanageChild(w);
    return;
  }

  if(XmStringGetLtoR(cbs->value,XmFONTLIST_DEFAULT_TAG,&text)){
    XtUnmanageChild(w);
    XtVaGetValues(w,XmNuserData,&orig,NULL);
    if(orig){
      switch(which){
	case LOCAL:
	  busy_cursor(True);
	  if(!rename_local(orig,text)){
	    char* mask=XmTextFieldGetString(appdata.local.text);
	    update_local(mask);
	    free(mask);
	  }
	  if(!appdata.job)busy_cursor(False);
	  break;
	case REMOTE:
	  appdata.job=9;
	  busy_cursor(True);
	  if(!rename_remote(orig,text)){
	    char* mask=XmTextFieldGetString(appdata.remote.text);
	    update_remote(mask);
	    free(mask);
	  }
	  busy_cursor(False);
	  appdata.job=0;
	  break;
       default:
	  break;
      }
    }
    free(text);
  } else {
    XtUnmanageChild(w);
  }
  return;
}

void init_rename_dialog(Widget dialog,char* text,char* orig){
  XmString xms;
  Widget t;

  xms=XmStringCreateLocalized(text);
  XtVaSetValues(dialog,XmNselectionLabelString,xms,
      XmNtextString,NULL,XmNuserData,(XtPointer)orig,NULL);
  XmStringFree(xms);
  
  t=XmSelectionBoxGetChild(dialog,XmDIALOG_TEXT);
  XmTextFieldReplace(t,0,XmTextFieldGetLastPosition(t),orig);

  XtManageChild(dialog);
  XmTextFieldSetSelection(t,0,strlen(orig),CurrentTime);
}
  

Widget create_rename_dialog(Widget parent,int which){
  Widget dialog,child;
  Arg args[10];
  Cardinal n;
  Pixel white_pixel,black_pixel;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP rename prompt");n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreatePromptDialog(parent,"rename",args,n);
  
  child=XmSelectionBoxGetChild(dialog,XmDIALOG_TEXT);
  XtVaSetValues(child,XmNbackground,white_pixel,
      XmNforeground,black_pixel,
      XmNhighlightThickness,0,NULL);
  
  XtAddCallback(dialog,XmNokCallback,rename_cb,(XtPointer)which);
  XtAddCallback(dialog,XmNhelpCallback,help_cb,(XtPointer)(10+which));

  return dialog;
}

static void exec_cb(Widget w,XtPointer app,XtPointer call){
  int which;
  XmSelectionBoxCallbackStruct *cbs;
  char* text;

  which=(int)app;
  cbs=(XmSelectionBoxCallbackStruct*)call;

  XtUnmanageChild(w);
  if(appdata.job && which==REMOTE)return;

  if(XmStringGetLtoR(cbs->value,XmFONTLIST_DEFAULT_TAG,&text)){
    switch(which){
      case LOCAL:
	busy_cursor(True);
	exec_local(text);
	if(!appdata.job)busy_cursor(False);
	break;
      case REMOTE:
	appdata.job=8;
	busy_cursor(True);
	exec_remote(text);
	busy_cursor(False);
	appdata.job=0;
	break;
     default:
	break;
    }
    free(text);
  }
  return;
}

  

Widget create_exec_dialog(Widget parent,int which){
  Widget dialog,child;
  Arg args[10];
  Cardinal n;
  XmString xms;
  Pixel white_pixel,black_pixel;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  switch(which){
    case LOCAL:
      xms=XmStringCreateLocalized("Enter command to execute localy:");
    break;
    case REMOTE:
      xms=XmStringCreateLocalized(
	  "Enter command to execute remotely\n(using SITE) :");
      break;
    default:
      xms=XmStringCreateLocalized("");
      break;
  }

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP exec prompt");n++;
  XtSetArg(args[n],XmNselectionLabelString,xms);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreatePromptDialog(parent,"exec",args,n);
  
  child=XmSelectionBoxGetChild(dialog,XmDIALOG_TEXT);
  XtVaSetValues(child,XmNbackground,white_pixel,
      XmNforeground,black_pixel,
      XmNhighlightThickness,0,NULL);
  
  XmStringFree(xms);
  XtAddCallback(dialog,XmNokCallback,exec_cb,(XtPointer)which);
  XtAddCallback(dialog,XmNhelpCallback,help_cb,(XtPointer)(20+which));

  return dialog;
}

static void mkdir_cb(Widget w,XtPointer app,XtPointer call){
  int which;
  XmSelectionBoxCallbackStruct *cbs;
  char* text;
  char* mask;

  which=(int)app;
  cbs=(XmSelectionBoxCallbackStruct*)call;

  XtUnmanageChild(w);
  if(appdata.job && which==REMOTE)return;

  if(XmStringGetLtoR(cbs->value,XmFONTLIST_DEFAULT_TAG,&text)){
    switch(which){
      case LOCAL:
	busy_cursor(True);
	if(!mkdir_local(text)){
          mask=XmTextFieldGetString(appdata.local.text);
	  update_local(mask);
	  free(mask);
	}
	if(!appdata.job)busy_cursor(False);
	break;
      case REMOTE:
	appdata.job=8;
	busy_cursor(True);
	if(!mkdir_remote(text)){
          mask=XmTextFieldGetString(appdata.remote.text);
	  update_remote(mask);
	  free(mask);
	}
	busy_cursor(False);
	appdata.job=0;
	break;
     default:
	break;
    }
    free(text);
  }
  return;
}

  

Widget create_mkdir_dialog(Widget parent,int which){
  Widget dialog,child;
  Arg args[10];
  Cardinal n;
  XmString xms;
  Pixel white_pixel,black_pixel;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  switch(which){
    case LOCAL:
      xms=XmStringCreateLocalized("Enter name of local directory to create:");
    break;
    case REMOTE:
      xms=XmStringCreateLocalized("Enter name of remote directory to create:");
      break;
    default:
      xms=XmStringCreateLocalized("");
      break;
  }

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP mkdir prompt");n++;
  XtSetArg(args[n],XmNselectionLabelString,xms);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreatePromptDialog(parent,"mkdir",args,n);
  
  child=XmSelectionBoxGetChild(dialog,XmDIALOG_TEXT);
  XtVaSetValues(child,XmNbackground,white_pixel,
      XmNforeground,black_pixel,
      XmNhighlightThickness,0,NULL);
  
  XmStringFree(xms);
  XtAddCallback(dialog,XmNokCallback,mkdir_cb,(XtPointer)which);
  XtAddCallback(dialog,XmNhelpCallback,help_cb,(XtPointer)(30+which));

  return dialog;
}
