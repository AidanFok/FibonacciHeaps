#include <stdio.h>
#include <time.h>
#include "fib.h"
#include "bin.h"
#define size 1000
#define times 100
//stores nodes got by search operation, be used in decrease key operation
void* node_pointers[times][size];
int main(){
    //
    time_t start, end, cost, cost_;
    int h1[size], h2[size];
    int i, repeat, index, value, median;
    fib_heap fh1[times], fh2[times];
    bin_heap bh1[times], bh2[times];
	//creat two lists of fib heap, 2*times heaps in total
    for (repeat = 0; repeat < times; repeat++){
        fh1[repeat] = fib_new_heap();
        fh2[repeat] = fib_new_heap();
    }
	//make sure all key value are unique
    for (i = 0; i < size; i++){
        h1[i] = 2 * i;
        h2[i] = 2 * i + 1;
    }
	//shuffle the two key list
    for (index = 0; index< size; index++){
        value = rand() % size;
        median = h1[index];
        h1[index] = h1[value];
        h1[value] = median;
    }
    for (index = 0; index< size; index++){
        value = rand() % size;
        median = h2[index];
        h2[index] = h2[value];
        h2[value] = median;
    }

	//insert the two shuffled key list in and creat heaps with the same structure
    //insert time
    start = clock();
    for (i = 0; i < size; i++){
        for (repeat = 0; repeat < times; repeat++){
            fib_insert(fh1[repeat], h1[i]);
            fib_insert(fh2[repeat], h2[i]);
        }
    }
    end = clock();
    cost = end - start;
    printf("fib insert %lfs\n", ((double)cost) / CLK_TCK);
	//merge the heaps with key in list 'h1' and list 'h2'
    //merge time
    start = clock();
    for (repeat = 0; repeat < times; repeat++){
        fh1[repeat] = fib_merge(fh1[repeat], fh2[repeat]);
    }
    end = clock();
    cost = end - start;
    printf("fib merge %lfs\n", ((double)cost) / CLK_TCK);
	
    //delete min nodes, not freed here to avoid disturbtion
    start = clock();
    
    for (repeat = 0; repeat < times; repeat++){
        for (int i = 0; i < size; i++){
            fib_del_min(fh1[repeat]);
        }
    }
    end = clock();
    cost = end - start;
    printf("fib del min %lfs \n", ((double)cost) / CLK_TCK);
    start = clock();
	//find the nodes with key value from size to 2*size-1, they are not extracted in size delete-min operation
	//store them in node pointers
    for (repeat = 0; repeat < times; repeat++){
        for (i = 0; i < size; i++){
            node_pointers[repeat][i] = fib_search(fh1[repeat], size + i);
        }
    }
    cost_ = clock();
	//decrease the keys of nodes in node_pointers by size (to avoid different nodes in the same heap owns the same key)
	//the decrease operation is done from larger key to smaller key to avoid best case
    for (repeat = 0; repeat < times; repeat++){
        for (i = size - 1; i >= 0; i--){
            fib_dec_key(fh1[repeat], (fib_node)node_pointers[repeat][i], ((fib_node)node_pointers[repeat][i])->key - size);
        }
    }
    end = clock();
    cost = end - cost_;
    cost_ = end -start;
    printf("fib dec key %lfs with search %lfs\n", ((double)cost) / CLK_TCK, ((double)cost_) / CLK_TCK);
	//followings are same operations on bin heap

	//new heap
    for (repeat = 0; repeat < times; repeat++){
        bh1[repeat] = bin_new_heap();
        bh2[repeat] = bin_new_heap();
    }
	//shuffle
    for (i = 0; i < size; i++){
        h1[i] = 2 * i;
        h2[i] = 2 * i + 1;
    }
    for (index = 0; index< size; index++){
        value = rand() % size;
        median = h1[index];
        h1[index] = h1[value];
        h1[value] = median;
    }
    for (index = 0; index< size; index++){
        value = rand() % size;
        median = h2[index];
        h2[index] = h2[value];
        h2[value] = median;
    }

    //insert time
    start = clock();
    for (i = 0; i < size; i++){
        for (repeat = 0; repeat < times; repeat++){
            bin_insert(bh1[repeat], h1[i]);
            bin_insert(bh2[repeat], h2[i]);
        }
    }
    end = clock();
    cost = end - start;
    printf("bin insert %lfs\n", ((double)cost) / CLK_TCK);

    //merge time
    start = clock();
    for (repeat = 0; repeat < times; repeat++){
        bh1[repeat] = bin_merge(bh1[repeat], bh2[repeat]);
    }
    end = clock();
    cost = end - start;
    printf("bin merge %lfs\n", ((double)cost) / CLK_TCK);

    //delete min or decrease key
    start = clock();
    for (repeat = 0; repeat < times; repeat++){
        for (int i = 0; i <size; i++){
            bin_del_min(bh1[repeat]);
        }
    }
    end = clock();
    cost = end - start;
    printf("bin del min %lfs \n", ((double)cost) / CLK_TCK);
    start = clock();
    for (repeat = 0; repeat < times; repeat++){
        for (i = 0; i < size; i++){
            node_pointers[repeat][i] = bin_search(bh1[repeat], size + i);
        }
    }
    cost_ = clock();
    for (repeat = 0; repeat < times; repeat++){
        for (i = size - 1; i >= 0; i--){
            bin_dec_key(bh1[repeat], (bin_node)node_pointers[repeat][i], ((bin_node)node_pointers[repeat][i])->key - size);
        }
    }
    end = clock();
    cost = end - cost_;
    cost_ = end - start;
    printf("bin dec key %lfs with search %lfs\n", ((double)cost) / CLK_TCK, ((double)cost_) / CLK_TCK);

    system("pause");
}