//
// Created by Tsinin on 09.11.2018.
//
#include "Vector.h"
#include <memory>

template <typename Key>
class binomialHeap {
public:
    class Pointer;
    struct node {
        node *firstChild, *lastChild, *parent, *next, *prev;
        Pointer* backPtr;
        int degree = 0;
        bool smallest = false;
        Key data;
        node(Key newData) {
            data = newData;
            backPtr = nullptr;
            firstChild = lastChild = parent = next = prev = nullptr;
        };
        node() {
            backPtr = nullptr;
            firstChild = lastChild = parent = next = prev = nullptr;
        };
    };
    class Pointer {
    public:
        ~Pointer() = default;
    private:
        friend class binomialHeap<Key>;
        Pointer(node* new_ptr): element(new_ptr) {}
        node* element;
    };
    bool is_empty() const {
        return (firstNode == nullptr);
    }
    void merge(binomialHeap &otherHeap) {
        node *firstPointer = firstNode, *secondPointer = otherHeap.firstNode;
        Vector<node*> queue;
        while(firstPointer && secondPointer) {
            if(firstPointer->degree < secondPointer->degree) {
                queue.push_back(firstPointer);
                if(!firstPointer->next){
                    firstPointer->prev = firstPointer->parent = nullptr;
                    firstPointer = firstPointer->next;
                }
                else {
                    firstPointer->prev = firstPointer->parent = nullptr;
                    firstPointer = firstPointer->next;
                    firstPointer->prev->next = nullptr;
                }
            } else {
                queue.push_back(secondPointer);
                if(!secondPointer->next){
                    secondPointer->prev = secondPointer->parent = nullptr;
                    secondPointer = secondPointer->next;
                }
                else {
                    secondPointer->prev = secondPointer->parent = nullptr;
                    secondPointer = secondPointer->next;
                    secondPointer->prev->next = nullptr;
                }
            }
        }
        while(firstPointer) {
            queue.push_back(firstPointer);
            if(!firstPointer->next){
                firstPointer->prev = firstPointer->parent = nullptr;
                firstPointer = firstPointer->next;
            }
            else {
                firstPointer->prev = firstPointer->parent = nullptr;
                firstPointer = firstPointer->next;
                firstPointer->prev->next = nullptr;
            }
        }
        while(secondPointer) {
            queue.push_back(secondPointer);
            if(!secondPointer->next){
                secondPointer->prev = secondPointer->parent = nullptr;
                secondPointer = secondPointer->next;
            }
            else {
                secondPointer->prev = secondPointer->parent = nullptr;
                secondPointer = secondPointer->next;
                secondPointer->prev->next = nullptr;
            }
        }
        int current = 0;
        node* temp = new node();
        node* begin = temp;
        while(current < queue.size()) {
            while(current < queue.size() - 2 && queue[current]->degree == queue[current + 2]->degree) {
                queue[current + 2] = merge(queue[current], queue[current + 2]);
                temp->next = queue[current + 1];
                temp->next->prev = temp;
                temp = temp->next;
                current += 2;
            }
            bool goAgain = false;
            while(current < queue.size() - 1 && queue[current]->degree == queue[current + 1]->degree) {
                if(current < queue.size() - 2 && queue[current]->degree == queue[current + 2]->degree) {
                    goAgain = true;
                    break;
                }
                queue[current + 1] = merge(queue[current], queue[current + 1]);
                current++;
            }
            if(goAgain)
                continue;
            temp->next = queue[current];
            temp->next->prev = temp;
            temp = temp->next;
            current++;
        }
        firstNode = begin->next;
        if(firstNode)
            firstNode->prev = nullptr;
        otherHeap.firstNode = nullptr;
    }
    Pointer* insert(Key x) {
        binomialHeap<Key> newHeap;
        newHeap.firstNode = new node(x);
        Pointer* pointer = new Pointer(newHeap.firstNode);
        Pointer* par = new Pointer(nullptr);
        newHeap.firstNode->backPtr = pointer;
        this->merge(newHeap);
        return pointer;
    }
    Key get_min()  {
        try {
            if(this->is_empty())
                throw std::logic_error("Heap is empty.");
        }
        catch(std::logic_error& error) {
            std::cerr << error.what() << std::endl;
            exit(1);
        }
        node* cur = firstNode;
        Key minElement = cur->data;
        while(cur) {
            if(cur->data < minElement)
                minElement = cur->data;
            cur = cur->next;
        }
        return minElement;
    }
    Key extract_min() {
        try {
            if(this->is_empty())
                throw std::logic_error("Heap is empty.");
        }
        catch(std::logic_error& error) {
            std::cerr << error.what() << std::endl;
            exit(1);
        }
        node* cur = firstNode;
        node* minElement = cur;
        while(cur) {
            if(cur->data < minElement->data || cur->smallest)
                minElement = cur;
            cur = cur->next;
        }
        Key ans = minElement->data;

        binomialHeap<Key> secondHeap;
        secondHeap.firstNode = minElement->firstChild;
        if(minElement->prev && minElement->next) {
            minElement->prev->next = minElement->next;
            minElement->next->prev = minElement->prev;
        } else if(minElement->prev) {
            minElement->prev->next = nullptr;
        } else if(minElement->next){
            firstNode = minElement->next;
            minElement->next->prev = nullptr;
        } else
            firstNode = nullptr;

        delete minElement->backPtr;
        delete minElement;
        this->merge(secondHeap);
        return ans;
    }
    void erase(Pointer* pointer) {
        pointer->element->smallest = true;
        decreaseKey(pointer, pointer->element->data);
        delete pointer;
        this->extract_min();
    }
    void change(Pointer* pointer, Key newKey) {
        if(newKey > pointer->element->data) {
            this->erase(pointer);
            pointer = insert(newKey);
        } else {
            decreaseKey(pointer, newKey);
        }
    }
    binomialHeap() = default;
    ~binomialHeap() {
        while(firstNode != nullptr) {
            node* temp = firstNode->next;
            destruct(firstNode);
            firstNode = temp;
        }
    };
private:
    node* firstNode = nullptr;
    void destruct(node* x) {
        if(x == nullptr)
            return;
        node* child = x->firstChild;
        while(child != x->lastChild) {
            node* temp = child->next;
            destruct(child);
            child = temp;
        }
        destruct(child);
        delete x->backPtr;
        delete x;
    }
    node* merge(node* first, node* second) {
        if(first->data >= second->data) {
            node* temp = first;
            first = second;
            second = temp;
        }
        if(!first->lastChild) {
            first->lastChild = first->firstChild = second;
            second->parent = first;
            second->next = second->prev = nullptr;
            first->degree++;
            return first;
        }
        first->lastChild->next = second;
        second->next = nullptr;
        second->prev = first->lastChild;
        first->lastChild = first->lastChild->next;
        second->parent = first;
        first->degree++;
        return first;
    }
    void swapWithParent(node*& ptr) {
        Key tempForData = ptr->data;
        ptr->data = ptr->parent->data;
        ptr->parent->data = tempForData;

        Pointer* tempForBackPtr = ptr->backPtr;
        ptr->backPtr = ptr->parent->backPtr;
        ptr->parent->backPtr = tempForBackPtr;

        ptr->backPtr->element = ptr->parent;
        ptr->parent->backPtr->element = ptr;
    }
    void decreaseKey(Pointer* pointer, int newKey) {
        node* currentNode = pointer->element;
        currentNode->data = newKey;
        while(currentNode->parent && (currentNode->smallest || currentNode->data < currentNode->parent->data)) {
            swapWithParent(currentNode);
            currentNode = currentNode->parent;
        }
    }
};
