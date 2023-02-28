import sys
import random

letters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
try:
    filename, n_tuples, attr_numbers = sys.argv[1:4]
    n = int(n_tuples)
    m = int(attr_numbers)
except:
    print('Usage: %s <outfile> <number of tuples> <number of attributes>' % sys.argv[0])
    sys.exit(0)

with open(filename, 'w') as f:
    for _ in range(n):
        for a in range(m):
            row = []
            row.append(''.join([random.choice(letters) for j in range(10)]))
            f.write(''.join(row))
            if(a>=m-1): break
            f.write(',')
        if(_>=n-1): break
        f.write('\n')

print("Generated %d random tuples in %s." % (n, filename))