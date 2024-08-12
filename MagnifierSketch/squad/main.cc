#include <bits/stdc++.h>
#include "Benchmark_squad.h"
#include <iomanip>
using namespace std;

int main(int argc, const char** argv){
    freopen("data_squad.out", "w", stdout);
    string path1 = "/share/datasets/CAIDA2016/1.dat";
	string path2 = "../../zipf_2022/zipf_2.0.dat";
	string path3 = "../../Seattle/SeattleData_";
    string path4 = "../tap.dat";

    //SeattleBenchmark benchmark(path_seattle);
    //WebgetBenchmark benchmark(path_webget);
    //CAIDABenchmark benchmark(path_caida);
    //benchmark.Run(atoi(argv[1]),stod(argv[2])); //memory单位是kb

    std::pair<std::pair<double, double>, double> res[333], cur;
    CAIDA_Benchmark benchmark(path1);
    int cnt = 0;
    ++cnt;
    cur = benchmark.Run(1500, 1, 0.99, 1000.00 / 17790002.00);
    res[cnt] = cur;
    
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

    Tap_Benchmark benchmark(path4);
    for (double w = 0.10; w < 0.95; w += 0.10)
    {
	    ++cnt;
        cur = benchmark.Run(1500, 1, w, 1000.00 / 17790002.00);
        res[cnt] = cur;
    }
        //++cnt;
        //cur = benchmark.Run(1100, 0.65, w, 1000.00 / 17790002.00);
        //res[cnt] = cur;
        //++cnt;
        //cur = benchmark.Run(1300, 0.8, w, 1000.00 / 17790002.00);
        //res[cnt] = cur;
        


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