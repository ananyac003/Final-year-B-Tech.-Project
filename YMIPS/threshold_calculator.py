# Threshold(L,M,H) : (mean, standev)
# YMIPS(0.62321, 0.69295, 0.7627) : (0.553460447, 0.4362381282)
import math


mean = 0.553460447
standev = 0.4362381282

k = [1, 2, 3]
# threshold[]
threshold_L = mean + k[0] * standev * (1 - 1 / (1 + math.pow(standev, 2)))
threshold_M = mean + k[1] * standev * (1 - 1 / (1 + math.pow(standev, 2)))
threshold_H = mean + k[2] * standev * (1 - 1 / (1 + math.pow(standev, 2)))
print(round(threshold_L, 5))
print(round(threshold_M, 5))
print(round(threshold_H, 5))
