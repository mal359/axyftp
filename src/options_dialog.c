/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <stdio.h>
#include <string.h>

#include "axyftp.h"
#include "options_data.h"
#include "options_dialog.h"
#include "options_general.h"
#include "options_display.h"
#include "options_sound.h"
#include "options_advanced.h"
#include "utils.h"
#include "read_init.h"
#include "functions.h"

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/TextF.h>
#include <Xm/ComboBox.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/Notebook.h>

void put_options_data(Widget dialog,options_data* opt){
  Widget current;
  
  current=XtNameToWidget(dialog,"*anonpass");
  XmTextFieldSetString(current,opt->anonpass);

  current=XtNameToWidget(dialog,"*Verify deletions");
  XmToggleButtonSetState(current,opt->deletions,FALSE);

  current=XtNameToWidget(dialog,"*Recurse deletions");
  XmToggleButtonSetState(current,opt->recurse_del,FALSE);


  current=XtNameToWidget(dialog,"*Show Session Window on startup");
  XmToggleButtonSetState(current,opt->show_session,FALSE);

  current=XtNameToWidget(dialog,"*Enable automatic column width adjustment");
  XmToggleButtonSetState(current,opt->columnadjustment,TRUE);

  current=XtNameToWidget(dialog,"*maxwidth");
  XmTextFieldSetString(current,opt->maxwidth);

  current=XtNameToWidget(dialog,"*Show progress bar");
  XmToggleButtonSetState(current,opt->show_progress,TRUE);
  
  current=XtNameToWidget(dialog,"*progress_treshold");
  XmTextFieldSetString(current,opt->progress_treshold);


  current=XtNameToWidget(dialog,"*Beep at the end of download/upload");
  XmToggleButtonSetState(current,opt->xferbeep,FALSE);


  current=XtNameToWidget(dialog,"*redial");
  XmTextFieldSetString(current,opt->redial);

  current=XtNameToWidget(dialog,"*delay");
  XmTextFieldSetString(current,opt->delay);

  XmUpdateDisplay(toplevel);
}

void fetch_options_data(Widget dialog,options_data* opt){
  Widget current;

  current=XtNameToWidget(dialog,"*anonpass");
  opt->anonpass=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*Verify deletions");
  opt->deletions=XmToggleButtonGetState(current);

  current=XtNameToWidget(dialog,"*Recurse deletions");
  opt->recurse_del=XmToggleButtonGetState(current);


  current=XtNameToWidget(dialog,"*Show Session Window on startup");
  opt->show_session=XmToggleButtonGetState(current);

  current=XtNameToWidget(dialog,"*Enable automatic column width adjustment");
  opt->columnadjustment=XmToggleButtonGetState(current);

  current=XtNameToWidget(dialog,"*maxwidth");
  opt->maxwidth=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*Show progress bar");
  opt->show_progress=XmToggleButtonGetState(current);

  current=XtNameToWidget(dialog,"*progress_treshold");
  opt->progress_treshold=XmTextFieldGetString(current);


  current=XtNameToWidget(dialog,"*Beep at the end of download/upload");
  opt->xferbeep=XmToggleButtonGetState(current);


  current=XtNameToWidget(dialog,"*redial");
  opt->redial=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*delay");
  opt->delay=XmTextFieldGetString(current);

  return;
}

void action_cb(Widget w,XtPointer app,XtPointer call){
  XmString xms;
  String s;

  XtVaGetValues(w,XmNlabelString,&xms,NULL);
  if(XmStringGetLtoR(xms,XmFONTLIST_DEFAULT_TAG,&s)){
    if(strcmp(s,"OK")==0){
      XtUnmanageChild(appdata.options);
      destroy_options_data(appdata.odata);
      appdata.odata=create_options_data(NULL);
      fetch_options_data(appdata.options,appdata.odata);
    } else if(strcmp(s,"Help")==0){
      show_help(2);
    } else if(strcmp(s,"Cancel")==0){
      XtUnmanageChild(appdata.options);
      put_options_data(appdata.options,appdata.odata);
    } else if(strcmp(s,"Save")==0){
      destroy_options_data(appdata.odata);
      appdata.odata=create_options_data(NULL);
      fetch_options_data(appdata.options,appdata.odata);
      write_options_data(options_file,appdata.odata);
    }
    XtFree(s);
  }
  XmStringFree(xms);
}

Widget create_actions(Widget parent){
  static String label[]={"OK","Cancel","Save","Help"};
  Widget actions,button;
  XmString xml;
  int i;
  Arg args[10];
  Cardinal n;

  n=0;
  XtSetArg(args[n],XmNfractionBase,XtNumber(label));n++;
  actions=XmCreateForm(parent,"actions",args,n);
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

Widget create_options_dialog(Widget parent){
  Widget options;
  Arg args[10];
  Cardinal n;
  Widget actions,notebook,general,display,sound,advanced;
  
  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP options");n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  XtSetArg(args[n],XmNverticalSpacing,6);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,6);n++;
  options=XmCreateFormDialog(parent,"session",args,n);

  actions=create_actions(options);

  n=0;
  /*
  XtSetArg(args[n],XmNbackPagePlacement,XmTOP_RIGHT);n++;
  XtSetArg(args[n],XmNmajorTabSpacing,0);n++;
  XtSetArg(args[n],XmNorientation,XmVERTICAL);n++;
  XtSetArg(args[n],XmNbindingType,XmNONE);n++;
  XtSetArg(args[n],XmNbackPageNumber,1);n++;
  XtSetArg(args[n],XmNbackPageSize,0);n++;
  */
  notebook=XmCreateNotebook(options,"notebook",args,n);
  XtManageChild(notebook);

  /*
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmPAGE_SCROLLER);n++;
  XtManageChild(XmCreateLabel(notebook,"",args,n));
  */
  

  general=create_options_general(notebook);
  XtManageChild(general);
  n=0;
  /*
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(notebook,"General",args,n));
  */

  display=create_options_display(notebook);
  XtManageChild(display);
  /*
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(notebook,"Display",args,n));
  */

  sound=create_options_sound(notebook);
  XtManageChild(sound);
  /*
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(notebook,"Sound",args,n));
  */

  advanced=create_options_advanced(notebook);
  XtManageChild(advanced);
  /*
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(notebook,"Advanced",args,n));
  */

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


  put_options_data(options,appdata.odata);

  return options;
}
