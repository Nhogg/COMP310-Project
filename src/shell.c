#include "shell.h"
#include "terminal.h"
#include "ramfs.h"
#include "vcs.h"

#define SHELL_INPUT_MAX 128

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

static void copy_token(char *dest, const char *src, int max)
{
    int i = 0;
    while (i < max - 1 && src[i] != '\0' && src[i] != ' ')
    {
        dest[i] = src[i];
        i = i + 1;
    }
    dest[i] = '\0';
}

static const char *next_token(const char *str)
{
    while (*str != '\0' && *str != ' ')
    {
        str = str + 1;
    }
    while (*str == ' ')
    {
        str = str + 1;
    }
    return str;
}

static void copy_rest(char *dest, const char *src, int max)
{
    int i = 0;
    while (i < max - 1 && src[i] != '\0')
    {
        dest[i] = src[i];
        i = i + 1;
    }
    dest[i] = '\0';
}

static int shell_read_line(char *buffer)
{
    int i = 0;
    char c;

    while (i < SHELL_INPUT_MAX - 1)
    {
        c = getc();

        if (c == '\r' || c == '\n')
        {
            putc('\n');
            break;
        }

        if (c == '\b' || c == 127)
        {
            if (i > 0)
            {
                i = i - 1;
                puts("\b \b");
            }
            continue;
        }

        putc(c);
        buffer[i] = c;
        i = i + 1;
    }

    buffer[i] = '\0';
    return i;
}

static void shell_help(void)
{
    puts("Available commands:\n");
    puts("  create <file>\n");
    puts("  write <file> <text>\n");
    puts("  read <file>\n");
    puts("  delete <file>\n");
    puts("  commit <file> <message>\n");
    puts("  log\n");
    puts("  help\n");
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
    print_result(ramfs_create(filename));
}

static void shell_write(const char *filename, const char *text)
{
    print_result(ramfs_write(filename, text));
}

static void shell_read(const char *filename)
{
    const char *content;
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
    print_result(ramfs_delete(filename));
}

static void shell_commit_file(const char *filename, const char *message)
{
    int blob_index;
    int tree_index;
    int add_result;
    int commit_index;

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

static void shell_dispatch(const char *line)
{
    char cmd[16];
    char arg1[RAMFS_MAX_FILENAME];
    char arg2[SHELL_INPUT_MAX];
    const char *p;

    if (line[0] == '\0')
    {
        return;
    }

    copy_token(cmd, line, 16);
    p = next_token(line);
    copy_token(arg1, p, RAMFS_MAX_FILENAME);
    p = next_token(p);
    copy_rest(arg2, p, SHELL_INPUT_MAX);

    if (strings_equal(cmd, "create"))
    {
        shell_create(arg1);
    }
    else if (strings_equal(cmd, "write"))
    {
        shell_write(arg1, arg2);
    }
    else if (strings_equal(cmd, "read"))
    {
        shell_read(arg1);
    }
    else if (strings_equal(cmd, "delete"))
    {
        shell_delete(arg1);
    }
    else if (strings_equal(cmd, "commit"))
    {
        shell_commit_file(arg1, arg2);
    }
    else if (strings_equal(cmd, "log"))
    {
        vcs_print_log();
    }
    else if (strings_equal(cmd, "help"))
    {
        shell_help();
    }
    else
    {
        puts("Unknown command. Type help.\n");
    }
}

void shell_start(void)
{
    char line[SHELL_INPUT_MAX];

    puts("Mini-Git RAMFS Shell\n");
    puts("Type help for available commands.\n");

    while (1)
    {
        puts("user@os:~$ ");
        shell_read_line(line);
        shell_dispatch(line);
    }
}
