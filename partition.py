import os
import subprocess as sb
import random
import math

import numpy as np
import collections

import time


# function that calculates integer partitions
def partition(number):
	answer = set()
	answer.add((number, ))
	for x in range(1, number):
		for y in partition(number - x):
			answer.add(tuple(sorted((x, ) + y, reverse = True)))
	return sorted(list(answer), key=lambda x: (-x[0], len(x)) )

def partition_repr(n):
	listik = []
	for x in partition(n):
		tmp = str(x)[1:-1]
		tmp = tmp.replace(' ', '')
		if tmp[-1] == ',':
			tmp = tmp[:-1]
		listik.append('{' + tmp + '}')
	return listik


if __name__ == '__main__':
	for z in range(2, 6+1):
		print("number of int partitions of %d is %d" % (z, len(partition(z))))

	for i, p in enumerate(partition(4)):
			print(i, p)

	for i, p in enumerate(partition(5)):
		print(i, p)

	for i, p in enumerate(partition(6)):
		print(i, p)
