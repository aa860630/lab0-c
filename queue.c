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
    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);
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

    if (!new_node) {
        return false;
    }

    s = strdup(s);
    new_node->value = s;
    if (!new_node->value) {
        free(new_node);
        return false;
    }

    list_add(&new_node->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_node = malloc(sizeof(element_t));

    if (!new_node) {
        return false;
    }

    new_node->value = strdup(s);
    if (!new_node->value) {
        free(new_node);
        return false;
    }

    list_add_tail(&new_node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

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
    if (!head || list_empty(head))
        return NULL;

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
    if (!head)
        return false;

    struct list_head *cur = head;
    element_t *mid;
    int size = q_size(head);

    for (int i = 0; i < size / 2 + 1; i++) {
        cur = cur->next;
    }

    // cur->prev->next = cur->next;
    // cur->next->prev = cur->prev;

    list_del_init(cur);
    mid = container_of(cur, element_t, list);
    free(mid->value);
    free(mid);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    element_t *cur, *safe;
    bool cmp = 0;
    list_for_each_entry_safe (cur, safe, head, list) {
        if (safe != container_of(head, element_t, list) &&
            strcmp(cur->value, safe->value) == 0) {
            list_del_init(&cur->list);
            free(cur->value);
            free(cur);
            cmp = 1;
        } else {
            if (cmp == 1) {
                list_del_init(&cur->list);
                free(cur->value);
                free(cur);
            }
            cmp = 0;
        }
    }
    //&safe->list != &head
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
    // base cases...
    if (head == NULL)
        return;
    if (head->next == NULL)
        return;

    // 1 case jo hum handle krenge...
    struct list_head *cur = head;
    struct list_head *first = head->next;
    struct list_head *tail = first;
    int len, i;
    len = q_size(head);
    while (len >= k) {
        for (i = 1; i < k; i++) {
            tail = tail->next;
        }
        cur->next = tail->next;
        tail->next->prev = cur;

        first->prev = tail;
        tail->next = first;
        q_reverse(first->prev);

        first->next = cur->next;
        cur->next->prev = first;

        cur->next = tail;
        tail->prev = cur;

        cur = first;
        first = cur->next;
        tail = first;

        len = len - k;
    }
    // // https://leetcode.com/problems/reverse-nodes-in-k-group/
}
struct list_head *merge(struct list_head *l1, struct list_head *l2)
{
    // merge with pseudo node
    struct list_head new_queue;
    struct list_head tmp1;
    struct list_head tmp2;

    struct list_head *cur;

    INIT_LIST_HEAD(&new_queue);
    INIT_LIST_HEAD(&tmp1);
    INIT_LIST_HEAD(&tmp2);


    list_add_tail(&tmp1, l1);
    list_add_tail(&tmp2, l2);

    struct list_head *t1 = (&tmp1)->next;
    struct list_head *t2 = (&tmp2)->next;

    element_t *element_l1;
    element_t *element_l2;

    while (t1 && t2) {
        if (t1->next == t1) {
            list_splice_tail(&tmp2, &new_queue);
            break;
        } else if (t2->next == t2) {
            list_splice_tail(&tmp1, &new_queue);
            break;
        } else {
            element_l1 = container_of(t1, element_t, list);
            element_l2 = container_of(t2, element_t, list);
            if (strcmp(element_l1->value, element_l2->value) <= 0) {
                cur = t1;
                t1 = t1->next;
                list_del_init(cur);
                list_add_tail(cur, &new_queue);

            } else {
                cur = t2;
                t2 = t2->next;
                list_del_init(cur);
                list_add_tail(cur, &new_queue);
            }
        }
    }

    l1 = new_queue.next;
    list_del(&new_queue);
    return l1;
}

struct list_head *mergeSortList(struct list_head *head)
{
    // merge sort
    if (!head || head->next == head)
        return head;

    // struct list_head *first = head;
    struct list_head *fast = head->next;
    struct list_head *slow = head;

    // split list
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // list_cut_position(&new, head, slow);

    // split two list
    fast = slow->next;

    fast->prev = head->prev;
    fast->prev->next = fast;

    slow->next = head;
    head->prev = slow;

    // sort each list
    struct list_head *l1 = mergeSortList(head);
    struct list_head *l2 = mergeSortList(fast);

    // merge sorted l1 and sorted l2
    return merge(l1, l2);
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || q_size(head) <= 1)
        return;

    // Bubble sort
    // q_bubble_sort(head);

    struct list_head *first = head->next;
    list_del_init(head);

    struct list_head *result = mergeSortList(first);
    list_add_tail(head, result);
    if (descend)
        q_reverse(head);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    struct list_head *cur = head->prev;
    struct list_head *tmp;
    char *max = container_of(head->prev, element_t, list)->value;
    int size;
    while (cur->prev != head) {
        if (strcmp(max, container_of(cur->prev, element_t, list)->value) < 0) {
            tmp = cur->prev;
            cur->prev = tmp->prev;
            // removemin tmp
            list_del_init(tmp);
            // printf("%s \n", container_of(tmp, element_t, list)->value);
            free(container_of(tmp, element_t, list)->value);
            free(container_of(tmp, element_t, list));
        } else {
            max = container_of(cur->prev, element_t, list)->value;
            cur = cur->prev;
        }
    }
    size = q_size(head);
    return size;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    struct list_head *cur = head->prev;
    struct list_head *tmp;
    char *max = container_of(head->prev, element_t, list)->value;
    int size;
    while (cur->prev != head) {
        if (strcmp(max, container_of(cur->prev, element_t, list)->value) > 0) {
            tmp = cur->prev;
            cur->prev = tmp->prev;
            // removemin tmp
            list_del_init(tmp);
            // printf("%s \n", container_of(tmp, element_t, list)->value);
            free(container_of(tmp, element_t, list)->value);
            free(container_of(tmp, element_t, list));
        } else {
            max = container_of(cur->prev, element_t, list)->value;
            cur = cur->prev;
        }
    }
    size = q_size(head);
    return size;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int __merge(struct list_head *l1, struct list_head *l2)
{
    if (!l1 || !l2)
        return 0;
    LIST_HEAD(tmp_head);
    while (!list_empty(l1) && !list_empty(l2)) {
        element_t *ele_1 = list_first_entry(l1, element_t, list);
        element_t *ele_2 = list_first_entry(l2, element_t, list);
        element_t *ele_min =
            strcmp(ele_1->value, ele_2->value) < 0 ? ele_1 : ele_2;
        list_move_tail(&ele_min->list, &tmp_head);
    }
    list_splice_tail_init(l1, &tmp_head);
    list_splice_tail_init(l2, &tmp_head);
    list_splice(&tmp_head, l1);
    return q_size(l1);
}
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head))
        return 0;
    else if (list_is_singular(head))
        return q_size(list_first_entry(head, queue_contex_t, chain)->q);
    int size = q_size(head);
    int count = (size % 2) ? size / 2 + 1 : size / 2;
    int queue_size = 0;
    for (int i = 0; i < count; i++) {
        queue_contex_t *first = list_first_entry(head, queue_contex_t, chain);
        queue_contex_t *second =
            list_entry(first->chain.next, queue_contex_t, chain);
        while (!list_empty(first->q) && !list_empty(second->q)) {
            queue_size = __merge(first->q, second->q);
            list_move_tail(&second->chain, head);
            first = list_entry(first->chain.next, queue_contex_t, chain);
            second = list_entry(first->chain.next, queue_contex_t, chain);
        }
    }
    return queue_size;
}
