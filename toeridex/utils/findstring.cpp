/*
        npos: means until the end of the string
*/

#include <iostream> 
#include <string> 
  
using namespace std; 
  
int main() 
{ 
    // string memo = "init ridex for driver";
    // string memo = "init ridex for commuter";
    string memo = "buy 1 ride";
    // string memo = "sell 2 rides";
    
    // string str1 = "driver"; 
    // string str1 = "commuter"; 
    string str1 = "buy"; 
    // string str1 = "sell"; 
  
    size_t found = memo.find(str1); 

    if (found != string::npos) 
        cout << "First occurrence is " << found << endl; 
    else {
      printf("not found\n");
    }
    // // Find next occurrence of "geeks". Note here we pass 
    // // "geeks" as C style string. 
    // char arr[] = "geeks"; 
    // found = memo.find(arr, found+1); 
    // if (found != string::npos) 
    //     cout << "Next occurrence is " << found << endl; 
  
    return 0; 
} 