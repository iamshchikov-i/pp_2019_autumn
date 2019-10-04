// Copyright 2019 Iamshchikov Ivan
#ifndef MODULES_TEST_TASKS_TEST_MPI_COLUMNS_MIN_H_
#define MODULES_TEST_TASKS_TEST_MPI_COLUMNS_MIN_H_

#include <mpi.h>
#include <vector>

void getRandomMatrix(std::vector<std::vector<int> >& matrix, int n);
void matrixToRow(std::vector<std::vector<int> >& matrix,
                 std::vector<int>& row, int row_num, int col_num);
int getMinElem(int* arr, int n);
int getInvolvedProcNumber(int n, int procnum);
MPI_Comm chooseComm(int n, int procnum, MPI_Comm& c);
void setSendcount(std::vector<int>& elem_count, std::vector<int>& col_count,
                  int k, int n, int m);
void setDispl(std::vector<int>& elem_displ, std::vector<int>& elem_count,
              std::vector<int>& col_displ, std::vector<int>& col_count);
std::vector<int> getMinElementsSequential
                 (std::vector<std::vector<int> >& matrix,
                 int row_num, int col_num);
std::vector<int> getMinElementsParallel
                 (std::vector<std::vector<int> >& matrix,
                 int row_num, int col_num);

#endif // MODULES_TEST_TASKS_TEST_MPI_COLUMNS_MIN_H_