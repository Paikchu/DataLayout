//
// Created by FZQ on 2/27/2023.
//
#include "library.h"
int main(int argc, char* argv[]) {
    // select3  <colstore_name> <attribute_id> <return_attribute_id> <start> <end> <page_size>

    auto startTime = system_clock::now();
    string colstore_name = argv[1];
    int attribute_id = atoi(argv[2]);
    int return_attribute_id = atoi(argv[3]);
    string start = argv[4];
    string end = argv[5];
    int page_size = atoi(argv[6]);

    string file_path_a = colstore_name + "/" + to_string(attribute_id);
    string file_path_b = colstore_name + "/" + to_string(return_attribute_id);

    FILE* heapFileA = fopen(file_path_a.c_str(), "r+");
    FILE* heapFileB = fopen(file_path_b.c_str(), "r+");

    // Initialize heapfile
    auto* heapfilea = new Heapfile;
    auto* heapfileb = new Heapfile;
    init_heapfile(heapfilea, page_size, heapFileA);
    init_heapfile(heapfileb, page_size, heapFileB);

    RecordIterator recordIteratora(heapfilea);
    RecordIterator recordIteratorb(heapfileb);

    Record recorda, recordb;
    vector<string> ans;
    int numOfRecord = 0;

    while (recordIteratora.hasNext() && recordIteratorb.hasNext()) {
        recorda = recordIteratora.next();
        recordb = recordIteratorb.next();
        if (recorda[0] >= start && recorda[0] <= end) {
            string temp = recordb[0];
            ans.push_back(temp.substr(0,5));
            numOfRecord+=1;
        }
    }
    for (auto i: ans) {
        cout << i << endl;
    }

    cout << "NUMBER OF RECORDS: " << numOfRecord << endl;
    auto endTime = system_clock::now();
    auto duration = duration_cast<microseconds>(endTime - startTime);
    std::cout << "TIME: " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;



}