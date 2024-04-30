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
#include "utils.h"
#include "multi.h"
#include "status.h"
#include "read_init.h"
#include "session_data.h"
#include "functions.h"
#include "session_dialog.h"
#include "session_general.h"
#include "session_startup.h"
#include "session_advanced.h"

static void time_to_retry(int sig){
  siglongjmp(jmp_down_env,2);
}

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/TextF.h>
#include <Xm/ComboBox.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/Notebook.h>

static void init_session_dialog(Widget);
static void action_cb(Widget,XtPointer,XtPointer);

static void request_connection(){
  char* volatile mask;
  volatile int count,ret,total,delay;
  char t[40];
  
  struct sigaction sa;
  sa.sa_handler = time_to_retry;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  
  fetch_session_data(appdata.session,appdata.sdata);
  XmTextFieldSetString(appdata.remote.text,appdata.sdata->remmask);
  XmTextFieldSetString(appdata.local.text,appdata.sdata->locmask);
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
}

void update_session_dialog(int pos,char set_profile){
  session_data *sd;
  int i;
  for(i=0,sd=appdata.sdata;i<pos;i++)sd=sd->next;
  put_session_data(appdata.session,sd,set_profile);
}

void put_session_data(Widget dialog,session_data* sd,char set_profile){
  Widget current;
  char* pass;

  if(set_profile){
    current=XtNameToWidget(dialog,"*profile*Text");
    XmTextFieldSetString(current,sd->profile);
  }
  
  current=XtNameToWidget(dialog,"*host");
  XmTextFieldSetString(current,sd->host);

  current=XtNameToWidget(dialog,"*Anonymous");
  XmToggleButtonSetState(current,sd->anon,True);

  current=XtNameToWidget(dialog,"*user");
  XmTextFieldSetString(current,sd->user);

  current=XtNameToWidget(dialog,"*pass");
  XtVaGetValues(current,XmNuserData,&pass,NULL);
  if(sd->anon){
    XmTextFieldSetString(current,sd->pass);
    pass[0]='\0';
  } else {
    XmTextFieldSetString(current,sd->pass);
  }

  current=XtNameToWidget(dialog,"*account");
  XmTextFieldSetString(current,sd->account);

  current=XtNameToWidget(dialog,"*comment");
  XmTextFieldSetString(current,sd->comment);

  current=XtNameToWidget(dialog,"*remdir");
  XmTextFieldSetString(current,sd->remdir);

  current=XtNameToWidget(dialog,"*locdir");
  XmTextFieldSetString(current,sd->locdir);

  current=XtNameToWidget(dialog,"*initcom");
  XmTextFieldSetString(current,sd->initcom);

  current=XtNameToWidget(dialog,"*locmask");
  XmTextFieldSetString(current,sd->locmask);

  current=XtNameToWidget(dialog,"*remmask");
  XmTextFieldSetString(current,sd->remmask);

  current=XtNameToWidget(dialog,"*Save Password");
  XmToggleButtonSetState(current,sd->save,False);

  current=XtNameToWidget(dialog,"*port");
  XmTextFieldSetString(current,sd->port);

  XmUpdateDisplay(toplevel);
}

void fetch_session_data(Widget dialog,session_data* sd){
  Widget current;
  char* pass;

  current=XtNameToWidget(dialog,"*profile*Text");
  sd->profile=XmTextFieldGetString(current);
  
  current=XtNameToWidget(dialog,"*host");
  sd->host=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*user");
  sd->user=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*Anonymous");
  sd->anon=XmToggleButtonGetState(current);

  current=XtNameToWidget(dialog,"*pass");
  if(sd->anon){
    sd->pass=XmTextFieldGetString(current);
  } else {
    XtVaGetValues(current,XmNuserData,&pass,NULL);
    sd->pass=XtNewString(pass);
  }

  current=XtNameToWidget(dialog,"*account");
  sd->account=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*comment");
  sd->comment=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*remdir");
  sd->remdir=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*locdir");
  sd->locdir=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*initcom");
  sd->initcom=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*locmask");
  sd->locmask=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*remmask");
  sd->remmask=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*Save Password");
  sd->save=XmToggleButtonGetState(current);

  current=XtNameToWidget(dialog,"*port");
  sd->port=XmTextFieldGetString(current);

  return;
}

static void action_cb(Widget w,XtPointer app,XtPointer call){
  XmString xms,item;
  String s;
  Widget list,combo;
  int * poslist;
  int poscount,i;
  session_data *sd;

  XtVaGetValues(w,XmNlabelString,&xms,NULL);
  if(XmStringGetLtoR(xms,XmFONTLIST_DEFAULT_TAG,&s)){
    if(strcmp(s,"OK")==0){
      XtUnmanageChild(appdata.session);
      appdata.job=1;
      busy_cursor(True);
      process_events();
      request_connection();
      busy_cursor(False);
      appdata.job=0;
    } else if(strcmp(s,"Help")==0){
      show_help(2);
    } else if(strcmp(s,"Cancel")==0){
      XtUnmanageChild(appdata.session);
    } else if(strcmp(s,"Save")==0){
      combo=XtNameToWidget(appdata.session,"*profile");
      list=XtNameToWidget(combo,"*List");
      if(XmListGetSelectedPos(list,&poslist,&poscount)){
	for(i=0,sd=appdata.sdata;i<poslist[0];i++)sd=sd->next;
	fetch_session_data(appdata.session,sd);
	item=XmStringCreateLocalized(sd->profile);
	XmListReplaceItemsPos(list,&item,1,poslist[0]);
	XmListUpdateSelectedList(list);
	XmListSelectPos(list,poslist[0],False);
	XmStringFree(item);
	free((char*)poslist);
        write_session_data(session_file,appdata.sdata);
      } else {
	appdata.sdata->next=create_session_data(appdata.sdata->next);
	fetch_session_data(appdata.session,appdata.sdata->next);
	item=XmStringCreateLocalized(appdata.sdata->next->profile);
	XtVaGetValues(list,XmNitemCount,&i,NULL);
	XmComboBoxAddItem(combo,item,1,FALSE);
	i++;
	if(i<=MAX_VISIBLE_ITEMS){
	  XtVaSetValues(combo,XmNvisibleItemCount,i,NULL);
	  XtVaSetValues(list,XmNvisibleItemCount,i,NULL);
	}
	XmStringFree(item);
	XmListSelectPos(list,1,TRUE);
        write_session_data(session_file,appdata.sdata);
      }
    } 
    free(s);
  }
  XmStringFree(xms);
}

static Widget create_actions(Widget parent){
  static String label[]={"OK","Cancel","Save","Help"};
  Widget actions,button;
  XmString xml;
  int i;
  Arg args[10];
  Cardinal n;

  n=0;
  XtSetArg(args[n],XmNfractionBase,XtNumber(label));n++;
  actions=XmCreateForm(parent,"Actions",args,n);
  XtManageChild(actions);

  for(i=0;i<XtNumber(label);i++){
    xml=XmStringCreateLocalized(label[i]);

    n=0;
    XtSetArg(args[n],XmNlabelString,xml);n++;
    XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
    XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
    XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
    XtSetArg(args[n],XmNleftPosition,i);n++;
    XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
    XtSetArg(args[n],XmNrightPosition,i+1);n++;
    button=XmCreatePushButton(actions,label[i],args,n);
    XtManageChild(button);
    XtAddCallback(button,XmNactivateCallback,action_cb,NULL);

    XmStringFree(xml);
  }
  
  return actions;
}

Widget create_session_dialog(Widget parent){
  Widget session;
  Arg args[10];
  Cardinal n;
  Widget actions,notebook,general,startup,advanced;
  
  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP sessions");n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  XtSetArg(args[n],XmNverticalSpacing,6);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,6);n++;
  session=XmCreateFormDialog(parent,"session",args,n);

  actions=create_actions(session);

  n=0;
/* #if XM_NOTEBOOK */
  XtSetArg(args[n],XmNbackPagePlacement,XmTOP_RIGHT);n++;
  XtSetArg(args[n],XmNmajorTabSpacing,0);n++;
  XtSetArg(args[n],XmNorientation,XmVERTICAL);n++;
  XtSetArg(args[n],XmNbindingType,XmNONE);n++;
  XtSetArg(args[n],XmNbackPageNumber,1);n++;
  XtSetArg(args[n],XmNbackPageSize,0);n++;
  notebook=XmCreateNotebook(session,"notebook",args,n);
/* #else
  notebook=XmAxyCreateNotebook(session,"notebook",args,n);
#endif */
  XtManageChild(notebook);

/* #if XM_NOTEBOOK */ 
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmPAGE_SCROLLER);n++;
  XtManageChild(XmCreateLabel(notebook,"",args,n));
/* #endif */

  general=create_session_general(notebook);
  XtManageChild(general);

/* #if XM_NOTEBOOK */ 
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(notebook,"General",args,n));
/* #endif */

  n=0;
  startup=create_session_startup(notebook);
  XtManageChild(startup);

/* #if XM_NOTEBOOK */ 
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(notebook,"Startup",args,n));
/* #endif */

  n=0;
  advanced=create_session_advanced(notebook);
  XtManageChild(advanced);

/* #if XM_NOTEBOOK */ 
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(notebook,"Advanced",args,n));
/* #endif */


  XtVaSetValues(actions,
      XmNbottomAttachment,XmATTACH_FORM,
      XmNrightAttachment,XmATTACH_FORM,
      XmNtopAttachment,XmATTACH_NONE,
      XmNleftAttachment,XmATTACH_FORM,NULL);

  XtVaSetValues(notebook,
      XmNbottomAttachment,XmATTACH_WIDGET,
      XmNbottomWidget,actions,
      XmNrightAttachment,XmATTACH_FORM,
      XmNtopAttachment,XmATTACH_FORM,
      XmNleftAttachment,XmATTACH_FORM,NULL);

/* #if !XM_NOTEBOOK
  XmAxyNotebookSetCurrentPage(notebook,1,FALSE);
#endif */
  init_session_dialog(session);
  
  return session;

}

void set_profile_strings(Widget combo){
  Widget list;
  session_data* top;
  XmString xms;
  int i;

  list=XtNameToWidget(combo,"*List");

  XmListDeleteAllItems(list);
  
  top=appdata.sdata;
  while(top->next!=NULL){
    top=top->next;
    xms=XmStringCreateLocalized(top->profile);
    XmComboBoxAddItem(combo,xms,0,FALSE);
    XmStringFree(xms);
  }
  XtVaGetValues(list,XmNitemCount,&i,NULL);
  if(i>MAX_VISIBLE_ITEMS)i=MAX_VISIBLE_ITEMS;
  XtVaSetValues(combo,XmNvisibleItemCount,i>0?i:1,NULL);
  XtVaSetValues(list,XmNvisibleItemCount,i>0?i:1,NULL);

}

static void init_session_dialog(Widget dialog){
  Widget combo;
  session_data* top;

  combo=XtNameToWidget(dialog,"*profile");

  set_profile_strings(combo);

  top=create_session_data(NULL);
  empty_session_data(top);
  put_session_data(dialog,top,TRUE);
  destroy_session_data(top);
}
