#include <stdint.h>
#include "terminal.h"
#include "ramfs.h"
#include "vcs.h"

#define MULTIBOOT2_HEADER_MAGIC 0xe85250d6

/* This header lets GRUB recognize the kernel as multiboot2. */
const unsigned int multiboot_header[] __attribute__((section(".multiboot"))) = {
    MULTIBOOT2_HEADER_MAGIC,
    0,
    24,
    -(MULTIBOOT2_HEADER_MAGIC + 24),
    0,
    8};

/* Print an unsigned int in decimal form. */
static void print_uint(unsigned int value)
{
    char buffer[16];
    int i = 0;

    if (value == 0)
    {
        putc('0');
        return;
    }

    while (value > 0)
    {
        buffer[i] = (char)('0' + (value % 10));
        value = value / 10;
        i = i + 1;
    }

    while (i > 0)
    {
        i = i - 1;
        putc(buffer[i]);
    }
}

void main(void)
{
    int first_result;
    int second_result;
    int write_result;
    int delete_result;
    int blob_index;
    const char *content;
    unsigned int hash1;
    unsigned int hash2;
    unsigned int hash3;

    /* Phase 0 */
    puts("COMP 310 project booted successfully.\n");
    puts("Terminal output layer is working.\n");

    /* Phase 1 */
    ramfs_init();
    puts("RAMFS initialized.\n");

    first_result = ramfs_create("notes.txt");

    if (first_result == 0)
    {
        puts("First create succeeded: notes.txt\n");
    }
    else
    {
        puts("First create failed: notes.txt\n");
    }

    second_result = ramfs_create("notes.txt");

    if (second_result == -1)
    {
        puts("Second create correctly failed because file already exists.\n");
    }
    else
    {
        puts("Second create did not fail as expected.\n");
    }

    write_result = ramfs_write("notes.txt", "hello from ramfs");

    if (write_result == 0)
    {
        puts("Write succeeded for notes.txt\n");
    }
    else
    {
        puts("Write failed for notes.txt\n");
    }

    content = ramfs_read("notes.txt");

    if (content != 0)
    {
        puts("Read succeeded for notes.txt\n");
        puts("File contents: ");
        puts(content);
        puts("\n");
    }
    else
    {
        puts("Read failed for notes.txt\n");
    }

    delete_result = ramfs_delete("notes.txt");

    if (delete_result == 0)
    {
        puts("Delete succeeded for notes.txt\n");
    }
    else
    {
        puts("Delete failed for notes.txt\n");
    }

    content = ramfs_read("notes.txt");

    if (content == 0)
    {
        puts("Read after delete correctly failed.\n");
    }
    else
    {
        puts("Read after delete did not fail as expected.\n");
    }

    /* Phase 2 */
    vcs_init();
    puts("VCS initialized.\n");

    hash1 = fnv1a_hash("hello from ramfs");
    hash2 = fnv1a_hash("hello from ramfs");
    hash3 = fnv1a_hash("different text");

    puts("Hash test 1: ");
    print_uint(hash1);
    puts("\n");

    puts("Hash test 2: ");
    print_uint(hash2);
    puts("\n");

    puts("Hash test 3: ");
    print_uint(hash3);
    puts("\n");

    if (hash1 == hash2)
    {
        puts("Matching content produced matching hashes.\n");
    }
    else
    {
        puts("Matching content did not produce matching hashes.\n");
    }

    if (hash1 != hash3)
    {
        puts("Different content produced different hashes.\n");
    }
    else
    {
        puts("Different content did not produce different hashes.\n");
    }

    /* Phase 2 Blob */
    first_result = ramfs_create("story.txt");

    if (first_result == 0)
    {
        puts("Blob test file create succeeded: story.txt\n");
    }
    else
    {
        puts("Blob test file create failed: story.txt\n");
    }

    write_result = ramfs_write("story.txt", "hello blob world");

    if (write_result == 0)
    {
        puts("Blob test file write succeeded: story.txt\n");
    }
    else
    {
        puts("Blob test file write failed: story.txt\n");
    }

    blob_index = vcs_create_blob_from_file("story.txt");

    if (blob_index >= 0)
    {
        puts("Blob creation from RAMFS succeeded.\n");

        puts("Blob index: ");
        print_uint((unsigned int)blob_index);
        puts("\n");

        puts("Blob filename: ");
        puts(vcs.blobs[blob_index].filename);
        puts("\n");

        puts("Blob contents: ");
        puts(vcs.blobs[blob_index].data);
        puts("\n");

        puts("Blob size: ");
        print_uint((unsigned int)vcs.blobs[blob_index].size);
        puts("\n");

        puts("Blob hash: ");
        print_uint(vcs.blobs[blob_index].hash);
        puts("\n");
    }
    else
    {
        puts("Blob creation from RAMFS failed.\n");
    }

    while (1)
    {
    }
}