#include <iostream>
//include <bits/stdc++.h>
#include <sys/time.h>
#include "SketchPolymer.h"
#include "GroundTruth.h"
#include "hash.h"
#include "Mmap.h"
#include <cassert>

struct CAIDA_Tuple {
    uint64_t timestamp;
    uint64_t id;
};

#define MAXN 20000000
#define mod 10000019
#define web_len 383360
#define n_slice 688
#define MOD 13908490328490391

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
	std::pair<std::pair<double, double>, double> Run(double w, uint32_t memory) {
        Init();
		uint32_t run_length = 20000000;
        double query_quantile = w;
        double tottime = 0.00, tt;
		assert(run_length <= length);

		GroundTruth<uint64_t, uint64_t> gt;
        SketchPolymer<uint64_t>* sketchpolymer = new SketchPolymer<uint64_t>(memory);
        std::vector<std::pair<uint64_t, uint64_t> > ins; ins.clear();
		for (int i = 0; i < run_length; ++i) {
            int g = dataset[i].id % mod;
            bool flag = 0;
            for (int j = head[g]; j; j = nxt[j])
            {
                if (tid[j] == dataset[i].id)
                {
                    flag = 1;
                    id_map[j] ++;
                    ins.push_back(std::make_pair(dataset[i].id, dataset[i].timestamp - last_time[j]));
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

        //std::cerr << cnt << std::endl;

        for (int i = 0; i < ins.size(); i++)
        {
            gt.insert(ins[i].first, ins[i].second);
        }

        gt.build();

        tt = clock();
        for (int i = 0; i < ins.size(); i++)
        {
            sketchpolymer -> insert(ins[i].first, 0, ins[i].second);
        }
        tottime = clock() - tt;

        double totaltime1 = (double)(tottime) / CLOCKS_PER_SEC;
        double throughput1 = double((int)ins.size()) / totaltime1;

        std::vector<uint64_t> qrys, ans; qrys.clear(); ans.clear();

		double error_qs = 0;
        int num = 0;
        for (int i = 1; i <= cnt; i++) {
            if (id_map[i] < 5000)
                continue;
            num++;

            qrys.push_back(tid[i]);
        }

        ans.resize(num);

        struct timeval t_start, t_end;
        gettimeofday( &t_start, NULL );
        for (int i = 0; i < num; i++)
        {
            ans[i] = sketchpolymer -> query(qrys[i], query_quantile);
        }
        gettimeofday( &t_end, NULL );

        double totaltime2 = (double)(t_end.tv_sec - t_start.tv_sec) + (double)(t_end.tv_usec - t_start.tv_usec) / 1000000.00;
        double throughput2 = double(num) / totaltime2;

        for (int i = 0; i < num; i++) {

            double predict_quantile_qs = gt.query(qrys[i], ans[i]);

            //std::cout << tid[i] << " " << predict_quantile_qs << " " << id_map[i] << std::endl;

            error_qs += fabs(predict_quantile_qs - query_quantile);
            //std::cout << predict_quantile_qs << "\n";
        }

        return std::make_pair(std::make_pair(throughput1, throughput2), error_qs / num);
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

struct zipf_Tuple {
    uint32_t id;
};


class zipf_Benchmark {
public:
	zipf_Benchmark() {}
	zipf_Benchmark(std::string path) {
		load_result = Load(path.c_str());
        dataset = (zipf_Tuple*)load_result.start;
        length = load_result.length / sizeof(zipf_Tuple);
	}
	~zipf_Benchmark() {}
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
	std::pair<std::pair<double, double>, double> Run(double w, uint32_t memory) {
        Init();
		uint32_t run_length = 20000000;
        double query_quantile = w;
        double tottime = 0.00, tt;
		assert(run_length <= length);

		GroundTruth<uint64_t, uint64_t> gt;
        SketchPolymer<uint64_t>* sketchpolymer = new SketchPolymer<uint64_t>(memory);
        std::vector<std::pair<uint64_t, uint64_t> > ins; ins.clear();
		for (int i = 0; i < run_length; ++i) {
            int g = dataset[i].id % mod;
            bool flag = 0;
            for (int j = head[g]; j; j = nxt[j])
            {
                if (tid[j] == dataset[i].id)
                {
                    flag = 1;
                    id_map[j] ++;
                    ins.push_back(std::make_pair(dataset[i].id, i - last_time[j]));
                    last_time[j] = i;
                    break;
                }
            }
            if (!flag)
            {
                cnt++;
                tid[cnt] = dataset[i].id; nxt[cnt] = head[g];
                head[g] = cnt;
                id_map[cnt] = 1;
                last_time[cnt] = i;
            }
		}

        for (int i = 0; i < ins.size(); i++)
        {
            gt.insert(ins[i].first, ins[i].second);
        }

        gt.build();

        tt = clock();
        for (int i = 0; i < ins.size(); i++)
        {
            sketchpolymer -> insert(ins[i].first, 0, ins[i].second);
        }
        tottime = clock() - tt;

        double totaltime1 = (double)(tottime) / CLOCKS_PER_SEC;
        double throughput1 = double((int)ins.size()) / totaltime1;

        std::vector<uint64_t> qrys, ans; qrys.clear(); ans.clear();

		double error_qs = 0;
        int num = 0;
        for (int i = 1; i <= cnt; i++) {
            if (id_map[i] < 5000)
                continue;
            num++;

            qrys.push_back(tid[i]);
        }

        ans.resize(num);

        struct timeval t_start, t_end;
        gettimeofday( &t_start, NULL );
        for (int i = 0; i < num; i++)
        {
            ans[i] = sketchpolymer -> query(qrys[i], query_quantile);
        }
        gettimeofday( &t_end, NULL );

        double totaltime2 = (double)(t_end.tv_sec - t_start.tv_sec) + (double)(t_end.tv_usec - t_start.tv_usec) / 1000000.00;
        double throughput2 = double(num) / totaltime2;

        for (int i = 0; i < num; i++) {

            double predict_quantile_qs = gt.query(qrys[i], ans[i]);

            //std::cout << tid[i] << " " << predict_quantile_qs << " " << id_map[i] << std::endl;

            error_qs += fabs(predict_quantile_qs - query_quantile);
            //std::cout << predict_quantile_qs << "\n";
        }

        return std::make_pair(std::make_pair(throughput1, throughput2), error_qs / num);
	}

private:
	std::string filename;
    LoadResult load_result;
    zipf_Tuple *dataset;
    uint64_t length;
    int cnt;
	std::vector<int> head, nxt, id_map;
    std::vector<uint64_t> last_time;
	std::vector<uint64_t> tid; 
};


struct Seattle_Tuple {
    uint32_t id;
    uint32_t fetch_time;
};

std::string itos(int n)
{
    std::string res = "";
    while(n)
    {
        res = (char)('0' + (n % 10)) + res;
        n /= 10;
    }
    return res;
}


class Seattle_Benchmark {
public:
	Seattle_Benchmark() {}
	Seattle_Benchmark(std::string path) {
        int cc;
        length = 0;
        dataset = new Seattle_Tuple [n_slice * 99 * 98];
        std::string path1;
        double cur;
        for (int t = 1; t <= n_slice; t++)
        {
            path1 = path + itos(t);
            freopen(path1.c_str(), "r", stdin);
            for (int i = 1; i <= 99; i++)
            {
                for (int j = 1; j <= 99; j++)
                {
                    scanf("%lf", &cur);
                    cc = (int)(cur * 100.00);
                    if (i != j)
                    {
                        dataset[length].id = i;
                        dataset[length].fetch_time = cc;
                        length++;
                    }
                }
            }
        }
        /*for (int i = 0; i < length; i++)
        {
            std::cout << dataset[i].id << " " << dataset[i].fetch_time << std::endl;
        }*/
	}
	~Seattle_Benchmark()
    {
        delete dataset;
    }
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
	std::pair<std::pair<double, double>, double> Run(double w, uint32_t memory) {
        Init();
		uint32_t run_length = length;
        double query_quantile = w;
        double tottime = 0.00, tt;
		assert(run_length <= length);

		GroundTruth<uint64_t, uint64_t> gt;
		SketchPolymer<uint64_t>* sketchpolymer = new SketchPolymer<uint64_t>(memory);
        std::vector<std::pair<uint64_t, uint64_t> > ins; ins.clear();
		for (int i = 0; i < run_length; ++i) {
            int g = dataset[i].id % mod;
            bool flag = 0;
            for (int j = head[g]; j; j = nxt[j])
            {
                if (tid[j] == dataset[i].id)
                {
                    flag = 1;
                    id_map[j] ++;
                    ins.push_back(std::make_pair(dataset[i].id, dataset[i].fetch_time));
                    
                    break;
                }
            }
            if (!flag)
            {
                cnt++;
                tid[cnt] = dataset[i].id; nxt[cnt] = head[g];
                head[g] = cnt;
                id_map[cnt] = 1;

                ins.push_back(std::make_pair(dataset[i].id, dataset[i].fetch_time));
            }
		}

        for (int i = 0; i < ins.size(); i++)
        {
            gt.insert(ins[i].first, ins[i].second);
        }

        gt.build();

        tt = clock();
        for (int i = 0; i < ins.size(); i++)
        {
            sketchpolymer -> insert(ins[i].first, 0, ins[i].second);
        }
        tottime = clock() - tt;


       double totaltime1 = (double)(tottime) / CLOCKS_PER_SEC;
        double throughput1 = double((int)ins.size()) / totaltime1;

        std::vector<uint64_t> qrys, ans; qrys.clear(); ans.clear();

		double error_qs = 0;
        int num = 0;
        for (int i = 1; i <= cnt; i++) {
            if (id_map[i] < 5000)
                continue;
            num++;

            qrys.push_back(tid[i]);
        }

        ans.resize(num);

        struct timeval t_start, t_end;
        gettimeofday( &t_start, NULL );
        for (int i = 0; i < num; i++)
        {
            ans[i] = sketchpolymer -> query(qrys[i], query_quantile);
        }
        gettimeofday( &t_end, NULL );

        double totaltime2 = (double)(t_end.tv_sec - t_start.tv_sec) + (double)(t_end.tv_usec - t_start.tv_usec) / 1000000.00;
        double throughput2 = double(num) / totaltime2;

        for (int i = 0; i < num; i++) {

            double predict_quantile_qs = gt.query(qrys[i], ans[i]);

            //std::cout << tid[i] << " " << predict_quantile_qs << " " << id_map[i] << std::endl;

            error_qs += fabs(predict_quantile_qs - query_quantile);
            //std::cout << predict_quantile_qs << "\n";
        }

        return std::make_pair(std::make_pair(throughput1, throughput2), error_qs / num);
	}

private:
    Seattle_Tuple *dataset;
    uint64_t length;
    int cnt;
	std::vector<int> head, nxt, id_map;
    std::vector<uint64_t> last_time;
	std::vector<uint64_t> tid; 
};

struct five_tuple
{
    char s[13];
    bool operator == (const five_tuple &u) const
    {
        for (int i = 0; i < 13; i++)
        {
            if (s[i] != u.s[i]) return 0;
        }
        return 1;
    }
    bool operator < (const five_tuple &u) const
    {
        for (int i = 0; i < 13; i++)
        {
            if (s[i] != u.s[i]) return s[i] < u.s[i];
        }
        return 0;
    }
    uint64_t get_hash(uint64_t md)
    {
        uint64_t res = 0;
        for (int i = 0; i < 13; i++)
        {
            res = (131ll * res + (int)s[i]) % md;
        }
        return res;
    }
};

struct Tap_Tuple {
    five_tuple id;
    uint64_t timestamp;
};


class Tap_Benchmark {
public:
	Tap_Benchmark() {}
	Tap_Benchmark(std::string path) {
		load_result = Load(path.c_str());
        dataset = (Tap_Tuple*)load_result.start;
        length = load_result.length / sizeof(Tap_Tuple);
	}
	~Tap_Benchmark() {}
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
	std::pair<std::pair<double, double>, double> Run(double w, uint32_t memory) {
        Init();
		uint32_t run_length = 14000000;
        double query_quantile = w;
        double tottime = 0.00, tt;
		assert(run_length <= length);

		GroundTruth<uint64_t, uint64_t> gt;
		SketchPolymer<uint64_t>* sketchpolymer = new SketchPolymer<uint64_t>(memory);
        std::vector<std::pair<uint64_t, uint64_t> > ins; ins.clear();
		for (int i = 0; i < run_length; ++i) {
            uint64_t id = dataset[i].id.get_hash(MOD);
            int g = id % mod;
            //std::cout << g << std::endl;
            bool flag = 0;
            for (int j = head[g]; j; j = nxt[j])
            {
                if (tid[j] == id)
                {
                    flag = 1;
                    id_map[j] ++;
                    ins.push_back(std::make_pair(id, dataset[i].timestamp - last_time[j]));
                    last_time[j] = dataset[i].timestamp;
                    break;
                }
            }
            if (!flag)
            {
                cnt++;
                tid[cnt] = id; nxt[cnt] = head[g];
                head[g] = cnt;
                id_map[cnt] = 1;
                last_time[cnt] = dataset[i].timestamp;
            }
		}

        for (int i = 0; i < ins.size(); i++)
        {
            gt.insert(ins[i].first, ins[i].second);
        }

        gt.build();

        tt = clock();
        for (int i = 0; i < ins.size(); i++)
        {
            sketchpolymer -> insert(ins[i].first, 0, ins[i].second);
        }
        tottime = clock() - tt;

        double totaltime1 = (double)(tottime) / CLOCKS_PER_SEC;
        double throughput1 = double((int)ins.size()) / totaltime1;

        std::vector<uint64_t> qrys, ans; qrys.clear(); ans.clear();

		double error_qs = 0;
        int num = 0;
        for (int i = 1; i <= cnt; i++) {
            if (id_map[i] < 5000)
                continue;
            num++;

            qrys.push_back(tid[i]);
        }

        ans.resize(num);

        struct timeval t_start, t_end;
        gettimeofday( &t_start, NULL );
        for (int i = 0; i < num; i++)
        {
            ans[i] = sketchpolymer -> query(qrys[i], query_quantile);
        }
        gettimeofday( &t_end, NULL );

        double totaltime2 = (double)(t_end.tv_sec - t_start.tv_sec) + (double)(t_end.tv_usec - t_start.tv_usec) / 1000000.00;
        double throughput2 = double(num) / totaltime2;

        for (int i = 0; i < num; i++) {

            double predict_quantile_qs = gt.query(qrys[i], ans[i]);

            //std::cout << tid[i] << " " << predict_quantile_qs << " " << id_map[i] << std::endl;

            error_qs += fabs(predict_quantile_qs - query_quantile);
            //std::cout << predict_quantile_qs << "\n";
        }

        return std::make_pair(std::make_pair(throughput1, throughput2), error_qs / num);
	}

private:
	std::string filename;
    LoadResult load_result;
    Tap_Tuple *dataset;
    uint64_t length;
    int cnt;
	std::vector<int> head, nxt, id_map;
    std::vector<uint64_t> last_time;
	std::vector<uint64_t> tid; 
};