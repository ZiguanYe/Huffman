#include <iostream>
#include "rem_dup_lib.h"
using namespace std;

// Add your implementations below

void removeConsecutive(Item* head)
{
    //base case: the list is empty or reach the end of list
    if (head==nullptr || head->next==nullptr)
    {
        return;
    }
    else if (head->val == head->next->val)
    {
        Item* Next = head->next;
        head->next = head->next->next;
        delete Next;
        removeConsecutive(head);
        
        //seg fault
        //Item* curr = head;
//        head= head->next;
//        delete curr;
//        removeConsecutive(head->next);
    }
    else
    {
        removeConsecutive(head->next);
    }
}

Item* concatenate(Item* head1, Item* head2)
{
    
  Item* ans;
    //base case: both empty
    if (head1 == nullptr && head2 == nullptr)
    {
        ans = nullptr;
    }
    else if(head1 == nullptr && head2 != nullptr)
    {
        ans=new Item(head2->val,concatenate(nullptr,head2->next));
        //delete head2;
    }
    else if(head1!=nullptr)
    {
        ans = new Item(head1->val, concatenate(head1->next, head2));
        //delete head1;
    }

    return ans;
    
}

