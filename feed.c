#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"

void bron_kerbosch(unsigned char **matrix, int *R, int R_size, int *P,
				   int P_size, int *X, int X_size, int *max_clique,
				   int *max_size)
{
	if (P_size == 0 && X_size == 0) {
		if (R_size > *max_size) {
			*max_size = R_size;
			memcpy(max_clique, R, R_size * sizeof(int));
		}
		return;
	}

	for (int i = 0 ; i < P_size ; i++) {
		int v = P[i];
		int new_R[518], new_P[518], new_X[518];
		int new_R_size = R_size, new_P_size = 0, new_X_size = 0;
		for (int i = 0 ; i < R_size ; i++)
			new_R[i] = R[i];
		new_R[new_R_size++] = v;

		for (int j = 0 ; j < P_size ; j++) {
			if (P[j] > -1) {
				if (matrix[v][P[j]])
					new_P[new_P_size++] = P[j];
			}
		}

		for (int j = 0 ; j < X_size ; j++) {
			if (matrix[v][X[j]])
				new_X[new_X_size++] = X[j];
		}

		bron_kerbosch(matrix, new_R, new_R_size, new_P, new_P_size, new_X,
					  new_X_size, max_clique, max_size);

		P[i] = -1;
		X[X_size++] = v;
	}
}

void common_groups(unsigned char **matrix, char *name)
{
	int user_id = get_user_id(name);
	if (user_id == -1) {
		printf("User not found\n");
		return;
	}

	int R[518], P[518], X[518];
	int R_size = 0, P_size = 0, X_size = 0;

	// Add the user_id to R initially
	R[R_size++] = user_id;

	// Initialize P with all neighbors of the user_id
	for (int i = 0 ; i < 518 ; i++) {
		if (matrix[user_id][i])
			P[P_size++] = i;
	}

	int max_clique[518], max_size = 0;

	bron_kerbosch(matrix, R, R_size, P, P_size, X, X_size,
				  max_clique, &max_size);

	for (int i = 0 ; i < max_size - 1 ; i++) {
		for (int j = i + 1 ; j < max_size ; j++) {
			if (max_clique[i] > max_clique[j]) {
				int aux = max_clique[i];
				max_clique[i] = max_clique[j];
				max_clique[j] = aux;
			}
		}
	}

	for (int i = 0; i < max_size; i++)
		printf("%s\n", get_user_name(max_clique[i]));
}

void FEED(unsigned char **matrix, post_t *posts, char *input)
{
	char *x = strtok(input, " ");
	char name[TITLE_MAX];
	x = strtok(NULL, " ");
	strcpy(name, x);
	x = strtok(NULL, "\n ");
	int f_size = atoi(x);
	int id1 = get_user_id(name);
	for (int i = posts->size - 1 ; i >= 0 ; i--) {
		int id2 = get_user_id(posts[i].root->user_id);
		if (!f_size)
			break;
		if (matrix[id1][id2] || id1 == id2) {
			printf("%s: \"%s\"\n", posts[i].root->user_id,
				   posts[i].root->title);
			f_size--;
		}
	}
}

void f_repo(tree_node_t *node, int id, unsigned char **matrix)
{
	if (matrix[get_user_id(node->user_id)][id])
		printf("%s\n", node->user_id);
	for (int i = 0 ; i < node->sons ; i++)
		f_repo(node->next[i], id, matrix);
}

void FRIENDS_REPO(unsigned char **matrix, post_t *posts, char *input)
{
	char *x = strtok(input, " ");
	x = strtok(NULL, " ");
	char name[TITLE_MAX];
	strcpy(name, x);
	x = strtok(NULL, "\n ");
	int id = get_user_id(name);
	int post_id = atoi(x);
	post_id = get_post_index(&posts, post_id);
	for (int i = 0 ; i < posts[post_id].root->sons ; i++)
		f_repo(posts[post_id].root->next[i], id, matrix);
}

void user_reposts(tree_node_t *node, char *name, char *title)
{
	if (!strcmp(node->user_id, name))
		printf("Reposted: \"%s\"\n", title);
	for (int i = 0 ; i < node->sons ; i++)
		user_reposts(node->next[i], name, title);
}

void VIEW_PROFILE(post_t *posts, char *input)
{
	char *x = strtok(input, " ");
	x = strtok(NULL, "\n ");
	char name[TITLE_MAX];
	strcpy(name, x);
	for (int i = 0 ; i < posts->size ; i++) {
		if (!strcmp(name, posts[i].root->user_id))
			printf("Posted: \"%s\"\n", posts[i].root->title);
	}
	for (int i = 0 ; i < posts->size ; i++) {
		for (int j = 0 ; j < posts[i].root->sons ; j++)
			user_reposts(posts[i].root->next[j], name,
						 posts[i].root->title);
	}
}

void handle_input_feed(char *input, unsigned char **matrix, post_t *posts)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		FEED(matrix, posts, input);
	} else if (!strcmp(cmd, "view-profile")) {
		VIEW_PROFILE(posts, input);
	} else if (!strcmp(cmd, "friends-repost")) {
		FRIENDS_REPO(matrix, posts, input);
	} else if (!strcmp(cmd, "common-group")) {
		char *name = strtok(NULL, "\n ");
		printf("The closest friend group of %s is:\n", name);
		common_groups(matrix, name);
	}

	free(commands);
}
