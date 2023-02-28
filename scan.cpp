//
// Created by Yuchen Zhang on 2/20/23.
//
#include "library.h"

int main(int argc, char* argv[]){
    auto startTime = system_clock::now();
    FILE *file = ::fopen(argv[1], "r");
    int page_size = std::atoi(argv[2]);

    Record record;

    Heapfile* heapfile = new Heapfile;
    init_heapfile(heapfile, page_size, file);

    Page* page = new Page;
    init_fixed_len_page(page, page_size, SLOT_SIZE);

    RecordIterator recordIterator(heapfile);

    int number_of_records = 0;
    while(recordIterator.hasNext()) {
        record = recordIterator.next();
        for (int j = 0; j < record.size(); ++j) {
            if (j == record.size() - 1) {
                cout << record[j];
            } else {
                cout << record[j] << ",";
            }
        }
        cout << endl;
        ++number_of_records;

        record.clear();
    }
    auto endTime = system_clock::now();
    cout << "NUMBER OF RECORDS: " << number_of_records << endl;
    auto duration = duration_cast<microseconds>(endTime - startTime);
    std::cout << "TIME: " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;
}