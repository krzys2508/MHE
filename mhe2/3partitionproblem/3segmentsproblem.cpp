#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool part( vector<int> &start, vector<vector<int>> &partition)
{

    int sumaStart = accumulate(start.begin(), start.end(), 0);
    if (sumaStart % 3 != 0)
    
        return false;
    }
    int expectedSizePerPartition = sumaStart / 3;
    int startSize = start.size();
    int numberOfPartitions = expectedSizePerPartition/3;

    vector<int> sumOfPartition(numberOfPartitions, 0);
    vector<int> numberOfPart(startSize);

    sort(start.begin(), start.end(), greater<int>());

    int partitionIndexInStart = 0;
    int partitionIndexToBePlacedIn = 0;

    while (partitionIndexInStart < startSize)
    {
        int value = start[partitionIndexInStart];
        while (partitionIndexToBePlacedIn < numberOfPartitions)
        {
            if (sumOfPartition[partitionIndexToBePlacedIn] + value <= expectedSizePerPartition && partition[partitionIndexToBePlacedIn].size()<=3)
            {
                partition[partitionIndexToBePlacedIn].push_back(value);
                sumOfPartition[partitionIndexToBePlacedIn] += value;
                numberOfPart[partitionIndexInStart] = partitionIndexToBePlacedIn;
                partitionIndexToBePlacedIn = 0;
                partitionIndexInStart++;
                break;
            }
            else {
                partitionIndexToBePlacedIn++;
            }
        }
    }
    if (partitionIndexToBePlacedIn == 3)
    {
        partitionIndexInStart--;
        if (partitionIndexInStart < 0)
        {
            return false;
        }
        partitionIndexToBePlacedIn = numberOfPart[partitionIndexInStart];
        partition[partitionIndexToBePlacedIn].pop_back();
        sumOfPartition[partitionIndexToBePlacedIn] -= start[partitionIndexInStart];
        partitionIndexToBePlacedIn++;
    }
    return true;
}


template<typename T> ostream &operator << ( ostream &strm, const vector<T> &V )
{
   for ( auto e : V ) strm << e << " ";
   return strm;
}

int main(){
// vector <int> results = loadData("3segments.txt");

// for (auto part : results) 
//         cout<<part<<endl;

vector <int> testData = {1,3,5,4,2,3};
    vector<vector<int>> wynik;
    bool works = part(testData, wynik);

    if (works ==true){
        for (auto &partycja : wynik) 
        cout<<partycja<<endl;
    }
    else {
        cout<<"Cannot be partitioned";
    }
    return 0;
}
        