//
// Created by Yuchen Zhang on 2/20/23.
//

#include "library.h"
bool insert(Record *record, Heapfile* heapfile) {
    int insertPid = 1;
    Page *page = new Page;
    init_fixed_len_page(page, heapfile->page_size, SLOT_SIZE);
    int PID = maxPid(heapfile);
    while (insertPid <= PID) {
        read_page(heapfile, insertPid, page);
        // find free slot
        int freeslotPid = first_freeslots(page);
        if (freeslotPid != -1) {
            add_fixed_len_page(page, record);
            write_page(page, heapfile, insertPid);
            return true;
        }
        insertPid++;
        init_fixed_len_page(page, heapfile->page_size, SLOT_SIZE);
    }
    return false;
}

int main(int argc, char* argv[]){
    auto startTime = system_clock::now();
    FILE *heapFile = ::fopen(argv[1], "r+");
    FILE *csvFile = ::fopen(argv[2], "r");
    int page_size = atoi(argv[3]);

    // Initialize heapfile
    auto* heapfile = new Heapfile;
    init_heapfile(heapfile, page_size, heapFile);

    // Initialize page
    Page* page = new Page;
    Record record;
    init_fixed_len_page(page, page_size, SLOT_SIZE);

    // line size of csv file
    const int oneLineSize = SLOT_SIZE  + (SLOT_SIZE / ATTRIBUTE_SIZE);
    char data[oneLineSize];

    // Record the number of records
    int numOfRecord = 0;

    int state = 0;

    // Read csv file by line
    while(fread(data, 1, oneLineSize, csvFile)){
        numOfRecord++;

        // Remove unless characters in one line
        string line(data);
        line = line.substr(0, SLOT_SIZE + SLOT_SIZE / ATTRIBUTE_SIZE - 1);
//        // For Linux
//        if(line.length() == SLOT_SIZE + SLOT_SIZE / ATTRIBUTE_SIZE - 1 + 2)
//            line = line.substr(0, line.size() - 2);
//            // For mac
//        else if(line.length() == SLOT_SIZE + SLOT_SIZE / ATTRIBUTE_SIZE - 1 + 3)
//            line = line.substr(0, line.size() - 3);
        stringstream ss(line);
        string attr;
        while(getline(ss, attr, ',')){
            char* temp = new char[ATTRIBUTE_SIZE+1];
            strcpy(temp, attr.c_str());
            record.push_back(temp);
        }

        if(state == 0){
            bool full = insert(&record, heapfile);
            if(!full)
                state = 1;
        }
        if(state == 1) {
            // When the page is not full, Add record into page
            // When the page is full, alloc a page in heapfile and then clear page for the next writing
            while (add_fixed_len_page(page, &record) == -1) {
                PageID pid = alloc_page(heapfile);
//                cout << pid << " ";
                write_page(page, heapfile, pid);
                init_fixed_len_page(page, atoi(argv[3]), fixed_len_sizeof(&record));
            }
        }
        record.clear();
    }
    // Write the rest of records into the heapfile
    write_page(page, heapfile, alloc_page(heapfile));

    ::fclose(heapFile);

    auto endTime = system_clock::now();
    cout << "NUMBER OF RECORDS: " << numOfRecord << endl;
    auto duration = duration_cast<microseconds>(endTime - startTime);
    std::cout << "TIME: " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;
}