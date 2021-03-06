#include"utility/p_allocator.h"
#include<iostream>
using namespace std;

// the file that store the information of allocator
const string P_ALLOCATOR_CATALOG_NAME = "p_allocator_catalog";
// a list storing the free leaves
const string P_ALLOCATOR_FREE_LIST    = "free_list";

PAllocator* PAllocator::pAllocator = new PAllocator();

PAllocator* PAllocator::getAllocator() {
    if (PAllocator::pAllocator == NULL) {
        PAllocator::pAllocator = new PAllocator();
    }
    return PAllocator::pAllocator;
}

/* data storing structure of allocator
   In the catalog file, the data structure is listed below
   | maxFileId(8 bytes) | freeNum = m | treeStartLeaf(the PPointer) |
   In freeList file:
   | freeList{(fId, offset)1,...(fId, offset)m} |
*/
PAllocator::PAllocator() {
    string allocatorCatalogPath = DATA_DIR + P_ALLOCATOR_CATALOG_NAME;
    string freeListPath         = DATA_DIR + P_ALLOCATOR_FREE_LIST;
    ifstream allocatorCatalog(allocatorCatalogPath, ios::in|ios::binary);
    ifstream freeListFile(freeListPath, ios::in|ios::binary);
    // judge if the catalog exists
    if (allocatorCatalog.is_open() && freeListFile.is_open()) {
        // exist
        // TODO:finished
        // printf("exist\n");
        //读取catalog file
        allocatorCatalog.read((char *)(&maxFileId), sizeof(maxFileId));
        allocatorCatalog.read((char *)(&freeNum), sizeof(freeNum));
        allocatorCatalog.read((char *)(&startLeaf), sizeof(startLeaf));
        //读取freelist file
 
        for(uint64_t i = 0; i< freeNum; i++)
        {
            PPointer buf;
            freeListFile.read((char *)(&buf), sizeof(buf));
            freeList.push_back(buf);
        }

        /*while(!freeListFile.eof()) {
            PPointer buf;
            allocatorCatalog.read((char *)(&buf), sizeof(buf));
            freeList.push_back(buf);
        }*/
    } else {
        // not exist, create catalog and free_list file, then open.
        // TODO:finished
        // printf("not exist\n");
        ofstream catalog(allocatorCatalogPath, ios::out);
        ofstream freelist(freeListPath, ios::out);
        //创建catalog file
        catalog.close();
        //创建freeList file
        freelist.close();
        //以读的方式打开文件
        // allocatorCatalog.open(allocatorCatalogPath, ios::in|ios::binary);
        // freeListFile.open(freeListPath, ios::in|ios::binary);
        //初始化变量
        maxFileId = 1;
        freeNum = 0;
    }
    this->initFilePmemAddr();
}

PAllocator::~PAllocator() {
    // TODO:finished
        persistCatalog();
        //persist freeList
        string freeListPath = DATA_DIR + P_ALLOCATOR_FREE_LIST;
        ofstream freelist(freeListPath, ios::out|ios::binary);
        uint64_t i;
        for (i = 0; i < freeNum; i++) {
            PPointer toWrite = freeList[i];
            freelist.write((char *)(&toWrite), sizeof(toWrite)); 
        }
        freelist.close();
        //if(pAllocator != NULL)
        // delete pAllocator;
        pAllocator=NULL;
  
    
}

// memory map all leaves to pmem address, storing them in the fId2PmAddr
void PAllocator::initFilePmemAddr() {
    // TODO:finished
    for(uint64_t i = 1; i < maxFileId; i++)
    {
        // size_t mapped_len;
        // int is_pmem;
        size_t len =LEAF_GROUP_HEAD+LEAF_GROUP_AMOUNT*calLeafSize();
        string fileIdPath=DATA_DIR + to_string(i);
        if ((fId2PmAddr[i] = (char*)pmem_map_file(fileIdPath.c_str(), 
           len, PMEM_FILE_CREATE,0666,NULL, NULL)) == NULL) 
        {
		    perror("pmem_map_file");
		    exit(1);
	    }
    }
}

// get the pmem address of the target PPointer from the map fId2PmAddr
char* PAllocator::getLeafPmemAddr(PPointer p) {
    // TODO:finished
    if (p.fileId <=maxFileId && p.fileId != ILLEGAL_FILE_ID)
        return fId2PmAddr[p.fileId]+p.offset;
    return NULL;
}

// get and use a leaf for the fptree leaf allocation
// return 
bool PAllocator::getLeaf(PPointer &p, char* &pmem_addr) {
    // TODO:finished
    if (freeList.empty())
        newLeafGroup();
    
    p = freeList.back();
    freeList.pop_back();
    
    pmem_addr = getLeafPmemAddr(p);
    freeNum--;
    string path=DATA_DIR + to_string(p.fileId);
    fstream leafGroup(path,ios::in|ios::out|ios::binary);
    uint64_t usedNum ;
    uint8_t bitmap[LEAF_GROUP_AMOUNT];
    leafGroup.read((char*)&usedNum,sizeof(usedNum));
    leafGroup.read((char*)&bitmap,sizeof(bitmap));
    usedNum++;
    bitmap[(p.offset-LEAF_GROUP_HEAD)/calLeafSize()] = 1;
    leafGroup.seekg(0,ios::beg);
    leafGroup.write((char*)&usedNum,sizeof(usedNum));
    leafGroup.write((char*)&bitmap,sizeof(bitmap));
    leafGroup.close();
    if (maxFileId == 2 && usedNum == 1) {
        startLeaf = p;
    }
    persistCatalog();
    return true;
}

bool PAllocator::ifLeafUsed(PPointer p) {
    // TODO:finished
    return ifLeafExist(p) && !ifLeafFree(p);
}

bool PAllocator::ifLeafFree(PPointer p) {
    // TODO:finished
    if (!ifLeafExist(p)) return false;
    Byte bit = 0;
    string path = DATA_DIR + to_string(p.fileId);
    ifstream in(path, ios::binary|ios::in);
    in.seekg(sizeof(uint64_t) + ((p.offset - LEAF_GROUP_HEAD) / calLeafSize()), ios::beg);
    in.read((char*)&(bit), sizeof(Byte));
    if (bit == 1) return false;
    return true;
}

// judge whether the leaf with specific PPointer exists. 
bool PAllocator::ifLeafExist(PPointer p) {
    // TODO:finished
    if (p.fileId < maxFileId && p.fileId != ILLEGAL_FILE_ID)
        return true;
    return false;
}

// free and reuse a leaf
bool PAllocator::freeLeaf(PPointer p) {
    // TODO:finished
    Byte bit = 0;
    uint64_t usedNum;
    string path = DATA_DIR + to_string(p.fileId);
    fstream in(path, ios::binary|ios::out|ios::in);
    if (!in.good()) 
        return false;
    in.read((char*)&(usedNum), sizeof(uint64_t));
    usedNum--;
    in.seekg(0,ios::beg);
    in.write((char*)&(usedNum), sizeof(uint64_t));
    in.seekg(sizeof(uint64_t) + ((p.offset - LEAF_GROUP_HEAD) / calLeafSize()), ios::beg);
    in.write((char*)&(bit), sizeof(Byte));
    freeList.push_back(p);
    freeNum++;
    if (startLeaf == p) {
        int bitmapSize = LEAF_DEGREE * 2 / 8;
        startLeaf = *((PPointer*)(getLeafPmemAddr(p) + bitmapSize));
    }
    persistCatalog();
    return true;
}

bool PAllocator::persistCatalog() {
    // TODO:finished
    string allocatorCatalogPath = DATA_DIR + P_ALLOCATOR_CATALOG_NAME;
    ofstream allocatorCatalog(allocatorCatalogPath, ios::out|ios::binary);
    if (!allocatorCatalog)
        return false;
    allocatorCatalog.write((char *)(&maxFileId), sizeof(maxFileId));
    allocatorCatalog.write((char *)(&freeNum), sizeof(freeNum));
    allocatorCatalog.write((char *)(&startLeaf), sizeof(startLeaf));
    return true;
}

/*
  Leaf group structure: (uncompressed)
  | usedNum(8b) | bitmap(n * byte) | leaf1 |...| leafn |
*/
// create a new leafgroup, one file per leafgroup
bool PAllocator::newLeafGroup() {
    // TODO:finished
    string fileIdPath=DATA_DIR + to_string(maxFileId);
    ofstream leafGroup(fileIdPath,ios::out|ios::binary);
    if(leafGroup.is_open())
    {
        uint64_t usedNum = 0;
        uint8_t bitmap[LEAF_GROUP_AMOUNT*(1+calLeafSize())] = {0};        
        leafGroup.write((char*)&usedNum,sizeof(usedNum));
        leafGroup.write((char*)&bitmap,sizeof(bitmap));
        int i = 0;
        for (i = 0; i < LEAF_GROUP_AMOUNT; i++) {
            PPointer p;
            p.fileId = maxFileId;
            p.offset = LEAF_GROUP_HEAD + i * calLeafSize();
            freeList.push_back(p);
        } 
    int len = 8+LEAF_GROUP_AMOUNT*(1+calLeafSize());
    if ((fId2PmAddr[maxFileId] = (char*)pmem_map_file(fileIdPath.c_str(), 
        len, PMEM_FILE_CREATE,0666,NULL, NULL)) == NULL) 
    {
        perror("pmem_map_file");
        exit(1);
    }
        freeNum += LEAF_GROUP_AMOUNT;   
        maxFileId++;    
        leafGroup.close();
        persistCatalog();
        return true;
    }
    return false;
}