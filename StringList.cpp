#include "StringList.h"
#include <stdexcept>
#include <iostream>
using std::cout;
using std::endl;
using std::out_of_range;

// Default constructor - makes an empty list of capacity 10
StringList::StringList()
{
	capacity = 10;
	n = 0;
	arr = new string[capacity];
	stack = new undostack();
}

// Copy constructor
StringList::StringList(const StringList &other)
{
	copyList(other);
}

// ***UNDOABLE
// Overloaded assignment operator
StringList &StringList::operator=(const StringList &other)
{
	for (int i = 0; i < n; i++)
	{
		stack->insert("Assignment", i, arr[i]);		//inserts each item into the stack one by one.
	}
	string *array = other.arr;		//makes an array pointing to other array.
	for (int i = 0; i < other.size(); i++)
	{
		checkCapacity();
		arr[i] = array[i];			//sets the stringlist array to the other array.
	}

	if (&other != this)
	{
		delete[] arr;
		copyList(other);
	}
	return *this;
}

// Destructor
StringList::~StringList()
{
	delete[] arr;
}

// ACCESSORS
// Returns the number of elements in the list
int StringList::size() const
{
	return n;
}

// Returns true if the list is empty, false otherwise
bool StringList::empty() const
{
	return n == 0;
}

// Returns the string at the given index
string StringList::get(int i) const
{
	checkBounds(i, "get");
	return arr[i];
}

// Returns the index of the first occurrence of the given string
int StringList::index(string s) const
{
	int i = 0;
	while (i < n && s != arr[i])
	{
		i++;
	}
	if (i == n)
	{
		return -1;
	}
	else
	{
		return i;
	}
}

// Returns true iff the given string is in the list
bool StringList::contains(string str) const
{
	return !(index(str) == -1);
}

// Returns true if the two lists are equal, false otherwise.
// Does *not* consider any undo information when comparing two Stringlists. All
// that matters is that they have the same strings in the same order.
bool StringList::operator==(const StringList &sl) const
{
	if (size() != sl.size())
	{
		return false;
	}
	for (int i = 0; i < size(); i++)
	{
		if (get(i) != sl.get(i))
		{
			return false;
		}
	}
	return true;
}

// Returns true if the two lists are not equal, false otherwise
bool StringList::operator!=(const StringList &sl) const
{
	return !(*this == sl);
}

// Returns a string representation of the list.
string StringList::toString() const
{
	string result = "{";
	for (int i = 0; i < size(); i++)
	{
		if (i > 0)
		{
			result += ", ";
		}
		result += "\"" + get(i) + "\"";
	}
	return result + "}";
}

// MUTATORS

// ***UNDOABLE
// Sets the value at the given index
void StringList::set(int i, string str)
{
	checkBounds(i, "set");
	string temp;
	temp = arr[i];
	arr[i] = str;

	stack->insert("Set", i, temp);		// inserts set into the stack.
}

// ***UNDOABLE
// Inserts the given string *before* the given index
void StringList::insertBefore(int pos, string str)
{
	// Doesn't use checkBounds because it's okay to insert at the end
	if (pos < 0 || pos > size())
	{
		throw out_of_range("StringList::insertBefore index out of bounds");
	}
	checkCapacity();
	for (int i = n; i > pos; i--)
	{
		arr[i] = arr[i - 1];
	}
	arr[pos] = str;
	n++;

	stack->insert("Remove", pos, str);  //inserts remove into the stack.
}

// ***UNDOABLE
// Inserts the given string at the front of the list
void StringList::insertFront(string str)
{
	checkCapacity();
	insertBefore(0, str);

	stack->insert("Remove", 0, str);	//inserts remove into the stack.
}

// ***UNDOABLE
// Inserts the given string at the back of the list
void StringList::insertBack(string str)
{
	checkCapacity();
	insertBefore(n, str);
	stack->insert("Remove", n, str);		//inserts remove into the stack with position and value.
}

// ***UNDOABLE
// Removes the element at the given index and moves elements after it down
void StringList::remove(int pos)
{
	checkBounds(pos, "remove");
	stack->insert("Insert", pos, arr[pos]); //inserts "insert" into the stack.

	for (int i = pos; i < n; i++)
	{
		arr[i] = arr[i + 1];
	}

	n--;
}

// ***UNDOABLE
// Empties the list
void StringList::removeAll()
{
	for (int i = 0; i < n; i++)
	{

		stack->insert("Insertall", i, arr[i]); // inserts "Insertall" into the stack but also the index and the value.
		arr[i] = "";
	}
	n = 0;
}

// Undoes the last operation that modified the list
void StringList::undo()
{
	// TO DO
	Undo lastoperation = stack->pop();		//assigns lastoperation to the values that were inserted to the top stack.

	if (lastoperation.undoarray == "Set")
	{
		this->arr[lastoperation.index] = lastoperation.value;		//this undos the set operation to its previous value.
	}

	else if (lastoperation.undoarray == "Remove")
	{
		for (int i = lastoperation.index; i < this->n; i++)		//this undos insert 
		{
			this->arr[i] = this->arr[i + 1];
		}
		this->n--;
	}

	else if (lastoperation.undoarray == "Insert")
	{

		for (int i = this->n; i > lastoperation.index; i--) 	//this undos remove
		{
			this->arr[i] = this->arr[i - 1];
		}
		this->arr[lastoperation.index] = lastoperation.value;
		this->n++;
	}

	else if ((lastoperation.undoarray == "Insertall"))
	{

		while (lastoperation.undoarray == "Insertall")     // this undos remove all
		{
			this->n++;
			arr[lastoperation.index] = lastoperation.value;
			lastoperation = stack->pop();
		}
		stack->insert(lastoperation.undoarray, lastoperation.index, lastoperation.value);
	}
	else if (lastoperation.undoarray == "Assignment")			// this undos the assignment operator to the previous array.
	{
		this->capacity = lastoperation.index;
		this->n = 0;
		while (lastoperation.undoarray == "Assignment")
		{
			this->n++;
			arr[lastoperation.index] = lastoperation.value;
			lastoperation = stack->pop();
		}
		stack->insert(lastoperation.undoarray, lastoperation.index, lastoperation.value);
	}
}

// Prints the list
void StringList::print() const
{
	for (int i = 0; i < n; i++)
	{
		cout << arr[i] << endl;
	}
}

// Helper function for checking index bounds.
void StringList::checkBounds(int i, string s) const
{
	if (i < 0 || i >= size())
	{
		throw out_of_range("StringList::" + s + " index out of bounds");
	}
}

// POST: Doubles the capacity of the list if n == capacity
void StringList::checkCapacity()
{
	if (n == capacity)
	{
		capacity *= 2;
		string *temp = new string[capacity];

		for (int i = 0; i < n; i++)
		{
			temp[i] = arr[i];
		}
		delete[] arr;
		arr = temp;
	}
}

// Helper function to copy the contents of another list
void StringList::copyList(const StringList &lst)
{
	capacity = lst.capacity;
	n = lst.n;
	arr = new string[capacity];

	for (int i = 0; i < n; i++)
	{
		arr[i] = lst.arr[i];
	}
}

StringList::undostack::undostack()
{

	undocapacity = 4;
	num = 0;
	array = new Undo[undocapacity];
}

StringList::undostack::~undostack()
{

	delete[] array;
}

void StringList::undostack::insert(string str, int pos, string value)
{

	if (num == undocapacity)
	{
		undocapacity *= 2;
		Undo *newArray = new Undo[undocapacity];
		for (int i = 0; i < num; i++)
		{
			newArray[i] = array[i];
		}
		delete[] array;
		array = newArray;
	}
	array[num] = Undo(str, pos, value);
	num++;
}

StringList::Undo StringList::undostack::pop()
{

	Undo temp;
	num--;
	temp = array[num];

	return temp;
}

StringList::Undo::Undo(string operation, int pos, string value)
{

	this->undoarray = operation;
	this->index = pos;
	this->value = value;
}

StringList::Undo::Undo()
{
}
