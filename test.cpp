#include <iostream>
#include <string>
#include "StringList.h"

using std::string;
using std::cout;
using std::endl;



int main(){

StringList * list =new StringList();
StringList * list2 =new StringList();


list->insertBack("dog");
list->insertBack("cat");
list->insertBack("aaa");

list->print();
cout<<"\n";

list2->insertBack("yellow");
list2->insertBack("green");
list2->insertBack("red");
list2->insertBack("orange");
list2->print();
cout <<"\n";


list=list2;

cout<<"printing list = list2\n";
list->print();
cout <<"\n";

list2->print();
cout <<"\n";



cout<<"undoing list1\n";
list->undo();
list->print();
cout<<"\n";

list2->print();
cout <<"\n";













// cout<<"removing all\n";
// list->removeAll();
// list->print();
// cout <<"\nUndo\n";
// list->undo();
// list->print();
// *list = *list2;
// list->print();
// list->undo();
// list->print();



//assignement






}