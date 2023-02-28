//
// Created by Yuchen Zhang on 2/7/23.
//
#include "library.h"


int main(int argc, char* argv[]){
    auto startTime = system_clock::now();
    std::ifstream page_file(argv[1]);
    int page_size = std::atoi(argv[2]);
    vector<V> record;
    Page* page = new Page;
    init_fixed_len_page(page, page_size, SLOT_SIZE);
    int number_of_records = 0;
    while(page_file.read((char*)page->data, page_size)){
        int len = fixed_len_page_capacity(page) - fixed_len_page_freeslots(page);
        for(int i = 0 ; i < len; ++i){
            read_fixed_len_page(page, i, &record);
            for(int j = 0 ; j < record.size(); ++j){
                if(j == record.size() - 1){
                    cout << record[j];
                }
                else{
                    cout << record[j] << ",";
                }
            }
            cout << endl;
            ++number_of_records;
        }
        record.clear();
        init_fixed_len_page(page, page_size, SLOT_SIZE);
    }
    auto endTime = system_clock::now();
    cout << "NUMBER OF RECORDS: " << number_of_records << endl;
    auto duration = duration_cast<microseconds>(endTime - startTime);
    std::cout << "TIME: " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;
}