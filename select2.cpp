//
// Created by FZQ on 2/27/2023.
//

#include "library.h"
// <colstore_name> <attribute_id> <start> <end> <page_size>
int main(int argc, char* argv[]) {
    auto startTime = system_clock::now();
    string colstore_name = argv[1];
    int attribute_id = atoi(argv[2]);
    string start = argv[3];
    string end = argv[4];
    int page_size = atoi(argv[5]);

    string file_path = colstore_name + "/" + to_string(attribute_id);
    FILE* heapFile = fopen(file_path.c_str(), "r+");

    // Initialize heapfile
    auto* heapfile = new Heapfile;
    init_heapfile(heapfile, page_size, heapFile);

    RecordIterator recordIterator(heapfile);

    Record record;

    vector<string> ans;
    int numOfRecord = 0;

    while(recordIterator.hasNext()){
        record = recordIterator.next();
//        cout << 1111 << endl;
        // cout << record[attribute_id] << endl;
        if(record[0] >= start && record[0] <= end){
            string temp = record[0];
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
