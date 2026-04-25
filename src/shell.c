#include "shell.h"
#include "terminal.h"
#include "ramfs.h"
#include "vcs.h"

static int strings_equal(const char *a, const char *b)
{
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
        {
            return 0;
        }

        i = i + 1;
    }

    return a[i] == '\0' && b[i] == '\0';
}



static void shell_help(void)
{
    puts("Available commands:\n");
    puts("create <file>\n");
    puts("write <file> <text>\n");
    puts("read <file>\n");
    puts("delete <file>\n");
    puts("commit <file> <message>\n");
    puts("log\n");
}

static void print_result(int result)
{
    if (result == 0)
    {
        puts("OK\n");
    }
    else
    {
        puts("ERROR\n");
    }
}

static void shell_create(const char *filename)
{
    puts("> create ");
    puts(filename);
    puts("\n");

    print_result(ramfs_create(filename));
}

static void shell_write(const char *filename, const char *text)
{
    puts("> write ");
    puts(filename);
    puts(" ");
    puts(text);
    puts("\n");

    print_result(ramfs_write(filename, text));
}

static void shell_read(const char *filename)
{
    const char *content;

    puts("> read ");
    puts(filename);
    puts("\n");

    content = ramfs_read(filename);

    if (content == 0)
    {
        puts("ERROR\n");
    }
    else
    {
        puts(content);
        puts("\n");
    }
}

static void shell_delete(const char *filename)
{
    puts("> delete ");
    puts(filename);
    puts("\n");

    print_result(ramfs_delete(filename));
}

static void shell_commit_file(const char *filename, const char *message)
{
    int blob_index;
    int tree_index;
    int add_result;
    int commit_index;

    puts("> commit ");
    puts(filename);
    puts(" ");
    puts(message);
    puts("\n");

    blob_index = vcs_create_blob_from_file(filename);

    if (blob_index < 0)
    {
        puts("ERROR: could not create blob\n");
        return;
    }

    tree_index = vcs_create_tree();

    if (tree_index < 0)
    {
        puts("ERROR: could not create tree\n");
        return;
    }

    add_result = vcs_tree_add_blob(tree_index, blob_index);

    if (add_result != 0)
    {
        puts("ERROR: could not add blob to tree\n");
        return;
    }

    commit_index = vcs_create_commit(tree_index, message);

    if (commit_index < 0)
    {
        puts("ERROR: could not create commit\n");
        return;
    }

    puts("Commit created\n");
}

void shell_start(void)
{

    puts("Mini-Git RAMFS Shell\n");
    puts("Simulated user command demo:\n");
    shell_help(); 
    shell_create("notes.txt");
    shell_write("notes.txt", "hello from user shell");
    shell_read("notes.txt");
    shell_commit_file("notes.txt", "first user commit");
    shell_write("notes.txt", "updated text");
    shell_read("notes.txt");

    shell_commit_file("notes.txt", "second user commit");

    puts("> log\n");
    vcs_print_log();
    shell_delete("notes.txt");
    shell_read("notes.txt");
    puts("Shell demo finished.\n");

    while (1)
    {
    }
}

