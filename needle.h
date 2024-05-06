#ifndef __NEEDLE_H__
#define __NEEDLE_H__

typedef struct List {
    char* value;
    struct List* next;
} List;

typedef struct Alignment {
    List* aligned1;
    List* aligned2;
} Alignment;

int length(List*);

Alignment needle(char*, char*, int, int);

#endif