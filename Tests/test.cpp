//
// Created by Tsinin on 09.11.2018.
//
#include <ctime>
#include <set>
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "binomialHeap.h"

using testing::Eq;

namespace {
    class testFixture : public testing::Test {
    public:
        binomialHeap<int> heap;
        Vector<int> testVector;
        std::multiset<int> testSet;
        testFixture() {

        }
    };
}
TEST_F(testFixture, isEmptyTest1) {
    ASSERT_TRUE(heap.is_empty());
}

TEST_F(testFixture, isEmptyTest2) {
    int t = rand();
    heap.insert(t);
    ASSERT_FALSE(heap.is_empty());
}
TEST_F(testFixture, exceptionTest1) {
    ASSERT_EXIT(heap.get_min(), ::testing::ExitedWithCode(1), "Heap is empty.");
}

TEST_F(testFixture, exceptionTest2) {
    ASSERT_EXIT(heap.extract_min(), ::testing::ExitedWithCode(1), "Heap is empty.");
}

TEST_F(testFixture, smallTest1) {
    heap.insert(1);
    ASSERT_EQ(heap.get_min(), 1);
    heap.insert(2);
    ASSERT_EQ(heap.get_min(), 1);
    heap.insert(3);
    ASSERT_EQ(heap.get_min(), 1);
    ASSERT_EQ(heap.extract_min(), 1);
    ASSERT_EQ(heap.extract_min(), 2);
    ASSERT_EQ(heap.extract_min(), 3);
}

TEST_F(testFixture, smallTest2) {
    binomialHeap<int>::Pointer* p1 = heap.insert(1),
    *p2 = heap.insert(2),
    *p3 = heap.insert(3);
    heap.erase(p1);
    ASSERT_EQ(heap.get_min(), 2);
    heap.erase(p3);
    ASSERT_EQ(heap.get_min(), 2);
    p3 = heap.insert(3);
    heap.extract_min();
    ASSERT_EQ(heap.get_min(), 3);
}

TEST_F(testFixture, smallTest3) {
    binomialHeap<int>::Pointer* p1 = heap.insert(1),
    *p2 = heap.insert(2);
    heap.change(p1, 3);
    ASSERT_EQ(heap.get_min(), 2);
    heap.change(p2, 4);
    ASSERT_EQ(heap.get_min(), 3);
}

TEST_F(testFixture, smallTest4) {
    binomialHeap<int>::Pointer* p1 = heap.insert(1),
    *p2 = heap.insert(2),
    *p3 = heap.insert(3);
    heap.change(p1, 0);
    ASSERT_EQ(heap.get_min(), 0);
    heap.change(p1, 1);
    ASSERT_EQ(heap.get_min(), 1);
    heap.change(p2, 0);
    ASSERT_EQ(heap.get_min(), 0);
    heap.change(p2, 4);
    ASSERT_EQ(heap.extract_min(), 1);
    ASSERT_EQ(heap.extract_min(), 3);
    ASSERT_EQ(heap.extract_min(), 4);
}

TEST_F(testFixture, simpleTest1) {
    for(int i = 400000; i >= 0; --i)
        heap.insert(i);
    for(int i = 0; i < 400000; ++i)
        ASSERT_EQ(i, heap.extract_min());
}

TEST_F(testFixture, simpleTest2) {
    for(int i = 0; i < 400000; ++i)
        heap.insert(i);
    for(int i = 0; i < 400000; ++i)
        ASSERT_EQ(i, heap.extract_min());
}

TEST_F(testFixture, randomSmallTest) {
    for(int i = 0; i < 40000; ++i) {
        int operation = rand() % 2;
        if(operation && !testVector.size() || testVector.size() < 5000)
            operation = 0;
        if(operation) {
            int minElementPtr = 0;
            for(int i = 1; i < testVector.size(); ++i)
                if(testVector[i] < testVector[minElementPtr])
                    minElementPtr = i;
            int minElement = testVector[minElementPtr];
            ASSERT_EQ(heap.get_min(), minElement);
            for(int i = minElementPtr; i < testVector.size() - 1; ++i)
                testVector[i] = testVector[i + 1];
            testVector.pop_back();
            heap.extract_min();
        } else {
            int x = rand();
            testVector.push_back(x);
            heap.insert(x);
        }
    }
}

TEST_F(testFixture, randomBigTest1) {
    time_t start = clock(), end;

    for(int i = 0; i < 1000000; ++i) {
        int operation = rand() % 2;
        if(operation && !testSet.size())
            operation = 0;
        if(operation) {
            ASSERT_EQ(heap.extract_min(), *(testSet.begin()));
            testSet.erase(testSet.begin());
        } else {
            int x = rand();
            testSet.insert(x);
            heap.insert(x);
        }
    }
    end = clock();
    ASSERT_GE(2, ((double)end - start) / (double)(CLOCKS_PER_SEC));
}

TEST_F(testFixture, randomBigTest2) {
    time_t start = clock(), end;

    for(int i = 0; i < 1000000; ++i) {
        int operation = rand() % 2;
        if(operation && !testSet.size() || testSet.size() < 10000)
            operation = 0;
        if(operation) {
            ASSERT_EQ(heap.extract_min(), *(testSet.begin()));
            testSet.erase(testSet.begin());
        } else {
            int x = rand();
            testSet.insert(x);
            heap.insert(x);
        }
    }
    end = clock();
    ASSERT_GE(2, ((double)end - start) / (double)(CLOCKS_PER_SEC));
}

TEST_F(testFixture, randomBigTest3) {
    time_t start = clock(), end;

    for(int i = 0; i < 1000000; ++i) {
        int operation = rand() % 2;
        if(operation  && !testSet.size() || testSet.size() < 70000)
            operation = 0;
        if(operation) {
            ASSERT_EQ(heap.extract_min(), *(testSet.begin()));
            testSet.erase(testSet.begin());
        } else {
            int x = rand();
            testSet.insert(x);
            heap.insert(x);
        }
    }
    end = clock();
    ASSERT_GE(3, ((double)end - start) / (double)(CLOCKS_PER_SEC));
}
