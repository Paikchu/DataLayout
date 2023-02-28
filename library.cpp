#include <sstream>
#include "library.h"

/**
 * Compute the number of bytes required to serialize record
 */
int fixed_len_sizeof(Record *record) {
    unsigned int len = 0;
    for (auto i: *record) {
        len += strlen(i);
    }
    return len;
}

/**
 * Serialize the record to a byte array to be stored in buf.
 */
void fixed_len_write(Record *record, void *buf) {
    char *buffer = new char[fixed_len_sizeof(record)];
    char *buff = buffer;
    for (auto &i: *record) {
        const char *temp = i;
        for (int j = 0; j < ::strlen(i); ++j) {
            *buffer = *temp;
            buffer++;
            temp++;
        }
    }
    memcpy(buf, buff, fixed_len_sizeof(record));
}

/**
 * Assume each attribute with 10 bytes
 * @param buf
 * @param size
 * @param record
 */
void fixed_len_read(void *buf, int size, Record *record) {
    char *data = (char *) buf;
    record->clear();
    for (int i = 0; i < size / ATTRIBUTE_SIZE; ++i) {
        char *temp = new char[ATTRIBUTE_SIZE];
        memcpy(temp, data + (i * ATTRIBUTE_SIZE), ATTRIBUTE_SIZE);
        record->push_back(temp);
    }
}

/**
 * Calculates the maximal number of records that fit in a page
 */
int fixed_len_page_capacity(Page *page) {
    return (page->page_size - 5) / (page->slot_size + 1);
}


/**
 * Initializes a page using the given slot size
 */
void init_fixed_len_page(Page *page, int page_size, int slot_size) {
    // new a page
    page->data = new char[page_size];
    page->page_size = page_size;
    page->slot_size = slot_size;
    // initialize M
    int *M = (int *) ((char *) (page->data) + page_size - 4);
    *M = fixed_len_page_capacity(page);
}

/**
 * Calculate the free space (number of free slots) in the page
 * @param page
 * @return the number of free slots
 */
int fixed_len_page_freeslots(Page *page) {
    int *M = (int *) ((char *) (page->data) + page->page_size - 4);
    char *temp = (char *) page->data + page->page_size - 4;
    int ans = 0;
    for (int i = 0; i < *M;) {
        temp = temp - 1;
        bitset<8> a(*temp);
        for (int j = 7; j >= 0 && i < *M; --j) {
            if (a[j] == 0) {
                ans++;
            }
            i++;
        }
    }
    return ans;
}

/**
 * Check the valid slot in a page
 * @param page
 * @return number of slot
 */
vector<int> fixed_len_page_usedSlots(Page *page) {
    int *M = (int *) ((char *) (page->data) + page->page_size - 4);
    char *temp = (char *) page->data + page->page_size - 4;
    vector<int> ans;
    for (int i = 0; i < *M;) {
        temp = temp - 1;
        bitset<8> a(*temp);
        for (int j = 7; j >= 0 && i < *M; --j) {
            if (a[j]) {
                ans.push_back(i);
            }
            i++;
        }
    }
    return ans;
}

// return the all freeslots in the page
vector<int> fixed_len_page_freeSlots(Page *page) {
    int *M = (int *) ((char *) (page->data) + page->page_size - 4);
    char *temp = (char *) page->data + page->page_size - 4;
    vector<int> ans;
    for (int i = 0; i < *M;) {
        temp = temp - 1;
        bitset<8> a(*temp);
        for (int j = 7; j >= 0 && i < *M; --j) {
            if (a[j] == 0) {
                ans.push_back(i);
            }
            i++;
        }
    }
    return ans;
}



/**
 * Return the offset of the first free slot
 * @param page
 * @return if exist return offset else return -1, else return the offset of the first free slot
 */
int first_freeslots(Page *page) {
    int *M = (int *) ((char *) (page->data) + page->page_size - 4);
    char *temp = (char *) page->data + page->page_size - 4;
    for (int i = 0; i < *M;) {
        temp = temp - 1;
        bitset<8> a(*temp);
        for (int j = 7; j >= 0 && i < *M; --j) {
            if (a[j] == 0) {
                return i;
            }
            i++;
        }
    }
    return -1;
}

/**
 * Mark the first free slot was used
 * @param page
 */
void used_first_freeslots(Page *page) {
    int *M = (int *) ((char *) (page->data) + page->page_size - 4);
    char *temp = (char *) page->data + page->page_size - 4;
    for (int i = 0; i < *M;) {
        temp = temp - 1;
        bitset<8> a(*temp);
        for (int j = 7; j >= 0 && i < *M; --j) {
            if (a[j] == 0) {
                a[j] = true;
                unsigned long n = a.to_ulong();
                *temp = static_cast<char>(n);
                return;
            }
            i++;
        }

    }
}

/**
 * Add a record to the page
 * Returns:
 *   record slot offset if successful,
 *   -1 if unsuccessful (page full)
 */
int add_fixed_len_page(Page *page, Record *r) {
    // check free slot exist
    int freeslot = first_freeslots(page);
    if (freeslot == -1) {
        return -1;
    }
    // write into first free slot
    char *slot = (char *) page->data + (freeslot * page->slot_size);
    fixed_len_write(r, slot);

    // record slot was used in M
    used_first_freeslots(page);

    // return offset
    return freeslot;
}

/**
 * Mark slot used
 * @param page
 * @param slot from [0..n]
 */
void used_slot(Page *page, int slot) {
    int *M = (int *) ((char *) (page->data) + page->page_size - 4);
    char *temp = (char *) page->data + page->page_size - 4;
    for (int i = 0; i < *M;) {
        temp = temp - 1;
        bitset<8> a(*temp);
        for (int j = 7; j >= 0 && i < *M; --j) {
            if (i == slot) {
                a[j] = true;
                unsigned long n = a.to_ulong();
                *temp = static_cast<char>(n);
                return;
            }
            i++;
        }
    }
}

void delete_slot(Page *page, int slot) {
    int *M = (int *) ((char *) (page->data) + page->page_size - 4);
    char *temp = (char *) page->data + page->page_size - 4;
    for (int i = 0; i < *M;) {
        temp = temp - 1;
        bitset<8> a(*temp);
        for (int j = 7; j >= 0 && i < *M; --j) {
            if (i == slot) {
                a[j] = false;
                unsigned long n = a.to_ulong();
                *temp = static_cast<char>(n);
                return;
            }
            i++;
        }
    }
}
/**
 * Write a record into a given slot.
 */
void write_fixed_len_page(Page *page, int slot, Record *r) {
    char *d = (char *) page->data + (slot * page->slot_size);
    fixed_len_write(r, d);
    used_slot(page, slot);
}

/**
 * Read a record from the page from a given slot.
 */
void read_fixed_len_page(Page *page, int slot, Record *r) {
    char *d = (char *) page->data + (slot * page->slot_size);
    fixed_len_read(d, page->slot_size, r);
}


void init_directory_page(Page *page, const int &page_size) {
    page->data = new char[page_size];
    page->page_size = page_size;
    int cap = page_size / 4;
    int *directoryData = new int[cap];
    int *dummy = directoryData;
    for (int i = 0; i < cap; i++) {
        *directoryData++ = -1;
    }
    memcpy(page->data, dummy, cap * 4);
}

/**
 * Initialize a heapfile to use the file and page size given.
 * @param heapfile
 * @param page_size page size in heapfile
 * @param file heapfile pointer
 */
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file) {
    heapfile->file_ptr = file;
    heapfile->page_size = page_size;
}


void read_page(Heapfile *heapfile, PageID pid, Page *page) {
    // Find the right place by pid
    int pageNumPerDic = (heapfile->page_size - 4) / 8;  // 每个directory能存几个page
    int sizeOfOneHeapFile = heapfile->page_size * (pageNumPerDic + 1);  // 每个heapfile的大小
    int directoryNum = (pid - 1) / pageNumPerDic; // 当前pid所在的page前有几个heapfile
    int offset =
            directoryNum * sizeOfOneHeapFile + heapfile->page_size + (pid - 1) % pageNumPerDic * heapfile->page_size;
    // (pid - (pageNumPerDic * directoryNum)) * heapfile->page_size + (sizeOfOneHeapFile * directoryNum);

    fseek(heapfile->file_ptr, offset, SEEK_SET);
    fread(page->data, 1, heapfile->page_size, heapfile->file_ptr);
    rewind(heapfile->file_ptr);

}

/**
 * Set FreeSpace entry
 * @param pid
 * @param heapfile
 * @param page
 */
void setFreeSpace(const PageID &pid, Heapfile *heapfile, Page *page) {
    int freeSlot = fixed_len_page_freeslots(page);
    Page *directoryPage = new Page;
    init_directory_page(directoryPage, heapfile->page_size);

    int pageNumPerDic = (heapfile->page_size - 4) / 8;  // 每个directory能存几个page
    int directoryNum = (pid - 1) / pageNumPerDic; // 当前pid属于哪个heapfile
    int indexNumInDirectory = pid - (directoryNum) * pageNumPerDic;     // 在对应directory page中的offset
    int moveToDic = (directoryNum) * (pageNumPerDic + 1) * heapfile->page_size;

    fseek(heapfile->file_ptr, moveToDic, SEEK_SET); // 移动到对应的heapfile
    fread(directoryPage->data, 1, heapfile->page_size, heapfile->file_ptr); // 读取
    fseek(heapfile->file_ptr, 0 - heapfile->page_size, SEEK_CUR);   //后移heapfile->page_size，便于写入
    int *directory = (int *) directoryPage->data;
    directory = directory + 2 + (indexNumInDirectory - 1) * 2;
    *directory = freeSlot;
    fwrite(directoryPage->data, 1, heapfile->page_size, heapfile->file_ptr); // 写入heapfile
    //rewind(heapfile->file_ptr); //重置指针
}

/**
 * Write a page from memory to disk
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid) {

    char *data = new char[page->page_size];
    memcpy(data, page->data, page->page_size);

    // Set freeSpace in Directory Page
    setFreeSpace(pid, heapfile, page);

    // Find the right place by pid
    int pageNumPerDic = (heapfile->page_size - 4) / 8;  // 每个directory能存几个page
    int sizeOfOneHeapFile = heapfile->page_size * (pageNumPerDic + 1);  // 每个heapfile的大小
    int directoryNum = (pid - 1) / pageNumPerDic; // 当前pid所在的page前有几个heapfile
    int offset = (pid - (pageNumPerDic * directoryNum)) * heapfile->page_size + (sizeOfOneHeapFile * directoryNum);

    fseek(heapfile->file_ptr, offset, SEEK_SET);
    fwrite(data, 1, heapfile->page_size, heapfile->file_ptr);
    rewind(heapfile->file_ptr);
}


/**
 * When a page is full, the page will be written into the heapfile.
 * Allocate a pageID for the page which is going to write into the heapfile
 */
PageID alloc_page(Heapfile *heapfile) {
    // Initialize parameters
    PageID id = 1;  // 初始化PageID
    bool state = false; // 是否当前heapfile还有能分配的空间
    int nextDirectoryIndex = 1; //The next Directory Offset if a new directory needs to be allocated

    // Initialize Directory Page
    Page *page = new Page;
    init_directory_page(page, heapfile->page_size);

    // Traverse all the free space in all the directory pages
    while (!state) {
        fread(page->data, 1, heapfile->page_size, heapfile->file_ptr);
        int *nextDirectory = (int *) page->data;
        int *directory = (int *) page->data + 1;

        // Traverse all the space in a directory page
        // If free space was found, update the page id
        for (int i = 0; i < (heapfile->page_size - 4) / 8; i++) {
            if (*directory == -1) {
                *directory = id;
                state = true;
                ::fseek(heapfile->file_ptr, 0 - (heapfile->page_size), SEEK_CUR);
                fwrite(page->data, 1, heapfile->page_size, heapfile->file_ptr);
                break;
            }
            directory += 2;
            id += 1;
        }

        // When all the directory pages were traversed, there is no more space for a new page.
        // Update the next directory page pointer of the last directory page.
        if (!state) {
            if (*nextDirectory == -1) {
                *nextDirectory = nextDirectoryIndex;
                fseek(heapfile->file_ptr, 0 - (heapfile->page_size), SEEK_CUR);
                fwrite(page->data, 1, heapfile->page_size, heapfile->file_ptr);
                Page *newPage = new Page;
                init_directory_page(newPage, heapfile->page_size);
                ::fseek(heapfile->file_ptr, ((heapfile->page_size - 4) / 8) * heapfile->page_size, SEEK_CUR);
                int *newDirectory = (int *) newPage->data + 1;
                *newDirectory = id;
                fwrite(newPage->data, 1, heapfile->page_size, heapfile->file_ptr);
                break;
            } else {
                ::fseek(heapfile->file_ptr, ((heapfile->page_size - 4) / 8) * heapfile->page_size, SEEK_CUR);
                nextDirectoryIndex++;
            }
        }

    }
    rewind(heapfile->file_ptr);
    return id;
}


RecordIterator::RecordIterator(Heapfile *heapfile) {
    // Initialize fields
    relativeSlot = -1;
    absoluteSlot = 0;
    pid = 1;
    // Initialize heapfile
    this->heapfile = heapfile;
    rewind(this->heapfile->file_ptr);
    // Initialize the page with the first page
    this->curPage = new Page;
    init_fixed_len_page(this->curPage, this->heapfile->page_size, SLOT_SIZE);
    read_page(this->heapfile, pid, this->curPage);
    rewind(this->heapfile->file_ptr);

}

int maxPid(Heapfile* heapfile) {
    int pageNumPerDic = (heapfile->page_size - 4) / 8;  // 每个directory能存几个page
    fseek(heapfile->file_ptr, 0, SEEK_END);
    int offset = ftell(heapfile->file_ptr);
    int sizeOfPerHeap = heapfile->page_size * (pageNumPerDic + 1);
    int maxPid = (offset / sizeOfPerHeap) * pageNumPerDic + (offset % sizeOfPerHeap) / heapfile->page_size -
                 1; // The total number of pid
    rewind(heapfile->file_ptr);
    return maxPid;
}

bool RecordIterator::hasNext() {
    while (pid <= maxPid(this->heapfile)) {
        vector<int> usedSlots = fixed_len_page_usedSlots(this->curPage);
        if (!usedSlots.empty() && (relativeSlot + 1) < usedSlots.size()) {
            relativeSlot += 1;
            absoluteSlot = usedSlots[relativeSlot];
            return true;
        } else {
            relativeSlot = -1;
            absoluteSlot = 0;
            pid += 1;
            init_fixed_len_page(this->curPage, this->heapfile->page_size, SLOT_SIZE);
            read_page(this->heapfile, pid, this->curPage);
        }
    }
    return false;

}

Record RecordIterator::next() {
    Record ans;
    read_fixed_len_page(this->curPage, absoluteSlot, &ans);
    return ans;
}

