//
// Created by Yuchen Zhang on 2/22/23.
//



#include "library.h"


bool deleteRecord(RecordID* recordId, Heapfile *heapfile){
    Record record;
    Page* page = new Page;
    init_fixed_len_page(page, heapfile->page_size, SLOT_SIZE);
    if(recordId->page_id > maxPid(heapfile)){
        return false;
    }
    read_page(heapfile, recordId->page_id, page);

    if(recordId->slot > fixed_len_page_capacity(page)){
        return false;
    }

    delete_slot(page, recordId->slot);

    write_page(page, heapfile, recordId->page_id);

    return true;
}


//<heapfile> <record_id> <page_size>
int main(int argc, char* argv[]){
    auto startTime = system_clock::now();
    FILE *heapFile = ::fopen(argv[1], "r+");
    string record_id = argv[2];
    auto* recordId = new RecordID;
    for(int i = 0; i < record_id.size(); ++i){
        if(record_id[i] == '-'){
            recordId->page_id = stoi(record_id.substr(0, i+1));
            recordId->slot = stoi(record_id.substr(i+1, record_id.size()-i-1));
        }
    }
    int page_size = atoi(argv[3]);
    // Initialize heapfile
    auto* heapfile = new Heapfile;
    init_heapfile(heapfile, page_size, heapFile);

    if(!deleteRecord(recordId, heapfile)){
        cerr << "ERROR";
    }
    auto endTime = system_clock::now();
    auto duration = duration_cast<microseconds>(endTime - startTime);
    std::cout << "TIME: " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;
}
