#include "ramfs.h"

ramfs_t ramfs;

/* Reset the RAM file system to an empty state. */
void ramfs_init(void)
{
    int i;
    int j;

    for (i = 0; i < RAMFS_MAX_FILES; i = i + 1)
    {
        ramfs.files[i].used = 0;
        ramfs.files[i].size = 0;

        for (j = 0; j < RAMFS_MAX_FILENAME; j = j + 1)
        {
            ramfs.files[i].name[j] = '\0';
        }

        for (j = 0; j < RAMFS_MAX_FILESIZE; j = j + 1)
        {
            ramfs.files[i].data[j] = '\0';
        }
    }
}