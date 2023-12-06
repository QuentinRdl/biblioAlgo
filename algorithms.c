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
	if(self->data != NULL){
		free(self->data);
		self->data = NULL;
	}
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
	if(self->size <= 0) return;
	self->size--;
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
		if (self->data[i] == value) return i;
	}
  	return self->size; // No match found
}

/*
 * Search for an element in the sorted array.
 */
size_t array_search_sorted(const struct array *self, int value) {
	// Implementation of binary search
	size_t left = 0;
	size_t right = self->size;
	
	while(left < right) {
		size_t mid = left + (right - left) / 2; 
		if(self->data[mid] == value) return mid; // Match found
		else if(self->data[mid] < value) left = mid + 1;
		else right = mid;
	}
	return self->size; // No match Found
}



/*
 * Tell if the array is sorted
 */
bool array_is_sorted(const struct array *self) {
	for(int i = 0; i < (int)self->size - 1; i++) {
		if(self->data[i] > self->data[i + 1]) return false;
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
	if(self->size <= 1) return; // Nothing to sort
	quick_sort_reccu(self, 0, self->size - 1);
	
}

static void heapify(struct array *self, size_t n, size_t i) {
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n && self->data[left] > self->data[largest]) {
        largest = left;
    }

    if (right < n && self->data[right] > self->data[largest]) {
        largest = right;
    }

    if (largest != i) {
        int temp = self->data[i];
        self->data[i] = self->data[largest];
        self->data[largest] = temp;

        heapify(self, n, largest);
    }
}
/*
 * Sort the array with heap sort
 * */
void array_heap_sort(struct array *self) {
    // Build max heap
    for (int i = self->size / 2 - 1; i >= 0; --i) {
        heapify(self, self->size, i);
    }

    // Extract elements from the heap one by one
    for (int i = self->size - 1; i > 0; --i) {
        // Swap the root (maximum element) with the last element
        int temp = self->data[0];
        self->data[0] = self->data[i];
        self->data[i] = temp;

        // Call heapify on the reduced heap
        heapify(self, i, 0);
    }
}


/*
 * Tell if the array is a heap
 */
bool array_is_heap(const struct array *self) {
	if(self == NULL || self->size < 1) return true;
	size_t n = self->size;
	for(size_t i = 0; i < n - 1; i++) {
		size_t lt = 2 * i + 1;
		size_t rt = 2 * i + 2;
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
	array_insert(self, value, i);
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
	return self->data[0];
}

/*
 * Remove the top value in the array considered as a heap
 */
void array_heap_remove_top(struct array *self) {
    if (self->size > 0) {
        self->data[0] = self->data[self->size - 1];
        self->size--;
        heapify(self, self->size, 0);
    }
}
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
	if(curr != NULL || count < size) return false; // One is snaller than the other
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
		free(self->first);
		self->first = NULL;
		self->last = NULL;
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

/*
 * Remove the element at the end of the list
 */
void list_pop_back(struct list *self) {
	if(list_empty(self)) return;
 
	struct list_node *curr = self->last;
	self->last = self->last->prev;
	if(self->last != NULL) self->last->next = NULL;
	else self->first = NULL; // List is empty
	free(curr);
	if(self->first->next == NULL) {
		free(self->first);
		self->first = NULL;
		self->last = NULL;
		return;
	}
}

/*
 * Insert an element in the list (preserving the order)
 * index is valid or equals to the size of the list (insert at the end)
 */
void list_insert(struct list *self, int value, size_t index) {
	if(index == 0) {
		list_push_front(self,  value);
		return;
	}
	if(self == NULL) return;
	struct list_node *curr = self->first;
	//struct list_node *prev = NULL;
	for(size_t i = 0; i < index - 1; i ++) curr = curr->next; 
	assert(curr != NULL);
	struct list_node *new = malloc(sizeof(struct list_node));
	new->next = curr->next;
	curr->next = new;
	new->data = value;
}

/*
 * Remove an element in the list (preserving the order)
 * index is valid
 */
void list_remove(struct list *self, size_t index) {
	if(list_empty(self)) return;
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
	if(curr == NULL) return; // Index is not correct
	prev->next = curr->next;	
	if(curr->next != NULL) curr->next->prev = prev;
	free(curr);
}

int list_get(const struct list *self, size_t index) {
	struct list_node *curr = self->first;
	size_t i = 0;
	while(i < index && curr != NULL) {
		curr = curr->next;
		i++;
	}
	if(curr == NULL) return 0; // i out of bounds
	return curr->data ;
}

void list_set(struct list *self, size_t index, int value) {
	struct list_node *curr = self->first;
	size_t i = 0;

	while(i < index && curr != NULL) {
		curr = curr->next;
		i++;
	}
	if(curr != NULL) curr->data = value;
}

/*
 * Search for an element in the list and return its index or the size of the list if not present.
 */
size_t list_search(const struct list *self, int value) {
	size_t i = 0;
	struct list_node *curr = self->first;
	while(curr != NULL) {
		if(curr->data == value) return i;
		curr = curr->next;	
		i++;
	}
	return i;
}

/*
struct list {
  struct list_node *first;
  struct list_node *last;

};
*/

bool list_is_sorted(const struct list *self) {
	if(self == NULL || self->first == NULL) return true;
	if(self->first->data > self->last->data) return false;
	struct list_node *curr = self->first;
	while(curr->next != NULL) {
		if(curr->next->data < curr->data) return false;
		curr = curr->next;
	}
	return true;
}

/*
 * Split a list in two. At the end, self should be empty.
 */
void list_split(struct list *self, struct list *out1, struct list *out2) {
	size_t size = list_size(self); // Get the size
	for(size_t i = 0; i < size; i++) {
		if(i < size / 2) {
			// If i < size / 2 but in out1
			list_push_back(out1, self->first->data);
			list_pop_front(self);
		} else {
			// Else put in out2
			list_push_back(out2, self->first->data);
			list_pop_front(self);
		}

	}
	assert(self->first == NULL); // Makes sure self if empty
}

/*
 * Merge two sorted lists in an empty list. At the end, in1 and in2 should be empty.
 */
void list_merge(struct list *self, struct list *in1, struct list *in2) {
	while(in1->first != NULL && in2->first != NULL) {
		if(in1->first->data < in2->first->data) {
			list_push_back(self, in1->first->data);
			list_pop_front(in1);
		} else {
			list_push_back(self, in2->first->data);
			list_pop_front(in2);
		}
	}
	// Finish to fill self if needed
	while(in1->first != NULL) {
		list_push_back(self, in1->first->data);
		list_pop_front(in1);
	}

	while(in2->first != NULL) {
		list_push_back(self, in2->first->data);
		list_pop_front(in2);
	}

}

/*
 * Sort a list with merge sort
 */
void list_merge_sort(struct list *self) {
	if(list_size(self) < 2) return;

	// Create
	struct list first;
	struct list last; 
	list_create(&first);
	list_create(&last);

	// Split & merge
	list_split(self, &first, &last);
	list_merge_sort(&first);
	list_merge_sort(&last);
	list_merge(self, &first, &last);

	// Destroy
	list_destroy(&first);
	list_destroy(&last);
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
	if(node->left != NULL) tree_node_destroy(node->left);
	if(node->right != NULL) tree_node_destroy(node->right);
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

struct tree_node *findMin(struct tree_node *self) {
	struct tree_node *min = self;
	while(min->left != NULL) min = min->left;
	return min;
}

struct tree_node *findMax(struct tree_node *self) {
	struct tree_node *max = self;
	while(max->right != NULL) max = max->right;
	return max;
}

void replaceNode(struct tree_node *oldNode, struct tree_node *newNode) {
    if (oldNode->left == NULL && oldNode->right == NULL) {
        // If oldNode is a leaf node, set tree's root to NULL
        // Not sure if this is the desired behavior. You might want to adjust it based on your requirements.
        oldNode = NULL;
    } else if (oldNode->left == NULL) {
        // If oldNode has no left child, replace it with its right child
        oldNode->data = oldNode->right->data;
        replaceNode(oldNode->right, newNode);
    } else if (oldNode->right == NULL) {
        // If oldNode has no right child, replace it with its left child
        oldNode->data = oldNode->left->data;
        replaceNode(oldNode->left, newNode);
    }
    // Adjust other pointers as needed
}

bool tree_remove_reccu(struct tree_node **root, int value) {
    if (*root == NULL) {
        return false; // Value not found
    }

    if (value > (*root)->data) {
        return tree_remove_reccu(&(*root)->right, value);
    } else if (value < (*root)->data) {
        return tree_remove_reccu(&(*root)->left, value);
    }

    // Value found, perform removal
    if ((*root)->left == NULL) {
        // Replace the current node with its right child
        struct tree_node *temp = (*root)->right;
        free(*root);
        *root = temp;
    } else if ((*root)->right == NULL) {
        // Replace the current node with its left child
        struct tree_node *temp = (*root)->left;
        free(*root);
        *root = temp;
    } else {
        // Case 2: Node with two children
        // Get the in-order successor (minimum in the right subtree)
        struct tree_node *successor = findMin((*root)->right);
        // Copy the in-order successor's value to this node
        (*root)->data = successor->data;
        // Remove the in-order successor
        tree_remove_reccu(&(*root)->right, successor->data);
    }

    return true;
}

bool tree_remove(struct tree *self, int value) {
    return tree_remove_reccu(&(self->root), value);
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
	if(self == NULL) return 0;
	return node_height(self->root);
}


void tree_walk_pre_order_reccu(const struct tree_node *self, tree_func_t func, void *user_data) {
	func(self->data, user_data);
	if(self->right != NULL) tree_walk_pre_order_reccu(self->right, func, user_data);
	if(self->left != NULL) tree_walk_pre_order_reccu(self->left, func, user_data);
}

void tree_walk_pre_order(const struct tree *self, tree_func_t func, void *user_data)  {
	tree_walk_pre_order_reccu(self->root, func, user_data);
}

void tree_walk_in_order_reccu(const struct tree_node *self, tree_func_t func, void *user_data) {
	if(self->left != NULL)	tree_walk_in_order_reccu(self->left, func, user_data);
	func(self->data, user_data);
	if(self->right != NULL)	tree_walk_in_order_reccu(self->right, func, user_data);
}

void tree_walk_in_order(const struct tree *self, tree_func_t func, void *user_data) {
	tree_walk_in_order_reccu(self->root, func, user_data);
}

void tree_walk_post_order_reccu(const struct tree_node *self, tree_func_t func, void *user_data) {
	if(self->left != NULL)	tree_walk_post_order_reccu(self->left, func, user_data);
	if(self->right != NULL)	tree_walk_post_order_reccu(self->right, func, user_data);
	func(self->data, user_data);
}
void tree_walk_post_order(const struct tree *self, tree_func_t func, void *user_data) {
	tree_walk_post_order_reccu(self->root, func, user_data);
}
