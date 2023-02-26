#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    if (list_empty(l)) {
        free(l);
        return;
    }

    struct list_head *tmp_node, *safe_node;
    list_for_each_safe (tmp_node, safe_node, l) {
        element_t *node = list_entry(tmp_node, element_t, list);
        free(node->value);
        free(node);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node)
        return false;

    new_node->value = strdup(s);
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
    if (!head)
        return false;

    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node)
        return false;

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

    // return pointer of first entry in list
    element_t *removed_node = list_first_entry(head, element_t, list);
    list_del(head->next);
    if (!sp)
        return NULL;
    strncpy(sp, removed_node->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    return removed_node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *removed_node = list_last_entry(head, element_t, list);
    list_del(head->prev);
    if (!sp)
        return NULL;

    strncpy(sp, removed_node->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    return removed_node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return -1;

    int len = 0;
    struct list_head *tmp;
    list_for_each (tmp, head)
        len++;

    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head *slow = head->next;
    struct list_head *fast = head->next;

    while (fast != head->prev && fast != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    element_t *node = list_entry(slow, element_t, list);
    list_del(slow);
    free(node->value);
    free(node);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *current = head->next;
    struct list_head *Next = current->next;

    while (current != head && Next != head) {
        element_t *current_node = list_entry(current, element_t, list);
        element_t *Next_node = list_entry(Next, element_t, list);

        __uint8_t isdup = false;
        while (strncmp(current_node->value, Next_node->value,
                       strlen(current_node->value)) == 0) {
            isdup = true;
            Next = Next->next;
            if (Next == head)
                break;
            Next_node = list_entry(Next, element_t, list);
        }
        if (isdup) {
            while (current != Next) {
                struct list_head *del = current;
                current = current->next;
                element_t *del_node = list_entry(del, element_t, list);
                list_del(del);
                free(del_node->value);
                free(del_node);
            }
        }
        current = current->next;
        Next = current->next;
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    for (struct list_head *current = head->next;
         current != head && current->next != head; current = current->next) {
        struct list_head *Next = current->next;
        list_del(current);
        list_add(current, Next);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *current, *safe;

    list_for_each_safe (current, safe, head) {
        list_move(current, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head))
        return;

    struct list_head *current, *safe, *temp_head = head;
    LIST_HEAD(head2);
    int i = 0;
    list_for_each_safe (current, safe, head) {
        i++;
        if (i == k) {
            list_cut_position(&head2, temp_head, current);
            q_reverse(&head2);
            list_splice(&head2, temp_head);
            temp_head = safe->prev;
            i = 0;
        }
    }
}

/* Merge two sorted list */
int Merge(struct list_head *l, struct list_head *r)
{
    if (!l || !r)
        return 0;

    LIST_HEAD(temp_head);

    while (!list_empty(l) && !list_empty(r)) {
        element_t *l_front = list_first_entry(l, element_t, list);
        element_t *r_front = list_first_entry(r, element_t, list);
        element_t *node =
            strcmp(l_front->value, r_front->value) < 0 ? l_front : r_front;
        list_move_tail(&node->list, &temp_head);
    }

    list_splice_tail_init(l, &temp_head);
    list_splice_tail_init(r, &temp_head);
    list_splice(&temp_head, l);
    return q_size(l);
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *slow = head, *fast = head->next;

    while (fast->next != head && fast != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    struct list_head left;
    list_cut_position(&left, head, slow);

    q_sort(&left);
    q_sort(head);
    Merge(head, &left);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    for (struct list_head *tail = head->prev; tail != head; tail = tail->prev) {
        while (tail->prev != head &&
               strcmp(list_entry(tail->prev, element_t, list)->value,
                      list_entry(tail, element_t, list)->value) < 0) {
            struct list_head *del = tail->prev;
            list_del_init(del);
            q_release_element(list_entry(del, element_t, list));
        }
    }

    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head))
        return 0;
    if (list_is_singular(head))
        return q_size(list_first_entry(head, queue_contex_t, chain)->q);

    int size = 0;

    queue_contex_t *first, *second;
    first = list_first_entry(head, queue_contex_t, chain);
    second = list_entry(first->chain.next, queue_contex_t, chain);

    while (!list_empty(second->q)) {
        // second queue will be merge to first queue
        size = Merge(first->q, second->q);
        list_move_tail(&second->chain, head);
        second = list_entry(first->chain.next, queue_contex_t, chain);
    }

    return size;
}