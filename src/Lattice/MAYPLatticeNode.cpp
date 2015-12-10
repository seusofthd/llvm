#include "../../include/Lattice/MAYPLatticeNode.h"
typedef set<Value*>::iterator setIter;
typedef map<Value*, set<Value*> >::iterator mapIter;

LatticeNode* MAYPLatticeNode::join(LatticeNode* node){
	MAYPLatticeNode* paNode = dyn_cast<MAYPLatticeNode>(node);

	set<Value*> point_to_all1 = this->point_to_all;
	set<Value*> point_to_all2 = paNode->point_to_all;
	set<Value*> point_to_all_final;
	map<Value*, set<Value*> > point_to_info1 = this->point_to_info;
	map<Value*, set<Value*> > point_to_info2 = paNode->point_to_info;
	map<Value*, set<Value*> > point_to_info_final;

	set<Value*> all_inst;
	for(setIter iter = point_to_all1.begin(); iter != point_to_all1.end(); iter++){
		all_inst.insert(*iter);
	}	
	for(setIter iter = point_to_all2.begin(); iter != point_to_all2.end(); iter++){
		all_inst.insert(*iter);
	}
	for(mapIter iter = point_to_info1.begin(); iter != point_to_info1.end(); iter++){
		all_inst.insert(iter->first);
	}
	for(mapIter iter = point_to_info2.begin(); iter != point_to_info2.end(); iter++){
		all_inst.insert(iter->first);
	}

	for(setIter iter = all_inst.begin(); iter != all_inst.end(); iter++){
		if(point_to_all1.count(*iter) || point_to_all.count(*iter)){
			point_to_all_final.insert(*iter);
		}else if(point_to_info1.count(*iter) && point_to_info2.count(*iter)){
			set<Value*> point_to;
			for(setIter it = point_to_info1[*iter].begin(); it != point_to_info1[*iter].end(); iter++){
				point_to.insert(*it);
			}
			for(setIter it = point_to_info2[*iter].begin(); it != point_to_info2[*iter].end(); it++){
				point_to.insert(*it);
			}
			point_to_info_final[*iter] = point_to;
		}else if(point_to_info1.count(*iter)){
			point_to_info_final[*iter] = point_to_info1[*iter];
		}else if(point_to_info2.count(*iter)){
			point_to_info_final[*iter] = point_to_info2[*iter];
		}	
	}

	MAYPLatticeNode* rNode = new MAYPLatticeNode(false, false, point_to_all_final, point_to_info_final);
	return rNode;
}

bool MAYPLatticeNode::equal(LatticeNode* node){
	if(this->isBottom || node->isBottom){
		return this->isBottom == node->isBottom;
	}
	if(this->isTop || node->isTop){
		return this->isTop == node->isTop;
	}

	MAYPLatticeNode* paNode = dyn_cast<MAYPLatticeNode>(node);
	if(point_to_all.size() != paNode->point_to_all.size())
		return false;
	for(setIter iter = point_to_all.begin(); iter != point_to_all.end(); iter++){
		if(!paNode->point_to_all.count(*iter))
			return false;
	}

	for(mapIter iter = point_to_info.begin(); iter != point_to_info.end(); iter++){
		if(!paNode->point_to_info.count(iter->first))
			return false;
		set<Value*> s1 = iter->second;
		set<Value*> s2 = paNode->point_to_info[iter->first];
		for(setIter it = s1.begin(); it != s1.end(); it++){
			if(!s2.count(*it))
				return false;
		}
	}
	return true;
}

void MAYPLatticeNode::print(){
	errs() << "---MAYPLatticeNode Info---\n";
	errs() << "Bottom:" <<this->isBottom <<"  Top:"<< this->isTop << "\n";
	for(mapIter iter = point_to_info.begin(); iter != point_to_info.end(); iter++){
		errs() << *iter->first << "->";
		for(setIter it = iter->second.begin(); it != iter->second.end(); it++){
			errs()<< **it << ";";
		}
		errs() << "\n";
	}
	errs() << "\n";
}

