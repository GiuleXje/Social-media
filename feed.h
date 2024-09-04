#ifndef FEED_H
#define FEED_H

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
#include "posts.h"

#pragma once

void bron_kerbosch(unsigned char **matrix, int *R, int R_size, int *P,
				   int P_size, int *X, int X_size, int *max_clique,
				   int *max_size);

void common_groups(unsigned char **matrix, char *name);

/// @brief
// This function prints the most recent n posts of a user and his
// friends on our social media. All we have to do is go through
// posts array from the end to the beginning and see if the current post
// we're looking at was made by our user or any of his friends
/// @param matrix
// The matrix containing all the frienships
/// @param posts
// The posts array containing each post's tree
/// @param input
// The line we read, used to get all the additional info required for this task
void FEED(unsigned char **matrix, post_t *posts, char *input);

/// @brief
// This function helps us go through a subtree and check if the current repost
//	was made by a friend of a certain user
/// @param node
// Node of a tree
/// @param id
// The user's id
/// @param matrix
// "----"
void f_repo(tree_node_t *node, int id, unsigned char **matrix);

/// @brief
// This function get a user and a certain post and looks for any reposts
// of the initial post that have been made by friends of the user
/// @param matrix
// The matrix containing all the frienships
/// @param posts
// The array containg each post's tree
/// @param input
// Used to get the username and the post_id
void FRIENDS_REPO(unsigned char **matrix, post_t *posts, char *input);

/// @brief
// This function prints all the reposts that have been made by the user
// in the name parameter
/// @param node
// The current tree node
/// @param name
// The username
/// @param title
// Name of the original post
void user_reposts(tree_node_t *node, char *name, char *title);

/// @brief
// This function prints all the posts and reposts(with the help of
// user_reposts()) made by a certain user
/// @param posts
// The posts's array containg each post
/// @param input
// Used to get the name of the user's profile
void VIEW_PROFILE(post_t *posts, char *input);

//TASK3 handler
void handle_input_feed(char *input, unsigned char **matrix, post_t *posts);

#endif // FEED_H
