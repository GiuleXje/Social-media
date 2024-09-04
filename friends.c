#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"

void add_edge(unsigned char **matrix, int src, int dest) {
		matrix[src][dest] = 1;
		matrix[dest][src] = 1;
}

void remove_edge(unsigned char **matrix, int src, int dest) {
		matrix[src][dest] = 0;
		matrix[dest][src] = 0;
}

int find_distance(unsigned char **matrix, int src, int dest) {
	int *visited = calloc(518, sizeof(int));
	int *queue = malloc(518 * sizeof(int));
	int front = 0, rear = 0;
	queue[rear++] = src;
	visited[src] = 1;
	int *distances = malloc(518 * sizeof(int));
	for (int i = 0; i < 518; i++)
		distances[i] = UINT16_MAX;
	distances[src] = 0;
	while (front != rear) {
		int current_node = queue[front++];
		for (int i = 0; i < 518; i++) {
			if (matrix[current_node][i] && !visited[i]) {
				queue[rear++] = i;
				visited[i] = 1;
				distances[i] = distances[current_node] + 1;
			}
		}
	}
	int distance = distances[dest];
	free(visited);
	free(queue);
	free(distances);
	return distance;
}

void suggestions(unsigned char **matrix, int src, char *name) {
	int *visited = calloc(518, sizeof(int));
	int nr = 0;
	visited[src] = 1;
	for (int i = 0; i < 518; i++)
		if (matrix[src][i])
			for (int j = 0; j < 518; j++)
				if (matrix[i][j] && j != src) {
					nr++;
					break;
				}
	if (nr)
		printf("Suggestions for %s:\n", name);
	else
		printf("There are no suggestions for %s\n", name);
	for (int i = 0; i < 518; i++)
		if (matrix[src][i])
			for (int j = 0; j < 518; j++)
				if (matrix[i][j] && !visited[j] && !matrix[src][j])
					visited[j] = 1;
	for (int i = 0; i < 518; i++)
		if (i != src && visited[i]) {
			char *name2 = get_user_name(i);
			printf("%s\n", name2);
		}
	free(visited);
}

void common(unsigned char **matrix, char *name1, char *name2)
{
	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);
	int nr = 0;
	for (int i = 0; i < 518; i++)
		if (matrix[id1][i] && matrix[id2][i]) {
			nr++;
			break;
		}
	if (nr)
		printf("The common friends between %s and %s are:\n", name1, name2);
	else
		printf("No common friends for %s and %s\n", name1, name2);
	for (int i = 0; i < 518; i++)
		if (matrix[id1][i] && matrix[id2][i]) {
			char *name3 = get_user_name(i);
			printf("%s\n", name3);
		}
}

int number_friends(unsigned char **matrix, char *name)
{
	int nr = 0;
	int id = get_user_id(name);
	for (int i = 0; i < 518; i++)
		if (matrix[id][i])
			nr++;
	return nr;
}

void popular(unsigned char **matrix, char *name)
{
	int id = get_user_id(name);
	int max = number_friends(matrix, name);
	char *name2;
	char *max_name = name;
	for (int i = 0; i < 518; i++) {
		if (matrix[id][i]) {
			name2 = get_user_name(i);
			int nr = number_friends(matrix, name2);
			if (max < nr) {
				max = nr;
				max_name = name2;
			}
		}
	}
	if (max_name == name)
		printf("%s is the most popular\n", name);
	else
		printf("%s is the most popular friend of %s\n", max_name, name);
}

void handle_input_friends(char *input, unsigned char **matrix)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;

	if (!strcmp(cmd, "add")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		int id1 = get_user_id(name1);
		int id2 = get_user_id(name2);
		add_edge(matrix, id1, id2);
		printf("Added connection %s - %s\n", name1, name2);
	} else if (!strcmp(cmd, "remove")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		int id1 = get_user_id(name1);
		int id2 = get_user_id(name2);
		remove_edge(matrix, id1, id2);
		printf("Removed connection %s - %s\n", name1, name2);
	} else if (!strcmp(cmd, "suggestions")) {
		char *name1 = strtok(NULL, "\n ");
		int id1 = get_user_id(name1);
		suggestions(matrix, id1, name1);
	} else if (!strcmp(cmd, "distance")) {
		char *name1 = strtok(NULL, " ");
		char *name2 = strtok(NULL, "\n");
		int id1 = get_user_id(name1);
		int id2 = get_user_id(name2);
		int dist = find_distance(matrix, id1, id2);
		if (dist != UINT16_MAX)
			printf("The distance between %s - %s is %u\n", name1, name2, dist);
		else
			printf("There is no way to get from %s to %s\n", name1, name2);
	} else if (!strcmp(cmd, "common")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		common(matrix, name1, name2);
	} else if (!strcmp(cmd, "friends")) {
		char *name = strtok(NULL, "\n ");
		int friends = number_friends(matrix, name);
		printf("%s has %d friends\n", name, friends);
	} else if (!strcmp(cmd, "popular")) {
		char *name = strtok(NULL, "\n ");
		popular(matrix, name);
	}
	free(commands);
}
