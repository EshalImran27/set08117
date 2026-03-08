#include<iostream>
using namespace std;

typedef struct{
    int age;
    char matric[10];
    float marks;
} student;
int main(){
    int integer_variable;
    char character_variable;
    double double_variable;
    float float_variable;
    string string_variable;
    size_t size;
    size = sizeof(integer_variable);
    cout<<"size of an integer variable "<<size<<"\n";
    size = sizeof(character_variable);
    cout<<"size of a character variable "<<size<<"\n";
    size = sizeof(double_variable);
    cout<<"size of a double variable "<<size<<"\n";
    size = sizeof(float_variable);
    cout<<"size of a float variable "<<size<<"\n";
    size = sizeof(string_variable);
    cout<<"size of a string variable "<<size<<"\n";

    float marks[3]={14.3,12.3,10.2};
    cout<<marks[0]<<"\n";
    cout<<"size of array "<<sizeof(marks)<<"\n";
    int a[100];
    cout<<"size of integer array "<<sizeof(a)<<"\n";
    cout<<"space for storing the elements is "<<sizeof(a)/sizeof(int)<<"\n";

    student s1={20, "123", 12.4};
    cout<<"age: "<<s1.age<<" matriculation number: "<<s1.matric<<" marks: "<<s1.marks<<"\n";
    cout<<"size of elements in struct\n";
    cout<<"age: "<<sizeof(s1.age)<<" matric: "<<sizeof(s1.matric)<<" marks: "<<sizeof(s1.marks)<<"\n";
    cout<<"size of struct: "<<sizeof(s1)<<"\n";
}