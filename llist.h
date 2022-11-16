#ifndef LLIST_H
#define LLIST_H
#include <stdexcept>

template<typename T>
class LList {
public:
    struct Item {
        T val;
        Item* next;
    };

    LList();
    ~LList();
    bool empty() const;
    size_t size() const;
    void push_front(const T& v);
    void push_back(const T& v);
    void remove(size_t idx);
    T const & operator[](size_t idx) const;
    T & operator[](size_t idx) ;
    
private:
    Item* getNodeAt(size_t idx) const;
    Item* head_;
    size_t size_;
};

template <typename T>
LList<T>::LList()
{
    head_ = nullptr;
    size_ = 0;
}

template <typename T>
LList<T>::~LList()
{
    while(nullptr != head_){
        this->remove(0);
    }
}

template <typename T>
bool LList<T>::empty() const
{
    return head_ == nullptr;
}

template <typename T>
size_t LList<T>::size() const
{
    return size_;
}

template <typename T>
void LList<T>::push_front(const T& v)
{
    Item* ptr = new Item;
    ptr->val = v;
    ptr->next = head_;
    head_ = ptr;
    size_++;
}

template <typename T>
void LList<T>::push_back(const T& v)
{
    Item* newItem = new Item;
    newItem->val = v;
    newItem->next = nullptr;
    if(head_ == nullptr){
        head_ = newItem;
    }
    else {
        Item* ptr = head_;
        while(ptr->next != nullptr) {
            ptr = ptr->next;
        }
        ptr->next = newItem;
    }
    size_++;
}

template <typename T>
void LList<T>::remove(size_t idx)
{
    if(nullptr == head_) {
        throw std::underflow_error("Cannot pop empty list");
    }
    else if(idx == 0) {
        Item* ptr = head_;
        head_ = head_->next;
        delete ptr;
    }
    else {
        Item* ptr = getNodeAt(idx-1);
        // if there is not even idx-1 items
        if(nullptr == ptr) {
            throw std::underflow_error("Bad index in remove");    
        }
        Item* temp = ptr->next;
        // if the idx-th item doesn't exist
        if(temp == nullptr) {
            throw std::underflow_error("Bad index in remove");    
        }
        ptr->next = temp->next;
        delete temp;
    }
    size_--;
}

template <typename T>
typename LList<T>::Item* LList<T>::getNodeAt(size_t idx) const
{
    Item* ptr = head_;
    while(idx != 0 && ptr != nullptr) {
        idx--;
        ptr = ptr->next;
    }
    return ptr;
}

template <typename T>
T const & LList<T>::operator[](size_t idx) const
{
    Item* ptr = getNodeAt(idx);
    if(nullptr == ptr) {
        throw std::underflow_error("Cannot get front item of empty list");   
    }
    return ptr->val;
}

template <typename T>
T & LList<T>::operator[](size_t idx)
{
    Item* ptr = getNodeAt(idx);
    if(nullptr == ptr) {
        throw std::underflow_error("Cannot get front item of empty list");    
    }
    return ptr->val;
}


#endif