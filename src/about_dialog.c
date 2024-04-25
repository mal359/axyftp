/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"pixmaps.h"
#include"about_dialog.h"

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/PushB.h>

void hide_about_cb(Widget w,XtPointer app,XtPointer call){
  XtUnmanageChild(appdata.about);
}

Widget create_about_dialog(Widget parent){
  Widget about,picture,version,copyright,form,button;
  char version_str[64];
  Arg args[10];
  Cardinal n;
  Dimension d;

  n=0;
  XtSetArg(args[n],XmNtitle,"About AxY FTP");n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  XtSetArg(args[n],XmNverticalSpacing,6);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,6);n++;
  XtSetArg(args[n],XmNnoResize,True);n++;
  about=XmCreateFormDialog(parent,"about",args,n);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNlabelType,XmPIXMAP);n++;
  if(portret){
    XtSetArg(args[n],XmNlabelPixmap,portret);n++;
  }
  picture=XmCreateLabel(about,"picture",args,n);n++;
  XtManageChild(picture);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNtopOffset,20);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,picture);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_CENTER);n++;
  sprintf(version_str,"AxY FTP  Motif %d.%d version %d.%d.%d",
   XmVERSION,XmREVISION,AXYFTP_MAJOR,AXYFTP_MINOR,AXYFTP_MICRO);
  version=XmCreateLabel(about,version_str,args,n);n++;
  XtManageChild(version);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,version);n++;
  XtSetArg(args[n],XmNtopOffset,20);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,picture);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_CENTER);n++;
  copyright=XmCreateLabel(about,"Copyright (c) 1999  Alexander Yukhimets."
      " All rights reserved.",args,n);n++;
  XtManageChild(copyright);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,copyright);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,picture);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  form=XmCreateForm(about,"form",args,n);
  XtManageChild(form);
  
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,50);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  button=XmCreatePushButton(form," OK ",args,n);n++;
  XtManageChild(button);

  XtVaGetValues(button,XmNwidth,&d,NULL);
  XtVaSetValues(button,XmNleftOffset,-d/2,NULL);


  XtAddCallback(button,XmNactivateCallback,hide_about_cb,NULL);

  return about;
}
