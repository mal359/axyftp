/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#ifdef USE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif

#include "axyftp.h"
#include "multi.h"
#include "utils.h"
#include "pixmaps.h"
#include "dirinfo.h"
#include "functions.h"
#include "status.h"
#include "little_dialogs.h"

static void time_to_retry(int sig){
  siglongjmp(jmp_down_env,2);
}

#include<Xm/Xm.h>
#include<Xm/TextF.h>
#include<Xm/MessageB.h>
#include<Xm/SelectioB.h>

static void ack_cb(Widget w,XtPointer app,XtPointer call){
  XtDestroyWidget(XtParent(w));
  return;
}

static void reconnect_cb(Widget w,XtPointer app,XtPointer call){
  int act;
  char* volatile mask;
  volatile int count,ret,total,delay;
  char t[40];
 
  act=(int)app;
  XtDestroyWidget(XtParent(w));
      
  struct sigaction sa;
  sa.sa_handler = time_to_retry;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if(act){
    appdata.job=1;
    busy_cursor(True);
    appdata.sdata->locdir[0]='\0';
    free(appdata.sdata->remdir);

    appdata.sdata->remdir=XtNewString(appdata.remote.list->dir);

    appdata.sdata->locmask[0]='\0';
    appdata.sdata->remmask[0]='\0';
    mask=XmTextFieldGetString(appdata.remote.text);

    total=atoi(appdata.odata->redial);
    if(total<0)total=0;
    delay=atoi(appdata.odata->delay);
    if(delay<0)delay=0;
    count=0;
    
    sigaction(SIGALRM, &sa, NULL);

    while(1){
      start_session(appdata.sdata,mask);
      if(appdata.connected){
	XmString l;
	l=XmStringCreateLocalized("Disconnect");
	XtVaSetValues(appdata.conbutton,XmNlabelString,l,NULL);
	XmStringFree(l);
	break;
      } else {
	sprintf(t,"Attempt %d failed\n",count++);
	append_status(t);
	if(count>total){
	  break;
	}
	if(delay>0){
	  alarm(delay);
	  if(!(ret=sigsetjmp(jmp_down_env,1))){
	    appdata.jump_on_cancel=1;
	    LOOP();
	  } 
	  appdata.jump_on_cancel=0;
	}
	if(appdata.interrupt){
	  appdata.interrupt=0;
	  break;
	}
	sprintf(t,"Attempt %d ...\n",count);
	append_status(t);
      }
    }
   
    free(mask);
    busy_cursor(False);
    appdata.job=0;
  }
}

Widget popup_reconnect_dialog(Widget parent){
  Widget dialog,button;
  XmString xms,yes,no;
  Arg args[10];
  Cardinal n;
  xms=XmStringCreateLocalized("Connection lost. Reconnect?");
  yes=XmStringCreateLocalized("Yes");
  no=XmStringCreateLocalized("No");


  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP reconnect");n++;
  XtSetArg(args[n],XmNokLabelString,yes);n++;
  XtSetArg(args[n],XmNcancelLabelString,no);n++;
  XtSetArg(args[n],XmNmessageString,xms);n++;
  XtSetArg(args[n],XmNmessageAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreateQuestionDialog(parent,"reconnect",args,n);
  XtAddCallback(dialog,XmNokCallback,reconnect_cb,(XtPointer)1);
  XtAddCallback(dialog,XmNcancelCallback,reconnect_cb,(XtPointer)0);
  
  button=XmMessageBoxGetChild(dialog,XmDIALOG_HELP_BUTTON);
  XtUnmanageChild(button);

  XtManageChild(dialog);
  
  XmStringFree(yes);
  XmStringFree(no);
  XmStringFree(xms);
  return dialog;
}
  
Widget popup_error_dialog(Widget parent,char* message){
  Widget dialog,button;
  XmString xms;
  Arg args[4];
  Cardinal n;
  
  xms=XmStringCreateLocalized(message);

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP error");n++;
  XtSetArg(args[n],XmNmessageString,xms);n++;
  XtSetArg(args[n],XmNmessageAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreateErrorDialog(parent,"error",args,n);
  XtAddCallback(dialog,XmNokCallback,ack_cb,NULL);
  
  button=XmMessageBoxGetChild(dialog,XmDIALOG_HELP_BUTTON);
  XtUnmanageChild(button);
  button=XmMessageBoxGetChild(dialog,XmDIALOG_CANCEL_BUTTON);
  XtUnmanageChild(button);

  XtManageChild(dialog);
  XmStringFree(xms);

  return dialog;
}

Widget popup_warning_dialog(Widget parent,char* message){
  Widget dialog,button;
  XmString xms;
  Arg args[4];
  Cardinal n;
  
  xms=XmStringCreateLocalized(message);

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP warning");n++;
  XtSetArg(args[n],XmNmessageString,xms);n++;
  XtSetArg(args[n],XmNmessageAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreateWarningDialog(parent,"info",args,n);
  XtAddCallback(dialog,XmNokCallback,ack_cb,NULL);
  
  button=XmMessageBoxGetChild(dialog,XmDIALOG_HELP_BUTTON);
  XtUnmanageChild(button);
  button=XmMessageBoxGetChild(dialog,XmDIALOG_CANCEL_BUTTON);
  XtUnmanageChild(button);

  XtManageChild(dialog);
  XmStringFree(xms);

  return dialog;
}
