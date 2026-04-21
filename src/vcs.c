#include "vcs.h"

vcs_t vcs;

/* Initialize all VCS structures as empty. */
void vcs_init(void)
{
    int i;
    int j;

    vcs.initialized = 1;
    vcs.head = 0;

    for (i = 0; i < VCS_MAX_BLOBS; i = i + 1)
    {
        vcs.blobs[i].size = 0;
        vcs.blobs[i].hash = 0;
        vcs.blobs[i].used = 0;

        for (j = 0; j < RAMFS_MAX_FILENAME; j = j + 1)
        {
            vcs.blobs[i].filename[j] = '\0';
        }

        for (j = 0; j < RAMFS_MAX_FILESIZE; j = j + 1)
        {
            vcs.blobs[i].data[j] = '\0';
        }
    }

    for (i = 0; i < VCS_MAX_TREE_ENTRIES; i = i + 1)
    {
        vcs.trees[i].entry_count = 0;
        vcs.trees[i].used = 0;

        for (j = 0; j < VCS_MAX_TREE_ENTRIES; j = j + 1)
        {
            vcs.trees[i].entries[j].blob_index = -1;
            vcs.trees[i].entries[j].used = 0;

            {
                int k;
                for (k = 0; k < RAMFS_MAX_FILENAME; k = k + 1)
                {
                    vcs.trees[i].entries[j].filename[k] = '\0';
                }
            }
        }
    }

    for (i = 0; i < VCS_MAX_BLOBS; i = i + 1)
    {
        vcs.commits[i].tree_index = -1;
        vcs.commits[i].hash = 0;
        vcs.commits[i].used = 0;
        vcs.commits[i].parent = 0;

        for (j = 0; j < VCS_MAX_COMMIT_MESSAGE; j = j + 1)
        {
            vcs.commits[i].message[j] = '\0';
        }
    }
}