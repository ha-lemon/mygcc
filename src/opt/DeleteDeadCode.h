#ifndef DELETEDEADCODEH
#define DELETEDEADCODEH

#include "opt.h"

extern std::set<std::string> sysLibFunc;

class DeadCodeDeletion : public Optimization {
  /*funcPptrArgs是存了函数以及对应的指针类型参数的map*/
  std::map<Function *, std::set<Value *>> funcPtrArgs;
  /*storePos是地址值以及使用了该地址值的指令的map*/
  std::map<Value *, std::vector<Value *>> storePos;
  BasicBlock *exitBlock;
  std::set<Instruction *> uselessInstr;
  std::set<BasicBlock *> uselessBlock;

public:
  DeadCodeDeletion(Module *m) : Optimization(m), exitBlock(nullptr) {}
  void execute();
  void initFuncPtrArg();
  void Init(Function *foo);
  bool checkOpt(Function *foo, Instruction *instr);
  void findInstr(Function *foo);
  void deleteInstr(Function *foo);
};

#endif // !DELETEDEADCODEH