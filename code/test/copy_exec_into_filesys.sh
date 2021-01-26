#!/bin/bash

#format disk
./../build/nachos-step5 -f

#part Get/Put
./../build/nachos-step5 -cp ../build/getchar getchar
./../build/nachos-step5 -cp ../build/getstring getstring
./../build/nachos-step5 -cp ../build/getint getint
./../build/nachos-step5 -cp ../build/putchar putchar
./../build/nachos-step5 -cp ../build/putstring putstring
./../build/nachos-step5 -cp ../build/putint putint

#part userthread
./../build/nachos-step5 -cp ../build/userthreadcreate_automaticend automaticend
./../build/nachos-step5 -cp ../build/userthreadcreate_fewthreads fewthreads
./../build/nachos-step5 -cp ../build/userthreadcreate_join join
./../build/nachos-step5 -cp ../build/userthreadcreate_manythreads manythreads

#part processes
./../build/nachos-step5 -cp ../build/userpages0 userpages0
./../build/nachos-step5 -cp ../build/userpages1 userpages1
./../build/nachos-step5 -cp ../build/process_12threads process_12threads
./../build/nachos-step5 -cp ../build/process_12processes process_12processes 
./../build/nachos-step5 -cp ../build/process_forkexec process_forkexec


#part file system
./../build/nachos-step5 -cp ../build/filesys_create filesys_create
./../build/nachos-step5 -cp ../build/filesys_userthread filesys_userthread
./../build/nachos-step5 -cp ../build/filesys_readwrite filesys_readwrite
./../build/nachos-step5 -cp ../build/filesys_writeread filesys_writeread