#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <time.h>

typedef struct __node {
    long value;
    struct list_head list;
} node_t;

/*Create an empty list*/
struct list_head *list_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Return number of nodes in list */
int list_length(struct list_head *head)
{
    int counter = 0;
    struct list_head *curr;
    list_for_each (curr, head)
        counter++;
    return counter;
}

void list_construct(struct list_head *head, int n)
{
    node_t *node = malloc(sizeof(node_t));
    node->value = n;
    list_add_tail(&node->list, head);
    return;
}

/*Free all storge used by list*/
void list_free(struct list_head *head)
{
    node_t *curr, *node;
    list_for_each_entry_safe (curr, node, head, list) {
        list_del(&curr->list);
        free(curr);
    }
    free(head);
}

/* Verify if list is order */
static bool list_is_ordered(struct list_head *head)
{       
    node_t *tmp;
    long val = list_first_entry(head, node_t, list)->value;
    list_for_each_entry(tmp, head, list) {
        long cmp = tmp->value;
        if (val > cmp)
            return false;
        val = cmp;
    }
    return true;
}

/* shuffle array, only work if n < RAND_MAX */
void shuffle(int *array, size_t n)
{
    if (n <= 0)
        return;
    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}


void quick_sort(struct list_head *head)
{
    int n = list_length(head);
    int i = 0;
    int max_level = n+1;
    struct list_head *begin[max_level];
    for (int i = 1; i < max_level; i++)
        begin[i] = list_new();
    struct list_head *result = list_new(), *left = list_new(), *right = list_new();
    begin[0] = head;
    int deepest = i;
    while (i >= 0) {
        struct list_head *L = begin[i]->next, *R = begin[i]->prev;
        if (L != R) {
            //rand_pivot(begin[i]);
            node_t *pivot = list_first_entry(begin[i], node_t, list);
            long val = pivot->value;

            list_del(&pivot->list);
            node_t *curr, *safe;
            list_for_each_entry_safe(curr, safe, begin[i], list) {
                list_del(&curr->list);
                list_add(&curr->list,curr->value > val ? right : left);                
            }

            list_splice_init(left, begin[i]);
            list_add(&pivot->list, begin[i + 1]);
            list_splice_init(right, begin[i + 2]);            
            i += 2;
            deepest = deepest < i ? i : deepest;            
        } else {
            if (list_is_singular(begin[i]))
                list_splice_init(begin[i], result);
            i--;
        }
    }
    head = result;

    /* print the result of sort*/
    /*
    node_t *tmp;
    list_for_each_entry(tmp, result, list) {
        int val = tmp->value;
        printf("%d", val);
    }
    */
    printf("level : %d\n", deepest);
}

void rand_pivot(struct list_head *list)
{
    int r = rand() % list_length(list);
    struct list_head *cur = list->next;
    for (int i = 0; i < r && cur->next != list; i++)
        cur = cur->next;
    list_move(cur, list);
    return;
}

int main(int argc, char **argv)
{
    struct list_head *list = list_new();
    srand( time(NULL) );
    int count = 10;
    clock_t start, end;
    int *test_arr = malloc(sizeof(int) * count);

    for (int i = 0; i < count; ++i)
        test_arr[i] = i;
    // shuffle(test_arr, count);

    while (count--)
        list_construct(list, test_arr[count]);
        
    start = clock();
    quick_sort(list);
    end = clock();
    assert(list_is_ordered(list));
    list_free(list);

    free(test_arr);
    
    double diff = end - start;
    printf(" %f  sec\n", diff / CLOCKS_PER_SEC );
    return 0;
}
