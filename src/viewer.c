/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>
#include<unistd.h>
#include<stdio.h>

#include"axyftp.h"
#include"utils.h"
#include"dirinfo.h"
#include"viewer.h"

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/Text.h>
#include<Xm/Label.h>
#include<Xm/PushB.h>

void fill_dirinfo(Widget text,dirinfo* di){
  int i;
  char* line;
  for(i=1;i<=di->total;i++){
    line=get_fileinfo_string(di->files[i]);
    if(line){
      XmTextInsert(text,XmTextGetLastPosition(text),line);
      XtFree(line);
    }
  }
  XmTextShowPosition(text,0);
  XmUpdateDisplay(text);
}


int fill_viewer(Widget text,int fd){
  char buf[1025];
  int len;

  while((len=read(fd,buf,1024))>0){
    buf[len]='\0';
    XmTextInsert(text,XmTextGetLastPosition(text),buf);
    XmUpdateDisplay(text);
  }
  close(fd);
  return len;
}

static void destroy_cb(Widget w,XtPointer app,XtPointer call){
  while(!XtIsWMShell(w))w=XtParent(w);
  XtDestroyWidget(w);
}

Widget create_viewer(Widget parent,char* name){
  Widget viewer,text,button;
  Arg args[10];
  Cardinal n;
  Dimension d;
  Pixel white_pixel,black_pixel;
  char* title;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  title=XtMalloc(strlen(name)+20);
  sprintf(title,"AxY FTP viewer - %s",name);

  n=0;
  XtSetArg(args[n],XmNtitle,title);n++;
  XtSetArg(args[n],XmNtransient,False);n++;
  XtSetArg(args[n],XmNverticalSpacing,8);n++;
  viewer=XmCreateFormDialog(parent,"viewer",args,n);
  XtFree(title);
/*
  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  label=XmCreateLabel(viewer,name,args,n);
  XtManageChild(label);
*/

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNrows,24);n++;
  XtSetArg(args[n],XmNcolumns,80);n++;
  XtSetArg(args[n],XmNeditable,False);n++;
  XtSetArg(args[n],XmNeditMode,XmMULTI_LINE_EDIT);n++;
  XtSetArg(args[n],XmNcursorPositionVisible,False);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  text=XmCreateScrolledText(viewer,"text",args,n);
  XtManageChild(text);
  XtVaSetValues(text,XmNbackground,white_pixel,
		     XmNforeground,black_pixel,NULL);
  

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNtopWidget,XtParent(text));n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,50);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  button=XmCreatePushButton(viewer,"CANCEL",args,n);
  XtManageChild(button);
  XtVaGetValues(button,XmNwidth,&d,NULL);
  XtVaSetValues(button,XmNleftOffset,-d/2,NULL);

  XtVaSetValues(XtParent(text),XmNbottomAttachment,XmATTACH_WIDGET,
      XmNbottomWidget,button,NULL);

  /*XtAddCallback(button,XmNactivateCallback,destroy_cb,NULL);*/
  XtAddCallback(XtParent(viewer),XmNpopdownCallback,destroy_cb,NULL);

  XtManageChild(viewer);

  return text;
}
