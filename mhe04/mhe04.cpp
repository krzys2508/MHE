#include <iostream>
#include <functional>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>

using namespace std;

using my_vector = vector<int>;

using my_result_vector = vector<vector<int>>;

random_device rd;
mt19937 rand_gen(rd());

void printSolution(my_result_vector res)
{
    for (auto a : res)
    {
        cout << "{";
        for (auto b : a)
        {
            cout << b;
        }
        cout << "}";
    }
    cout << endl;
}

void printProblem(my_vector vec)
{
    for (int value : vec)
    {
        cout << value << " ";
    }
    cout << endl;
}

my_vector loadData(string fname)
{
    ifstream inputfile(fname);
    my_vector result;
    string line;
    int value;
    while (inputfile >> value)
    {
        if (value > 0)
        {
            result.push_back(value);
        }
    }
    return result;
}

void saveData(string fileName, my_result_vector v, int a)
{
    ofstream outdata;
    outdata.open(fileName);
    if (!outdata)
    {
        cout << "File cannot be found!";
    }
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            outdata<<v[i][j]<<",";
        }
    }
    outdata<<endl;
    outdata<<a;
    outdata.close();
}

my_result_vector next_solution(my_vector v)
{
    int sum = accumulate(v.begin(), v.end(), 0);
    int size = v.size();
    int numberOfPartitions = size / 3;
    vector<vector<int>> result(numberOfPartitions);
    int partitionIndex = 0;
    if (sum % 3 == 0)
    {
        while (v.size())
        {
            uniform_int_distribution<int> distr(0, v.size() - 1);
            int index = distr(rand_gen);
            int value = v.at(index);
            if (result[partitionIndex].size() < 3)
            {
                result[partitionIndex].push_back(value);
                cout << "number " << value << "has been added to partition number " << partitionIndex << endl;
                v.erase(v.begin() + index);
            }
            else
            {
                cout << "rising partition index " << endl;
                partitionIndex++;
            }
        }
        return result;
    }
    else
    {

        cout << "Cannot be partitioned !!";
    }
}
double goal_function(my_result_vector triplets)
{
    double total = 0;
    vector<int> targets;

    for (int i = 0; i < triplets.size(); i++)
    {
        int target = accumulate(triplets[i].begin(), triplets[i].end(), 0);
        targets.push_back(target);
    }
    total = accumulate(targets.begin(), targets.end(), 0);
    int expectedSumPerPartition = total / triplets.size();

    int final_target = 0;
    double final_score = total;
    int score = 0;
    for (int j = 0; j < targets.size(); j++)
    {
        if (targets[j] != expectedSumPerPartition)
        {
            score += abs(sums[j] - expectedSumPerPartition);
        }
    }
    cout << score << endl;
    return score;
}

vector<my_result_vector> all_permutations(my_result_vector triplets)
{
    vector<my_result_vector> all_permutations;
    for (auto &triplet : triplets)
    {
        sort(triplet.begin(), triplet.end());
        do
        {
            all_permutations.push_back(triplets);
        } while (next_permutation(triplet.begin(), triplet.end()));
    }
    return all_permutations;
}

my_result_vector brute_force(my_result_vector triplets, vector<int> numbers)
{
    vector<my_result_vector> checked_solutions;
    double current_score;
    my_result_vector best_solution;
    double best_score;

    do
    {
        my_result_vector current_triplets = next_solution(numbers);
        if (!(find(checked_solutions.begin(), checked_solutions.end(), current_triplets) != checked_solutions.end()))
        {
            printSolution(current_triplets);
            current_score = goal_function(current_triplets);
            checked_solutions.push_back(current_triplets);

            if (current_score < best_score)
            {
                best_score = current_score;
                best_solution = current_triplets;
            }
            vector<my_result_vector> every_permutation = all_permutations(current_triplets);
            cout << "Printing all permutations of the given set : " << endl;

            for (int w = 0; w < every_permutation.size(); w++)
            {
                printSolution(every_permutation[w]);
                current_score = goal_function(every_permutation[w]);
                checked_solutions.push_back(every_permutation[w]);
            }
        }
    } while (current_score != 0);
    return best_solution;
}

int main(int argc, char **argv)
{

    string directory = "C:\\Users\\krzys\\Desktop\\szkola\\MHE\\";
    auto problem = loadData( argv[1]);
    cout << "Values uploaded: " << endl;
    printProblem(problem);
    my_result_vector random_solution = next_solution(problem);
    cout << endl;
    my_result_vector best_solution = brute_force(random_solution, problem);
    cout << "Best found solution: ";
    printSolution(best_solution);
    int goal = goal_function(best_solution);
    cout << "With score of : "<<goal;
    saveData("results.txt", best_solution, goal);
}
