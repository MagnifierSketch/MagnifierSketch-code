#include <bits/stdc++.h>
#include "Benchmark.h"
using namespace std;
#include <iomanip>
int mm;
double f[11][11];
std::vector<int> v;

int main(int argc, const char** argv) {
	freopen("data_req_single.out", "w", stdout);
	string path1 = "../CAIDA2016/1.dat";
	string path2 = "../zipf9.dat";
    
    std::pair<std::pair<double, double>, double> res[333], cur;
    int cnt = 0;

    v.push_back(230);
    v.push_back(2);
    v.push_back(2);
    CAIDA_Benchmark benchmark(path1);
    for (double w = 0.10; w < 0.95; w += 0.10)
    {
        for (int memory = 10; memory <= 10; memory += 10)
        {
			++cnt;
            cur = benchmark.Run(w, memory, 10);
            res[cnt] = cur;
            
        }
    }

    for (int i = 1; i <= cnt; i++) 
    {
        std::cout << std::fixed<<std::setprecision(4) << res[i].first.first << "	";
    }

    std::cout << std::endl;

    for (int i = 1; i <= cnt; i++) 
    {
        std::cout << std::fixed<<std::setprecision(4) << res[i].first.second << "	";
    }

    std::cout << std::endl;

    for (int i = 1; i <= cnt; i++) 
    {
        std::cout << std::fixed<<std::setprecision(4) << res[i].second << "	";
    }

    std::cout << std::endl;

    synthetic_Benchmark benchmark4(path2);

    v.clear();
    v.push_back(920);
    v.push_back(2);
    v.push_back(2);
    v.push_back(2);
    v.push_back(2);
    v.push_back(2);
    v.push_back(2);

    int cnt1 = 0;

    for (double w = 0.10; w < 0.95; w += 0.10)
    {
        for (int memory = 10; memory <= 10; memory += 10)
        {
            ++cnt1;
            cur = benchmark4.Run(w, memory, 10);
            res[cnt1] = cur;
        }
    }

    for (int i = 1; i <= cnt1; i++) 
    {
        std::cout << std::fixed<<std::setprecision(4) << res[i].first.first << "	";
    }

    std::cout << std::endl;

    for (int i = 1; i <= cnt1; i++) 
    {
        std::cout << std::fixed<<std::setprecision(4) << res[i].first.second << "	";
    }

    std::cout << std::endl;

    for (int i = 1; i <= cnt1; i++) 
    {
        std::cout << std::fixed<<std::setprecision(4) << res[i].second << "	";
    }

    std::cout << std::endl;

	return 0;
}