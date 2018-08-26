#include <stdio.h>
// Add your system includes here.
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include "ftree.h"
#include "hash.h"
void print_ftree(struct TreeNode *root);
void HELPER(struct TreeNode *root, int depth);

int main(int argc, const char** argv) {
	
	if (argc != 2) {return 1;}

	struct TreeNode *ft;
	ft = generate_ftree(argv[1]);
	//print_ftree(ft);
	return 0;
}


struct TreeNode *generate_ftree(const char *fname) {

    struct stat fileStat;
    int result = lstat(fname, &fileStat);
    
    //check if file exists
    if (result < 0) { 
    	printf("File not found.\n"); 
    	return NULL;
    }

    //create new TreeNode
    struct TreeNode *fileNode = malloc(sizeof(struct TreeNode*));

    //assigning name and permissions of TreeNode
    fileNode->fname = malloc(sizeof(char)*strlen(fname));
    strncpy(fileNode->fname, fname, sizeof(char)*strlen(fname));
    printf("Name: %s\n", fileNode->fname);

    fileNode->permissions = (fileStat.st_mode & 0777);
    printf("Permissions: %d\n", fileNode->permissions );

    //for regular files and links
    if ((S_ISREG(fileStat.st_mode) != 0)) {

            printf("NOT A DIR\n");
            printf("FILENODE NAME: %s\n", fileNode->fname);

            FILE *fp;
            fp = fopen(fileNode->fname, "r");
            fileNode->hash = (char *) malloc((sizeof(char)*8));
            char *hash_ptr = "abdev";
            strncpy(fileNode->hash, hash_ptr, sizeof(char)*8);
            fileNode->contents = NULL;
            //fileNode->next = NULL;
            return fileNode;
    	
    }


    //if File is a directory
    if (S_ISDIR(fileStat.st_mode) != 0) {
    	printf("IS A DIR\n");
    	DIR *dir;
    	dir = opendir(fname);
    	struct dirent *first_content;

    	first_content = readdir(dir);

    	//avoiding parent and current directories
    	while (strncmp(&(first_content->d_name)[0], ".", sizeof(char)) == 0) {
    		first_content = readdir(dir);
    	}

    	//creating LinkedList of all TreeNodes
    	printf("First Content: %s\n", first_content->d_name);
    	fileNode->contents = malloc(sizeof(struct TreeNode*));

    	//creating new path
    	char new_path[265];
    	strcpy(new_path, fname);
    	strcat(new_path, "/");
    	strcat(new_path, first_content->d_name);
    	printf("--------------");
    	fileNode->contents = generate_ftree(new_path);
    	struct dirent *next_file;
 
    	//reading contents of directory
        while (dir) {
    		while ((next_file = readdir(dir)) != NULL) {
    			printf("NEXT FILE: %s\n", next_file->d_name);
    			printf("CONTENT FILE: %s\n", fileNode->contents->fname);
    			
    			char new_path1[265];
    			strcpy(new_path1, fname);
    			strcat(new_path1, "/");
    			strcat(new_path1, next_file->d_name);
    			printf("NEW PATH: %s\n", new_path1);
                (fileNode->contents)->next = (struct TreeNode*)malloc(sizeof(struct TreeNode*));
                printf("MALLOC assigned\n");
    			(fileNode->contents)->next = generate_ftree(new_path1);

                printf("2nd RECURSION\n");
    			
    			
    		}
    	}
    }
    
    return fileNode;
}


/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    HELPER(root, depth);

    }

void HELPER(struct TreeNode *node, int depth) {

        //checking if root is a directory
        if (node->contents != NULL) {
            printf("%*s=====  %s (%d) =====\n", depth * 2, "", 
                node->fname, node->permissions);
        }

        //printing out contents
        //struct TreeNode *next_ptr = node->contents;
        //     printf("%s\n", node->contents->fname);
        //     while (next_ptr != NULL) {
        //         printf("  %s (%d)\n", next_ptr->fname, next_ptr->permissions);
        //         next_ptr = next_ptr->next;
        //     }
        // }
            
}











