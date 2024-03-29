
#include<iostream>
#include<vector>
#include<stack>
/*
1 задача
struct Test
{
    int number;

    ~Test()
    {
        std::cout << "Test::~Test(): " << number << std::endl;
    }
    Test()
    {
        std::cout << "Test::Test()" << std::endl;
    }
    Test(int const n)
        : number(n)
    {
        std::cout << "Test::Test(int): " << number << std::endl;
    }
    Test(Test const &other)
        : number(other.number)
    {
        std::cout << "Test::Test(Test const &) : " << number << std::endl;
    }
    Test(Test      &&other)
        : number(other.number)
    {
        other.number = 0;
        std::cout << "Test::Test(Test &&) : " << number << std::endl;
    }
    Test &operator=(Test const &other)
    {
        std::cout << "Test &Test::operator=(Test const &) : " << number << " -> " << other.number << std::endl;
        if(this != &other)
        {
            this->~Test();
            new (this) Test(other);
        }
        return *this;
    }
    Test &operator=(Test &&other)
    {
        std::cout << "Test &Test::operator=(Test &&) : " << number << " -> " << other.number << std::endl;
        if(this != &other)
        {
            this->~Test();
            new (this) Test(static_cast<Test &&>(other));
        }
        return *this;
    }
};


namespace student
{
template<typename T>

struct stack : private std::vector<T>
{
    T       &top() ;
    T const &top() const;

    bool        empty() const;
    std::size_t  size() const;

    void push(T const &value);
    void push(T      &&value);

    void pop();

    template<typename... Args>
    void emplace(Args &&... args)
    {
        this->emplace_back(static_cast<Args &&>(args)...);
    }
};

template<typename T>
T &stack<T>::top()       {return this->back();};

template<typename T>
T const &stack<T>::top() const {return this->back();};

template<typename T>
bool stack<T>::empty()       const {return static_cast<const std::vector<T> *>(this)->empty();};

template<typename T>
std::size_t stack<T>::size() const {return static_cast<const std::vector<T> *>(this)->size();};

template<typename T>
void stack<T>::push(T const &value) {return this->push_back(value);};

template<typename T>
void stack<T>::push(T      &&value) {return this->push_back(std::move(value));};

template<typename T>
void stack<T>::pop() {return this->pop_back();};

}

int main()
{
    student::stack<Test> s;
    std::cout << s.empty() << std::endl;
    std::cout << s.size() << std::endl;
    
    Test test(-2);
    s.push(test);
    std::cout << s.top().number << std::endl;
    
    for(int i = 0; i < 5; ++i)
        s.push(Test(i + 1));
        
    test.number = -1;
    s.push(std::move(test));
    
    student::stack<Test> copy(s);
    std::cout << copy.empty() << std::endl;
    std::cout << copy.size() << std::endl;
    student::stack<Test> moved(std::move(s));
    
    while(!moved.empty())
    {
        std::cout << moved.top().number << std::endl;
        moved.pop();
    }
}
*/
namespace student
{
template<typename T>
class dynarray
{
    T *ptr;
    std::size_t size, capacity;

    void reallocate(std::size_t n);

public:
    ~dynarray()
    {
        for(std::size_t i = 0; i < size; ++i)
            (ptr + i)->~T();
        std::free(ptr);
    }
    dynarray()
        : ptr(nullptr)
        , size(0)
        , capacity(0)
    {}
    dynarray(dynarray<T> const &);
    dynarray(dynarray<T>      &&);
    dynarray<T> &operator=(dynarray<T> const &);
    dynarray<T> &operator=(dynarray<T>      &&);

    T       *begin()       {return ptr;}
    T       *  end()       {return ptr + size;}
    T const *begin() const {return ptr;}
    T const *  end() const {return ptr + size;}

    template<typename... Args>
    void emplace_back(Args &&... args)
    {
        if(size == capacity)
            reallocate(capacity == 0 ? 1 : 2 * capacity);
        new (ptr + size++) T(static_cast<Args &&>(args)...);
    }
};

template<typename T>
void reallocate(std::size_t n)
{
    T *ptr1 = static_cast<T *>(std::malloc(sizeof(T)) * n)
    for(std::size_t i = 0; i < size; i++)
    {
        new (ptr1 + i) T(static_cast<T &&>(ptr[i]));
        (ptr + i)->~T();
    }
    free(ptr);
    ptr = ptr1;
    capacity = n;

}

}
int main()
{
     student::dynarray<Test> array;
    
    Test test(-2);
    array.emplace_back(test);
    test.number = -1;
    array.emplace_back(std::move(test));
    
    for(int i = 1; i < 5; ++i)
        array.emplace_back(i);
    array.emplace_back(Test(10));
    
    for(Test const &test : array)
        std::cout << test.number << ' ';
    std::cout << std::endl;
}