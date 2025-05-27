#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>


//Edge
typedef struct {
	int neighbor;
	int cost;
} Edge;

typedef struct edge_node {
	Edge e;
	struct edge_node* next;
} EdgeNode;

typedef struct {
	EdgeNode* head;
	EdgeNode* tail;
} EdgeList;

//Candidate
typedef struct {
	int computer;
	int min;
} Candidate;

typedef struct candidate_node {
	Candidate c;
	struct candidate_node* next;
	struct candidate_node* prev;
} CandidateNode;

typedef struct {
	CandidateNode* head;
	CandidateNode* tail;
} CandidateList;


EdgeList* build_net(int n);
int* build_prim_tree(EdgeList net[], int n);
EdgeList* build_paths(int* tree, int n);
//void find_and_print_path(EdgeList primpaths[], int n, int first, int last);

//Edge
EdgeList build_empty_edge_list();
EdgeNode* create_edge_node(int neighbor, int cost);
void insert_to_list(EdgeNode* previous, EdgeNode* new_node);
void add_to_sorted_list(EdgeList* lst, int computer, int price);
//Candidate
CandidateList build_empty_candidate_list();
CandidateNode* create_candidate_node(int comp, int min, CandidateNode* prev, CandidateNode* next);
void insert_to_dlist(CandidateNode* previous, CandidateNode* new_node);
int DeleteMin(CandidateList priority, CandidateNode** loc);
void DecreaseKey(CandidateNode** loc, int comp, int new_min);

//Print
void print_edge_list(EdgeList* lists, int n);
void print_priority(CandidateList lst, int n);
//Free
void free_edge_list_array(EdgeList* lst, int size);
void free_edge_list(EdgeList* lst);
void check_memory_allocation(void* ptr);

int main() {
	int n;
	EdgeList* Net;
	int values[] = { -1, 2, 0, 0, -1 };
	int* Prim = values;
	EdgeList* PrimPath;
	int first, last;
	scanf("%d", &n);
	Net = build_net(n);
	//print_edge_list(Net, n);

	//printf("\n");
	PrimPath = build_paths(Prim, n);
	//print_edge_list(PrimPath, n);

	Prim = build_prim_tree(Net, n);
	if (Prim == NULL)
	{
		return 0;
	}
	/*scanf("%d%d", &first, &last);
	find_and_print_path(PrimPath, n, first, last);*/
	return 0;
}


//A function that builds the computer network
EdgeList* build_net(int n)
{
	int lines_number;
	EdgeList* Net;

	printf("Please enter the number of communication lines:\n");
	scanf("%d", &lines_number);

	//Memory allocation and testing
	Net = (EdgeList*)malloc(sizeof(EdgeList) * n);
	check_memory_allocation(Net);

	// bulid empty Net
	for (int j = 0; j < n;j++)
	{
		Net[j] = build_empty_edge_list();
	}

	//fill them with the communications links
	for (int i = 0; i < lines_number; i++)
	{
		int computer1, computer2, price;
		printf("Please enter the %d communication line and cost in this order [a,b,price]:\n", i + 1);
		scanf("%d %d %d", &computer1, &computer2, &price);

		add_to_sorted_list(&Net[computer1], computer2, price);
		add_to_sorted_list(&Net[computer2], computer1, price);
	}

	return Net;
}

EdgeList* build_paths(int* tree, int n) {
	EdgeList* new_net;

	if (n == 0) //If tree is empty return null
		return NULL;

	new_net = (EdgeList*)malloc(n * sizeof(EdgeList)); //Create a new PC net 
	check_memory_allocation(new_net);

	for (int i = 0; i < n; i++) //Initialise pc's in the new net
		new_net[i] = build_empty_edge_list();

	for (int i = 0; i < n; i++) { //Add PC's connections to the new net
		if (tree[i] != -1) {
			add_to_sorted_list(&new_net[i], tree[i], 0);
			add_to_sorted_list(&new_net[tree[i]], i, 0);
		}
	}

	//free(tree);
	return new_net;
}

int* build_prim_tree(EdgeList Net[], int n) {
	int* min, *prim;
	CandidateList priority;
	CandidateNode** location;
	CandidateNode* curr;
	int v = 0, u;
	bool* inT; //Important! change to bytes and change size

	min = (int*)malloc(sizeof(int) * n);
	check_memory_allocation(min);

	prim = (int*)malloc(sizeof(int) * n);
	check_memory_allocation(prim);

	for (int i = 0; i < n; i++) {
		min[i] = INT_MAX;
		prim[i] = -1;
	}

	priority = build_empty_candidate_list();
	location = (CandidateNode**)malloc(n * sizeof(CandidateNode*));
	check_memory_allocation(location);

	inT = (bool*)malloc(sizeof(bool) * n); //Important! change to bytes and change size
	check_memory_allocation(inT);

	for (int i = 0; i < n; i++) 
	{
		inT[i] = 0;
	}
	 
	for (int j = 0;j < n;j++)
	{
		curr = create_candidate_node(j, min[j], priority.tail->prev, priority.tail);
		insert_to_dlist(priority.tail->prev, curr);
		location[j] = curr;
	}

	
	min[v] = 0;
	prim[0] = -1;

	while (priority.head->next != priority.tail) 
	{
		u = DeleteMin(priority, location);

		if (min[u] == INT_MAX)
		{
			printf("top");
			printf("Cannot build prim");
			return NULL;
		}
		else
		{
			inT[u] = true;
			EdgeList u_list = Net[u];
			EdgeNode* node = u_list.head->next;
			int curr_node;
			printf("1");
			while (node != u_list.tail)
			{
				curr_node = node->e.neighbor;
				printf("2");
				if (!inT[curr_node] && node->e.cost < min[curr_node])
				{
					printf("3");
					min[curr_node] = node->e.cost;
					prim[curr_node] = u;
					printf("4");
					DecreaseKey(location, curr_node, node->e.cost);
					printf("5");
				}
				node = node->next;
			}
			
		}
	}

	/*  0 : -1
		1 : 4
		2 : 3
		3 : 0
		4 : 2
		*/


}

void printer(int* arr,int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d : %d \n", i, arr[i]);
	}
}

/////////////////////////Help Functions////////////////////////////////////////////////////////////////////////////////
CandidateList build_empty_candidate_list() {
	CandidateList newList;

	newList.head = create_candidate_node(-1, INT_MAX, NULL, NULL);
	newList.tail = create_candidate_node(-1, INT_MAX, newList.head, NULL);
	newList.head->next = newList.tail;

	return newList;
}

void delete_node(CandidateNode* del_node)
{
	del_node->next->prev = del_node->prev;
	del_node->prev->next = del_node->next;

	free(del_node);
}

int DeleteMin(CandidateList priority, CandidateNode** loc)
{
	CandidateNode* curr = priority.head->next->next;
	CandidateNode* min_can = priority.head->next;
	int num_comp;

	while (curr != priority.tail)
	{
		if (curr->c.min < min_can->c.min)
		{
			min_can = curr;
		}
		curr = curr->next;
	}

	num_comp = min_can->c.computer;
	loc[num_comp] = NULL;
	delete_node(min_can);

	return num_comp;
}

void DecreaseKey(CandidateNode** loc, int comp, int new_min) 
{
	loc[comp]->c.min = new_min;
}

void print_priority(CandidateList lst, int n)
{
	CandidateNode* current = lst.head;
	while (current != NULL) 
	{
		printf(" -> (comp: %d, min: %d)", current->c.computer, current->c.min);
		current = current->next;
	}
	printf("\n");
	
}

CandidateNode* create_candidate_node(int comp, int min, CandidateNode* prev, CandidateNode* next) {
	CandidateNode* newNode;
	Candidate newCand;

	newNode = (CandidateNode*)malloc(sizeof(CandidateNode));
	check_memory_allocation(newNode);

	newCand.computer = comp;
	newCand.min = min;

	newNode->c = newCand;
	newNode->prev = prev;
	newNode->next = next;

	return newNode;
};

//A function that inserts new CandidateNode into a Candidatelist
void insert_to_dlist(CandidateNode* previous, CandidateNode* new_node)
{
	previous->next->prev = new_node;
	previous->next = new_node;
	
}

//A function that inserts new EdgeNode into a EdgeList
void insert_to_list(EdgeNode* previous, EdgeNode* new_node)
{
	new_node->next = previous->next;
	previous->next = new_node;
}

//A function that adds to the EdgeList in a sorted manner
void add_to_sorted_list(EdgeList* lst, int computer, int price) {
	EdgeNode* prev = lst->head;

	/*We would like to promote the previous one as long as it is smaller than it, the head is
	not coincidentally the tail, and as long as we are not in an empty list.*/
	while (prev->next != NULL && prev->next->e.neighbor != -1 && prev->next->e.neighbor < computer)
	{
		prev = prev->next;
	}

	// Avoid duplicates
	if (prev->next != NULL && prev->next->e.neighbor == computer)
	{
		return;
	}

	EdgeNode* new_node = create_edge_node(computer, price);
	insert_to_list(prev, new_node);
}

//Function that constructs an empty EdgeList
EdgeList build_empty_edge_list()
{
	EdgeList newList;


	newList.head = create_edge_node(-1, -1);
	newList.tail = create_edge_node(-1, -1);
	newList.head->next = newList.tail;

	return newList;
}

//Function that creates a new EdgeNode
EdgeNode* create_edge_node(int neighbor, int cost)
{
	EdgeNode* newNode;
	Edge newEdge;

	newNode = (EdgeNode*)malloc(sizeof(EdgeNode));
	check_memory_allocation(newNode);

	newEdge.neighbor = neighbor;
	newEdge.cost = cost;

	newNode->e = newEdge;
	newNode->next = NULL;

	return newNode;
}

void print_edge_list(EdgeList* lists, int n)
{
	for (int i = 0; i < n; i++) {
		printf("Node %d:", i);
		EdgeNode* current = lists[i].head;
		while (current != NULL) {
			printf(" -> (neighbor: %d, cost: %d)", current->e.neighbor, current->e.cost);
			current = current->next;
		}
		printf("\n");
	}
}

//A function that releases the EdgeListArray
void free_edge_list_array(EdgeList* lst, int size)
{
	if (lst != NULL) {
		for (int i = 0; i < size; i++)
			free_edge_list(&lst[i]);
		free(lst);
	}
}

//A function that releases the EdgeList
void free_edge_list(EdgeList* lst) {
	EdgeNode* tempNode = lst->head;

	while (tempNode != NULL) {
		lst->head = lst->head->next;
		free(tempNode);  // Free node
		tempNode = lst->head;
	}

}

//A function that checks memory allocation
void check_memory_allocation(void* ptr)
{
	if (ptr == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
}
