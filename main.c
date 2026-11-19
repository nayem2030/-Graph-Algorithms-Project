#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX 100

typedef struct {
    int u, v, w;
} Edge;


void BuildGraph(int vertices, int edgeCount, Edge edges[], int graph[MAX][MAX]) {
    for (int i = 0; i < vertices; i++)
        for (int j = 0; j < vertices; j++)
            graph[i][j] = 0;

    for (int i = 0; i < edgeCount; i++) {
        graph[edges[i].u][edges[i].v] = edges[i].w;
        graph[edges[i].v][edges[i].u] = edges[i].w;
    }
}


int FindMinDistance(int vertices, int distance[], bool included[]) {
    int minimum = INT_MAX;
    int minVertex = -1;
    for (int i = 0; i < vertices; i++) {
        if (included[i] == false && distance[i] < minimum) {
            minimum = distance[i];
            minVertex = i;
        }
    }
    return minVertex;
}

void Dijkstra(int vertices, int graph[MAX][MAX], int source,
              int distance[], int parent[]) {
    bool included[MAX];

    for (int i = 0; i < vertices; i++) {
        distance[i] = INT_MAX;
        parent[i] = -1;
        included[i] = false;
    }
    distance[source] = 0;

    for (int count = 0; count < vertices; count++) {
        int currentVertex = FindMinDistance(vertices, distance, included);
        if (currentVertex == -1)
            break;
        included[currentVertex] = true;

        for (int j = 0; j < vertices; j++) {
            if (graph[currentVertex][j] != 0 &&
                included[j] == false &&
                distance[currentVertex] != INT_MAX &&
                distance[currentVertex] + graph[currentVertex][j] < distance[j]) {
                distance[j] = distance[currentVertex] + graph[currentVertex][j];
                parent[j] = currentVertex;
            }
        }
    }
}


int FindParent(int parent[], int x) {
    while (parent[x] != x)
        x = parent[x];
    return x;
}

void SortEdges(Edge edges[], int edgeCount) {  
    for (int i = 0; i < edgeCount - 1; i++) {
        for (int j = 0; j < edgeCount - i - 1; j++) {
            if (edges[j].w > edges[j + 1].w) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
}

void Kruskal(int vertices, int edgeCount, Edge original[], int tree[MAX][MAX]) {
    Edge edges[MAX * MAX];
    int parent[MAX];
    int count = 0;

    for (int i = 0; i < edgeCount; i++)
        edges[i] = original[i];
    SortEdges(edges, edgeCount);

    for (int i = 0; i < vertices; i++) {
        parent[i] = i;
        for (int j = 0; j < vertices; j++)
            tree[i][j] = 0;
    }

    for (int i = 0; i < edgeCount && count < vertices - 1; i++) {
        int parentU = FindParent(parent, edges[i].u);
        int parentV = FindParent(parent, edges[i].v);

        if (parentU != parentV) {
            tree[edges[i].u][edges[i].v] = edges[i].w;
            tree[edges[i].v][edges[i].u] = edges[i].w;
            parent[parentU] = parentV;
            count++;
        }
    }
}


int FindMinCost(int vertices, int minCost[], bool included[]) {
    int minimum = INT_MAX;
    int minVertex = -1;
    for (int i = 0; i < vertices; i++) {
        if (included[i] == false && minCost[i] < minimum) {
            minimum = minCost[i];
            minVertex = i;
        }
    }
    return minVertex;
}

void Prim(int vertices, int graph[MAX][MAX], int tree[MAX][MAX]) {
    int minCost[MAX], parents[MAX];
    bool included[MAX];

    for (int i = 0; i < vertices; i++) {
        minCost[i] = INT_MAX;
        parents[i] = -1;
        included[i] = false;
        for (int j = 0; j < vertices; j++)
            tree[i][j] = 0;
    }
    minCost[0] = 0;                       

    for (int count = 0; count < vertices; count++) {
        int CurrentVertex = FindMinCost(vertices, minCost, included);
        if (CurrentVertex == -1)          
            break;
        included[CurrentVertex] = true;

        if (parents[CurrentVertex] != -1) {
            int p = parents[CurrentVertex];
            tree[p][CurrentVertex] = graph[p][CurrentVertex];
            tree[CurrentVertex][p] = graph[p][CurrentVertex];
        }

        for (int j = 0; j < vertices; j++) {
            if (graph[CurrentVertex][j] != 0 &&
                included[j] == false &&
                graph[CurrentVertex][j] < minCost[j]) {
                minCost[j] = graph[CurrentVertex][j];
                parents[j] = CurrentVertex;
            }
        }
    }
}


bool BFSPath(int vertices, int tree[MAX][MAX], int source, int destination,
             int path[], int *length, int *cost) {
    int queue[MAX], front = 0, rear = 0;
    int parent[MAX];
    bool visited[MAX];

    for (int i = 0; i < vertices; i++) {
        parent[i] = -1;
        visited[i] = false;
    }

    queue[rear++] = source;
    visited[source] = true;

    while (front < rear) {
        int current = queue[front++];
        for (int j = 0; j < vertices; j++) {
            if (tree[current][j] != 0 && visited[j] == false) {
                visited[j] = true;
                parent[j] = current;
                queue[rear++] = j;
            }
        }
    }

    if (visited[destination] == false)
        return false;


    int temp[MAX], n = 0;
    for (int v = destination; v != -1; v = parent[v])
        temp[n++] = v;

    *length = n;
    *cost = 0;
    for (int i = 0; i < n; i++)
        path[i] = temp[n - 1 - i];
    for (int i = 0; i < n - 1; i++)
        *cost += tree[path[i]][path[i + 1]];
    return true;
}

void PrintPath(int path[], int length, int cost) {
    for (int i = 0; i < length; i++) {
        printf("%d", path[i]);
        if (i != length - 1)
            printf(" -> ");
    }
    printf(" (Cost: %d)\n", cost);
}


void Solve(int vertices, int edgeCount, Edge edges[], int source,
           int destination, bool updated) {
    int graph[MAX][MAX], kTree[MAX][MAX], pTree[MAX][MAX];
    int distance[MAX], parent[MAX];
    int path[MAX], length, cost;

    BuildGraph(vertices, edgeCount, edges, graph);

   
    Dijkstra(vertices, graph, source, distance, parent);
    printf(updated ? "Updated Shortest Path: " : "Shortest Path: ");
    if (distance[destination] == INT_MAX) {
        printf("Not Possible\n");
    } else {
        int temp[MAX], n = 0;
        for (int v = destination; v != -1; v = parent[v])
            temp[n++] = v;
        for (int i = 0; i < n; i++)
            path[i] = temp[n - 1 - i];
        PrintPath(path, n, distance[destination]);
    }

    Kruskal(vertices, edgeCount, edges, kTree);
    printf(updated ? "Updated Kruskal Path:  " : "Kruskal Path:   ");
    if (BFSPath(vertices, kTree, source, destination, path, &length, &cost))
        PrintPath(path, length, cost);
    else
        printf("Not Possible\n");


    Prim(vertices, graph, pTree);
    printf(updated ? "Updated Prim Path:     " : "Prim Path:      ");
    if (BFSPath(vertices, pTree, source, destination, path, &length, &cost))
        PrintPath(path, length, cost);
    else
        printf("Not Possible\n");
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("File not found!\n");
        return 1;
    }

    int vertices, edgeCount;
    Edge edges[MAX * MAX], newEdges[MAX * MAX];
    int source, destination, removeU, removeV;

    fscanf(file, "%d %d", &vertices, &edgeCount);
    for (int i = 0; i < edgeCount; i++)
        fscanf(file, "%d %d %d", &edges[i].u, &edges[i].v, &edges[i].w);
    fscanf(file, "%d %d", &source, &destination);
    fscanf(file, "%d %d", &removeU, &removeV);
    fclose(file);


    Solve(vertices, edgeCount, edges, source, destination, false);


    int newEdgeCount = 0;
    for (int i = 0; i < edgeCount; i++) {
        if (!((edges[i].u == removeU && edges[i].v == removeV) ||
              (edges[i].u == removeV && edges[i].v == removeU))) {
            newEdges[newEdgeCount++] = edges[i];
        }
    }

    printf("After Removing Edge (%d,%d):\n", removeU, removeV);
    Solve(vertices, newEdgeCount, newEdges, source, destination, true);

    return 0;
}