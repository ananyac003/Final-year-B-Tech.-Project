# segregattion of essential and non essential proteins
# into separate files
import csv

with open('average.csv', 'r') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='\t')

    with open('calc_EP.csv', 'w', newline='') as new_file:
        csv_writer = csv.writer(new_file, delimiter='\t')

        with open('calc_NEP.csv', 'w', newline='') as new_file2:
            csv_writer2 = csv.writer(new_file2, delimiter='\t')

            count = 0
            for line in csv_reader:
                count = count + 1

            cutoff = round(count * 0.2)

            csv_file.seek(0)
            count = 1
            for line in csv_reader:
                if(count <= cutoff):
                    csv_writer.writerow(line)
                    E_Pro = count
                    count = count + 1
                else:
                    csv_writer2.writerow(line)
                    NE_Pro = count - E_Pro
                    count = count + 1

# evaluation
with open('essen_pro.csv', 'r') as csv_file1:
    csv_reader1 = csv.reader(csv_file1, delimiter='\t')

    with open('non-essen_pro.csv', 'r') as csv_file2:
        csv_reader2 = csv.reader(csv_file2, delimiter='\t')

        with open('calc_EP.csv', 'r') as csv_file3:
            csv_reader3 = csv.reader(csv_file3, delimiter='\t')

            with open('calc_NEP.csv', 'r') as csv_file4:
                csv_reader4 = csv.reader(csv_file4, delimiter='\t')

                # True Positive calculation
                count = 0
                csv_file3.seek(0)
                for line in csv_reader3:
                    line = ''.join(line)
                    csv_file1.seek(0)
                    for item in csv_reader1:
                        item = ''.join(item)
                        if(line == item):
                            count = count + 1
                True_Pos = count
                print('{} : {}'.format('True_Pos', count))

                # False Positive calculation
                count = 0
                csv_file3.seek(0)
                for line in csv_reader3:
                    line = ''.join(line)
                    csv_file2.seek(0)
                    for item in csv_reader2:
                        item = ''.join(item)
                        if(line == item):
                            count = count + 1
                False_Pos = count
                print('{} : {}'.format('False_Pos', count))

                # True negative calculation
                count = 0
                csv_file4.seek(0)
                for line in csv_reader4:
                    line = ''.join(line)
                    csv_file2.seek(0)
                    for item in csv_reader2:
                        item = ''.join(item)
                        if(line == item):
                            count = count + 1
                True_Neg = count
                print('{} : {}'.format('True_Neg', count))

                # False negative calculation
                count = 0
                csv_file4.seek(0)
                for line in csv_reader4:
                    line = ''.join(line)
                    csv_file1.seek(0)
                    for item in csv_reader1:
                        item = ''.join(item)
                        if(line == item):
                            count = count + 1
                False_Neg = count
                print('{} : {}'.format('False_Neg', count))

# 6 statistical measures
print('{} : {}'.format('Total_essential_Proteins', E_Pro))  # = 246  # 1285
print('{} : {}'.format('Total_non-essential_Proteins', NE_Pro))  # = 984  # 4394
Sensitivity = round(True_Pos / (True_Pos + False_Neg), 3)
Specificity = round(True_Neg / (True_Neg + False_Pos), 3)
Positive_predictive_value = round(True_Pos / (True_Pos + False_Pos), 3)
Negative_predictive_value = round(True_Neg / (True_Neg + False_Neg), 3)
F_measure = round((2 * Sensitivity * Positive_predictive_value) / (Sensitivity + Positive_predictive_value), 3)
Accuracy = round((True_Pos + True_Neg) / (E_Pro + NE_Pro), 3)
print('{} : {}'.format('Sensitivity', Sensitivity))
print('{} : {}'.format('Specificity', Specificity))
print('{} : {}'.format('Positive_predictive_value', Positive_predictive_value))
print('{} : {}'.format('Negative_predictive_value', Negative_predictive_value))
print('{} : {}'.format('F_measure', F_measure))
print('{} : {}'.format('Accuracy', Accuracy))
