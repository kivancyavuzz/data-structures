#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char name[50];
    int start_year;
    int end_year;
    int income;
} VizierRecord;

static VizierRecord const history[] = {
        {"Lutfi Pasa", 1539, 1541, 155000},
        {"Mahmud Pasa", 1456, 1468, 4000000},
        {"Piri Mehmed", 1518, 1523, 145000},
        {"Candarli Ali", 1387, 1406, 82000},
        {"Sokollu Mehmed", 1565, 1579, 2000000},
        {"Pargali Ibrahim", 1523, 1536, 3000000},
        {"Koca Sinan", 1580, 1582, 210000},
        {"Alaeddin Pasa", 1320, 1331, 50000},
        {"Kemankes Mustafa", 1638, 1644, 230000},
        {"Gedik Ahmed", 1474, 1477, 115000},
        {"Koprulu Mehmed", 1656, 1661, 260000},
        {"Ayas Mehmed", 1536, 1539, 160000},
        {"Kuyucu Murad", 1606, 1611, 220000},
        {"Candarli Halil", 1364, 1387, 75000},
        {"Rustem Pasa", 1544, 1553, 2800000},
        {"Merzifonlu Kara", 1676, 1683, 290000},
        {"Semiz Ali", 1561, 1565, 170000},
        {"Ferhad Pasa", 1591, 1592, 190000},
        {"Nevsehirli Damat", 1718, 1730, 240000},
        {"Koprulu Fazil", 1661, 1676, 275000}
};

typedef struct Node{
    VizierRecord data;
    struct Node* left, *right, *parent;
    int height;
}Node;

int max(int a, int b){
    return a>b ? a : b;
}

int getHeight(Node* node){
    if(!node) return 0;
    return node->height;
}

int getBalance(Node* root){
    if(root == NULL) return 0;
    return getHeight(root->left) - getHeight(root->right);
}

Node* createNode(VizierRecord data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->height = 1;
    return node;
}

Node* leftRotate(Node* root){
    Node* newRoot = root->right;
    Node* orphan = root->right->left;
    newRoot->left = root;
    newRoot->parent = root->parent;
    root->right = orphan;
    if(orphan)
        orphan->parent = root;
    root->parent = newRoot;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}

Node* rightRotate(Node* root){
    Node* newNode = root->left;
    Node* orphan = root->left->right;
    newNode->right = root;
    newNode->parent = root->parent;
    root->left = orphan;
    if(orphan)
        orphan->parent = root;
    root->parent = newNode;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));

    return newNode;
}

Node* insertVizier(Node* node, VizierRecord data){
    if(!node)
        return createNode(data);
    if(node->data.start_year < data.start_year){
        node->right = insertVizier(node->right, data);
        node->right->parent = node;
    }
    else{
        node->left = insertVizier(node->left, data);
        node->left->parent = node;
    }
    
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if(balance > 1 && data.start_year < node->left->data.start_year){
        return rightRotate(node);
    }
    else if(balance < -1 && data.start_year > node->right->data.start_year){
        return leftRotate(node);
    }
    else if(balance > 1 && data.start_year > node->left->data.start_year){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    else if(balance < -1 && data.start_year < node->right->data.start_year){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Node* findPredecessors(Node* node){
    if(node == NULL) return NULL;

    if(node->left){
        Node* temp = node->left;
        while(temp->right)
            temp = temp->right;
        return temp;
    }

    Node* p = node->parent;
    Node* curr = node;
    while(p && curr == p->left){
        curr = p;
        p = p->parent;
    }

    return p;
}

Node* findSuccessors(Node* node){
    if(!node) return NULL;

    if(node->right){
        Node* temp = node->right;
        while(temp->left)
            temp = temp->left;
        return temp;
    }

    Node* p = node->parent;
    Node* curr = node;

    while(p && curr == p->right){
        curr = p;
        p = p->parent;
    }

    return p;
}

Node* findNode(Node* root, int year){
    if(root == NULL || root->data.start_year == year)
        return root;
    if(year < root->data.start_year)
        return findNode(root->left, year);
    return findNode(root->right, year);
}

void compareVizier(Node* root, int target_year, int window){
    Node* target = findNode(root, target_year);
    if (target == NULL) {
        printf("Vizier with start year %d not found!\n", target_year);
        return;
    }
    printf("QUERY: %d (%s)---\n", target_year, target->data.name);
    printf("[TARGET]\n");
    printf("> %s (%d Akce)\n", target->data.name, target->data.income);
    printf("[NEIGHBOR COMPARISON (Window: %d)]\n", window);
    Node* curr = target;
    for(int i = 0; i < window; i++){
        Node* temp = findPredecessors(curr);
        if(temp)
            printf("(<<) PREDECESSOR: %s (%d Akce)\n", temp->data.name, temp->data.income);
        curr = temp;
        if(!curr)
            break;
    }
    curr = target;
    for(int i = 0; i < window; i++){
        Node* temp = findSuccessors(curr);
        if(temp)
            printf("(<<) SUCCESSOR: %s (%d Akce)\n", temp->data.name, temp->data.income);
        curr = temp;
        if(!curr)
            break;
    }
}

int main() {
    Node* root = NULL;

    int n = sizeof(history) / sizeof(history[0]);

    for (int i = 0; i < n; i++) {
        root = insertVizier(root, history[i]);
    }

    compareVizier(root, 1523, 1);
    compareVizier(root, 1523, 2);

    return 0;
}