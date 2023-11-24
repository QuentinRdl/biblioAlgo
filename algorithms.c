#include "algorithms.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define debug false
/*
 * Create an empty array
 */
void array_create(struct array *self) {
	self->data = (int *) malloc(20 * sizeof(int));
	// We check for errors with malloc
	if(self->data == NULL) {
		printf("Error with memory allocation !");
		return;
	}

	self->capacity = 20;
	self->size = 0;
}

/*
 * Create an array with initial content
 */
void array_create_from(struct array *self, const int *other, size_t size) {
	array_create(self); // We create an empty array
	// We check if there has been an error with the previous function
	if(self->data == NULL) {
		printf("Error with memory allocation on array_create_from !");
		return;
	}
	// If there is not enough space in the newly created array we need to realloc more space
	if(size > self->capacity) {
		int *newData = realloc(self->data, size * sizeof(int));
		self->data = newData;
		self->capacity = size;	
	}

	self->size = size;
	// We put the data inside the new array
	for(int i = 0; i < (int)size; i++) {
		self->data[i] = other[i];
	}
}

/*
 * Destroy an array
 */
void array_destroy(struct array *self) {
	if(self->data != NULL)
		free(self->data);
	// Set the values to 0 (not necessary)
	self->capacity = 0;
	self->size = 0;
}

/*
 * Tell if the array is empty
 */
bool array_empty(const struct array *self) {
	if(self->data == NULL || self->size == 0)
		return true;
	return false;
}

/*
 * Get the size of the array
 */
size_t array_size(const struct array *self) {
	return self->size;
}

bool array_equals(const struct array *self, const int *content, size_t size) {
	// We look if the array and content don't have a matching size
	if(self->size != size)
		return false;
	for(int i = 0; i < (int)size; i++) {
		if(self->data[i] != content[i])	
			return false;
	}
  return true;
}

/*
 * Add an element at the end of the array
 */
void array_push_back(struct array *self, int value) {
    // We treat the case where the array has NOT enough capacity
	if (self->size>= self->capacity) {
		int *newData = (int *) realloc(self->data, (self->capacity + 1) * sizeof(int));
		if(newData == NULL) {
			printf("Problem with memory allocation in array_push_back\n");
			return;
		}
		self->capacity++;
		self->data = newData;
	}

    self->data[self->size] = value;
    self->size++;
}

/*
 * Remove the element at the end of the array
 */
void array_pop_back(struct array *self) {
	// In the case there is nothing to pop
	if(self->size <= 0)
		return;
	// We simply reallocate with capacity - 1 which will scrape the last element of the array
	int *newData = (int *) realloc(self->data, (self->capacity - 1) * sizeof(int));
	if(newData == NULL) {
		printf("Problem with memory allocation in array_pop_back\n");
		return;
	}
	self->capacity--;
	self->size--;
	self->data = newData;
}


/*
 * Insert an element in the array (preserving the order)
 */
void array_insert(struct array *self, int value, size_t index) {
	// We realloc if there is not enough space
	if(self->capacity > self->size + 1) {
		int *newData = (int *) realloc(self->data, (self->capacity + 1) * sizeof(int));
		if(newData == NULL) {
			printf("Problem with memory allocation is array_insert\n");
			return;
		}
		
		self->data = newData;
		self->capacity++;
	}
	self->size++; // Because we are going to add an element
	// We shift each element that are after our index (included) to the right
	for(int i = (int)self->size;i > (int)index; i--) {
		self->data[i] = self->data[i - 1];
	}
	self->data[index] = value;
}


/*
 * Remove an element in the array (preserving the order)
 */
void array_remove(struct array *self, size_t index) {
	// Looking for out of bounds
	if (index >= self->size) {
		if(debug) printf("Index out of bounds on array_remove\n");
		return;
	}
	// Move every element to the right starting at index meaning it will be deleted
	for(int i = index; i < (int)self->size; i++) {
		self->data[i] = self->data[i + 1];
	}
	self->size--;
}

/*
 * Get an element at the specified index in the array, or 0 if the index is not valid
 * */
int array_get(const struct array *self, size_t index) {
    if (index >= self->size) {
        // Looking for out of bounds
        printf("Index out of bounds on array_get\n");
		return 0;
    }
    return self->data[index];
}

/*
 * Set an element at the specified index in the array to a new value, or do nothing if the index is not valid
 */
void array_set(struct array *self, size_t index, int value) {
	if(index > self->size) {
		// Looking for out of bounds
        printf("Index out of bounds on array_get\n");
		return;
	}
	self->data[index] = value;
}

/*
 * Search for an element in the array.
 */
size_t array_search(const struct array *self, int value) {
	for(int i = 0; i < (int)self->size; i++) {
		if (self->data[i] == value)
			return i;
	}
  	return self->size; // No match found
}

/*
 * Search for an element in the sorted array.
 */
size_t array_search_sorted(const struct array *self, int value) {
	// Implementation of binary search
	int left = 0;
	int right = self->size;
	int mid;
	while(left <= right) {
		mid = (left + right) / 2; 
		if(self->data[mid] == value)
			return (size_t)mid; // Match found
		else if(self->data[mid] > value) 
			right = mid - 1;
		else
			left = mid + 1;
	}
	return self->size; // No match found
}

/*
 * Tell if the array is sorted
 */
bool array_is_sorted(const struct array *self) {
	for(int i = 0; i < (int)self->size - 1; i++) {
		if(self->data[i] > self->data[i + 1]) 
		return false;
	}
	return true;
}

/*
 * Make a partition of the array between i and j (inclusive) and returns the index of the pivot
 */
ptrdiff_t array_partition(struct array *self, ptrdiff_t i, ptrdiff_t j) {
	// We choose the pivot (the first value stored in the array)
	ptrdiff_t pivotIndex = i;
	ptrdiff_t left = i;
	ptrdiff_t right = j;
	int temp;
	while(left <= right) {
		while(left <= right && self->data[left] <= self->data[pivotIndex]) {
			left++;
		}
		while(left <= right && self->data[right] >= self->data[pivotIndex]) {
			right--;
		}
		if(left <= right) {
			// We need to swap the values of left and right
			temp = self->data[left];
			self->data[left] = self->data[right];
			self->data[right] = temp;
		}
	}
	// Swap the value that is stored in right with the position of the pivot
	temp = self->data[pivotIndex];
	self->data[pivotIndex] = self->data[right];
	self->data[right] = temp;
	return right;
}

void quick_sort_reccu(struct array *self, ptrdiff_t low, ptrdiff_t high) {
	if(low < high) {
		ptrdiff_t pivotIndex = array_partition(self, low, high); // Paritions the array and get the index of the pivot
		quick_sort_reccu(self, low, pivotIndex - 1);
		quick_sort_reccu(self, pivotIndex + 1, high);
	}
}

/*
 * Sort the array with quick sort
 */
void array_quick_sort(struct array *self) {
	if(self->size <= 1)
		return; // Nothing to sort
	quick_sort_reccu(self, 0, self->size - 1);
	
}

/*
 * Sort the array with heap sort
 * */
void array_heap_sort(struct array *self) {
}

/*
 * Tell if the array is a heap
 */
bool array_is_heap(const struct array *self) {
	if(self == NULL || self->size < 1) return true;
	size_t n = self->size;
	for(size_t i = 0; i < n - 1; i++) {
		int lt = 2 * i + 1;
		int rt = 2 * i + 2;
		if(lt < n && self->data[lt] > self->data[i]) return false;
		if(rt < n && self->data[rt] > self->data[i]) return false;
	}
	return true;
}

/*
 * Add a value into the array considered as a heap
 */
void array_heap_add(struct array *self, int value) {
	size_t i = self->size; // Get the size of the heap
	self->data[i] = value;
	while(i > 0) {
		size_t j = (i - 1) / 2; // Find location of parent
		if(self->data[i] <= self->data[j]) break;
		int tmp = self->data[i];
		self->data[i] = self->data[j];
		self->data[j] = tmp; 
		i = j;
	}
}

/*
 * Get the value at the top of the array
 */
int array_heap_top(const struct array *self) {
	if(self == NULL || self->size < 1) return 42;
	return self->data[0];
}

/*
 * Remove the top value in the array considered as a heap
 */
void array_heap_remove_top(struct array *self) {
	size_t n = self->size;
	if(n <= 1) return; // Nothing to remove

	self->data[0] = self->data[n - 1];
	self->size--;
	size_t i = 0;
	while(i < (n / 2)) {
		size_t lt = 2 * i + 1;
		size_t rt = 2 * i + 2;
		size_t j = i;
		if (lt < self->size && self->data[lt] > self->data[j]) j = lt;
		if (rt < self->size && self->data[rt] > self->data[j]) j = rt;
		if (j != i) {
			int tmp = self->data[i];
			self->data[i] = self->data[j];
			self->data[j] = tmp;
			i = j;
		} else {
			break;
		}
	}
}

/*
 * list
 */

/*
 * Create an empty list
 */
void list_create(struct list *self) {
	self->first = NULL;
	self->last = NULL;
}


/*
 * Create a list with initial content
 */
void list_create_from(struct list *self, const int *other, size_t size) {
	if(self == NULL || other == NULL || size == 0)
		return;
	self->first = NULL;
	self->last = NULL;
	for(size_t i = 0; i < size; i++) {

		// We allocate a new node, put the data in it 
		struct list_node *newNode = malloc(sizeof(struct list_node));
		if (newNode == NULL) {
			printf("Allocation error");
			return;
		}
		newNode->data = other[i];
		newNode->prev = self->last;
		newNode->next = NULL;

		if(self->last != NULL) {
			self->last->next = newNode; // Set the last pointer to our new node
		} else {
			self->first = newNode;
		}
		self->last = newNode;
	}
}

/*
 * Destroy a list
 */
void list_destroy(struct list *self) {
	if(self == NULL) return;
	struct list_node *curr = self->first;
	struct list_node *tmp;
	while(curr != NULL) {
		tmp = curr; 
		curr = curr->next;
		free(tmp);
	}
	
	// Put the values of start and end ptr to NULL
	self->first = NULL;
	self->last = NULL;
}

/*
 * Tell if the list is empty
 */
bool list_empty(const struct list *self) {
	return self->first == NULL;
}

size_t list_size(const struct list *self) {
	size_t count = 0;
	struct list_node *curr = self->first;
	while(curr != NULL) {
		count++;
		curr = curr->next;
	}
	return count;
	//return -1;

}

/*
struct list_node {
  int data;
  struct list_node *next;
  struct list_node *prev;
};

struct list {
  struct list_node *first;
  struct list_node *last;
};
*/

/*
 * Compares the list to an array (data and size) 
 */
bool list_equals(const struct list *self, const int *data, size_t size) {
	size_t count = 0;
	struct list_node *curr = self->first;
	while(curr != NULL && count < size) {
		if(*data != curr->data)
			return false;
		count++;
		curr = curr->next;
		data++;
	}
	if(curr != NULL || count < size)
		return false; // One is snaller than the other
	return true;
}

void list_push_front(struct list *self, int value) {
	struct list_node *new = malloc(sizeof(struct list_node));
	new->data = value;
	new->prev = NULL;
	if(list_empty(self)) {
		new->next = NULL;
		self->first = new;
		self->last = new;
	} else {
		new->next = self->first;
		self->first->prev = new;
		self->first = new;
	}
	return;
}

void list_pop_front(struct list *self) {
	if(self == NULL || self->first == NULL)
		return; // Nothing to pop
	else if(self->first->next == NULL) {
		self->first = NULL;
		self->last = NULL;
		free(self->first);
		return;
	}
	struct list_node *curr = self->first;
	self->first = self->first->next;
	free(curr);
}

/*
 * Add an element in the list at the end
 */
void list_push_back(struct list *self, int value) {
	struct list_node *new = malloc(sizeof(struct list_node));
	new->data = value;
	new->next = NULL;
	if(list_empty(self)) {
		self->first = new;
		self->last = new;
		new->prev = NULL;
	} else {
		new->prev = self->last;
		self->last->next = new;
		self->last = new;
	}
}


void list_pop_back(struct list *self) {
	if(list_empty(self))
		return;
 
	struct list_node *curr = self->last;
	self->last = self->last->prev;
	if(self->last != NULL) {
		self->last->next = NULL;
	} else {
		self->first = NULL; // List is empty
	}
	free(curr);
}

void list_insert(struct list *self, int value, size_t index) {
	if(self == NULL)
		return;
	size_t i = 0;
	struct list_node *curr = self->first;
	//struct list_node *prev = NULL;
	while(curr->next != NULL && i < index) {
	//	prev = curr;
		curr = curr->next;
	}
	if(i >= index) {
		return; // Out of bounds
	}
	struct list_node *new = malloc(sizeof(struct list_node));
	new->data = value;
	new->prev = curr;
	new->next = curr->next;
	curr->next = new;
}

/*
 * Remove an element in the list (preserving the order)
 * index is valid
 */
void list_remove(struct list *self, size_t index) {
	if(list_empty(self))
		return;
	if(index == 0) {
		struct list_node *curr = self->first;
		if(self->first->next == NULL) {
			self->first = NULL;
			free(self->first);
			return;
		}
		self->first = self->first->next;
		free(curr);
		return;
	}
	struct list_node *curr = self->first;
	struct list_node *prev = NULL;
	size_t i = 0;
	while(i < index && curr != NULL) {
		prev = curr;
		curr = curr->next;
		i++;
	}
	if(curr == NULL)
		return; // Index is not correct
	prev->next = curr->next;	
	if(curr->next != NULL)
		curr->next->prev = prev;
	free(curr);
}

int list_get(const struct list *self, size_t index) {
	struct list_node *curr = self->first;
	size_t i = 0;
	while(i < index && curr != NULL) {
		curr = curr->next;
		i++;
	}
	if(curr == NULL) {
		return 0; // i was out of bounds
	}
	return curr->data ;
}

void list_set(struct list *self, size_t index, int value) {
	if(index < 0)
		return;
	struct list_node *curr = self->first;
	size_t i = 0;
	while(i < index && curr != NULL) {
		curr = curr->next;
		i++;
	}
	if(curr != NULL) 
		curr->data = value;
}

/*
 * Search for an element in the list and return its index or the size of the list if not present.
 */
size_t list_search(const struct list *self, int value) {
	size_t i = 0;
	struct list_node *curr = self->first;
	while(curr != NULL) {
		if(curr->data == value)
			return i;
		curr = curr->next;	
		i++;
	}
	return i;
}

bool list_is_sorted(const struct list *self) {
	return false;
}

void list_split(struct list *self, struct list *out1, struct list *out2) {
}

void list_merge(struct list *self, struct list *in1, struct list *in2) {
}

void list_merge_sort(struct list *self) {
}


/*
 * tree
 */
/*
struct tree_node {
  int data;
  struct tree_node *left;
  struct tree_node *right;
};

struct tree {
  struct tree_node *root;
};
*/
void tree_node_destroy(struct tree_node *node);

/*
 * Create an empty tree
 */
void tree_create(struct tree *self) {
	self->root = NULL;
}

/*
 * Destroys a tree
 */
void tree_destroy(struct tree *self) {
	if(self == NULL) return;
	tree_node_destroy(self->root);
	self = NULL;
}

void tree_node_destroy(struct tree_node *node) {
	if(node == NULL) return;
	if(node->right != NULL) tree_node_destroy(node->right);
	if(node->left != NULL) tree_node_destroy(node->left);

	free(node);
}

bool tree_contains_reccu(struct tree_node *node, int value) {
	if(node == NULL) return false;
	if(node->data == value) return true;

	if(value < node->data) return(tree_contains_reccu(node->left, value));
	if(value > node->data) return(tree_contains_reccu(node->right, value));
	return false;
}
/*
 * Tell if a value is in the tree
 */
bool tree_contains(const struct tree *self, int value) {
	if(self == NULL) return false;
	return tree_contains_reccu(self->root, value);
}

struct tree_node* create_node(int value) {
	struct tree_node *new_node = (struct tree_node*)malloc(sizeof(struct tree_node));
	if (new_node != NULL) {
		new_node->data = value;
		new_node->left = NULL;
		new_node->right = NULL;
	}
	return new_node;
}

bool tree_insert_reccu(struct tree_node **node, int value) {
	if (*node == NULL) {
		*node = create_node(value);
		return true; // Value insertedy
	}

	if (value < (*node)->data) {
		return tree_insert_reccu(&((*node)->left), value);
	} else if (value > (*node)->data) {
		return tree_insert_reccu(&((*node)->right), value);
	} else {
		return false; // Value present
	}
}

/*
 * Insert a value in the tree and return false if the value was already present
 */
bool tree_insert(struct tree *self, int value) {
	return tree_insert_reccu(&(self->root), value);
}

/*
 * Remove a value from the tree and return false if the value was not present
 */
bool tree_remove(struct tree *self, int value) {
  return false;
}

/*
 * Tell if the tree is empty
 */
bool tree_empty(const struct tree *self) {
	return(self->root == NULL);
}

/*
 * Get the size of the tree
 */
size_t node_size(const struct tree_node *self) {
	if(self == NULL) return 0; 
	return 1 + node_size(self->left) + node_size(self->right);
}

size_t tree_size(const struct tree *self) {
	if(self == NULL) return 0;
	return node_size(self->root);
}

size_t node_height(const struct tree_node *node) {
	if(node == NULL) return 0;

	size_t sizeLeft = node_height(node->left);
	size_t sizeRight = node_height(node->right);
	
	if(sizeLeft > sizeRight) return sizeLeft + 1;
	return sizeRight + 1;
}
/*
 * Get the height of the tree
 */
size_t tree_height(const struct tree *self) {
	if(self == NULL) return NULL;
	return node_height(self->root);
}


void tree_walk_pre_order(const struct tree *self, tree_func_t func, void *user_data)  {
}

void tree_walk_in_order(const struct tree *self, tree_func_t func, void *user_data) {
}

void tree_walk_post_order(const struct tree *self, tree_func_t func, void *user_data) {
}
