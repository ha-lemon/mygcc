#include "DAGopt.h"
#include<algorithm>
void DAGopt::DAG_opt_forBB(BasicBlock* BB){
    std::map<Value*,GNode*> ValueToNode;
    std::vector<GNode*> GNodes;
    for(auto instr : BB->instr_list_){


        if(instr->num_ops_ == 0) continue;
        //操作数是否都加入了结点
        bool isAllInNode = true;
        for(int i = 0 ; i < instr->num_ops_ ; i++){
            if(ValueToNode.count(instr->get_operand(i)) == 0){
                isAllInNode = false;
                break;
            }
        }


        //全部都加入了结点,则检查是否有与instr同样的结点
        if(isAllInNode){   
            std::set<GNode*> pa_set;
            pa_set = ValueToNode[instr->get_operand(0)]->fa;
            for(int i = 1 ; i < instr->num_ops_ ; i++){
                std::set<GNode*> sec;
                std::set_intersection(pa_set.begin(),pa_set.end(),ValueToNode[instr->get_operand(i)]->fa.begin(),ValueToNode[instr->get_operand(i)]->fa.end(),std::inserter(sec,sec.begin()));//求交集
                pa_set = sec;
            }
            bool isfind = false;
            if(pa_set.empty()) isfind = false;
            else{
                for(auto panode:pa_set){
                    if(panode->op_ID == instr->op_id_){
                        panode->defs.insert(instr);
                        ValueToNode[instr] = panode;
                        isfind = true;
                        break;
                    }
                }
            }
            if(isfind == false){
                GNode* now = new GNode();
                GNodes.push_back(now);
                now->op_ID = instr->op_id_;
                now->defs.insert(instr);
                ValueToNode[instr] = now;
                for(int i = 0 ; i < instr->num_ops_ ; i++){
                    GNode* childnode = ValueToNode[instr->get_operand(i)];
                    childnode->fa.insert(now);
                    now->childs.insert(childnode);
                }
            }
        }else{//没有全部加入结点

            GNode* now = new GNode();
            GNodes.push_back(now);
            now->defs.insert(instr);
            now->op_ID = instr->op_id_;
            ValueToNode[instr] = now;
            //操作数可能为constant(不是引用某个instr)
            for(int i = 0 ; i < instr->num_ops_ ; i++){
                Instruction* childI = dynamic_cast<Instruction *>(instr->get_operand(i));
                if(ValueToNode.count(instr->get_operand(i)) == 0){ //未加入结点的操作数
                    GNode* child_node = new GNode();
                    GNodes.push_back(child_node);
                    Instruction* childI = dynamic_cast<Instruction *>(instr->get_operand(i));
                    if(childI){
                        child_node->op_ID = childI->op_id_;
                        child_node->fa.insert(now);
                        child_node->defs.insert(childI);
                        now->childs.insert(child_node);
                    }else{
                        child_node->fa.insert(now);
                        child_node->defs.insert(instr->get_operand(i));
                        now->childs.insert(child_node);
                    }
                    ValueToNode[instr->get_operand(i)] = child_node;
                }else{//加入结点的操作数
                    GNode* child_node = ValueToNode[instr->get_operand(i)];
                    child_node->fa.insert(now);
                    now->childs.insert(child_node);
                }
            }
        }
    }
    std::vector<Instruction*> instr_to_dele;
    for(auto instr : BB->instr_list_){
        if(ValueToNode.count(instr) > 0){
            GNode* now = ValueToNode[instr];
            if(now->isused) continue;
            for(auto v:now->defs){
                if(v != instr){
                    v->replace_all_use_with(instr);
                    Instruction* instr2 = dynamic_cast<Instruction*>(v);
                    instr_to_dele.push_back(instr2);
                }
            }
            now->isused = true;
        }
    }
    for(auto it: instr_to_dele){
        BB->delete_instr(it);
    }
    for(auto node:GNodes){
        delete node;
    }
    GNodes.clear();
}


void DAGopt::execute(){
    for(auto func:m->function_list_){
        for(auto bb:func->basic_blocks_){
            DAG_opt_forBB(bb);
        }
    }
}