#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 6144

typedef struct list {
    long dist;
    struct list *next;
} list;

typedef list *list_t;

typedef struct tree {
    struct node *root;
} tree_t;

typedef struct node {
    long dist;
    long max;
    int moves;
    list_t vehicles;
    struct node *left;
    struct node *right;
    struct node *p;
} node_t;

void insert(tree_t *t, node_t *n);

void insert_list(list_t *l, long dist);

void delete(tree_t *t, node_t *n);

bool delete_list(list_t *l, long num);

node_t *tree_minimum(node_t *n);

node_t *tree_maximum(node_t *n);

node_t *tree_successor(node_t *n);

node_t *tree_predecessor(node_t *n);

node_t *search(node_t *n, long dist);

void process_input(tree_t *t, char *input, long *numbers);

void aggiungi_stazione(tree_t *t, node_t *staz);

void demolisci_stazione(tree_t *t, long staz);

void aggiungi_auto(tree_t *t, long staz, long aut);

void rottama_auto(tree_t *t, long staz, long aut);

void pianifica_percorso(tree_t *t, long staz1, long staz2);

int calculate_distance(node_t **nodes, int start, node_t *dest, int max);

long max_car(node_t *n);

long *get_numbers(long *numbers, char *str, int n);

/*
 * Please keep in mind that this project was evaluated only on the basis of execution time and memory occupied.
 * It could have been written much better, both stylistically and conceptually and it could have been better optimized.
 * Nevertheless, this code was enough to pass the exam with full grades, and it far exceeded the requirements of
 * execution time and memory, so I didn't bother to do better.
 */

int main() {
    char str[MAX_SIZE];
    long *numbers = malloc(MAX_SIZE);

    tree_t *t = malloc(sizeof(tree_t));
    t->root = NULL;

    while (fgets(str, MAX_SIZE, stdin) != NULL) {
        process_input(t, str, numbers);
    }

    free(t);
    free(numbers);
    return 0;
}

/**
 * @brief Creates a new node of a station with a given distance.
 *
 * @param dist The distance of the station from 0.
 *
 * @return The pointer to the newly allocated node.
 */
node_t *create_node(int dist) {
    node_t *n = malloc(sizeof(node_t));
    n->dist = dist;
    n->max = -1;
    n->moves = -1;
    n->vehicles = NULL;
    n->left = NULL;
    n->right = NULL;
    n->p = NULL;
    return n;
}

/**
 * @brief Parses and processes the command received from stdin.
 *
 * @param t The pointer to the tree that contains the nodes of the stations.
 * @param input The string to process.
 * @param numbers The pointer to the first element of the array where the numbers parsed from the string will be temporary stored.
 */
void process_input(tree_t *t, char *input, long *numbers) {
    char *args;

    //Command: "aggiungi-stazione"
    if (input[9] == 's') {
        args = input + 18;

        numbers = get_numbers(numbers, args, 2);
        int staz = numbers[0];
        int numAuto = numbers[1];
        node_t *nodo = create_node(staz);

        //Adds the cars to the list in the station node
        if (numAuto > 0) {
            long *distances = get_numbers(numbers, args, numAuto + 2);
            list_t alloc = malloc(numAuto * (sizeof(list_t) + 8));
            for (int i = 0; i < numAuto; i++) {
                list_t temp = &alloc[i];
                long autoDist = distances[i + 2];
                temp->dist = autoDist;
                temp->next = nodo->vehicles;
                nodo->vehicles = temp;
            }
        }
        aggiungi_stazione(t, nodo);

      //Command: "demolisci-stazione"
    } else if (input[0] == 'd') {
        args = input + 19;
        numbers = get_numbers(numbers, args, 1);
        long staz = numbers[0];
        demolisci_stazione(t, staz);

      //Command: "aggiungi-auto"
    } else if (input[9] == 'a') {
        args = input + 14;
        numbers = get_numbers(numbers, args, 2);
        long staz = numbers[0];
        long aut = numbers[1];
        aggiungi_auto(t, staz, aut);

      //Command: "rottama-auto"
    } else if (input[0] == 'r') {
        args = input + 13;
        numbers = get_numbers(numbers, args, 2);
        long staz = numbers[0];
        long aut = numbers[1];
        rottama_auto(t, staz, aut);

      //Command: "pianifica-percorso"
    } else if (input[0] == 'p') {
        args = input + 19;
        numbers = get_numbers(numbers, args, 2);
        long staz1 = numbers[0];
        long staz2 = numbers[1];
        pianifica_percorso(t, staz1, staz2);
    }
}

/**
 * @brief Creates a node of a station with a given distance and adds it to a tree.
 *
 * @param t The pointer to the tree that contains the nodes of the stations.
 * @param staz The pointer to the node of the station to add.
 */
void aggiungi_stazione(tree_t *t, node_t *staz) {
    if (search(t->root, staz->dist) == NULL) {
        insert(t, staz);
        puts("aggiunta");
    } else {
        puts("non aggiunta");
    }
}

/**
 * @brief Deletes from a tree the node of a station with a given distance.
 *
 * @param t The pointer to the tree that contains the nodes of the stations.
 * @param staz The distance of the station to remove.
 */
void demolisci_stazione(tree_t *t, long staz) {
    node_t *n = search(t->root, staz);
    if (n != NULL) {
        delete(t, n);
        puts("demolita");
    } else {
        puts("non demolita");
    }
}

/**
 * @brief Adds a car with a given autonomy in a given station
 *
 * @param t The pointer to the tree that contains the nodes of the stations.
 * @param staz The distance of the station that contains the car
 * @param aut The autonomy of the car to add.
 */
void aggiungi_auto(tree_t *t, long staz, long aut) {
    node_t *n = search(t->root, staz);
    if (n != NULL) {
        if (n->max > -1 && aut > n->max) {
            n->max = aut;
        }
        insert_list(&n->vehicles, aut);
        puts("aggiunta");
    } else {
        puts("non aggiunta");
    }
}

/**
 * @brief Removes a car with a given autonomy from a given station
 *
 * @param t The pointer to the tree that contains the nodes of the stations.
 * @param staz The distance of the station that contains the car.
 * @param aut The autonomy of the car to remove.
 */
void rottama_auto(tree_t *t, long staz, long aut) {
    node_t *n = search(t->root, staz);
    if (n != NULL) {
        if (delete_list(&n->vehicles, aut)) {
            if (aut == n->max) {
                n->max = -1;
            }
            puts("rottamata");
        } else {
            puts("non rottamata");
        }
    } else {
        puts("non rottamata");
    }
}

/**
 * @brief Calculates and prints the shortest path from a starting station to a destination.
 *
 * @param t The pointer to the tree that contains the nodes of the stations.
 * @param staz1 The distance of the starting station.
 * @param staz2 The distance of the destination station.
 */
void pianifica_percorso(tree_t *t, long staz1, long staz2) {
    //Case 1: The starting station and the destination are the same
    if (staz1 == staz2) {
        printf("%ld\n", staz1);
        return;
    }

    node_t *dest = search(t->root, staz1);
    node_t *start = search(t->root, staz2);

    bool reached = false;
    long result[512], moves = 0;

    //Case 2: The distance of the starting station is smaller
    if (dest->dist < start->dist) {
        node_t *curr = start;
        node_t *prec = tree_predecessor(start);
        if (prec == NULL) return;
        node_t *temp = curr;

        while (curr != dest) {
            while (prec != NULL) {
                long succTempMaxDist = prec->dist + max_car(prec);
                if (succTempMaxDist >= curr->dist && prec->dist >= dest->dist) {
                    temp = prec;
                    if (prec == dest) {
                        break;
                    }
                }
                prec = tree_predecessor(prec);
            }
            if (curr == temp) break;
            curr = temp;
            prec = tree_predecessor(curr);
            result[moves++] = curr->dist;
            if (curr == dest) {
                reached = true;
            }
        }

      //Case 3: The distance of the starting station is bigger
    } else {
        node_t *curr = start;
        node_t *succ = tree_successor(start);
        if (succ == NULL) return;
        node_t *temp = curr;
        register int k = 1, x = 0;
        node_t **nodes = calloc(6144, sizeof(node_t));
        nodes[0] = start;

        while (succ != NULL && succ->dist <= dest->dist) {
            succ->moves = -1;
            nodes[k] = succ;
            ++k;
            succ = tree_successor(succ);
        }
        k = 1;
        succ = nodes[k];
        while (curr != dest) {
            int minDist = 2147483647;
            while (succ != NULL) {
                long succTempMaxDist = succ->dist - max_car(succ);
                if (succTempMaxDist <= curr->dist) {
                    if (succ->dist == dest->dist) {
                        reached = true;
                        temp = succ;
                        break;
                    }
                    int distance;
                    if (succ->moves != -1) {
                        distance = succ->moves;
                    } else {
                        distance = calculate_distance(nodes, k, dest, minDist);
                        succ->moves = distance;
                    }
                    if (distance < minDist && distance >= 0) {
                        minDist = distance;
                        temp = succ;
                        x = k;
                    }
                }
                succ = nodes[++k];
            }
            if (curr == temp) break;
            curr = temp;
            k = x + 1;
            succ = nodes[k];
            result[moves++] = curr->dist;
            if (curr == dest) {
                reached = true;
            }
        }
    }
    if (reached) {
        //If the destination has been successfully reached, print the path
        result[moves] = dest->dist;
        for (int j = moves - 1; j >= 0; j--) {
            printf("%ld ", result[j]);
        }
        printf("%ld\n", start->dist);
    } else {
        puts("nessun percorso");
    }
}

/**
 * @brief Inserts a given node in the tree.
 *
 * @param t The pointer to the tree that contains the nodes of the stations.
 * @param n The pointer to the node to add.
 */
void insert(tree_t *t, node_t *n) {
    node_t *y = NULL;
    node_t *x = t->root;
    while (x != NULL) {
        y = x;
        if (n->dist < x->dist) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    n->p = y;
    if (y == NULL) {
        t->root = n;
    } else if (n->dist < y->dist) {
        y->left = n;
    } else {
        y->right = n;
    }
}

/**
 * @brief Calculates the node with the minimum key in a tree.
 *
 * @param n The pointer to the node from which to start the search (usually the root of the tree).
 *
 * @return The pointer to the node with the minimum key.
 */
node_t *tree_minimum(node_t *n) {
    while (n->left != NULL) {
        n = n->left;
    }
    return n;
}

/**
 * @brief Calculates the node with the maximum key in a tree.
 *
 * @param n The pointer to the node from which to start the search (usually the root of the tree).
 *
 * @return The pointer to the node with the maximum key.
 */
node_t *tree_maximum(node_t *n) {
    while (n->right != NULL) {
        n = n->right;
    }
    return n;
}

/**
 * @brief Calculates the value of the autonomy of the car with greater range in a given station and saves the
 * value in the node of the station to be able to retrieve it without unnecessarily calculating it again.
 * If a value is already present in the node, the function retrieves it immediately.
 *
 * @param n The pointer to the node of the station.
 *
 * @return The value of the autonomy of the car with greater range in the station.
 */
long max_car(node_t *n) {
    if (n->max > -1)
        return n->max; //Returns the max without calculating it again, if present.

    list_t l = n->vehicles;
    long max = 0;
    while (l != NULL) {
        if (l->dist > max) {
            max = l->dist;
        }
        l = l->next;
    }
    n->max = max;
    return max;
}

/**
 * @brief Calculates the node with the smallest key greater than the key of a given node.
 *
 * @param n The pointer to the input node.
 *
 * @return The pointer to the successor of the input node.
 */
node_t *tree_successor(node_t *n) {
    if (n->right != NULL) {
        return tree_minimum(n->right);
    }
    node_t *y = n->p;
    while (y != NULL && n == y->right) {
        n = y;
        y = y->p;
    }
    return y;
}

/**
 * @brief Calculates the node with the largest key smaller than the key of a given node.
 *
 * @param n The pointer to the input node.
 *
 * @return The pointer to the predecessor of the input node.
 */
node_t *tree_predecessor(node_t *n) {
    if (n->left != NULL) {
        return tree_maximum(n->left);
    }
    node_t *y = n->p;
    while (y != NULL && n == y->left) {
        n = y;
        y = y->p;
    }
    return y;
}

/**
 * @brief Deletes a node from a given tree.
 *
 * @param t The pointer to the tree that contains the nodes of the stations.
 * @param n The pointer to the node to delete.
 */
void delete(tree_t *t, node_t *n) {
    node_t *y;
    node_t *x;
    if (n->left == NULL || n->right == NULL) {
        y = n;
    } else {
        y = tree_successor(n);
    }
    if (y->left != NULL) {
        x = y->left;
    } else {
        x = y->right;
    }
    if (x != NULL) {
        x->p = y->p;
    }
    if (y->p == NULL) {
        t->root = x;
    } else if (y == y->p->left) {
        y->p->left = x;
    } else {
        y->p->right = x;
    }
    if (y != n) {
        n->dist = y->dist;
        n->vehicles = y->vehicles;
        n->max = y->max;
    }
    free(y);
}

/**
 * @brief Deletes from a given list the first element with a given key, if present.
 *
 * @param l The pointer to the head of the list.
 * @param num The key of the element to delete.
 *
 * @return true if a value is found and deleted, false otherwise.
 */
bool delete_list(list_t *l, long num) {
    list_t curr = *l;

    if (curr != NULL && curr->dist == num) {
        *l = curr->next;
        return true;
    }

    list_t prec = curr;
    curr = curr->next;
    while (curr != NULL) {
        if (curr->dist == num) {
            prec->next = curr->next;
            return true;
        }
        prec = curr;
        curr = curr->next;
    }
    return false;
}

/**
 * @brief Inserts at the head of a given list an element with a specified key.
 *
 * @param l The pointer to the head of the list.
 * @param dist The value of the key of the element to add.
 */
void insert_list(list_t *l, long dist) {
    list_t temp = malloc(sizeof(list_t) + 8);
    temp->dist = dist;
    temp->next = *l;
    *l = temp;
}

/**
 * @brief Searches if a node with a given key is present in a tree.
 *
 * @param n The pointer to the node from which to start the search (usually the root of the tree).
 * @param dist The value of the key of the node to search.
 *
 * @return The pointer to the node if found, NULL otherwise.
 */
node_t *search(node_t *n, long dist) {
    if (n == NULL || dist == n->dist) {
        return n;
    }
    if (dist < n->dist) {
        return search(n->left, dist);
    } else {
        return search(n->right, dist);
    }
}

/**
 * @brief Calculates the minimum number of moves needed to reach a destination station.
 *
 * @param nodes The array of nodes in which to perform the calculation.
 * @param start The index of the starting station in the nodes array.
 * @param dest The pointer to the node of the destination station.
 * @param max The maximum number of iterations of the search. If the number of moves needed exceeds this value the function will stop.
 *
 * @return The number of needed moves. If the return value is -1 the number of iterations exceeded max or an error occurred.
 */
int calculate_distance(node_t **nodes, int start, node_t *dest, int max) {

    node_t *curr = nodes[start];
    node_t *succ = nodes[start + 1];
    if (succ == NULL) return -1;
    if (succ == dest) return 1;
    register int i = 1, j = 0, moves = 0;
    node_t *temp = curr;
    node_t *stations[1024];

    while (curr != dest) {
        while (succ != NULL) {
            if (succ->moves + moves >= max) return -1;
            long succTempMaxDist = succ->dist - max_car(succ);
            if (succTempMaxDist <= curr->dist) {
                if (succ == dest) {
                    for (int x = moves - 1; x >= 0; x--) {
                        stations[x]->moves = moves - x;
                    }
                    curr->moves = 1;
                    return ++moves;
                }
                temp = succ;
                j = i;
            }
            succ = nodes[start + ++i];
        }
        if (curr == temp) break;
        curr = temp;
        stations[moves] = curr;
        i = j + 1;
        succ = nodes[start + i];
        ++moves;
        if (moves >= max) {
            return -1;
        }
    }
    return moves;
}

/**
 * @brief Parses the first n numbers in the input string, converts them to an integer, and inserts them in an array.
 *
 * @param numbers Array where the numbers will be stored.
 * @param str The input string.
 * @param n The first n numbers to parse from the string.
 *
 * @return The pointer to the first element of the input array containing the parsed numbers.
 */
long *get_numbers(long *numbers, char *str, int n) {
    register int i = 0, j = 0;
    long temp = 0;
    while (str[i] != '\0' && str[i] != '\n') {
        temp *= 10;
        temp += (str[i] - '0');
        ++i;
        if (str[i] == ' ') {
            numbers[j] = temp;
            ++j;
            if (j >= n) break; //Stop after parsing n numbers
            temp = 0;
            ++i;
            continue;
        }
        if (str[i] == '\n' || str[i] == '\r' || str[i] == '\0') {
            numbers[j] = temp;
            break;
        }
        if (str[i] < '0' || str[i] > '9') {
            ++i;
        }
    }
    return numbers;
}
