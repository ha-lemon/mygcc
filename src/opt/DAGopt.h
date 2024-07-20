#ifndef DAGOPT
#define DAGOPT

#include "opt.h"
class GNode{
    bool isLeaf(){ return childs.size() == 0;};
public:
    std::set<GNode*> childs;
    std::set<Value*> defs;
    Instruction::OpID op_ID;
    std::set<GNode*> fa; //父结点
    bool isused = false;
};
//std::map<Value*,GNode*> ValueToNode;
//std::vector<GNode*> GNodes;
class DAGopt : public Optimization {
public:
  DAGopt(Module *m) : Optimization(m){}
  
  void execute();
  
  void DAG_opt_forBB(BasicBlock* bb);
};
#endif