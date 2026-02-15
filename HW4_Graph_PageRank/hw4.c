#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Edge {
    struct Node *dst;
    int num_link;
} Edge;
typedef struct Node {
    int webpage_id;
    struct Edge **adjacents;
    int num_adj;
    int capacity;
} Node;
typedef struct Graph {
    Node **nodes;
    int num_node;
    int capacity;
}Graph;

Graph* createGraph(){
    Graph* newGraph = (Graph*)malloc(sizeof(Graph));
    newGraph->capacity = 0;
    newGraph->nodes = NULL;
    newGraph->num_node = 0;
    return newGraph;
}

Node* createNode(int id){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->adjacents = NULL;
    newNode->capacity = 0;
    newNode->num_adj = 0;
    newNode->webpage_id = id;
    return newNode;
}

void addNodeToGraph(Graph* g, int id){
    if(g->num_node == g->capacity){
        int newCapacity = g->capacity == 0 ? 4 : g->capacity * 2;
        Node** newNodes = malloc(sizeof(Node*) * newCapacity);
        for(int i = 0; i < g->num_node; i++){
            newNodes[i] = g->nodes[i];
        }
        free(g->nodes);
        g->nodes = newNodes;
        g->capacity = newCapacity;
    }
    g->nodes[g->num_node++] = createNode(id);
}

void addEdgeToNode(Node* n, Node* adj_n, int weight){
    for(int i = 0; i < n->num_adj; i++){
        if(n->adjacents[i]->dst == adj_n){
            n->adjacents[i]->num_link += weight;
            return;
        }
    }
    if(n->capacity == n->num_adj){
        int newCapacity = n->capacity == 0 ? 2 : n->capacity * 2;
        Edge** newAdj = (Edge**)malloc(sizeof(Edge*) * newCapacity);
        for(int i = 0; i < n->num_adj; i++){
            newAdj[i] = n->adjacents[i];
        }
        free(n->adjacents);
        n->adjacents = newAdj;
        n->capacity = newCapacity;
    }
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->dst = adj_n;
    newEdge->num_link = weight;

    n->adjacents[n->num_adj++] = newEdge;
}

Node* findNode(Graph* g, int id){
    for(int i = 0; i < g->num_node; i++){
        if(g->nodes[i]->webpage_id == id)
            return g->nodes[i];
    }
    return NULL;
}

void removeEdge(Node* n, Node* adj_n){
    for(int i = 0; i < n->num_adj; i++){
        if(n->adjacents[i]->dst == adj_n){
            free(n->adjacents[i]);
            n->adjacents[i] = n->adjacents[n->num_adj-1];
            n->num_adj -= 1;
            return;
        }
    }
}

void removeNode(Graph* g, int id){
    Node* target = findNode(g, id);
    if(!target) return;
    for(int i = 0; i < g->num_node; i++){
        if(g->nodes[i] != target)
            removeEdge(g->nodes[i], target);
    }
    for(int i = 0; i < target->num_adj; i++){
        free(target->adjacents[i]);
    }
    free(target->adjacents);
    for(int i = 0; i < g->num_node; i++){
        if(g->nodes[i]->webpage_id == id){
            g->nodes[i] = g->nodes[g->num_node-1];
            g->nodes[g->num_node-1] = NULL;
            g->num_node--;
            break;
        }
    }
    free(target);
}

int get_node_index(Graph *graph, int webpage_id) {
    for (int i = 0; i < graph->num_node; i++) {
        if (graph->nodes[i]->webpage_id == webpage_id) { 
            return i;
        }
    }
    return -1; 
}

Graph* undirectedGraphFromFile(const char* filename){
    FILE* fp = fopen(filename, "r");
    if(!fp){
        printf("File couldn't open");
        return NULL;
    }

    char line[1024];
    int size = 0;
    while(fgets(line, sizeof(line), fp)) 
        size++;
    rewind(fp);

    Graph* newGraph = createGraph();
    for(int i = 0; i < size; i++){
        addNodeToGraph(newGraph, i);
    }
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            int weight;
            if(fscanf(fp, "%d", &weight) != 1){
                printf("File couldn't be read");
                free(newGraph);
                fclose(fp);
                return NULL;
            }
            if(weight > 0 && j > i){
                addEdgeToNode(newGraph->nodes[i], newGraph->nodes[j], weight);
                addEdgeToNode(newGraph->nodes[j], newGraph->nodes[i], weight);
            }
        }
    }
    fclose(fp);
    return newGraph;
}

Graph* directedGraphFromFile(const char* filename){
    FILE* fp = fopen(filename, "r");
    int N = 0;
    char line[1024];
    while (fgets(line, sizeof(line), fp)) 
        N++;
    rewind(fp);

    Graph* g = createGraph();

    for(int i = 0; i < N; i++)
        addNodeToGraph(g, i);

    int val;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            fscanf(fp, "%d", &val);
            if(val > 0){
                addEdgeToNode(g->nodes[i], g->nodes[j], val);
            }
        }
    }
    fclose(fp);
    return g;
}

double* pagerank_random_walk(Graph* g, double damping_factor, int num_steps){
    int n = g->num_node;
    int* visits = (int*)calloc(n, sizeof(int));
    double* pagerank = (double*)calloc(n, sizeof(double));
    int curr_idx = rand() % n;

    for(int step = 0; step < num_steps; step++){
        visits[curr_idx]++;
        Node* curr_ptr = g->nodes[curr_idx];
        double rand_val = (double)rand() / ((double)RAND_MAX + 1.0);
        if(curr_ptr->num_adj == 0 || rand_val > damping_factor){
            curr_idx = rand() % n;
        }
        else{
            int total_link = 0;
            for(int i = 0; i < curr_ptr->num_adj; i++)
                total_link += curr_ptr->adjacents[i]->num_link;
            int random_link = rand() % total_link;
            int link_sum = 0;
            for(int i = 0; i < curr_ptr->num_adj; i++){
                link_sum += curr_ptr->adjacents[i]->num_link;
                if(random_link < link_sum){
                    curr_idx = curr_ptr->adjacents[i]->dst->webpage_id;
                    break;
                }
            }

        }
    }
    for(int i = 0; i < n; i++){
        pagerank[i] = (double)visits[i] / num_steps;
    }
    free(visits);
    return pagerank;
}

int shortest_path(Graph* g, int src_id, int dst_id){
    if(!g) return -1;
    if(src_id == dst_id) return 0;
    int src_idx = get_node_index(g, src_id);
    int dst_idx = get_node_index(g, dst_id);
    if(src_idx < 0 || dst_id < 0) return -1;

    int n = g->num_node;
    int* dist = (int*)malloc(n * sizeof(int));
    int* visited = (int*)calloc(n, sizeof(int));
    int* q = (int*)malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
        dist[i] = -1;
    }
    int head = 0, tail = 0;
    int result = -1;
    q[tail++] = src_idx;
    dist[src_idx] = 0;
    visited[src_idx] = 1;
    while(head < tail){
        int curr_idx = q[head++];
        if(curr_idx == dst_idx){
            result = dist[curr_idx];
            break;
        }
        Node* curr_ptr = g->nodes[curr_idx];
        for(int i = 0; i < curr_ptr->num_adj; i++){
            Node* adj_ptr = curr_ptr->adjacents[i]->dst;
            int next_idx = get_node_index(g, adj_ptr->webpage_id);
            if(!visited[next_idx]){
                visited[next_idx] = 1;
                dist[next_idx] = dist[curr_idx] + 1;
                q[tail++] = next_idx;
            }
        }
    }
    free(dist);
    free(visited);
    free(q);
    return result;
}

void freeGraph(Graph *g) {
    for (int i = 0; i < g->num_node; i++) {
        Node *node = g->nodes[i];
        for (int j = 0; j < node->num_adj; j++) {
            free(node->adjacents[j]);
        }
        free(node->adjacents);
        free(node);
    }
    free(g->nodes);
    free(g);
}

int main() {
    srand(time(NULL));
    
    printf("=== PAGERANK TESTI ===\n");
    Graph *graph = directedGraphFromFile("web.txt");
    if (graph == NULL) {
        return 1;
    }
    
    printf("Graph yuklendi: %d node\n", graph->num_node);
    
    double *pr = pagerank_random_walk(graph, 0.85, 1000000);
    
    printf("\nPageRank sonuclari:\n");
    for (int i = 0; i < graph->num_node; i++) {
        printf("Node %d: %.6f\n", i, pr[i]);
    }
    free(pr);
    freeGraph(graph);

    printf("\n=== BFS TESTI ===\n");
    Graph *undirected_graph = undirectedGraphFromFile("web.txt");
    if (undirected_graph == NULL) {
        return 1;
    }
    
    printf("Undirected graph yuklendi: %d node\n", undirected_graph->num_node);
    
    // BFS test
    printf("\nEn kisa yol testleri:\n");
    printf("0 -> 8: %d\n", shortest_path(undirected_graph, 0, 8));
    printf("1 -> 5: %d\n", shortest_path(undirected_graph, 1, 5));
    printf("0 -> 0: %d\n", shortest_path(undirected_graph, 0, 0));
    
    freeGraph(undirected_graph);

    return 0;
}