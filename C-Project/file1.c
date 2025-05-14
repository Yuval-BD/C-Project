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
int* build_prim_tree(EdgeList net[], int n);
EdgeList* build_paths(int* tree, int n);
void find_and_print_path(EdgeList primpaths[], int n, int first, int last);

EdgeList* BuildEmptyList();
EdgeNode* CreateNode(int neighbor, int cost);
void FreeEdgeListArray(EdgeList* lst, int size);
void FreeEdgeList(EdgeList* lst);
void CheckMemoryAllocation(void* ptr);

int main() {
	int n;
	EdgeList* Net;
	int* Prim;
	EdgeList* PrimPath;
	int first, last;
	scanf("%d", &n);
	Net = build_net(n);
	Prim = build_prim_tree(Net, u);
	PrimPath = build_paths(Prim, n);
	scanf("%d%d", &first, &last);
	find_and_print_path(PrimPath, n, first, last);
	return 0;
}

//Help Functions////////////////
EdgeList* BuildEmptyList() {
	EdgeList* newList;

	newList = (EdgeList*)malloc(sizeof(EdgeList));
	CheckMemoryAllocation(newList);

	newList->head = CreateNode(-1, -1);
	newList->tail = CreateNode(-1, -1);
	newList->head->next = newList->tail;

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