#ifndef HEAP_H
#define HEAP_H
#include <functional>
#include <stdexcept>
#include <vector>


template <typename T, typename PComparator = std::less<T> >
class Heap
{
public:
  /**
   * @brief Construct a new Heap object
   * 
   * @param m ary-ness of heap tree (default to 2)
   * @param c binary predicate function/functor that takes two items
   *          as an argument and returns a bool if the first argument has
   *          priority over the second.
   */
  Heap(int m=2, PComparator c = PComparator());

  /**
  * @brief Destroy the Heap object
  * 
  */
  ~Heap();

  /**
   * @brief Push an item to the heap
   * 
   * @param item item to heap
   */
  void push(const T& item);

  /**
   * @brief Returns the top (priority) item
   * 
   * @return T const& top priority item
   * @throw std::underflow_error if the heap is empty
   */
  T const & top() const;

  /**
   * @brief Remove the top priority item
   * 
   * @throw std::underflow_error if the heap is empty
   */
  void pop();

  /// returns true if the heap is empty

  /**
   * @brief Returns true if the heap is empty
   * 
   */
  bool empty() const;
    
    int size() const;

private:
  /// Add whatever helper functions and data members you need below
    std::vector<T> list;
    PComparator c;
    //m-ary heap
    int m;

    void trickleUp(int pos);
    void trickleDown(int pos);

};

// Add implementation of member functions here

template<class T, class PComparator>
Heap<T,PComparator>::Heap(int m, PComparator c)
{
    this->m = m;
    this->c = c;
}

template<class T, class PComparator>
Heap<T,PComparator>::~Heap()
{
    
}

// We will start top() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
T const & Heap<T,PComparator>::top() const
{
  // Here we use exceptions to handle the case of trying
  // to access the top element of an empty heap
  	if(empty())
  	{
      	throw std::underflow_error("the heap is empty");
  	}
    
    return list[0];

}


// We will start pop() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
void Heap<T,PComparator>::pop()
{
    if(empty())
    {
        throw std::underflow_error("the heap is empty");
    }
    
    list[0] = list[list.size()-1];
    list.pop_back();
    trickleDown(0);

}

template <typename T, typename PComparator>
void Heap<T, PComparator>::push(const T& item)
{
    list.push_back(item);
    trickleUp(list.size()-1);
}
#endif

template <typename T, typename PComparator>
bool Heap<T, PComparator>::empty() const
{
    return list.empty();
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::trickleUp(int pos)
{
    
    if(pos==0 || list.empty())
    {
        return;
    }
    else
    {
        int prtIdx = (pos-1)/m;
        
        while(prtIdx>=0 && c(list[pos], list[prtIdx]))
        {
            std::swap(list[pos], list[prtIdx]);
            pos = prtIdx;
            prtIdx = (pos-1)/m;
            //if (pos==prtIdx) break;
        }
    }
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::trickleDown(int pos)
{
    int minChd = pos*m+1;
    int lastIdx = list.size()-1;
    
    //make sure that children exists, i.e. not leaf nodes
    if(minChd>lastIdx || list.empty())
    {
        return;
    }
    else
    {
        //let the first child be temporary min child
        for (int i=0; i<m; i++)
        {
            if(pos*m+1+i<=lastIdx && c(list[pos*m+1+i], list[minChd]))
            {
                minChd=pos*m+1+i;
            }
        }
    }
    
    if (c(list[minChd], list[pos]))
    {
        std::swap(list[pos], list[minChd]);
        trickleDown(minChd);
    }
}

template <typename T, typename PComparator>
int Heap<T, PComparator>::size() const
{
    return list.size();
}
