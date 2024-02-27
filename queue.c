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
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (head == NULL)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    struct list_head *list_iter, *_safe;

    list_for_each_safe (list_iter, _safe, l) {
        element_t *entry = list_entry(list_iter, element_t, list);
        list_del(list_iter);
        free(entry->value);
        free(entry);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node)
        return false;

    // 複製字串
    new_node->value = malloc(strlen(s) + 1);  // +1為'\0'
    if (new_node->value == NULL) {
        free(new_node);
        return false;
    }
    strncpy(new_node->value, s, strlen(s) + 1);

    list_add(&new_node->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node)
        return false;

    // 複製字串
    new_node->value = malloc(strlen(s) + 1);  // +1為'\0'
    if (new_node->value == NULL) {
        free(new_node);
        return false;
    }
    strncpy(new_node->value, s, strlen(s) + 1);

    list_add_tail(&new_node->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (list_empty(head))
        return NULL;

    element_t *entry = list_entry(head->next, element_t, list);
    if (entry->value != NULL) {
        strncpy(sp, entry->value, bufsize);
        free(entry->value);
        entry->value = NULL;
    }
    list_del(head->next);
    return entry;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (list_empty(head))
        return NULL;

    element_t *entry = list_entry(head->prev, element_t, list);
    if (entry->value != NULL) {
        strncpy(sp, entry->value, bufsize);
        free(entry->value);
        entry->value = NULL;
    }
    list_del(head->prev);
    return entry;
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
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (list_empty(head))
        return false;
    struct list_head *slow, *fast;
    slow = fast = head;

    // 快慢指標, fast 走兩步, slow 走一步, fast 到達終點時 slow 剛好是一半
    while (fast->next != head->prev && fast->next->next != head->prev) {
        slow = slow->next;
        fast = fast->next->next;
    }
    slow = slow->next;

    element_t *middle_entry = list_entry(slow, element_t, list);
    list_del(slow);
    free(middle_entry->value);
    free(middle_entry);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (list_is_singular(head) || list_empty(head))
        return true;

    // Step 1: Sort the list in ascending order
    q_sort(head, false);

    // Step 2: Delete duplicate string nodes
    struct list_head *li, *tmp;

    list_for_each_safe (li, tmp, head->next) {
        element_t *entry = list_entry(li, element_t, list);
        element_t *next_entry = list_entry(tmp, element_t, list);

        // Check if the current and next entries have the same value
        if (strcmp(entry->value, next_entry->value) == 0) {
            // Remove the duplicate node
            list_del(tmp);
            free(next_entry->value);
            free(next_entry);
        }
    }

    return true;
}

/* Swap two nodes*/
void swap(element_t *a, element_t *b)
{
    element_t t = *a;
    *a = *b;
    *b = t;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

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
