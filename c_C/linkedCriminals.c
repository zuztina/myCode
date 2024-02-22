/*The task is to implement a set of functions for working with linked lists. 
The linked list represents the list of criminals in the police register. The criminal is represented by name and references to other criminals he has contact with. 
With such a list we want to perform the following operations: add a criminal, add a contact, copy the list and delete the list.
*/
#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef struct TCriminal
{
    struct TCriminal *m_Next;
    char *m_Name;
    struct TCriminal **m_Contacts;
    size_t m_Cnt;
    size_t m_Capacity;
} TCRIMINAL;

#endif /* __PROGTEST__ */
#define SIZE_ARRAY 10
void printList(TCRIMINAL *list)
{
    TCRIMINAL *current = list;
    while (current != NULL)
    {
        printf("Name: %s\n", current->m_Name);
        printf("Contacts Capacity: %zu\n", current->m_Capacity);
        printf("Contacts Count: %zu\n", current->m_Cnt);
        current = current->m_Next;
    }
}
TCRIMINAL *createRecord(const char *name,
                        TCRIMINAL *next)
{
    TCRIMINAL *newRecord = (TCRIMINAL *)malloc(sizeof(TCRIMINAL));
    newRecord->m_Name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newRecord->m_Name, name);
    newRecord->m_Next = next;
    newRecord->m_Cnt = 0;
    newRecord->m_Capacity = 0;
    return newRecord;
}
void addContact(TCRIMINAL *dst,
                TCRIMINAL *contact)
{
    if (dst->m_Capacity == 0) // initial allocation of array
    {
        dst->m_Capacity = SIZE_ARRAY;
        dst->m_Contacts = (TCRIMINAL **)malloc(SIZE_ARRAY * sizeof(TCRIMINAL *));
    }
    if (dst->m_Cnt + 1 >= dst->m_Capacity) // ralloc when needed
    {
        dst->m_Capacity = dst->m_Capacity + dst->m_Capacity / 2;
        dst->m_Contacts = (TCRIMINAL **)realloc(dst->m_Contacts, dst->m_Capacity * sizeof(TCRIMINAL *));
    }
    dst->m_Contacts[dst->m_Cnt] = contact; // adding contact
    dst->m_Cnt++;
}
int findIndex(TCRIMINAL *src, TCRIMINAL *model) // finds position of criminal in the original list, used for finding the same ciriminal in coppied list
{
    int index = 0;
    while (src != NULL)
    {
        if (src == model)
        {
            return index;
        }
        index++;
        src = src->m_Next;
    }
    return -1;
}
TCRIMINAL *findMatch(TCRIMINAL *head, TCRIMINAL *src, TCRIMINAL *model) // returns matchinf criminal in coppied list, to be saved in array of contacts
{
    if (head == NULL || src == NULL)
    {
        return NULL;
    }
    int index = findIndex(src, model);
    if (index == -1)
    {
        return NULL; // match not found
    }
    int tmpIndex = 0;
    while (head != NULL)
    {
        if (tmpIndex == index) // found matching criminal
        {
            return head;
        }
        tmpIndex++;
        head = head->m_Next;
    }
    return NULL; // no match found
}
void freeList(TCRIMINAL *src)
{
    while (src != NULL)
    {
        TCRIMINAL *tmp = src;
        src = src->m_Next;
        free(tmp->m_Name);
        if (tmp->m_Capacity > 0)
        {
            free(tmp->m_Contacts);
        }
        free(tmp);
    }
    src = NULL;
}
TCRIMINAL *copyList(TCRIMINAL *src)
{
    if (src == NULL)
        return NULL;

    TCRIMINAL *newRecord = (TCRIMINAL *)malloc(sizeof(TCRIMINAL));                // new node
    newRecord->m_Name = (char *)malloc((strlen(src->m_Name) + 1) * sizeof(char)); // allocate memory for string + ending zero
    strcpy(newRecord->m_Name, src->m_Name);                                       // copy name
    newRecord->m_Contacts = NULL;                                                 // initilize array for contacts
    newRecord->m_Cnt = src->m_Cnt;                                                // copy count
    newRecord->m_Capacity = src->m_Capacity;                                      // copy capacity
    // Initialize m_Contacts array
    if (newRecord->m_Capacity > 0)
    {
        newRecord->m_Contacts = (TCRIMINAL **)malloc(newRecord->m_Capacity * sizeof(TCRIMINAL *)); // alocate array based on capacity of the oroginal record
    }
    newRecord->m_Next = copyList(src->m_Next);
    return newRecord;
}
TCRIMINAL *cloneList(TCRIMINAL *src)
{
    // nebyl predan platny seznam zlocincu
    if (src == NULL)
    {
        return NULL;
    }
    TCRIMINAL *tmpSrc = src; // Keep a reference to the original list
    TCRIMINAL *head = copyList(tmpSrc);
    TCRIMINAL *currentCopy = head; // curent record for which array of cintacts needs to be coppied
    TCRIMINAL *currentSrc = src;   // curent record from which array of cintacts needs to be coppied
    while (currentCopy != NULL)    // copy contacts
    {
        // printf("while\n");
        for (size_t i = 0; i < currentCopy->m_Cnt; i++) // position of coppied contact in array
        {
            // printf("for\n");
            TCRIMINAL *retMatch = findMatch(head, src, currentSrc->m_Contacts[i]);
            if (retMatch == NULL) // contact not found
            {
                freeList(head);
                return NULL;
            }
            currentCopy->m_Contacts[i] = retMatch;
        }
        // move to the next record
        currentCopy = currentCopy->m_Next;
        currentSrc = currentSrc->m_Next;
    }
    return head;
}

#ifndef __PROGTEST__
int main(int argc, char *argv[])
{
    TCRIMINAL *a, *b;
    char tmp[100];

    assert(sizeof(TCRIMINAL) == 3 * sizeof(void *) + 2 * sizeof(size_t));
    a = nullptr;
    a = createRecord("Peter", a);
    a = createRecord("John", a);
    a = createRecord("Joe", a);
    a = createRecord("Maria", a);
    addContact(a, a->m_Next);
    addContact(a->m_Next->m_Next, a->m_Next->m_Next->m_Next);
    addContact(a->m_Next->m_Next->m_Next, a->m_Next);
    assert(a && !strcmp(a->m_Name, "Maria") && a->m_Cnt == 1 && a->m_Contacts[0] == a->m_Next);
    assert(a->m_Next && !strcmp(a->m_Next->m_Name, "Joe") && a->m_Next->m_Cnt == 0);
    assert(a->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Name, "John") && a->m_Next->m_Next->m_Cnt == 1 && a->m_Next->m_Next->m_Contacts[0] == a->m_Next->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Name, "Peter") && a->m_Next->m_Next->m_Next->m_Cnt == 1 && a->m_Next->m_Next->m_Next->m_Contacts[0] == a->m_Next);
    assert(a->m_Next->m_Next->m_Next->m_Next == nullptr);
    b = cloneList(a);
    strcpy(tmp, "Moe");
    a = createRecord(tmp, a);
    strcpy(tmp, "Victoria");
    a = createRecord(tmp, a);
    strcpy(tmp, "Peter");
    a = createRecord(tmp, a);
    addContact(b->m_Next->m_Next->m_Next, b->m_Next->m_Next);
    assert(a && !strcmp(a->m_Name, "Peter") && a->m_Cnt == 0);
    assert(a->m_Next && !strcmp(a->m_Next->m_Name, "Victoria") && a->m_Next->m_Cnt == 0);
    assert(a->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Name, "Moe") && a->m_Next->m_Next->m_Cnt == 0);
    assert(a->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Name, "Maria") && a->m_Next->m_Next->m_Next->m_Cnt == 1 && a->m_Next->m_Next->m_Next->m_Contacts[0] == a->m_Next->m_Next->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Next->m_Name, "Joe") && a->m_Next->m_Next->m_Next->m_Next->m_Cnt == 0);
    assert(a->m_Next->m_Next->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "John") && a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Cnt == 1 && a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Contacts[0] == a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "Peter") && a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Cnt == 1 && a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Contacts[0] == a->m_Next->m_Next->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == nullptr);
    assert(b && !strcmp(b->m_Name, "Maria") && b->m_Cnt == 1 && b->m_Contacts[0] == b->m_Next);
    assert(b->m_Next && !strcmp(b->m_Next->m_Name, "Joe") && b->m_Next->m_Cnt == 0);
    assert(b->m_Next->m_Next && !strcmp(b->m_Next->m_Next->m_Name, "John") && b->m_Next->m_Next->m_Cnt == 1 && b->m_Next->m_Next->m_Contacts[0] == b->m_Next->m_Next->m_Next);
    assert(b->m_Next->m_Next->m_Next && !strcmp(b->m_Next->m_Next->m_Next->m_Name, "Peter") && b->m_Next->m_Next->m_Next->m_Cnt == 2 && b->m_Next->m_Next->m_Next->m_Contacts[0] == b->m_Next && b->m_Next->m_Next->m_Next->m_Contacts[1] == b->m_Next->m_Next);
    assert(b->m_Next->m_Next->m_Next->m_Next == nullptr);
    freeList(a);
    addContact(b->m_Next, b->m_Next);
    a = cloneList(b);
    assert(a && !strcmp(a->m_Name, "Maria") && a->m_Cnt == 1 && a->m_Contacts[0] == a->m_Next);
    assert(a->m_Next && !strcmp(a->m_Next->m_Name, "Joe") && a->m_Next->m_Cnt == 1 && a->m_Next->m_Contacts[0] == a->m_Next);
    assert(a->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Name, "John") && a->m_Next->m_Next->m_Cnt == 1 && a->m_Next->m_Next->m_Contacts[0] == a->m_Next->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next && !strcmp(a->m_Next->m_Next->m_Next->m_Name, "Peter") && a->m_Next->m_Next->m_Next->m_Cnt == 2 && a->m_Next->m_Next->m_Next->m_Contacts[0] == a->m_Next && a->m_Next->m_Next->m_Next->m_Contacts[1] == a->m_Next->m_Next);
    assert(a->m_Next->m_Next->m_Next->m_Next == nullptr);
    assert(b && !strcmp(b->m_Name, "Maria") && b->m_Cnt == 1 && b->m_Contacts[0] == b->m_Next);
    assert(b->m_Next && !strcmp(b->m_Next->m_Name, "Joe") && b->m_Next->m_Cnt == 1 && b->m_Next->m_Contacts[0] == b->m_Next);
    assert(b->m_Next->m_Next && !strcmp(b->m_Next->m_Next->m_Name, "John") && b->m_Next->m_Next->m_Cnt == 1 && b->m_Next->m_Next->m_Contacts[0] == b->m_Next->m_Next->m_Next);
    assert(b->m_Next->m_Next->m_Next && !strcmp(b->m_Next->m_Next->m_Next->m_Name, "Peter") && b->m_Next->m_Next->m_Next->m_Cnt == 2 && b->m_Next->m_Next->m_Next->m_Contacts[0] == b->m_Next && b->m_Next->m_Next->m_Next->m_Contacts[1] == b->m_Next->m_Next);
    assert(b->m_Next->m_Next->m_Next->m_Next == nullptr);
    freeList(b);
    freeList(a);
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */