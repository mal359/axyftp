/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#ifdef USE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif

#include "axyftp.h"
#include "utils.h"
#include "functions.h"
#include "little_dialogs.h"
#include "dirinfo.h"
#include "dirlist.h"
#include "pixmaps.h"

#define NUMFIELDS 5

#include <Xm/Xm.h>
#include <Xm/TextF.h>
#include <XmAxy/List.h>


static void doubleclick_cb(Widget,XtPointer,XtPointer);

void doubleclick_cb(Widget w,XtPointer app,XtPointer call){
  int which;
  XmAxyListDefaultActionCallbackStruct *cbs;
  int ret;

  which=(int)app;
  cbs=(XmAxyListDefaultActionCallbackStruct*)call;

  switch(which){
    case LOCAL:
      busy_cursor(True);
      ret=chg_local_dir(cbs->row);
      if(!ret){
         char* mask=XmTextFieldGetString(appdata.local.text);
	 update_local(mask);
	 free(mask);
      }
      if(!appdata.job)busy_cursor(False);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
        break;
      }
      appdata.job=5;
      busy_cursor(True);
      ret=chg_remote_dir(cbs->row);
      if(!ret){
         char* mask=XmTextFieldGetString(appdata.remote.text);
	 update_remote(mask);
	 free(mask);
      }
      busy_cursor(False);
      appdata.job=0;
      break;
  }
  /*
  printf("DOUBLECLICK for %s row #%d\n",(which==REMOTE)?"remote":"local",
      cbs->row);
      */
  return;
}

void cancel_table_update(Widget w){
/*  XbaeMatrixDisableRedisplay(w);*/
}

void restore_table_update(Widget w){
  XmAxyListRefresh(w);
}

int* get_selected_rows(Widget w){
  int num,*list;
  int *retval;

  if(!XmAxyListGetSelectedRows(w,&list,&num)){
    return NULL;
  }

  retval=(int*)realloc((char*)list,sizeof(int)*(num+1));
  retval[num]=-1;
  return retval;
}

int get_selected_row(Widget w){
  int num,*list;
  int retval;
  if(!XmAxyListGetSelectedRows(w,&list,&num)){
    return NONE;
  }
  if(num>1) retval=MANY;
  else retval=list[0];
  free((char*)list);
  return retval;
}

static void redraw_cb(Widget,XtPointer,XtPointer);
/*
static void enter_cb(Widget,XtPointer,XtPointer);
static void select_cb(Widget,XtPointer,XtPointer);
*/

void update_dirlist(Widget table,dirinfo* di){
  XtCallbackRec rw[2];
  int i,temp;
  short max[NUMFIELDS];
  Pixel **colors;
  Pixel white_pixel;
  
  rw[1].callback=NULL;
  rw[1].closure=NULL;
  rw[0].callback=redraw_cb;
  rw[0].closure=di;
  
  white_pixel=WhitePixelOfScreen(XtScreen(table));
  colors=(Pixel**)malloc(di->total*sizeof(Pixel*));
  for(i=0;i<di->total;i++){
    colors[i]=(Pixel*)malloc(NUMFIELDS*sizeof(Pixel));
    for(temp=0;temp<NUMFIELDS;temp++)colors[i][temp]=white_pixel;
  }

  max[0]=4;for(i=1;i<NUMFIELDS;i++)max[i]=6;
  for(i=1;i<=di->total;i++){
    if(max[1]<(temp=strlen(di->files[i]->name)))max[1]=temp;
    if(max[2]<(temp=2+strlen(di->files[i]->date)))max[2]=temp;
    if(max[3]<(temp=2+strlen(di->files[i]->time)))max[3]=temp;
    if(max[4]<(temp=strlen(di->files[i]->size_str)))max[4]=temp;
  } 

  XtVaSetValues(table,
      XmNrowCount,di->total,
      /*XmNcellBackgrounds,colors,*/
      XmNdrawCellCallback,rw,
      NULL);

  if(appdata.odata->columnadjustment){
    temp=atoi(appdata.odata->maxwidth);
    if(temp>0)if(max[1]>temp)max[1]=temp;
    XtVaSetValues(table,XmNcolumnWidths,max,NULL);
  }

  /*XbaeMatrixDeselectAll(table);*/
  for(i=0;i<di->total;i++) free((char*)colors[i]);
  free((char*)colors);

  return;
}

void redraw_cb(Widget w,XtPointer app,XtPointer call){
  dirinfo *di;
  XmAxyListDrawCellCallbackStruct *cs;
  
  cs=(XmAxyListDrawCellCallbackStruct*)call;
  di=(dirinfo*)app;
  cs->type=XmSTRING;
  switch(cs->column){
    case 0:
      switch(di->files[cs->row+1]->perms[0]){
        case 'd':
	  if(strcmp(di->files[cs->row+1]->name,"..")==0){
	    cs->pixmap=pixmap[0];
	    cs->pixmask=mask[0];
	  } else {
	    cs->pixmap=pixmap[1];
	    cs->pixmask=mask[1];
	  }
	  cs->string=" D";
	  break;

	case 'l':
	  cs->pixmap=pixmap[3];
	  cs->pixmask=mask[3];
	  cs->string=" L";
	  break;

	default:
	  cs->pixmap=pixmap[2];
	  cs->pixmask=mask[2];
	  cs->string=" F";
	  break;
      }
      if(cs->pixmap && cs->pixmask){
	cs->type=XmPIXMAP;
      } else {
	cs->type=XmSTRING;
      }
      break;
	      
    case 1:
      cs->string=di->files[cs->row+1]->name;
      break;

    case 2:
      cs->string=di->files[cs->row+1]->date;
      break;

    case 3:
      cs->string=di->files[cs->row+1]->time;
      break;

    case 4:
      cs->string=di->files[cs->row+1]->size_str;
      break;

    default:
      break;
  }
  return;
}

void clear_dirlist(Widget table){
  static short mw[NUMFIELDS]={4,6,6,6,6};

  XtVaSetValues(table,XmNrowCount,0,XmNcolumnWidths,mw,
      XmNdrawCellCallback,NULL,NULL);
}

struct _select {
  int anchor;
  int extend;
  int select;
};

Widget create_dirlist(Widget parent,int which){
  Widget list;
  Arg args[20];
  Cardinal n;
  static short mw[NUMFIELDS]={4,6,6,6,6};
  static unsigned char  ca[NUMFIELDS]=
   {XmALIGNMENT_CENTER,XmALIGNMENT_BEGINNING,
    XmALIGNMENT_CENTER,XmALIGNMENT_CENTER,XmALIGNMENT_END};
  static unsigned char  la[NUMFIELDS]=
   {XmALIGNMENT_CENTER,XmALIGNMENT_CENTER,XmALIGNMENT_CENTER,
    XmALIGNMENT_CENTER,XmALIGNMENT_CENTER};
    /*
  Pixel white_pixel,black_pixel;
  Colormap cmap;
  XColor color,exact;
  */

  static String labels[NUMFIELDS]={"^","Name","Date","Time","Size"};

  /*
  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  */

  n=0;
  XtSetArg(args[n],XmNrowCount,0);n++;
  XtSetArg(args[n],XmNselectionPolicy,XmEXTENDED_SELECT);n++;
  /*XtSetArg(args[n],XmNselectedForeground,white_pixel);n++;*/
  /*XtSetArg(args[n],XmNforeground,black_pixel);n++;*/
  XtSetArg(args[n],XmNcolumnWidths,mw);n++;
  /*XtSetArg(args[n],XmNbuttonLabels,True);n++;*/
  /*XtSetArg(args[n],XmNcellHighlightThickness,0);n++;*/
  XtSetArg(args[n],XmNcellMarginWidth,2);n++;
  XtSetArg(args[n],XmNcolumnCount,NUMFIELDS);n++;
  XtSetArg(args[n],XmNcolumnLabels,labels);n++;
  XtSetArg(args[n],XmNcolumnLabelAlignments,la);n++;
  XtSetArg(args[n],XmNcolumnAlignments,ca);n++;
  XtSetArg(args[n],XmNrowSpacing,2);n++;
  XtSetArg(args[n],XmNcellMarginHeight,5);n++;
  XtSetArg(args[n],XmNlabelMarginHeight,4);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  /*XtSetArg(args[n],XmNfill,True);n++;*/
  /*XtSetArg(args[n],XmNspace,0);n++;*/
  /*XtSetArg(args[n],XmNshowArrows,True);n++;*/
  /*XtSetArg(args[n],XmNgridType,XmGRID_NONE);n++;*/
  /*XtSetArg(args[n],XmNallowColumnResize,True);n++;*/
  list=XmAxyCreateScrolledList(parent,"dirlist",args,n);
  XtManageChild(list);

  /*
  XtVaGetValues(list,XmNcolormap,&cmap,NULL);
  if(XAllocNamedColor(XtDisplay(list),cmap,"#2F2FCF",&color,&exact)){
    XtVaSetValues(list,XmNselectedBackground,color.pixel,NULL);
  }
  */

  /*XtOverrideTranslations(list,XtParseTranslationTable(translations));*/
  
  /*XtAddCallback(list,XmNenterCellCallback,enter_cb,NULL);*/
  /*xp=(XtPointer)XtNew(struct _select);*/
  /*XtAddCallback(list,XmNselectCellCallback,select_cb,xp);*/

  XtAddCallback(list,XmNdefaultActionCallback,doubleclick_cb,(XtPointer)which);

  switch(which){
    case LOCAL:
      appdata.local.table=list;
      break;
    case REMOTE:
      appdata.remote.table=list;
      break;
    default:
      break;
  }
  
  return XtParent(list);
}



  /*
void deselect_rows(Widget w,int start,int end){
  if(start<end){
    for(;start<=end;end--){
      XbaeMatrixDeselectRow(w,end);
    }
  } else {
    for(;end<=start;end++){
      XbaeMatrixDeselectRow(w,end);
    }
  }
}

  */
  /*
void select_rows(Widget w,int start,int end){
  if(start<end){
    for(;start<=end;end--){
      XbaeMatrixSelectRow(w,end);
    }
  } else {
    for(;end<=start;end++){
      XbaeMatrixSelectRow(w,end);
    }
  }
}

  */
#if 0
void select_cb(Widget w,XtPointer app,XtPointer call){
  int top;
  struct _select * s;
  XbaeMatrixSelectCellCallbackStruct *cbs;
  cbs=(XbaeMatrixSelectCellCallbackStruct *) call;
  s=(struct _select *)app;

  /*
  printf("Row/Column: %d/%d \n%d params: ",
      cbs->row,cbs->column,cbs->num_params);
  for(i=0;i<cbs->num_params;i++)printf("%s ",cbs->params[i]);
  printf("\n");
  */


  XtVaGetValues(w,XmNrows,&top,NULL);
  if(top<=cbs->row)return;

  switch(cbs->params[0][0]){
    case 'S': 
      XbaeMatrixDeselectAll(w);
      s->anchor=cbs->row;
      s->extend=-1;
      s->select=1;
      XbaeMatrixSelectRow(w,s->anchor);
      break;

    case 'T':
      if(XbaeMatrixIsRowSelected(w,cbs->row)){
	XbaeMatrixDeselectRow(w,cbs->row);
	s->select=-1;
      } else {
	XbaeMatrixSelectRow(w,cbs->row);
	s->select=1;
      }
      s->anchor=cbs->row;
      s->extend=-1;
      break;

    case 'E':
      XbaeMatrixDisableRedisplay(w);
      XtVaGetValues(w,XmNtopRow,&top,NULL);
      if(s->extend>=0){
	deselect_rows(w,s->extend,s->anchor);
      }
      s->extend=cbs->row;
      if(s->select==1){
	select_rows(w,cbs->row,s->anchor);
      } else {
	deselect_rows(w,cbs->row,s->anchor);
      }
      XtVaSetValues(w,XmNtopRow,top,NULL);
      XbaeMatrixEnableRedisplay(w,True);
      break;

    case 'e':
      XbaeMatrixDisableRedisplay(w);
      if(s->extend>=0){
        if(s->select==1){
	  deselect_rows(w,s->extend,s->anchor);
	} else {
	  select_rows(w,s->extend,s->anchor);
	}
      }
      s->extend=cbs->row;
      if(s->select==1){
	select_rows(w,cbs->row,s->anchor);
      } else {
	deselect_rows(w,cbs->row,s->anchor);
      }
      XbaeMatrixEnableRedisplay(w,True);
      break;

    case 'U':
      XbaeMatrixDisableRedisplay(w);
      XtVaGetValues(w,XmNtopRow,&top,NULL);
      XbaeMatrixUnhighlightAll(w);
      XbaeMatrixHighlightRow(w,cbs->row);
      XtVaSetValues(w,XmNtopRow,top,NULL);
      XbaeMatrixEnableRedisplay(w,True);
      break;
  }
  return; 
}

void enter_cb(Widget w,XtPointer app,XtPointer call){
  /*
  XbaeMatrixEnterCellCallbackStruct *cbs=
   (XbaeMatrixEnterCellCallbackStruct*) call;
  cbs->map=False;
  cbs->doit=False;
  */
}
#endif
