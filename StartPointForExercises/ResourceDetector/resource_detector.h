#ifndef  RESOURCE_DETECTOR_H
#define  RESOURCE_DETECTOR_H

#include <stdio.h>

#define  main               xmain
#define  malloc(size)       xmalloc ((size), (__FILE__), (__LINE__))
#define  free(addr)         xfree ((addr), __FILE__, __LINE__)
#define  fopen(name,mode)   xfopen ((name),(mode), __FILE__, __LINE__)
#define  fclose(fptr)       xfclose ((fptr), __FILE__, __LINE__)

extern int   xmain(int argc, char* argv[]);
extern void* xmalloc(unsigned int size, const char* file, unsigned int line);
extern void  xfree(void* addr, const char* filename, int linenr);
extern FILE* xfopen(const char* file_to_open, const char* mode, const char* filename, int linenr);
extern int   xfclose(FILE* fptr, const char* filename, int linenr);

#endif
