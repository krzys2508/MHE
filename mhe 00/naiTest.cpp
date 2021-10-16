#include <iostream>
#include <vector>
#include <cstdlib>
#include <list>
#include <string>
#include <algorithm> 
using namespace std;

 // LISTY
// void buildList(int& size,  list <int>& lista ){ 


//  for (int i = 0; i<size; i++){

// int r = rand();
// bool inserted = false;
// for (auto j = begin(lista); j !=end(lista); j++){ //loop that takes the begining gining of the list and end 
// if (*j > r){
//   lista.insert(*j, r); // adds numbers to specfic postition 
//   inserted = true;
//   break;
// }
// }
// if(!inserted){
//   lista.push_back(r); // if not found, adds to the end since the number is the largest 
// }
//  }
// }

int main()
{
  // VEKTORY
  // vector<int> liczby{1, 2, 3, 4, 5};
  // liczby.push_back(6);
  // liczby.push_back(7);
  // liczby.pop_back();
  // int rozmiar = liczby.size();
  // cout << rozmiar << endl;
  // cout << liczby[3] << endl;
  // list <int> finallista;
  // // for (int i : liczby)
  // // {
  // //   cout << i << endl;
  // // }

  // // Lambdas 

  auto isOdd = [](int number){if (number % 2 != 0){
  return true;
  }
  else {
    return false; 
  }
  };
//  cout << isOdd(2)<<endl;
//   cout << isOdd(3)<<endl;

  vector <int> numbers {1,2,3,4,5,-5,-2,-3};

  int odds = count_if(begin(numbers),end(numbers),isOdd);
  int even = count_if(begin(numbers), end(numbers),[](int number) {if (number %2 == 0){
    return true;
  }
  else {
    return false;
  }} ) ;
  
  // cout <<odds<<endl;
  // cout<< even<<endl;

int x = 3;
int y = 7;
string message = "elements between ";
message += to_string(x) + " and " + to_string(y) + " inclusive ";
for_each(begin(numbers), end(numbers),[x,y,&message](int n){
  if (n>=x && n<=y){
    message += " " + to_string(n);
  }
});

int k=0;
int w = 0;
for_each(begin(numbers), end(numbers), [&,k](int n)mutable{
k +=n;
w+=n;
});
// cout<<message;
cout<<k<<endl<<w<<endl;
return 0;
}