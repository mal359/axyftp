/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */

#include <stdlib.h>

#include"axyftp.h"
#include"utils.h"
#include"multi.h"
#include"functions.h"
#include"dirlist.h"
#include"dirname.h"
#include"little_dialogs.h"
#include"buttonbar.h"

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/PushB.h>
#include<Xm/TextF.h>

void activate_cb(Widget,XtPointer,XtPointer);

void activate_cb(Widget w,XtPointer app,XtPointer call){
  XmPushButtonCallbackStruct *cbs;
  XmString xmlabel;
  char* strlabel;

  cbs=(XmPushButtonCallbackStruct*)call;

  XtVaGetValues(w,XmNlabelString,&xmlabel,NULL);
  if(XmStringGetLtoR(xmlabel,XmFONTLIST_DEFAULT_TAG,&strlabel)){
    if(strcmp(strlabel,"connect")==0){
      XtManageChild(appdata.session);
    } else if(strcmp(strlabel,"help")==0){
      show_help(1);
    } else if(strcmp(strlabel,"logWnd")==0){
      show_log();
    } else if(strcmp(strlabel,"about")==0){
      XtManageChild(appdata.about);
    } else if(strcmp(strlabel,"exit")==0){
      exit(0);
    } else if(strcmp(strlabel,"options")==0){
      XtManageChild(appdata.options);
    } else if(strcmp(strlabel,"disconnect")==0){
      XmString l;
      if(!appdata.job){
	busy_cursor(True);
	process_events();
	appdata.job=2;
	end_session();
	clear_dirlist(appdata.remote.table);
	clear_dirname(appdata.remote.combo);
	XmTextFieldSetString(appdata.remote.text,"");
	l=XmStringCreateLocalized("connect");
	XtVaSetValues(w,XmNlabelString,l,NULL);
	XmStringFree(l);
	busy_cursor(False);
	appdata.job=0;
      }
    } else if(strcmp(strlabel,"cancel")==0){
      if(appdata.job)appdata.interrupt=1;
      if(appdata.jump_on_cancel){
	siglongjmp(jmp_down_env,1);
      }
    }
    XtFree(strlabel);
  } 
}

Widget create_buttonbar(Widget parent){
  Arg args[10];
  Cardinal n;

  Widget buttonbar;
  Widget connect,cancel,logwnd,help,options,about,exit;
  XmString label;
  Dimension height;

  n=0;
  XtSetArg(args[n],XmNfractionBase,7);n++;
  buttonbar=XmCreateForm(parent,"buttonbar",args,n);
  XtManageChild(buttonbar);

  n=0;
  label=XmStringCreateLocalized("connect");
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnArm,False);n++;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,1);n++;
  connect=XmCreatePushButton(buttonbar,"connect",args,n);
  XtManageChild(connect);XmStringFree(label);
  XtAddCallback(connect,XmNactivateCallback,activate_cb,NULL);
  
  XtVaGetValues(connect,XmNheight,&height,NULL);
  height=height*7/8;
  XtVaSetValues(connect,XmNheight,height,NULL);
  
  n=0;
  label=XmStringCreateLocalized("cancel");
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnArm,False);n++;
  XtSetArg(args[n],XmNheight,height);n++;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,connect);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,2);n++;
  cancel=XmCreatePushButton(buttonbar,"cancel",args,n);
  XtManageChild(cancel);XmStringFree(label);
  XtAddCallback(cancel,XmNactivateCallback,activate_cb,NULL);
  
  n=0;
  label=XmStringCreateLocalized("logWnd");
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnArm,False);n++;
  XtSetArg(args[n],XmNheight,height);n++;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,cancel);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,3);n++;
  logwnd=XmCreatePushButton(buttonbar,"logWnd",args,n);
  XtManageChild(logwnd);XmStringFree(label);
  XtAddCallback(logwnd,XmNactivateCallback,activate_cb,NULL);
  
  n=0;
  label=XmStringCreateLocalized("help");
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnArm,False);n++;
  XtSetArg(args[n],XmNheight,height);n++;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,logwnd);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,4);n++;
  help=XmCreatePushButton(buttonbar,"help",args,n);
  XtManageChild(help);XmStringFree(label);
  XtAddCallback(help,XmNactivateCallback,activate_cb,NULL);
  
  n=0;
  label=XmStringCreateLocalized("options");
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnArm,False);n++;
  XtSetArg(args[n],XmNheight,height);n++;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,help);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,5);n++;
  options=XmCreatePushButton(buttonbar,"options",args,n);
  XtManageChild(options);XmStringFree(label);
  XtAddCallback(options,XmNactivateCallback,activate_cb,NULL);
  
  n=0;
  label=XmStringCreateLocalized("about");
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnArm,False);n++;
  XtSetArg(args[n],XmNheight,height);n++;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,options);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,6);n++;
  about=XmCreatePushButton(buttonbar,"about",args,n);
  XtManageChild(about);XmStringFree(label);
  XtAddCallback(about,XmNactivateCallback,activate_cb,NULL);
  
  n=0;
  label=XmStringCreateLocalized("exit");
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnArm,False);n++;
  XtSetArg(args[n],XmNheight,height);n++;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,about);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  exit=XmCreatePushButton(buttonbar,"cancel",args,n);
  XtManageChild(exit);XmStringFree(label);
  XtAddCallback(exit,XmNactivateCallback,activate_cb,NULL);
 
  appdata.conbutton=connect;
  
  return buttonbar;
}
