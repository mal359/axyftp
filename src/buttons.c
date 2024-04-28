/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <unistd.h>
#include <stdlib.h>
#ifdef USE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif

#include "axyftp.h"
#include "utils.h"
#include "multi.h"
#include "buttons.h"
#include "dirlist.h"
#include "functions.h"
#include "little_dialogs.h"
#include "prompt_dialogs.h"

#include <Xm/Screen.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/TextF.h>


static void chgdir_cb(Widget,XtPointer,XtPointer);
static void delete_cb(Widget,XtPointer,XtPointer);
static void refresh_cb(Widget,XtPointer,XtPointer);
static void view_cb(Widget,XtPointer,XtPointer);
static void mkdir_cb(Widget,XtPointer,XtPointer);
static void exec_cb(Widget,XtPointer,XtPointer);
static void rename_cb(Widget,XtPointer,XtPointer);
static void dirinfo_cb(Widget,XtPointer,XtPointer);

void dirinfo_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;

  switch(which){
    case LOCAL:
      show_local_dirinfo();
      break;
    case REMOTE:
      if(!appdata.remote.list){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      show_remote_dirinfo();
      break;
  }
  return;
}

void rename_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;
  int selrow;

  switch(which){
    case LOCAL:
      selrow=get_selected_row(appdata.local.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one local file should be selected");
	  break;
	default:
	  rename_local_num(selrow);
	  break;
      }
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_row(appdata.remote.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one remote file should be selected");
          break;
	default:
	  rename_remote_num(selrow);
	  break;
      }
      break;
    default:
      break;
  }
  return;
}

void exec_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;

  switch(which){
    case LOCAL:
      XtManageChild(appdata.exec_local);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      XtManageChild(appdata.exec_remote);
      break;
  }
  return;
}

void mkdir_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;

  switch(which){
    case LOCAL:
      XtManageChild(appdata.mkdir_local);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      XtManageChild(appdata.mkdir_remote);
      break;
  }
  return;
}

void view_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;
  int selrow;

  switch(which){
    case LOCAL:
      selrow=get_selected_row(appdata.local.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one local file should be selected");
	  break;
	default:
	  busy_cursor(True);
	  view_local_num(selrow);
	  if(!appdata.job)busy_cursor(False);
	  break;
      }
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_row(appdata.remote.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one remote file should be selected");
          break;
	default:
	  appdata.job=3;
	  busy_cursor(True);
	  view_remote_num(selrow);
	  busy_cursor(False);
	  appdata.job=0;
	  break;
      }
      break;
    default:
      break;
  }
  return;
}

void refresh_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;
  char* mask;

  switch(which){
    case LOCAL:
      busy_cursor(True);
      XmUpdateDisplay(toplevel);
      mask=XmTextFieldGetString(appdata.local.text);
      update_local(mask);
      XtFree(mask);
      if(!appdata.job)busy_cursor(False);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      appdata.job=7;
      busy_cursor(True);
      XmUpdateDisplay(toplevel);
      mask=XmTextFieldGetString(appdata.remote.text);
      update_remote(mask);
      XtFree(mask);
      busy_cursor(False);
      appdata.job=0;
      break;
  }
  return;
}

void delete_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;
  int *selrow;

  switch(which){
    case LOCAL:
      selrow=get_selected_rows(appdata.local.table);
      if(selrow==NULL){
	(void)popup_warning_dialog(toplevel,"Nothing selected");
	return;
      }
      busy_cursor(TRUE);
      if(!delete_local_files(selrow)){
	char* mask=XmTextFieldGetString(appdata.local.text);
	update_local(mask);
	free(mask);
      }
      if(!appdata.job)busy_cursor(FALSE);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_rows(appdata.remote.table);
      if(selrow==NULL){
	(void)popup_warning_dialog(toplevel,"Nothing selected");
	return;
      }
      appdata.job=4;
      busy_cursor(TRUE);
      if(!delete_remote_files(selrow)){
	char* mask=XmTextFieldGetString(appdata.remote.text);
	update_remote(mask);
	free(mask);
      }
      busy_cursor(FALSE);
      appdata.job=0;
      break;
    default:
      break;
  }
  return;
}

void chgdir_cb(Widget w,XtPointer app,XtPointer cbs){
  int which=(int)app;
  int selrow;
  char* mask;
  int ret;

  switch(which){
    case LOCAL:
      selrow=get_selected_row(appdata.local.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one local entry should be selected");
	  break;
	default:
	  busy_cursor(True);
	  XmUpdateDisplay(toplevel);
	  ret=chg_local_dir(selrow);
	  mask=XmTextFieldGetString(appdata.local.text);
	  if(!ret)update_local(mask);
	  XtFree(mask);
	  if(!appdata.job)busy_cursor(False);
	  break;
      }
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_row(appdata.remote.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one remote entry should be selected");
          break;
	default:
	  appdata.job=5;
	  busy_cursor(True);
	  XmUpdateDisplay(toplevel);
	  ret=chg_remote_dir(selrow);
	  mask=XmTextFieldGetString(appdata.remote.text);
	  if(!ret)update_remote(mask);
	  XtFree(mask);
	  busy_cursor(False);
	  appdata.job=0;
	  break;
      }
      break;
    default:
      break;
  }
  return;
}


Widget create_buttons(Widget parent,int which){
  Widget bar;
  Arg args[10];
  Cardinal n;
  Widget chdir,mkdir,text,view,exec,rename,delete,refresh,dirinfo;
  Dimension h,w;

  n=0;
  XtSetArg(args[n],XmNfractionBase,9);n++;
  bar=XmCreateForm(parent,"buttons",args,n);
  XtManageChild(bar);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,1);n++;
  */
  chdir=XmCreatePushButton(bar,"ChgDir",args,n);
  XtManageChild(chdir);
  XtAddCallback(chdir,XmNactivateCallback,chgdir_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,chdir);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,2);n++;
  */
  mkdir=XmCreatePushButton(bar,"MkDir",args,n);
  XtManageChild(mkdir);
  XtAddCallback(mkdir,XmNactivateCallback,mkdir_cb,(XtPointer)which);

  n=0;
  /*XtSetArg(args[n],XmNhighlightThickness,0);n++;*/
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,mkdir);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,3);n++;
  */
  XtSetArg(args[n],XmNresizeWidth,True);n++;
  XtSetArg(args[n],XmNbackground,WhitePixelOfScreen(XtScreen(parent)));n++;
  text=XmCreateTextField(bar,"text",args,n);
  XtManageChild(text);
  XtVaGetValues(text,XmNheight,&h,NULL);
  XtVaSetValues(mkdir,XmNheight,h,NULL);
  XtVaSetValues(chdir,XmNheight,h,NULL);
  XtAddCallback(text,XmNactivateCallback,refresh_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,text);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,4);n++;
  */
  view=XmCreatePushButton(bar,"View",args,n);
  XtManageChild(view);
  XtAddCallback(view,XmNactivateCallback,view_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,view);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,5);n++;
  */
  exec=XmCreatePushButton(bar,"Exec",args,n);
  XtManageChild(exec);
  XtAddCallback(exec,XmNactivateCallback,exec_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,exec);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,6);n++;
  */
  rename=XmCreatePushButton(bar,"Rename",args,n);
  XtManageChild(rename);
  XtAddCallback(rename,XmNactivateCallback,rename_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,rename);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,7);n++;
  */
  delete=XmCreatePushButton(bar,"Delete",args,n);
  XtManageChild(delete);
  XtAddCallback(delete,XmNactivateCallback,delete_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,delete);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,8);n++;
  */
  refresh=XmCreatePushButton(bar,"Refresh",args,n);
  XtManageChild(refresh);
  XtAddCallback(refresh,XmNactivateCallback,refresh_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,refresh);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  */
  dirinfo=XmCreatePushButton(bar,"DirInfo",args,n);n++;
  XtManageChild(dirinfo);
  XtAddCallback(dirinfo,XmNactivateCallback,dirinfo_cb,(XtPointer)which);

  XtVaGetValues(refresh,XmNwidth,&w,NULL);
  XtVaSetValues(chdir,XmNwidth,w,NULL);
  XtVaSetValues(mkdir,XmNwidth,w,NULL);
  XtVaSetValues(text,XmNwidth,w,NULL);
  XtVaSetValues(text,XmNresizeWidth,False,NULL);
  XtVaSetValues(view,XmNwidth,w,NULL);
  XtVaSetValues(exec,XmNwidth,w,NULL);
  XtVaSetValues(rename,XmNwidth,w,NULL);
  XtVaSetValues(delete,XmNwidth,w,NULL);
  XtVaSetValues(dirinfo,XmNwidth,w,NULL);

  switch(which){
    case LOCAL:
      appdata.local.text=text;
      break;
    case REMOTE:
      appdata.remote.text=text;
      break;
    default:
      break;
  }
  
  return bar;
}
