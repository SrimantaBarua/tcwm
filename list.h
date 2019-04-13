// list.h - Interface for implementing linked lists
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>
//
// These linked lists are embedded into the structure, for better cache locality.


#ifndef __TCWM_LIST_H__
#define __TCWM_LIST_H__


#include <stdbool.h>
#include <stddef.h>


// List node to be embedded into structures
struct list {
	struct list *next, *prev;
};


// Static initializer for list node
#define LIST_INIT(node) { &(node), &(node) }


// Initialize list node
static inline void list_init(struct list *node) {
	node->next = node->prev = node;
}


// Insert node at the beginning of a list
// Params: list = pointer to list head
//         node = pointer to node to be inserted
static inline void list_add_head(struct list *list, struct list *node) {
	node->prev = list;
	node->next = list->next;
	list->next->prev = node;
	list->next = node;

}


// Insert node at the tail of a list
// Params: list = pointer to list head
//         node = pointer to node to be inserted
static inline void list_add_tail(struct list *list, struct list *node) {
	node->prev = list->prev;
	node->next = list;
	list->prev->next = node;
	list->prev = node;
}


// Remove node from list, and re-initialize it
// Params: node = pointer to node to be removed
static inline void list_del(struct list *node) {
	node->next->prev = node->prev;
	node->prev->next = node->next;
	node->next = node->prev = node;
}


// Is list empty?
// Params: list = pointer to list head
static inline bool list_is_empty(const struct list *list) {
	return list->next == list;
}


// Get first list node
static inline struct list* list_first(struct list *list) {
	return list->next;
}


// Get last list node
static inline struct list* list_last(struct list *list) {
	return list->prev;
}


// Get pointer to struct from pointer to list node. Assumes list is not empty
// Params: node = pointer to list node
//         type = type of struct containing node
//         member = name of list node in struct
#define LIST_DATA(node, type, member) \
	(((type)*) (((void*) (node)) - offsetof(type, member)))


// Get pointer to struct from pointer to list node (or NULL if list is empty)
// Params: node = pointer to list node
//         type = type of struct containing node
//         member = name of list node in struct
#define LIST_DATA_OR_NULL(node, type, member) \
	((node)->next == (node) ? NULL : LIST_DATA(node, type, member))


// Get data of next node
// Params: node = pointer to this node
//         type = type of struct containing node
//         member = name of list node in struct
#define LIST_NEXT_DATA(node, type, member) LIST_DATA((node)->next, type, member)


// Get data of previous node
// Params: node = pointer to this node
//         type = type of struct containing node
//         member = name of list node in struct
#define LIST_PREV_DATA(node, type, member) LIST_DATA((node)->prev, type, member)


// Get first list element
#define LIST_FIRST_DATA(list, type, member) LIST_DATA(list_first(list), type, member)


// Get last list element
#define LIST_LAST_DATA(list, type, member) LIST_DATA(list_last(list), type, member)


// Iterate over list nodes
// Params: list = pointer to list head
//         iter = pointer to list node used as iterator
#define LIST_FOREACH(list, iter) \
	for ((iter) = (list)->next; (iter) != (list); (iter) = (iter)->next)


// Reverse iterate over list nodes
// Params: list = pointer to list head
//         iter = pointer to list node used as iterator
#define LIST_FOREACH_REV(list, iter) \
	for ((iter) = (list)->prev; (iter) != (list); (iter) = (iter)->prev)


// Iterate over list items
// Params: list = pointer to list head
//         iter = pointer to structure containing list
//         member = name of list node in struct
#define LIST_FOREACH_DATA(list, iter, member) \
	for ((iter) = LIST_DATA_OR_NULL((list)->next, typeof(*iter), member); \
		(iter)->(member) != (list); \
		(iter) = LIST_DATA_OR_NULL((iter)->(member).next, typeof(*iter), member))


// Iterate over list items in reverse
// Params: list = pointer to list head
//         iter = pointer to structure containing list
//         member = name of list node in struct
#define LIST_FOREACH_DATA_REV(list, iter, member) \
	for ((iter) = LIST_DATA_OR_NULL((list)->prev, typeof(*iter), member); \
		(iter)->(member) != (list); \
		(iter) = LIST_DATA_OR_NULL((iter)->(member).prev, typeof(*iter), member))



#endif // __TCWM_LIST_H__
