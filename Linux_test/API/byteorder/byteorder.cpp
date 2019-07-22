#include<iostream>

using namespace std;

void typeorder(){
  union
  {
    short value;
    char union_bytes[sizeof(short)];
  }test;
  test.value = 0x0102;
  if((test.union_bytes[0] == 1)
     && (test.union_bytes[1] == 2) )
  {
    cout << "Big endian" << endl;
  }
  else if((test.union_bytes[0] == 2)
      && test.union_bytes[1] == 1)
  {
    cout << "Little endian" << endl;
  }
  else{
    cout << "Unkonwn..." << endl;
  }
}
int main(){
  typeorder();
  return 0;
}
