/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"session_startup.h"

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/TextF.h>

Widget create_session_startup(Widget parent){
  Widget startup;
  Arg args[15];
  Cardinal n;
  Widget current;
  Pixel white_pixel,black_pixel;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  XtSetArg(args[n],XmNrubberPositioning,True);n++;
  startup=XmCreateForm(parent,"Startup",args,n);
  XtManageChild(startup);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*1);n++;
  XtSetArg(args[n],XmNbottomOffset,4);n++;
  current=XmCreateLabel(startup,
      "Initial Command: (use ';' to separate multiple commands)",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*2);n++;
  current=XmCreateTextField(startup,"initcom",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*3);n++;
  XtSetArg(args[n],XmNbottomOffset,4);n++;
  current=XmCreateLabel(startup,"Initial Remote Directory:",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*4);n++;
  current=XmCreateTextField(startup,"remdir",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*5);n++;
  XtSetArg(args[n],XmNbottomOffset,4);n++;
  current=XmCreateLabel(startup,"Initial Local Directory:",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*6);n++;
  current=XmCreateTextField(startup,"locdir",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,30);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomOffset,4);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*8);n++;
  current=XmCreateLabel(startup,"Remote File Mask:",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNcolumns,10);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,50);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomOffset,4);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*8);n++;
  current=XmCreateTextField(startup,"remmask",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,30);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  current=XmCreateLabel(startup,"Local File Mask:",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNcolumns,10);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,50);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  current=XmCreateTextField(startup,"locmask",args,n);
  XtManageChild(current);


  return startup;
}
