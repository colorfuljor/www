#include"fptree/fptree.h"

using namespace std;

// Initial the new InnerNode
InnerNode::InnerNode(const int& d, FPTree* const& t, bool _isRoot) {
    // TODO:
    this->isRoot = _isRoot;
    this->keys = new Key(2 * d + 1);
    this->childrens = new Node*(2 * d + 2);
    this->nKeys = 0;
    this->nChild = 0;
}

// delete the InnerNode
InnerNode::~InnerNode() {
    // TODO:
    delete this->nChild;
    delete this->nKeys;
}

// binary search the first key in the innernode larger than input key
int InnerNode::findIndex(const Key& k) {
    // TODO:
    int low = 1, high = nKeys, mid;
    while(low < high){
        mid = (low + high) / 2;
        if (keys[mid] == k) {
            return mid;
        } else if (keys[mid] > k) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

// insert the node that is assumed not full
// insert format:
// ======================
// | key | node pointer |
// ======================
// WARNING: can not insert when it has no entry
void InnerNode::insertNonFull(const Key& k, Node* const& node) {
    // TODO:
}

// insert func
// return value is not NULL if split, returning the new child and a key to insert
KeyNode* InnerNode::insert(const Key& k, const Value& v) {
    KeyNode* newChild = NULL;

    // 1.insertion to the first leaf(only one leaf)
    if (this->isRoot && this->nKeys == 0) {
        // TODO:
        LeafNode 
        childrens[1] = ;
        return newChild;
    }
    
    // 2.recursive insertion
    // TODO:
    return newChild;
}

// ensure that the leaves inserted are ordered
// used by the bulkLoading func
// inserted data: | minKey of leaf | LeafNode* |
KeyNode* InnerNode::insertLeaf(const KeyNode& leaf) {
    KeyNode* newChild = NULL;
    // first and second leaf insertion into the tree
    if (this->isRoot && this->nKeys == 0) {
        // TODO:
        return newChild;
    }
    
    // recursive insert
    // Tip: please judge whether this InnerNode is full
    // next level is not leaf, just insertLeaf
    // TODO:

    // next level is leaf, insert to childrens array
    // TODO:

    return newChild;
}

KeyNode* InnerNode::split() {
    KeyNode* newChild = new KeyNode();
    // right half entries of old node to the new node, others to the old node. 
    // TODO:

    return newChild;
}

// remove the target entry
// return TRUE if the children node is deleted after removement.
// the InnerNode need to be redistributed or merged after deleting one of its children node.
bool InnerNode::remove(const Key& k, const int& index, InnerNode* const& parent, bool &ifDelete) {
    bool ifRemove = false;
    // only have one leaf
    // TODO:
    
    // recursive remove
    // TODO:
    return ifRemove;
}

// If the leftBro and rightBro exist, the rightBro is prior to be used
void InnerNode::getBrother(const int& index, InnerNode* const& parent, InnerNode* &leftBro, InnerNode* &rightBro) {
    // TODO:
}

// merge this node, its parent and left brother(parent is root)
void InnerNode::mergeParentLeft(InnerNode* const& parent, InnerNode* const& leftBro) {
    // TODO:
}

// merge this node, its parent and right brother(parent is root)
void InnerNode::mergeParentRight(InnerNode* const& parent, InnerNode* const& rightBro) {
    // TODO:
}

// this node and its left brother redistribute
// the left has more entries
void InnerNode::redistributeLeft(const int& index, InnerNode* const& leftBro, InnerNode* const& parent) {
    // TODO:
}

// this node and its right brother redistribute
// the right has more entries
void InnerNode::redistributeRight(const int& index, InnerNode* const& rightBro, InnerNode* const& parent) {
    // TODO:
}

// merge all entries to its left bro, delete this node after merging.
void InnerNode::mergeLeft(InnerNode* const& leftBro, const Key& k) {
    // TODO:
}

// merge all entries to its right bro, delete this node after merging.
void InnerNode::mergeRight(InnerNode* const& rightBro, const Key& k) {
    // TODO:
}

// remove a children from the current node, used by remove func
void InnerNode::removeChild(const int& keyIdx, const int& childIdx) {
    // TODO:
}

// update the target entry, return true if the update succeed.
bool InnerNode::update(const Key& k, const Value& v) {
    // TODO:
    return false;
}

// find the target value with the search key, return MAX_VALUE if it fails.
Value InnerNode::find(const Key& k) {
    // TODO:
    return MAX_VALUE;
}

// get the children node of this InnerNode
Node* InnerNode::getChild(const int& idx) {
    // TODO:
    return NULL;
}

// get the key of this InnerNode
Key InnerNode::getKey(const int& idx) {
    if (idx < this->nKeys) {
        return this->keys[idx];
    } else {
        return MAX_KEY;
    }
}

// print the InnerNode
void InnerNode::printNode() {
    cout << "||#|";
    for (int i = 0; i < this->nKeys; i++) {
        cout << " " << this->keys[i] << " |#|";
    }
    cout << "|" << "    ";
}

// print the LeafNode
void LeafNode::printNode() {
    cout << "||";
    for (int i = 0; i < 2 * this->degree; i++) {
        if (this->getBit(i)) {
            cout << " " << this->kv[i].k << " : " << this->kv[i].v << " |";
        }
    }
    cout << "|" << " ====>> ";
}

// new a empty leaf and set the valuable of the LeafNode
LeafNode::LeafNode(FPTree* t) {
    // DONE
    tree = t;
    isLeaf = true;
    degree = LEAF_DEGREE;

    int n = LEAF_DEGREE * 2;
    int bitArrNum = (n + 7) / 8;    
    pAllocator->getLeaf(pPointer,pmem_addr);
    
    // the pointer below are all pmem address based on pmem_addr
    bitmap = (Byte*)pmem_addr;
    pNext = (PPointer*)pmem_addr + bitArrNum;
    fingerprints = (Byte*)pNext + sizeof(PPointer);
    kv = (KeyValue*)fingerprints + n * sizeof(Byte);

    n = 0;
    prev = next = NULL;
    filePath = DATA_DIR + to_string(pPointer.fileId);
    bitmapSize = 0;
}

// reload the leaf with the specific Persistent Pointer
// need to call the PAllocator
LeafNode::LeafNode(PPointer p, FPTree* t) {
    // DONE
    tree = t;
    isLeaf = true;
    degree = LEAF_DEGREE;

    int n = LEAF_DEGREE * 2;
    int bitArrNum = (n + 7) / 8;    
   
    pPointer = p;
    pmem_addr = pAllocator->getLeafPmemAddr(p);
    
    // the pointer below are all pmem address based on pmem_addr
    bitmap = (Byte*)pmem_addr;
    pNext = (PPointer*)pmem_addr + bitArrNum;
    fingerprints = (Byte*)pNext + sizeof(PPointer);
    kv = (KeyValue*)fingerprints + n * sizeof(Byte);

    n = 0;
    prev = next = NULL;
    filePath = DATA_DIR + to_string(pPointer.fileId);
    bitmapSize = bitArrNum;
}

LeafNode::~LeafNode() {
    // TODO:
}

// insert an entry into the leaf, need to split it if it is full
KeyNode* LeafNode::insert(const Key& k, const Value& v) {
    KeyNode* newChild = NULL;
    // TODO:
    return newChild;
}

// insert into the leaf node that is assumed not full
void LeafNode::insertNonFull(const Key& k, const Value& v) {
    // DONE
    /*From Oukid_FPTree.pdf Algorithm2:
        slot = Leaf.Bitmap.FindFirstZero();
        Leaf.KV[slot] = (K, V); Leaf.Fingerprints[slot] = hash(K);
        //Persist(Leaf.KV[slot]); Persist(Leaf.Fingerprints[slot]);
        Leaf.Bitmap[slot] = 1; 
        //Persist(Leaf.Bitmap);
    */
    int slot = findFirstZero();
    kv[slot].k = k;
    kv[slot].v = v;
    fingerprints[slot] = keyHash(k);
    bitmap[slot / 8] |= 0x01>>(7-(slot % 8));
    n++;
}

// split the leaf node
KeyNode* LeafNode::split() {
    KeyNode* newChild = new KeyNode();
    // TODO:
    return newChild;
}

// use to find a mediant key and delete entries less then middle
// called by the split func to generate new leaf-node
// qsort first then find
Key LeafNode::findSplitKey() {
    Key midKey = 0;
    // TODO:
    return midKey;
}

// get the targte bit in bitmap
// TIPS: bit operation
int LeafNode::getBit(const int& idx) {
    // DONE
    int nshift = 7-(idx%8);
    return ((bitmap[idx/8] >> nshift) & 0x01);
}

Key LeafNode::getKey(const int& idx) {
    return this->kv[idx].k;
}

Value LeafNode::getValue(const int& idx) {
    return this->kv[idx].v;
}

PPointer LeafNode::getPPointer() {
    return this->pPointer;
}

// remove an entry from the leaf
// if it has no entry after removement return TRUE to indicate outer func to delete this leaf.
// need to call PAllocator to set this leaf free and reuse it
bool LeafNode::remove(const Key& k, const int& index, InnerNode* const& parent, bool &ifDelete) {
    bool ifRemove = false;
    // TODO:
    return ifRemove;
}

// update the target entry
// return TRUE if the update succeed
bool LeafNode::update(const Key& k, const Value& v) {
    bool ifUpdate = false;
    // TODO:
    return ifUpdate;
}

// if the entry can not be found, return the max Value
Value LeafNode::find(const Key& k) {
    // TODO:
    return MAX_VALUE;
}

// find the first empty slot
int LeafNode::findFirstZero() {
    // DONE
    for(int i = 0; i < LEAF_DEGREE*2; i++)
    {   
        if( getBit(i) == 0)
            return i;
    }
    return -1;
}

// persist the entire leaf
// use PMDK
void LeafNode::persist() {
    // TODO:
}

// call by the ~FPTree(), delete the whole tree
void FPTree::recursiveDelete(Node* n) {
    if (n->isLeaf) {
        delete n;
    } else {
        for (int i = 0; i < ((InnerNode*)n)->nChild; i++) {
            recursiveDelete(((InnerNode*)n)->childrens[i]);
        }
        delete n;
    }
}

FPTree::FPTree(uint64_t t_degree) {
    FPTree* temp = this;
    this->root = new InnerNode(t_degree, temp, true);
    this->degree = t_degree;
    bulkLoading();
}

FPTree::~FPTree() {
    recursiveDelete(this->root);
}

// get the root node of the tree
InnerNode* FPTree::getRoot() {
    return this->root;
}

// change the root of the tree
void FPTree::changeRoot(InnerNode* newRoot) {
    this->root = newRoot;
}

void FPTree::insert(Key k, Value v) {
    if (root != NULL) {
        root->insert(k, v);
    }
}

bool FPTree::remove(Key k) {
    if (root != NULL) {
        bool ifDelete = false;
        InnerNode* temp = NULL;
        return root->remove(k, -1, temp, ifDelete);
    }
    return false;
}

bool FPTree::update(Key k, Value v) {
    if (root != NULL) {
        return root->update(k, v);
    }
    return false;
}

Value FPTree::find(Key k) {
    if (root != NULL) {
        return root->find(k);
    }
}

// call the InnerNode and LeafNode print func to print the whole tree
// TIPS: use Queue
void FPTree::printTree() {
    // TODO:
}

// bulkLoading the leaf files and reload the tree
// need to traverse leaves chain
// if no tree is reloaded, return FALSE
// need to call the PALlocator
bool FPTree::bulkLoading() {
    // TODO:
    return false;
}
