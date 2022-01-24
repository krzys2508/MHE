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
#include <map>

using namespace std;
using my_vector = vector<int>;
using my_result_vector = vector<vector<int>>;

// random_device rd;
// mt19937 rand_gen(rd());
std::mt19937 rand_gen(time(nullptr));

bool test_for_triplets(my_vector multiset)
{
    if (multiset.size() % 3 != 0)
    {
        cout << "Multiset can't be divided into triplets.";
        return false;
    }
    return true;
}
int test_for_target(my_vector multiset)
{
    test_for_triplets(multiset);
    int number_of_triplets = multiset.size() / 3;

    int sum_of_numbers = 0;
    for (int n : multiset)
    {
        sum_of_numbers += n;
    }

    int target = sum_of_numbers / number_of_triplets;
    cout << "Your target is: " << target << "\n";

    return target;
}

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
my_result_vector next_solution(my_vector v)
{
    int sum = accumulate(v.begin(), v.end(), 0);
    int size = v.size();
    int numberOfPartitions = size / 3;
    my_result_vector result(numberOfPartitions);
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
                // cout << "number " << value << "has been added to partition number " << partitionIndex << endl;
                v.erase(v.begin() + index);
            }
            else
            {
                // cout << "rising partition index " << endl;
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
    double full_score = 0;
    vector<int> targets;

    for (auto triplet : triplets)
    {
        int triplet_target = accumulate(triplet.begin(), triplet.end(), 0);
        full_score += triplet_target;
        targets.push_back(triplet_target);
    }

    int final_target = 0;
    double final_score = full_score;

    for (auto target : targets)
    {
        double current_score = full_score;
        for (auto triplet : triplets)
        {
            if (accumulate(triplet.begin(), triplet.end(), 0) == target)
            {
                current_score -= target;
            }
        }
        if (current_score < final_score)
        {
            final_score = current_score;
            final_target = target;
        }
    }

    //    cout << "->Current target: " << final_target << " ->Score: " << final_score <<"\n";
    return final_score;
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

my_result_vector brute_force(
    vector<int> numbers,
    function<void(int c, double dt)> on_statistics = [](int c, double dt) {},
    function<void(int i, double current_goal_val, double goal_val)>
        on_iteration = [](int i, double current_goal_val, double goal_val) {})
{
    auto start = chrono::steady_clock::now();

    vector<my_result_vector> checked_solutions;
    my_result_vector best_solution;
    double score_check;
    double best_score = accumulate(numbers.begin(), numbers.end(), 0);
    int stat_goal_function_calls = 0;

    int iteration_limit = 1;
    for (int i = numbers.size(); i >= 1; i--)
    {
        iteration_limit *= i;
    }

    int iteration = 1;

    do
    {
        my_result_vector current_triplets = next_solution(numbers);
        if (!(find(checked_solutions.begin(), checked_solutions.end(), current_triplets) != checked_solutions.end()))
        {
            cout << stat_goal_function_calls + 1 << ") ";
            iteration++;
            stat_goal_function_calls++;
            printSolution(current_triplets);
            score_check = goal_function(current_triplets);
            cout << " =>Score: " << score_check << "\n";
            checked_solutions.push_back(current_triplets);

            if (score_check < best_score)
            {
                best_score = score_check;
                best_solution = current_triplets;
            }

            for (auto permutation : all_permutations(current_triplets))
            {
                iteration++;
                checked_solutions.push_back(permutation);
            }

            on_iteration(stat_goal_function_calls, score_check, best_score);
        }
    } while (score_check != 0 && iteration < iteration_limit);
    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    on_statistics(stat_goal_function_calls, duration.count());

    return best_solution;
}

my_result_vector generate_random_neighbour(my_result_vector current)
{
    uniform_int_distribution<int> distr_all(0, current.size() - 2);
    uniform_int_distribution<int> distr_singular(0, 2);
    int index_set = distr_all(rand_gen);
    int index_triplet_1 = distr_singular(rand_gen);
    int index_triplet_2 = distr_singular(rand_gen);
    swap(current[index_set][index_triplet_1], current[index_set + 1][index_triplet_2]);
    return current;
}

vector<my_result_vector> generate_all_neighbours(my_result_vector current_triplets)
{
    vector<my_result_vector> all_neighbours;
    int number_of_triplets = current_triplets.size();
    int number_of_swaps = (number_of_triplets - 1) * 3;

    for (int i = 0; i < number_of_triplets; i++)
    {
        int second_triplet_index = 0;
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < number_of_swaps; k++)
            {
                int cop = k;
                int second_element_index = cop % 3;
                my_result_vector buffer = current_triplets;
                if (second_triplet_index != i)
                {
                    swap(buffer[i][j], buffer[second_triplet_index][second_element_index]);
                    if (!(find(all_neighbours.begin(), all_neighbours.end(), buffer) != all_neighbours.end()))
                    {
                        all_neighbours.push_back(buffer);
                    }
                }
                else
                {
                    k--;
                    second_triplet_index++;
                }
            }
        }
    }
    return all_neighbours;
}

my_result_vector hill_climb_stochastic(
    vector<int> numbers, int N,
    function<void(int c, double dt)> on_statistics = [](int c, double dt) {},
    function<void(int i, double current_goal_val, double goal_val)>
        on_iteration = [](int i, double current_goal_val, double goal_val) {})
{

    auto start = chrono::steady_clock::now();

    my_result_vector best_solution = next_solution(numbers);
    double best_score = accumulate(numbers.begin(), numbers.end(), 0);

    double score_check;

    my_result_vector current_triplets = best_solution;

    for (int i = 0; i < N; i++)
    {
        current_triplets = generate_random_neighbour(best_solution);
        cout << i + 1 << ") ";
        printSolution(current_triplets);
        score_check = goal_function(current_triplets);
        cout << " =>Score: " << score_check << "\n";

        if (score_check < best_score)
        {
            best_solution = current_triplets;
            best_score = score_check;
        }

        on_iteration(i, score_check, best_score);
    }

    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    on_statistics(N, duration.count());

    return best_solution;
}

my_result_vector hill_climb(
    vector<int> numbers, int N,
    function<void(int c, double dt)> on_statistics = [](int c, double dt) {},
    function<void(int i, double current_goal_val, double goal_val)>
        on_iteration = [](int i, double current_goal_val, double goal_val) {})
{

    auto start = chrono::steady_clock::now();

    my_result_vector best_solution = next_solution(numbers);
    double best_score = goal_function(best_solution);

    double score_check;

    for (int i = 0; i < N; i++)
    {
        auto all_neighbours = generate_all_neighbours(best_solution);
        for (auto neighbour : all_neighbours)
        {
            score_check = goal_function(neighbour);
            if (score_check < best_score)
            {
                best_solution = neighbour;
                best_score = score_check;
            }
        }
        cout << i + 1 << ") ";
        printSolution(best_solution);
        cout << " =>Score: " << best_score << "\n";

        on_iteration(i, score_check, best_score);
    }

    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    on_statistics(N, duration.count());

    return best_solution;
}

my_result_vector tabu_search(
    vector<int> numbers, int N, int tabu_size,
    function<void(int c, double dt)> on_statistics = [](int c, double dt) {},
    function<void(int i, double current_goal_val, double goal_val)>
        on_iteration = [](int i, double current_goal_val, double goal_val) {})
{

    auto start = chrono::steady_clock::now();

    my_result_vector best_solution = next_solution(numbers);
    double best_score = goal_function(best_solution);

    set<vector<vector<int>>> tabu_set;
    list<vector<vector<int>>> tabu_list;

    double score_check;

    auto is_in_tabu = [&](auto e)
    {
        return tabu_set.count(e);
    };

    auto add_to_tabu = [&](auto e)
    {
        tabu_set.insert(e);
        tabu_list.push_back(e);
    };

    auto shrink_taboo = [&]()
    {
        if (tabu_set.size() > tabu_size)
        {
            tabu_set.erase(tabu_list.front());
            tabu_list.pop_front();
        }
    };

    auto current_triplets = best_solution;

    for (int i = 0; i < N; i++)
    {

        auto current_neighbours = generate_all_neighbours(best_solution);

        current_neighbours.erase(remove_if(current_neighbours.begin(), current_neighbours.end(), [&](auto e)
                                           { return is_in_tabu(e); }),
                                 current_neighbours.end());

        if (current_neighbours.size() == 0)
            break;

        current_triplets = *min_element(current_neighbours.begin(), current_neighbours.end(), [&](auto a, auto b)
                                        { return goal_function(a) > goal_function(b); });

        add_to_tabu(current_triplets);

        score_check = goal_function(current_triplets);

        if (goal_function(current_triplets) < goal_function(best_solution))
        {
            best_solution = current_triplets;
            best_score = score_check;
        }

        cout << i + 1 << ") ";
        printSolution(current_triplets);
        cout << " =>Score: " << score_check << "\n";

        on_iteration(i, score_check, best_score);

        shrink_taboo();
    }

    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;

    on_statistics(N, duration.count());

    return best_solution;
}

map<string, string> args_to_map(vector<string> arguments)
{
    map<string, string> ret;
    string argname = "";
    for (auto param : arguments)
    {
        if ((param.size() > 2) && (param.substr(0, 2) == "--"))
        {
            argname = param.substr(2);
        }
        else
        {
            ret[argname] = param;
        }
    }
    return ret;
}

vector<int> generate_random_problem(int number_of_triplets)
{
    vector<int> problem_set;

    uniform_int_distribution<int> random(1, 30);

    for (int i = 0; i < number_of_triplets * 3; i++)
    {
        problem_set.push_back(random(rand_gen));
    }

    int sum = accumulate(problem_set.begin(), problem_set.end(), 0);
    int control = sum % number_of_triplets;

    while (control != 0)
    {
        problem_set[random(rand_gen)] -= 1;
        sum = accumulate(problem_set.begin(), problem_set.end(), 0);
        control = sum % number_of_triplets;
    }

    return problem_set;
}

double temperature(int k)
{
    return 1000 / k;
}

my_result_vector simulated_annealing(
    vector<int> p0,
    int iterations,
    function<double(int)> T,
    function<void(int c, double dt)> on_statistics = [](int c, double dt) {},
    function<void(int i, double current_goal_val, double goal_val)>
        on_iteration = [](int i, double current_goal_val, double goal_val) {})
{

    auto start = chrono::steady_clock::now();

    my_result_vector current_triplets = next_solution(p0);
    my_result_vector best_triplets = current_triplets;
    int stat_goal_function_calls = 0;

    uniform_real_distribution<> u_k(0.0, 1.0);

    for (int i = 0; i < iterations; i++)
    {
        my_result_vector neighbour_solution = generate_random_neighbour(current_triplets);
        if (goal_function(neighbour_solution) < goal_function(current_triplets))
        {
            current_triplets = neighbour_solution;
        }
        else
        {
            double u = u_k(rand_gen);
            if (u < exp(-abs(goal_function(neighbour_solution) - goal_function(current_triplets)) / T(i)))
            {
                current_triplets = neighbour_solution;
            }
            else
            {
            }
        }
        if (goal_function(current_triplets) < goal_function(best_triplets))
        {
            best_triplets = current_triplets;
        }
        stat_goal_function_calls++;
        cout << i + 1 << ") ";
        printSolution(current_triplets);
        cout << " score: " << goal_function(current_triplets) << endl;

        on_iteration(stat_goal_function_calls, goal_function(current_triplets), goal_function(best_triplets));
    }

    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    on_statistics(stat_goal_function_calls, duration.count());

    return best_triplets;
}

int main(int argc, char **argv)
{

    map<string, string> parameters = {
        {"size", "3"},
        {"iterations", "1000"},
        {"method", "brute_force"},
        {"print_result", "false"},
        {"tabu_size", "50"}};

    for (auto [k, v] : args_to_map(vector<string>(argv, argv + argc)))
    {
        parameters[k] = v;
    }

    my_vector problem = generate_random_problem(stoi(parameters["size"]));

    printProblem(problem);
    cout << endl;

    int iterations = stoi(parameters["iterations"]);

    my_result_vector best_solution;

    auto on_finish =
        [](int c, double dt)
    {
        cout << "# count: " << c << "; dt:  " << dt << endl;
    };

    auto on_step = [&](int i, double current_goal_val, double goal_v)
    {
        cout << i << " " << current_goal_val << " " << goal_v << endl;
    };

    if (parameters["method"] == "brute_force")
    {
        best_solution = brute_force(
            problem,
            on_finish,
            on_step);
    }
    else if (parameters["method"] == "hill_climb_stochastic")
    {
        best_solution = hill_climb_stochastic(
            problem,
            iterations,
            on_finish,
            on_step);
    }
    else if (parameters["method"] == "hill_climb")
    {
        best_solution = hill_climb(
            problem,
            iterations,
            on_finish,
            on_step);
    }
    else if (parameters["method"] == "tabu_search")
    {
        best_solution = tabu_search(
            problem,
            iterations,
            stoi(parameters["tabu_size"]),
            on_finish,
            on_step);
    }
    else if (parameters["method"] == "simulated_annealing")
    {
        best_solution = simulated_annealing(problem,
                                            iterations,
                                            temperature,
                                            on_finish,
                                            on_step);
    }
    cout << "# best " << parameters["method"] << " result: " << goal_function(best_solution) << endl;

    return 0;
}