#ifndef FIB_H
#define FIB_H	
#define MAXDEGREE 20
typedef struct fib_node{
	int key;
	int marked;
	int degree;
	struct fib_node* parent;
	struct fib_node* fchild;
	struct fib_node* left;
	struct fib_node* right;
}*fib_node;
//set the MAXDEGREE to 20 is sufficient for this project as 2^20 is a great number
typedef struct fib_heap{
	int count;
	fib_node min;
	fib_node nodes[MAXDEGREE];
}*fib_heap;
//search a key value in a fib heap (trees)
fib_node fib_search(fib_heap fh, int key);
//search a key in a tree
fib_node fib_search_node(fib_node root, int key);
//extract the min value of the heap and return it
fib_node fib_del_min(fib_heap fh);
//add root to the node list and merge roots if mecessary
void fib_add_to_nodes(fib_heap fh, fib_node iter);
//pre-operation of node merge, determine which node is the base
fib_node fib_merge_nodes(fib_node fn1, fib_node fn2);
//set a root as another's child
fib_node fib_merge_nodes__(fib_node base, fib_node addition);
//decrease the key of a fib node 'target' to new_key
fib_heap fib_dec_key(fib_heap fh, fib_node target, int new_key);
//remove a decreased-key node to the root list
void fib_remove_to_roots(fib_heap fh, fib_node target);
//add a node in heap into root list
fib_heap fib_to_roots(fib_heap fh, fib_node target);
//merge two fib heaps
//result is the former heap
//note that the latter heap won't be freed, but shouldn't be used any more
fib_heap fib_merge(fib_heap base, fib_heap addition);
//insert a key into a fib heap
void fib_insert(fib_heap fh, int key);
//insert a double circled list into fib heap the root list of 'fh'
//increase the count of 'fh' with count
//fn is supposed to be the min node of the inserted list
void fib_ins_node(fib_heap fh, fib_node fn, int count);
//new a fib heap, set pointers to NULL and count to 0
fib_heap fib_new_heap();
//new a fib node, set its sibling to itself and other pointers to NULL
//'degree' to 0 and 'marked' is defaulted being 0
fib_node fib_new_node(int key);
#endif