#include "LiveVopt.h"
#include<algorithm>


void LiveVopt::getBlockLiveV(Function* foo){
    for(auto bb:foo->basic_blocks_){
        for(auto instr:bb->instr_list_){
            bb->def_b.insert(instr);
            for(auto op:instr->operands_){
                bb->use_b.insert(op);
            }
        }
    }
    bool change = true;
    while(change){
        change = false;
        for(auto bb:foo->basic_blocks_){
            for(auto suc:bb->succ_bbs_){
                bb->live_out.insert(suc->live_in.begin(),suc->live_in.end());
            }
            int cnt = bb->live_in.size();
            std::set<Value* > pre_in = bb->live_in;
            for(auto it = bb->live_out.begin() ; it != bb->live_out.end() ;){
                if(bb->def_b.count(*it) > 0){
                    it = bb->live_out.erase(it);
                }else{
                    ++it;
                }
            }
            bb->live_in = bb->live_out;
            bb->live_in.insert(bb->use_b.begin(),bb->use_b.end());
            int cnt2 = bb->live_in.size();
            if(cnt2 != cnt){
                change = true;
            }else{
                bool is_equal = std::equal(bb->live_in.begin(),bb->live_in.end(),pre_in.begin(),pre_in.end());
                if(is_equal == false){
                    change = true;
                }
            }
        }
    }
}



void LiveVopt::execute(){
    for(auto foo:m->function_list_){
        
    }
}