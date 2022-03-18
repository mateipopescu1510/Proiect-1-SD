#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <limits>
#include <chrono>
#include <algorithm>
#define MAXN 20000000
#define MAXMAX 100000000
using namespace std;
using namespace chrono;
ifstream in("input.txt");
ofstream out("output.txt");

int max(int b, vector<int>v) {
    int m = v[0];
    for (int i = 1; i < v.size(); i++)
        if (m < v[i])
            m = v[i];
    int nr = 0;
    while (m)
    {
        m /= b;
        nr++;
    }
    return nr;
}
void radixSort(int b, vector<int>& v) {
    vector<vector<int>>bins;
    for (int i = 0; i < b; i++) {
        vector<int>bbase;
        bins.push_back(bbase);
    }
    int m = max(b, v), p = 1;
    for (int c = 0; c < m; c++) {
        for (int i = 0; i < v.size(); i++) {
            bins[(v[i] / p) % b].push_back(v[i]);
        }
        p *= b;
        v.clear();
        for (int i = 0; i < b; i++) {
            for (int j = 0; j < bins[i].size(); j++) {
                v.push_back(bins[i][j]);
            }
            bins[i].clear();
        }
    }
}

void shellSort(vector<int>& v) {
    int gap = 1, left, right;
    while (gap < v.size() / 3) {
        gap = gap * 3 + 1;
    }
    while (gap > 0) {
        for (right = gap; right < v.size(); right++) {
            int temp = v[right];
            left = right;
            while (left > gap - 1 && v[left - gap] >= temp) {
                v[left] = v[left - gap];
                left -= gap;
            }
            v[left] = temp;
        }
        gap = (gap - 1) / 3;
    }
}

void merge(int left, int middle, int right, vector<int>& v) {
    int leftLen = middle - left + 1, rightLen = right - middle, i = 0, j = 0, k = left;
    int* leftHalf = new int[leftLen];
    int* rightHalf = new int[rightLen];
    for (int z = 0; z < leftLen; z++) {
        leftHalf[z] = v[left + z];
    }
    for (int z = 0; z < rightLen; z++) {
        rightHalf[z] = v[middle + z + 1];
    }
    while (i < leftLen && j < rightLen) {
        if (leftHalf[i] <= rightHalf[j]) {
            v[k] = leftHalf[i++];
        }
        else {
            v[k] = rightHalf[j++];
        }
        k++;
    }
    while (i < leftLen) {
        v[k++] = leftHalf[i++];
    }
    while (j < rightLen) {
        v[k++] = rightHalf[j++];
    }
}
void mergeSort(int left, int right, vector <int>& v) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSort(left, middle, v);
        mergeSort(middle + 1, right, v);
        merge(left, middle, right, v);
    }
}

void countSort(vector<int>& v) {
    int size = v[0];
    for (int i = 1; i < v.size(); i++)
        if (size < v[i])
            size = v[i];
    int* fr = new int[size + 1];
    for (int i = 0; i <= size + 1; i++)
        fr[i] = 0;
    for (int i = 0; i < v.size(); i++) {
        fr[v[i]]++;
    }
    v.clear();
    for (int i = 0; i < size; i++)
        for (int j = 0; j < fr[i]; j++)
            v.push_back(i);
}


void combSort(vector<int>& v) {
    int gap = v.size();
    bool ok = true;
    while (gap != 1 || ok == true) {
        gap = (gap * 10) / 13;
        if (gap < 1)
            gap = 1;
        ok = false;
        for (int i = 0; i < v.size() - gap; i++) {
            if (v[i] > v[i + gap]) {
                int temp = v[i];
                v[i] = v[i + gap];
                v[i + gap] = temp;
                ok = true;
            }
        }
    }
}

void generateVector(int max, vector<int>& v) {
    mt19937_64 gen(time(NULL));
    uniform_int_distribution<int> random(0, max);
    for (auto i = v.begin(); i != v.end(); i++) {
        *i = random(gen);
    }
}

void checkIfSorted(vector<int>& v) {
    int k = 1;
    for (int i=1; i<v.size();i++)
        if (v[i] < v[i - 1]) {
            k = 0;
            break;
        }
    k == 1 ? out << "s; Correctly sorted\n" : out << "s; Incorrectly sorted\n";
}

void testAllSorts(vector<int>& v) {
    vector<int>w(v.size());
    w = v;
    auto start = high_resolution_clock::now();
    radixSort(10, w);
    auto stop = high_resolution_clock::now();
    duration<double> time = stop - start;
    out << "RADIX (base 10): " << time.count();
    checkIfSorted(w);
    for (int b = 16; b <= 65536; b *= 16) {
        w = v;
        auto start = high_resolution_clock::now();
        radixSort(b, w);
        auto stop = high_resolution_clock::now();
        duration<double> time = stop - start;
        out << "RADIX (base " << b << "): " << time.count();
        checkIfSorted(w);
    }

    w = v;
    start = high_resolution_clock::now();
    sort(w.begin(), w.end());
    stop = high_resolution_clock::now();
    time = stop - start;
    out << "STD SORT: " << time.count();
    checkIfSorted(w);

    if (MAXN < v.size()) {
        out << "Merge sort doesn't work for vectors with more than 20 million elements\n";
    }
    else {
        w = v;
        start = high_resolution_clock::now();
        mergeSort(0, v.size() - 1, w);
        stop = high_resolution_clock::now();
        time = stop - start;
        out << "MERGE: " << time.count();
        checkIfSorted(w);
    }

    w = v;
    int size = w[0];
    for (int i = 1; i < w.size(); i++)
        if (size < w[i])
            size = w[i];
    if (MAXMAX < size) 
        out << "Count sort doesn't work for vectors with max > " << MAXMAX;
    else {
        start = high_resolution_clock::now();
        countSort(w);
        stop = high_resolution_clock::now();
        time = stop - start;
        out << "COUNT: " << time.count();
        checkIfSorted(w);
    }
    
    if (MAXN < v.size()) {
        out << "Shell sort doesn't work for vectors with more than 20 million elements\n";
    }
    else {
        w = v;
        start = high_resolution_clock::now();
        shellSort(w);
        stop = high_resolution_clock::now();
        time = stop - start;
        out << "SHELL: " << time.count();
        checkIfSorted(w);
    }

    w = v;
    start = high_resolution_clock::now();
    combSort(w);
    stop = high_resolution_clock::now();
    time = stop - start;
    out << "COMB: " << time.count();
    checkIfSorted(w);
}


int main()
{
    int n, max, T;
    in >> T;
    for (int i = 0; i < T; i++) {
        in >> n >> max;
        out << "\n\nRandom vector:\nn   = " << n << "\nmax = " << max << "\n\n";
        vector<int>v(n);
        generateVector(max, v);
        testAllSorts(v);
    }
    in >> n >> max;
    out << "\n\nSorted vector(ascending):\nn   = " << n << "\nmax = " << max << "\n\n";
    vector<int>x(n);
    generateVector(max, x);
    countSort(x);
    testAllSorts(x);

    out << "\n\nSorted vector(descending):\nn   = " << n << "\nmax = " << max << "\n\n";
    vector<int>y(n);
    generateVector(max, y);
    countSort(y);
    reverse(y.begin(), y.end());
    testAllSorts(y);
    return 0;
}
