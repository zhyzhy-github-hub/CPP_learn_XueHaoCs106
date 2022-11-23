#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */

#define deAllocate02
#ifdef deAllocate02

void printVec(Vector<int> &vec)
{
    cout << "{";
    for (int i = 0; i < vec.size(); ++i)
    {
        cout << vec[i];
        if (i != vec.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "}" << endl;
}

void printList(ListNode *front)
{
    cout << "{";
    for (ListNode *cur = front; cur != nullptr; cur = cur->next)
    {
        cout << cur->data;
        if (cur->next != nullptr)
        {
            cout << ", ";
        }
    }
    cout << "}" << endl;
}

void deallocateList(ListNode *front)
{
    /* TODO: Implement this function. */
    while (front != nullptr)
    {
        ListNode *temp = front;
        front = front->next;
        delete temp;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode *createList(Vector<int> values)
{
    /* TODO: Implement this function. */
    ListNode *list = nullptr;
    //     for (int i = 0; i<values.size();i++)
    for (int i = values.size() - 1; i >= 0; i--)
    {
        list = new ListNode(values[i], list);
    }
    return list;
    //    return nullptr;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode *front, Vector<int> v)
{
    /* TODO: Implement this function. */
    int i = 0;
    while (front != nullptr)
    {
        //        if (front->data == v[i])
        if (i < v.size())
        {
            if (front->data == v[i])
            {
                front = front->next;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        ++i;
    }
    if (i < v.size())
    {
        return false;
    }
    return true;
}
#endif

STUDENT_TEST("print a list\n")
{
    ListNode *list = nullptr;
    printList(list);
    for (int i = 0; i < 4; i++)
    {
        list = new ListNode(i, list);
    }
    printList(list);
    EXPECT_NO_ERROR(deallocateList(list));
}

STUDENT_TEST("createList a list\n")
{
    Vector<int> vec;
    for (int i = 0; i < 4; i++)
    {
        vec.add(i + 1);
    }
    ListNode *list = createList(vec);
    printVec(vec);
    printList(list);
    EXPECT_NO_ERROR(deallocateList(list));
}

STUDENT_TEST("areEquivalent ---------- \n")
{
    Vector<int> vec;
    int i;
    for (i = 0; i < 4; i++)
    {
        vec.add(i + 1);
    }
    ListNode *list = createList(vec);

    // -------------------------------------
    printVec(vec);
    printList(list);
    EXPECT(areEquivalent(list, vec));
    // -------------------------------------

    Vector<int> vec01 = vec;
    vec01[2] = 6;
    printVec(vec01);
    printList(list);
    EXPECT(!areEquivalent(list, vec01));
    // -------------------------------------

    Vector<int> vec02;
    for (i = 0; i < 3; i++)
    {
        vec02.add(i + 1);
    }
    printVec(vec02);
    printList(list);
    EXPECT(!areEquivalent(list, vec02));
    // -------------------------------------
    ListNode *list0 = list->next;
    Vector<int> vec03 = vec.subList(1);
    printVec(vec03);
    printList(list0);
    EXPECT(areEquivalent(list0, vec03));
    EXPECT_NO_ERROR(deallocateList(list));
}

// #define partition03
#ifdef partition03
void addListToTail(ListNode *&list, ListNode *&listTail, ListNode *&temp)
{
    if (list == nullptr)
    {
        list = temp;
        listTail = list;
    }
    else
    {
        listTail->next = temp;
        listTail = listTail->next;
    }
}

void partition(ListNode *&list, ListNode *&small, ListNode *&middle, ListNode *&big)
{
    int key = list->data;

    ListNode *end = nullptr;

    small = nullptr;
    middle = nullptr;
    big = nullptr;
    ListNode *smallPtr = small;
    ListNode *middlePtr = middle;
    ListNode *bigPtr = big;

    for (ListNode *cur = list; cur != nullptr; cur = cur->next)
    {
        ListNode *temp = list;
        list = list->next;

        if (cur->data < key)
        {
            addListToTail(small, smallPtr, temp);
        }
        else if (cur->data == key)
        {
            addListToTail(middle, middlePtr, temp);
        }
        else
        {
            addListToTail(big, bigPtr, temp);
        }
    }
    if (bigPtr != nullptr)
    {
        bigPtr->next = nullptr;
    }
    if (middlePtr != nullptr)
    {
        middlePtr->next = nullptr;
    }
    if (smallPtr != nullptr)
    {
        smallPtr->next = nullptr;
    }
}
#endif

#define partition04
#ifdef partition04
void addListToHead(ListNode *&list, ListNode *&head)
{
    if (list == nullptr)
    {
        if (head != nullptr)
        {
            ListNode *tempHead = head;
            head = head->next;
            list = tempHead;
            list->next = nullptr;
        }
        else
        {
            list = head;
            head = nullptr;
        }
    }
    else
    {
        if (head != nullptr)
        {
            ListNode *temp = list;
            ListNode *headTemp = head;
            head = head->next;
            list = headTemp;
            list->next = temp;
        }
        else
        {
            return;
        }
        // list = head;
    }
}

void partition(ListNode *&list, ListNode *&small, ListNode *&middle, ListNode *&big)
{
    int key = list->data;

    ListNode *end = nullptr;

    small = nullptr;
    middle = nullptr;
    big = nullptr;
    // ListNode *smallPtr = small;
    // ListNode *middlePtr = middle;
    // ListNode *bigPtr = big;

//    for (ListNode *cur = list; cur != nullptr; cur = cur->next)
    while(list!=nullptr)
    {
        ListNode *temp = list;
        list = list->next;

        if (temp->data < key)
        {
            //            ListNode* tempPtr = small;
            //            small = temp;
            //            small->next = tempPtr;
            addListToHead(small,  temp);
        }
        else if (temp->data == key)
        {
            //            ListNode* tempPtr = middle;
            //            middle = temp;
            //            middle->next = tempPtr;
            addListToHead(middle, temp);
        }
        else
        {
            //            ListNode* tempPtr = big;
            //            big = temp;
            //            big->next = tempPtr;
            addListToHead(big, temp);
        }
    }
}



STUDENT_TEST("addListToHead")
{

    Vector<int> values = {2, 2, 0, 1, 3, 2, 4};
    printVec(values);
    // ---------------------------------------
    cout << "---------normal cases \n";
    ListNode *list = createList(values);
    printVec(values);
    printList(list);
    Vector<int> add = {52, 32, 22};
    ListNode *addList = createList(add);
    addListToHead(list, addList);
    printList(list);
    printList(addList);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(addList));

    cout << "values is empty\n";
    list = nullptr;
    add = {52, 32, 22};
    addList = createList(add);
    printList(list);
    printList(addList);
    addListToHead(list, addList);
    printList(list);
    printList(addList);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(addList));

    cout << "add is empty and values is not empty\n";
    values = {21, 2, 43, 54, 22, 555, 1};
    list = createList(values);
    add = {};
    addList = createList(add);
    printList(list);
    printList(addList);
    addListToHead(list, addList);
    printList(list);
    printList(addList);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(addList));

    cout << "add is empty and values is also empty\n";
    values = {};
    list = createList(values);
    add = {};
    addList = createList(add);
    printList(list);
    printList(addList);
    addListToHead(list, addList);
    printList(list);
    printList(addList);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(addList));
}
#endif

STUDENT_TEST("vec & list partition.")
{

    Vector<int> values = {2, 2, 0, 1, 3, 2, 4};
    // Vector<int> values = {3, 2, 1, 0, 5, 6, 8, 7};
    printVec(values);
    // ---------------------------------------
    ListNode *list = createList(values);
    printVec(values);
    printList(list);

    ListNode *small = nullptr;
    ListNode *middle = nullptr;
    ListNode *big = nullptr;

    partition(list, small, middle, big);
    cout << "---------------\n";
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));

    values = {4, 123, 32, 33, 4, 664, 213, 0, 11, 12, 3, -4, 95, 6, 7};
    list = createList(values);
    partition(list, small, middle, big);
    cout << "---------------\n";
    printList(list);
    printList(small);
    printList(middle);
    printList(big);

    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));
}

STUDENT_TEST("vec & list partition.")
{
    int n = 30;
    Vector<int> values(n);
    printVec(values);

    for (int i = n - 1; i >= 0; i--)
    {
        values[i] = randomInteger(-10000, 10000);
    }
    printVec(values);
    // ---------------------------------------
    ListNode *list = createList(values);
    printVec(values);
    printList(list);

    ListNode *small = nullptr;
    ListNode *middle = nullptr;
    ListNode *big = nullptr;

    partition(list, small, middle, big);
    cout << "---------------\n";
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));
}

void concatenate(ListNode *&list, ListNode *&small, ListNode *&middle, ListNode *&big)
{
    ListNode *tempStr = small;
    if (small == nullptr)
    {
        tempStr = middle;
        list = middle;
        if (middle == nullptr)
        {
            tempStr = big;
            list = big;
        }
        else
        {
            for (ListNode *cur = middle; cur != nullptr; cur = cur->next)
            {
                tempStr = cur;
            }
            tempStr->next = big;
        }
    }
    else
    {
        list = small;
        for (ListNode *cur = small; cur != nullptr; cur = cur->next)
        {
            tempStr = cur;
        }
        tempStr->next = middle;
        if (middle == nullptr)
        {
            tempStr->next = big;
        }
        else
        {
            for (ListNode *cur = middle; cur != nullptr; cur = cur->next)
            {
                tempStr = cur;
            }
            tempStr->next = big;
        }
    }
    small = nullptr;
    middle = nullptr;
    big = nullptr;
}

STUDENT_TEST("concatenate\n")
{
    cout << " all three is not empty " << endl;
    Vector<int> smallVec = {2, 2};
    Vector<int> middleVec = {3, 3, 3};
    Vector<int> bigVec = {4, 4};
    ListNode *small = createList(smallVec);
    ListNode *middle = createList(middleVec);
    ListNode *big = createList(bigVec);
    ListNode *list = nullptr;
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    concatenate(list, small, middle, big);
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));

    // ---------------------------------------
    cout << "only small is empty " << endl;
    smallVec = {};
    middleVec = {3, 3, 3};
    bigVec = {4, 4};
    small = createList(smallVec);
    middle = createList(middleVec);
    big = createList(bigVec);
    list = nullptr;
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    concatenate(list, small, middle, big);
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));

    // ---------------------------------------
    cout << "only middle is empty " << endl;
    smallVec = {3, 3, 3};
    middleVec = {};
    bigVec = {4, 4};
    small = createList(smallVec);
    middle = createList(middleVec);
    big = createList(bigVec);
    list = nullptr;
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    concatenate(list, small, middle, big);
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));

    // ---------------------------------------
    cout << "small and middle are empty " << endl;
    smallVec = {};
    middleVec = {};
    bigVec = {4, 4};
    small = createList(smallVec);
    middle = createList(middleVec);
    big = createList(bigVec);
    list = nullptr;
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    concatenate(list, small, middle, big);
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));

    // ---------------------------------------
    cout << "big and middle are empty " << endl;
    smallVec = {222,54,2221};
    middleVec = {};
    bigVec = {};
    small = createList(smallVec);
    middle = createList(middleVec);
    big = createList(bigVec);
    list = nullptr;
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    concatenate(list, small, middle, big);
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));

    // ---------------------------------------
    cout << "small and big are empty " << endl;
    smallVec = {};
    middleVec = {2,2,2};
    bigVec = {};
    small = createList(smallVec);
    middle = createList(middleVec);
    big = createList(bigVec);
    list = nullptr;
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    concatenate(list, small, middle, big);
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));

    // ---------------------------------------
    cout << "all are empty " << endl;
    smallVec = {};
    middleVec = {};
    bigVec = {};
    small = createList(smallVec);
    middle = createList(middleVec);
    big = createList(bigVec);
    list = nullptr;
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    concatenate(list, small, middle, big);
    printList(list);
    printList(small);
    printList(middle);
    printList(big);
    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));
}

STUDENT_TEST("vec & list partition01.")
{

    Vector<int> values = {2, 2, 0, 1, 4, 2, 3};
    printVec(values);
    // ---------------------------------------
    ListNode *list = createList(values);
    printVec(values);
    printList(list);

    ListNode *small = nullptr;
    ListNode *middle = nullptr;
    ListNode *big = nullptr;

    partition(list, small, middle, big);

    cout << "---------------\n";
    printList(list);
    printList(small);
    printList(middle);
    printList(big);

    concatenate(list, small, middle, big);
    printList(list);
    printList(small);
    printList(middle);
    printList(big);

    EXPECT_NO_ERROR(deallocateList(list));
    EXPECT_NO_ERROR(deallocateList(small));
    EXPECT_NO_ERROR(deallocateList(middle));
    EXPECT_NO_ERROR(deallocateList(big));
}

STUDENT_TEST("vec & list partition02.")
{

    Vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8};
    printVec(values);
    // ---------------------------------------
    ListNode *list = createList(values);
    printVec(values);
    printList(list);

    ListNode *small = nullptr;
    ListNode *middle = nullptr;
    ListNode *big = nullptr;

    partition(list, small, middle, big);
    cout << "---------------\n";
    printList(list);
    printList(small);
    printList(middle);
    printList(big);

    concatenate(list, small, middle, big);
    printList(list);
    printList(small);
    printList(middle);
    printList(big);

    EXPECT_NO_ERROR(deallocateList(list));
}
STUDENT_TEST("vec & list partition03.")
{

    Vector<int> values = {8, 7, 6, 5, 4, 3, 2, 1, 0};
    printVec(values);
    // ---------------------------------------
    ListNode *list = createList(values);
    printVec(values);
    printList(list);

    ListNode *small = nullptr;
    ListNode *middle = nullptr;
    ListNode *big = nullptr;

    partition(list, small, middle, big);
    cout << "---------------\n";
    printList(list);
    printList(small);
    printList(middle);
    printList(big);

    concatenate(list, small, middle, big);
    printList(list);
    printList(small);
    printList(middle);
    printList(big);

    EXPECT_NO_ERROR(deallocateList(list));
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void quickSort01(ListNode *&front)
{
    /* TODO: Implement this function. */
    if (front == nullptr)
    {
        return;
    }
    else
    {
        ListNode *small = nullptr;
        ListNode *middle = nullptr;
        ListNode *big = nullptr;
        partition(front, small, middle, big);
        quickSort01(small);
        quickSort01(big);
        concatenate(front, small, middle, big);
    }
}

void quickSort(ListNode *&front)
{
    /* TODO: Implement this function. */
    quickSort01(front);
}

STUDENT_TEST("vec & list quickSort.")
{

    Vector<int> values = {2, 2, 0, 1, 4, 2, 3};
    // Vector<int> values = {3, 2, 1, 0, 5, 6, 8, 7};
    printVec(values);
    // ---------------------------------------
    ListNode *list = createList(values);
    printVec(values);
    printList(list);

    quickSort01(list);
    cout << "---------------\n";
    printList(list);
    EXPECT_NO_ERROR(deallocateList(list));
}

// ----------------------------------------------
STUDENT_TEST("vec quick sort.")
{
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 5, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode *list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement.")
{
    /* Creates a small test list containing the values 1->2->3. */
    ListNode *testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode *studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode *cur = studentList;
    for (int i = 1; i <= 4; i++)
    {
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

STUDENT_TEST("Time test NlogN")
{
    int startSize = 1000;
    int k = 20000;
    for (int i = 1; i < k; i *= 2)
    {
        int listSize = startSize * i;

        Vector<int> v(listSize);
        ListNode *list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = listSize - 1; i >= 0; i--)
        {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(listSize, quickSort(list));
        //        v.sort();
        //        EXPECT(areEquivalent(list, v));

        deallocateList(list);
    }
}

STUDENT_TEST("Time test N^2")
{
    int startSize = 1000;
    int k = 200;
    for (int i = 1; i < k; i *= 2)
    {
        int listSize = startSize * i;

        Vector<int> v(listSize);
        ListNode *list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = listSize - 1; i >= 0; i--)
        {
            //            v[i] = randomInteger(-10000, 10000);
            v[i] = i;
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(listSize, quickSort(list));
        //        v.sort();
        //        EXPECT(areEquivalent(list, v));

        deallocateList(list);
    }
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort")
{
    int startSize = 50000;

    for (int n = startSize; n < 10 * startSize; n *= 2)
    {
        Vector<int> v(n);
        ListNode *list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n - 1; i >= 0; i--)
        {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort()); /* Standard vector sort operation is backed
                                        with quicksort algorithm. */

        EXPECT(areEquivalent(list, v));

        deallocateList(list);
    }
}
