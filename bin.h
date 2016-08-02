#ifndef BIN_H
#define BIN_H
#define MAXSIZE 20
typedef struct bin_node{
	struct bin_node *fchild;
	struct bin_node *parent;
	struct bin_node *left;
	struct bin_node *right;
	int degree;
	int key;
}*bin_node;
//set the MAXSIZE to 20, this is sufficient as 2^20 is a quite large number and is enough in this project
typedef struct bin_heap{
	bin_node roots[MAXSIZE];
	int count;
}*bin_heap;
//search a key value from a bin heap (trees)
bin_node bin_search(bin_heap bh, int key);
//search a key in a tree
bin_node bin_search_node(bin_node bn, int key);
//extract the min node from the heap
bin_node bin_del_min(bin_heap bh);
//decrease the key of a node target to 'new_key'
void bin_dec_key(bin_heap bh, bin_node bn, int key);
//swap a bin_node with its parent
bin_node swap_with_parent(bin_node child);
//merge two heaps to the former heap
// the latter heap won'tbe freed, but shouldn't be used any more
bin_heap bin_merge(bin_heap base, bin_heap addition);
//insert a key value into a bin heap
void bin_insert(bin_heap bh, int key);
//add a node to the root list of the heap
//nodes merg eoperation will be done if necessary
bin_node bin_add_to_roots(bin_heap bh, bin_node bn);
//pre-operations of node merge, determine which one is the base
bin_node bin_node_merge(bin_node bn1, bin_node bn2);
//modify a node as another's first child
bin_node bin_node_merge__(bin_node base, bin_node addition);
//new a bin heap, set roots list to empty, count of nodes to 0
bin_heap bin_new_heap();
//new a bin node, set all pointers to NULL, key value to key and degree to 0
bin_node bin_new_node(int key);
#endif