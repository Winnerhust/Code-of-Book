/**
 * Inside The C++ Object Model
 */
#include <iostream>
#include <stdio.h>
using namespace std;

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
                cout<<c<<endl;
        }
public:
        int a;
        int b;
        static int c;
};
int A::c = 20;

class B:public A
{
        public:
                void hello(int s)
                {
                }
        public:
                int c;
                int d;
};


void test1()
{
        cout<<"sizeof(A)="<<sizeof(A)<<endl;
        cout<<"sizeof(int)="<<sizeof(int)<<endl;

        A t;
        t.a = 11;
        t.b = 22;
        A::c = 33;

        int *p = (int *)&t;
        cout<<"t[0]="<<p[0]<<endl;
        cout<<"t[1]="<<p[1]<<endl;

        cout<<"&t = "<<&t<<endl;
        cout<<"&t.a = "<<&t.a<<endl;
        cout<<"&t.b = "<<&t.b<<endl;
        cout<<"&A::c = "<<&A::c<<endl;

}
void test2()
{
        cout<<"sizeof(B)="<<sizeof(B)<<endl;
        cout<<"sizeof(int)="<<sizeof(int)<<endl;

        B h;
        h.a = 11;
        h.b = 22;
        A::c = 33;
        h.c = 44;
        h.d = 55;

        int *p = (int *)&h;
        cout<<"h[0]="<<p[0]<<endl;
        cout<<"h[1]="<<p[1]<<endl;
        cout<<"h[2]="<<p[2]<<endl;
        cout<<"h[3]="<<p[3]<<endl;

        cout<<"&h = "<<&h<<endl;
        cout<<"&h.a = "<<&h.a<<endl;
        cout<<"&h.b = "<<&h.b<<endl;
        cout<<"&A::c = "<<&A::c<<endl;
        cout<<"&h.c = "<<&h.c<<endl;
        cout<<"&h.d = "<<&h.d<<endl;
}

int main()
{
        test1();
        test2();

        return 0;
}
