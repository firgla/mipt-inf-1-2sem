template<typename T>
class vector
{
private:

    T *ptr;
    std::size_t size, capacity;

    void reallocate(std::size_t n);

public:

    vector()
        : ptr(nullptr)
        , size(0)
        , capacity(0)
    {}

    vector(vector<T> const &);
    vector(vector<T>      &&);

    vector<T> &operator=(vector<T> const &);
    vector<T> &operator=(vector<T>      &&);

/*member types*/

    using      value_type = T;
    using       size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using       reference = value_type       &;
    using const_reference = value_type const &;


    /*pointer, const_pointer, iterator, const_iterator*/


    using       reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

/*member functions*/



/*element access*/

    T       &operator[](std::size_t i)       {return ptr[i];}
    T const &operator[](std::size_t i) const {return ptr[i];}

    T       &front()                         {return ptr[0];}
    T       &back()                          {return ptr[size] - 1;}

    T const &front()                    const {return ptr[0];}
    T const &back()                     const {return ptr[size - 1];}



/*iterators*/

    T       *begin()       {return ptr;}
    T       *  end()       {return ptr + size;}

    T const *begin() const {return ptr;}
    T const *  end() const {return ptr + size;}

/*capacity*/

    bool         empty() const {return capacity == 0;}

    size_type     size() const {return size;}

    size_type capacity() const {return capacity;}

    void shrink_to_fit()
    {
        if (size == capacity) return;

        reallocate(size);
    }

    void emplace_back(Args &&... args)
    {
        if(size == capacity)
            reallocate(capacity == 0 ? 1 : 2 * capacity);
        new (ptr + size++) T(static_cast<Args &&>(args)...);
    }

    void reallocate(std::size_t n)
    {

        T *ptr1 = static_cast<T *>(std::malloc(sizeof(T)) * n)
        for(std::size_t i = 0; i < size; i++)
        {
            new (ptr1 + i) T(static_cast<T &&>(ptr[i]));
            (ptr + i)->~T();
        }

        std::free(ptr);
        ptr = ptr1;
        capacity = n;

    }

    ~vector()
    {
        for(std::size_t i = 0; i < size; ++i)
            (ptr + i)->~T();
        std::free(ptr);
    }
    
};


