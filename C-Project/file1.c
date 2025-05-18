#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

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

//EdgeList* build_net(int n);
//int* build_prim_tree(EdgeList net[], int n);
EdgeList* build_paths(int* tree, int n);
//void find_and_print_path(EdgeList primpaths[], int n, int first, int last);

void InsertNodeSorted(EdgeList* lst, int size, int pc1, int pc2, int cost);
EdgeList BuildEmptyList();
EdgeNode* CreateNode(int neighbor, int cost);
void PrintEdgeListArray(EdgeList* lst, int size);
void PrintEdgeList(EdgeNode* head);
void FreeEdgeListArray(EdgeList* lst, int size);
void FreeEdgeList(EdgeList* lst);
void CheckMemoryAllocation(void* ptr);


int main() {
	EdgeList* PrimPath;
	int n = 4;
	int values[] = {-1, 2, 0, 0};
	int* Prim = values;
	
	PrimPath = build_paths(Prim, n);

	PrintEdgeListArray(PrimPath, n);

	return 0;
}

EdgeList* build_paths(int* tree, int n) {
	EdgeList* new_net;
	
	if (n == 0) //If tree is empty return null
		return NULL;

	new_net = (EdgeList*)malloc(n * sizeof(EdgeList)); //Create a new PC net 
	CheckMemoryAllocation(new_net);

	for(int i = 0; i < n; i++) //Initialise pc's in the new net
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
	EdgeNode* current, *newNode;
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

//Help Functions////////////////
void PrintEdgeListArray(EdgeList* lst, int size){
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

EdgeList BuildEmptyList() {
	EdgeList newList;

	newList.head = CreateNode(-1, -1);
	newList.tail = CreateNode(-1, -1);
	newList.head->next = newList.tail;

	return newList;
}

EdgeNode* CreateNode(int neighbor, int cost) {
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

void FreeEdgeListArray(EdgeList* lst, int size) {
	if (lst != NULL) {
		for (int i = 0; i < size; i++)
			FreeEdgeList(&lst[i]);
		free(lst);
	}
}

void FreeEdgeList(EdgeList* lst) {
	EdgeNode* tempNode = lst->head;

	while (tempNode != NULL) {
		lst->head = lst->head->next;
		free(tempNode);  // Free node
		tempNode = lst->head;
	}

}

void CheckMemoryAllocation(void* ptr) {
	if (ptr == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
}