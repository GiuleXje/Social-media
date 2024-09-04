#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"

tree_node_t *init_tree_node(int id, char *title, char *user_id)
{
	tree_node_t *new_node = malloc(sizeof(tree_node_t));
	DIE(!new_node, "malloc() failed!\n");
	new_node->id = id;
	new_node->sons = 0;
	if (title) // if it's a repost, TITLE = NULL
		strcpy(new_node->title, title);
	new_node->next = NULL;
	strcpy(new_node->user_id, user_id);
	new_node->likes = 0;
	new_node->liked_by = NULL;
	return new_node;
}

void CREATE(post_t **posts, char *input)
{
	if (!(*posts)) { // creating the post_t arrays
		*posts = malloc(sizeof(post_t));
		DIE(!(*posts), "malloc() failed!\n");
		(*posts)->size = 1;
		(*posts)->curr_post_idx = 1;
	} else {
		(*posts)->size++;
		(*posts)->curr_post_idx++;
		*posts = realloc(*posts, (*posts)->size * sizeof(post_t));
		DIE(!(*posts), "realloc() failed!\n");
	}

	char title[TITLE_MAX], name[TITLE_MAX];
	char *x = strtok(input, " ");
	x = strtok(NULL, " ");
	strcpy(name, x);
	x = strtok(NULL, "\n");
	strcpy(title, x + 1);// remove " "
	title[strlen(title) - 1] = '\0';
	int n = (*posts)->size;
	tree_node_t *root = init_tree_node((*posts)->curr_post_idx, title,
									   name);
	(*posts)[n - 1].root = root;
	//save the post's index for each new post
	(*posts)[n - 1].curr_post_idx = (*posts)->curr_post_idx;
	printf("Created \"%s\" for %s\n", title, name);
}

int get_post_index(post_t **posts, int post_id)
{
	int index = 0;
	for (int i = 0 ; i < (*posts)->size ; i++) {
		if ((*posts)[i].curr_post_idx == post_id)
			index = i;
	}
	return index;
}

void place_repost(tree_node_t *node, int repo_id, int *ok,
				  int curr_idx, char *user_id)
{
	if (node->id == repo_id) { // we found the repost
		*ok = 1;
		if (node->sons) {
			node->next = realloc(node->next,
								 (node->sons + 1) * sizeof(tree_node_t *));
			DIE(!node->next, "malloc() failed!\n");
			tree_node_t *new_node = init_tree_node(curr_idx, NULL, user_id);
			node->next[node->sons] = new_node;
			node->sons++;
		} else {
			node->next = malloc(sizeof(tree_node_t *));
			DIE(!node->next, "malloc() failed!");
			tree_node_t *new_node = init_tree_node(curr_idx, NULL, user_id);
			node->next[node->sons++] = new_node;
		}
		return;
	}
	for (int i = 0 ; i < node->sons ; i++) {
		place_repost(node->next[i], repo_id, ok, curr_idx, user_id);
		if (*ok) // we already found out repost, no need to keep looking
			return;
	}
}

void REPOST(post_t **posts, char *input)
{
	char *x = strtok(input, " ");
	char name[TITLE_MAX];
	x = strtok(NULL, " ");
	strcpy(name, x);
	x = strtok(NULL, "\n ");
	int post_id = atoi(x);
	post_id = get_post_index(posts, post_id);
	x = strtok(NULL, "\n");
	int repo_id = -1;

	if (x) // if we have a repost
		repo_id = atoi(x);
	(*posts)->curr_post_idx++; // increment the post index
	if (repo_id == -1) { // if we don't repost a repost
		if (!((*posts)[post_id].root->sons)) {
			(*posts)[post_id].root->next = malloc(sizeof(tree_node_t *));
			DIE(!((*posts)[post_id].root->next), "malloc() failed!\n");
			(*posts)[post_id].root->sons++;
			tree_node_t *new_node = init_tree_node((*posts)->curr_post_idx,
												   NULL, name);
			(*posts)[post_id].root->next[0] = new_node;
		} else {
			int n = (*posts)[post_id].root->sons;
			(*posts)[post_id].root->sons++;
			tree_node_t *aux = (*posts)[post_id].root;
			(*posts)[post_id].root->next = realloc(aux->next,
												   aux->sons *
												   sizeof(tree_node_t *));
			DIE(!((*posts)[post_id].root->next), "realloc() failed!\n");
			tree_node_t *new_node = init_tree_node((*posts)->curr_post_idx,
												   NULL, name);
			(*posts)[post_id].root->next[n] = new_node;
		}
	} else { // if we do, we have to search for it first
		int ok = 0;
		place_repost((*posts)[post_id].root, repo_id, &ok,
					 (*posts)->curr_post_idx, name);
	}
	printf("Created repost #%d for %s\n", (*posts)->curr_post_idx, name);
}

void print_tree(tree_node_t *node)
{
	printf("Repost #%d by %s\n", node->id, node->user_id);
	for (int i = 0 ; i < node->sons ; i++)
		print_tree(node->next[i]);
}

void get_repo(tree_node_t *node, int *ok, int repo_id)
{
	if (node->id == repo_id) { // found the repost
		print_tree(node);
		*ok = 1;
		return;
	}
	for (int i = 0 ; i < node->sons ; i++) {
		get_repo(node->next[i], ok, repo_id);
		if (*ok) // no need to keep looking
			return;
	}
}

void GET_REPOSTS(post_t **posts, char *input)
{
	char *x = strtok(input, " ");
	x = strtok(NULL, "\n ");
	int post_id = atoi(x);
	post_id = get_post_index(posts, post_id);
	int repo_id = -1;
	x = strtok(NULL, "\n");

	if (x)
		repo_id = atoi(x);
	if (repo_id == -1) { // not a repost
		printf("\"%s\" - Post by %s\n", (*posts)[post_id].root->title,
			   (*posts)[post_id].root->user_id);
		for (int i = 0 ; i < (*posts)[post_id].root->sons ; i++)
			print_tree((*posts)[post_id].root->next[i]);
	} else { // repost, search for it first
		int ok = 0;
		get_repo((*posts)[post_id].root, &ok, repo_id);
	}
}

void like_repo(tree_node_t *node, int *ok, int repo_id, char *nume, char *title)
{	//same logic as for a post, only now we search for the repost
	if (node->id == repo_id) {
		if (node->likes == 0) {
			printf("User %s liked repost \"%s\"\n", nume, title);
			node->likes++;
			node->liked_by = malloc(sizeof(char *));
			DIE(!node->liked_by, "malloc() failed!\n");
			node->liked_by[0] = strdup(nume);
			DIE(!node->liked_by[0], "strdup() failed!");
		} else {
			int found = 0;
			for (int i = 0; i < node->likes; i++) {
				if (strcmp(node->liked_by[i], nume) == 0) {
					found = 1;
					printf("User %s unliked repost \"%s\"\n", nume, title);
					free(node->liked_by[i]);
					for (int j = i; j < node->likes - 1; j++)
						node->liked_by[j] = node->liked_by[j + 1];
					node->likes--;
					if (node->likes == 0) {
						free(node->liked_by);
						node->liked_by = NULL;
					} else {
						node->liked_by = realloc(node->liked_by,
												 node->likes * sizeof(char *));
						DIE(!node->liked_by, "realloc() failed!");
					}
					break;
				}
			}
			if (!found) {
				printf("User %s liked repost \"%s\"\n", nume, title);
				node->likes++;
				node->liked_by = realloc(node->liked_by,
										 node->likes * sizeof(char *));
				DIE(!node->liked_by, "realloc() failed!");
				node->liked_by[node->likes - 1] = strdup(nume);
				DIE(!node->liked_by[node->likes - 1], "strdup() failed!");
			}
		}
		*ok = 1;
	return;
	}
	for (int i = 0; i < node->sons; i++) {
		like_repo(node->next[i], ok, repo_id, nume, title);
		if (*ok == 1) // found the repost, now back home
			return;
	}
}

void LIKE(post_t **posts, char *input)
{
	char *x = strtok(input, " ");
	x = strtok(NULL, " ");
	char nume[TITLE_MAX];
	strcpy(nume, x);
	x = strtok(NULL, "\n ");
	int post_id = atoi(x);
	post_id = get_post_index(posts, post_id);
	x = strtok(NULL, "\n");
	int repo_id = -1;

	if (!x) { // not a repost
		tree_node_t *aux = (*posts)[post_id].root;
		if (!aux->likes) { // no likes
			printf("User %s liked post \"%s\"\n", nume, aux->title);
			aux->likes++;
			aux->liked_by = malloc(sizeof(char *));
			DIE(!aux->liked_by, "malloc() failed!\n");
			aux->liked_by[0] = strdup(nume);
			DIE(!aux->liked_by[0], "malloc() failed!\n");
		} else { // already liked
			int found = 0; // check if this person already liked this post
			for (int i = 0 ; i < aux->likes ; i++) {
				if (strcmp(nume, aux->liked_by[i]) == 0) {
					found = 1; // it did, now unlike it
					printf("User %s unliked post \"%s\"\n", nume,
						   aux->title);
					free(aux->liked_by[i]);
					for (int j = i ; j < aux->likes - 1 ; j++)
						aux->liked_by[j] = aux->liked_by[j + 1];
					aux->likes--;
					if (!aux->likes) {
						free(aux->liked_by);
						aux->liked_by = NULL;
					} else {
						aux->liked_by = realloc(aux->liked_by,
												aux->likes * sizeof(char *));
						DIE(!aux->liked_by, "realloc() failed!\n");
					}
					break;
				}
			}
			if (!found) { // it didn't, like it
				printf("User %s liked post \"%s\"\n", nume, aux->title);
				aux->likes++;
				aux->liked_by = realloc(aux->liked_by,
										aux->likes * sizeof(char *));
				DIE(!aux->liked_by, "realloc() failed!\n");
				aux->liked_by[aux->likes - 1] = strdup(nume);
				DIE(!aux->liked_by[aux->likes - 1], "malloc() failed!\n");
			}
		}
	} else { // it's a repost, find it, then apply the same logic as above
		repo_id = atoi(x);
		int ok = 0;
		like_repo((*posts)[post_id].root, &ok, repo_id,
				  nume, (*posts)[post_id].root->title);
	}
}

void repo_likes(tree_node_t *node, int *ok, int repo_id)
{
	if (node->id == repo_id) {
		printf("Repost #%d has %d likes\n", node->id, node->likes);
		*ok = 1;
		return;
	}
	for (int i = 0 ; i < node->sons ; i++) {
		repo_likes(node->next[i], ok, repo_id);
		if (*ok == 1) // back home
			return;
	}
}

void GET_LIKES(post_t **posts, char *input)
{
	char *x = strtok(input, " ");
	x = strtok(NULL, "\n ");
	int post_id = atoi(x);
	post_id = get_post_index(posts, post_id);
	int repo_id = -1;
	x = strtok(NULL, "\n");

	if (x)
		repo_id = atoi(x);
	if (repo_id == -1) { // not a repost, just print the root's number of likes
		printf("Post \"%s\" has %d likes\n", (*posts)[post_id].root->title,
			   (*posts)[post_id].root->likes);
	} else { // search for the repost, than print it's number of likes
		int ok = 0;
		repo_likes((*posts)[post_id].root, &ok, repo_id);
	}
}

void check_likes(tree_node_t *node, int *most_liked_id, int *max_likes)
{
	if (node->likes > *max_likes) {
		*max_likes = node->likes;
		*most_liked_id = node->id;
	}
	for (int i = 0 ; i < node->sons ; i++)
		check_likes(node->next[i], most_liked_id, max_likes);
}

void RATIO(post_t **posts, char *input)
{
	char *x = strtok(input, " ");
	x = strtok(NULL, "\n");
	int post_id = atoi(x);
	post_id = get_post_index(posts, post_id);

	int most_liked_id = post_id;
	int max_likes = (*posts)[post_id].root->likes;
	for (int i = 0 ; i < (*posts)[post_id].root->sons ; i++) {
		check_likes((*posts)[post_id].root->next[i], &most_liked_id,
					&max_likes); // check the tree for a higher number of likes
	}
	if (max_likes == (*posts)[post_id].root->likes) //OG better
		printf("The original post is the highest rated\n");
	else
		printf("Post %d got ratio'd by repost %d\n", post_id + 1,
			   most_liked_id);
}

void delete_tree(tree_node_t *node)
{
	for (int i = 0 ; i < node->sons; i++)
		delete_tree(node->next[i]);
	if (node->sons)
		free(node->next);
	if (node->likes) {
		for (int i = 0 ; i < node->likes ; i++)
			free(node->liked_by[i]);
		free(node->liked_by);
	}
	free(node);
}

void cut_branch(tree_node_t *node, int *ok, int repo_id)
{
	for (int i = 0 ; i < node->sons ; i++) {
		if (node->next[i]->id == repo_id) { // found the repost
			*ok = 1;
			delete_tree(node->next[i]);
			for (int j = i ; j < node->sons - 1 ; j++)
				node->next[j] = node->next[j + 1];
			node->sons--;
			if (!node->sons) {
				free(node->next);
			} else {
				node->next = realloc(node->next,
									 node->sons * sizeof(tree_node_t *));
				DIE(!node->next, "realloc() failed!");
			}
			break;
		} else if (!(*ok)) { // keep looking
			cut_branch(node->next[i], ok, repo_id);
		}
	}
}

void DELETE(post_t **posts, char *input)
{
	char *x = strtok(input, " ");
	x = strtok(NULL, "\n ");
	int post_id = atoi(x);
	post_id = get_post_index(posts, post_id);
	x = strtok(NULL, "\n");
	int repo_id = -1;

	if (x)
		repo_id = atoi(x);
	if (repo_id == -1) { // not a repost, cut the whole tree
		printf("Deleted \"%s\"\n", (*posts)[post_id].root->title);
		delete_tree((*posts)[post_id].root);
		free(posts[post_id]);
		for (int i = post_id ; i < (*posts)->size - 1 ; i++) {
			(*posts)[i] = (*posts)[i + 1];
			(*posts)[i].curr_post_idx--;
		}
		(*posts)->size--;
		if (!(*posts)->size) {
			free(*posts);
		} else {
			(*posts) = realloc(*posts, (*posts)->size * sizeof(post_t));
			DIE(!(*posts), "realloc() failed\n");
		}
	} else { // repost, find the node, cut it's subtree
		int ok = 0;
		cut_branch((*posts)[post_id].root, &ok, repo_id);
		printf("Deleted repost #%d of post \"%s\"\n",
			   repo_id, (*posts)[post_id].root->title);
	}
}

void go_to_repo(tree_node_t *node, int *ok, int **path, int repo_id, int *size)
{
	if (node->id == repo_id) { // found the repost we're looking for
		*ok = 1; // add it to the path
		if (!(*path)) {
			*size = *size + 1;
			*path = malloc(sizeof(int));
			DIE(!(*path), "malloc() failed\n");
		} else { //never the case, but just to make sure
			*size = *size + 1;
			*path = realloc(*path, *size * sizeof(int));
			DIE(!(*path), "malloc() failed!\n");
		}
		(*path)[*size - 1] = node->id;
		return;
	}
	for (int i = 0 ; i < node->sons ; i++) {
		go_to_repo(node->next[i], ok, path, repo_id, size);
		if (*ok) { // add rest of the path to the root
			if (!(*path)) { // not really needed, good to do though
				*size = *size + 1;
				*path = malloc(sizeof(int));
				DIE(!(*path), "malloc() failed\n");
			} else {
				*size = *size + 1;
				*path = realloc(*path, *size * sizeof(int));
				DIE(!(*path), "malloc() failed!\n");
			}
			(*path)[*size - 1] = node->id;
			return;
		}
	}
}

void COMMON_REPOST(post_t **posts, char *input)
{
	char *x = strtok(input, " ");
	x = strtok(NULL, " ");
	int post_id = atoi(x);
	post_id = get_post_index(posts, post_id);
	x = strtok(NULL, " ");
	int repo1 = atoi(x);
	x = strtok(NULL, "\n");
	int repo2 = atoi(x);

	int *path1, *path2;
	int size1, size2;
	size1 = 0;
	size2 = 0;
	path1 = NULL;
	path2 = NULL;
	int ok = 0;
	// get path no 1
	go_to_repo((*posts)[post_id].root, &ok, &path1, repo1, &size1);
	ok = 0;
	// get path no 2
	go_to_repo((*posts)[post_id].root, &ok, &path2, repo2, &size2);
	int common_id = 0;
	// look for the highest common node
	for (int i = 0 ; i < size1 ; i++) {
		for (int j = 0 ; j < size2 ; j++) {
			if (path1[i] == path2[j] && path1[i] > common_id)
				common_id = path1[i];
		}
	}
	printf("The first common repost of %d and %d is %d\n", repo1,
		   repo2, common_id);
	free(path1);
	free(path2);
}

void handle_input_posts(char *input, post_t **posts)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "create"))
		CREATE(posts, input);
	else if (!strcmp(cmd, "repost"))
		REPOST(posts, input);
	else if (!strcmp(cmd, "common-repost"))
		COMMON_REPOST(posts, input);
	else if (!strcmp(cmd, "like"))
		LIKE(posts, input);
	else if (!strcmp(cmd, "ratio"))
		RATIO(posts, input);
	else if (!strcmp(cmd, "delete"))
		DELETE(posts, input);
	else if (!strcmp(cmd, "get-likes"))
		GET_LIKES(posts, input);
	else if (!strcmp(cmd, "get-reposts"))
		GET_REPOSTS(posts, input);
	else if (!strcmp(cmd, "get-likes"))
		GET_LIKES(posts, input);

	free(commands);
}

