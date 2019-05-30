#include "fptree/fptree.h"
#include <leveldb/db.h>
#include <string>

#define KEY_LEN 8
#define VALUE_LEN 8
using namespace std;

const string workload = "../workloads/"; // TODO: the workload folder filepath

const string load = workload + "1w-rw-50-50-load.txt"; // TODO: the workload_load filename
const string run  = workload + "1w-rw-50-50-run.txt"; // TODO: the workload_run filename

const int READ_WRITE_NUM = 10000; // TODO: amount of operations

int main()
{        
    FPTree fptree(1028);
    uint64_t inserted = 0, queried = 0, t = 0;
    uint64_t* key = new uint64_t[2200000];
    // bool* ifInsert = new bool[2200000];
    bool* op = new bool[2200000]; // the operation op 0:insert 1:delete 2:update 3:read
	FILE *ycsb_load, *ycsb_run;
	char *buf_ycsb = NULL;
    char *buf_fptree = NULL;
	size_t len = 0;
    struct timespec start, finish;
    double single_time;

    printf("===================FPtreeDB===================\n");
    printf("Load phase begins \n");

    // TODO: read the ycsb_load
    t = 0;
    if((ycsb_load = fopen(load.c_str,"r") == NULL)){
        printf("fptree can not open file!\n");
        return -1;
    }
    buf_fptree = new char[32];
    while(fgets(buf,32,ycsb_load) != NULL){
        len = strlen(buf_fptree);
        buf_fptree[len] = '\0';
        op[t] = 0;
        key[t] = atoll(buf_fptree + 7);
    }

    fclose(ycsb_load);
    clock_gettime(CLOCK_MONOTONIC, &start);

    // TODO: load the workload in the fptree
    int i;
    for(i = 0; i < READ_WRITE_NUM; i++){
        if(op[i] == 0){
           fptree.insert(key[i],key[i]);
           inserted++;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
	single_time = (finish.tv_sec - start.tv_sec) * 1000000000.0 + (finish.tv_nsec - start.tv_nsec);
    printf("Load phase finishes: %d items are inserted \n", inserted);
    printf("Load phase used time: %fs\n", single_time / 1000000000.0);
    printf("Load phase single insert time: %fns\n", single_time / inserted);

	printf("Run phase begins\n");

	int operation_num = 0;
    inserted = 0;		
    // TODO: read the ycsb_run
    if((ycsb_run = fopen(run.c_str(), "r")) == NULL){
        printf("fptree cannot open file!");
        return -1;
    }
    t = 0;
     while (fgets(buf,32,ycsb_run) != NULL)  {
        len = strlen(buf);
        buf_fptree[len] = '\0';
        switch(buf[0]){
			case 'I': //insert
				op[t] = 0;
				key[t] = atoll(buf_fptree + 7);
				break;
			case 'D': //delete
				op[t] = 1;
				key[t] = atoll(buf_fptree + 7);
				break;
			case 'U': //update
				op[t] = 2;
				key[t] = atoll(buf_fptree + 7);
				break;
			case 'R': //read
				op[t] = 3;
				key[t] = atoll(buf_fptree + 5);
				break;
		}
        operation_num++;
        t++; 
    }
    fclose(ycsb_run);
    clock_gettime(CLOCK_MONOTONIC, &start);

    // TODO: operate the fptree
    for (i = 0; i < operation_num; i++) {
    	// //增其实就是改 因为key == value 
        // if (op[i] == 0 && op[i] == 2) {
        //     status = db->Put(write_options, to_string(key[i]), to_string(key[i]));
        //     assert(status.ok());
        //     inserted++;
        // }
        // else if(op[i] == 1){
		//     // 删除key
		//     status = db->Delete(leveldb::WriteOptions(), to_string(key[i]));
		//     assert(status.ok());
		// }
        // else if(op[i] == 3){
        //     string val;
        //     status = db->Get(leveldb::ReadOptions(), to_string(key[i]), &val);
        //     assert(status.ok());
        //     queried++;
        // }
		switch(op[i]){
            case 0:
                fptree.insert(key[i],key[i]);
                inserted++;
                break;
            case 1:
                fptree.remove(key[i]);
                break;
            case 2:
                fptree.update(key[i],value[i]);
                break;
            case 3:
                Value value_temp = fptree.find(key[i]);
                queried++;
                break;
        }


    }
	clock_gettime(CLOCK_MONOTONIC, &finish);
	single_time = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Run phase finishes: %d/%d items are inserted/searched\n", inserted, operation_num - inserted);
    printf("Run phase throughput: %f operations per second \n", READ_WRITE_NUM/single_time);	
    
    // LevelDB
    printf("===================LevelDB====================\n");
    const string filePath = ""; // data storing folder(NVM)

    memset(key, 0, 2200000);
    memset(op, 0, 2200000);

    leveldb::DB* db;
    leveldb::Options options;
    leveldb::WriteOptions write_options;
    // TODO: initial the levelDB
    // DONE
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    assert(status.ok());

    inserted = 0;
    queried = 0;
    t = 0;
    printf("Load phase begins \n");
    // TODO: read the ycsb_read
    // DONE
    ycsb_load = (FILE*)malloc(sizeof(FILE));
    if ((ycsb_load = fopen(load.c_str(), "r")) == NULL) {
        printf("cannot open file!\n");
        return -1;
    }

    buf_ycsb = new char[32];
    while (fgets(buf_ycsb,32,ycsb_load) != NULL)  {
        len = strlen(buf_ycsb);
        buf_ycsb[len] = '\0';
       	op[t] = 0;
        key[t] = atoll(buf_ycsb + 7);
        t++;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    // TODO: load the levelDB
    // DONE
    int i;
    for (i = 0; i < READ_WRITE_NUM; i++) {
        if (op[i] == 0) {
            status = db->Put(write_options, to_string(key[i]), to_string(key[i]));
            assert(status.ok());
            inserted++;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &finish);
	single_time = (finish.tv_sec - start.tv_sec) * 1000000000.0 + (finish.tv_nsec - start.tv_nsec);
    printf("Load phase finishes: %d items are inserted \n", inserted);
    printf("Load phase used time: %fs\n", single_time / 1000000000.0);
    printf("Load phase single insert time: %fns\n", single_time / inserted);

	printf("Run phase begin\n");
	operation_num = 0;
    inserted = 0;		
    // TODO: read the ycsb_run
    // DONE
    if ((ycsb_run = fopen(run.c_str(), "r")) == NULL) {
        printf("cannot open file!\n");
        return -1;
    }
    t = 0;
    while (fgets(buf_ycsb,32,ycsb_run) != NULL)  {
        len = strlen(buf_ycsb);
        buf_ycsb[len] = '\0';
        switch(buf_ycsb[0]){
			case 'I': //insert
				op[t] = 0;
				key[t] = atoll(buf_ycsb + 7);
				break;
			case 'D': //delete
				op[t] = 1;
				key[t] = atoll(buf_ycsb + 7);
				break;
			case 'U': //update
				op[t] = 2;
				key[t] = atoll(buf_ycsb + 7);
				break;
			case 'R': //read
				op[t] = 3;
				key[t] = atoll(buf_ycsb + 5);
				break;
		}
        operation_num++;
        t++; 
    }
    clock_gettime(CLOCK_MONOTONIC, &start);

    // TODO: run the workload_run in levelDB
    // DONE
     for (i = 0; i < operation_num; i++) {
    	//增其实就是改 因为key == value 
        if (op[i] == 0 && op[i] == 2) {
            status = db->Put(write_options, to_string(key[i]), to_string(key[i]));
            assert(status.ok());
            inserted++;
        }
        else if(op[i] == 1){
		    // 删除key
		    status = db->Delete(leveldb::WriteOptions(), to_string(key[i]));
		    assert(status.ok());
		}
        else if(op[i] == 3){
            string val;
            status = db->Get(leveldb::ReadOptions(), to_string(key[i]), &val);
            assert(status.ok());
            queried++;
        }
		
    }
	clock_gettime(CLOCK_MONOTONIC, &finish);
    fclose(ycsb_read);
	single_time = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Run phase finishes: %d/%d items are inserted/searched\n", inserted, operation_num - inserted);
    printf("Run phase throughput: %f operations per second \n", READ_WRITE_NUM/single_time);	
    return 0;
}
