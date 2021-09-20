
#include <memory>
#include <iostream>
using namespace std;

class Entity
{
	public:
		Entity() {  cout << "Created Entity!" << endl; }
		~Entity() {  cout << "Destroyed Entity!" << endl; }
                void Print() { cout << "Mardi Gras!" << endl; }
};

int main()
{

	unique_ptr<Entity> entity(new Entity());
        entity -> Print();

        shared_ptr<Entity> sharedEntity = make_shared<Entity>();
        shared_ptr<Entity> e0 = sharedEntity;

	return 0;
}
