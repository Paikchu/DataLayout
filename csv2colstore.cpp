//
// Created by FZQ on 2/23/2023.
//




#include "library.h"

int main(int argc, char* argv[]) {
    auto startTime = system_clock::now();
    FILE* csvFile = fopen(argv[1], "r");
    string colstore_name = argv[2];
    int page_size = atoi(argv[3]);

    // matrix to store csvfile
    vector<vector<string>> records;

    // create dir
    filesystem::create_directory(colstore_name);


    // Initialize heapfiles for each column
    std::vector<Heapfile*> heapfiles;
    for (int i = 0; i < NUM_OF_COLS; i++) {
        string filename = colstore_name + "/" + to_string(i);
        FILE* heapFile = fopen(filename.c_str(), "w+");
        Heapfile* heapfile = new Heapfile;
        init_heapfile(heapfile, page_size, heapFile);
        heapfiles.push_back(heapfile);
    }

    // Initialize pages for each column
    vector<Page*> pages;
    for (int i = 0; i < NUM_OF_COLS; i++) {
        Page* page = new Page;
        init_fixed_len_page(page, page_size, ATTRIBUTE_SIZE);
        pages.push_back(page);
    }

    // line size of csv file
    const int oneLineSize = LINE_SIZE  + (LINE_SIZE / ATTRIBUTE_SIZE);
    char data[oneLineSize];

    // Record the number of records
    int numOfRecord = 0;

    // Read csv file by line
    while(fread(data, 1, oneLineSize, csvFile)) {
        numOfRecord++;

        // Remove useless characters in one line
        string line(data);
        line = line.substr(0, LINE_SIZE + LINE_SIZE/ATTRIBUTE_SIZE -1);
        stringstream ss(line);
        vector<string> attrs;
        string attr;
        while(getline(ss, attr, ',')) {
            attrs.push_back(attr);
        }
        records.push_back(attrs);

    }

   cout << numOfRecord << endl;

    // // iterate through records by row
    // for (int i = 0; i < numOfRecord; i++) {
    //     for (int j = 0; j< NUM_OF_COLS; j++) {
    //         Record record;
    //         record.push_back(records[i][j].c_str());
    //         // cout << record[0] << endl;
    //         while (add_fixed_len_page(pages[j], &record) == -1) {
    //             PageID pid = alloc_page(heapfiles[j]);
    //             write_page(pages[j], heapfiles[j], pid);
    //             init_fixed_len_page(pages[j], atoi(argv[3]), fixed_len_sizeof(&record));
    //         }
    //         record.clear();

    //     }
    // }


    // int part_size = NUM_OF_COLS/8;
    // int start_col[8] = {0, part_size, part_size*2, part_size*3, part_size*4, part_size*5, part_size*6, part_size*7};
    // int end_col[8] = {part_size, part_size*2, part_size*3, part_size*4, part_size*5, part_size*6, part_size*7, NUM_OF_COLS};
    


    // by column
    // for (int j = 0; j< NUM_OF_COLS; j++) { 
    //     for (int i = 0; i < numOfRecord; i++) {
    //         Record record;
    //         record.push_back(records[i][j].c_str());
    //         // cout << record[0] << endl;
    //         while (add_fixed_len_page(pages[j], &record) == -1) {
    //             PageID pid = alloc_page(heapfiles[j]);
    //             write_page(pages[j], heapfiles[j], pid);
    //             init_fixed_len_page(pages[j], atoi(argv[3]), fixed_len_sizeof(&record));
    //         }
    //         record.clear();

    //     }
    // }

    // for (int j = 0; j< NUM_OF_COLS; j++) {
    //     PageID pid = alloc_page(heapfiles[j]);
    //     write_page(pages[j], heapfiles[j], pid);
    // }


    // std::vector<std::thread> threads;
    // for (int t = 0; t < 8; t++) {
    //     threads.emplace_back([&pages, &heapfiles, &records, &start_col, &end_col, &numOfRecord, &page_size](int thread_id) {
    //         for (int j = start_col[thread_id]; j < end_col[thread_id]; j++) {
    //             for (int i = 0; i < numOfRecord; i++) {
    //                 Record record;
    //                 record.push_back(records[i][j].c_str());
    //                 while (add_fixed_len_page(pages[j], &record) == -1) {
    //                     PageID pid = alloc_page(heapfiles[j]);
    //                     write_page(pages[j], heapfiles[j], pid);
    //                     init_fixed_len_page(pages[j], page_size, fixed_len_sizeof(&record));
    //                 }
    //                 record.clear();
    //             }
    //             PageID pid = alloc_page(heapfiles[j]);
    //             write_page(pages[j], heapfiles[j], pid);
    //         }
    //     }, t);
    // }


    // Using OpenMP
    int num_threads = omp_get_max_threads();

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();

        for (int j = thread_id; j < NUM_OF_COLS; j += num_threads) {
            for (int i = 0; i < numOfRecord; i++) {
                Record record;
                record.push_back(records[i][j].c_str());
                while (add_fixed_len_page(pages[j], &record) == -1) {
                    PageID pid = alloc_page(heapfiles[j]);
                    write_page(pages[j], heapfiles[j], pid);
                    init_fixed_len_page(pages[j], page_size, fixed_len_sizeof(&record));
                }
                record.clear();
            }
            PageID pid = alloc_page(heapfiles[j]);
            write_page(pages[j], heapfiles[j], pid);
        }
    }

    // for (auto& thread : threads) {
    //     thread.join();
    // }


    // close all heapfiles
    for (int i = 0; i< NUM_OF_COLS; i++) {
        fclose(heapfiles[i]->file_ptr);
    }


    auto endTime = system_clock::now();
    cout << "NUMBER OF RECORDS: " << numOfRecord << endl;
    auto duration = duration_cast<microseconds>(endTime - startTime);
    std::cout << "TIME: " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;

}
