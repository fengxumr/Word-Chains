/*

Task 1: compute and output, for each word ω, all words that could immediately follow ω in a word chain.

    Time complexity: O(m*(n^2))
    Explanation: 
    Time complexity of check two words: O(m) -- there is only one loop in the check_words function.
    Time complexity of finding all max paths and output them: O(n^2) -- two loop for checking all the words' relation with others.

Task 2: compute and output
a. the maximum length of a word chain that can be built from the given words, 
b. all word chains of maximum length that can be built from the given words.

    Time complexity: O(n^2)
    Explanation:
    Time complexity of computing the max length of all nodes: O(n^2) -- need compute each node with the others.
    Time complexity of finding all max paths and output them: O(n^2).

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"

#define WORD_LENGTH 25
#define WORDS_QTY 1000

void findChain(Graph g, int n, char ** words);
void findMax(Graph g, char ** words);
void dfsPath(char ** words, int ** next, int container[], int n, int i);

int main(int argc, char *argv[]) {
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);

    char ** words = calloc(n, sizeof(char *));
    assert(words != NULL);
    for (int i = 0; i < n; i++) {
        words[i] = calloc(WORD_LENGTH, sizeof(char));
        assert(words[i] != NULL);
    }

    for (int i = 0; i < n; i++) {
        printf("Enter word: ");
        scanf("%s", words[i]);
    }

    printf("\n");

    Graph g = newGraph(n);
    findChain(g, n, words);

    for (int i = 0; i < n; i++) {
        printf("%s: ", words[i]);

        for (int j = i + 1; j < n; j++) {
            if (g->edges[i][j]) {
                printf("%s ", words[j]);
            }
        }
        printf("\n");
    }
    printf("\n");

    findMax(g, words);
    freeGraph(g);

    for (int i = 0; i < n; i++) {
        free(words[i]);
    }
    free(words);

    return 0;
}


void findChain(Graph g, int n, char ** words) {
    int unmatch, index;
    int seq = 0;

    for (int i = 0; i < n; i++) {
        for (int a = i + 1; a < n; a++) {
            int gap = strlen(words[i]) - strlen(words[a]);

            unmatch = 0;
            if (gap == 1) {
                for (int b = 0; b < strlen(words[i]) && unmatch <= 1; b++) {
                    if (words[i][b] != words[a][b - unmatch]) {
                        unmatch++;
                    }
                }
            } else if (gap == 0) {
                for (int b = 0; b < strlen(words[i]) && unmatch <= 1; b++) {
                    if (words[i][b] != words[a][b]) {
                        unmatch++;
                    }
                }
            } else if (gap == -1) {
                for (int b = 0; b < strlen(words[a]) && unmatch <= 1; b++) {
                    if (words[i][b - unmatch] != words[a][b]) {
                        unmatch++;
                    }
                }
            }

            if (unmatch == 1) {
                Edge e;
                e.v = i;
                e.w = a;
                insertEdge(g, e);
            }
        }
    }

    return;
}

void findMax(Graph g, char ** words) {
    int count, maxNode, maxValue;

    int * dist = calloc(g->nV, sizeof(int));
    assert(dist != NULL);

    int ** next = calloc(g->nV, sizeof(int *));
    assert(next != NULL);
    for (int a = 0; a < g->nV; a++) {
        next[a] = calloc(g->nV, sizeof(int));
        assert(next[a] != NULL);
        for (int b = 0; b < g->nV; b++) {
            next[a][b] = -1;
        }
    }

    for (int i = g->nV - 1; i >= 0 ; i--) {
        maxNode = -1;
        maxValue = -1;

        for (int j = i + 1; j < g->nV; j++) {
            if (g->edges[i][j]) {
                if (maxNode < 0 || (maxNode >= 0 && maxValue < dist[j])) {
                    maxNode = j;
                    maxValue = dist[j];
                }
            }
        }

        dist[i] = maxValue + 1;

        count = 0;
        for (int k = i + 1; k < g->nV; k++) {
            if (dist[k] == maxValue && g->edges[i][k]) {
                next[i][count] = k;
                count++;
            }
        }
    }

    for (int l = 0; l < g->nV; l++) {
        if (dist[l] > maxValue) {
            maxValue = dist[l];
        }
    }

    printf("Maximum chain length: %d\n", maxValue + 1);
    printf("Maximal chains:\n");

    for (int l = 0; l < g->nV; l++) {
        if (dist[l] == maxValue) {


            int container[g->nV];
            for (int m = 0; m < g->nV; m++) {
                container[m] = -1;
            }

            dfsPath(words, next, container, g->nV, l);

        }
    }

    free(dist);
    for (int a = 0; a < g->nV; a++) {
        free(next[a]);
    }
    free(next);

    return;
}


void dfsPath(char ** words, int ** next, int container[], int n, int i) {

    if (container[0] == -1) {
        container[0] = i;
    }

    for (int j = 0; j < n && next[i][j] >= 0; j++) {
        int index = 0;
        while (container[index] >= 0) {
            index++;
        }

        int container2[n];
        for (int m = 0; m < n; m++) {
            container2[m] = container[m];
        }
        container2[index] = next[i][j];
        dfsPath(words, next, container2, n, next[i][j]);
    }

    int k;
    if (next[i][0] < 0) {
        for (k = 0; k < n - 1 && container[k] >= 0 && container[k + 1] >= 0; k++) {
            printf("%s -> ", words[container[k]]);
        }
        printf("%s\n", words[container[k]]);
    }
}









