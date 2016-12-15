#include    <stdio.h>
#include    <stdlib.h>
#include    <stdbool.h>
#include    <time.h>
//#include    <malloc.h>
#include    <string.h>

#include    "resource_detector.h"

#undef      malloc
#undef      free
#undef      main
#undef      fopen
#undef      fclose

#include    "list.h"

#define MAGIC_CODE      0x78563412

#define ADDR(addr,offset)               ((addr) + (offset))
#define SET_MAGIC_CODE(addr,offset)     *((int*) ADDR(addr,offset)) = MAGIC_CODE
#define MAGIC_CODE_OK(addr,offset)      (*((int*) ADDR(addr,offset)) == MAGIC_CODE)

typedef struct
{
    char*         addr;
    unsigned int  size;
    const char*   file;
    unsigned int  line;
} mem_data;

typedef struct
{
    FILE*         addr;
    const char*   file_to_open;
    const char*   mode;
    const char*   file;
    unsigned int  line;
} file_data;

static list_admin* memlist = NULL;
static list_admin* filelist = NULL;

static int memory_compare(const void* meminfo, const void* address, size_t size)
{
    size = size; // to prevent compiler warnings
    mem_data* info = (mem_data*)meminfo;
    char*     addr = (char*)address;
    return info->addr - addr;
}

static int file_compare(const void* fileinfo, const void* address, size_t size)
{
    size = size; // to prevent compiler warnings
    file_data* info = (file_data*)fileinfo;
    FILE*      addr = (FILE*)address;
    return info->addr - addr;
}


static void
invalidate (mem_data* info)
{
    char* user_addr;
    char* raw_addr;
    unsigned int size;

    user_addr = info->addr;
    size = info->size;
    raw_addr = ADDR (user_addr, -sizeof(int));

    if (!MAGIC_CODE_OK(user_addr, -sizeof(int)) || !MAGIC_CODE_OK(user_addr, size))
    {
        fprintf (stderr, "ERROR: addr %p (%s, line %d): out-of-bound access\n", user_addr, info->file, info->line);
    }

    memset (raw_addr, 0xff, size + (2 * sizeof(int)));
    free (raw_addr);
}

/*
 * replacement of malloc
 */
extern void*
xmalloc (unsigned int size, const char* file, unsigned int line)
{
    char* raw_addr = malloc (size + (2 * sizeof(int)));
    char* user_addr;
    mem_data info = {NULL, 0, NULL, 0};

    if (raw_addr == NULL)
    {
        fprintf (stderr, "ERROR: malloc failed for %d bytes (%s, line %d)\n", size, file, line);
        return (NULL);
    }
    else
    {
        user_addr = ADDR (raw_addr, sizeof (int));
        SET_MAGIC_CODE (raw_addr, 0);
        SET_MAGIC_CODE (user_addr, size);

        info.addr = user_addr;
        info.size = size;
        info.file = file;
        info.line = line;
        list_add_tail(memlist, &info);
    }
    return ((void*) user_addr);
}

/*
 * replacement of free
 */
extern void
xfree (void* addr, const char* filename, int linenr)
{
    char* user_addr = (char*) addr;
    mem_data* info = NULL;

    /* check if allocated memory is in our list and retrieve its info */
    int index = list_index_of_special(memlist, addr, memory_compare);
    info = list_get_element(memlist, index);

    if (info == NULL)
    {
        fprintf (stderr, "ERROR: trying to free memory that was not malloc-ed (addr %p) in %s : %d\n", user_addr, filename, linenr);
        return;
    }

    invalidate (info);
    list_delete_item(memlist, index);
}

static void
print_empty_lines(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        fprintf(stderr, "\n");
    }
}

static void set_colour_red(void)
{
    fprintf(stderr, "\033[10;31m");
}
static void set_colour_gray(void)
{
    fprintf(stderr, "\033[22;37m");
}
static void
print_line(void)
{
    fprintf (stderr, "---------------------------------------------------------\n");
}

static void
print_not_good(void)
{
    set_colour_gray();
    print_line();
    set_colour_red();
    fprintf (stderr, " #     #                                             ###\n");
    fprintf (stderr, " ##    #  ####  #####     ####   ####   ####  #####  ###\n");
    fprintf (stderr, " # #   # #    #   #      #    # #    # #    # #    # ###\n");
    fprintf (stderr, " #  #  # #    #   #      #      #    # #    # #    #  # \n");
    fprintf (stderr, " #   # # #    #   #      #  ### #    # #    # #    #    \n");
    fprintf (stderr, " #    ## #    #   #      #    # #    # #    # #    # ###\n");
    fprintf (stderr, " #     #  ####    #       ####   ####   ####  #####  ###\n");
    set_colour_gray();
    print_line();
}

/*
 * writes all info of the unallocated memory
 */
static void
resource_detection (void)
{ 
    time_t now = time (NULL);

    int forgotten_frees = list_get_nr_elements(memlist);
    int nr_files_open = list_get_nr_elements(filelist);

    if ((forgotten_frees > 0) || (nr_files_open > 0))
    {
        print_empty_lines(15);
    }

    if (forgotten_frees > 0)
    {
        mem_data* info = NULL;
        print_line();
        fprintf (stderr, "Memory Leak Summary, generated: %s", ctime (&now));
        print_not_good();
        set_colour_red();
        while((info = list_get_element(memlist, 0)) != NULL)
        {
            fprintf (stderr, "forgot to free address: %p (%d bytes) that was allocated in: %s on line: %d\n",
                    info->addr, info->size, info->file, info->line);
            list_delete_item(memlist, 0);
        }
        set_colour_gray();
        print_line();
        print_empty_lines(1);
    }

    if (nr_files_open > 0)
    {
        file_data* info = NULL;
        print_line();
        fprintf (stderr, "File Management Summary, generated: %s", ctime (&now));
        print_not_good();
        set_colour_red();
        while((info = list_get_element(filelist, 0)) != NULL)
        {
            fprintf (stderr, "forgot to close file: %s (ptr %p, mode \"%s\") that was opened from: %s on line: %d\n",
                    info->file_to_open, (void*)(info->addr), info->mode, info->file, info->line);
            list_delete_item(filelist, 0);
        }
        set_colour_gray();
        print_line();
        print_empty_lines(1);
    }
    if ((forgotten_frees == 0) && (nr_files_open == 0))
    {
        printf ("\nResource checks: OK\n\n");
    }

    destruct_list(&memlist);
    destruct_list(&filelist);
}

extern FILE*
xfopen (const char* file_to_open, const char* mode, const char* filename, int linenr)
{
    file_data info = {NULL, NULL, 0};
    info.addr = fopen(file_to_open, mode);
    if (info.addr != NULL)
    {
        info.file_to_open = file_to_open;
        info.mode = mode;
        info.file = filename;
        info.line = linenr;
        list_add_tail(filelist, &info);
    }
    return info.addr;
}

extern int
xfclose(FILE* fptr, const char* filename, int linenr)
{
    int index = list_index_of_special(filelist, fptr, file_compare);
    if (index < 0)
    {
        fprintf (stderr, "ERROR: trying to close an unopened file in %s on line number %d.\n", filename, linenr);
        return -1;
    }
    list_delete_item(filelist, index);
    return (fclose (fptr));
}

extern int
main (int argc, char* argv[])
{
    atexit (resource_detection);
    memlist = construct_list(sizeof(mem_data));
    filelist = construct_list(sizeof(file_data));

    return (xmain (argc, argv));
}
