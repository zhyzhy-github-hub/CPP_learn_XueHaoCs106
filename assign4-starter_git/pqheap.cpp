#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
PQHeap::PQHeap()
{
    /* TODO: Implement this function. */
    allocatedSize = INITIAL_CAPACITY;
    filledSize = 0;
    elems = new DataPoint[allocatedSize];
}

void PQHeap::swap(const int indexA, const int indexB)
{
    DataPoint tmp = elems[indexA];
    elems[indexA] = elems[indexB];
    elems[indexB] = tmp;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
PQHeap::~PQHeap()
{
    /* TODO: Implement this function. */
    filledSize = 0;
    allocatedSize = 0;
    delete[] elems;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::enqueue(DataPoint elem)
{
    /* TODO: Implement this function. */
    if (filledSize == allocatedSize)
    {
        expand();
    }
    elems[filledSize] = elem;
    ++filledSize;

    int child = filledSize - 1;
    int parent = getParentIndex(child);

    while (elems[child].priority < elems[parent].priority && parent != -1)
    {
        swap(child, parent);
        child = parent;
        parent = getParentIndex(child);
    }
}

// 扩容
void PQHeap::expand()
{
    // do sth
    DataPoint *oldElems = elems;
    allocatedSize *= 2;
    elems = new DataPoint[allocatedSize];
    for (int j = 0; j < filledSize; j++)
    {
        elems[j] = oldElems[j];
    }
    delete[] oldElems;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::peek() const
{
    /* TODO: Implement this function. */
    //    return { "", 0 };
    if (filledSize == 0)
    {
        error("!!! The PQHeap is empty !!!");
    }
    return elems[0];
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::dequeue()
{
    /* TODO: Implement this function. */
    //    return { "", 0 };
    if (filledSize == 0)
    {
        error("!!! The PQHeap is empty !!!");
    }
    auto elem = elems[0];
    --filledSize;

    elems[0] = elems[filledSize];

    int parent = 0;
    int childLeft = getLeftChildIndex(parent);
    int childRight = getRightChildIndex(parent);

    while (childLeft != NONE)
    {
        if (childRight != NONE)
        {
            int exChild = elems[childLeft].priority <= elems[childRight].priority ? childLeft : childRight;
            if (elems[parent].priority > elems[exChild].priority)
            {
                swap(parent, exChild);
                parent = exChild;
                childLeft = getLeftChildIndex(parent);
                childRight = getRightChildIndex(parent);
            }
            else
            {
                childLeft = NONE;
            }
        }
        else
        {
            if (elems[parent].priority > elems[childLeft].priority)
            {
                swap(parent, childLeft);
                parent = childLeft;
                childLeft = getLeftChildIndex(parent);
                childRight = getRightChildIndex(parent);
            }
            else
            {
                childLeft = NONE;
            }
        }
    }

    return elem;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
bool PQHeap::isEmpty() const
{
    /* TODO: Implement this function. */
    return size() == 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
int PQHeap::size() const
{
    /* TODO: Implement this function. */
    return filledSize;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::clear()
{
    /* TODO: Implement this function. */
    delete[] elems;
    elems = new DataPoint[allocatedSize];
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::printDebugInfo(string msg) const
{
    cout << msg << endl;
    /* TODO: Implement this function. */
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const
{
    /* TODO: Implement this function. */
    cout << "{";
    for (int i = 0; i < size(); i++)
    {
        cout << elems[i] << " ";
    }
    cout << "} \n";
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const
{
    /* TODO: Implement this function. */

    int parent = (child - 1) / 2;
    return child == 0 ? NONE : parent;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const
{
    /* TODO: Implement this function. */
    //    return 0;
    int left = 2 * parent + 1;
    if (filledSize >= (left + 1))
    {
        return left;
    }
    else
    {
        return NONE;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const
{
    /* TODO: Implement this function. */
    //    return 0;
    int right = 2 * parent + 2;
    if (filledSize >= (right + 1))
    {
        return right;
    }
    else
    {
        return NONE;
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */

// STUDENT_TEST("test the basic funs"){
//     PQHeap pqHeapTest;
//     EXPECT_EQUAL(pqHeapTest.isEmpty(), 1);
//     EXPECT_EQUAL(pqHeapTest.size(), 0);

//    pqHeapTest.enqueue({"R", 4});
//    pqHeapTest.enqueue({"A", 5});
//    cout << pqHeapTest.peek() << endl;
//    cout << pqHeapTest.dequeue() << endl;
//    cout << pqHeapTest.peek() << endl;
//    cout << pqHeapTest.dequeue() << endl;
//    cout << pqHeapTest.peek() << endl;
//    cout << pqHeapTest.dequeue() << endl;
//}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step")
{
    PQHeap pq;
    Vector<DataPoint> input = {
        {"R", 4}, {"A", 5}, {"B", 3}, {"K", 7}, {"G", 2}, {"V", 9}, {"T", 1}, {"O", 8}, {"S", 6}};

    pq.validateInternalState();
    for (DataPoint dp : input)
    {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty())
    {
        pq.dequeue();
        pq.validateInternalState();
    }
}
