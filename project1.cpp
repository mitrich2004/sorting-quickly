#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
using namespace std;

void insertion_sort(vector<int>& list, int start, int end);
void quick_sort(vector<int>& list, int start, int end);
vector<int> merge(vector<int>& left, vector<int>& right);
vector<int> merge_sort(vector<int>& list);
void fix_heap(vector<int>& list, int start, int end);
void heap_sort(vector<int>& list, int start, int end);
void hybrid_sort(vector<int>& list, int start, int end);

int main()
{
    //data harvesting parameters
    int maxSize = 10000;
    int sizeStep = 100;
    int maxTimes = 100;
    
    //proper randomisation
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(-maxSize / 2, maxSize / 2);

    //time measurement
    using clock = chrono::steady_clock;
    using ns = chrono::nanoseconds;

    //record file
    ofstream file_out;
    file_out.open("hybrid_sort.txt");

    for (int size = sizeStep; size <= maxSize; size += sizeStep)
    {
        ns total_time_taken = ns(0);
        
        for (int times = 0; times < maxTimes; ++times)
        {
            vector<int> list;
            list.reserve(size);

            for (int i = 0; i < size; ++i)
            {
                list.push_back(dist(gen));
            }

            auto start = clock::now();
            //insertion_sort(list, 0, list.size() - 1);
            //quick_sort(list, 0, list.size() - 1);
            //list = merge_sort(list);
            //heap_sort(list, 0, list.size() - 1);
            hybrid_sort(list, 0, list.size() - 1);
            auto end = clock::now();

            total_time_taken += chrono::duration_cast<ns>(end - start);
        }
        
        file_out << size << ' ' << total_time_taken.count() / maxTimes << endl;
    }

    file_out.close();
}

void hybrid_sort(vector<int>& list, int start, int end)
{
    if (end - start <= 75)
    {
        int val;

        for (int i = start; i <= end; ++i)
        {
            val = list[i];

            for (int j = i - 1; j + 1 >= 0; --j)
            {
                if (j >= 0 && val < list[j])
                {
                    list[j + 1] = list[j];
                }
                else
                {
                    list[j + 1] = val;
                    break;
                }
            }
        }
    }
    else
    {
        int pivot = start;
        int left = start + 1;
        int right = end;

        while (left < right)
        {
            while (list[left] <= list[pivot] && left < right)
            {
                ++left;
            }

            while (list[right] > list[pivot])
            {
                --right;
            }

            if (left <= right)
            {
                swap(list[left], list[right]);
            }
        }

        swap(list[pivot], list[right]);
        pivot = right;

        if (pivot - start > 1)
        {
            hybrid_sort(list, start, pivot - 1);
        }

        if (end - pivot > 1)
        {
            hybrid_sort(list, pivot + 1, end);
        }
    }
}

void insertion_sort(vector<int>& list, int start, int end)
{
    int val;

    for (int i = start; i <= end; ++i)
    {
        val = list[i];

        for (int j = i - 1; j + 1 >= 0; --j)
        {
            if (j >= 0 && val < list[j])
            {
                list[j + 1] = list[j];
            }
            else
            {
                list[j + 1] = val;
                break;
            }
        }
    }
}

void quick_sort(vector<int>& list, int start, int end)
{
    int pivot = start;
    int left = start + 1;
    int right = end;

    while (left < right)
    {
        while (list[left] <= list[pivot] && left < right)
        {
            ++left;
        }

        while (list[right] > list[pivot])
        {
            --right;
        }

        if (left <= right)
        {
            swap(list[left], list[right]);
        }
    }

    swap(list[pivot], list[right]);
    pivot = right;

    if (pivot - start > 1)
    {
        quick_sort(list, start, pivot - 1);
    }
    
    if (end - pivot > 1)
    {
        quick_sort(list, pivot + 1, end);
    }
}

vector<int> merge(vector<int>& left, vector<int>& right)
{
    vector<int> result;
    result.reserve(left.size() + right.size());
    int l = 0;
    int r = 0;

    while (l < left.size() && r < right.size())
    {
        if (left[l] <= right[r])
        {
            result.push_back(left[l]);
            ++l;
        }
        else
        {
            result.push_back(right[r]);
            ++r;
        }
    }

   
    while (l < left.size())
    {
        result.push_back(left[l]);
        ++l;
    }
    
    while (r < right.size())
    {
        result.push_back(right[r]);
        ++r;
    }

    return result;
}

vector<int> merge_sort(vector<int>& list)
{
    if (list.size() == 1)
    {
        return list;
    }
    else
    {
        int middle = list.size() / 2;
        vector<int> left(list.begin(), list.begin() + middle);
        vector<int> right(list.begin() + middle, list.begin() + list.size());

        left = merge_sort(left);
        right = merge_sort(right);
        return merge(left, right);
    }
}

void fix_heap(vector<int>& list, int start, int end)
{
    int biggest = start;
    int left = 2 * start + 1;
    int right = 2 * start + 2;
    
    if (left <= end && list[left] > list[biggest]) {
        biggest = left;
    }
    if (right <= end && list[right] > list[biggest]) {
        biggest = right;
    }

    if (start != biggest)
    {
        swap(list[start], list[biggest]);       
        fix_heap(list, biggest, end);
    }
}

void heap_sort(vector<int>& list, int start, int end)
{
    for (int i = (end - 1 ) / 2; i >= start; --i)
    {
        fix_heap(list, i, end);
    }

    for (int i = start; i < end; ++i)
    {
        swap(list[start], list[end - i]);
        fix_heap(list, start, end - 1 - i);
    }
}