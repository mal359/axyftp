/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <stdio.h>
#include <pthread.h>

#include "axyftp.h"
#include "utils.h"
#include "functions.h"
#include "progress_dialog.h"

static void translate_time(char* buf,long tot){
  long hours;
  long mins;
  long secs;

  hours=tot/(60*60);
  tot-=(hours*60*60);
  mins=tot/60;
  tot-=(mins*60);
  secs=tot;

  if(hours){
    sprintf(buf,"%ld:%.2ld:%.2ld",hours,mins,secs);
  } else {
    sprintf(buf,"%ld:%.2ld",mins,secs);
  }
}

#include<Xm/Xm.h>
#include<Xm/Scale.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/PushB.h>
#include<Xm/ScrollBar.h>

static void xfer_cancel_cb(Widget w,XtPointer app,XtPointer call){
  appdata.small_interrupt=1;
  XtSetSensitive(w,False);
}

void update_progress_dialog(Widget w,
  int value,long size,float rate,long elapsed,long left){
  Widget child;
  char buf[25];
  XmString xms;


  if(value>=0){
    child=XtNameToWidget(w,"*scale");
    XtVaSetValues(child,XmNvalue,0,XmNsliderSize,(value==0)?1:value,NULL);

    child=XtNameToWidget(w,"*percent");
    sprintf(buf,"%d %%",value);
    xms=XmStringCreateLocalized(buf);
    XtVaSetValues(child,XmNlabelString,xms,NULL);
    XmStringFree(xms);
  }

  
  if(size>=0){
    child=XtNameToWidget(w,"*size");
    sprintf(buf,"%ld",size);
    xms=XmStringCreateLocalized(buf);
    XtVaSetValues(child,XmNlabelString,xms,NULL);
    XmStringFree(xms);
  }
  
  if(rate>=0){
    child=XtNameToWidget(w,"*rate");
    sprintf(buf,"%.3f kBps",rate);
    xms=XmStringCreateLocalized(buf);
    XtVaSetValues(child,XmNlabelString,xms,NULL);
    XmStringFree(xms);
  }
  
  if(elapsed>=0){
    child=XtNameToWidget(w,"*elapsed");
    translate_time(buf,elapsed);
    xms=XmStringCreateLocalized(buf);
    XtVaSetValues(child,XmNlabelString,xms,NULL);
    XmStringFree(xms);
  }
  
  if(left>=0){
    child=XtNameToWidget(w,"*left");
    translate_time(buf,left);
    xms=XmStringCreateLocalized(buf);
    XtVaSetValues(child,XmNlabelString,xms,NULL);
    XmStringFree(xms);
  }
}

void init_progress_dialog(Widget w,char* name,long size){
  Widget child;
  char* buf;
  XmString xms;
  
  child=XtNameToWidget(w,"*toptext");
  buf=XtMalloc(strlen(name)+30);
  sprintf(buf,"%s (%ld bytes)",name,size);
  xms=XmStringCreateLocalized(buf);
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);
  XtFree(buf);

  child=XtNameToWidget(w,"*percent");
  xms=XmStringCreateLocalized("0 %");
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);

  child=XtNameToWidget(w,"*scale");
  XtVaSetValues(child,XmNvalue,0,XmNsliderSize,1,NULL);

  child=XtNameToWidget(w,"*size");
  xms=XmStringCreateLocalized("0");
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);
  
  child=XtNameToWidget(w,"*rate");
  xms=XmStringCreateLocalized("? kBps");
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);
  
  child=XtNameToWidget(w,"*elapsed");
  xms=XmStringCreateLocalized("0:00");
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);
  
  child=XtNameToWidget(w,"*left");
  xms=XmStringCreateLocalized("?:??");
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);
  
  child=XtNameToWidget(w,"*Cancel");
  XtSetSensitive(child,True);
}

void show_progress_dialog(Widget w){
  XtManageChild(w);
  XmUpdateDisplay(toplevel);
  process_events();
}

void hide_progress_dialog(Widget w){
  XtUnmanageChild(w);
  XmUpdateDisplay(toplevel);
  XmUpdateDisplay(w);
  process_events();
/*  XtDestroyWidget(w);*/
}

static void slider_cb(Widget w,XtPointer app,XtPointer call){
  XtVaSetValues(w,XmNvalue,0,NULL);
  return;
}

Widget create_progress_dialog(Widget parent){
  Widget progress;
  Widget toptext,scale,size,rate,elapsed,left,cancel,percent;
  Arg args[15];
  Cardinal n;
  Dimension d;
  XmString xms;
  Colormap cmap;
  XColor color,exact;

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP transfer");n++;
  /*XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;*/
  XtSetArg(args[n],XmNverticalSpacing,6);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,6);n++;
  XtSetArg(args[n],XmNresizePolicy,XmRESIZE_NONE);n++;
  XtSetArg(args[n],XmNinput,False);n++;
  progress=XmCreateFormDialog(parent,"progress",args,n);

  xms=XmStringCreateLocalized("|");
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginLeft,12);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  toptext=XmCreateLabel(progress,"toptext",args,n);
  XtManageChild(toptext);
  
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,toptext);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginTop,10);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNrecomputeSize,FALSE);n++;
  percent=XmCreateLabel(progress,"percent",args,n);
  XtManageChild(percent);

  /*
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,percent);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNorientation,XmHORIZONTAL);n++;
  XtSetArg(args[n],XmNprocessingDirection,XmMAX_ON_RIGHT);n++;
  XtSetArg(args[n],XmNslidingMode,XmTHERMOMETER);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNsliderVisual,XmFOREGROUND_COLOR);n++;
  XtSetArg(args[n],XmNshowValue,FALSE);n++;
  scale=XmCreateScale(progress,"scale",args,n);
  XtManageChild(scale);

  XtVaGetValues(parent,XmNcolormap,&cmap,NULL);
  if(XAllocNamedColor(XtDisplay(parent),cmap,"#2F2FCF",&color,&exact)){
    scroll=XtNameToWidget(scale,"*Scrollbar");
    XtVaSetValues(scroll,XmNforeground,color.pixel,NULL);
  }
  */

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,percent);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;

  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  /*XtSetArg(args[n],XmNsensitive,FALSE);n++;*/
  XtSetArg(args[n],XmNminimum,0);n++;
  XtSetArg(args[n],XmNmaximum,100);n++;
  XtSetArg(args[n],XmNvalue,0);n++;
  XtSetArg(args[n],XmNorientation,XmHORIZONTAL);n++;
  XtSetArg(args[n],XmNprocessingDirection,XmMAX_ON_RIGHT);n++;
  XtSetArg(args[n],XmNshowArrows,FALSE);n++;
  XtSetArg(args[n],XmNsliderSize,1);n++;
  XtVaGetValues(parent,XmNcolormap,&cmap,NULL);
  if(XAllocNamedColor(XtDisplay(parent),cmap,"#2F2FCF",&color,&exact)){
    XtSetArg(args[n],XmNtroughColor,color.pixel);n++;
  }
  scale=XmCreateScrollBar(progress,"scale",args,n);
  XtManageChild(scale);
  XtAddCallback(scale,XmNvalueChangedCallback,slider_cb,NULL);
  XtAddCallback(scale,XmNdragCallback,slider_cb,NULL);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,scale);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,1*100/5);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginLeft,12);n++;
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  size=XmCreateLabel(progress,"size",args,n);
  XtManageChild(size);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,scale);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,1*100/5);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,2*100/5);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginLeft,12);n++;
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  rate=XmCreateLabel(progress,"rate",args,n);
  XtManageChild(rate);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,scale);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,2*100/5);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,3*100/5);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginLeft,12);n++;
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  elapsed=XmCreateLabel(progress,"elapsed",args,n);
  XtManageChild(elapsed);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,scale);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,3*100/5);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,4*100/5);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginLeft,12);n++;
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  left=XmCreateLabel(progress,"left",args,n);
  XtManageChild(left);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,scale);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  cancel=XmCreatePushButton(progress,"Cancel",args,n);
  XtManageChild(cancel);
  XtAddCallback(cancel,XmNactivateCallback,xfer_cancel_cb,NULL);
  XtAddCallback(cancel,XmNdisarmCallback,xfer_cancel_cb,NULL);

  XtVaGetValues(cancel,XmNwidth,&d,NULL);
  XtVaSetValues(progress,XmNwidth,d*10,NULL);

  XmStringFree(xms);

  return progress;
}
