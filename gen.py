import sys
import random
import csv

letters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

try:
    filename, n_tuples, tuple_size, attribute_size = sys.argv[1:5]
    n_tuples = int(n_tuples)
    tuple_size = int(tuple_size)
    attribute_size = int(attribute_size)
except:
    print('Usage: %s <outfile> <number of tuples> <size of a tuple> <size of attribute>' % sys.argv[0])
    sys.exit(0)

with open(filename, "w") as csvfile:
    writer = csv.writer(csvfile)
    for i in range(n_tuples):
        row = []
        for a in range(tuple_size):
            row.append(''.join([random.choice(letters) for j in range(attribute_size)]))
        writer.writerow(row)
