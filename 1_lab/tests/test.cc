#include "gtest.h"
#include "../src/structures.h"
#include <stdlib.h>

#include <string>

TEST(Pair, PairFullInt) {
        Structures::Pair<int, int> x = {1, 2};
        ASSERT_EQ(x.first, 1);
        ASSERT_EQ(x.second, 2);

        auto y = x;
        ASSERT_EQ(y.first, 1);
        ASSERT_EQ(y.second, 2);

        ASSERT_NE(&y, &x);
        ASSERT_NE(&y.first, &x.first);

        ASSERT_TRUE(x == y);
}

TEST(Pair, ArrayOfPairs) {
        Structures::Pair<int, int> *x = new Structures::Pair<int, int>[10];
        x[0].first = 10;
        x[0].second = 20;

        x[1].first = 10;
        x[1].second = 20;

        ASSERT_EQ(x[0], x[1]);

        ASSERT_EQ(x[1].first, 10);

        delete[] x;
}

TEST(Pair, PairFloat) {
        Structures::Pair<double, double> x = {1.2, 2.123};
        ASSERT_EQ(x.first, 1.2);
        ASSERT_EQ(x.second, 2.123);

        auto y = x;
        ASSERT_EQ(y.first, 1.2);
        ASSERT_EQ(y.second, 2.123);

        ASSERT_NE(&y, &x);
        ASSERT_NE(&y.first, &x.first);

        ASSERT_TRUE(x == y);
}

TEST(Pair, PairMixedTypes) {
        Structures::Pair<double, std::string> x = {1.2, "hello guys"};
        ASSERT_EQ(x.first, 1.2);
        ASSERT_EQ(x.second, "hello guys");

        ASSERT_TRUE(x == x);
}

TEST(Vector, Constructor) {
        Structures::Vector<int> vec;
        ASSERT_EQ(vec.capacity(), 10);
        ASSERT_EQ(vec.length(), 0);
}

TEST(Vector, PushAndAccess) {
        Structures::Vector<int> vec;
        vec.push(10);
        vec.push(13);

        ASSERT_EQ(vec[0], 10);
        ASSERT_EQ(vec[1], 13);

        ASSERT_EQ(vec[0], vec.get(0));
}

TEST(Vector, Autoresize) {
        Structures::Vector<int> vec;
        for (int i = 0; i < 15; ++i) {
                vec.push(i);
        }

        ASSERT_EQ(vec.capacity(), 20);
        ASSERT_EQ(vec.length(), 15);

        for (int i = 0; i < 15; ++i) {
                ASSERT_EQ(vec[i], i);
        }
}

TEST(Vector, OutOfBounds) {
        Structures::Vector<int> vec;
        vec.push(1);
        vec.push(2);

        EXPECT_THROW({
                        vec[3];
        }, std::out_of_range);
}

TEST(Vector, Set) {
        Structures::Vector<int> vec;
        for (int i = 0; i < 15; ++i) {
                vec.push(i);
        }
        
        vec[10] = 228;
        ASSERT_EQ(vec[10], 228);

        vec.set(1337, 11);
        ASSERT_EQ(vec[11], 1337);
}

TEST(Vector, SetPair) {
	Structures::Vector<Structures::Pair<int, int>> a;
	a.push(Structures::Pair<int, int>(13, 37));
	a[0] = Structures::Pair<int, int>(2, 28);
	bool res = a[0] == Structures::Pair<int, int>(2, 28);
	ASSERT_TRUE(res);
}

TEST(CSRMatrix, Init) {
        Structures::CSRMatrix<double> matrix(10, 10);
}

TEST(CSRMatrix, PushbackFirstline) {
        Structures::CSRMatrix<double> matrix(10, 10);
        for (int i = 0; i < 10; ++i) {
                matrix._push(0, Structures::Pair<size_t, double>(i, i+1.5));
        }
}

TEST(CSRMatrix, PushbackAlllines) {
        Structures::CSRMatrix<double> matrix(10, 10);
        for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                        matrix._push(i, Structures::Pair<size_t, double>(j, i+1.5));
                }
        }

        for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                        bool res = matrix[i][j] == Structures::Pair<size_t, double>(j, i+1.5);
                        ASSERT_TRUE(res);
                }
        }
}

TEST(CSRMatrix, MatrixAccess) {
	Structures::CSRMatrix<int> matrix(10, 10);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			matrix._push(i, Structures::Pair<size_t, int>(j, i));
		}
	}

	Structures::Pair<size_t, int> a;
	a.first = 0;
	a.second = 0.0;

	bool res = a == matrix[0][0];

	ASSERT_TRUE(res);
}

TEST(CSRMatrix, Set) {
	Structures::CSRMatrix<double> matrix(10, 10);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			matrix._push(i, Structures::Pair<size_t, double>(j, i));
		}
	}

	matrix.set(2, 2, 10);
	ASSERT_EQ(matrix[2][2].second, 10);
	
	// matrix[3][3] = Structures::Pair<size_t, double>(matrix[3][3].first, 12);
	// ASSERT_EQ(matrix[3][3].second, 12);
}

TEST(CSRMatrix, RealPushAndGet1) {
	Structures::CSRMatrix<int> matrix(10, 10);
	// 1 0 0 0 0 0 0 0 0 1
	matrix.push(0, 1);
	for (int i = 0; i < 8; ++i) {
		matrix.push(0, 0);
	}
	matrix.push(0, 1);

	ASSERT_EQ(matrix.get(0, 0), 1);
	for (int i = 0; i < 8; ++i) {
		ASSERT_EQ(matrix.get(0, 1+i), 0);
	}
	ASSERT_EQ(matrix.get(0, 9), 1);
}

TEST(CSRMatrix, RealPushAndGet2) {
	Structures::CSRMatrix<int> matrix(10, 10);
	// 0 1 0 1 0 1 0 1 0 1
	for (int i = 0; i < 10; ++i) {
		matrix.push(0, i%2);
	}

	for (int i = 0; i < 10; ++i) {
		ASSERT_EQ(matrix.get(0, i), i%2);
	}
}

TEST(CSRMatrix, RealPushAndGet3) {
	Structures::CSRMatrix<int> matrix(10, 10);
	// 1 2 1 2 1 2 1 2 1 2
	for (int i = 0; i < 10; ++i) {
		matrix.push(0, i%2+1);
	}

	for (int i = 0; i < 10; ++i) {
		ASSERT_EQ(matrix.get(0, i), i%2+1);
	}
}

TEST(CSRMatrix, RealPushAndGet4) {
	srand(0);
	int size = 10;
	Structures::CSRMatrix<int> matrix(size, size);
	Structures::Vector<int> log;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			int val = 0;
			if (rand()%10 > 7) {
				val = rand();
			}
			log.push(val);
			matrix.push(i, val);
		}
	}

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			ASSERT_EQ(matrix.get(i, j), log[i*size+j]);
		}
	}
}
