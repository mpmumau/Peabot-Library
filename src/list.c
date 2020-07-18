/*
 File:          list.c
 Description:   Implementation of linked list object functionality.
 */

#ifndef LIST_DEF
#define LIST_DEF

// System dependencies
#include <stdlib.h>
#include <stdio.h>

// Header
#include "list.h"

void list_push(List **head, void *data)
{
    List *node = malloc(sizeof(List));
    if (node == NULL) {
        return;
    }

    node->data = data;
    node->next = NULL; 

    if (*head == NULL) {    
        *head = node;
        return;
    }

    while ((*head)->next != NULL)
    {
        head = &(*head)->next;
    }

    (*head)->next = node;
}

void *list_pop(List **head)
{
    void *data = NULL;

    if (*head == NULL)
    {
        return data;
    }

    List *next = NULL;
    next = (*head)->next;

    data = (*head)->data;

    free(*head);
    *head = next;

    return data;
}

void list_insert(List **head, void *data)
{
    List *node = malloc(sizeof(List));
    node->data = data;
    node->next = *head;

    *head = node;
}

unsigned int list_sizeof(List *head)
{
    if (head == NULL)
    {
        return 0;
    }    

    unsigned int size = 1;

    while (head->next != NULL)
    {
        size++;
        head = head->next;
    }

    return size;
}

List *list_get(List *head, unsigned int index)
{
    for (unsigned int i = 0; i < index; i++) {
        head = head->next;
    }

    return head;
}

void list_remove(List **head, unsigned int index)
{
    if (!head || !*head || !index || index > list_sizeof(*head))
        return;    

    List **precursor;

    unsigned int i;
    for (i = 1; i < index; i++)
    {
        if ((*head)->next == NULL)
            break;
        precursor = head;
        head = &(*head)->next;
    }

    (*precursor)->next = (*head)->next;

    free(*head);
}

void *list_last(List *head)
{
    int count = list_sizeof(head);
    if (!count)
        return (void *) NULL;

    List *last = list_get(head, count - 1);
    return (void *) last->data;
}

#endif