
#include <memory>
#include <iostream>
using namespace std;

class Foo
{
	public:
		 void doSomething() { cout << "Mardi Gras!" << endl; }
};

class Bar
{
	private:
       		shared_ptr<Foo> pFoo;
	public:
		Bar()
		{
			pFoo = shared_ptr<Foo>(new Foo());
                        cout << "Created a Bar" << endl;
		}

                ~Bar() { cout << "Destroyed a Bar" << endl; }

		shared_ptr<Foo> getFoo()
		{
			return pFoo;
		}
};

void SomeAction();
void SomeOtherAction(shared_ptr<Bar> pBar);

int main()
{
	shared_ptr<Bar> pBar = shared_ptr<Bar>(new Bar()); 

	SomeAction();
        SomeOtherAction(pBar);

	return 0;
}

void SomeAction()
{
	Bar* pBar = new Bar();  //reference counter = 1

        //a copy of the shared pointer is created
	shared_ptr<Foo> pFoo = pBar->getFoo(); //reference counter = 2

	pFoo->doSomething();

	delete pBar;  //reference counter = 1

	return;  //reference counter = 0
}

void SomeOtherAction(shared_ptr<Bar> pBar)
{
	shared_ptr<Foo> pFoo = pBar -> getFoo();  // a copy of the shared pointer is created
        // reference counter = 2

	pFoo -> doSomething();

	return;
        // reference counter = 1
}
