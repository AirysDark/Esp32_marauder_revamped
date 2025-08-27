// LinkedListCompat.h
#pragma once
#include <Arduino.h>
#include <functional>

/*
 * Minimal LinkedList replacement that provides only
 * the subset your project actually uses:
 *   - add()
 *   - size()
 *   - operator[]
 *   - clear()
 *   - begin() / end() (range-based for)
 */

template <class T>
class LinkedList {
  struct Node {
    T value;
    Node* next;
    Node(const T& v) : value(v), next(nullptr) {}
  };
  Node* head = nullptr;
  Node* tail = nullptr;
  size_t count = 0;

public:
  LinkedList() = default;
  ~LinkedList() { clear(); }

  void add(const T& v) {
    Node* n = new Node(v);
    if (!head) {
      head = tail = n;
    } else {
      tail->next = n;
      tail = n;
    }
    count++;
  }

  size_t size() const { return count; }

  T& operator[](int idx) {
    Node* cur = head;
    for (int i = 0; cur && i < idx; i++) cur = cur->next;
    return cur->value;
  }

  void clear() {
    Node* cur = head;
    while (cur) {
      Node* tmp = cur->next;
      delete cur;
      cur = tmp;
    }
    head = tail = nullptr;
    count = 0;
  }

  // Iterator support
  struct Iterator {
    Node* node;
    Iterator(Node* n) : node(n) {}
    T& operator*() { return node->value; }
    Iterator& operator++() { node = node->next; return *this; }
    bool operator!=(const Iterator& other) const { return node != other.node; }
  };

  Iterator begin() { return Iterator(head); }
  Iterator end() { return Iterator(nullptr); }
};