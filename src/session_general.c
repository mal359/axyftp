/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <string.h>
#include <stdlib.h>
#ifdef USE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif

#include "session_general.h"
#include "session_dialog.h"
#include "axyftp.h"
#include "utils.h"
#include "read_init.h"

#define MAXPASS 40

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/TextF.h>
#include <Xm/ComboBox.h>
#include <Xm/Label.h>
#include <Xm/List.h>

static void password_cb(Widget,XtPointer,XtPointer);
static void anon_cb(Widget,XtPointer,XtPointer);
static void listaction_cb(Widget,XtPointer,XtPointer);
static void listselect_cb(Widget,XtPointer,XtPointer);

static void listselect_cb(Widget w,XtPointer app,XtPointer call){
  XmComboBoxCallbackStruct *cbs;
  int* pos;
  int num;
  Widget list;

  cbs=(XmComboBoxCallbackStruct*)call;
  list=XtNameToWidget(w,"*List");

  if(XmListGetSelectedPos(list,&pos,&num)){
    update_session_dialog(pos[0],'\0');
  }
  return;
}

static void listaction_cb(Widget w,XtPointer app,XtPointer call){
  XmString xms;
  String s;
  Widget list,combo;
  session_data *sd,*temp;
  int *poslist;
  int poscount,i;

  XtVaGetValues(w,XmNlabelString,&xms,NULL);

  if(XmStringGetLtoR(xms,XmFONTLIST_DEFAULT_TAG,&s)){
    if(strcmp(s,"New")==0){
      list=XtNameToWidget(appdata.session,"*profile*List");
      if(XmListGetSelectedPos(list,&poslist,&poscount))
			  XmListDeselectPos(list,poslist[0]);
      /*
      XmListGetSelectedPos(list,&poslist,&poscount);
      combo=XtNameToWidget(appdata.session,"*profile");
      XtVaGetValues(combo,XmNselectedPosition,&i,NULL);
      printf("Selected positions after deselect: %d %d\n",poscount,i);
      */
      sd=create_session_data(NULL);
      empty_session_data(sd);
      put_session_data(appdata.session,sd,(char)1);
      destroy_session_data(sd);
    } else if(strcmp(s,"Delete")==0){
      combo=XtNameToWidget(appdata.session,"*profile");
      list=XtNameToWidget(combo,"*List");
      if(XmListGetSelectedPos(list,&poslist,&poscount)){
        for(i=0,sd=appdata.sdata;i<poslist[0]-1;i++)sd=sd->next;
	temp=sd->next;
	sd->next=temp->next;
	temp->next=NULL;
	destroy_session_data(temp);

	set_profile_strings(combo);
	/*
	XtVaGetValues(list,XmNitemCount,&i,NULL);
	XmComboBoxDeletePos(list,poslist[0]);
	i--;
	if(i<=MAX_VISIBLE_ITEMS){
	  XtVaSetValues(combo,XmNvisibleItemCount,i>0?i:1,NULL);
	  XtVaSetValues(list,XmNvisibleItemCount,i>0?i:1,NULL);
	}
	*/
	/*XmListDeselectAllItems(list);*/
	sd=create_session_data(NULL);
	empty_session_data(sd);
	put_session_data(appdata.session,sd,(char)1);
	destroy_session_data(sd);
	write_session_data(session_file,appdata.sdata);
      }
    }
    free(s);
  }
  XmStringFree(xms);
}

static void anon_cb(Widget w,XtPointer app,XtPointer call){
  XmToggleButtonCallbackStruct *cbs;
  Widget passfield;
  char *pass,*save,*anon;
  char* stars;

  cbs=(XmToggleButtonCallbackStruct*)call;
  passfield=(Widget)app;
  XtVaGetValues(passfield,XmNuserData,&pass,NULL);

  if(cbs->set){
    XmTextFieldSetString(passfield,appdata.odata->anonpass);
    passfield=XtNameToWidget(appdata.session,"*user");
    XmTextFieldSetString(passfield,"anonymous");
  } else {
    stars=malloc(strlen(pass)+1);
    memset(stars,'*',strlen(pass));
    stars[strlen(pass)]='\0';
    save=XtNewString(pass);
    anon=XmTextFieldGetString(passfield);
    memmove(pass, anon, strlen(pass) + 1);
    XmTextFieldSetString(passfield,stars);
    memmove(pass, save, strlen(pass) + 1);
    free(anon);
    free(save);
    free(stars);
  }

}

static void password_cb(Widget w,XtPointer app,XtPointer call){
  XmTextVerifyCallbackStruct *cbs;
  Widget anon;
  char* pass;

  cbs=(XmTextVerifyCallbackStruct*)call;
  anon=(Widget)app;
  XtVaGetValues(w,XmNuserData,&pass,NULL);

  if(XmToggleButtonGetState(anon)){
    return;
  }

  if(cbs->startPos<cbs->endPos){
    memmove(&pass[cbs->startPos],&pass[cbs->endPos],
	strlen(pass)-cbs->endPos+1);
  }
  if(cbs->text->length==0){
    return;
  }
  memmove(&pass[cbs->startPos+cbs->text->length],&pass[cbs->startPos],
	strlen(pass)-cbs->startPos+1);
  memmove(&pass[cbs->startPos],cbs->text->ptr,cbs->text->length);
  memset(cbs->text->ptr,'*',cbs->text->length);
  return;
    
}

Widget create_session_general(Widget parent){
  static String label[]={
    "Profile Name:",
    "Host Name/Address:",
    "User ID:",
    "Password:",
    "Account:",
    "Comment:"
  };
  Widget general;
  Arg args[20];
  Dimension width;
  Cardinal n;
  int i;
  char* passbuf;
  Widget current,child,commentlabel,passfield,anontoggle;
  Pixel white_pixel;
  Pixel black_pixel;

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  general=XmCreateForm(parent,"General",args,n);
  XtManageChild(general);

  /* first column */
  for(i=0;i<XtNumber(label)-1;i++){
    n=0;
    XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
    XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
    XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
    XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
    XtSetArg(args[n],XmNrightPosition,30);n++;
    XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
    XtSetArg(args[n],XmNbottomPosition,100*(i+1)/XtNumber(label));n++;
    current=XmCreateLabel(general,label[i],args,n);
    XtManageChild(current);
  }

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  commentlabel=XmCreateLabel(general,label[i],args,n);
  XtManageChild(commentlabel);

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  /*second column */
  /*
  n=0;
  XtSetArg(args[n],XmNcomboBoxType,XmDROP_DOWN_COMBO_BOX);n++;
  XtSetArg(args[n],XmNselectionPolicy,XmBROWSE_SELECT);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*1/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateComboBox(general,"profile",args,n);
  XtManageChild(current);
  XtAddCallback(current,XmNselectionCallback,listselect_cb,NULL);
  child=XtNameToWidget(current,"*Text");
  XtVaSetValues(child,XmNbackground,white_pixel, NULL);
  child=XtNameToWidget(current,"*List");
  XtVaSetValues(child,XmNbackground,white_pixel,NULL);
  */
  n=0;
  XtSetArg(args[n],XmNcomboBoxType,XmDROP_DOWN_COMBO_BOX);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*1/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateComboBox(general,"profile",args,n);
  XtManageChild(current);
  XtAddCallback(current,XmNselectionCallback,listselect_cb,NULL);

  child=XtNameToWidget(current,"*Text");
  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNmarginWidth,3);n++;/* Motif 2.x workaround */
  XtSetArg(args[n],XmNmarginHeight,3);n++;
  XtSetValues(child,args,n);

  child=XtNameToWidget(current,"*List");
  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNselectionPolicy,XmBROWSE_SELECT);n++;
  XtSetValues(child,args,n);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateTextField(general,"host",args,n);
  XtManageChild(current);


  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNbottomPosition,100*3/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateTextField(general,"user",args,n);
  XtManageChild(current);

  passbuf=malloc(MAXPASS+1);
  passbuf[0]='\0';
  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*4/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  XtSetArg(args[n],XmNmaxLength,MAXPASS);n++;
  XtSetArg(args[n],XmNuserData,(XtPointer)passbuf);n++;
  passfield=current=XmCreateTextField(general,"pass",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*5/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateTextField(general,"account",args,n);
  XtManageChild(current);

  n=0; /* comment text field */
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,commentlabel);n++;
  current=XmCreateTextField(general,"comment",args,n);
  XtManageChild(current);

  /* third column */
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*1/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,85);n++;
  current=XmCreatePushButton(general,"New",args,n);
  XtManageChild(current);
  XtAddCallback(current,XmNactivateCallback,listaction_cb,NULL);


  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,85);n++;
  child=XmCreatePushButton(general,"Delete",args,n);
  XtManageChild(child);
  XtAddCallback(child,XmNactivateCallback,listaction_cb,NULL);

  XtVaGetValues(child,XmNwidth,&width,NULL);
  XtVaSetValues(child,XmNleftOffset,-width/2,NULL);
  XtVaSetValues(current,XmNwidth,width,XmNleftOffset,-width/2,NULL);

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*3/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  anontoggle=current=XmCreateToggleButton(general,"Anonymous",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*4/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  current=XmCreateToggleButton(general,"Save Password",args,n);
  XtManageChild(current);

  XtAddCallback(passfield,XmNmodifyVerifyCallback,password_cb,
      (XtPointer)anontoggle);
  XtAddCallback(anontoggle,XmNvalueChangedCallback,anon_cb,
      (XtPointer)passfield);

  return general;
}

