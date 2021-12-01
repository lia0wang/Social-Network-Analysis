# Makefile for Assignment 2

CC = gcc
CFLAGS = -Wall -Werror -g

all: testFloydWarshall testCentralityMeasures testGirvanNewman

testFloydWarshall: testing/testFloydWarshall.c analysisAlgorithms/FloydWarshall.c graphADT/Graph.c graphADT/GraphRead.c
	$(CC) $(CFLAGS) -o testing/testFloydWarshall testing/testFloydWarshall.c analysisAlgorithms/FloydWarshall.c graphADT/Graph.c graphADT/GraphRead.c

testCentralityMeasures: testing/testCentralityMeasures.c analysisAlgorithms/CentralityMeasures.c analysisAlgorithms/FloydWarshall.c graphADT/Graph.c graphADT/GraphRead.c
	$(CC) $(CFLAGS) -o testing/testCentralityMeasures testing/testCentralityMeasures.c analysisAlgorithms/CentralityMeasures.c analysisAlgorithms/FloydWarshall.c graphADT/Graph.c graphADT/GraphRead.c

testGirvanNewman: testing/testGirvanNewman.c analysisAlgorithms/GirvanNewman.c analysisAlgorithms/CentralityMeasures.c analysisAlgorithms/FloydWarshall.c graphADT/Graph.c graphADT/GraphRead.c bsTreeADT//BSTree.c
	$(CC) $(CFLAGS) -o testing/testGirvanNewman testing/testGirvanNewman.c analysisAlgorithms/GirvanNewman.c analysisAlgorithms/CentralityMeasures.c analysisAlgorithms/FloydWarshall.c graphADT/Graph.c graphADT/GraphRead.c bsTreeADT/BSTree.c

clean:
	rm -rf testing/testFloydWarshall testing/testCentralityMeasures testing/testGirvanNewman

