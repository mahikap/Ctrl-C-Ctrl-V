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

void print_ftree_HELPER(struct TreeNode *node, int depth);
struct TreeNode *generate_ftree_HELPER(const char *fname, const char *path);
/*
 * Returns the FTree rooted at the path fname.
 */
struct TreeNode *generate_ftree(const char *fname) {

    return generate_ftree_HELPER(fname, fname);

}

struct TreeNode *generate_ftree_HELPER(const char *fname, const char *path) {

    struct stat fileStat;
    int result = lstat(path, &fileStat);
    
    //check if file exists
    if (result < 0) { 
        perror("lstat");
        exit(1);
    }

    //create new TreeNode
    struct TreeNode *fileNode = malloc(sizeof(struct TreeNode));

    //setting name and permissions of TreeNode
    fileNode->fname = malloc(sizeof(char)*strlen(fname)+1);
    strncpy(fileNode->fname, fname, sizeof(char)*strlen(fname)+1);
    fileNode->permissions = (fileStat.st_mode & 0777);

    fileNode->contents = NULL;
    fileNode->next = NULL;
    fileNode->hash = NULL;
    
    //for regular files and links
    if ((S_ISREG(fileStat.st_mode) != 0)) {
        FILE *fp;
        fp = fopen(path, "r");
        if (fp == NULL) {
            perror("fopen");
            return fileNode;
        }

        //setting hash
        fileNode->hash = malloc((sizeof(char)*8));
        strncpy(fileNode->hash, hash(fp), sizeof(char)*8);

        int err = fclose(fp);
        if (err != 0) {
            perror("fclose");
            return fileNode;
        }
    }

    //if File is a directory
    else if (S_ISDIR(fileStat.st_mode)) {
        
        DIR *dir;
        dir = opendir(path);
        if (dir == NULL) {
            perror("opendir");
            return fileNode;
        }

        struct dirent *next_file;

        //setting linked list of contents
        while ((next_file = readdir(dir)) != NULL) {

            if (strncmp(&(next_file->d_name)[0], ".", sizeof(char)) != 0) {

                //creating new path
                char new_path[sizeof(char)*(strlen(path) + strlen(next_file->d_name) + 2)];
                strcpy(new_path, path);
                strcat(new_path, "/");
                strcat(new_path, next_file->d_name);

                //printf("File content: %s\n", fileNode->contents->fname);

                if (fileNode->contents == NULL) {
                    fileNode->contents = malloc(sizeof(struct TreeNode));
                    fileNode->contents = generate_ftree_HELPER(next_file->d_name, new_path);
                }

                else {
                    struct TreeNode* next_node = fileNode->contents;

                    struct TreeNode* list = malloc(sizeof(struct TreeNode));
                    list = generate_ftree_HELPER(next_file->d_name, new_path);

                    //keep incrementing next_node until space is found
                    while (next_node->next != NULL) {
                        next_node = next_node->next;
                    }
                    //set this next node
                    next_node->next = list;
                    
                }
            }
        }
        int err_dir = closedir(dir);
        if (err_dir != 0) {
            perror("closedir");
            return fileNode;
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
    int depth = 0;
    print_ftree_HELPER(root, depth);
    
}


void print_ftree_HELPER(struct TreeNode *node, int depth) {

    if (node != NULL) {

        //checking if node is a directory
        if (node->hash == NULL) {

            printf("%*s=====  %s (%o) =====\n", depth * 2, "", 
                    node->fname, node->permissions);
            print_ftree_HELPER(node->contents, depth + 1);

        //if node is a reg file or a link  
        } else {
            printf("%*s%s (%o)\n",  depth * 2, "", node->fname, node->permissions);
        }

        print_ftree_HELPER(node->next, depth);
    }
}












