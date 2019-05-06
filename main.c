#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int dim; // number of nodes
int start; // start node
int *routes; // matrix with routes
int *nodes; // nodes

int readData() {
    FILE *f = fopen("routes.dat", "r");

    if (!f)
        return 0;

    fscanf(f, "%d %d", &dim, &start);

    routes = (int *)malloc(sizeof(int) * dim * dim);
    nodes = (int *)malloc(sizeof(int) * dim);

    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++) {
            fscanf(f, "%d", routes + i * dim + j);
            
            if ((i == j) && ((i + 1) != start))
                *(routes + i * dim + j) = INT_MAX;
        }

    for (int i = 0; i < dim; i++)
        *(nodes + i) = (((i) + 1) == (start)) ? 1 : 0;

    fclose(f);

    return 1;
}

int allAreSet() {
    for (int i = 0; i < dim; i++)
        if (*(nodes + i) == 0)
            return 0;

    return 1;
}

int getMinimal(int node) {
    int neigh = -1;
    int route = 0;

    for (int i = 0; i < dim; i++)
        if ((neigh == -1) && (*(nodes + i) == 0) && (*(routes + node * dim + i) != 0)) { // we've found first neighbour node
            neigh = i;
            route = *(routes + node * dim + i);
        }
        else if ((*(nodes + i) == 0) && (*(routes + node * dim + i) != 0) && (*(routes + node * dim + i) < route)) { // we've found next minimal neighbour node
            neigh = i;
            route = *(routes + node * dim + i);
        }

    return neigh;
}

int getNext() {
    if (allAreSet())
        return 0;

    int next = -1;
    int route = 0;

    for (int i = 0; i < dim; i++)
        if ((next == -1) && (*(nodes + i) != 1)) { // we've found first node for comparison
            next = i;
            route = *(routes + i * dim + i);
        }
        else if ((*(nodes + i) != 1) && (*(routes + i * dim + i) < route)) { // we've found next minimal node
            next = i;
            route = *(routes + i * dim + i);
        }

    return next;
}

int main(int argc, char *argv[]) {
    if (!readData()) {
        printf("Error while reading routes data!\n");
        return 1;
    }

    int node = start - 1;
    int next;

    while (!allAreSet()) {
        while ((next = getMinimal(node)) != -1) { // get next minimal neighbour
            if ((*(routes + node * dim + node) + *(routes + node * dim + next)) < *(routes + next * dim + next)) // reduce route
                *(routes + next * dim + next) = *(routes + node * dim + node) + *(routes + node * dim + next);

            *(nodes + next) = 2;
        }

        for (int i = 0; i < dim; i++)
            if (*(nodes + i) == 2)
                *(nodes + i) = 0;

        *(nodes + node) = 1;

        if ((node = getNext()) == -1) // get next starting node
            break;
    }

    printf("Minimal routes from node #%d to:\n", start);
    
    for (int i = 0; i < dim; i++)
        if ((i + 1) != start)
            printf("node #%d = %d\n", i + 1, *(routes + i * dim + i));

    free(routes);
    free(nodes);

    return 0;
}
