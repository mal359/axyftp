/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"session_advanced.h"

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/TextF.h>

Widget create_session_advanced(Widget parent){
  Widget advanced;
  Arg args[15];
  Cardinal n;
  Widget current;
  Pixel white_pixel,black_pixel;
  XmString xms;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  XtSetArg(args[n],XmNrubberPositioning,True);n++;
  advanced=XmCreateForm(parent,"Advanced",args,n);
  XtManageChild(advanced);

  n=0;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,20);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100);n++;
  current=XmCreateLabel(advanced,"Remote Port ",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNcolumns,10);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,20);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,40);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100);n++;
  current=XmCreateTextField(advanced,"port",args,n);
  XtManageChild(current);

  xms=XmStringCreateSimple("Remote Host control connection port. Usually 21.");
  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,40);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  current=XmCreateLabel(advanced,"port_label",args,n);
  XmStringFree(xms);
  XtManageChild(current);

  return advanced;
}
