struct Node
{
    int value;
    Node *next;
};

using List = Node *;

struct ListUncons
{
    Node *head;
    List tail;
};


/*
1 задача
List concatList(List left, List right)
{
    if (left == nullptr)
        return right;
    List last = left;
    while(last->next != nullptr)
        last = last -> next;
    last->next = right;
    return left;
}
*/

/*
2 задача
ListIterator begin(List list)
{
    return {list};
}

ListIterator end(List list)
{
    return {nullptr};
}
*/

/*
3 задача
ListIterator next(ListIterator iterator, int n = 1)
{
int sc;
while(sc =< n)
    {
    iterator++;
    sc++;
    }
    return iterator;
}
*/

int distance(ListIterator first, ListIterator last)
{
    int k = 0;
    while(first != last)
        {
        first++;
        k++;
        }
    return k; 
}

void iter_swap(ListIterator first, ListIterator second)
{
    int perv = *first;
    *first = * second;
    *second = perv;
}
