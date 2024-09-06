# Social Media


 # This project's goal is to set up a small social media. I focussed on 3 main things:
 ## 1)	Establishing connections between users

  This section of our project will manage friendships between users, give new friendship suggestions
and so on.

## 2)	Posts and reposts of users

  This section will focus on posts or reposts made by users, liking a user's post and so on.

## 3)	Social media

   This section will focus on multiple features such as: feed, user's profile, user's that
reposted a specific post and so on.

   Before we jump into each task, let's talk about each file of the project and what it contains.
The file "social_media.c" is the main file of this project and it contains mostly the main menu, where we chose
which task we would like to perform. The files "friends.c" and "friends.h" will handle the functions required
for TASK1. Files "posts.c" and "posts.h" will handle the functions required for TASK2. Lastly, files "feed.c"
and "feed.h" will handel the functions required for TASK3.


# TASK1

## add_friend < user1, user2 >
	
   In this function we will connect this 2 users. For this we will be using a bidirectional graph, where the
nodes will be represented by users. When connect we will display the following message:
![image](https://github.com/user-attachments/assets/03dc39c0-9cd9-4561-acdb-63b5a45073e2)

I would suggest, just for better transparency of the data, to use as input user with different names.


## remove <user1, user2>

   This function removes the connection between 2 users.

 ![image](https://github.com/user-attachments/assets/81f9f734-29be-4877-99ee-71ac282db880)


## distance <user1, user2>

   This function calculates the distance between 2 users. Basically, it finds out the shortest distance between
 node x and node y of a graph.
 
 ![image](https://github.com/user-attachments/assets/c0ba6475-05e0-45ec-9bff-2cba08229d7e)


## suggestions < user >

   This function will find the users with whom you are not connected, and display them as connection suggestions.

![image](https://github.com/user-attachments/assets/56adcb8a-02f2-4263-a8c5-6ec30e6c5c34)


## common <user1, user2>

   This function will display the common friends of the users given as parameters.

 ![image](https://github.com/user-attachments/assets/5de535a6-5519-44c9-af31-94b267d6722b)


 ## friends < username >
 
   This function will diplay the number of friends that a user has.

  ![image](https://github.com/user-attachments/assets/2350d815-000a-4d8c-b4ea-02b7683b6247)

  ## popular < username >
  
   This function will display a friend of the user given as parameter that has the most connections.
   If the original user has the most connections, he will be diplayed.
   	For this example let's asume Andrei, Ana and Mihai are all friends.
    ![image](https://github.com/user-attachments/assets/260ce1e0-89de-4242-805e-0705c8f7614f)



# TASK2





	
