#include <leveldb/db.h>
#include <string>

#define KEY_LEN 8
#define VALUE_LEN 8
using namespace std;

const string workload = "../workloads/";

const string load = workload + "220w-rw-50-50-load.txt"; // TODO: the workload_load filename
const string run  = workload + "220w-rw-50-50-run.txt"; // TODO: the workload_run filename

const string filePath = "";

const int READ_WRITE_NUM = 2200000; // TODO: how many operations

int main()
{        
    leveldb::DB* db;
    leveldb::Options options;
    leveldb::WriteOptions write_options;
    // TODO: open and initial the levelDB
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    assert(status.ok());

    uint64_t inserted = 0, queried = 0, t = 0;
    uint64_t* key = new uint64_t[2200000]; // the key and value are same
    bool* ifInsert = new bool[2200000]; // the operation is insertion or not
	FILE *ycsb_load, *ycsb_run; // the files that store the ycsb operations
	char *buf = NULL;
	size_t len = 0;
    struct timespec start, finish; // use to caculate the time
    double single_time; // single operation time

    printf("Load phase begins \n");
    // TODO: read the ycsb_load and store
    if ((ycsb_load = fopen(load, "r")) = NULL) {
        printf("cannot open file!\n");
        return -1;
    }
    while (!feof(ycsb_load))  {
        fgets(buf,1024,ycsb_load);
        ifInsert[len] = true;
        key[len] = atoi(buf);
        printf("%d", key[len]);
        len++;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    // TODO: load the workload in LevelDB
    int i;
    for (i = 0; i < len; i++) {
        if (ifInsert[i]) {
            status = db->Put(write_options, key[i], key[i]);
            assert(status.ok());
            inserted++;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
	single_time = (finish.tv_sec - start.tv_sec) * 1000000000.0 + (finish.tv_nsec - start.tv_nsec);

    printf("Load phase finishes: %d items are inserted \n", inserted);
    printf("Load phase used time: %fs\n", single_time / 1000000000.0);
    printf("Load phase single insert time: %fns\n", single_time / inserted);

	int operation_num = 0;
    inserted = 0;		

    // TODO:read the ycsb_run and store
    len = 0;
    if ((ycsb_run = fopen(run, "r")) = NULL) {
        printf("cannot open file!\n");
        return -1;
    }
    while (!feof(ycsb_run))  {
        fgets(buf,1024,ycsb_run);
        if ((string)buf.find("INSERT") != s.npos)
            ifInsert[len] = 0;
        key[len] = atoi(buf);
        printf("%d", key[len]);
        len++;
        operation_num++;
    }
    clock_gettime(CLOCK_MONOTONIC, &start);

    // TODO: operate the levelDB
    int i;
    for (i = 0; i < len; i++) {
        if (ifInsert[i]) {
            status = db->Put(write_options, key[i], key[i]);
            assert(status.ok());
            inserted++;
        }
        else {
            status = db->Get(write_options, key[i], &t);
            assert(status.ok());
            queried++;
        }
    }

	clock_gettime(CLOCK_MONOTONIC, &finish);
	single_time = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Run phase finishes: %d/%d items are inserted/searched\n", operation_num - inserted, inserted);
    printf("Run phase throughput: %f operations per second \n", READ_WRITE_NUM/single_time);	
    return 0;
}
