# Program to calculate node weight
# from a given text file of datasets
import csv

# To read from dataset file (.csv)
with open('dataset_YMIPS.csv', 'r') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='\t')

# New file for writing unique nodes
    with open('YMIPS_NodeSet.csv', 'w', newline='') as new_file:
        csv_writer = csv.writer(new_file, delimiter='\t')
        unique_list = []
        for line in csv_reader:
            unique_list.extend(line)
        unique_list = list(dict.fromkeys(unique_list))  # to take unique values of the list
        unique_list.sort()
        for items in unique_list:
            csv_writer.writerow([items])

# To read from new file
with open('YMIPS_NodeSet.csv', 'r') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='\t')

# Opening data set file
    with open('dataset_YMIPS.csv', 'r') as csv_file2:
        csv_reader2 = csv.reader(csv_file2, delimiter='\t')

# To write nodes, its surrounding nodes and its degree
        with open('YMIPS_NodeSet_2.csv', 'w', newline='') as new_file:
            csv_writer = csv.writer(new_file, delimiter='\t')
            for item in csv_reader:
                item = ''.join(item)
                chain = [item]
                csv_file2.seek(0)
                for line in csv_reader2:
                    if(item == line[0]):
                        chain.append(line[1])
                csv_writer.writerow(chain)


# to calculate level 1 and level 2 nodes
with open('YMIPS_NodeSet_2.csv', 'r') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='\t')

    with open('YMIPS_NodeSet_2.csv', 'r') as csv_file2:
        csv_reader2 = csv.reader(csv_file2, delimiter='\t')

        with open('YMIPS_NodeSet_3.csv', 'w', newline='') as new_file:
            csv_writer = csv.writer(new_file, delimiter='\t')

            for line in csv_reader:
                level_1 = line[1:]
                level_2 = []
                total = []
                for item in level_1:
                    csv_file2.seek(0)
                    for row in csv_reader2:
                        if(row[0] == item):
                            level_2 = level_2 + row[1:]

                total = level_1 + level_2
                total = list(dict.fromkeys(total))
                total.append(len(level_2))
                csv_writer.writerow(total)

# to calculate node-weight
# this file contains parent nodes and their child nodes (level 1)
# with the help of this file node degree is calculated
with open('YMIPS_NodeSet_2.csv', 'r') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='\t')

    # this file contains both level 1 and level 2 nodes
    with open('YMIPS_NodeSet_3.csv', 'r') as csv_file2:
        csv_reader2 = csv.reader(csv_file2, delimiter='\t')

        # to write the node weight of respective nodes
        with open('YMIPS_NodeSet_4.csv', 'w', newline='') as new_file:
            csv_writer = csv.writer(new_file, delimiter='\t')

            for line in csv_reader:
                value = [line[0]]       # for writing parent node
                for item in csv_reader2:
                    node_set = len(item) - 1    # no. of nodes in a subgraph
                    degree = int(item[-1])      # total number of level 2 node
                    value.append(degree)
                    value.append(node_set)
                    if (node_set != 0):
                        node_weight = degree / node_set     # by formula
                        value.append(node_weight)
                        node_weight_list = [value[0], round(value[-1], 5)]
                        csv_writer.writerow(node_weight_list)
                    else:
                        value.append('NULL')
                        csv_writer.writerow([value[0]])
                    break
