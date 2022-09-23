/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include <cmath>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n)
{
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++)
    {
        if (n % divisor == 0)
        {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n)
{
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop)
{
    for (long num = 1; num < stop; num++)
    {
        if (isPerfect(num))
        {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0)
            cout << "." << flush; // progress bar
    }
    cout << endl
         << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long smarterSum(long n)
{
    /* TODO: Fill in this function. */

    long total = 0;
    long sqrtN = floor(sqrt(n));

    for (long divisor = 1; divisor <= sqrtN; divisor++)
    {
        if (n % divisor == 0)
        {
            long quotient = n / divisor;
            long sumQuoDiv = ((divisor != quotient && divisor != 1) ? divisor + quotient : divisor);
            total += sumQuoDiv;
        }
    }
    return total;
    //    return 0;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool isPerfectSmarter(long n)
{
    /* TODO: Fill in this function. */
    //    return false;
    return (n != 0) && (n == smarterSum(n));
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void findPerfectsSmarter(long stop)
{
    /* TODO: Fill in this function. */
    for (long num = 1; num < stop; num++)
    {
        if (isPerfectSmarter(num))
        {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0)
            cout << "." << flush; // progress bar
    }
    cout << endl
         << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long findNthPerfectEuclid(long n)
{
    /* TODO: Fill in this function. */
    int count = 0;
    long k = 1;
    long long resultNumber = 0;
    while (count < n)
    {
        long m = pow(2, k) - 1;
        long isPrimeNumber = smarterSum(m);
        //        int count=0;
        if (k > 1 && isPrimeNumber == 1)
        {
            resultNumber = pow(2, k - 1) * (pow(2, k) - 1);
            count++;
            cout << "The " << count << "th result is " << resultNumber << endl;
        }
        k++;
    }
    return resultNumber;
}

// 把返回值换成 long long 长整型，防止溢出
long long findNthPerfectEuclidLongLong(long n)
{
    /* TODO: Fill in this function. */
    int count = 0;
    long k = 1;
    long long resultNumber = 0;
    while (count < n)
    {
        long m = pow(2, k) - 1;
        long isPrimeNumber = smarterSum(m);
        //        int count=0;
        if (k > 1 && isPrimeNumber == 1)
        {
            resultNumber = pow(2, k - 1) * (pow(2, k) - 1);
            count++;
            cout << "The " << count << "th result is " << resultNumber << endl;
        }
        k++;
    }
    return resultNumber;
}

/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs")
{
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect")
{
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect")
{
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect")
{
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect")
{
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes")
{
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// STUDENT_TEST("Time trials of findPerfects (student) on doubling input sizes") {
//     TIME_OPERATION(10000, findPerfects(10000));
//     TIME_OPERATION(20000, findPerfects(20000));
//     TIME_OPERATION(40000, findPerfects(40000));
//     TIME_OPERATION(80000, findPerfects(80000));
//     TIME_OPERATION(160000, findPerfects(160000));
//     TIME_OPERATION(320000, findPerfects(320000));
//     TIME_OPERATION(640000, findPerfects(640000));
//     TIME_OPERATION(1280000, findPerfects(1280000));
// }

// TODO: add your student test cases here

// STUDENT_TEST("negative test, use EXPECT") {
//     EXPECT(!isPerfect(-1000));
//     EXPECT(!isPerfect(-1322320));
//     EXPECT(!isPerfect(-432423132));
// }

// STUDENT_TEST("negative test, use EXPECT_EQUAL false") {
//     EXPECT_EQUAL(isPerfect(-1000),0);
//     EXPECT_EQUAL(isPerfect(-12), 0);
//     EXPECT_EQUAL(isPerfect(-1322320),0);
//     EXPECT_EQUAL(isPerfect(-432423132),0);

//}

// ----------------- question 6
// STUDENT_TEST("test function smarterSum is true") {
//    EXPECT_EQUAL(smarterSum(6),6);
//    EXPECT_EQUAL(smarterSum(28),28);
//    EXPECT_EQUAL(smarterSum(496),496);
//    EXPECT_EQUAL(smarterSum(8128),8128);
//    EXPECT_EQUAL(smarterSum(33550336),33550336);
//}

// STUDENT_TEST("test function smarterSum is false") {
//     EXPECT_EQUAL(smarterSum(25),25);
//     EXPECT_EQUAL(smarterSum(4),4);
//     EXPECT_EQUAL(smarterSum(1000000),1000000);
//     EXPECT_EQUAL(smarterSum(625),625);
// }

// STUDENT_TEST("test function smarterSum, using E") {
//     EXPECT (smarterSum(25)!=25);
//     EXPECT (smarterSum(4)!=4);
//     EXPECT (smarterSum(1000000)!=1000000);
//     EXPECT (smarterSum(625)!=625);
//     EXPECT (smarterSum(1625)!=1625);
//     EXPECT (smarterSum(33550336)==33550336);
// }

// STUDENTSMARTERSUM_TEST("test function smarterSum is true") {
//     EXPECT_EQUAL(smarterSum(6),6);
//     EXPECT_EQUAL(smarterSum(28),28);
//     EXPECT_EQUAL(smarterSum(496),496);
//     EXPECT_EQUAL(smarterSum(8128),8128);
// }

// ----------------- question 7
// STUDENT_TEST("Time trials of findPerfects (student) on doubling input sizes") {
//    TIME_OPERATION(10000, findPerfectsSmarter(10000));
//    TIME_OPERATION(20000, findPerfectsSmarter(20000));
//    TIME_OPERATION(40000, findPerfectsSmarter(40000));
//    TIME_OPERATION(80000, findPerfectsSmarter(80000));
//    TIME_OPERATION(160000, findPerfectsSmarter(160000));
//    TIME_OPERATION(320000, findPerfectsSmarter(320000));
//    TIME_OPERATION(640000, findPerfectsSmarter(640000));
//    TIME_OPERATION(1280000, findPerfectsSmarter(1280000));
//    TIME_OPERATION(2560000, findPerfectsSmarter(2560000));
//    TIME_OPERATION(5120000, findPerfectsSmarter(5120000));
//    TIME_OPERATION(10240000, findPerfectsSmarter(10240000));
//    TIME_OPERATION(20480000, findPerfectsSmarter(20480000));
//    TIME_OPERATION(40960000, findPerfectsSmarter(40960000));
//}

// ----------------- question 9
// findNthPerfectEuclid
STUDENT_TEST("EXPECT_EQUAL, test function findNthPerfectEuclid is true")
{
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28);
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
    EXPECT_EQUAL(findNthPerfectEuclid(6), 8589869056);
    EXPECT_EQUAL(findNthPerfectEuclid(7), 137438691328);
}

// findNthPerfectEuclid
STUDENT_TEST("EXPECT, test function findNthPerfectEuclid is true")
{
    EXPECT(findNthPerfectEuclid(1) == 6);
    EXPECT(findNthPerfectEuclid(2) == 28);
    EXPECT(findNthPerfectEuclid(3) == 496);
    EXPECT(findNthPerfectEuclid(4) == 8128);
    EXPECT(findNthPerfectEuclid(5) == 33550336);
    EXPECT(findNthPerfectEuclid(6) == 8589869056);
    EXPECT(findNthPerfectEuclid(7) == 137438691328);
}

// findNthPerfectEuclid
STUDENT_TEST("EXPECT, test function findNthPerfectEuclid using the incorrect values")
{
    EXPECT(findNthPerfectEuclid(1) != 1 + 6);
    EXPECT(findNthPerfectEuclid(2) != 1 + 28);
    EXPECT(findNthPerfectEuclid(3) != 1 + 496);
    EXPECT(findNthPerfectEuclid(4) != 1 + 8128);
    EXPECT(findNthPerfectEuclid(5) != 1 + 33550336);
    EXPECT(findNthPerfectEuclid(6) != 1 + 8589869056);
    EXPECT(findNthPerfectEuclid(7) != 1 + 137438691328);
}

// findNthPerfectEuclid
STUDENT_TEST("test function findNthPerfectEuclidLongLong is true")
{
    EXPECT(findNthPerfectEuclidLongLong(1) == 6);
    EXPECT(findNthPerfectEuclidLongLong(2) == 28);
    EXPECT(findNthPerfectEuclidLongLong(3) == 496);
    EXPECT(findNthPerfectEuclidLongLong(4) == 8128);
    EXPECT(findNthPerfectEuclidLongLong(5) == 33550336);
    EXPECT(findNthPerfectEuclidLongLong(6) == 8589869056);
    EXPECT(findNthPerfectEuclidLongLong(7) == 137438691328);
}
