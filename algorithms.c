#include "algorithms.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
		printf("Index out of bounds on array_remove\n");
		return;
	}
	// Move every element to the right starting at index meaning it will be deleted
	for(int i = index; i < (int)self->size; i++) {
		self->data[i] = self->data[i + 1];
	}
	self->size--;
}

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
struct array {
  int *data;
  size_t capacity;
  size_t size;
};
*/

void array_heap_sort(struct array *self) {
}

bool array_is_heap(const struct array *self) {
  return false;
}

void array_heap_add(struct array *self, int value) {
}

int array_heap_top(const struct array *self) {
  return 42;
}

void array_heap_remove_top(struct array *self) {
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
 * Tell if the list is empty
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
}

void list_push_back(struct list *self, int value) {
}

void list_pop_back(struct list *self) {
}


void list_insert(struct list *self, int value, size_t index) {
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
}

size_t list_search(const struct list *self, int value) {
  return -1;
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

void tree_create(struct tree *self) {
}


void tree_destroy(struct tree *self) {
}


bool tree_contains(const struct tree *self, int value) {
  return false;
}


bool tree_insert(struct tree *self, int value) {
  return false;
}


bool tree_remove(struct tree *self, int value) {
  return false;
}

bool tree_empty(const struct tree *self) {
  return true;
}


size_t tree_size(const struct tree *self) {
  return -1;
}


size_t tree_height(const struct tree *self) {
  return -1;
}


void tree_walk_pre_order(const struct tree *self, tree_func_t func, void *user_data)  {
}

void tree_walk_in_order(const struct tree *self, tree_func_t func, void *user_data) {
}

void tree_walk_post_order(const struct tree *self, tree_func_t func, void *user_data) {
}
