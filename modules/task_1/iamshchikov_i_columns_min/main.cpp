// Copyright 2019 Iamshchikov Ivan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./columns_min.h"

TEST(Parallel_Operations_MPI, cant_create_matrix_with_negative_scale) {
    ASSERT_ANY_THROW(std::vector <std::vector<int> > matrix(-1));
    std::vector <std::vector<int> > matrix(3);
    ASSERT_ANY_THROW(getRandomMatrix(matrix, -5));
}

TEST(Parallel_Operations_MPI, can_create_matrix) {
    std::vector <std::vector<int> > matrix(3);
    getRandomMatrix(matrix, 5);
    ASSERT_EQ(3, matrix.size());
    ASSERT_EQ(5, matrix[0].size());
}

TEST(Parallel_Operations_MPI, can_transform_matrix_to_row) {
    int k = 0;
    const int m = 5, n = 6;
    std::vector <std::vector<int> > matrix(m);
    std::vector<int> matrix_in_row(m*n);
    getRandomMatrix(matrix, n);
    matrixToRow(matrix, matrix_in_row, m, n);
    for (int i = 0;i < n;++i)
        for (int j = 0;j < m;++j)
            ASSERT_EQ(matrix[j][i], matrix_in_row[k++]);
}

TEST(Parallel_Operations_MPI, find_min_in_row) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int m = 1, n = 6;
    if (rank == 0) {
        std::vector <std::vector<int> > matrix(m);
        getRandomMatrix(matrix, n);
        ASSERT_EQ(getMinElementsSequential(matrix, m, n),
            getMinElementsParallel(matrix, m, n));
    }
}

TEST(Parallel_Operations_MPI, find_min_in_col) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int m = 5, n = 1;
    if (rank == 0) {
        std::vector <std::vector<int> > matrix(m);
        getRandomMatrix(matrix, n);
        ASSERT_EQ(getMinElementsSequential(matrix, m, n),
            getMinElementsParallel(matrix, m, n));
    }
}

TEST(Parallel_Operations_MPI, find_mins_in_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int m = 5, n = 6;
    if (rank == 0) {
        std::vector <std::vector<int> > matrix(m);
        getRandomMatrix(matrix, n);
        ASSERT_EQ(getMinElementsSequential(matrix, m, n),
            getMinElementsParallel(matrix, m, n));
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}