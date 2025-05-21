#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

EdgeList* build_net(int n);
//int* build_prim_tree(EdgeList net[], int n);
EdgeList* build_paths(int* tree, int n);
//void find_and_print_path(EdgeList primpaths[], int n, int first, int last);

void insert_to_list(EdgeNode* previous, EdgeNode* new_node);
void add_to_sorted_list(EdgeList* lst, int computer, int price);
EdgeList BuildEmptyList();
EdgeNode* CreateNode(int neighbor, int cost);
void FreeEdgeListArray(EdgeList* lst, int size);
void FreeEdgeList(EdgeList* lst);
void CheckMemoryAllocation(void* ptr);
void printEdgeLists(EdgeList* lists, int n);
//////////////
void InsertNodeSorted(EdgeList* lst, int size, int pc1, int pc2, int cost);
void PrintEdgeListArray(EdgeList* lst, int size);
void PrintEdgeList(EdgeNode* head);

int main() {
	int n;
	EdgeList* Net;
	int* Prim;
	EdgeList* PrimPath;
	int first, last;
	scanf("%d", &n);
	Net = build_net(n);
	printEdgeLists(Net, n);
	/*Prim = build_prim_tree(Net, n);
	PrimPath = build_paths(Prim, n);*/
	/*scanf("%d%d", &first, &last);
	find_and_print_path(PrimPath, n, first, last);*/
	return 0;
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

	EdgeNode* new_node = CreateNode(computer, price);
	insert_to_list(prev, new_node);
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
		Net[j] = BuildEmptyList();
	}

	//fill them with the communications links
	for (int i = 0; i < lines_number; i++)
	{
		int computer1, computer2, price;
		printf("Please enter the %d communication line and cost in this order [a,b,price]:\n", i + 1);
		scanf("%d %d %d", &computer1, &computer2, &price);

		add_to_sorted_list(&Net[computer1], computer2, price);
		add_to_sorted_list(&Net[computer2],computer1,price );

	}

	return Net;
}

//Help Functions////////////////


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

//A function that inserts new EdgeNode into a EdgeList
void insert_to_list(EdgeNode* previous, EdgeNode* new_node)
{
	new_node->next = previous->next;
	previous->next = new_node;
}

//Function that constructs an empty EdgeList
EdgeList BuildEmptyList() 
{
	EdgeList newList;


	newList.head = CreateNode(-1, -1);
	newList.tail = CreateNode(-1, -1);
	newList.head->next = newList.tail;

	return newList;
}

//Function that creates a new EdgeNode
EdgeNode* CreateNode(int neighbor, int cost) 
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

////Yuval was here
EdgeList* build_paths(int* tree, int n) {
	EdgeList* new_net;

	if (n == 0) //If tree is empty return null
		return NULL;

	new_net = (EdgeList*)malloc(n * sizeof(EdgeList)); //Create a new PC net 
	CheckMemoryAllocation(new_net);

	for (int i = 0; i < n; i++) //Initialise pc's in the new net
		new_net[i] = BuildEmptyList();

	for (int i = 0; i < n; i++) { //Add PC's connections to the new net
		if (tree[i] != -1) {
			InsertNodeSorted(new_net, n, i, tree[i], 0); //Add PC b to a's connections
			InsertNodeSorted(new_net, n, tree[i], i, 0); //Add PC a to b's connections

		}
	}

	//free(tree);
	return new_net;
}

void InsertNodeSorted(EdgeList* lst, int size, int pc1, int pc2, int cost) {
	EdgeNode* current, * newNode;
	int next_neighbor;
	bool is_inserted = false;

	current = lst[pc1].head; //Set current to pc1's dummy head
	newNode = CreateNode(pc2, cost); //Create new connection from pc1 to pc2


	while (current != lst[pc1].tail && !is_inserted) { //Run through the nodes untill reached dummy tail or new node was inserted
		next_neighbor = current->next->e.neighbor; //Get next connection's pc number
		if (next_neighbor > newNode->e.neighbor || next_neighbor == -1) { //If next pc num is bigger or is tail, add new node
			newNode->next = current->next;
			current->next = newNode;
			is_inserted = true;
		}
		current = current->next; //Advence current node
	}

	return;
}
void PrintEdgeListArray(EdgeList* lst, int size) {
	for (int i = 0; i < size; i++) {
		printf("%d -> ", i);
		PrintEdgeList(lst[i].head);
		printf("\n");
	}
}

void PrintEdgeList(EdgeNode* head) {
	head = head->next;
	while (head->next != NULL) {
		printf("%d -> ", head->e.neighbor);
		head = head->next;
	}
}


