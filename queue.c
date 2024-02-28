#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    head->next = head;
    head->prev = head;
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    element_t *node, *safe = NULL;
    list_for_each_entry_safe (node, safe, l, list) {
        if (node->value)
            free(node->value);
        free(node);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new_node = malloc(sizeof(element_t));

    s = strdup(s);
    new_node->value = s;

    new_node->list.next = head->next;
    head->next->prev = &new_node->list;

    new_node->list.prev = head;
    head->next = &new_node->list;

    // printf("insert.value = %p",&new_node->value);
    // printf("insert = %p",&new_node->list);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_node = malloc(sizeof(element_t));

    new_node->value = strdup(s);

    head->prev->next = &new_node->list;
    new_node->list.prev = head->prev;

    new_node->list.next = head;
    head->prev = &new_node->list;

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *node = list_first_entry(head, element_t, list);

    list_del_init(head->next);

    if (sp) {
        strncpy(sp, node->value, bufsize);
        strcat(sp, "\0");
    }

    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *node = list_last_entry(head, element_t, list);

    list_del_init(head->prev);

    if (sp) {
        strncpy(sp, node->value, bufsize);
        strcat(sp, "\0");
    }

    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;

    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list//
    if (head == NULL)
        return false;
    if (head->next == NULL)
        return false;

    struct list_head *cur = head;
    int size = q_size(head);

    for (int i = 0; i < size / 2 + 1; i++) {
        cur = cur->next;
    }

    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;

    list_del_init(cur);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs//
    struct list_head *cur = head;
    struct list_head *first = head->next;
    struct list_head *second = first->next;

    while (first != head) {
        if (second == head) {
            cur->next = first;
            first->prev = cur;

            first->next = head;
            head->prev = first;

            break;
        } else {
            first->next = second->next;
            second->next->prev = first;

            second->next = first;
            first->prev = second;

            cur->next = second;
            second->prev = cur;

            cur = first;
            first = first->next;
            second = first->next;
        }
        cur->next = head;
        head->prev = cur;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *h = head->prev;
    struct list_head *cur = head;
    struct list_head *t = NULL;
    list_for_each (t, head) {
        cur->next = cur->prev;
        cur->next->prev = cur;
        h = cur;
        cur = t;
    }
    cur->next = h;
    h->prev = cur;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
