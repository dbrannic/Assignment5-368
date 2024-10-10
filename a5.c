#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Point {
    int x, y;
} Point;

typedef struct kdnode {
    Point point;
    struct kdnode *left, *right;    
} kdnode;

int comparex(const void *a, const void *b) {
    return ((Point *)a)->x - ((Point *)b)->x;
}

int comparey(const void *a, const void *b) {
    return ((Point *)a)->y - ((Point *)b)->y;
}

kdnode *build_tree(Point *points, int n, int depth) {
    if (n <= 0) return NULL;
    int axis = depth % 2;
    if (axis == 0) qsort(points, n, sizeof(Point), comparex);
    else qsort(points, n, sizeof(Point), comparey);
    int middle = n / 2;
    kdnode *node = (kdnode *)malloc(sizeof(kdnode));
    node->point = points[middle];
    node->left = build_tree(points, middle, depth + 1);
    node->right = build_tree(points + middle + 1, n - middle - 1, depth + 1);
    return node;
}

int distance_squared(Point p1, Point p2) {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

void search_tree(kdnode *root, Point center, int radius, int depth, int *count) {
    if (!root) return;
    int axis = depth % 2;
    int sqDist = distance_squared(root->point, center);
    int sqRad = radius * radius;
    if (sqDist <= sqRad) (*count)++;
    if (axis == 0) {
        if (center.x - radius <= root->point.x) {
            search_tree(root->left, center, radius, depth + 1, count);
        }
        if (center.x + radius >= root->point.x) {
            search_tree(root->right, center, radius, depth + 1, count);
        }
    }
    else {
        if (center.y - radius <= root->point.y) {
            search_tree(root->left, center, radius, depth + 1, count);
        }
        if (center.y + radius >= root->point.y) {
            search_tree(root->right, center, radius, depth + 1, count);
        }
    }
}

void freeTree(kdnode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
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

    Point points[10000000];
    int count = 0;
    while (fscanf(file, "%d %d", &points[count].x, &points[count].y) == 2) {
        count++;
        if (count >= 10000000) {
            perror("Error: Too many points\n");
            return 1;
        }
    }
    fclose(file);
    kdnode *root = build_tree(points, count, 0);

    int cx, cy, radius;
    while (scanf("%d %d %d", &cx, &cy, &radius) == 3) {
        int count2 = 0;
        Point center = {cx, cy};
        search_tree(root, center, radius, 0, &count2);
        printf("%d\n", count2);
    }
    freeTree(root);

    return 0;
}