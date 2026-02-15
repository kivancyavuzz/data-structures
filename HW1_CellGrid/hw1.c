#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *up, *down, *left, *right;
} Node;

typedef struct {
    Node* head;
    size_t rows, cols;
} CellGrid;

//  Tüm hücrelerin toplamını döndüren fonksiyon
int sumgrid(CellGrid *g) {
    if (!g || !g->head) return 0;
    Node* row_ptr = g->head;
    while (row_ptr->up) row_ptr = row_ptr->up;
    while (row_ptr->left) row_ptr = row_ptr->left;

    int total = 0;
    while (row_ptr) {
        Node* col_ptr = row_ptr;
        while (col_ptr) {
            total += col_ptr->value;
            col_ptr = col_ptr->right;
        }
        row_ptr = row_ptr->down;
    }
    return total;
}

// Izgarayı ve tüm düğümleri bellekten temizler 
void freegrid(CellGrid *g) {
    if (!g || !g->head) return;
    Node* row_ptr = g->head;
    while (row_ptr->up) row_ptr = row_ptr->up;
    while (row_ptr->left) row_ptr = row_ptr->left;

    while (row_ptr) {
        Node* next_row = row_ptr->down;
        Node* col_ptr = row_ptr;
        while (col_ptr) {
            Node* next_col = col_ptr->right;
            free(col_ptr);
            col_ptr = next_col;
        }
        row_ptr = next_row;
    }
    free(g);
}

CellGrid* gridcreate(int rows, int cols, int initvalue) {
    if (rows <= 0 || cols <= 0) return NULL;

    CellGrid* grid = (CellGrid*)malloc(sizeof(CellGrid));
    if (!grid) return NULL;
    grid->rows = rows;
    grid->cols = cols;

    // Düğüm adreslerini tutmak için geçici dizi
    Node*** node_arr = (Node***)malloc(sizeof(Node**) * rows);
    for (int i = 0; i < rows; i++) {
        node_arr[i] = (Node**)malloc(sizeof(Node*) * cols);
        for (int k = 0; k < cols; k++) {
            node_arr[i][k] = (Node*)malloc(sizeof(Node));
            node_arr[i][k]->value = initvalue;
            node_arr[i][k]->up = node_arr[i][k]->down = node_arr[i][k]->left = node_arr[i][k]->right = NULL;
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i > 0) node_arr[i][j]->up = node_arr[i - 1][j];
            if (i < rows - 1) node_arr[i][j]->down = node_arr[i + 1][j];
            if (j > 0) node_arr[i][j]->left = node_arr[i][j - 1];
            if (j < cols - 1) node_arr[i][j]->right = node_arr[i][j + 1];
        }
    }

    // Head her zaman merkezde olmalı
    grid->head = node_arr[rows / 2][cols / 2];

    for (int i = 0; i < rows; i++) free(node_arr[i]);
    free(node_arr);

    return grid;
}

void gridupdate(CellGrid* grid, int col, int row, int new_value) {
    if (!grid || !grid->head) return;
    Node* pos = grid->head;
    if (row < 0) for (int i = 0; i > row; i--) pos = pos->up;
    else for (int i = 0; i < row; i++) pos = pos->down;
    
    if (col < 0) for (int i = 0; i > col; i--) pos = pos->left;
    else for (int i = 0; i < col; i++) pos = pos->right;

    pos->value = new_value;
}

int main() {
    // 1. Izgara oluştur (3x3, başlangıç değeri 10)
    CellGrid* myGrid = gridcreate(3, 3, 10);
    if (!myGrid) return 1;

    printf("Baslangic Toplami (Beklenen 90): %d\n", sumgrid(myGrid));

    // 2. Bazı hücreleri güncelle (Merkez 0,0 noktasıdır)
    gridupdate(myGrid, 0, 0, 50); // Head (merkez) 50 oldu
    gridupdate(myGrid, 1, 1, 20); // Sağ alt köşe 20 oldu
    
    printf("Guncelleme Sonrasi Toplam (Beklenen 140): %d\n", sumgrid(myGrid));

    // 3. Belleği temizle
    freegrid(myGrid);
    printf("Bellek temizlendi, program basariyla sonlandirildi.\n");

    return 0;
}