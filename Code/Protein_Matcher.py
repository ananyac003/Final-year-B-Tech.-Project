import csv

with open('average.csv', 'r') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='\t')

    with open('essen_pro.csv', 'r') as csv_file2:
        csv_reader2 = csv.reader(csv_file2, delimiter='\t')

        count = 0
        for line in csv_reader:
            line = ''.join(line)
            csv_file2.seek(0)
            for item in csv_reader2:
                item = ''.join(item)
                if(line == item):
                    print(line)
                    count = count + 1
        print(count)
