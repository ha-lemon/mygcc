#ifndef LIVEVOPT
#define LIVEVOPT

#include "opt.h"

class LiveVopt : public Optimization {
public:
  LiveVopt(Module *m) : Optimization(m){}
  /*删除无用变量*/
  void execute();
  /*获取各block的livein和liveout*/
  void getBlockLiveV(Function* foo);
};
#endif // !DELETEDEADCODEH