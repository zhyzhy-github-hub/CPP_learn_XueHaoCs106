/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream> // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

//#define VERSION0
#ifdef VERSION0
Queue<int> binaryMerge(Queue<int> a, Queue<int> b)
{
    Queue<int> result;
    /* TODO: Implement this function. */
    // 由于 a, b 的长度会变，提前把它们的长度保存
    int lenA = a.size();
    int lenB = b.size();
    // 如果 b 为空，则返回a
    if (lenB == 0)
    {
        // 加这个循环是为了判断a是否有序
        for (int i = 0; i < lenA; ++i)
        {
            int varA = a.dequeue();
            if (!a.isEmpty() && varA > a.peek())
            {
                error("At least one queue is not order!!!");
            }
            result.enqueue(varA);
        }
    }
    else
    {
        // 定义 b 的元素，先取出一个，索引起始就变成1
        int indexB = 1;
        int varB = b.dequeue();
        // 遍历 a，并排序在 a 的长度内有关的 b 的元素
        for (int i = 0; i < lenA; ++i)
        {
            int varA = a.dequeue();
            // 判断a是否有序
            if (!a.isEmpty() && varA > a.peek())
            {
                error("At least one queue is not order!!!");
            }
            // 如果 varB 不大于 varA，就先遍历部分 b
            while (varA >= varB && indexB < lenB)
            {
                result.enqueue(varB);
                // 这个分支我们可以确保b至少有一个元素
                if (varB > b.peek())
                {
                    error("At least one queue is not order!!!");
                }
                varB = b.dequeue();
                indexB += 1;
            }
            // 由于 while 循环取出的 b 的最后一个元素没有加入result，
            // 额外加一个判断，
            // 为了保证索引 indexB正确，还是要加一个 indexB + =1 操作
            // 这个分支用来处理 b 的最后一个元素
            if (varA >= varB && indexB == lenB)
            {
                result.enqueue(varB);
                indexB += 1;
            }
            // 如果 varB 大于 varA，就把 varA 加入 result
            result.enqueue(varA);
        }
        // 最后，如果 b 还剩下元素，
        // 由于此时剩下的元素都是更大的
        // 只需要再把剩下的元素加入 result
        for (int i = indexB - 1; i < lenB; ++i)
        {
            result.enqueue(varB);
            // 还是要判断b的有序性
            // if (!b.isEmpty())
            // {
            //     varB = b.dequeue();
            // }

            if (!b.isEmpty())
            {
                if (varB <= b.peek())
                {
                    varB = b.dequeue();
                }
                else
                {
                    error("At least one queue is not order!!!");
                }
            }
        }
    }
    return result;
}
#endif

#define VERSION1
#ifdef VERSION1
bool isOrder(Queue<int> a)
{
    int lenA = a.size();
    for (int i = 0; i < lenA; i++)
    {
        int thisOne = a.dequeue();
        if (!a.isEmpty())
        {
            int nextOne = a.peek();
            if (thisOne > nextOne)
            {
                return false;
            }
        }
    }
    return true;
}
STUDENT_TEST("Is the queue order? Student test!!!")
{
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3, 5, 5, 5, 5};
    Queue<int> expected = {1, 2, 3, 3, 4, 5, 5, 5, 5, 5};
    EXPECT_EQUAL(isOrder(a), true);
    EXPECT_EQUAL(isOrder(b), true);
    EXPECT_EQUAL(isOrder(expected), true);
    a = {2, 3, 3, 4, 4, 5, 6, 9};
    b = {1, 1, 3, 3, 3, 4, 8, 9, 9, 9, 10, 10, 11};
    expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_EQUAL(isOrder(a), true);
    EXPECT_EQUAL(isOrder(b), true);
    EXPECT_EQUAL(isOrder(expected), true);
    a = {};
    EXPECT_EQUAL(isOrder(a), true);
    a = {2, 3, 3, 10, 4, 4, 5, 6, 9};
    b = {1, 1, 3, 3, 4, 3, 4, 8, 9, 9, 9, 10, 10, 11};
    EXPECT_EQUAL(isOrder(a), false);
    EXPECT_EQUAL(isOrder(b), false);
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> binaryMerge(Queue<int>  a, Queue<int>  b)
{
    Queue<int> result;
    /* TODO: Implement this function. */
    // 先判断输入是否是排序好的队列
    if (!(isOrder(a) && isOrder(b)))
    {
        error("At least one queue is not order!!!");
    }
    // 由于 a, b 的长度会变，提前把它们的长度保存
    int lenA = a.size();
    int lenB = b.size();
    // 如果 b 为空，则返回a
    if (lenB == 0)
    {
        result = a;
    }
    else
    {
        // 定义 b 的元素，先取出一个，索引起始就变成1
        int indexB = 1;
        int varB = b.dequeue();
        // 遍历 a，并排序在 a 的长度内有关的 b 的元素
        for (int i = 0; i < lenA; ++i)
        {
            int varA = a.dequeue();
            // 如果 varB 不大于 varA，就先遍历部分 b
            while (varA >= varB && indexB < lenB)
            {
                result.enqueue(varB);
                varB = b.dequeue();
                indexB += 1;
            }
            // 由于 while 循环取出的 b 的最后一个元素没有加入result，
            // 额外加一个判断，
            // 为了保证索引 indexB正确，还是要加一个 indexB + =1 操作
            if (varA >= varB && indexB == lenB)
            {
                result.enqueue(varB);
                indexB += 1;
            }
            // 如果 varB 大于 varA，就把 varA 加入 result
            result.enqueue(varA);
        }
        // 最后，如果 b 还剩下元素，
        // 由于此时剩下的元素都是更大的
        // 只需要再把剩下的元素加入 result
        for (int i = indexB - 1; i < lenB; ++i)
        {
            result.enqueue(varB);
            if (!b.isEmpty())
            {
                varB = b.dequeue();
            }
        }
    }
    return result;
}
#endif

#define VERSION_REC
#ifdef VERSION_REC
// 转换接口的递归函数
void binaryMergeRec(Queue<int> a, Queue<int> b, Queue<int> &result)
{
    //    Queue<int> result;
    /* TODO: Implement this function. */
    // 最简单的情况: a, b 都为空
    if (b.isEmpty() && a.isEmpty())
    {
    }
    else
    {
        // 经过上一个判断，a,b 至少有一个不为空
        // 如果 b 为空，那么只需要处理 a if (b.isEmpty())
        if (b.isEmpty())
        {
            // 弹出 a 的队列首元素
            int varA = a.dequeue();
            // 这里判断 a 是不是有序数列，
            // 如果经过上一次弹出操作，a还不为空
            if (!a.isEmpty())
            {
                // 比较当前弹出值与下一个元素
                // 如果弹出值比下一个还大，那么报错
                if (varA > a.peek())
                {
                    error("At least one queue is not order!!!");
                }
            }
            // 如果前面的判断没有报错，就在结果加上当前值
            result.enqueue(varA);
        }
        else if (a.isEmpty())
        {
            // 同 b 为空的分支
            int varB = b.dequeue();
            if (!b.isEmpty())
            {
                if (varB > b.peek())
                {
                    error("At least one queue is not order!!!");
                }
            }
            result.enqueue(varB);
        }
        else
        {
            // 如果 a, b 都不为空
            // 先比较a,b队列首元素，不弹出
            int varA = a.peek();
            int varB = b.peek();
            // 谁的首元素小，就弹出谁的首元素
            int resultIn = (varA >= varB ? b.dequeue() : a.dequeue());
            // 这个时候需要判断数列的有序性，
            // 由于刚才没有显式说明弹出了谁的元素，现在需要知道是谁被弹出了
            // 为了减少复制开销，构造一个指针变量，指向被弹出队列的地址
            Queue<int> *arrOut = &(varA >= varB ? b : a);

            // 判断被弹出元素队列的有序性
            if (!arrOut->isEmpty())
            {
                // 这里只读取值，不弹出，确保指针不会改变变量
                if (resultIn > arrOut->peek())
                {
                    error("At least one queue is not order!!!");
                }
            }
            // 上面过程无误，result 加入新元素
            result.enqueue(resultIn);
        }
        // 进入下一次递归
        binaryMergeRec(a, b, result);
    }
}
// 二元归并递归实现版本
Queue<int> binaryMerge01(Queue<int> a, Queue<int> b)
{
    Queue<int> result;
    // 用引用将每一个结果保存进 result，需要再写一个接口递归函数
    binaryMergeRec(a, b, result);
    return result;
}

STUDENT_TEST("Rec test, binaryMerge01, two short sequences")
{
    Queue<int> a = {2, 2, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 2, 3, 3, 5};
    EXPECT_EQUAL(binaryMerge01(a, b), expected);
    EXPECT_EQUAL(binaryMerge01(b, a), expected);
    a = {};
    b = {};
    expected = {};
    EXPECT_EQUAL(binaryMerge01(a, b), expected);
    EXPECT_EQUAL(binaryMerge01(b, a), expected);
    a = {};
    b = {4};
    expected = {4};
    EXPECT_EQUAL(binaryMerge01(a, b), expected);
    EXPECT_EQUAL(binaryMerge01(b, a), expected);
}
STUDENT_TEST("Rec test, binaryMerge01, if one queue is unorder, we expect error will occur!!!")
{
    Queue<int> a = {2, 3, 3, 5, 4, 4, 5, 6, 9};
    Queue<int> b = {1, 1, 3, 3, 3, 4, 8, 9, 9, 9, 10, 10, 11};
    Queue<int> expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_ERROR(binaryMerge01(a, b));
    EXPECT_ERROR(binaryMerge01(b, a));
    a = {2, 3, 3, 4, 4, 5, 6, 9};
    b = {1, 1, 3, 3, 3, 5, 4, 8, 9, 9, 9, 10, 10, 11};
    expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_ERROR(binaryMerge01(a, b));
    EXPECT_ERROR(binaryMerge01(b, a));
    a = {2, 3, 3, 4, 7, 4, 5, 6, 9};
    b = {1, 1, 3, 3, 3, 4, 8, 9, 9, 9, 10, 10, 11};
    expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_ERROR(binaryMerge01(a, b));
    EXPECT_ERROR(binaryMerge01(b, a));
    a = {2, 3, 4, 5, 6, 9};
    b = {1, 1, 3, 3, 3, 4, 8, 9, 9, 9, 10, 10, 12, 11};
    expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_ERROR(binaryMerge01(a, b));
    EXPECT_ERROR(binaryMerge01(b, a));
    b = {};
    a = {1, 1, 3, 3, 3, 4, 8, 9, 9, 9, 10, 10, 12, 11};
    expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_ERROR(binaryMerge01(a, b));
    EXPECT_ERROR(binaryMerge01(b, a));
}
#endif

STUDENT_TEST("If one queue is unorder, we expect error will occur!!!")
{
    Queue<int> a = {2, 3, 3, 5, 4, 4, 5, 6, 9};
    Queue<int> b = {1, 1, 3, 3, 3, 4, 8, 9, 9, 9, 10, 10, 11};
    Queue<int> expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
    a = {2, 3, 3, 4, 4, 5, 6, 9};
    b = {1, 1, 3, 3, 3, 5, 4, 8, 9, 9, 9, 10, 10, 11};
    expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
    a = {2, 3, 3, 4, 7, 4, 5, 6, 9};
    b = {1, 1, 3, 3, 3, 4, 8, 9, 9, 9, 10, 10, 11};
    expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
    a = {2, 3, 4, 5, 6, 9};
    b = {1, 1, 3, 3, 3, 4, 8, 9, 9, 9, 10, 10, 12, 11};
    expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
    b = {};
    a = {1, 1, 3, 3, 3, 4, 8, 9, 9, 9, 10, 10, 12, 11};
    expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}

STUDENT_TEST("binaryMerge, two short sequences")
{
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3, 5, 5, 5, 5};
    Queue<int> expected = {1, 2, 3, 3, 4, 5, 5, 5, 5, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
    a = {};
    b = {};
    expected = {};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
    a = {};
    b = {4};
    expected = {4};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
    a = {2, 3, 3, 4, 4, 5, 6, 9};
    b = {1, 1, 3, 3, 3, 4, 8, 9, 9, 9, 10, 10, 11};
    expected = {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 8, 9, 9, 9, 9, 10, 10, 11};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>> &all)
{
    Queue<int> result;

    for (Queue<int> &q : all)
    {
        result = binaryMerge(q, result);
    }
    return result;
}

STUDENT_TEST("naiveMultiMerge, small collection of short sequences")
{
    Vector<Queue<int>> all = {{},
                              {},
                              {},
                              {},
                              {-5},
                              {3402}};
    Queue<int> expected = {-5, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
    all = {
        {},
        {},
        {},
        {},
        {},
    };
    expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}
STUDENT_TEST("naiveMultiMerge, error test")
{
    Vector<Queue<int>> all = {{},
                              {},
                              {2, 2, 2, 2, 2},
                              {},
                              {-5, -6},
                              {3402, 32323}};
    EXPECT_ERROR(naiveMultiMerge(all));
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>> &all)
{
    Queue<int> result;
    /* TODO: Implement this function. */
    int sizeOfAll = all.size();
    if (sizeOfAll == 1)
    {
//        return binaryMerge(all.get(0), result);
        return binaryMerge(all[0], result);
    }
    else
    {
        int halfSize = sizeOfAll / 2;
        auto left = all.subList(0, halfSize);
        auto right = all.subList(halfSize, sizeOfAll - halfSize);
        Queue<int> res1 = recMultiMerge(left);
        Queue<int> res2 = recMultiMerge(right);
        result = binaryMerge(res1, res2);
    }
    return result;
}

STUDENT_TEST("recMultiMerge, about empty queue, small collection of short sequences")
{
    Vector<Queue<int>> all = {{},
                              {},
                              {},
                              {},
                              {-5},
                              {3402}};
    Queue<int> expected = {-5, 3402};
    EXPECT_EQUAL(recMultiMerge(all), expected);
    all = {
        {},
        {},
        {},
        {},
        {},
    };
    expected = {};
    EXPECT_EQUAL(recMultiMerge(all), expected);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
    all = {
        {},
    };
    expected = {};
    EXPECT_EQUAL(recMultiMerge(all), expected);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
    all = {
        {5, 6, 7},
    };
    expected = {5, 6, 7};
    EXPECT_EQUAL(recMultiMerge(all), expected);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

STUDENT_TEST("recMultiMerge, error test")
{
    Vector<Queue<int>> all = {{},
                              {},
                              {2, 2, 2, 2, 2},
                              {},
                              {-5, -6},
                              {3402, 32323}};
    EXPECT_ERROR(recMultiMerge(all));
}

STUDENT_TEST("recMultiMerge, small collection of short sequences")
{
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                              {1, 5, 9, 9, 12},
                              {5},
                              {},
                              {-5, -5},
                              {3402}};
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(recMultiMerge(all), expected);
}

//---------------------------------------------------------------
// This is all of the provided test
//---------------------------------------------------------------

/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>> &all);

PROVIDED_TEST("binaryMerge, two short sequences")
{
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences")
{
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                              {1, 5, 9, 9, 12},
                              {5},
                              {},
                              {-5, -5},
                              {3402}};
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge")
{
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation")
{
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation")
{
    int n = 11000;
    int k = n / 10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation")
{
    int n = 90000;
    int k = n / 10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size)
{
    Queue<int> q;
    for (int i = 0; i < size; i++)
    {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>> &all)
{
    while (!input.isEmpty())
    {
        all[randomInteger(0, all.size() - 1)].enqueue(input.dequeue());
    }
}
