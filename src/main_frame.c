/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"main_frame.h"
#include"buttonbar.h"
#include"arrows.h"
#include"status.h"
#include"proto.h"
#include"panel.h"

#include<Xm/Xm.h>
#include<Xm/MainW.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/PanedW.h>


Widget create_main_frame(Widget parent){
  Arg args[10];
  Cardinal n;
  Widget form;
  Widget local,remote,arrows,proto,status,buttonbar,lbox;
  XmString xms;

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  form=XmCreateForm(parent,"form",args,n);
  XtManageChild(form);

  /*
  n=0;
  XtSetArg(args[n],XmNorientation,XmHORIZONTAL);n++;
  XtSetArg(args[n],XmNseparatorOn,FALSE);n++;
  XtSetArg(args[n],XmNspacing,0);n++;
  pane=XmCreatePanedWindow(form,"pane",args,n);
  XtManageChild(pane);
  */

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  /*XtSetArg(args[n],XmNrubberPositioning,TRUE);n++;*/
  lbox=XmCreateForm(form,"lbox",args,n);
  XtManageChild(lbox);
  /*
  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  lbox=XmCreateForm(pane,"lbox",args,n);
  XtManageChild(lbox);
  */

  local=create_panel(lbox,LOCAL);
  arrows=create_arrows(lbox);
  remote=create_panel(lbox,REMOTE);
  /*
  local=create_panel(lbox,LOCAL);
  arrows=create_arrows(lbox);
  remote=create_panel(pane,REMOTE);
  */

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNrightWidget,arrows);n++;
  XtSetValues(local,args,n);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,50);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetValues(arrows,args,n);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftWidget,arrows);n++;
  XtSetValues(remote,args,n);

  proto=create_proto(form);
  status=create_status(form);
  buttonbar=create_buttonbar(form);


  appdata.local.list=NULL;
  appdata.remote.list=NULL;
  
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNbottomWidget,proto);n++;
  XtSetValues(lbox,args,n);


  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,50);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNbottomWidget,status);n++;
  XtSetValues(proto,args,n);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNbottomWidget,buttonbar);n++;
  XtSetValues(status,args,n);
 
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetValues(buttonbar,args,n);

  n=0;
  xms=XmStringCreateLocalized("                    ");
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  XtSetArg(args[n],XmNmarginRight,12);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,remote);n++;
  appdata.listprogress=XmCreateLabel(form,"listprogress",args,n);
  XtManageChild(appdata.listprogress);
  XmStringFree(xms);
  
  return form;
}

int adjust_main_frame(Widget id){
  Widget arrows;
  Dimension d;
  
  arrows=XtNameToWidget(id,"*arrows");
  
  XtVaGetValues(appdata.proto,XmNwidth,&d,NULL);
  XtVaSetValues(appdata.proto,XmNleftOffset,-d/2,NULL);

  XtVaGetValues(arrows,XmNwidth,&d,NULL);
  XtVaSetValues(arrows,XmNleftOffset,-d/2,NULL);

  return 0;
}
