#ifndef CQSORT_HPP
#define CQSORT_HPP
#include <algorithm>
#include <thread>
#include <future>
#include <atomic>
#include <iostream>
#include <chrono>
#include <functional>

template <typename T>
class C_QSort {
public:

    explicit C_QSort(size_t num_threads = std::thread::hardware_concurrency())
        : num_threads_(num_threads) {
    }

    void operator()(T& v) {
        quicksort(v, 0, v.size() - 1);
    }

private:

    template <typename U>
    int partition(std::vector<U>& v, int m, int n)
    {
        int i, j, pindex;

        pindex = (m + n) / 2;

        U pivot = v[pindex];

        int storeIndex = m;
        for (i = m; i < n; i++)
        {
            if (v[i] <= pivot)
            {
                std::swap(v[i], v[storeIndex]);

                storeIndex++;
            }
        }

        std::swap(v[pindex], v[n]);

        return pindex;
    }

    int quicksort(T& v, int m, int n)
    {
        int index;
        if (m >= n)
            return 0;
        {
            index = partition(v, m, n);
            quicksort(v, m, index - 1);
            quicksort(v, index + 1, n);
        }
    }

    size_t num_threads_;

};

template <typename T>
class CQSort {
public:
    CQSort(int mthr) {
        num_threads_ = std::min<unsigned int>(mthr, std::thread::hardware_concurrency());
    }

    void operator()(T& v) {
        C_QSort(v, 0, v.size() - 1);
    }

private:

    void C_QSort(T& v, int left, int right) {

        if (left >= right) {
            return;
        }

        int pivotIndex = partition(v, left, right);

        int right_1 = pivotIndex - 1;
        int left_2 = pivotIndex + 1;

        if (num_threads_ >= 2) {

            num_threads_ -= 2;

            auto left_branch = std::async(std::launch::async, &CQSort<T>::C_QSort, this, std::ref(v), left, pivotIndex - 1);

            auto right_branch = std::async(std::launch::async, &CQSort<T>::C_QSort, this, std::ref(v), pivotIndex + 1, right);


            left_branch.get();
            right_branch.get();
            num_threads_ += 2;
            
        }
        else {
            C_QSort(v, left, pivotIndex - 1);
            C_QSort(v, pivotIndex + 1, right);
            //return;
        }

    }


    template <typename U>
    int partition(std::vector<U>& v, int left, int right) const {
        int mid = left + (right - left) / 2;

        // Find median of left, mid, and right
        if (v[mid] < v[left]) {
            std::swap(v[left], v[mid]);
        }
        if (v[right] < v[left]) {
            std::swap(v[left], v[right]);
        }
        if (v[mid] < v[right]) {
            std::swap(v[mid], v[right]);
        }

        U pivotValue = v[right]; // pivot is now the median

        int storeIndex = left;

        for (int i = left; i < right; i++) {
            if (v[i] < pivotValue) {
                std::swap(v[i], v[storeIndex]);
                storeIndex++;
            }
        }

        std::swap(v[storeIndex], v[right]);
        return storeIndex;
    }

    size_t num_threads_;

};

#include "cqsort.tpp"
#endif // CQSORT_HPP