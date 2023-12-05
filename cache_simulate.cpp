#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <bitset>
using namespace std;

const long long MAX_L1_SETS = 2048, MAX_L2_SETS = 16384;
long long BLOCKSIZE, L1_SIZE, L1_ASSOC, L2_SIZE, L2_ASSOC;

struct Block{
    bool valid;
    bool dirty;
    long long tag;
    unsigned long long data;
    long long lru;
    Block(){
        valid = false;
        dirty = false;
        tag = -1;
        data = -1;
        lru = 0;
    }
};

struct Set {
    vector<Block> set_block;
    long long associativity;
    Set(long long assoc){
        set_block.resize(assoc);
        associativity = assoc;
    }
    Block* find_block(long long tag){
        for(long long i=0;i<associativity;i++){
            if(set_block[i].valid && set_block[i].tag == tag){
                set_block[i].lru = 0;
                return &set_block[i];
            }
        }
        return nullptr;
    }
    Block* find_lru_block(){
        long long max_lru = -1;
        long long lru_block_index = -1;
        for(long long i=0;i<associativity;i++){
            if (!set_block[i].valid) {
                return &set_block[i];
            }
            if (set_block[i].lru > max_lru) {
                max_lru = set_block[i].lru;
                lru_block_index = i;
            }
        }
        return &set_block[lru_block_index];
    }
    bool lru_block(){
        long long max_lru = -1;
        long long lru_block_index = -1;
        for(long long i=0;i<associativity;i++){
            if (!set_block[i].valid) {
                return true;
            }
            if (set_block[i].lru > max_lru) {
                max_lru = set_block[i].lru;
                lru_block_index = i;
            }
        }
        return false;
    }
    void update_lru(Block* block){
        for(long long i=0;i<associativity;i++){
            if(set_block[i].valid ){
                set_block[i].lru++;
            }
        }
        block->lru = 0;
    }
};

struct Cache{
    long long num_sets;
    long long assoc;
    long long size;
    long long reads;
    long long read_miss;
    long long writes;
    long long write_miss;
    long long block_size;
    long long writebacks = 0;
    bool write_back;
    vector<Set> cache;

    Cache(long long block_size,long long assoc,long long size){
        this->block_size = block_size;
        this->assoc = assoc;
        this->size = size;
        this->num_sets = size / (block_size * assoc);
        cache.resize(num_sets,Set(assoc));
        reads = read_miss = writes = write_miss = 0;
    }
    Block* update_block(long long tag,long long index,long long address){
        Set* set = &cache[index];
        Block* block = set->find_lru_block();
        Block* checkker = new Block;
        if (block->valid){
            *checkker = *block;
        }else{
            checkker = nullptr;
        }
        if (block->dirty && block->valid){
            writebacks++;
        }
        block->valid = true;
        block->tag = tag;
        block->dirty= false;
        block->data = address;
        set->update_lru(block);
        return checkker;
    }
    Block* update_block_dirty(long long tag,long long index,long long address){
        Set* set = &cache[index];
        Block* block = set->find_lru_block();
        Block* checkker = new Block;
        if (block->valid){
            *checkker = *block;
        }else{
            checkker = nullptr;
        }
        if (block->dirty && block->valid){
            writebacks++;
        }
        block->valid = true;
        block->tag = tag;
        block->dirty= true;
        block->data = address;
        set->update_lru(block);
        return checkker;
    }
    bool update_check(long long tag,long long index,bool dirty,long long address){
        Set* set = &cache[index];
        Block* block = set->find_block(tag);
        writes++;
        if (block==nullptr){
            Block* block1 = set->find_lru_block();
            if (block1->dirty && block1->valid){
                writebacks++;
            }if (dirty){
                write_miss++;
            }
            block1->valid = true;
            block1->tag = tag;
            block1->dirty = dirty;
            block1->data = address;
            set->update_lru(block1);  
            return false;
        }else{
            block->dirty = dirty;
            set->update_lru(block);

            return true;
        }
    }
    bool read_block(long long tag,long long index){
        Set* set = &cache[index];
        Block* block = set->find_block(tag);    
        reads++;
        if (block==nullptr){
            read_miss++;
            return false;
        }else{
            set->update_lru(block);
            return true;
        }
    }
    bool write_block(long long tag,long long index){
        Set* set = &cache[index];
        Block* block = set->find_block(tag);    
        writes++;
        if (block==nullptr){
            write_miss++;
            return false;
        }else{
            block->dirty = true;
            set->update_lru(block);
            return true;
        }
    }
    void print_stats_1(){
        // cout << "reads: " << reads << endl;
        // cout << "Cache misses: " << read_miss << endl;
        // cout << "writes: " << writes << endl;
        // cout << "writes misses: " << write_miss << endl;
        // cout << "Cache miss rate: " << (double)(read_miss + write_miss)/(reads+writes)<< endl;
        // cout << "Writebacks: " << writebacks << endl;
        cout << "\ni. number of l1 reads:\t\t\t\t" << dec << reads;
        cout << "\nii. number l1 read misses:\t\t\t" << dec << read_miss;
        cout << "\niii. number of l1 writes:\t\t\t" << dec << writes;
        cout << "\niv. number of l1 write misses:\t\t\t" << dec << write_miss;
        cout << "\nv. L1 miss rate:\t\t\t\t" << fixed << setprecision(4) << (double)(read_miss + write_miss)/(reads+writes);
        cout << "\nvi. number of writebacks from l1 memory:\t" << dec <<  writebacks;
    }
        void print_stats_2(){
        // cout << "reads: " << reads << endl;
        // cout << "Cache misses: " << read_miss << endl;
        // cout << "writes: " << writes << endl;
        // cout << "writes misses: " << write_miss << endl;
        // cout << "Cache miss rate: " << (double)(read_miss + write_miss)/(reads+writes)<< endl;
        // cout << "Writebacks: " << writebacks << endl;
        cout << "\nvii. number of l2 reads:\t\t\t" << dec << reads;
        cout << "\nviii. number l2 read misses:\t\t\t" << dec << read_miss;
        cout << "\nix. number of l2 writes:\t\t\t" << dec << writes;
        cout << "\nx. number of l2 write misses:\t\t\t" << dec << write_miss;
        cout << "\nxi. L2 miss rate:\t\t\t\t" << fixed << setprecision(4) << (double)(read_miss + write_miss)/(reads+writes);
        cout << "\nxii. number of writebacks from l2 memory:\t" << dec <<  writebacks;
    }
};

int main(int argc, char* argv[]){
    if (argc != 7){
        cout << "Invalid number of arguments" << endl;
        return -1;
    }
    BLOCKSIZE = atoi(argv[1]);
    L1_SIZE =  atoi(argv[2]);
    L1_ASSOC = atoi(argv[3]);
    L2_SIZE = atoi(argv[4]);   
    L2_ASSOC = atoi(argv[5]);
    Cache l1(BLOCKSIZE,L1_ASSOC,L1_SIZE);
    Cache l2(BLOCKSIZE,L2_ASSOC,L2_SIZE);
    ifstream infile(argv[6]);
    string line;
    while (getline(infile, line)){
        stringstream ss(line);
        string op;
        unsigned long long address;
        ss >> op >> hex >> address;
        long long tag,index,tag1,index1,tag2,index2;
        tag = address >> ((long long)log2((long long)BLOCKSIZE)+(long long)log2(l1.num_sets));
        index = (address >> (long long)log2(BLOCKSIZE)) & ((1 << (long long)log2(l1.num_sets))-1);
                bitset<32> binary(tag);
        if (op =="r" or op == "R"){
            bool check1 = l1.read_block(tag,index);
            if (!check1){
                tag2 = address >> ((long long)log2((long long)BLOCKSIZE)+(long long)log2(l2.num_sets));
                index2 = (address >> (long long)log2(BLOCKSIZE)) & ((1 << (long long)log2(l2.num_sets))-1);
                bool check2 = l2.read_block(tag2,index2);
                if (!check2){
                    l2.update_block(tag2,index2,address);
                }
                Block* block = l1.update_block(tag,index,address);
                if (block != nullptr){
                    tag1 = block->data >> ((long long)log2((long long)BLOCKSIZE)+(long long)log2(l2.num_sets));
                    index1 = (block->data >> (long long)log2(BLOCKSIZE) )& ((1 << (long long)log2(l2.num_sets))-1);
                    if(block->dirty){
                        l2.update_check(tag1,index1,block->dirty,block->data);
                    }
                }
            }
        }else{
            bool check1 = l1.write_block(tag,index);
            if (!check1){
                tag2 = address >> ((long long)log2((long long)BLOCKSIZE)+(long long)log2(l2.num_sets));
                index2 = (address >> (long long)log2(BLOCKSIZE)) & ((1 << (long long)log2(l2.num_sets))-1);
                bool check2 = l2.read_block(tag2,index2);
                if (!check2){
                    l2.update_block_dirty(tag2,index2,address);
                }
                Block* block = l1.update_block_dirty(tag,index,address);
                if (block != nullptr){
                    tag1 = block->data >> ((long long)log2((long long)BLOCKSIZE)+(long long)log2(l2.num_sets));
                    index1 = (block->data >> (long long)log2(BLOCKSIZE)) & ((1 << (long long)log2(l2.num_sets))-1);                       
                    if(block->dirty){
                        l2.update_check(tag1,index1,block->dirty,block->data);
                    }
                }
            }
        }
    }
    // cout<<"<-----------stats for L1----------->"<<endl;
    // l1.print_stats();
    // cout<<"<-----------stats for L2----------->"<<endl;
    // l2.print_stats();
    cout << "\n\n===== Simulation Results =====";
    l1.print_stats_1(); 
    l2.print_stats_2();
    return 0;
    // ( l1 reads + l1 writes )* 1 + (l1 writemiss + l1readmiss + l1writeback)*20 + (l2 writemiss + l2readmiss + l2writeback)*200
}
