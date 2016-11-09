/*
 * analysis.c
 *
 *  Created on: Oct 25, 2016
 *      Author: haozhang
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "queueNew.h"
#define N 100

/**
 * method that parse the input graph to adjacency list
 */
void parseGraph(Queue list[N], int argc, char *argv[]) {
	FILE* file = fopen(argv[1], "r");
	int node1 = 0;
	int node2 = 0;

	fscanf(file, "%d,%d", &node1, &node2);
	while (!feof(file)) {
		enqueue(&list[node1], node2);
		enqueue(&list[node2], node1);
		fscanf(file, "%d,%d", &node1, &node2);
	}
	fclose(file);
}

/**
 * methods that print out the final results
 */
//void output(Queue info[N], double max_d[N], double diameter) {
//	printf("Total Node: %d, Diameter: %.0f\n", N, diameter);
//	printf("source\tmax\tdestination\n");
//	for (int i = 0; i < N; i++) {
//		Part *x = dequeue(&info[i]);
//		printf("%d\t%.0f\t", i, max_d[i]);
//		while(x != NULL) {
//			if (x->next != NULL) {
//				printf("%d, ", x->nodeNumber);
//				x = dequeue(&info[i]);
//			} else {
//				printf("%d\n", x->nodeNumber);
//				x = dequeue(&info[i]);
//			}
//		}
//	}
//}

/**
 * methods that output a text file
 */
void generateFile(Queue info[N], double max_d[N], double diameter, int argc, char *argv[]) {
	FILE *f = fopen(argv[3], "w");
	fprintf(f, "Total Node: %d, Diameter: %.0f\n", N, diameter);
	fprintf(f, "source\tmax\tdestination\n");
	for (int i = 0; i < N; i++) {
		Part *x = dequeue(&info[i]);
		fprintf(f, "%d\t%.0f\t", i, max_d[i]);
		while(x != NULL) {
			if (x->next != NULL) {
				fprintf(f, "%d, ", x->nodeNumber);
				x = dequeue(&info[i]);
			} else {
				fprintf(f, "%d\n", x->nodeNumber);
				x = dequeue(&info[i]);
			}
		}
	}
}

/**
 * methods that perform the breadth first search
 */
void BFS(Queue list[N], double distance[N], double visited[N], int source) {
	Queue *process = createQueue();
	enqueue(process, source);
	distance[source] = 0;

	while (process->size != 0) {
		Part *now = dequeue(process);
		Queue *adjacent = &list[now->nodeNumber];
		Part *neighbor = adjacent->front;
		while(neighbor != NULL) {
			if (visited[neighbor->nodeNumber] != 1) {
				if (distance[neighbor->nodeNumber] == INFINITY) {
					distance[neighbor->nodeNumber] = distance[now->nodeNumber] + 1;
				}
				enqueue(process, neighbor->nodeNumber);
			}
			neighbor = neighbor->next;
		}
		visited[now->nodeNumber] = 1;
	}
}

/**
 * Calculate the max distance between each pair of nodes
 */
void findMax(Queue info[N], double max_d[N], double distance[N], int m, double *diameter) {
	double max = 0;
	for (int i = 0; i < N; i++) {
		if (distance[i] > max) {
			max = distance[i];
		}
	}
	max_d[m] = max;
	if (max > *diameter) {
		*diameter = max;
	}
	for (int i = 0; i < N; i++) {
		if (distance[i] == max) {
			enqueue(&info[m], i);
		}
	}
}

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("Not enough input argument.");
		exit(1);
	}

	//initiate a ajacency list
	Queue list[N];
	for (int i = 0; i< N; i++) {
		Queue *new = createQueue();
		list[i] = *new;
	}

	parseGraph(list, argc, argv);

	//Data structure to store max distance and the destination node
	double max_d[N];
	Queue info[N];
	for (int i = 0; i< N; i++) {
		Queue *new = createQueue();
		info[i] = *new;
		max_d[i] = 0;
	}

	//Calculate the shorted distant with BFS
	double diameter = 0;
	for (int m = 0; m < N; m++) {
		double distance[N];
		double visited[N];
		for (int i = 0; i < N; i++) {
			visited[i] = INFINITY;
			distance[i] = INFINITY;
		}
		BFS(list, distance, visited, m);
		findMax(info, max_d, distance, m, &diameter);
	}

	if (argc == 2) {
		printf("Total Node: %d, Diameter: %.0f\n", N, diameter);
		return 20;
	}

	if (strcmp(argv[2], "-o") == 0) {
		generateFile(info, max_d, diameter, argc, argv);
		printf("Total Node: %d, Diameter: %.0f\n", N, diameter);
	}
	return 10;
}
