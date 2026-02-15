#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ödevdeki Item yapısı
typedef struct Item {
    char description[24];
    unsigned int price;
    unsigned int expiration;
} Item;

// Ağaç düğüm yapısı
typedef struct BSTNode {
    Item item;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

// 1. Yeni eleman ekleme (Fiyata göre sıralıyor)
BSTNode* insertItem(BSTNode* root, Item item) {
    if (root == NULL) {
        BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
        newNode->item = item;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (item.price < root->item.price)
        root->left = insertItem(root->left, item);
    else
        root->right = insertItem(root->right, item);

    return root;
}

// 2. Fiyat eşiğinin üzerindekileri artan sırada yazdır (In-order traversal)
void printAbovePrice(BSTNode* root, unsigned int threshold) {
    if (root == NULL) return;

    printAbovePrice(root->left, threshold);
    
    if (root->item.price > threshold) {
        printf("Urun: %s | Fiyat: %u USD | SKT: %u Gun\n", 
               root->item.description, root->item.price, root->item.expiration);
    }

    printAbovePrice(root->right, threshold);
}

// 3. Son kullanma tarihi yaklaşanları yazdır (Herhangi bir sırada olabilir)
void printExpiring(BSTNode* root, unsigned int days) {
    if (root == NULL) return;

    if (root->item.expiration < days) {
        printf("DIKKAT! %s urununun son kullanma tarihi geciyor (%u gun).\n", 
               root->item.description, root->item.expiration);
    }

    printExpiring(root->left, days);
    printExpiring(root->right, days);
}

// Belleği temizle
void freeTree(BSTNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    // Ödevdeki örnek veriler
    Item items[] = {
        {"Premium Milk", 12, 1},
        {"Toast Breads", 6, 2},
        {"Feta Cheese", 8, 24},
        {"Standard Milk", 3, 8},
        {"Salty Crackers", 4, 52},
        {"Orange Juice", 7, 30},
        {"Chocolate Bar", 2, 44}
    };
    int n = sizeof(items) / sizeof(items[0]);

    BSTNode* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insertItem(root, items[i]);
    }

    printf("--- 5 USD uzerindeki urunler ---\n");
    printAbovePrice(root, 5);

    printf("\n--- 10 gunden az suresi kalanlar ---\n");
    printExpiring(root, 10);

    freeTree(root);
    return 0;
}