### Assignment 1.1

Yuchen Zhang 	yzh370@ur.rochester.edu

Ziqi Feng	zfeng16@ur.rochester.edu

Rui Guo	rguo7@ur.rochester.edu

#### Compile

```
cmake .
make
```

#### Run

```
./write_fixed_len_pages <csv_file> <page_file> <page_size>
./read_fixed_len_page <page_file> <page_size>
./csv2heapfile <csv_file> <heapfile> <page_size>
./scan <heapfile> <page_size>
./insert <heapfile> <csv_file> <page_size>
./update <heapfile> <record_id> <attribute_id> <new_value> <page_size>
./delete <heapfile> <record_id> <page_size>
./select <heapfile> <attribute_id> <start> <end> <page_size>
./csv2colstore <csv_file> <colstore_name> <page_size>
./select2  <colstore_name> <attribute_id> <start> <end> <page_size>
./select3  <colstore_name> <attribute_id> <return_attribute_id> <start> <end> <page_size>
```

**Note**

The program have some default numbers. If you want to test different data, please change the default number in `library.h`.

**slot_size**: the maximum capacity of each slot. 

```
SLOT_SIZE = 1000;
```

**attribute_size**: the size of each attribute. 

```
ATTRIBUTE_SIZE = 10;
```

**Important !!!**

If you trying to use colume store, you need to change `SLOT_SIZE = ATTRIBUTE_SIZE`, and`LINE_SIZE= SIZE OF A RECORD`



#### Experiment 2.1

1. Calculate the size of the fixed length serialization of records in the table.

   Assume there are 100 attributes in a record and the size of each attribute is 10 Bytes. The size of the fixed length serialization of the record is 1000 in this example. 

2. Use `fixed_len_sizeof()` to check if it agrees with your calculation.

   The output of `fixed_len_sizeof()` is 1000.

#### Experiment 3.2

1. Plot the performance (records / second) versus page size for write and read.

   The y coordinate is the number of records per second. (Records/s)

   The x coordinate is the size of page. (Byte)

![image-20230214142225283](./assets/image-20230214142225283.png)

2. Compare this to the blocked disk I/O characteristics you observed in the tutorial.

   In the tutorial, we can read different data from a different tables from different page files. And if the record is too long for a page, the record can be stored in two pages to use disk space better. The above code only achieves the data from the same table, and these data will be stored in a page file. If the record size is larger than the half size of the page. The rest space on the page will be wasted.

3. Discuss why a page-based format is superior to storing records using a CSV file.

   1. The page-based format is good for storing large data. The data in the page file is sequential, the program can write or read quickly in the page-based file. But in the CSV file, the record will be split with '\n' and each record locate at its own line. 
   2. Increase hard drive utilization.
   3. The page-based format is easy to manipulate and manage, so we can find a record from a large data set quickly. In the CSV file, if we want to find a record, we need to traverse all the records in the file.
   4. Reduce the number of I/O, we can retrieve a single page from a page file instead of reading from the whole file.

4. Discuss the shortcomings of the way we organize pages. 

   The shortcomings of the way we organize pages are that we only store the data and some page management information. 

   The page does not have an index so if we try to find the data from a range we still need to traverse all the data. Besides, we do not declare the rest of the space on each page. When some data is deleted from a page, the space for deleted records will be wasted. Or we still need to traverse all the pages to get the empty space but that will waste a lot of time. 

   If we store all the data on a page, the size of the page file will be huge. Some hard drives may not support such large individual files. We need to split the page file and store data into multiple data spaces. 
   
   Page-based organization can lead to contention, which may cause locking and blocking issues if multiple threads. need to access the same page at the same time.

