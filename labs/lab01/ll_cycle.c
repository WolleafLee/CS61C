#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    if (head == NULL || head->next == NULL) {
        return 0;
    }
    node* tortoise = head;
    node* hare = head;
    while (hare->next != NULL && hare->next->next != NULL) {
        tortoise = tortoise->next;
        hare = hare->next->next;
        if (tortoise == hare) {
            return 1;
        }
    }
    return 0;
}
