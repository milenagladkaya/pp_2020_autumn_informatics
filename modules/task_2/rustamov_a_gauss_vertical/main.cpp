// Copyright 2020 Rustamov Azer
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./gauss_vertical.h"

#define EPSILON 0.000001

TEST(Gauss_Vertical, Incorrect_sizes) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 0, cols = 10, vecSize = 10;
    Matrix matrix;
    Matrix vec;
    if (procRank == 0) {
        matrix = RandomMatrix(rows, cols);
        vec = RandomMatrix(vecSize, 1);
    }
    ASSERT_ANY_THROW(ParallelGauss(matrix, rows, cols, vec, vecSize));
}

TEST(Gauss_Vertical, Correct_Answer_Seq_3x3x3) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    const int rows = 3, cols = 3;
    Matrix mat = {  1, 5, 3,
                    4, 2, -1,
                    -2, 2, 3};
    Matrix vec = {  -3, 2, 0};
    if (procRank == 0) {
        Matrix result = SequentialGauss(mat, rows, cols, vec, rows);
        Matrix exRes = { 7, -8, 10 };
        for (int row = 0; row < rows; row++) {
            ASSERT_NEAR(result[row], exRes[row], EPSILON);
        }
    }
}

TEST(Gauss_Vertical, Correct_Answer_Seq_4x4x4) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  const int rows = 4, cols = 4;
  Matrix mat = {1, 2, 3, 4,
                5, 6, 7, 8,
                9, 1, 2, 3,
                4, 5, 1, 2};
  Matrix vec = {3, 7, 5, 1};
  if (procRank == 0) {
    Matrix result = SequentialGauss(mat, rows, cols, vec, rows);
    Matrix exRes = { 0.333333, -0.333333, 0.666667, 0.333333 };
    for (int row = 0; row < rows; row++) {
        ASSERT_NEAR(result[row], exRes[row], EPSILON);
    }
  }
}

TEST(Gauss_Vertical, Correct_Answer_Par_3x3x3) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    const int rows = 3, cols = 3;
    Matrix mat = {  1, 5, 3,
                    4, 2, -1,
                    -2, 2, 3};
    Matrix vec = {  -3, 2, 0};
    Matrix result = ParallelGauss(mat, rows, cols, vec, rows);
    if (procRank == 0) {
      Matrix exRes = SequentialGauss(mat, rows, cols, vec, rows);
      for (int row = 0; row < rows; row++) {
          ASSERT_NEAR(result[row], exRes[row], EPSILON);
      }
  }
}
TEST(Gauss_Vertical, Correct_Answer_Par_4x4x4) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  const int rows = 4, cols = 4;
  Matrix mat = {1, 2, 3, 4,
                5, 6, 7, 8,
                9, 1, 2, 3,
                4, 5, 1, 2};
  Matrix vec = {3, 7, 5, 1};
  Matrix result = ParallelGauss(mat, rows, cols, vec, rows);
  if (procRank == 0) {
    Matrix exRes = SequentialGauss(mat, rows, cols, vec, rows);
    for (int row = 0; row < rows; row++) {
        ASSERT_NEAR(result[row], exRes[row], EPSILON);
    }
  }
}

TEST(Gauss_Vertical, 4x4x4) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 4, cols = 4, vecSize = 4;
    Matrix matrix;
    Matrix vec;
    Matrix res1, res2;
    if (procRank == 0) {
        matrix = RandomMatrix(rows, cols);
        vec = RandomMatrix(vecSize, 1);
    }
    res1 = ParallelGauss(matrix, rows, cols, vec, vecSize);
    if (procRank == 0) {
        Matrix res2 = SequentialGauss(matrix, rows, cols, vec, vecSize);
        for (int row = 0; row < rows; row++) {
            ASSERT_NEAR(res1[row], res2[row], EPSILON);
        }
    }
}

TEST(Gauss_Vertical, 5x5x5) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 5, cols = 5, vecSize = 5;
    Matrix matrix;
    Matrix vec;
    Matrix res1, res2;
    if (procRank == 0) {
        matrix = RandomMatrix(rows, cols);
        vec = RandomMatrix(vecSize, 1);
    }
    res1 = ParallelGauss(matrix, rows, cols, vec, vecSize);
    if (procRank == 0) {
        Matrix res2 = SequentialGauss(matrix, rows, cols, vec, vecSize);
        for (int row = 0; row < rows; row++) {
            ASSERT_NEAR(res1[row], res2[row], EPSILON);
        }
    }
}

TEST(Gauss_Vertical, 6x6x6) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 30, cols = 30, vecSize = 30;
    Matrix matrix;
    Matrix vec;
    Matrix res1, res2;
    if (procRank == 0) {
        matrix = RandomMatrix(rows, cols);
        vec = RandomMatrix(vecSize, 1);
    }
    res1 = ParallelGauss(matrix, rows, cols, vec, vecSize);
    if (procRank == 0) {
        Matrix res2 = SequentialGauss(matrix, rows, cols, vec, vecSize);
        for (int row = 0; row < rows; row++) {
            ASSERT_NEAR(res1[row], res2[row], EPSILON);
        }
    }
}


TEST(Gauss_Vertical, 100x100x100) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 100, vecSize = 100, cols = 100;
    Matrix vec;
    Matrix matrix;
    Matrix res1, res2;
    if (procRank == 0) {
        matrix = RandomMatrix(rows, cols);
        vec = RandomMatrix(vecSize, 1);
    }
    double time_start1 = MPI_Wtime();
    res1 = ParallelGauss(matrix, rows, cols, vec, vecSize);
    double time_end1 = MPI_Wtime();
    if (procRank == 0) {
        double time_start2 = MPI_Wtime();
        Matrix res2 = SequentialGauss(matrix, rows, cols, vec, vecSize);
        double time_end2 = MPI_Wtime();
        for (int row = 0; row < rows; row++) {
            ASSERT_NEAR(res1[row], res2[row], EPSILON);
        }
        std::cout << "Seq:" << time_end2 - time_start2 << std::endl;
        std::cout << "Par:" << time_end1 - time_start1 << std::endl;
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
