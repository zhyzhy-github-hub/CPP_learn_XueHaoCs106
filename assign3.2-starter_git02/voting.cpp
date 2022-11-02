// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "map.h"
#include "stack.h"
#include "testing/SimpleTest.h"
using namespace std;

// 自己先写一个求和函数
template <class T>
T sumVecInt(const Vector<T> &vec)
{
    T sum = 0;
    for (const auto &item : vec)
    {
        sum += item;
    }
    return sum;
}


//#define doNotUseRemove
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


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int> &blocks)
{
    Vector<int> result;
    // TODO your code here
    result = computePowerIndexesMy02(blocks);
    return result;
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
#endif

//#define doRecNiu00
#ifdef doRecNiu00
// computePowerIndexes的辅助函数
// 找出不包括当前地区的所有组合（子集）并判断该地区是否为该组合的关键票。
// 最终返回当前地区的关键票总数。

int computePowerIndexesHelper(const Vector<int>& blocks, const int& winVotes, const int& curBlockIndex,
                              int soFarVotes, int index) { // v2 start

    if (index == curBlockIndex) // 跳过当前地区
        index += 1;

    if (soFarVotes >= winVotes) // 不加当前地区的票数已经胜选，则当前地区肯定不是关键区，该二叉树节点向下所有节点均不是关键区，不需要再继续了
        return 0;

    if (index == blocks.size()) {

        // 当组合总票数未超过总票数的一半 且 加上当前地区的票就能超一半，说明当前地区是关键区，返回 1。
        if (soFarVotes < winVotes && soFarVotes + blocks.get(curBlockIndex) >= winVotes)
            return 1;

    } else {

        return  computePowerIndexesHelper(blocks, winVotes, curBlockIndex, soFarVotes + blocks.get(index), index + 1) // 包含的情况
               + computePowerIndexesHelper(blocks, winVotes, curBlockIndex, soFarVotes, index + 1); // 不包含的情况
    }
    return 0;
} // v2 end


// 将提供向量中的元素值相对于所有元素值之和进行百分比转换
Vector<int> percentConversion(const Vector<int>& keyBlocks) {
    Vector<int> result;
    int totalKeyBlock = 0;
    for (int keyBlock : keyBlocks) {
        totalKeyBlock += keyBlock;
    }

    for (int i = 0; i < keyBlocks.size(); i++) {
        result.add(float(keyBlocks.get(i)) / totalKeyBlock * 100);
    }

    return result;
}


/*
 * 此函数计算区块的Banzhaf影响力指数。
 * 接收包括区块票数的向量作为参数，
 * 返回包含各个区块Banzhaf影响力指数的向量。
*/
Vector<int> computePowerIndexes(Vector<int>& blocks){
    // 计算胜选票数
    int winVotes = 0;
    for (int block : blocks) {
        winVotes += block;
    }
    winVotes = (winVotes / 2) + 1;

    Vector<int> keyBlocks; // 关键地区集合

    // 迭代每一个地区调用辅助函数 v2，此版本对相同票数的地区略过
    Map<int, int> alreadyCountedBlocks;
    for (int i = 0; i < blocks.size(); i++) {
        if (!alreadyCountedBlocks.containsKey(blocks.get(i))) {
            int keyBlock = computePowerIndexesHelper(blocks, winVotes, i, 0, 0);  //递归不带深度版本
            //int keyBlock = computePowerIndexesHelper(blocks, winVotes, i, 0, 0, blocks.size()); // //带递归深度的版本
            keyBlocks.add(keyBlock);
            alreadyCountedBlocks.put(blocks.get(i), keyBlock);
        } else {
            keyBlocks.add(alreadyCountedBlocks.get(blocks.get(i)));
        }
    }

    // 迭代每一个地区调用辅助函数 v1
    /*
    for (int i = 0; i < blocks.size(); i++) {
            int keyBlock = computePowerIndexesHelper(blocks, winVotes, i, 0, 0);
            keyBlocks.add(keyBlock);
    }
    */

    //cout << endl << keyBlocks << endl;
    return percentConversion(keyBlocks);
}
#endif

//#define doRecNiu01
#ifdef doRecNiu01

// 挨个寻找关键票数组合
void computePowerIndexesHelper(const Vector<int>& blocks,
                              const int currentBlockIndex,
                              const int halfSumBlocks,
                               int &thisBlockPoint,
                               int sumSubset,
                              int index) { // v2 start
    if(index == currentBlockIndex){
        index++;
    }

    if(sumSubset - blocks.get(currentBlockIndex) > halfSumBlocks){
        ;
    }
    else{
        if(index == blocks.size()){
            if((sumSubset - blocks.get(currentBlockIndex) <= halfSumBlocks) &&
                (sumSubset > halfSumBlocks)){
                thisBlockPoint ++;
            }
        }
        else{
            sumSubset+=blocks[index];
            computePowerIndexesHelper(blocks, currentBlockIndex, halfSumBlocks,
                                      thisBlockPoint, sumSubset, index+1);
            sumSubset-=blocks[index];
            computePowerIndexesHelper(blocks, currentBlockIndex, halfSumBlocks,
                                      thisBlockPoint, sumSubset, index+1);
        }
    }

}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    // TODO your code here

    const int sumBlocks = sumVecInt(blocks);
    const int halfSumBlocks = sumBlocks / 2;

    for(int i = 0; i < blocks.size(); ++i){
        int thisPoint = blocks.get(i);
        result.add(0);
        computePowerIndexesHelper(blocks,i ,halfSumBlocks, result[i], thisPoint, 0);
    }

    cout << "-------------------------------------------------------" << endl;
    int sumAllPoint = sumVecInt(result);

    for(int i = 0; i < result.size();++i){
        result[i] = int(double(result.get(i)) / sumAllPoint * 100);
         cout << result[i] << ", ";
    }
    cout << endl;
    return result;
}
#endif

//#define doRecNiu02
#ifdef doRecNiu02
// 挨个寻找关键票数组合
void computePowerIndexesHelper(const Vector<int>& blocks,
                               const int &currentBlockIndex,
                               const int &halfSumBlocks,
                               int &thisBlockPoint,
                               const int sumSubset,
                                int index) { // v2 start
    if(index == currentBlockIndex){
        index++;
    }


    if(sumSubset > halfSumBlocks){
        ;
    }
    else if(index == blocks.size()){
        if((sumSubset <= halfSumBlocks) &&
            (sumSubset+blocks[currentBlockIndex] > halfSumBlocks)){
            thisBlockPoint ++;
        }
    }
    else{
        int temp = blocks.get(index);
        computePowerIndexesHelper(blocks, currentBlockIndex, halfSumBlocks,
                                  thisBlockPoint, sumSubset+temp, index+1);

        computePowerIndexesHelper(blocks, currentBlockIndex, halfSumBlocks,
                                  thisBlockPoint, sumSubset, index+1);
    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    // TODO your code here

    const int sumBlocks = sumVecInt(blocks);
    const int halfSumBlocks = sumBlocks / 2;



//    for(int i = 0; i < blocks.size(); ++i){
//        int thisPoint = 0;
//        result.add(0);
//        computePowerIndexesHelper(blocks,i ,halfSumBlocks, result[i], thisPoint, 0);
//    }

    Map<int, int> alreadyCountedBlocks;
    for(int i = 0; i < blocks.size(); ++i){

        if (!alreadyCountedBlocks.containsKey(blocks.get(i))) {
            int thisPoint = 0;
            result.add(0);
            computePowerIndexesHelper(blocks,i ,halfSumBlocks, result[i], thisPoint, 0);
            alreadyCountedBlocks.put(blocks.get(i), result[i]);
        }
        else{
            result.add(alreadyCountedBlocks.get(blocks.get(i)));
        }
    }

    cout << "-------------------------------------------------------" << endl;
    int sumAllPoint = sumVecInt(result);

    for(int i = 0; i < result.size();++i){
        result[i] = int(double(result.get(i)) / sumAllPoint * 100);
        cout << result[i] << ", ";
    }
    cout << endl;
    return result;
}
#endif

//#define doRecNiu03
#ifdef doRecNiu03

// 挨个寻找关键票数组合
int computePowerIndexesHelper(const Vector<int>& blocks,
                               const int &currentBlockIndex,
                               const int &halfSumBlocks,
                               const int sumSubset,
                               int index) { // v2 start
    if(index == currentBlockIndex){
        index++;
    }

    if(sumSubset > halfSumBlocks){
        return 0;
    }

    if(index == blocks.size()){
        if((sumSubset <= halfSumBlocks) &&
            (sumSubset+blocks[currentBlockIndex] > halfSumBlocks)){
            return 1;
        }
    }
    else{
        int temp = blocks.get(index);
        return computePowerIndexesHelper(blocks, currentBlockIndex, halfSumBlocks,
                                         sumSubset+temp, index+1)
               + computePowerIndexesHelper(blocks, currentBlockIndex, halfSumBlocks,
                                            sumSubset, index+1);
    }
    return 0;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    // TODO your code here

    const int sumBlocks = sumVecInt(blocks);
    const int halfSumBlocks = sumBlocks / 2;


    Map<int, int> alreadyCountedBlocks;
    for(int i = 0; i < blocks.size(); ++i){

        if (!alreadyCountedBlocks.containsKey(blocks.get(i))) {
            int sumSubset = 0;
            result.add(0);
            result[i] = computePowerIndexesHelper(blocks,i ,halfSumBlocks, sumSubset, 0);
            alreadyCountedBlocks.put(blocks.get(i), result[i]);
        }
        else{
            result.add(alreadyCountedBlocks.get(blocks.get(i)));
        }
    }

    cout << "-------------------------------------------------------" << endl;
    int sumAllPoint = sumVecInt(result);

    for(int i = 0; i < result.size();++i){
        result[i] = int(double(result.get(i)) / sumAllPoint * 100);
        cout << result[i] << ", ";
    }
    cout << endl;
    return result;
}
#endif

//#define doRecNiu04
#ifdef doRecNiu04

// 将提供向量中的元素值求总和 int
int sumOf(const Vector<int>& vec) {
    int total = 0;
    for (int v : vec) {
        total += v;
    }
    return total;
}

// 将提供向量中的元素值求总和 unsigned long long int
unsigned long long int sumOf(const Vector<unsigned long long int>& vec) {
    unsigned long long int total = 0;
    for (unsigned long long int v : vec) {
        total += v;
    }
    return total;
}

// 将提供向量中的元素值相对于所有元素值之和进行百分比转换
Vector<int> percentConversion(const Vector<unsigned long long int>& keyBlocks) {
    Vector<int> result;

    for (int i = 0; i < keyBlocks.size(); i++) {
        int n = (long double)(keyBlocks[i]) / sumOf(keyBlocks) * 100;
        result.add(n);
    }

    return result;
}



// computePowerIndexes的辅助函数
// 找出不包括当前地区的所有组合（子集）并判断该地区是否为该组合的关键票。
// 最终返回当前地区的关键票总数。

unsigned long long int computePowerIndexesHelper(
    const Vector<int>& blocks,
    const int& winVotes,
    const int& targetBlock,
    unsigned long long int soFarVotes,
    unsigned long long int index,
    const int& blocksSize,
    Map<Vector<unsigned long long int>, unsigned long long int>& calculatedSubsets
    ) {
    // 不加当前地区的票数已经胜选，则当前地区肯定不是关键区，该二叉树节点向下所有节点均不是关键区，不需要再继续了
    if (soFarVotes >= winVotes)
        return 0;
    if(calculatedSubsets.containsKey({soFarVotes, index}))
        return calculatedSubsets.get({soFarVotes, index});


    if (index == blocks.size()) {

        // 当组合总票数未超过总票数的一半 且 加上当前地区的票就能超一半，说明当前地区是关键区，返回 1。
        if (soFarVotes < winVotes && soFarVotes + targetBlock >= winVotes)
            return 1;

    } else {

        unsigned long long int n1 = computePowerIndexesHelper(blocks, winVotes, targetBlock, soFarVotes + blocks[index], index + 1, blocksSize, calculatedSubsets); // 包含的情况

        unsigned long long int n2 = computePowerIndexesHelper(blocks, winVotes, targetBlock, soFarVotes, index + 1, blocksSize, calculatedSubsets); // 不包含的情况

        unsigned long long int total = n1 + n2;

        calculatedSubsets.put({soFarVotes, index}, total);

        //if (index == 0)
        //    cout << "Map Size: " << calculatedSubsets.size() << endl;

        return total;
    }
    return 0;
}

/*
 * 此函数计算区块的Banzhaf影响力指数。
 * 接收包括区块票数的向量作为参数，
 * 返回包含各个区块Banzhaf影响力指数的向量。
*/
Vector<int> computePowerIndexes(Vector<int>& blocks){
    // 计算胜选票数
    int winVotes = 0;
    int totalVotesBlocks = sumOf(blocks);
    winVotes = (totalVotesBlocks / 2) + 1;


    Vector<unsigned long long int> keyBlocks;

    Map<unsigned long long int, unsigned long long int> alreadyCountedBlocks;

    int blocksSize = blocks.size();
    for (int i = 0; i < blocksSize; i++) {
        Map<Vector<unsigned long long int>, unsigned long long int> calculatedSubsets;
        int block = blocks[i];
        if (!alreadyCountedBlocks.containsKey(block)) {
            Vector<int> newBlocks = blocks;
            newBlocks.remove(i);
            unsigned long long int keyBlock = computePowerIndexesHelper(newBlocks, winVotes, block, 0, 0, blocksSize - 1, calculatedSubsets);
            keyBlocks.add(keyBlock);
            alreadyCountedBlocks.put(block, keyBlock);
        } else {
            keyBlocks.add(alreadyCountedBlocks.get(block));
        }
    }

    //cout << endl << keyBlocks << endl;
    Vector<int> result = percentConversion(keyBlocks);
    cout << endl << result << endl;
    //return percentConversion(keyBlocks);
    return result;
}

#endif


//#define doRecNiu05
#ifdef doRecNiu05

typedef unsigned long long int theType;

void computePowerIndexesHelper(const Vector<int>& blocks,
                               const theType halfSumBlocks,
                               const theType sumSubset,
                               theType &thisBlockPoint,
                               const int thisBlock,
                               Map<Vector<int>, theType>& calculatedSubsets,
                               int index)
{

//    Vector<int> thisSubset = {thisBlock, sumSubset, index};
    Vector<int> thisSubset = { sumSubset, index};

    // 不加当前地区的票数已经胜选，则当前地区肯定不是关键区，该二叉树节点向下所有节点均不是关键区，不需要再继续了
    if(sumSubset > halfSumBlocks){
        ;
    }
    else if (calculatedSubsets.containsKey(thisSubset)){
        // 如果这个子集之前已经被计算过，直接代入上一次计算的值
        thisBlockPoint += calculatedSubsets.get(thisSubset);
    }
    // 如果递归到头了
    else if (index == blocks.size()) {
        // 判断是否是关键区域的条件
        if((sumSubset <= halfSumBlocks) &&
            (sumSubset+thisBlock > halfSumBlocks)){
            thisBlockPoint++;
        }
    }
    else
    {
        int temp = blocks.get(index);
        theType before = thisBlockPoint;
        computePowerIndexesHelper(blocks, halfSumBlocks, sumSubset+temp,
                                  thisBlockPoint, thisBlock, calculatedSubsets,index+1);

        computePowerIndexesHelper(blocks, halfSumBlocks, sumSubset,
                                  thisBlockPoint, thisBlock, calculatedSubsets,index+1);
        theType rec2 = thisBlockPoint;
        calculatedSubsets.put(thisSubset, rec2-before);

    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    // TODO your code here

    Vector<theType> result00;

    const theType sumBlocks = sumVecInt(blocks);
    const theType halfSumBlocks = sumBlocks / 2;

    Map<int, theType> alreadyCountedBlocks;
    int blocksSize = blocks.size();

    for(int i = 0; i < blocksSize; ++i){
        int thisBlock = blocks.get(i);
        Map<Vector<int>, theType> calculatedSubsets;

        if (!alreadyCountedBlocks.containsKey(thisBlock)) {
            Vector<int> blocksWithoutThis = blocks;
            blocksWithoutThis.remove(i);
            // 给 result 加上一个元素
            result00.add(0);
            // 递归计算结果
            computePowerIndexesHelper(blocksWithoutThis,halfSumBlocks, theType(0),
                                         result00[i], thisBlock, calculatedSubsets, 0);
            // 给map加上这个结果
            alreadyCountedBlocks.put(thisBlock, result00[i]);
        }
        else{
            result00.add(alreadyCountedBlocks.get(thisBlock));
        }
    }

    cout << "-------------------------------------------------------" << endl;
    theType sumAllPoint = sumVecInt(result00);

    for(int i = 0; i < result00.size();++i){
        result.add(0);
        result[i] = int(double(result00.get(i)) / sumAllPoint * 100);
        cout << result00[i] << ", ";
    }
    cout << endl;
    return result;
}
#endif


#define doRecNiu06
#ifdef doRecNiu06

typedef unsigned long long int theType;

void computePowerIndexesHelper(const Vector<int>& blocks,
                               const theType halfSumBlocks,
                               const theType sumSubset,
                               theType &thisBlockPoint,
                               const int thisBlock,
                               Map<Vector<int>, theType>& calculatedSubsets,
                               int index)
{

    //    Vector<int> thisSubset = {thisBlock, sumSubset, index};
    Vector<int> thisSubset = {sumSubset, index};

    // 不加当前地区的票数已经胜选，则当前地区肯定不是关键区，该二叉树节点向下所有节点均不是关键区，不需要再继续了
    if(sumSubset > halfSumBlocks){
        ;
    }
    else if (calculatedSubsets.containsKey(thisSubset)){
        // 如果这个子集之前已经被计算过，直接代入上一次计算的值
        thisBlockPoint += calculatedSubsets.get(thisSubset);
    }
    // 如果递归到头了
    else if (index == blocks.size()) {
        // 判断是否是关键区域的条件
        if((sumSubset <= halfSumBlocks) &&
            (sumSubset+thisBlock > halfSumBlocks)){
            thisBlockPoint++;
        }
    }
    else
    {
        int temp = blocks.get(index);
        theType before = thisBlockPoint;
        computePowerIndexesHelper(blocks, halfSumBlocks, sumSubset+temp,
                                  thisBlockPoint, thisBlock, calculatedSubsets,index+1);

        computePowerIndexesHelper(blocks, halfSumBlocks, sumSubset,
                                  thisBlockPoint, thisBlock, calculatedSubsets,index+1);
        theType rec2 = thisBlockPoint;
        calculatedSubsets.put(thisSubset, rec2-before);

    }
}

void computePowerIndexesRec(const Vector<int>& blocks,
                            Vector<theType>& result00,
                            const theType halfSumBlocks,
                            Map<int, theType> &alreadyCountedBlocks,
                            int index){


    if(index == blocks.size()){

    }
    else{
        int thisBlock = blocks.get(index);
        if(!alreadyCountedBlocks.containsKey(thisBlock))
        {
            Map<Vector<int>, theType> calculatedSubsets;
            Vector<int> blocksWithoutThis = blocks;
            blocksWithoutThis.remove(index);

            computePowerIndexesHelper(blocksWithoutThis,halfSumBlocks, theType(0),
                                      result00[index], thisBlock, calculatedSubsets, 0);

            alreadyCountedBlocks.put(thisBlock, result00[index]);

//            computePowerIndexesRec(blocks,result00,halfSumBlocks, alreadyCountedBlocks,index+1);
        }
        else
        {
            result00[index] = (alreadyCountedBlocks.get(thisBlock));
//            computePowerIndexesRec(blocks,result00,halfSumBlocks, alreadyCountedBlocks,index+1);
        }
        computePowerIndexesRec(blocks,result00,halfSumBlocks, alreadyCountedBlocks,index+1);
    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    // TODO your code here

    Vector<theType> result00(blocks.size(),0);

    const theType sumBlocks = sumVecInt(blocks);
    const theType halfSumBlocks = sumBlocks / 2;

    Map<int, theType> alreadyCountedBlocks;
    int blocksSize = blocks.size();

    computePowerIndexesRec(blocks, result00, halfSumBlocks, alreadyCountedBlocks, 0);

    cout << "-------------------------------------------------------" << endl;
    theType sumAllPoint = sumVecInt(result00);

    for(int i = 0; i < result00.size();++i){
        result.add(0);
        result[i] = int(double(result00.get(i)) / sumAllPoint * 100);
        cout << result00[i] << ", ";
    }
    cout << endl;
    return result;
}
#endif

/* * * * * * Test Cases * * * * * */

STUDENT_TEST("Test power index, blocks 33-29-29-28") {
    Vector<int> blocks = {33, 29,  29,28};
    Vector<int> expected = {50, 16, 16, 16};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}


PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}



PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}
PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}
PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Student --- Test power index, blocks EU post-Nice")
{
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29, 29, 29, 29, 27, 27, 14, 13, 12, 12, 12, 12, 12, 10, 10, 10, 7, 7, 7, 7, 7, 4, 4, 4, 4, 4, 3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    // EXPECT_EQUAL(computePowerIndexesMy02(blocks), expected);
    TIME_OPERATION(27, (computePowerIndexes(blocks)));
}

STUDENT_TEST("Time power index operation, 票数重合度高，重复元素非常多") {
    for (int n = 10; n <= 50; n += 2){
        Vector<int> blocks;
        for (int i = 0; i < n; i++) {
            blocks.add(randomInteger(1, 5));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}

STUDENT_TEST("Time power index operation, 票数重合度适中，重复元素占一半") {
    for (int n = 10; n <= 50; n += 2){
        Vector<int> blocks;
        for (int i = 0; i < n; i++) {
            blocks.add(randomInteger(1, n / 2));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}

STUDENT_TEST("Time power index operation, 票数重合度非常低，重复元素几乎没有") {
    for (int n = 10; n <= 30; n += 2){
        Vector<int> blocks;
        for (int i = 0; i < n; i++) {
            blocks.add(randomInteger(1, 5 * n));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}

