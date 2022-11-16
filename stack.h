#ifndef STACK_H
#define STACK_H

/** 
 * Your Stack<T> class must be templated and
 * privately inherit from LList<T>. All
 * operations must run in O(1).
 *
 * Both pop() and top() must throw std::underflow_error
 * if the stack is empty.
 */
#include <stdexcept>
#include "llist.h"

template <typename T>
class Stack : private LList<T>// complete the declaration and implementation
{
	public:
    Stack();
    size_t size() const;
    bool empty() const;
    void push(const T& item);
    void pop();
    T const & top() const;

};

template <typename T>
Stack<T>::Stack() : LList<T>()
{
}

template <typename T>
size_t Stack<T>::size() const
{
    return LList<T>::size();
}

template <typename T>
bool Stack<T>::empty() const
{
    return LList<T>::empty();
}

template <typename T>
void Stack<T>::push(const T& item)
{
    return LList<T>::push_front(item);
}

template <typename T>
void Stack<T>::pop()
{
    if(Stack<T>::size()==0)
    {
        throw std::underflow_error("empty stack");
    }
    else
    {
        return LList<T>::remove(0);
    }
}

template <typename T>
T const & Stack<T>::top() const
{
    if(Stack<T>::size()==0)
    {
        throw std::underflow_error("empty stack");
    }
    else
    {
        return LList<T>::operator[](0);
    }
}
#endif
