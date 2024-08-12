#include <bits/stdc++.h>
#include "Benchmark.h"
#include <iomanip>
using namespace std;
//#if defined(TEST_MREQ)
//#define METATYPE mReqSketch
//#define metaname "mreq"
#if defined(TEST_TD)
#define METATYPE TDigest
#define metaname "tdigest"
//#elif defined(TEST_DD)
//#define METATYPE DDSketch
//#define metaname "dd"
#endif

int main(int argc, const char** argv) {
	freopen("data_m4.out", "w", stdout);
	string path1 = "../CAIDA2016/1.dat";
    string path2 = "../zipf_2022/zipf_2.0.dat";
	string path3 = "../Seattle/SeattleData_";
    string path4 = "../tap.dat";

	std::pair<std::pair<double, double>, double> res[333], cur;
    int cnt = 0;

    CAIDA_Benchmark benchmark(path1);
    for (double w = 0.10; w < 0.95; w += 0.10)
    {
        for (int memory = 1500; memory <= 1500; memory += 200)
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

    int cnt = 0;

    zipf_Benchmark benchmark2(path2);
    for (double w = 0.10; w < 0.95; w += 0.10)
    {
        for (int memory = 7000; memory <= 7000; memory += 10)
        {
			++cnt;
            cur = benchmark2.Run(w, memory, 10);
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
        for (int memory = 80; memory <= 80; memory += 10)
        
			++cnt;
            cur = benchmark3.Run(w, memory, 10);
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
            cur = benchmark4.Run(w, memory, 10);
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