#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"

/**
 * Initializez every task based on which task we are running
*/
void init_tasks(void)
{
	#ifdef TASK_1

	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

unsigned char **create_matrix(void) {
	unsigned char **matrix = malloc(518 * sizeof(unsigned char *));
	DIE(!matrix, "malloc() failed!\n");

	for (int i = 0; i < 518; i++) {
		matrix[i] = malloc(518 * sizeof(unsigned char));
		if (!matrix[i]) {
			for (int j = 0; j < i; j++)
				free(matrix[j]);
			free(matrix);
			return NULL;
		}
	}

	// New social media, no friendships just yet
	for (int i = 0; i < 518; i++)
		for (int j = 0; j < 518; j++)
			matrix[i][j] = 0;

	return matrix;
}

// freeing the memory helpers

void free_matrix(unsigned char **matrix) {
	for (int i = 0; i < 518; i++)
		free(matrix[i]);
	free(matrix);
}

void cut_that_tree(tree_node_t *node)
{
	for (int i = 0 ; i < node->sons ; i++) {
		if (node->next[i])
			cut_that_tree(node->next[i]);
	}
	if (node->sons)
		free(node->next);
	if (node->likes) {
		for (int i = 0 ; i < node->likes ; i++)
			free(node->liked_by[i]);
		free(node->liked_by);
	}
	free(node);
}

void cut_forest(post_t **posts)
{
	for (int i = 0 ; i < (*posts)->size ; i++)
		cut_that_tree((*posts)[i].root); // cut each tree
	free(*posts); // put the axe away
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();

	init_tasks();

	unsigned char **matrix = create_matrix();// friendship matrix

	char *input = (char *)malloc(MAX_COMMAND_LEN);

	post_t *posts = NULL; // the posts of our social media

	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, matrix);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, &posts);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, matrix, posts);
		#endif
	}
	free_users();
	free(input);

	free_matrix(matrix); // free the friendship matrix
	if (posts) // free the posts
		cut_forest(&posts); // get the axe
	return 0;
}
