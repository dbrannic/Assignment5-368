#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Point {
    int x, y;
} Point;

typedef struct tnode {
    Point point;
    int height;
    struct tnode *left, *right;
    int *y_list;        
    int y_count;         
} tnode;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(tnode *node) {
    return node ? node->height : 0;
}

void update_height(tnode *node) {
    node->height = 1 + max(height(node->left), height(node->right));
}

tnode* rotate_r(tnode* current) {
    tnode* new_root = current->left;
    current->left = new_root->right;
    new_root->right = current;
    update_height(current);
    update_height(new_root);
    return new_root;
}

tnode* rotate_l(tnode* current) {
    tnode* new_root = current->right;
    current->right = new_root->left;
    new_root->left = current;
    update_height(current);
    update_height(new_root);
    return new_root;
}

tnode* rotate_r_l(tnode* current) {
    current->right = rotate_r(current->right);
    return rotate_l(current);
}

tnode* rotate_l_r(tnode* current) {
    current->left = rotate_l(current->left);
    return rotate_r(current);
}

tnode* balance(tnode* node) {
    update_height(node);
    int balance_factor = height(node->left) - height(node->right);

    if (balance_factor > 1) {
        if (height(node->left->left) >= height(node->left->right)) {
            node = rotate_r(node);
        } else {
            node = rotate_l_r(node);
        }
    } else if (balance_factor < -1) {
        if (height(node->right->right) >= height(node->right->left)) {
            node = rotate_l(node);
        } else {
            node = rotate_r_l(node);
        }
    }
    return node;
}

tnode* createNode(Point point) {
    tnode* new_node = (tnode*)malloc(sizeof(tnode));
    new_node->point = point;
    new_node->left = new_node->right = NULL;
    new_node->height = 1;
    new_node->y_list = (int*)malloc(sizeof(int) * 10); 
    new_node->y_count = 0;
    return new_node;
}

tnode* insert(tnode* node, Point point) {
    if (!node) {
        return createNode(point);
    }
    if (point.x < node->point.x || (point.x == node->point.x && point.y < node->point.y)) {
        node->left = insert(node->left, point);
    } else if (point.x > node->point.x || (point.x == node->point.x && point.y > node->point.y)) {
        node->right = insert(node->right, point);
    } else {
        if (node->y_count % 10 == 0) {
            node->y_list = (int*)realloc(node->y_list, sizeof(int) * (node->y_count + 10));
        }
        node->y_list[node->y_count++] = point.y;
    }
    return balance(node);
}

void range_search(tnode* node, int x_min, int x_max, int* y_values, int* count) {
    if (!node) return;

    if (node->point.x >= x_min && node->point.x <= x_max) {
        for (int i = 0; i < node->y_count; i++) {
            y_values[(*count)++] = node->y_list[i];
        }
    }

    if (node->point.x > x_min) {
        range_search(node->left, x_min, x_max, y_values, count);
    }
    if (node->point.x < x_max) {
        range_search(node->right, x_min, x_max, y_values, count);
    }
}

int isPointInCircle(Point p, int cx, int cy, int radius) {
    return (p.x - cx) * (p.x - cx) + (p.y - cy) * (p.y - cy) <= radius * radius;
}

int countPointsInCircle(tnode* root, int cx, int cy, int radius) {
    if (!root) return 0;
    int count = isPointInCircle(root->point, cx, cy, radius);
    count += countPointsInCircle(root->left, cx, cy, radius);
    count += countPointsInCircle(root->right, cx, cy, radius);
    return count;
}

void freeTree(tnode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root->y_list); 
    free(root);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <points_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    tnode* root = NULL;
    Point point;
    while (fscanf(file, "%d %d", &point.x, &point.y) == 2) {
        root = insert(root, point);
    }
    fclose(file);

    int cx, cy, radius;
    while (scanf("%d %d %d", &cx, &cy, &radius) == 3) {
        printf("%d\n", countPointsInCircle(root, cx, cy, radius));
    }

    freeTree(root);
    return 0;
}

