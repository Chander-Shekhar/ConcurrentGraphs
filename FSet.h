#include "ll.h"
#include<atomic>
using namespace std;

enum OPType{
	INS,
	REM
};

inline long set_mark(long i){
  i |= 0x1L;
  return i;
}

template<typename T,typename S>
struct FSetOP{
	OPType type;
	T key;
	S value;
	int resp; // we will add different types of responses 0 ==> error, 1 ==> new key and value added, 2==> new value added 
	FSetOP(OPType type,T key, S value){
		this->type=type;
		this->key=key;
		this->value=value;
	}
	bool getResponse() {
		return resp;
	}
};

template<typename T,typename S>
struct FSetNode
{
	List<T,S> *map;
	bool ok;
	FSetNode(){
		this->ok = true;
	}
	FSetNode(List<T,S> *m_map, bool ok ) {
        this->map = m_map;
        this->ok = ok;
    }
};

template<typename T,typename S>
class FSet{
private:
	atomic<FSetNode<T,S>*> node;
public:
	FSet(List<T,S> *m_map, bool ok){
		FSetNode<T,S> *p = new FSetNode<T,S>(m_map, ok);
		node.store(p,memory_order_seq_cst);
	}

	List<T,S>* freeze(){
		FSetNode<T,S>* o = node.load(memory_order_seq_cst);
		List<T,S> *new_map = new List<T,S>();
		// Edited until here
		while(o->ok){
			*new_map = *o->map;
			FSetNode<T,S> *n= new FSetNode<T,S>(new_map,false);
			if(node.compare_exchange_strong(o,n))
				break;
			o=node.load(memory_order_seq_cst);
		}
		return node.load()->map;
	}

	bool invoke(FSetOP<T,S>* op){
		FSetNode<T,S>* o = node.load(memory_order_seq_cst);
		Node<T,S> *newNode = Node<T,S>(op->key, op->value);
		Node<T,S> *pred, *curr, *succ;
		int resp = -1;
		FSetNode<T,S> *n = new FSetNode<T,S>(o->map, o->ok);
		while(o->ok) {
			// List<T,S> *map = new List<T,S>();
			// *map=*(o->map);
			// int resp;
			// if(op->type==INS){
			// 	if(o->map->find(op->key)==o->map->end()){
			// 		(*map)[op->key]=op->value;
			// 		resp=1;
			// 	}
			// 	else{
			// 		// if(op->value == o->map->at(op->key))
			// 			resp=0;
			// 		// else{
			// 		// 	*map=*(o->map);
			// 		// 	(*map)[op->key]=op->value;
			// 		// 	resp=2;
			// 		// }
			// 	}
			// }
			if(op->type == INS) {
				// resp = map->insert(op->key, op->value);
				o->map->locate(&pred, &curr, key);
				if(curr->key == key){
					resp = 0;
				}
				else{
					newNode->next.store(curr,memory_order_seq_cst);  
					if(pred->next.compare_exchange_strong(curr, newNode, memory_order_seq_cst))
						resp = 1;
						node.compare_exchange_strong(o, n);
				}
			}

			else if(op->type == REM) {
				// resp = map->remove(op->key);
				pred = Head;
				o->map->locate(&pred, &curr, key);
				if(curr->key != key)
					resp = 0;
				else {
					succ = curr->next.load(memory_order_seq_cst);
					if(!is_marked_ref((long) succ))
						if(atomic_compare_exchange_strong_explicit(&curr->next, &succ,(Node<T,S>*)get_marked_ref((long)succ), memory_order_seq_cst, memory_order_seq_cst)) {
							resp = 1;
							node.compare_exchange_strong(o, n);
							if(!atomic_compare_exchange_strong_explicit(&pred->next, &curr, succ, memory_order_seq_cst, memory_order_seq_cst))
								o->map->locate(&pred, &curr, curr->key);
						}
				}
			}
			// else if(op->type==REM) {
			// 	if(o->map->find(op->key)==o->map->end())
			// 		resp=0;
			// 	else{
			// 		o->map[op->key] = set_mark(o->map->at(op->key));
			// 		map->erase(op->key);
			// 		resp=1;
			// 	}
			// }

			// FSetNode<T,S>* n=new FSetNode<T,S>(map,true);
			// if(node.compare_exchange_strong(o,n)){
			// 	op->resp=resp;
			// 	return true;
			// }

			// See how to return right response.
			o = node.load(memory_order_seq_cst);
			if(resp >= 0 && o->ok) {
				op->resp = resp;
				return true;
			}
		}
		return false;
	}

	bool hasMember(T k, Node<T,S> *n){
		FSetNode<T,S>* o = node.load(memory_order_seq_cst);
		return o->map->contains(k, &n);
	}

    FSetNode<T,S> *getHead() {
        return node.load();
    }

};
