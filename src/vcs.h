#ifndef VCS_H
#define VCS_H

#include "ramfs.h"

#define VCS_MAX_BLOBS 16
#define VCS_MAX_TREE_ENTRIES 16
#define VCS_MAX_COMMITS 16
#define VCS_MAX_COMMIT_MESSAGE 64

/* One file snapshot stored by the VCS. */
typedef struct
{
    char filename[RAMFS_MAX_FILENAME];
    char data[RAMFS_MAX_FILESIZE];
    int size;
    unsigned int hash;
    int used;
} vcs_blob_t;

/* One tracked file entry inside a tree. */
typedef struct
{
    char filename[RAMFS_MAX_FILENAME];
    int blob_index;
    int used;
} vcs_tree_entry_t;

/* One tree for a commit. */
typedef struct
{
    vcs_tree_entry_t entries[VCS_MAX_TREE_ENTRIES];
    int entry_count;
    int used;
} vcs_tree_t;

/* One commit in the history chain. */
typedef struct vcs_commit
{
    char message[VCS_MAX_COMMIT_MESSAGE];
    int tree_index;
    unsigned int hash;
    int used;
    struct vcs_commit *parent;
} vcs_commit_t;

/* Full VCS state stored in memory. */
typedef struct
{
    vcs_blob_t blobs[VCS_MAX_BLOBS];
    vcs_tree_t trees[VCS_MAX_TREE_ENTRIES];
    vcs_commit_t commits[VCS_MAX_COMMITS];
    vcs_commit_t *head;
    int initialized;
} vcs_t;

/* Global VCS instance. */
extern vcs_t vcs;

/* Initialize all VCS structures as empty. */
void vcs_init(void);

/* Hash a string using FNV-1a. */
unsigned int fnv1a_hash(const char *data);

/* Create one blob from one RAMFS file. */
int vcs_create_blob_from_file(const char *filename);

/* Create one empty tree. */
int vcs_create_tree(void);

/* Add one blob into one tree. */
int vcs_tree_add_blob(int tree_index, int blob_index);

/* Create one commit from one tree. */
int vcs_create_commit(int tree_index, const char *message);

#endif