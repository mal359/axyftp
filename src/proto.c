/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"utils.h"
#include"proto.h"

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/ToggleB.h>
#include<Xm/RowColumn.h>
#include<Xm/ScrollBar.h>

char get_proto_state(Widget proto){
  Widget child;

  child=XtNameToWidget(proto,"*ASCII");
  if(XmToggleButtonGetState(child))return 'A';

  child=XtNameToWidget(proto,"*Binary");
  if(XmToggleButtonGetState(child))return 'I';

  return '\0';
}


Widget create_proto(Widget parent){
  Arg args[10];
  Cardinal n;
  Widget proto,b1,b2,b3;
  Pixel black_pixel;

  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  
  n=0;
  XtSetArg(args[n],XmNorientation,XmHORIZONTAL);n++;
  XtSetArg(args[n],XmNresizeWidth,True);n++;
  XtSetArg(args[n],XmNpacking,XmPACK_COLUMN);n++;
  XtSetArg(args[n],XmNentryAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNisAligned,True);n++;
  XtSetArg(args[n],XmNradioAlwaysOne,True);n++;
  XtSetArg(args[n],XmNnumColumns,1);n++;
  XtSetArg(args[n],XmNbuttonCount,3);n++;
  proto=XmCreateRadioBox(parent,"proto",args,n);
  XtManageChild(proto);

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNindicatorType,XmONE_OF_MANY);n++;
  XtSetArg(args[n],XmNset,FALSE);n++;
  /*XtSetArg(args[n],XmNselectColor,black_pixel);n++;*/
  b1=XmCreateToggleButton(proto,"ASCII",args,n);
  XtManageChild(b1);

  

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNindicatorType,XmONE_OF_MANY);n++;
  XtSetArg(args[n],XmNset,TRUE);n++;
  /*XtSetArg(args[n],XmNselectColor,black_pixel);n++;*/
  b2=XmCreateToggleButton(proto,"Binary",args,n);
  XtManageChild(b2);

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNindicatorType,XmN_OF_MANY);n++;
  XtSetArg(args[n],XmNset,FALSE);n++;
  b3=XmCreateToggleButton(proto,"Auto",args,n);
  XtManageChild(b3);

  appdata.proto=proto;
  
  return proto;
}
