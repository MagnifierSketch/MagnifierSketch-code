#include <iostream>
#include <bits/stdc++.h>
#include "Benchmark_gk.h"
#include <cmath>
#include <random>
#include <ctime>
#include <chrono>
#include <iomanip>
using namespace std;

int main(int argc, char** argv)
{
    freopen("data_gk.out", "w", stdout);
    double epsilon = 0.01;
    
	string path1 = "../CAIDA2016/1.dat";
	string path2 = "../zipf_2022/zipf_2.0.dat";
	string path3 = "../Seattle/SeattleData_";
    string path4 = "../tap.dat";

    std::pair<std::pair<double, double>, double> res[333], cur;
    int cnt = 0;

    CAIDA_Benchmark benchmark(path1);
    for (int memory = 900; memory <= 1500; memory += 200)
    {
		++cnt;
        cur = benchmark.Run(memory, 0.05, 0.99);
        res[cnt] = cur;
    }

    for (double w = 0.10; w < 0.95; w += 0.10)
    {
        for (int memory = 500; memory <= 1500; memory += 200)
        {
			++cnt;
            cur = benchmark.Run(memory, 0.05, w);
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

    int cnt = 0;

    zipf_Benchmark benchmark2(path2);
    for (double w = 0.10; w < 0.95; w += 0.10)
    {
        
        ++cnt;
        cur = benchmark2.Run(80, 0.05, w);
        res[cnt] = cur;
        
        for (int memory = 30; memory <= 80; memory += 10)
        {
			++cnt;
            cur = benchmark2.Run(memory, 0.05, w);
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


    int cnt = 0;

    Seattle_Benchmark benchmark3(path3);
    for (double w = 0.10; w < 0.95; w += 0.10)
    {
        
        ++cnt;
        cur = benchmark3.Run(80, 0.05, w);
        res[cnt] = cur;
        
        for (int memory = 50; memory <= 100; memory += 10)
        {
			++cnt;
            cur = benchmark3.Run(memory, 0.05, w);
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

    int cnt = 0;

    Tap_Benchmark benchmark4(path4);
    for (double w = 0.10; w < 0.95; w += 0.10)
    {
        for (int memory = 900; memory <= 1500; memory += 200)
        {
			++cnt;
            cur = benchmark4.Run(memory, 0.05, w);
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

    return 0;
}
