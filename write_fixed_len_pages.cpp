#include <bitset>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "library.h"

int main(int argc, char* argv[]){
    Record record;
    auto startTime = system_clock::now();
    std::ifstream fp(argv[1]);
    std::ofstream page_file(argv[2]);
    if(!fp.is_open() || !page_file.is_open()){
        cout << "failure";
    }
    std::string line;
    Page* page = new Page;
    int number_of_records = 0;
    int number_of_pages = 1;

    while (getline(fp,line)){
        line = line.substr(0, line.size() - 1); // remove '\n'
        stringstream ss(line);
        string attr;
        while(getline(ss, attr, ',')){
            char* temp = new char[ATTRIBUTE_SIZE+1];
            strcpy(temp, attr.c_str());
            record.push_back(temp);
        }
        if(number_of_records == 0){
            init_fixed_len_page(page, atoi(argv[3]), fixed_len_sizeof(&record));
        }
        while (add_fixed_len_page(page, &record) == -1){
            page_file.write((char*)page->data, page->page_size);
            init_fixed_len_page(page, atoi(argv[3]), fixed_len_sizeof(&record));
            ++number_of_pages;
        }
        record.clear();
        ++number_of_records;
    }
    page_file.write((char*)page->data, page->page_size) ;
    auto endTime = system_clock::now();
    cout << "NUMBER OF RECORDS: " << number_of_records << endl;
    cout << "NUMBER OF PAGES: " << number_of_pages << endl;
    auto duration = duration_cast<microseconds>(endTime - startTime);
    std::cout << "TIME: " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;
}