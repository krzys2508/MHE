#include <iostream>
#include <functional>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <set>
#include <list>
#include <math.h>
#include <map>
#define M_PI 3.1415926535

using namespace std;

using my_vector = vector<int>;

using my_result_vector = vector<vector<int>>;

random_device rd;
mt19937 gen(rd());

void printSolution(my_result_vector res)
{
    for (auto a : res)
    {
        cout << "{";
        for (auto b : a)
        {
            cout << b << ",";
        }
        cout << "}";
    }
    cout << endl;
}


ostream& operator << (ostream& o, vector<double>v) {
    for (auto e : v) {
        o << e << ",";

    }
    return o;
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

double goal (vector <double> x){
    double result = 0;
    int sigma = 0;
    int it = x.size();

    for (int i = 0; i<it;i++){
        sigma += pow(x[i],2)-(10*cos(2*(M_PI*x[i])));
}
result = (10*it)+sigma;
}
// double goal (vector <double>x){
//     double result =0;
//     int iterations  = x.size()-1;
//     for (int i =0;i<iterations;i++){
//         result+= 100 * pow(x[i+1] - pow(x[i],2),2)+pow(1-x[i],2);
//     }
//     return result;
// }
auto domain = [](vector<double> v) {
    int score =0;
    for (int i = 0;i<v.size();i++){
        if (abs(v[i])<=5.12){ 
        score +=0;
        }
        else {
            score +=1;
        }
    }
    if (score == 0){
        return true;
    }
    else {
        return false;
    }
};

vector<double> generate_random_neighbour(vector<double> current_numbers){
    uniform_int_distribution<int> random_index(0, current_numbers.size()-1);
    uniform_int_distribution<int> random_decision(0,1);
    int n;
    if(random_decision(gen)==1){
        n = -1;
    } else {
        n = 1;
    }
    current_numbers[random_index(gen)] += n;
    vector<double> neighbour_numbers = current_numbers;
    return neighbour_numbers;
}
vector<double> simulated_annealing(
    function<double(vector<double>)> f,
    function<bool(vector<double>)> f_domain,
    vector<double> p0,
    int iterations,
    function<vector<double>(vector<double>)> N,
    function<double(int,double)> T,
    double a,
    ofstream&out)
{
    auto s_current = p0;
    auto s_global_best = p0;
    uniform_real_distribution<> u_k(0.0, 1.0);

    if (!f_domain(s_current)) throw std::invalid_argument("The p0 point must be in domain");

    for (int k = 0; k < iterations; k++) {
        auto s_next = N(s_current);
        if (!f_domain(s_next)) continue;
        if (f(s_next) < f(s_current)) {
            s_current = s_next;
        } else {
            double u = u_k(gen);
            if (u < exp(-abs(f(s_next) - f(s_current)) / T(k,a))) {
                s_current = s_next;
            } 
        }
        if (f(s_current) < f(s_global_best)){
            s_global_best = s_current;
        }
        out <<"iterations: "<<iterations <<" "<< s_global_best << " " << f(s_global_best) << endl;
}
return s_global_best;
}
double temp_1 (int it,double a){
return 1/it;
}
double temp_2 (int it,double a){
return 1/log10(it);
}
double temp_3 (int it,double a){
return pow(a,it);
}

map<string, string> args_to_map(vector<string> arguments)
{
    map<string, string> ret;
    string argname = "";
    for (auto param : arguments) {
        if ((param.size() > 2) && (param.substr(0, 2) == "--")) {
            argname = param.substr(2);
        } else {
            ret[argname] = param;
        }
    }
    return ret;
}
int main (int argc, char** argv){
     vector <double> x = {2,0,5};
     ofstream out("resultstest.txt");
    map<string, string> parameters = {
        {"temp", "1"},
        {"iterations","100"},
        {"a","0"},
        };
    for (auto [k, v] : args_to_map(vector<string>(argv, argv + argc))) {
        parameters[k] = v;
    }
    int temp = stoi(parameters["temp"]);
    int iterations = stoi(parameters["iterations"]);
    double a = stod(parameters["a"]);
    if (parameters["temp"]=="1"){
   vector<double> result = simulated_annealing(goal,domain,x,iterations,generate_random_neighbour,temp_1,a,out);
}
    else if (parameters["temp"]=="2"){
   vector<double> result = simulated_annealing(goal,domain,x,iterations,generate_random_neighbour,temp_2,a,out);
}
    else if(parameters["temp"]=="3"){
   vector<double> result = simulated_annealing(goal,domain,x,iterations,generate_random_neighbour,temp_3,a,out);
    }
    return 0;
}
