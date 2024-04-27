/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "main_frame.h"
#include "axyftp.h"
#include "utils.h"
#include "read_init.h"
#include "functions.h"
#include "pixmaps.h"
#include "dialogs.h"

struct _appstate appdata;

#include <Xm/Xm.h>
#include <Xm/TextF.h>

void xtmesg(String name,String type,String class, String default_s, 
    String* param,Cardinal* num){
  return;
}

appstate appdata;
XtAppContext appcontext;
Widget toplevel;

int main(int argc, char* argv[]){
  char *mask,*env;
  Widget mainwin;

  read_init();

  toplevel=XtAppInitialize(&appcontext,"AxY_FTP",
      NULL,0,&argc,argv,NULL,NULL,0);

  XtAppSetWarningMsgHandler(appcontext,xtmesg);

  XtVaSetValues(toplevel,XmNforeground,BlackPixelOfScreen(XtScreen(toplevel)),
     XmNtitle,"AxY FTP",NULL);

  mainwin=create_main_frame(toplevel);
  XtRealizeWidget(toplevel);
  adjust_main_frame(mainwin);
  
  create_pixmaps();

  make_dialogs(mainwin);
  
  mask=XmTextFieldGetString(appdata.local.text);
  update_local(mask);
  XtFree(mask);
  appdata.job=0;
  appdata.jump_on_cancel=0;
  appdata.connected=0;
  appdata.interrupt=0;

  signal(SIGALRM,SIG_IGN);
  signal(SIGCHLD,SIG_IGN);
  signal(SIGPIPE,SIG_IGN);
/*printf("SHM=%d\n",XShmQueryExtension(XtDisplay(toplevel)));*/
/*  XSynchronize(XtDisplay(toplevel),1);*/
  mask=DisplayString(XtDisplay(toplevel));
  env=XtMalloc(strlen(mask)+12);
  sprintf(env,"DISPLAY = %s",mask);
  putenv(env);
  XtFree(env);

  if(appdata.odata->show_session){
    XtManageChild(appdata.session);
  }

  XtAppMainLoop(appcontext);

  return 0;
}
