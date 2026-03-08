 #include<iostream>
 using namespace std;

 #define MAX 5
 void display(int array[]){
    for(int i=0;i<MAX;i++){
        cout<<array[i]<<" ";
    }
    cout<<"\n";
 }
 void init(int array[]){
    for(int i=0;i<MAX;i++){
        array[i]=i;
    }
 }
 void insert(int* array, int position, int num){
    int i;
    for(i=MAX-1;i>=position;i--){
        array[i]=array[i-1];
    }
    array[i]=num;
 }
 void del(int* array, int pos){
    int i;
    for(i=pos;i<MAX;i++){
        array[i-1]=array[i];
    }
    array[i-1]=0;
 }


 int main(){
    int array[MAX];
    init(array);
    display(array);
    insert(array,3,19);
    display(array);
    del(array, 2);
    display(array);
 }
