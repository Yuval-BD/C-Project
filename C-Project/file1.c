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
EdgeList BuildEmptyEdgeList();
EdgeNode* CreateEdgeNode(int neighbor, int cost);
void insert_to_list(EdgeNode* previous, EdgeNode* new_node);
void add_to_sorted_list(EdgeList* lst, int computer, int price);
//Candidate
CandidateList Build_Empty_CandidateList();
CandidateNode* CreateCandidateNode(int comp, int min, CandidateNode* prev, CandidateNode* next);
//Print
void printEdgeLists(EdgeList* lists, int n);
//Free
void FreeEdgeListArray(EdgeList* lst, int size);
void FreeEdgeList(EdgeList* lst);
void CheckMemoryAllocation(void* ptr);

int main() {
	int n;
	EdgeList* Net;
	int values[] = { -1, 2, 0, 0, -1 };
	int* Prim = values;
	EdgeList* PrimPath;
	int first, last;
	scanf("%d", &n);
	Net = build_net(n);
	printEdgeLists(Net, n);

	printf("\n");
	PrimPath = build_paths(Prim, n);
	printEdgeLists(PrimPath, n);

	//Prim = build_prim_tree(Net, n);
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
	CheckMemoryAllocation(Net);

	// bulid empty Net
	for (int j = 0; j < n;j++)
	{
		Net[j] = BuildEmptyEdgeList();
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
	CheckMemoryAllocation(new_net);

	for (int i = 0; i < n; i++) //Initialise pc's in the new net
		new_net[i] = BuildEmptyEdgeList();

	for (int i = 0; i < n; i++) { //Add PC's connections to the new net
		if (tree[i] != -1) {
			add_to_sorted_list(&new_net[i], tree[i], 0);
			add_to_sorted_list(&new_net[tree[i]], i, 0);
		}
	}

	//free(tree);
	return new_net;
}

int* build_prim_tree(EdgeList net[], int n) {
	int* min, *prim;
	CandidateList priority;
	CandidateNode** location;
	bool* inT; //Important! change to bytes and change size

	min = (int*)malloc(sizeof(int) * n);
	CheckMemoryAllocation(min);

	prim = (int*)malloc(sizeof(int) * n);
	CheckMemoryAllocation(prim);

	for (int i = 0; i < n; i++) {
		min[i] = INT_MAX;
		prim[i] = -1;
	}

	priority = Build_Empty_CandidateList();
	location = (CandidateNode**)malloc(n * sizeof(CandidateNode*));
	CheckMemoryAllocation(location);
	
	inT = (bool*)malloc(sizeof(bool) * n); //Important! change to bytes and change size
	CheckMemoryAllocation(inT);

	for (int i = 0; i < n; i++) {
		inT[i] = 0;
	}

}

/////////////////////////Help Functions////////////////////////////////////////////////////////////////////////////////
CandidateList Build_Empty_CandidateList() {
	CandidateList newList;

	newList.head = CreateCandidateNode(-1, INT_MAX, NULL, NULL);
	newList.tail = CreateCandidateNode(-1, INT_MAX, newList.head, NULL);
	newList.head->next = newList.tail;

	return newList;
}

CandidateNode* CreateCandidateNode(int comp, int min, CandidateNode* prev, CandidateNode* next) {
	CandidateNode* newNode;
	Candidate newCand;

	newNode = (EdgeNode*)malloc(sizeof(EdgeNode));
	CheckMemoryAllocation(newNode);

	newCand.computer = comp;
	newCand.min = min;

	newNode->c = newCand;
	newNode->prev = prev;
	newNode->next = next;

	return newNode;
};

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

	EdgeNode* new_node = CreateEdgeNode(computer, price);
	insert_to_list(prev, new_node);
}

//Function that constructs an empty EdgeList
EdgeList BuildEmptyEdgeList()
{
	EdgeList newList;


	newList.head = CreateEdgeNode(-1, -1);
	newList.tail = CreateEdgeNode(-1, -1);
	newList.head->next = newList.tail;

	return newList;
}

//Function that creates a new EdgeNode
EdgeNode* CreateEdgeNode(int neighbor, int cost)
{
	EdgeNode* newNode;
	Edge newEdge;

	newNode = (EdgeNode*)malloc(sizeof(EdgeNode));
	CheckMemoryAllocation(newNode);

	newEdge.neighbor = neighbor;
	newEdge.cost = cost;

	newNode->e = newEdge;
	newNode->next = NULL;

	return newNode;
}

void printEdgeLists(EdgeList* lists, int n)
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
void FreeEdgeListArray(EdgeList* lst, int size)
{
	if (lst != NULL) {
		for (int i = 0; i < size; i++)
			FreeEdgeList(&lst[i]);
		free(lst);
	}
}

//A function that releases the EdgeList
void FreeEdgeList(EdgeList* lst) {
	EdgeNode* tempNode = lst->head;

	while (tempNode != NULL) {
		lst->head = lst->head->next;
		free(tempNode);  // Free node
		tempNode = lst->head;
	}

}

//A function that checks memory allocation
void CheckMemoryAllocation(void* ptr)
{
	if (ptr == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
}
