#include <iostream>
#include <vector>
#include <climits>

void read_data(std::vector<int> &data, size_t &element_number)
{
    data.reserve(element_number);

    for (size_t i = 0; i < element_number; ++i)
    {
        int input_value;
        std::cin >> input_value;
        data.push_back(input_value);
    }
}

void count_LIS(std::vector<size_t> &index, const std::vector<int> &data, const size_t element_number)
{
    std::vector<int> ending(element_number + 1, INT_MAX);
    std::vector<int> previous(element_number, INT_MAX);

    index.clear();
    ending[0] = INT_MIN;
    previous[0] = 0;

    for (size_t i = 0; i < element_number; ++i)
    {
        size_t current_pos = (std::lower_bound(ending.begin(), ending.end(), data[i]) - ending.begin());
        if (ending[current_pos - 1] < data[i] && ending[current_pos] > data[i])
        {
            ending[current_pos] = data[i];
            previous[current_pos] = i;
        }
    }

    size_t i = 1;
    while (previous[i] != INT_MAX)
    {
        index.push_back(previous[i]);
        i++;
    }
}

void write_answer(const std::vector<size_t> &answer)
{
    for (auto result:answer)
    {
        std::cout << result << ' ';
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    size_t element_number;
    std::vector<int> data;

    std::cin >> element_number;
    read_data(data, element_number);

    std::vector<size_t> answer;

    count_LIS(answer, data, element_number);
    write_answer(answer);

    return 0;
}
