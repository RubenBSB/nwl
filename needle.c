#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100

int max_of_two(int x, int y)
{
   return (x > y) ? x : y;
}

int max_of_three(int m, int n, int p)
{
   return max_of_two(max_of_two(m, n), p);
}

int min_of_three(int m, int n, int p)
{
   return -max_of_three(-m, -n, -p);
}

char** split_string_into_words(char* str, int *count) {

    char** words = malloc(MAX_WORDS * sizeof(char *));
    char* word = strtok(str, " ");
    int i = 0;

    while(word != NULL && i < MAX_WORDS) {
        words[i] = malloc(strlen(word) + 1);
        strcpy(words[i], word);
        word = strtok(NULL, " ");
        i++;
    }
    *count = i;

    return words;
}

typedef struct List {
    char* value;
    struct List* next;
} List;

typedef struct Alignment {
    List* aligned1;
    List* aligned2;
} Alignment;

void append_end(List** li, char* value) {

    List* new_node = malloc(sizeof(List));
    new_node->value = value;
    new_node->next = NULL;

    if (*li == NULL) {
        *li = new_node;
    }
    else {
        List* current = *li;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

int length(List* li) {
    if (li == NULL) {
        return 0;
    }
    else {
        return 1 + length(li->next);
    }
}

List* reverse(List* li) {
    if (li == NULL) {
        return li;
    }
    else {
        List* rev = reverse(li->next);
        append_end(&rev, li->value);
        return rev;
    }
}

void print_alignment(Alignment align) {
    for (List* node = align.aligned1; node != NULL; node = node->next){
        printf("%s ", node->value);
    }
    printf("\n");
    for (List* node = align.aligned2; node != NULL; node = node->next){
        printf("%s ", node->value);
    }
}

int levensthein(char* str1, char* str2) {
    int n = strlen(str1);
    int m = strlen(str2);

    int* v0 = malloc((n + 1) * sizeof(int));
    int* v1 = malloc((n + 1) * sizeof(int));

    for (int i = 0; i < n + 1; i++) {
        v0[i] = i;
    }

    for (int i = 0; i < m; i++) {
        v1[0] = i + 1;

        for (int j = 0; j < n; j++) {
            int deletion_cost = v0[j + 1] + 1;
            int insertion_cost = v1[j] + 1;
            
            int substitution_cost;
            if (str1[i] == str2[j]) {
                substitution_cost = v0[j];
            }
            else {
                substitution_cost = v0[j] + 1;   
            }

            v1[j + 1] = min_of_three(deletion_cost, insertion_cost, substitution_cost);
        }

        memcpy(v0, v1, (n + 1) * sizeof(int));
    }

    return v0[n];
}

int match_score(char* word1, char* word2, int MATCH_AWARD, int GAP_PENALTY) {
    if (strcmp(word1, word2) == 0) {
        return MATCH_AWARD;
    }
    else if (word1 == "-" || word2 == "-") {
        return GAP_PENALTY;
    }
    else {
        return -levensthein(word1, word2);
    }
}

Alignment needle(char* str1, char* str2, int MATCH_AWARD, int GAP_PENALTY) {

    int count_words1;
    int count_words2;

    char** words1 = split_string_into_words(str1, &count_words1);
    char** words2 = split_string_into_words(str2, &count_words2);

    int m = count_words1;
    int n = count_words2;


    int* score = (int *)malloc((m + 1) * (n + 1) * sizeof(int));
    for (int i = 0; i < m + 1;  i++) {
        for (int j = 0; j < n + 1; j++) {
            score[i * (n + 1) + j] = 0;
        }
    }

    
    for (int i = 0; i < m + 1;  i++) {
        score[i * (n + 1) + 0] = GAP_PENALTY * i;
    }
    for (int j = 0; j < n + 1;  j++) {
        score[j] = GAP_PENALTY * j;
    }
    for (int i = 1; i < m + 1;  i++) {
        for (int j = 1; j < n + 1; j++) {
            int match = score[(i-1) * (n + 1) + (j - 1)] + match_score(words1[i-1], words2[j-1], MATCH_AWARD, GAP_PENALTY);
            int delete = score[(i-1) * (n + 1) + j] + GAP_PENALTY;
            int insert = score[i * (n + 1) + (j - 1)] + GAP_PENALTY;
            score[i * (n + 1) + j] = max_of_three(match, delete, insert);
        }
    }

    List* aligned1 = NULL;
    List* aligned2 = NULL;
    
    int i = m;
    int j = n;
    while (i > 0 && j > 0) {
        int score_current = score[i * (n + 1) + j];
        int score_diagonal = score[(i-1) * (n + 1) + (j-1)];
        int score_up = score[i * (n + 1) + (j-1)];
        int score_left = score[(i-1) * (n + 1) + j];

        
        if (score_current == score_diagonal + match_score(words1[i-1], words2[j-1], MATCH_AWARD, GAP_PENALTY)) {
            append_end(&aligned1, words1[i-1]);
            append_end(&aligned2, words2[j-1]);
            i--;
            j--;
        }
        else if (score_current == score_left + GAP_PENALTY) {
            append_end(&aligned1, words1[i-1]);
            append_end(&aligned2, "-");
            i--;
        }
        else if (score_current == score_up + GAP_PENALTY) {
            append_end(&aligned1, "-");
            append_end(&aligned2, words2[j-1]);
            j--;
        }
    } 

    while (i > 0) {
        append_end(&aligned1, words1[i-1]);
        append_end(&aligned2, "-");
        i--;
    }

    while (j >0) {  
        append_end(&aligned1, "-");
        append_end(&aligned2, words2[j-1]);
        j--;
    }

    Alignment alignment = {reverse(aligned1), reverse(aligned2)};
    return alignment;
}

int main(int argc, char* argv[]) {
    const int MATCH_AWARD = 20;
    const int GAP_PENALTY = -5;

    char* seq1 = argv[1];
    char* seq2 = argv[2];

    Alignment align = needle(seq1, seq2, MATCH_AWARD, GAP_PENALTY);
    print_alignment(align);
    return 0;
}