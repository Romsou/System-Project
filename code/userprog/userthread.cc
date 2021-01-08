
// userthread.cc 
//      
int do_UserThreadCreate(int f,int arg);


/**
 * do_UserThreadExit erases and ends properly current thread
 */
void do_UserThreadExit(){

  delete currentThread->space;
  currentThread->Finish();
}