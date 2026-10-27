# Linked List Implementation Checklist

## 1. Constructors & Memory Management

### Basic

* Default constructor
* Destructor

### Copy Semantics

* Copy constructor
* Copy assignment operator

### Move Semantics

* Move constructor
* Move assignment operator

### Utility

* clear()

---

## 2. State Queries

* empty()
* size()

---

## 3. Front/Back Access

* front()
* back()

---

## 4. Insertion Operations

### End Insertions

* push_front(value)
* push_back(value)

### Position-Based Insertions

* insert(index, value)

### Node-Based Insertions

* insert_after(node, value)
* insert_before(node, value)

---

## 5. Deletion Operations

### End Deletions

* pop_front()
* pop_back()

### Position-Based Deletions

* erase(index)

### Node-Based Deletions

* erase(node)

### Value-Based Deletions

* remove(value)
* remove_all(value)

---

## 6. Search Operations

* find(value)
* contains(value)
* index_of(value)

---

## 7. Element Access

* at(index)
* operator[](index)

---

## 8. Traversal Utilities

* print()
* for_each(function)

---

## 9. Reordering & Transformation

### Reversal

* reverse()

### Rotation

* rotate_left(k)
* rotate_right(k)

### Copying

* clone()

### Swapping

* swap(other)

---

## 10. Sorting & Deduplication

### Sorting

* sort()

### Duplicate Handling

* unique()
* remove_duplicates()

---

## 11. Combining Lists

* merge(other)
* append(other)
* splice(other)

---

## 12. Iterator Support

### Iterator Class

* begin()
* end()

### Const Iterator

* begin() const
* end() const

### Iterator Operations

* Increment
* Dereference
* Equality comparison
* Inequality comparison

---

## 13. Structural Algorithms

### Middle Element

* middle()

### K-th Element from End

* kth_from_end(k)

### N-th Element from Start

* nth_from_start(n)

### Length Computation

* length_iterative()
* length_recursive()

---

## 14. Cycle Algorithms

### Detection

* has_cycle()

### Cycle Information

* cycle_start()
* cycle_length()

### Cycle Manipulation

* remove_cycle()

---

## 15. Palindrome Algorithms

* is_palindrome()

---

## 16. Intersection Algorithms

* intersection(other)
* intersection_node(other)

---

## 17. Splitting Algorithms

* split_at(index)
* split_middle()

---

## 18. Advanced Linked List Interview Problems

### Reverse Variants

* reverse_recursive()
* reverse_in_groups(k)

### Rearrangement

* odd_even_rearrange()
* pairwise_swap()

### Merge-Based

* merge_sorted_lists(other)

### Node Manipulation

* delete_middle()
* delete_nth_from_end(n)

### Alternate Merging

* zip_merge(other)

---

## 19. Testing Checklist

### Empty List

* Insert into empty list
* Delete from empty list
* Search in empty list

### Single Node List

* Insert
* Delete
* Reverse

### Multiple Nodes

* Insert at beginning
* Insert at middle
* Insert at end
* Delete at beginning
* Delete at middle
* Delete at end

### Edge Cases

* Duplicate values
* Large lists
* Invalid indices
* Self-assignment
* Copy correctness
* Move correctness

---

# Suggested Implementation Order

1. Node structure
2. Constructor + destructor
3. push_front()
4. push_back()
5. print()
6. size()
7. front() / back()
8. pop_front()
9. pop_back()
10. insert()
11. erase()
12. find() / contains()
13. reverse()
14. sort()
15. iterator support
16. copy constructor
17. copy assignment
18. move constructor
19. move assignment
20. advanced interview algorithms
