#include <iostream>
#include <string>
using namespace std;

class Student
{
private:
  string name;
  uint8_t age;
  string school;
public:
  Student(string name ,uint8_t age,string school):name(name),age(age),school(school){}
  Student(const Student &other){
      name = other.name;
      age = other.age;
      school = other.school;
  }
  ~Student(){}
  friend void printStudent(const Student & student);
};

void printStudent(const Student & student)
{
    cout<<"Name = "<<student.name<<" Age = "<<unsigned(student.age)<<" School = "<<student.school<<endl;
}