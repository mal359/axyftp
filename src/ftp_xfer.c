/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#ifdef USE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pthread.h>

#include "ftp_xfer.h"

static volatile sig_atomic_t interrupt = 0;

static void handler(int sig){
  interrupt = 1;
}

static int remove_cr(char* buf,int len){
  int i,j;
  for(i=0,j=0;j<len;j++){
    if(buf[j]!='\r')buf[i++]=buf[j];
  }
  return i;
}

static int insert_cr(char* buf,int len,int total){
  int i,j;
  for(i=len,j=total;i>0;){
    if((buf[--j]=buf[--i])=='\n'){
      buf[--j]='\r';
      if(i>=0 && buf[i]=='\r')i--;
    }
  }
  memmove(buf,&buf[j],len=total-j);
  return len;
}

int ftp_xfer_get(char mode,int remote,int local,ftp_xfer_proc proc,void* arg){
  char* buf = (char*)malloc(1024);
  fd_set selset;
  struct timeval seltime;
  int retval;

  struct sigaction sa;
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGPIPE, &sa, NULL);
  
  interrupt = 0;

  for(;;){
    FD_ZERO(&selset);
    FD_SET(remote,&selset);
    seltime.tv_sec=0;
    seltime.tv_usec=100000;
    retval=select(remote+1,&selset,NULL,NULL,&seltime);
    if(retval<0){
      interrupt=0;
      return 1;
    }
    if(retval==0){
      if(proc && (*proc)(0,arg)){
	interrupt=0;
	return -1;
      }
    } else {
      if((retval=read(remote,buf,1024))<0){
	interrupt=0;
	return -1;
      }
      if(retval==0){
	interrupt=0;
	return 0;
      }
      if(mode=='A' || mode=='a' || mode=='T' || mode=='t'){
	retval=remove_cr(buf,retval);
      }
      if(write(local,buf,retval)<0){
	interrupt=0;
	return 2;
      }
      if(proc && (*proc)(retval,arg)){
	interrupt=0;
	return -1;
      }
      if(interrupt){
	interrupt=0;
	return 1;
      }
    }
  }
}   

int ftp_xfer_put(char mode,int remote,int local,ftp_xfer_proc proc,void* arg){
  char* buf = (char*)malloc(1024);
  fd_set selset;
  struct timeval seltime;
  int retval,len;

  struct sigaction sa;
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGPIPE, &sa, NULL);
    
  interrupt=0;

  for(;;){
    if((len=read(local,buf,
	(mode=='A' || mode=='a' || mode=='T' || mode=='t')?512:1024))<0){
      interrupt=0;
      return 2;
    }
    if(len==0){
      interrupt=0;
      return 0;
    }
    if(mode=='A' || mode=='a' || mode=='T' || mode=='t'){
      len=insert_cr(buf,len,1024);
    }

    for(;;){
      FD_ZERO(&selset);
      FD_SET(remote,&selset);
      seltime.tv_sec=0;
      seltime.tv_usec=100000;
      retval=select(remote+1,NULL,&selset,NULL,&seltime);
      if(retval<0){
	interrupt=0;
	return 1;
      }
      if(retval==0){
	if(proc && (*proc)(0,arg)){
	  interrupt=0;
	  return -1;
	}
      } else {
	if(write(remote,buf,len)<0){
	  interrupt=0;
	  return 1;
	}
	if(proc && (*proc)(len,arg)){
	  interrupt=0;
	  return -1;
	}
	if(interrupt){
	  interrupt=0;
	  return 1;
	}
	break;
      }
    }
  }
}   
