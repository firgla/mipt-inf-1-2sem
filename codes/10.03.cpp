#include<iostream>


void print(int        &) {std::cout << "int        &" << std::endl;}
void print(int       &&) {std::cout << "int       &&" << std::endl;}
void print(int const  &) {std::cout << "int const  &" << std::endl;}
void print(int const &&) {std::cout << "int const &&" << std::endl;}

/*
1 задача
void forward_print(int        &a){print(a);}
void forward_print(int        &&a){print(static_cast<int       &&>(a));}
void forward_print(int const   &a){print(a);}
void forward_print(int const  &&a){print(static_cast<int const &&>(a));}

int main()
{
    int i = 0;
    int const ci = 0;
    auto const func = []() {return 0;};
    print(i);       
    print(ci);      
    print(0);     
    print(func());  
    print(static_cast<int       &&>(i)); 
    print(static_cast<int const &&>(i));

    std::cout<< "-----------------"<< std::endl;

    forward_print(i);       
    forward_print(ci);      
    forward_print(0);     
    forward_print(func());  
    forward_print(static_cast<int       &&>(i)); 
    forward_print(static_cast<int const &&>(i));

    std::cout<< "-----------------"<< std::endl;
    forward_print(i);
    forward_print(0);
    forward_print(static_cast<int const  &>(i));
    forward_print(static_cast<int const &&>(i));
}
*/





#include <type_traits>
template<typename T>
void universal_reference(T &&i)
{
    if(std::is_same_v<T, int>)
        std::cout << "int         " << std::endl;
    if(std::is_same_v<T, int const>)
        std::cout << "int const   " << std::endl;
    if(std::is_same_v<T, int &>)
        std::cout << "int        &" << std::endl;
    if(std::is_same_v<T, int &&>)
        std::cout << "int       &&" << std::endl;
    if(std::is_same_v<T, int const &>)
        std::cout << "int const  &" << std::endl;
    if(std::is_same_v<T, int const &&>)
        std::cout << "int const &&" << std::endl;
}




template<typename T>
void forward_print(T && a)
{
    print(static_cast<T       &&>(a));
}

int main()
{
    int i = 0;
    forward_print(i);
    std::cout<<" 1 "<< std::endl;
    forward_print(0);
    std::cout<<" 2 "<< std::endl;
    forward_print(static_cast<int const  &>(i));
    std::cout<<" 3 "<< std::endl;
    forward_print(static_cast<int const &&>(i));
}

