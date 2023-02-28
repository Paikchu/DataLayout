//
// Created by Yuchen Zhang on 2/6/23.
//

#ifndef ASSGINMENT1_LIBRARY_H
#define ASSGINMENT1_LIBRARY_H

#include <vector>
#include <bitset>
#include <iostream>
#include <cmath>
#include <fstream>
#include <chrono>
#include <sstream>
#include <cstring>
#include <thread>
#include <filesystem>
typedef const char* V;
typedef std::vector<V> Record;
using namespace std;
using namespace std::chrono;

const int SLOT_SIZE = 10;   // if column, slot_size == attribute size else slot_size == attribute_size * num_of_attributes

const int ATTRIBUTE_SIZE = 10;
const int NUM_OF_COLS = 100; // 100 attributes 100 column
const int LINE_SIZE = 1000; // column, the line size of csv file

/**
 * Compute the number of bytes required to serialize record
 */
int fixed_len_sizeof(Record *record);

/**
 * Serialize the record to a byte array to be stored in buf.
 */
void fixed_len_write(Record *record, void *buf);

/**
 * Deserializes `size` bytes from the buffer, `buf`, and
 * stores the record in `record`.
 */
void fixed_len_read(void *buf, int size, Record *record);


typedef struct {
    void *data;
    int page_size;
    int slot_size;
} Page;
/**
 * Initializes a page using the given slot size
 */
void init_fixed_len_page(Page *page, int page_size, int slot_size);

/**
 * Calculates the maximal number of records that fit in a page
 */
int fixed_len_page_capacity(Page *page);
/**
 * Calculate the free space (number of free slots) in the page
 */
int fixed_len_page_freeslots(Page *page);

/**
 * Add a record to the page
 * Returns:
 *   record slot offset if successful,
 *   -1 if unsuccessful (page full)
 */
int add_fixed_len_page(Page *page, Record *r);

/**
 * Write a record into a given slot.
 */
void write_fixed_len_page(Page *page, int slot, Record *r);

/**
 * Read a record from the page from a given slot.
 */
void read_fixed_len_page(Page *page, int slot, Record *r);

typedef struct {
    FILE *file_ptr; // point to the heapfile
    int page_size;
} Heapfile;

typedef int PageID;

typedef struct {
    int page_id;
    int slot;
} RecordID;

const int FREESPACE = 0;

/**
* Initalize a heapfile to use the file and page size given.
*/
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file);



/**
 * Allocate another page in the heapfile.  This grows the file by a page.
 */
PageID alloc_page(Heapfile *heapfile);


/**
 * Read a page into memory
 */
void read_page(Heapfile *heapfile, PageID pid, Page *page);

/**
 * Write a page from memory to disk
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid);

int first_freeslots(Page *page);

class RecordIterator {
private:
    Heapfile *heapfile;
    Page *curPage;
    int relativeSlot;   // The slot which are going to read
    int pid; // 当前page的pid
    int absoluteSlot;
public:
    RecordIterator(Heapfile *heapfile);
    Record next();
    bool hasNext();
};

void init_directory_page(Page *page, const int &page_size);

int maxPid(Heapfile* heapfile);

void delete_slot(Page *page, int slot);

#endif //ASSGINMENT1_LIBRARY_H