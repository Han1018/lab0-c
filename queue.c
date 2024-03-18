#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_sort.h"
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
    if (head == NULL)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    struct list_head *list_iter, *_safe;
    if (!l)
        return;
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
    if (!head)
        return false;
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
    if (!head)
        return false;
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
    if (!head || list_empty(head))
        return NULL;

    element_t *entry = list_entry(head->next, element_t, list);
    if (entry->value != NULL) {
        strncpy(sp, entry->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    list_del(head->next);
    return entry;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
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
    if (!head || list_empty(head))
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
    if (!head || list_empty(head) || list_is_singular(head))
        return true;

    // Step 1: Sort the list in ascending order
    q_sort(head, false);

    // Step 2: Delete duplicate string nodes
    struct list_head *li, *tmp;
    bool last_dup = false;

    list_for_each_safe (li, tmp, head) {
        element_t *entry = list_entry(li, element_t, list);
        element_t *next_entry = list_entry(tmp, element_t, list);

        if (tmp != head && !strcmp(entry->value, next_entry->value)) {
            last_dup = true;
            list_del(li);
            free(entry->value);
            free(entry);
        } else if (last_dup) {
            last_dup = false;
            list_del(li);
            free(entry->value);
            free(entry);
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_is_singular(head) || list_empty(head))
        return;

    struct list_head *li, *tmp;

    for (li = head->next, tmp = li->next; li != head && tmp != head;
         li = li->next, tmp = li->next) {
        list_move(li, tmp);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *end = head->prev;
    struct list_head *current = head->next;
    while (current != end) {
        struct list_head *next = current->next;
        list_move(current, head);
        current = next;
    }
    list_move(current, head);  // add last node
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head) || list_is_singular(head) || k == 1)
        return;
    struct list_head *node, *safe, target, *last_k_head = head;
    int count = 0;
    INIT_LIST_HEAD(&target);

    list_for_each_safe (node, safe, head) {
        count++;
        if (count == k) {
            list_cut_position(&target, last_k_head, node);  // cut k nodes
            q_reverse(&target);                             // reverse k nodes
            list_splice_init(&target, last_k_head);
            count = 0;
            last_k_head = safe->prev;
        }
    }
}


void merge(struct list_head *l_head,
           struct list_head *r_head,
           struct list_head *head)
{
    while (!list_empty(l_head) && !list_empty(r_head)) {
        element_t *l_entry = list_entry(l_head->next, element_t, list);
        element_t *r_entry = list_entry(r_head->next, element_t, list);

        if (strcmp(l_entry->value, r_entry->value) <= 0)
            list_move_tail(l_head->next, head);
        else
            list_move_tail(r_head->next, head);
    }
    if (!list_empty(l_head))
        list_splice_tail_init(l_head, head);
    else
        list_splice_tail_init(r_head, head);
}

void merge_sort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    // Find middle node
    struct list_head *slow = head, *fast = head;
    do {
        fast = fast->next->next;
        slow = slow->next;
    } while (fast != head && fast->next != head);

    LIST_HEAD(l_head);
    LIST_HEAD(r_head);

    // Split list into two parts - Left and Right
    list_splice_tail_init(head, &r_head);
    list_cut_position(&l_head, &r_head, slow);
    // Recursively split the left and right parts
    merge_sort(&l_head);
    merge_sort(&r_head);
    // Merge the left and right parts
    merge(&l_head, &r_head, head);
}

/* Compare two nodes in queue */
int cmp(const struct list_head *a, const struct list_head *b, bool descend)
{
    element_t *a_entry = list_entry(a, element_t, list);
    element_t *b_entry = list_entry(b, element_t, list);

    if (!descend)
        return strcmp(a_entry->value, b_entry->value) < 0 ? 0 : 1;
    else
        return strcmp(a_entry->value, b_entry->value) > 0 ? 0 : 1;
}

void q_sort2(struct list_head *head, bool descend)
{
    list_sort(head, cmp, descend);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    merge_sort(head);

    if (descend)
        q_reverse(head);
}


/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    struct list_head *last_biggest, *tmp, *li;
    if (!head || list_empty(head))
        return 0;
    if (list_is_singular(head))
        return 1;

    element_t *last_biggest_entry = list_entry(head->next, element_t, list);
    last_biggest = head->next;

    list_for_each_safe (li, tmp, head) {
        element_t *entry = list_entry(tmp, element_t, list);
        if (li == head->prev) {
            continue;
        }
        if (strcmp(entry->value, last_biggest_entry->value) < 0) {
            while (last_biggest != tmp) {
                struct list_head *next = last_biggest->next;
                last_biggest_entry = list_entry(last_biggest, element_t, list);

                list_del(last_biggest);
                free(last_biggest_entry->value);
                free(last_biggest_entry);

                last_biggest = next;
            }
            last_biggest_entry = list_entry(last_biggest, element_t, list);
        }
    }

    // calculate size
    int size = 0;
    list_for_each_safe (li, tmp, head) {
        size++;
    }
    return size;
}

/* Remove every node which has a node with a strictly greater value anywhere
 * to the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    struct list_head *last_biggest, *tmp, *li;
    if (!head || list_empty(head))
        return 0;
    if (list_is_singular(head))
        return 1;

    element_t *last_biggest_entry = list_entry(head->next, element_t, list);
    last_biggest = head->next;

    list_for_each_safe (li, tmp, head) {
        element_t *entry = list_entry(tmp, element_t, list);
        if (li == head->prev) {
            continue;
        }
        if (strcmp(entry->value, last_biggest_entry->value) > 0) {
            while (last_biggest != tmp) {
                struct list_head *next = last_biggest->next;
                last_biggest_entry = list_entry(last_biggest, element_t, list);

                list_del(last_biggest);
                free(last_biggest_entry->value);
                free(last_biggest_entry);

                last_biggest = next;
            }
            last_biggest_entry = list_entry(last_biggest, element_t, list);
        }
    }

    // calculate size
    int size = 0;
    list_for_each_safe (li, tmp, head) {
        size++;
    }
    return size;
}

/* Merge all the queues into one sorted queue, which is in
 * ascending/descending order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    // insert all queue into one queue
    if (!head || list_empty(head))
        return 0;
    queue_contex_t *output = list_entry(head->next, queue_contex_t, chain);
    queue_contex_t *current = NULL;
    list_for_each_entry (current, head, chain) {
        if (current == output)
            continue;
        list_splice_init(current->q, output->q);
        output->size = output->size + current->size;
        current->size = 0;
    }

    // sort
    q_sort(output->q, descend);

    // calculate size
    int size = 0;
    struct list_head *li, *tmp;
    list_for_each_safe (li, tmp, output->q) {
        size++;
    }
    return size;
}
