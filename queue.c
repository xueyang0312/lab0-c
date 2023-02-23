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
    new_node->value = malloc(strlen(s) + 1);
    if (!new_node->value) {
        free(new_node);
        return false;
    }

    memcpy(new_node->value, s, strlen(s));
    new_node->value[strlen(s)] = '\0';

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

    new_node->value = malloc(strlen(s) + 1);
    if (!new_node->value) {
        free(new_node);
        return false;
    }

    memcpy(new_node->value, s, strlen(s));
    new_node->value[strlen(s)] = '\0';

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
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *current = head->next;
    struct list_head *last = head->prev;

    while (current != last) {
        element_t *current_node = list_entry(current, element_t, list);
        element_t *last_node = list_entry(last, element_t, list);

        char *temp = current_node->value;
        current_node->value = last_node->value;
        last_node->value = temp;

        if (current->next == last)
            break;

        current = current->next;
        last = last->prev;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
