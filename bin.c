#include <stdio.h>
#include <stdlib.h>
#include "bin.h"
//search a key value from a bin heap (trees)
bin_node bin_search(bin_heap bh, int key){
	int index;
	bin_node result;
	//search the key in the trees
	for (index = 0; index<MAXSIZE; index++){
		result = bin_search_node(bh->roots[index], key);
		if (result)break;
	}
	return result;
}
//search a key in  a tree
bin_node bin_search_node(bin_node bn, int key){
	bin_node temp = bn, target = NULL;
	if (!bn)return NULL;
	do{
		//when find the key, end all this and preparea to return it
		if (temp->key == key){
			target = temp;
			break;
		}
		//search the key in the subtrees
		if (key > temp->key){
			target = bin_search_node(temp->fchild, key);
			if (target)break;
		}
		//search its siblings
		temp = temp->right;
	} while (temp);
	return target;
}
//extract the min node from the heap
bin_node bin_del_min(bin_heap bh){
	bin_node iterator ,next, min_node;
	int index, min = 0;
	//if the heap is empty, do nothing
	if (bh->count == 0)return NULL;
	//find the min root, which is also the min node as it's a min heap
	for (index = 0; index < MAXSIZE; index++){
		if (!bh->roots[index])continue;
		if (!bh->roots[min] || bh->roots[index]->key < bh->roots[min]->key){
			min = index;
		}
	}
	min_node = bh->roots[min];
	//add all the subnodes of min root to roots, add_to_roots operation includes portential nodes merge
	for (iterator = bh->roots[min]->fchild, next = iterator ? iterator->right : NULL, bh->roots[min] = NULL; iterator; iterator = next, next = next ? next->right : NULL){
		//remove the sub node from siblings and erase parent
		iterator->right = NULL;
		iterator->left = NULL;
		iterator->parent = NULL;
		bin_add_to_roots(bh, iterator);
	}
	//decrease the count of nodes
	bh->count--;
	return min_node;
}
//decrease the key of a node target to 'new_key'
void bin_dec_key(bin_heap bh, bin_node target, int new_key){
	//deal with unsupposed situations
	if (!target){
		printf("Error: NULL pointer\n");
		return;
	}
	if (new_key > target->key){
		printf("Error: invalid decrease key parameter, the new key is supposed to be smaller\n");
		return;
	}
	//decrease the key
	target->key = new_key;
	//swap the target with its parent if necessary, and do it recursively
	while (target->parent&&target->parent->key > target->key){
		target = swap_with_parent(target);
	}
}
//swap a bin_node with its parent
bin_node swap_with_parent(bin_node child){
	//as swap operation isn't supposed to change the structure of the heap
	//what we need to do is only swap the key value
	int parent_key = child->parent->key;
	child->parent->key = child->key;
	child->key = parent_key;
	return child->parent;
}
//merge two heaps to the former heap
// the latter heap won'tbe freed, but shouldn't be used any more
bin_heap bin_merge(bin_heap base, bin_heap addition){
	//add a carry variable to replace recursion with loop
	bin_node base_tree, addition_tree, carry = NULL;
	int base_index;
	//treat the nodes merge operation like add operation
	for (base_index = 0; base_index < MAXSIZE; base_index++) {
		base_tree = base->roots[base_index]; addition_tree = addition->roots[base_index]; /*current trees */
		//8 situations:
		switch (4 * !!carry + 2 * !!addition_tree + !!base_tree) {
		case 0: /* 000 */
		case 1: /* 001 */  break;
		case 2: /* 010 */  base->roots[base_index] = addition_tree; addition->roots[base_index] = NULL; break;
		case 4: /* 100 */  base->roots[base_index] = carry; carry = NULL; break;
		case 3: /* 011 */  carry = bin_node_merge(base_tree, addition_tree);
			base->roots[base_index] = addition->roots[base_index] = NULL; break;
		case 5: /* 101 */  carry = bin_node_merge(base_tree, carry);
			base->roots[base_index] = NULL; break;
		case 6: /* 110 */  carry = bin_node_merge(addition_tree, carry);
			addition->roots[base_index] = NULL; break;
		case 7: /* 111 */  base->roots[base_index] = carry;
			carry = bin_node_merge(base_tree, addition_tree);
			addition->roots[base_index] = NULL; break;
		} /* end switch */
	} /* end for-loop */
	//maintain the count of nodes
	base->count += addition->count;
	return base;

}
//insert a key value into a bin heap
void bin_insert(bin_heap bh, int key){
	bh->count++;
	//generate a node with the key and insert it to the roots list
	//add_to_list operation merges nodes if necessary
	bin_add_to_roots(bh, bin_new_node(key));
}
//add a node to the root list of the heap
//nodes merg eoperation will be done if necessary
bin_node bin_add_to_roots(bin_heap bh, bin_node bn){
	if (!bn)return NULL;
	int degree = bn->degree;
	//merge nodes when two nodes in the roots list owns the same degree
	if (bh->roots[degree]){
		bin_add_to_roots(bh, bin_node_merge(bh->roots[degree], bn));
		bh->roots[degree] = NULL;
	}
	//if the degree of the new node is unique, just add it in
	else {
		bh->roots[degree] = bn;
	}
	return bh->roots[degree];
}
//pre-operations of node merge, determine which one is the base
bin_node bin_node_merge(bin_node bn1, bin_node bn2){
	if (bn1->key < bn2->key){
		return bin_node_merge__(bn1, bn2);
	}
	else{
		return bin_node_merge__(bn2, bn1);
	}
}
//modify a node as another's first child
bin_node bin_node_merge__(bin_node base, bin_node addition){
	addition->right = base->fchild;
	if (base->fchild){
		base->fchild->left = addition;
	}
	addition->parent = base;
	base->fchild = addition;
	base->degree++;
	return base;
}
//new a bin heap, set roots list to empty, count of nodes to 0
bin_heap bin_new_heap(){
	bin_heap new = malloc(sizeof(struct bin_heap));
	for (int i = 0; i < MAXSIZE; i++){
		new->roots[i] = NULL;
	}
	new->count = 0;
	return new;
}
//new a bin node, set all pointers to NULL, key value to key and degree to 0
bin_node bin_new_node(int key){
	bin_node new = malloc(sizeof(struct bin_node));
	new->degree = 0;
	new->key = key;
	new->fchild = NULL;
	new->parent = NULL;
	new->left = NULL;
	new->right = NULL;
	return new;
}