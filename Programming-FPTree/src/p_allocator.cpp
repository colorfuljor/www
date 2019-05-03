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
    return PAllocator::pAllocator;;
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
        // TODO:
        printf("exist\n");
        //读取catalog file
        allocatorCatalog.read((char *)(&maxFileId), sizeof(maxFileId));
        allocatorCatalog.read((char *)(&freeNum), sizeof(freeNum));
        allocatorCatalog.read((char *)(&startLeaf), sizeof(startLeaf));
        //读取freelist file
        while(!freeListFile.eof()) {
            PPointer buf;
            allocatorCatalog.read((char *)(&buf), sizeof(buf));
            freeList.push_back(buf);
        }
    } else {
        // not exist, create catalog and free_list file, then open.
        // TODO:
        printf("not exist\n");
        ofstream fcreate;
        //创建catalog file
        fcreate.open(allocatorCatalogPath);
        fcreate.close();
        //创建freeList file
        fcreate.open(freeListPath);
        createFile.close();
        //以读的方式打开文件
        allocatorCatalog.open(allocatorCatalogPath, ios::in|ios::binary);
        freeListFile.open(freeListPath, ios::in|ios::binary);
    }
    this->initFilePmemAddr();
}

PAllocator::~PAllocator() {
    // TODO:
    persistCatalog();
}

// memory map all leaves to pmem address, storing them in the fId2PmAddr
void PAllocator::initFilePmemAddr() {
    // TODO:
    uint64_t i = startLeaf;
    for ()
}

// get the pmem address of the target PPointer from the map fId2PmAddr
char* PAllocator::getLeafPmemAddr(PPointer p) {
    // TODO:
    if (p.field < maxFileId && p.field != ILLEGAL_FILE_ID)
        return fId2PmAddr.find(p.field)->second;
    return NULL;
}

// get and use a leaf for the fptree leaf allocation
// return 
bool PAllocator::getLeaf(PPointer &p, char* &pmem_addr) {
    // TODO:
    return false;
}

bool PAllocator::ifLeafUsed(PPointer p) {
    // TODO:
    return false;
}

bool PAllocator::ifLeafFree(PPointer p) {
    // TODO:
    return false;
}

// judge whether the leaf with specific PPointer exists. 
bool PAllocator::ifLeafExist(PPointer p) {
    // TODO:
}

// free and reuse a leaf
bool PAllocator::freeLeaf(PPointer p) {
    // TODO:
    return false;
}

bool PAllocator::persistCatalog() {
    // TODO:
    return false;
}

/*
  Leaf group structure: (uncompressed)
  | usedNum(8b) | bitmap(n * byte) | leaf1 |...| leafn |
*/
// create a new leafgroup, one file per leafgroup
bool PAllocator::newLeafGroup() {
    // TODO:
    return false;
}