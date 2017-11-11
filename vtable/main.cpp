
/*First, what is the contract for instances of virtual classes? E.g. if you have a variable IFace1* obj:

There is a pointer to virtual table at obj+0.
Any member data fields would continue at obj+8 (sizeof(void*)).
The virtual table contains one record which points to void fcn(int) at vtbl+0.
In the table, there is also a pointer to typeinfo of the class at vtbl-8 (used by dynamic_cast etc.) and "offset to base" at vtbl-16.
Any function which sees a variable of type IFace1* can depend on this being true. Similarly for IFace2*.

If they want to call the virtual function void fcn(int), they look at obj+0 to get the vtable, then at vtbl+0 and call the address found there. this is set to obj.
If they want to access a member field (by themselves, e.g. if the field has public access, or if there is an inline accessor), they just read/write the member at its address obj+xxx.
If they want to see what type they really have, they subtract the value at vtbl-16 from the address to their object, then look at the typeinfo pointer of the vtable referenced by the base object.
Now, how can you compiler satisfy these requirements for a class with multiple inheritance?

1) First it needs to generate the structure for itself. The virtual table pointer must be at obj+0, so there it is. How will the table look like? Well, the offset to base is 0, obviously, the typeinfo data and pointer to it is generated easily, then the first virtual function and the second, nothing special. Anyone who knows the definition of RealClass can do the same calculations, so they know where to find the functions in the vtable etc.

2) Then it goes to make it possible to let RealClass be passed around as IFace1. So it needs to have a pointer to virtual table in the IFace1 format somewhere in the object, then the virtual table must have that one record for void fcn(int).

The compiler is clever and sees that it can reuse the first virtual table it has generated, because it complies with these requirements. If there were any member fields, they would be stored after the first pointer to the virtual table, so even them could be accessed simply as if the derived class was the base one. So far so good.

3) Finally, what to do with the object so others will be able to use it as IFace2? The one vtable already created cannot be used anymore, because IFace2 needs its void fcn1(int) to be at vtbl+0.

So another virtual table is created, the one you see immediately following the first one in your dump, and a pointer to it is stored in RealClass at the next available place. This second table needs to have offset to base set to -8, because the real object starts at offset -8. And it contains just the pointer to that IFace2 virtual function, void fcn1(int).

The virtual pointer in the object (at offset obj+8) would be then followed by any member data fields of IFace2, so that any inherited or inline functions could again work when given the pointer to this interface.

OK, now how can someone call the fcn1() from IFace2? What is that non-virtual thunk to RealClass::fcn1(int)?

If you pass your RealClass* pointer to a stranger function which takes IFace2*, the compiler will emit code to increase your pointer by 8 (or however large sizeof(void*) + sizeof(IFace1) is), so that the function gets the pointer which starts with the virtual table pointer of IFace2, then its member fields -- just as agreed in the contract I outlined earlier.

When that function wants to call void IFace2::fcn1(int), it looks into the virtual table, goes to the record of this particular function (the first and only one) and calls it, with this set to the address being passed as pointer to IFace2.

And here arises a problem: If someone invokes this method implemented in RealClass on a RealClass pointer, this points to the base of RealClass. The same with IFace1. But if it is invoked by someone having a pointer to the IFace2 interface, this points 8 (or however many) bytes into the object instead!

So the compiler would need to generate the function multiple times to accomodate this, otherwise it could not access member fields and other methods correctly, as it differs depending on who is calling the method.

Instead of having the code really twice, the compiler optimizes this by creating that hidden implicit small thunk function instead, which just

decreases the this pointer by the proper amount,
calls the real method, which can now work fine regardless of who invoked it.
        
 * --------------------------------
 * --------------------------------
 Basically if D1 pointer is initalized with C1 pointer, and D1 pointer calls C::f9(this), it has to pass the pointer of C! for C::F9 call to work
 * That is why the chunk in C virtual table is generated to adjust this pointer in C::f9(this) call.
 
 */
class B
{
public:
	virtual ~B(){}
	virtual int f1() { return 3; }
	virtual int f2() { return 5; }
	int v1;
};

class D : public B
{
public:
    virtual int f1() { return 0; }	
    virtual void f3() {}
    int v2;
    int v11;
};

class D3 : public D
{
public:
    virtual int f1() { return 0; }	
    virtual void f3() {}
    virtual void f13() {}
    int v21;
    int v111;
};

class D1
{
public:
	virtual void f8() {}
	virtual void f9() {}
    virtual void f5() {}
    virtual void f10() {}
    int v7;
    int v8;
    int v9;
	
};

class D2
{
public:
	virtual void f18() {}
	virtual void f19() {}
    virtual void f15() {}
    virtual void f110() {}
	
};


class C : public D, public D1, public D2
{
public:
	virtual void f4() { v2 = 9; }
        /*
          Basically if D1 pointer is initalized with C1 pointer, and D1 pointer calls C::f9(this), it has to pass the pointer of C! for C::F9 call to work
 * That is why the chunk in C virtual table is generated to adjust this pointer in C::f9(this) call.*/
        virtual void f9()  {}
};

struct B100 {};
struct D100 : B100 {};

#include <vector>

void foo(D100* dp)
{
   B100* bp = dynamic_cast<B100*>(dp); // equivalent to B* bp = dp;
   if (bp == 0)
   {
       //int haha  = 12;
   }
   else
   {
       //int haha = 10;
   }
}

int main()
{
    C c1;
    B* b = new D();
    b->f1();
    b->f2();
    c1.f5();
    D1* d1 = &c1;
    d1->f5();
    D*  d2 = static_cast<D*>(&c1);
    d1->f9();
    d2->f2();
    D1* d3 = new D1();
    C* c2 = static_cast<C*>(d3); // dynamic_cast won't work and C-style cast neither, 
    //however c2 will be half constructed as it only vpointer to D1 will be set by static_cast
    c2->f5();  
    c2->f8();
    c2->f10();
   
   // c2->f9();//tries to call (int (*)(...))C::f9 not not have in D1 vtable!
    D100 d100;
//     const char * name = typeid(d100).name();
    foo(&d100);
    
    std::vector<int> v(12);
    v.push_back(12);
    
   
}

