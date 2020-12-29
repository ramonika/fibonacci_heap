#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

typedef struct node
{

	// pointers
	struct node * left;		
	struct node * right;
	struct node * child;
	struct node * parent;

	// value of the node
	int key;

	// extra information
	int degree;   
	bool mark;   
	int visited;

}NODE;


typedef struct heap
{
	NODE * min; // pointer to the minimum value node in the heap

	int n;  // number of nodes in the heap
}HEAP;


HEAP * make_fib_heap(); 
// this function delares a new fibonacci heap

void insert( HEAP * H , int val ); 
// insert a new node into the fibonacci heap with value = val

NODE * find_min( HEAP * H ); 
// returns the node containing the minimum value in the fibonacci heap 

void print_heap(NODE * H_min);
// prints the fibonacci heap -> the input is the pointer to the minimum node of the heap

NODE * find_node(NODE * H_min , int val);
// return the node with the given val -> the input is the pointer to the minimum node of the heap and the key of the node to be found

void decrease_key(HEAP * H , int curr_val , int dec_val);
// decrease the value of a node and reconfigure the heap

void link_heaps(HEAP *H, NODE *y , NODE *x);
// remove heap with root node as y from the root list and add as a child to the node x

void consolidate (HEAP * H);
// consolidate the heap 

NODE * ext_min(HEAP * m);
// extract the minimum element from the heap


// used for adding nodes at positions other than the root list
NODE * create_dummy_node(int val)
{
	NODE * new_node;
	new_node = malloc(sizeof(NODE));
	NODE * temp = NULL;

	new_node->left = new_node;
	new_node->right = new_node;
	new_node->parent = temp;
	new_node->child = temp;

	new_node->key = val;

	new_node->degree = 0;
	new_node->mark = false;
	new_node->visited = 0; 

	return new_node;
}

// linking node a and b
void add_node(NODE * a , NODE * b)
{
	( a -> left ) -> right = b;
	b->right = a;
	b->left = a->left;
    a->left = b;
}

HEAP * make_fib_heap()
{
	printf("Making a new fibonacci heap !\n");

	HEAP * new_heap;

	new_heap = malloc(sizeof(HEAP));

	new_heap->min = NULL;

	new_heap->n = 0;

	return new_heap;
}

void insert( HEAP * H , int val ) 
{
	NODE * new_node;
	new_node = malloc(sizeof(NODE));

	new_node = create_dummy_node(val);


	if( (H->min) == NULL ) // no node already added to the fibonacci heap
	{
		H->min = new_node;

		printf("Adding the first node ( val = %d ) to an empty fibonacci heap !\n",val);

		(H->n)++;
		return;
	}

	// adding the new node to the root list
	
	add_node(H->min , new_node);

    printf("Adding a new node ( val = %d ) to the fibonacci heap in the root list !\n",val);
    (H->n)++;  // increasing the size of heap by one

    if( (new_node->key) < (H->min)->key )  // if the value of the new node is smaller then make it the minimum
    {
    	H->min = new_node;
    }

    return;
}


NODE * find_min( HEAP * H )  
{
	if( H->min == NULL )
	{
		printf("The fibonacci heap is empty, cannot get the minimum !\n");
		return (NODE *)NULL;
	}

	printf("Returning the minimum node !\n");
	return H->min;
}

void print_heap(NODE * H_min)
{
	NODE * ptr = H_min;

	if(H_min == NULL)
	{
		printf("Heap is empty !");
		return;
	}

	ptr->visited = 1;

	printf("  -->  %d 	{ degree : %d , mark : %d }\n",ptr->key,ptr->degree,ptr->mark);

		if( ptr->child != NULL )
		{
			printf("[child of %d]",ptr->key);
			print_heap(ptr->child);
		}

		if( (ptr->right)->visited != 1 )
		{
			printf("[right of %d]",ptr->key);
			print_heap(ptr->right);
		}

	ptr->visited = 0;

}

NODE * find_node(NODE * H_min , int val)
{
	NODE * ptr = H_min;

	if(H_min == NULL)
	{
		printf("Heap is empty !");
		return NULL;
	}

	ptr->visited = 1;

	if( ptr->key == val )
	{
		ptr->visited = 0;
		return ptr;
	}

	NODE * x = NULL;
	
	if( ptr->child != NULL )
	{
		x = find_node(ptr->child,val);
	}

	if( (ptr->right)->visited != 1 && x == NULL )
	{
		x = find_node(ptr->right,val);
	}

	ptr->visited = 0;
	
	return x;
}

void CUT( NODE* H_min , NODE * node , NODE * parent )
{
	if( node == node->right ) // node is the only child of parent
	{
		parent->child = NULL;
	}

	else{

		parent->child = node->right;

		(node->left)->right = node->right;
		(node->right)->left = node->left;

	}

	parent->degree = parent->degree - 1;  // decrease the degree of the parent by 1  

	node->right = node;  
	node->left = node;

	add_node(H_min , node);	// add node to the root list

	node->parent = NULL;
	node->mark = false;

	printf("Node of value = %d added to the root list\n",node->key);
}

void CASCADING_CUT(NODE * H_min , NODE * parent)
{
	NODE * parent_parent = parent->parent;
	
	if(parent_parent == NULL)
	{

	}	
	
	else
	{
		if(parent->mark == true)
		{
			CUT( H_min , parent , parent_parent );
			CASCADING_CUT( H_min , parent_parent );	
		}	
		else
		{
			parent->mark = true;
		}
	}
}

void decrease_key(HEAP * H , int curr_val , int dec_val)
{
	NODE * H_min = H->min;

	if(H_min == NULL)
	{
		printf("Error!, The heap is empty\n");
		return;
	}

	NODE * found_node = find_node(H_min , curr_val);

	if( found_node == NULL )
	{
		printf("Error! Cannot call decrease_key as node not found\n");
		return;
	}

	if( found_node->key < dec_val )
	{
		printf("Error!, the new value is greater than previous value.\n");
		return;
	}

	NODE * parent = found_node->parent;

	found_node->key = dec_val;	// decrease the value of the node

	if( parent != NULL )
	{
		if( parent->key > found_node->key )
		{
			CUT(H_min , found_node , parent);
			CASCADING_CUT(H_min , parent);
		}
	}

	if( H->min->key > found_node->key )
	{
		H->min = found_node;
	}

	return;
}


NODE * ext_min(HEAP * m)
{
	int nodesCount = m->n;
	printf("heap is having %d nodes \n", nodesCount);

	NODE *extra, *lft,*extra_child;

	if( m->min == NULL)
		printf("Error ! The Heap Is Empty, cannot extarct the minimum.\n");
	else
	{
		NODE *c =NULL;

		extra =  m->min;
		lft = extra;
		extra_child = extra->child;
		printf("extracted minimum is temp (val = %d)\n",extra->key);

		//adding the child nodes to the root list//
		if( extra->child !=NULL)
		{ 
		    c = extra_child; 
		    
		    do 
		    { 
		        lft = c->left; 

		        add_node(m->min , c); 

		        printf ("adding node (val = %d) to root list \n", c->key);
		                
		        c->parent = NULL; 

		        //extracting the min value//
		        if ( m->min->key > c->key ) 
		        	m->min = c; 
		                 
		        c = lft; 

		    } while (lft != extra_child); 
		} 

		//extracting the Min Node from the heap//
    	(extra->right)->left = extra->left; 
    	(extra->left)->right = extra->right; 

    	NODE * right_node = extra->right;

    	m->min = right_node; 

		if ( extra == right_node ) //if min is the only node//  
    	{
    		if( extra_child == NULL )
    			m->min = NULL;
    	}
    	else 
    	{ 
    		
    		m->min = right_node;  //rearrange the tree//
			printf ("node (val = %d) is the m->min now \n", m->min->key);
	    	
	    	consolidate(m); 
	    } 
	
		nodesCount-=1; 
		m->n = nodesCount;
    	printf("heap is having %d nodes \n", nodesCount);
	}

	return extra; 
} 

void consolidate (HEAP * H)
{
	int i;
	int dMax,d;

	dMax = (int)log2(H->n)+1;

	//dMax = degree(H->n);

	NODE * deg_array[dMax], *x,*y,*temp;
	printf("\nmax degree of a node in H is = %d \n", (dMax-1));
 
	for ( i=0 ; i<dMax ; i++)
	{
		deg_array[i]= (NODE *) NULL;
	}

	x = H->min;

	do
	{
		printf ("visiting node (val = %d) \n", x->key);
		
		d = x->degree ;
		printf ("degree of node (val = %d) is (d = %d) \n", x->key, d);
		
		while(deg_array[d]!=NULL && x->key != deg_array[d]->key )
		{
			y = deg_array[d];     
			printf ("found node with same degree(%d) having (val = %d) \n",d, y->key);

			if (x->key > y->key)
			{
				temp = x;  // exchange x and y
				x = y;
				y = temp;
			}

			link_heaps (H,x,y);

			deg_array[d] = (NODE *)NULL;
			d = d+1;
		}
	
		deg_array[d] = x;    //x is node having degree d

		x = x->right;

	}while(x != H->min);

	printf ("visited all nodes in the root list \n");



	H->min = (NODE *)NULL;

	for ( int i=0 ; i<dMax ; i++) 
	{ // checking a[i]
		if(*(deg_array+i) == (NODE *)NULL)
		{

		}
		else
		{ //adding a[i] to the root list
			deg_array[i] -> right = deg_array[i];
			deg_array[i] -> left = deg_array[i];

			if(H->min != (NODE *)NULL)
			{
				add_node(H->min , deg_array[i]);

				if( deg_array[i]->key < H->min->key )
				H->min = deg_array[i]; //updating H->min
			}
			else
			{
				H->min = deg_array[i];
			}
		}
	}
	
	return;
}

void link_heaps(HEAP *H, NODE *x , NODE *y)
{  
	(y->left)->right = y->right;  // remove y from root list of H
  	(y->right)->left = y->left;
  	y->left = y;
  	y->right = y;

  	y->parent = x;   // make y a child of x

  	if (x->child == NULL)  // if x has no child previously
    	x->child = y;

    // if x already has a child list, add y to the child list

    add_node(x->child , y);

  	x->degree++;
  	y->mark = false;
}


