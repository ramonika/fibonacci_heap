#include"fibonacci_heap_utils_working.h"

int main()
{
	HEAP * my_heap;

	my_heap = make_fib_heap();

	printf("Enter number of root list elements.\n");

	int rootNum;

	scanf("%d",&rootNum);

	for(int i=0;i<rootNum;i++)
	{
		int key;

		scanf("%d",&key);

		insert(my_heap,key);
	}

	int non_rootNum;

	printf("Enter number of non-root list elements. \n");

	scanf("%d",&non_rootNum);

	for(int i=0;i<non_rootNum;i++)
	{
		int main_node;
		int CorR;
		int key;
		bool mark;
		int temp;

		scanf("%d %d %d %d",&main_node,&CorR,&key,&temp);
		mark = temp;

		NODE * n = find_node(my_heap->min,main_node);

		if( CorR == 0 )
		{
			NODE * d = create_dummy_node(key);
			d->mark = mark;
			n->child = d;
			n->degree+=1;
			d->parent = n;
			(my_heap->n)++;
		}
		if( CorR == 1 )
		{
			NODE * d = create_dummy_node(key);
			d->mark = mark;
			d->parent = n->parent;
			n->parent->degree+=1;

			( n -> left ) -> right = d;
			d->right = n;
			d->left = n->left;
			n->left = d;
			(my_heap->n)++;		
		}

	}


	printf("\n\n*************************************************** \n\n");
	print_heap(my_heap->min);
	printf("\n\n*************************************************** \n\n");

	printf("number of nodes in my_heap is %d \n", my_heap->n);
	printf("\n");
	
	printf ("calling extract_min on H \n\n");
	ext_min(my_heap);

	printf("\nextract min done \n");

	printf("\n\n*************************************************** \n\n");
	print_heap(my_heap->min);
	printf("\n\n*************************************************** \n\n");

	printf("Calling decrease key on node 46 \n");
	decrease_key(my_heap,46,15);

	//printf("Printing mark of node of val 24 : %c\n",n->child->mark);

	printf("\n\n*************************************************** \n\n");
	print_heap(my_heap->min);
	printf("\n\n*************************************************** \n\n");

	printf("minimum value is : %d\n",my_heap->min->key);	

	printf("Calling decrease key on node 35 \n");
	decrease_key(my_heap,35,5);

	printf("\n\n*************************************************** \n\n");
	print_heap(my_heap->min);
	printf("\n\n*************************************************** \n\n");

	printf("minimum value is : %d\n",my_heap->min->key);

    return 0;

}