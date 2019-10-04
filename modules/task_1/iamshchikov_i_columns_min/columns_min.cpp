// Copyright 2019 Iamshchikov Ivan
#include <random>
#include <ctime>
#include "../../../modules/task_1/iamshchikov_i_columns_min/columns_min.h"

void getRandomMatrix(std::vector<std::vector<int> >& matrix, int n) {
    if (n > 0 && matrix.size() > 0) {
        std::mt19937 gen;
        gen.seed(static_cast<unsigned int>(time(0)));
        for (int i = 0;i < matrix.size();++i)
            for (int j = 0;j < n;++j)
                matrix[i].push_back(gen() % 100);
    }
    else throw - 1;
}

void matrixToRow(std::vector<std::vector<int> >& matrix,
                 std::vector<int>& row, int row_num, int col_num) {
    int k = 0;
    for (int i = 0;i < col_num;++i)
        for (int j = 0;j < row_num;++j)
            row[k++] = matrix[j][i];
}

int getMinElem(int* arr, int n) {
    if (n > 0) {
        int min = arr[0];
        for (int i = 1;i < n;++i)
            if (arr[i] < min)
                min = arr[i];
        return min;
    }
    else throw -1;
}

int getInvolvedProcNumber(int n, int procnum) {
    if (n / procnum == 0) return n;
    else if (n / procnum > 0) return procnum;
    else throw -1;
}

MPI_Comm chooseComm(int n, int procnum, MPI_Comm& c) {
    if (n < procnum) return c;
    else return MPI_COMM_WORLD;
}

void setSendcount(std::vector<int>& elem_count, std::vector<int>& col_count,
                  int k, int n, int m) {
    int count = n / k;
    if (n%k != 0) {
        for (int i = 0;i < n%k;++i) {
            elem_count.push_back((count + 1)*m);
            col_count.push_back(count + 1);
        }
        for (int i = n % k;i < k;++i) {
            elem_count.push_back(count*m);
            col_count.push_back(count);
        }	
    }
    else
        for (int i = 0;i < k;++i) {
            elem_count.push_back(count*m);
            col_count.push_back(count);
        }
}

void setDispl(std::vector<int>& elem_displ, std::vector<int>& elem_count,
              std::vector<int>& col_displ, std::vector<int>& col_count) {
    int tmp_e = 0;
    int tmp_c = 0;
    elem_displ.push_back(0);
    col_displ.push_back(0);
    for (int i = 1;i < elem_count.size();++i) {
        tmp_e += elem_count[i - 1];
        tmp_c += col_count[i - 1];
        elem_displ.push_back(tmp_e);
        col_displ.push_back(tmp_c);
    }
}

std::vector<int> getMinElementsSequential
                 (std::vector<std::vector<int> >& matrix,
                  int row_num, int col_num) {
    int min;
    std::vector<int> res(col_num);
    for (int i = 0;i < col_num;++i) {
        min = matrix[0][i];
        for (int j = 1;j < row_num;++j)
            if (matrix[j][i] < min) min = matrix[j][i];
        res[i] = min;
    }
    return res;
}

std::vector<int> getMinElementsParallel(std::vector<std::vector<int> >& matrix,
                                int row_num, int col_num) {
    MPI_Status st;
    MPI_Comm new_comm;
    MPI_Group group_world, new_group;
    int ProcRank, ProcNum, involved_proc_num;
    const int m = row_num, n = col_num;
    std::vector<int> matrix_in_row(m*n), local_buf, local_res, global_res(n),
                     elem_count, col_count, elem_displ, col_displ, member;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    involved_proc_num = getInvolvedProcNumber(n, ProcNum);
    if (ProcRank == 0)
        matrixToRow(matrix, matrix_in_row, m, n);
    if (involved_proc_num < ProcNum) {
        MPI_Comm_group(MPI_COMM_WORLD, &group_world);
        for (int i = 0;i < involved_proc_num;++i)
            member.push_back(i);
        MPI_Group_incl(group_world, involved_proc_num, member.data(),
                       &new_group);
        MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
    }
    setSendcount(elem_count, col_count, involved_proc_num, n, m);
    setDispl(elem_displ, elem_count, col_displ, col_count);
    if (ProcRank < involved_proc_num) {
        local_buf.resize(elem_count[ProcRank]);
        MPI_Scatterv(matrix_in_row.data(), elem_count.data(),
                     elem_displ.data(), MPI_INT, local_buf.data(),
                     elem_count[ProcRank], MPI_INT, 0,
                     chooseComm(involved_proc_num, ProcNum, new_comm));
        for (int i = 0;i < col_count[ProcRank];++i)
            local_res.push_back(getMinElem(local_buf.data() + m * i, m));
        MPI_Gatherv(local_res.data(), col_count[ProcRank], MPI_INT,
                    global_res.data(), col_count.data(), col_displ.data(),
                    MPI_INT, 0, chooseComm(involved_proc_num, ProcNum, new_comm));
    }
    return global_res;
}