#ifndef POSTS_H
#define POSTS_H

#define TITLE_MAX 285
#define REPOSTS_MAX 100

#include <errno.h>

// used for memory allocation failure
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

typedef struct tree_node_t tree_node_t;
/// @brief
// Struct containg information about a current post or repost
// Id represent's the post's/repost's id, title is the post's/repost's title
// User_id is the person's name which posted/reposted on our social media
// Sons is the number of a current's post/repost reposts(children of tree node)
// Next is the array containing the reposts of our current post/repost
// Likes represents the number of likes for our current post/repost
// Liked_by is the array containing every person that liked our post/repost

struct tree_node_t {
	int id;
	char title[TITLE_MAX];
	char user_id[TITLE_MAX];
	int sons;
	int likes;
	char **liked_by;
	tree_node_t **next;
};

typedef struct post_t post_t;
/// @brief
// Struct containing the info about all our posts
// Root is the initial post, giving us info about our current's post tree
// Size is the number of posts created on our social media
// Curr_post_idx is used to help us keep track when indexing the posts/reposts
struct post_t {
	tree_node_t *root;
	int size;
	int curr_post_idx;
};

/// @brief
// Function used to initialize a tree node
/// @param id
// Contains the id of our post/repost
/// @param title
// Contains the name of the post, or NULL if it's a repost
/// @param user_id
// Contains the name of the person who posted/reposted
/// @return
// The function returns our post's/repost's info
tree_node_t *init_tree_node(int id, char *title, char *user_id);

/// @brief
// Creates a post on our social media, adding it to the existing ones
/// @param posts
// Is the array containing all of the post's tree
/// @param input
// The line we have read, helping us get the necessary info for our post/repost
void CREATE(post_t **posts, char *input);

/// @brief
// Used to find the index in posts array of the #post_id post
/// @param posts
// Is the array containing all of the post's tree
/// @param post_id
// The post's id we are looking for
/// @return
// The index of the post we're looking for
int get_post_index(post_t **posts, int post_id);

/// @brief
// Adding a repost as a new node to the post tree from post_id/repost_id
/// @param posts
// Is the array containing all of the post's tree
/// @param input
// The line we have read, helping us get the necessary info for out repost
// like what post we are going to repost, the name of the persone reposting etc
void REPOST(post_t **posts, char *input);

/// @brief
// Helping function for REPOST()
// Searches for the node in which we should add our new repost
// and adds the info about our new repost to the next array
/// @param node
// The current node of the post's tree, starting from the root
/// @param repo_id
// The repost's id, used to compare it with the current's node id
// If they match, we should insert the new repost
/// @param ok
// Used to check if we reached the node where we should insert our repost
/// @param curr_idx
// Repost's index, needed int init_tree_node
/// @param user_id
// The person's reposting name
void place_repost(tree_node_t *node, int repo_id, int *ok,
				  int curr_idx, char *user_id);

/// @brief
// Used to print a post's/repost's tree, used in GET_REPOSTS()
/// @param node
// Current node of the tree
void print_tree(tree_node_t *node);

/// @brief
// Used to reach the repost's node, and the print it's subtree
// using print_tree(); Used in GET_REPOSTS()
/// @param node
// Current node
/// @param ok
// Used to check if we reached our repost
/// @param repo_id
// The repost we are looking for
void get_repo(tree_node_t *node, int *ok, int repo_id);

/// @brief
// Print's a post's/repost's tree, using the functions from above
/// @param posts
// "----"
/// @param input
// "----"
void GET_REPOSTS(post_t **posts, char *input);

/// @brief
// Used to reach a repost and like it if not liked already
// Called by LIKE()
/// @param node
// Current node
/// @param ok
// Check if we reached the node we're looking for
/// @param repo_id
// The repost's we're looking for id
/// @param nume
// The person that liked the repost
/// @param title
// The original post's title
void like_repo(tree_node_t *node, int *ok, int repo_id,
			   char *nume, char *title);

/// @brief
// Used to like a post/repost, if we have to like a post we do it in this
// function, if we have to like a repost we call for like_repo()
// We could've used like_repo() for liking a post as well, but we would have
// to change the messages which are being printed
/// @param posts
// "----"
/// @param input
// "----"
void LIKE(post_t **posts, char *input);

/// @brief
// Used to get numeber of likes for a repost, called by GET_LIKES()
/// @param node
// Current node of the post's tree
/// @param ok
// Check if we reached the repost we're looking for
/// @param repo_id
// The post we're looking for
void repo_likes(tree_node_t *node, int *ok, int repo_id);

/// @brief
// Used to get the number of likes for a post/repost
// If it's repost we call repo_likes()
/// @param posts
// "----"
/// @param input
// "----"
void GET_LIKES(post_t **posts, char *input);

/// @brief
// Get's the ratio for a post
/// @param posts
// "----"
/// @param input
// "----"
void RATIO(post_t **posts, char *input);

/// @brief
// Used in ratio, to go through the tree and calculate the max number
// of likes
/// @param node
// Current node of the tree
/// @param most_liked_id
// The most liked post/repost
/// @param max_likes
// The number of likes of the most liked post/repost
void check_likes(tree_node_t *node, int *most_liked_id, int *max_likes);

/// @brief
// Used to free a post's/repost's tree
/// @param node
// Current node
void delete_tree(tree_node_t *node);

/// @brief
// Used to search for the repost and free it's subtree
/// @param node
// Current node
/// @param ok
// If we found the repost, we call delete_tree()
/// @param repo_id
// The repost's id
void cut_branch(tree_node_t *node, int *ok, int repo_id);

/// @brief
// Process data about a post/repost and delete it
/// @param posts
// "----"
/// @param input
// "----"
void DELETE(post_t **posts, char *input);

/// @brief
// Used to reach a repost, and save it's path to the root(original post)
// Called by COMMON_REPOST()
/// @param node
// Current node
/// @param ok
// Used to see if we found the repost we're looking for
/// @param path
// Save the path from the root to the repost's node
/// @param repo_id
// The repost we're looking for
/// @param size
// The length of the path
void go_to_repo(tree_node_t *node, int *ok, int **path, int repo_id, int *size);

/// @brief
// Calling for go_to_repo() after getting the necessary info
/// @param posts
// "----"
/// @param input
// "----"
void COMMON_REPOST(post_t **posts, char *input);

void handle_input_posts(char *input, post_t **posts);

#endif // POSTS_H

