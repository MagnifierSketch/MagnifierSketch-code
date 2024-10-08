#include <bits/stdc++.h>
#include <sys/time.h>
#include "include/meta/mreq/mreq_sketch.hpp"
#include "GroundTruth.h"
#include "hash.h"
#include "Mmap.h"
#include <cassert>
#include <linux/types.h>
#define MAXN 1000000
#define mod 10000019
__u64 rdtsc()
{
        __u32 lo,hi;


        __asm__ __volatile__
        (
         "rdtsc":"=a"(lo),"=d"(hi)
        );
        return (__u64)hi<<32|lo;
}

struct CAIDA_Tuple {
    uint64_t timestamp;
    uint64_t id;
};

class CAIDA_Benchmark {
public:
	CAIDA_Benchmark() {}
	CAIDA_Benchmark(std::string path) {
		load_result = Load(path.c_str());
        dataset = (CAIDA_Tuple*)load_result.start;
        length = load_result.length / sizeof(CAIDA_Tuple);
	}
	~CAIDA_Benchmark() {}
    void Init()
    {
        nxt.resize(MAXN + 5);
		tid.resize(MAXN + 5);
		head.resize(mod + 5);
        id_map.resize(MAXN + 5);
        last_time.resize(MAXN + 5);
		cnt = 0;
		for (int i = 1; i <= MAXN; i++) 
		{
			nxt[i] = 0;
            id_map[i] = 0;
		}
		for (int i = 0; i < mod; i++) head[i] = 0;
    }
	std::pair<std::pair<double, double>, double> Run(double w, uint32_t memory, uint32_t compression) {
        Init();
		uint32_t run_length = 20000000;
        double query_quantile = w;
        double tottime = 0.00, tt;
		assert(run_length <= length);

		GroundTruth<uint64_t, uint64_t> gt;

		sketch::mReqSketch req(memory, compression);

        std::vector<uint64_t> ins; ins.clear();
		for (int i = 0; i < run_length; ++i) {
            int g = dataset[i].id % mod;
            bool flag = 0;
            for (int j = head[g]; j; j = nxt[j])
            {
                if (tid[j] == dataset[i].id)
                {
                    flag = 1;
                    id_map[j] ++;
                    if (dataset[i].timestamp > last_time[j])
                    {
                        gt.insert(1, dataset[i].timestamp - last_time[j]);

                        ins.push_back(dataset[i].timestamp - last_time[j]);
                    }
                    last_time[j] = dataset[i].timestamp;
                    break;
                }
            }
            if (!flag)
            {
                cnt++;
                tid[cnt] = dataset[i].id; nxt[cnt] = head[g];
                head[g] = cnt;
                id_map[cnt] = 1;
                last_time[cnt] = dataset[i].timestamp;
            }
		}
        gt.build();
        tt = clock();
        
        for (int i = 0; i < ins.size(); i++)
        {
            req.append(ins[i]);
        }

        tottime = clock() - tt;

        double totaltime1 = (double)(tottime) / CLOCKS_PER_SEC;
        double throughput1 = double((int)ins.size()) / totaltime1;

		//std::cout << totaltime << std::endl;

		double error_qs = 0;

        uint64_t begin = rdtsc();
        uint64_t predict_qs;
        for (int t = 1; t <= 1000; t++)
        {
            //predict_qs = td.query(1, query_quantile); ->value
            predict_qs = req.quantile(query_quantile, true);
            //std::cout << "query_quantile: " << query_quantile << std::endl;
            //std::cout << "predict_qs: " << predict_qs << std::endl;
        }
        uint64_t end = rdtsc();
        double throughput2 = 1000000000.00 / (double)(end - begin);
        double predict_quantile_qs = gt.query(1, predict_qs);

        //std::cout << predict_quantile_qs << std::endl;

        error_qs += fabs(predict_quantile_qs - query_quantile);
        //std::cout << "predict_quantile_qs: " << predict_quantile_qs << std::endl;
        //std::cout << "Insert Throughput: " << throughput1 << " items/sec" << std::endl;
        //std::cout << "Query Throughput: " << throughput2 << " queries/sec" << std::endl;
        //std::cout << "Quantile Error: " << error_qs << std::endl;
        return std::make_pair(std::make_pair(throughput1, throughput2), error_qs);
	}

private:
	std::string filename;
    LoadResult load_result;
    CAIDA_Tuple *dataset;
    uint64_t length;
    int cnt;
	std::vector<int> head, nxt, id_map;
    std::vector<uint64_t> last_time;
	std::vector<uint64_t> tid; 
};

struct synthetic_Tuple {
    uint64_t stamp;
};


class synthetic_Benchmark {
public:
	synthetic_Benchmark() {}
	synthetic_Benchmark(std::string path) {
		freopen(path.c_str(), "r", stdin);
        length = 20000000;
        dataset = new synthetic_Tuple [length];
        for (int i = 0; i < length; i++)
        {
            scanf("%llu", &dataset[i].stamp);
        }
	}
	~synthetic_Benchmark() {}
    std::pair<std::pair<double, double>, double> Run(double w, uint32_t memory, double compression) {
        uint32_t run_length = 20000000;
        double query_quantile = w;
        double tottime = 0.00, tt;
		assert(run_length <= length);

		GroundTruth<uint64_t, uint64_t> gt;
		sketch::mReqSketch req(memory, compression);

        std::vector<uint64_t> ins; ins.clear();
		for (int i = 0; i < run_length; ++i) {
            gt.insert(1, dataset[i].stamp);
            ins.push_back(dataset[i].stamp);
            //std::cout << dataset[i].stamp << std::endl;
		}
        gt.build();
        tt = clock();
        
        for (int i = 0; i < ins.size(); i++)
        {
            req.append(ins[i]);
        }

        tottime = clock() - tt;

        double totaltime1 = (double)(tottime) / CLOCKS_PER_SEC;
        double throughput1 = double((int)ins.size()) / totaltime1;

		//std::cout << totaltime << std::endl;

		double error_qs = 0;

        uint64_t begin = rdtsc();
        uint64_t predict_qs;
        for (int t = 1; t <= 1000; t++)
        {
            predict_qs = req.quantile(query_quantile,true);
            //std::cout << "predict_qs: " << predict_qs << std::endl;
        }
        uint64_t end = rdtsc();
        double throughput2 = 1000000000.00 / (double)(end - begin);
        double predict_quantile_qs = gt.query(1, predict_qs);

        //std::cout << "predict_quantile_qs: " <<  predict_quantile_qs << std::endl;

        error_qs += fabs(predict_quantile_qs - query_quantile);


        return std::make_pair(std::make_pair(throughput1, throughput2), error_qs);
	}

private:
    synthetic_Tuple *dataset;
    uint64_t length;
};