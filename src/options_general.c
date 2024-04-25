/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>

#include"options_general.h"
#include"axyftp.h"
#include"utils.h"
#include"read_init.h"

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/PushB.h>
#include<Xm/ToggleB.h>
#include<Xm/TextF.h>
#include<Xm/Label.h>
#include<Xm/List.h>

Widget create_options_general(Widget parent){
  Widget general;
  Arg args[20];
  Cardinal n;
  Widget current;
  Pixel white_pixel;
  Pixel black_pixel;

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  general=XmCreateForm(parent,"General",args,n);
  XtManageChild(general);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*1/6);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,30);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/6);n++;
  current=XmCreateLabel(general,"Anonymous password:",args,n);
  XtManageChild(current);

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*1/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,100);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateTextField(general,"anonpass",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*3/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*4/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,50);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,0);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  XtSetArg(args[n],XmNrightOffset,10);n++;
  current=XmCreateToggleButton(general,"Verify deletions",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*4/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*5/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,50);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,0);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  XtSetArg(args[n],XmNrightOffset,10);n++;
  current=XmCreateToggleButton(general,"Recurse deletions",args,n);
  XtManageChild(current);

  return general;
}
