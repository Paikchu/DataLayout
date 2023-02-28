//
// Created by Yuchen Zhang on 2/22/23.
//
#include "library.h"
//<heapfile> <attribute_id> <start> <end> <page_size>
int main(int argc, char* argv[]){
    auto startTime = system_clock::now();
    FILE* heapFile = fopen(argv[1], "r+");
    int attribute_id = atoi(argv[2]);
    string start = argv[3];
    string end = argv[4];
    int page_size = atoi(argv[5]);

    // Initialize heapfile
    auto* heapfile = new Heapfile;
    init_heapfile(heapfile, page_size, heapFile);

    RecordIterator recordIterator(heapfile);

    Record record;

    vector<string> ans;

    int numOfRecord = 0;

    while(recordIterator.hasNext()){
        record = recordIterator.next();
        if(record[attribute_id] >= start && record[attribute_id] <= end){
            string temp = record[attribute_id];
            ans.push_back(temp.substr(0,5));
            numOfRecord+=1;
        }
    }
    for(auto i : ans){
        cout << i << endl;
    }
    cout << "NUMBER OF RECORDS: " << numOfRecord << endl;
    auto endTime = system_clock::now();
    auto duration = duration_cast<microseconds>(endTime - startTime);
    std::cout << "TIME: " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;

}