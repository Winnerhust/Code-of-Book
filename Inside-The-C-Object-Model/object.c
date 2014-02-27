/**************
 *convert C++ Class to C
 ***********************
class A
{
public:
        int max()
        {
                int m = (a > b ? a : b);
                return m;
        }
        static void bar()
        {
                printf("%d\n",c);
        }
public:
        int a;
        int b;
        static int c;
};
int A::c = 20;

int main()
{
        A a;
        a.a = 10;
        a.b = 20;
        printf("%d\n",a.max());

        A::bar();

        return 0;
}
************/
#include <stdio.h>

/*class object*/
typedef struct A{
        int a;
        int b;
}A;
// static member definition
int A_c_static = 20;

//convert nonstatic member function to nonmember function
//add this pointer
int A_max_vi(A * _this)
{
        //change to reference path
        int m = (_this->a > _this->b ? _this->a : _this->b);
        return m;
}
//convert static member function to nonmember function
void A_bar_vv()
{
        //static member reference
        printf("%d\n",A_c_static);
}

int main()
{
        A a;
        a.a = 10;
        a.b = 20;
        //change reference
        printf("%d\n",A_max_vi(&a));

        A_bar_vv();

        return 0;
}
