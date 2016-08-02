#include <stdio.h>
#include <stdlib.h>
#include "fib.h"
//search a key value in a fib heap (trees)
fib_node fib_search(fib_heap fh, int key){
	if (fh->min == NULL)return NULL;
	//search the key in the trees
	return fib_search_node(fh->min, key);
}
//search a key in a tree
fib_node fib_search_node(fib_node root, int key){
	fib_node target = NULL, temp = root;
	if (!root)return NULL;
	do{
		//skip all this and return the node if the key is found
		if (temp->key == key){
			target = temp;
			break;
		}
		//search the key in subnodes
		if (key > temp->key){
			target = fib_search_node(temp->fchild, key);
			if (target)break;
		}
		//search the key in siblings
		temp = temp->right;
	} while (temp != root);
	return target;
}
//extract the min value of the heap and return it
fib_node fib_del_min(fib_heap fh){
	int i;
	//store min node with variable 'min'
	fib_node iterator, next_min=fh->min->right, next_iter, min = fh->min;
	//init the nodes list
	for (i = 0; i < MAXDEGREE; i++){
		fh->nodes[i] = NULL;
	}
	//add min's children to root list 
	fib_ins_node(fh, fh->min->fchild, 0);
	//insert the root list to nodes and do portential node merge operation
	for (iterator = min->right, next_iter = iterator->right; iterator != fh->min; iterator = next_iter, next_iter = next_iter->right){
		fib_add_to_nodes(fh, iterator);
	}
	//erase min in the heap
	fh->min = NULL;
	//extract nodes from node list and reinsert into root list 
	for (i = 0; i < MAXDEGREE; i++){
		fib_ins_node(fh, fh->nodes[i], 0);
	}
	//maintain count of nodes
	fh->count--;
	/*not necessary step, left to confirm the consistence of tested version and annotated version*/
	if (!fh->count){
		fh->min = NULL;
	}
	/*unnecessary steps end*/
	return min;
}
//add root to the node list and merge roots if mecessary
void fib_add_to_nodes(fib_heap fh, fib_node iter){
	int degree = iter->degree;
	//remove the root from its siblings
	//as it's cycle list, its siblings are set to itself instead of NULL 
	iter->parent = NULL;
	iter->left->right = iter->right;
	iter->right->left = iter->left;
	iter->left = iter;
	iter->right = iter;
	//merge nodes when two roots own the same degree
	if (fh->nodes[degree]){
		fib_add_to_nodes(fh, fib_merge_nodes(fh->nodes[degree], iter));
		fh->nodes[degree] = NULL;
	}
	//just add the root in nodes if its degree is unique
	else{
		fh->nodes[degree] = iter;
	}
}
//pre-operation of root merge, determine which root is the base
fib_node fib_merge_nodes(fib_node fn1, fib_node fn2){
	if (fn1->key > fn2->key){
		return fib_merge_nodes__(fn2, fn1);
	}
	else{
		return fib_merge_nodes__(fn1, fn2);
	}
}
//set a root as another's child
fib_node fib_merge_nodes__(fib_node base, fib_node addition){
	if (!base->fchild){
		base->fchild = addition;
	}
	else{
		addition->left = base->fchild->left;
		addition->right = base->fchild;
		addition->left->right = addition;
		base->fchild->left = addition;
	}
	addition->parent = base;
	base->degree++;
	return base;
}
//decrease the key of a fib node 'target' to new_key
fib_heap fib_dec_key(fib_heap fh, fib_node target, int new_key){
	//deal with unsupposed situation
	if (!target){
		printf("Error: NULL pointer\n");
	}
	if (new_key > target->key){
		printf("Error: invalid decrease key parameter, the new key is supposed to be smaller\n");
	}
	//decrease the keys
	target->key = new_key;
	//remove the target and its ancestors to root until root or a node hasn't lost any descenant
	if (target->parent && target->key < target->parent->key){
		fib_remove_to_roots(fh, target);
	}
	return fh;
}
//remove a decreased-key node to the root list
void fib_remove_to_roots(fib_heap fh, fib_node target){
	//clear the mark on target as it will be a root
	target->marked = 0;
	//do nothing more if target is root
	if (!target->parent)return;
	/* 
	the following is not necessary as they are done in fib_to_roots:
	target->right->left = target->left;
	target->left->right = target->right;
	*/
	//if target is the first child
	//set parent's fchild to NULL if target is the only child
	//else set it to the sibling of target
	if (target->parent->fchild == target){
		if (target->left == target){
			target->parent->fchild = NULL;
		}
		else{
			target->parent->fchild = target->right;
		}
	}
	/*unnecessary step ends*/
	//decrease the degree of target's parent
	target->parent->degree--;
	//if target's parent has lost a child already
	//recursively move its ancestors to root until a node hasn't lost any child
	if (target->parent->marked){
		fib_remove_to_roots(fh, target->parent);
	}
	//if target's parent hasn't lost any child, mark the parent
	else{
		target->parent->marked = 1;
	}
	//move target to root list
	fib_to_roots(fh, target);
}
//add a node in heap into root list
fib_heap fib_to_roots(fib_heap fh, fib_node target){
	//same thing stated in fib_remove to roots
	//erase the relation between target and its parent and siblings
	//then add it to root list
	target->right->left = target->left;
	target->left->right = target->right;
	if (target->parent->fchild == target){
		if (target->right == target){
			target->parent->fchild = NULL;
		}
		else{
			target->parent->fchild = target->right;
		}
	}
	target->left = target;
	target->right = target;
	fib_ins_node(fh, target, 0);
	return fh;
}
//merge two fib heaps
//result is the former heap
//note that the latter heap will be freed
fib_heap fib_merge(fib_heap base, fib_heap addition){
	//insert the root list of 'addition' to base's
	fib_ins_node(base, addition->min, addition->count);
	free(addition);
	return base;
}
//insert a key into a fib heap
void fib_insert(fib_heap fh, int key){
	//creat a node with the key and insert it in the heap's root list
	fib_node new = fib_new_node(key);
	fib_ins_node(fh, new ,1);
}
//insert a double circled list into fib heap the root list of 'fh'
//increase the count of 'fh' with count
//fn is supposed to be the min node of the inserted list
void fib_ins_node(fib_heap fh, fib_node fn, int count){
	//do nothing if a NULL pointer is inserted
	if (!fn){
		return;
	}
	//set 'fh->min' to the new node if 'fh->min' doesn't exist
	if (!fh->min){
		fh->min = fn;
	}
	//merge two double circled list and flush the min value if necessary
	else{
		fib_node fn_right = fn->right, min_left = fh->min->left;
		fn->right = fh->min;
		fh->min->left = fn;
		fn_right->left = min_left;
		min_left->right = fn_right;
		if (fh->min->key > fn->key){
			fh->min = fn;
		}
	}
	//maitain the count of nodes
	fh->count += count;
}
//new a fib heap, set pointers to NULL and count to 0
fib_heap fib_new_heap(){
	fib_heap new = malloc(sizeof(struct fib_heap));
	new->count = 0;
	new->min = NULL;
	return new;
}
//new a fib node, set its sibling to itself and other pointers to NULL
//'degree' to 0 and 'marked' is defaulted being 0
fib_node fib_new_node(int key){
	fib_node new = malloc(sizeof(struct fib_node));
	new->degree = 0;
	new->marked = 0;
	new->key = key;
	new->parent = NULL;
	new->fchild = NULL;
	new->left = new;
	new->right = new;
	return new;
}