// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream> // for cout, endl
#include <string>   // for string class
#include "voting.h"
#include "stack.h"
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

// 自己先写一个求和函数
int sumVecInt(const Vector<int> &vec)
{
    int sum = 0;
    for (const auto &item : vec)
    {
        sum += item;
    }
    return sum;
}
STUDENT_TEST("Test sumVecInt ------------- sum ---------")
{
    Vector<int> blocks = {50, 49, 1};
    EXPECT_EQUAL(sumVecInt(blocks), 100);
    blocks = {1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1};
    EXPECT_EQUAL(sumVecInt(blocks), 12);
}

//#define RES01
//不管怎么样，先实现出来，然后再精简化简代码提高效率
#ifdef RES01
// 递归法求子集，引用参数
void computePowerIndexesRec(const Vector<int> &blocks,
                            Vector<int> &tempSubset,
                            int index,
                            Vector<Vector<int>> &resultSet,
                            Vector<int> &powerIndex,
                            Vector<int> &result)
{
    const int sumBlocks = sumVecInt(blocks);
    // 每一个决策树分支都遍历 blocks.size() 次
    // 不管最后有几个元素，只要遍历次数够了，就加入子集的集合
    if (index == blocks.size())
    {
        resultSet.add(tempSubset);
        for (auto &item : powerIndex)
        {
            int sumSubset = sumVecInt(tempSubset);

            if (double(sumSubset) / sumBlocks >= 0.5 && double(sumSubset - blocks[item]) / sumBlocks < 0.5)
            {
                result[item] += 1;
            }
        }
        cout << powerIndex << endl;
    }
    else
    {
        // 递归过程不能改变中间变量值
        // 加入下一个元素的中间变量
        tempSubset.add(blocks[index]);
        powerIndex.add(index);
        computePowerIndexesRec(blocks, tempSubset, index + 1, resultSet, powerIndex, result);
        // 加入空集的中间变量
        tempSubset.remove(tempSubset.size() - 1);
        powerIndex.remove(powerIndex.size() - 1);
        computePowerIndexesRec(blocks, tempSubset, index + 1, resultSet, powerIndex, result);
    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexesMy(Vector<int> &blocks)
{
    Vector<int> result;

    // TODO your code here
    Vector<int> powerIndex(0, 0);
    Vector<int> resultPercent(blocks.size(), 0);
    Vector<Vector<int>> resultSet;
    int index = 0;
    Vector<int> tempSubset;
    computePowerIndexesRec(blocks, tempSubset, index, resultSet, powerIndex, resultPercent);

    cout << "-------------" << endl;

    int sumAllPoint = sumVecInt(resultPercent);
    for (const auto &item : resultPercent)
    // for (const auto &item : resultSet)
    {
        // 为了保险起见，除法先强制转换为double，结果再强制转换为int
        result.add(int(double(item) / sumAllPoint * 100));
        cout << item << ", ";
    }
    cout << endl;
    return result;
}

/* * * * * * Test Cases * * * * * */

STUDENT_TEST("Student --- Test power index, blocks 50-49-1")
{
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexesMy(blocks), expected);
}
STUDENT_TEST("Student --- Test power index, blocks CA-TX-GA 55-38-16")
{
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexesMy(blocks), expected);
}

STUDENT_TEST("Student --- Test power index, blocks CA-TX-NY 55-38-39")
{
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexesMy(blocks), expected);
}
STUDENT_TEST("Student --- Test power index, blocks CA-TX-NY 55-10-10-10-10-5")
{
    Vector<int> blocks = {55, 10, 10, 10, 10, 5};
    Vector<int> expected = {100, 0, 0, 0, 0, 0};
    EXPECT_EQUAL(computePowerIndexesMy(blocks), expected);
}
STUDENT_TEST("Student --- Test power index, blocks CA-TX-NY 55-5-5-10-10-10-5")
{
    Vector<int> blocks = {55, 5, 5, 10, 10, 10, 5};
    Vector<int> expected = {100, 0, 0, 0, 0, 0, 0};
    EXPECT_EQUAL(computePowerIndexesMy(blocks), expected);
}
// STUDENT_TEST("Student --- Test power index, blocks EU post-Nice")
//{
//     // Estonia is one of those 4s!!
//     Vector<int> blocks = {29, 29, 29, 29, 27, 27, 14, 13, 12, 12, 12, 12, 12, 10, 10, 10, 7, 7, 7, 7, 7, 4, 4, 4, 4, 4, 3};
//     Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
//     EXPECT_EQUAL(computePowerIndexesMy(blocks), expected);
// }

#endif

// #define RES02
// 优化效率
#ifdef RES02
/// Constructor
/** \param blocks      原始的所有区块联盟
 *  \param tempSubset  当前的临时子集
 *  \param index       当前决策树到达的索引
 *  \param powerIndex  向量，记录子集里面的元素在 blocks 里面的位置索引，用于计数
 *  \param result      向量，记录每个目标区块成为关键区块的次数
 */
void computePowerIndexesRec(const Vector<int> &blocks,
                            Vector<int> &tempSubset,
                            int index,
                            Vector<int> &powerIndex,
                            Vector<int> &result)
{
    // 计算一次总和,const变量
    const int sumBlocks = sumVecInt(blocks);
    // 每一个决策树分支都遍历 blocks.size() 次
    // 不管最后有几个元素，只要遍历次数够了，到达决策树末端，就加入子集的集合
    if (index == blocks.size())
    {
        for (auto &item : powerIndex)
        {
            int sumSubset = sumVecInt(tempSubset);

            // 有这个区块，获胜或平均，
            // 没有这个区块，必输
            if (double(sumSubset) / sumBlocks >= 0.5 && double(sumSubset - blocks[item]) / sumBlocks < 0.5)
            {
                result[item] += 1;
            }
        }
        // cout << powerIndex << endl;
    }
    else
    {
        // 递归过程不能改变中间变量值
        if (double(sumVecInt(tempSubset)) / sumBlocks <= 0.5)
        {
            // 加入下一个区块的子集联盟
            tempSubset.add(blocks[index]);
            powerIndex.add(index);
            computePowerIndexesRec(blocks, tempSubset, index + 1, powerIndex, result);

            // 不加入目标区块（即，加入空寂空集）的子集联盟
            tempSubset.remove(tempSubset.size() - 1);
            powerIndex.remove(powerIndex.size() - 1);
            computePowerIndexesRec(blocks, tempSubset, index + 1, powerIndex, result);
        }
        else
        {
            computePowerIndexesRec(blocks, tempSubset, blocks.size(), powerIndex, result);
        }
    }
}
Vector<int> computePowerIndexesMy02(Vector<int> &blocks)
{
    Vector<int> result;
    // TODO your code here
    // 记录子集联盟里面的区块在 blocks 里面的位置索引，用于计数
    Vector<int> powerIndex;
    // 记录每个区块成为关键区块的次数,每一个从零开始
    Vector<int> resultPercent(blocks.size(), 0);
    // 决策树索引
    int index = 0;
    // 临时联盟子集
    Vector<int> tempSubset;

    // 开始递归
    computePowerIndexesRec(blocks, tempSubset, index, powerIndex, resultPercent);

    cout << "-------------" << endl;
    int sumAllPoint = sumVecInt(resultPercent);
    for (const auto &item : resultPercent)
    {
        // 为了保险起见，除法先强制转换为double，结果再强制转换为int
        result.add(int(double(item) / sumAllPoint * 100));
        cout << item << ", ";
    }
    cout << endl;
    return result;
}
#endif

//#define RES03
// 优化效率
#ifdef RES03

// #define useRemove
#ifdef useRemove

/// Constructor
/** \param blocks      原始的所有区块联盟
 *  \param tempSubset  当前的临时子集
 *  \param index       当前决策树到达的索引
 *  \param powerIndex  向量，记录子集里面的元素在 blocks 里面的位置索引，用于计数
 *  \param result      向量，记录每个目标区块成为关键区块的次数
 */
void computePowerIndexesRec(const Vector<int> &blocks,
                            Vector<int> &tempSubset,
                            int index,
                            const int sumBlocks,
                            int sumSubset,
                            Vector<int> &powerIndex,
                            Vector<int> &result)
{
    // 每一个决策树分支都遍历 blocks.size() 次
    // 不管最后有几个元素，只要遍历次数够了，到达决策树末端，就加入子集的集合
    if (index == blocks.size())
    {
        for (auto &item : powerIndex)
        {
            // 有这个区块，获胜或平均，
            // 且 没有这个区块，必输
            // if (double(sumSubset) / sumBlocks >= 0.5 && double(sumSubset - blocks[item]) / sumBlocks < 0.5)
            if (sumSubset > sumBlocks / 2 && (sumSubset - blocks[item]) <= sumBlocks / 2)
            {
                result[item]++;
            }
        }
    }
    else
    {
        if (sumSubset <= sumBlocks / 2)
        {
            // 递归过程不能改变中间变量值
            // 如果积分票数不大于一半的总票数，当前选票可能会是关键票数
            // 然后就要包含这个索引

            // 加入下一个区块的子集联盟
            tempSubset.add(blocks[index]);
            powerIndex.add(index);

            sumSubset += blocks[index];
            computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);

            // 不加入目标区块（即，加入空寂空集）的子集联盟
            sumSubset -= blocks[index];
            tempSubset.remove(tempSubset.size() - 1);

            powerIndex.remove(powerIndex.size() - 1);
            computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);
        }
        else
        {
            // 如果积分票数已经大于一半的总票数，当前选票可能会是关键票数
            // 那么有没有后面的票都无所谓，后面的不可能成为关键票数
            // 不必包含这个索引

            // 递归过程不能改变中间变量值
            tempSubset.add(blocks[index]);
            sumSubset += blocks[index];
            computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);

            // 不加入目标区块（即，加入空寂空集）的子集联盟
            sumSubset -= blocks[index];
            tempSubset.remove(tempSubset.size() - 1);
            computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);
        }
    }
}
#endif

// // 递归过程不能改变中间变量值
// if (double(sumSubset) / sumBlocks > 0.5)
// {
//     // 加入下一个区块的子集联盟
//     tempSubset.add(blocks[index]);
//     //             powerIndex.add(index);
//     sumSubset += blocks[index];
//     computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);

//     // 不加入目标区块（即，加入空寂空集）的子集联盟
//     sumSubset -= blocks[index];
//     tempSubset.remove(tempSubset.size() - 1);
//     //            powerIndex.remove(powerIndex.size() - 1);
//     computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);
// }
// else
// { // 加入下一个区块的子集联盟
//     tempSubset.add(blocks[index]);
//     powerIndex.add(index);
//     sumSubset += blocks[index];
//     computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);

//     // 不加入目标区块（即，加入空寂空集）的子集联盟
//     sumSubset -= blocks[index];
//     tempSubset.remove(tempSubset.size() - 1);
//     powerIndex.remove(powerIndex.size() - 1);
//     computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);
// }

// -----------------------------------------------------------------------------------------------/
// 递归过程不能改变中间变量值
// 加入下一个区块的子集联盟
// tempSubset.add(blocks[index]);

// // if (double(sumSubset) / sumBlocks <= 0.5)
// if (sumSubset <= sumBlocks / 2)
// {
//     powerIndex.add(index);
// }

// sumSubset += blocks[index];
// computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);

// // 不加入目标区块（即，加入空寂空集）的子集联盟
// sumSubset -= blocks[index];
// tempSubset.remove(tempSubset.size() - 1);

// if (sumSubset <= sumBlocks / 2)
// // if (double(sumSubset) / sumBlocks <= 0.5)
// {
//     powerIndex.remove(powerIndex.size() - 1);
// }

// computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);
// -----------------------------------------------------------------------------------------------/

Vector<int> computePowerIndexesMy02(Vector<int> &blocks)
{
    Vector<int> result;
    // TODO your code here
    // 记录子集联盟里面的区块在 blocks 里面的位置索引，用于计数
    Vector<int> powerIndex;
    // 记录每个区块成为关键区块的次数,每一个从零开始
    Vector<int> resultPercent(blocks.size(), 0);
    // 决策树索引
    int index = 0;
    // 临时联盟子集
    Vector<int> tempSubset;
    const int sumBlocks = sumVecInt(blocks);
    int sumSubset = 0;
    // 开始递归
    computePowerIndexesRec(blocks, tempSubset, index, sumBlocks, sumSubset, powerIndex, resultPercent);

    cout << "-------------" << endl;
    int sumAllPoint = sumVecInt(resultPercent);
    for (const auto &item : resultPercent)
    {
        // 为了保险起见，除法先强制转换为double，结果再强制转换为int
        result.add(int(double(item) / sumAllPoint * 100));
        cout << item << ", ";
    }
    cout << endl;
    return result;
}
#endif

#define doNotUseRemove
#ifdef doNotUseRemove
/// recursion function
/** \param blocks      原始的所有区块联盟
 *  \param tempSubset  栈，当前的临时子集
 *  \param index       当前决策树到达的索引
 *  \param sumBlocks   总共有多少积分
 *  \param sumSubset   现在这个联盟有多少积分
 *  \param powerIndex  栈，记录子集里面的元素在 blocks 里面的位置索引，用于计数
 *  \param result      向量，记录每个目标区块成为关键区块的次数
 */
void computePowerIndexesRec(const Vector<int> &blocks,
                            Stack<int> &tempSubset,
                            int index,
                            const int sumBlocks,
                            int sumSubset,
                            Stack<int> &powerIndex,
                            Vector<int> &result)
{
    // 每一个决策树分支都遍历 blocks.size() 次
    // 不管最后有几个元素，只要遍历次数够了，到达决策树末端，就加入子集的集合
    if (index == blocks.size())
    {
        Stack<int> powerIndex01 = powerIndex;
        const int powerIndexSize = powerIndex.size();
        for (int i = 0; i < powerIndexSize; ++i)
        {
            // 有这个区块，获胜或平均，
            // 且 没有这个区块，必输
            int item = powerIndex01.pop();
            if (sumSubset > sumBlocks / 2 && (sumSubset - blocks[item]) <= sumBlocks / 2)
            {
                result[item]++;
            }
        }
    }
    else
    {
        if (sumSubset <= sumBlocks / 2)
        {
            // -------------------------------------------------------
            // 递归过程不能改变中间变量值
            // 如果积分票数不大于一半的总票数，当前选票可能会是关键票数
            // 然后就要包含这个索引
            // -------------------------------------------------------

            // 加入下一个区块的子集联盟
            tempSubset.push(blocks[index]);
            powerIndex.push(index);
            sumSubset += blocks[index];
            computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);
            // 不加入目标区块（即，加入空寂空集）的子集联盟
            sumSubset -= blocks[index];
            tempSubset.pop();

            powerIndex.pop();
            computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);
        }
        else
        {
            // -------------------------------------------------------
            // 如果积分票数已经大于一半的总票数，当前选票可能会是关键票数
            // 那么有没有后面的票都无所谓，后面的不可能成为关键票数
            // 不必包含这个索引
            // -------------------------------------------------------

            // 递归过程不能改变中间变量值
            tempSubset.push(blocks[index]);
            sumSubset += blocks[index];
            computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);
            // 不加入目标区块（即，加入空寂空集）的子集联盟
            sumSubset -= blocks[index];
            tempSubset.pop();
            computePowerIndexesRec(blocks, tempSubset, index + 1, sumBlocks, sumSubset, powerIndex, result);
        }
    }
}

Vector<int> computePowerIndexesMy02(Vector<int> &blocks)
{
    Vector<int> result;
    // TODO your code here
    // 记录子集联盟里面的区块在 blocks 里面的位置索引，用于计数
    Stack<int> powerIndex;
    // 记录每个区块成为关键区块的次数,每一个计数都从零开始
    Vector<int> resultSocre(blocks.size(), 0);
    // 决策树索引
    int index = 0;
    // 临时联盟子集
    Stack<int> tempSubset;
    const int sumBlocks = sumVecInt(blocks);
    // 积分总和
    int sumSubset = 0;

    // 开始递归
    computePowerIndexesRec(blocks, tempSubset, index, sumBlocks, sumSubset, powerIndex, resultSocre);

    cout << "-------------------------------------------------------" << endl;
    int sumAllPoint = sumVecInt(resultSocre);
    for (const int item : resultSocre)
    {
        // 为了保险起见，除法先强制转换为double，结果再强制转换为int
        result.add(int(double(item) / sumAllPoint * 100));
        cout << item << ", ";
    }
    cout << endl;
    return result;
}
#endif

STUDENT_TEST("Student --- Test power index, blocks 50-49-1")
{
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexesMy02(blocks), expected);
}
STUDENT_TEST("Student --- Test power index, blocks CA-TX-GA 55-38-16")
{
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexesMy02(blocks), expected);
}

STUDENT_TEST("Student --- Test power index, blocks CA-TX-NY 55-38-39")
{
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexesMy02(blocks), expected);
}
STUDENT_TEST("Student --- Test power index, blocks CA-TX-NY 55-10-10-10-10-5")
{
    Vector<int> blocks = {55, 10, 10, 10, 10, 5};
    Vector<int> expected = {100, 0, 0, 0, 0, 0};
    EXPECT_EQUAL(computePowerIndexesMy02(blocks), expected);
}
STUDENT_TEST("Student --- Test power index, blocks CA-TX-NY 55-5-5-10-10-10-5")
{
    Vector<int> blocks = {55, 5, 5, 10, 10, 10, 5};
    Vector<int> expected = {100, 0, 0, 0, 0, 0, 0};
    EXPECT_EQUAL(computePowerIndexesMy02(blocks), expected);
}
STUDENT_TEST("Student --- Test power index, blocks CA-TX-NY 51, 9, 5, 10, 10, 10, 5")
{
    Vector<int> blocks = {51, 9, 5, 10, 10, 10, 5};
    Vector<int> expected = {100, 0, 0, 0, 0, 0, 0};
    EXPECT_EQUAL(computePowerIndexesMy02(blocks), expected);
}
STUDENT_TEST("Student --- Test power index, blocks CA-TX-NY 51, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 10, 10, 10, 5")
{
    Vector<int> blocks = {51, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 10, 10, 10, 5};
    Vector<int> expected = {100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    EXPECT_EQUAL(computePowerIndexesMy02(blocks), expected);
}
// STUDENT_TEST("Student --- Test power index, blocks EU post-Nice 00")
// {
//     // Estonia is one of those 4s!!
//     Vector<int> blocks = {29, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
//     Vector<int> expected = {8, 8, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
//     EXPECT_EQUAL(computePowerIndexesMy02(blocks), expected);
// }
STUDENT_TEST("Student --- Test power index, blocks EU post-Nice")
{
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29, 29, 29, 29, 27, 27, 14, 13, 12, 12, 12, 12, 12, 10, 10, 10, 7, 7, 7, 7, 7, 4, 4, 4, 4, 4, 3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    // EXPECT_EQUAL(computePowerIndexesMy02(blocks), expected);
    TIME_OPERATION(27, (computePowerIndexesMy02(blocks)));
}

STUDENT_TEST("Time power index operation")
{
    for (int t = 2; t < 28; ++t)
    {
        Vector<int> blocks;
        for (int i = 0; i < t; i++)
        {
            blocks.add(randomInteger(1, 10));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int> &blocks)
{
    Vector<int> result;
    // TODO your code here
    result = computePowerIndexesMy02(blocks);
    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1")
{
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9")
{
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39")
{
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16")
{
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice")
{
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29, 29, 29, 29, 27, 27, 14, 13, 12, 12, 12, 12, 12, 10, 10, 10, 7, 7, 7, 7, 7, 4, 4, 4, 4, 4, 3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation")
{
    Vector<int> blocks;
    for (int i = 0; i < 15; i++)
    {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}
