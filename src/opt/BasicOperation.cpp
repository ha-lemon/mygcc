#include "BasicOperation.h"

void deleteUse(Value *opnd, Instruction *inst) {
  for (auto it = opnd->use_list_.begin(); it != opnd->use_list_.end(); ++it)
    if (it->val_ == inst) {
      opnd->use_list_.erase(it);
      return;
    }
}

void SolvePhi(BasicBlock *bb, BasicBlock *suc) {     //删除suc中的phi指令值的bb来源。
  std::vector<Instruction *> uselessPhi;
  for (auto instr : suc->instr_list_) {
    if (instr->op_id_ == Instruction::PHI) {
      for (int i = 1; i < instr->num_ops_; i = i + 2)
        if (instr->get_operand(i) == bb) {
          instr->remove_operands(i - 1, i);
          break;
        }
      if (instr->parent_->pre_bbs_.size() == 1) {
        Value *only = instr->get_operand(0);
        instr->replace_all_use_with(only);
        uselessPhi.push_back(instr);
      }
    }
  }
  for (auto instr : uselessPhi)
    suc->delete_instr(instr);
}

void dfsGraph(BasicBlock *bb, std::set<BasicBlock *> &vis) {
  if (!bb)
    return;
  vis.insert(bb);
  for (auto suc : bb->succ_bbs_) {
    if (vis.find(suc) == vis.end())
      dfsGraph(suc, vis);
  }
}

void DeleteUnusedBB(Function *func) {   //遍历func中的所有bb，然后保存在vis中，再删除func中不在vis中的bb
  std::set<BasicBlock *> vis;
  for (auto bb : func->basic_blocks_)
    if (bb->name_ == "label_entry") {
      dfsGraph(bb, vis);
      break;
    }
  for (auto bb : func->basic_blocks_)
    if (vis.find(bb) == vis.end()) {
      bb->parent_->remove_bb(bb);
      for (auto suc : bb->succ_bbs_)
        SolvePhi(bb, suc);
    }
}
