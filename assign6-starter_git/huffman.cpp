#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */

string decodeTextRec(EncodingTreeNode *tree, Queue<Bit> &messageBits)
{
    string result;
    if (tree == nullptr)
    {
        return "";
    }

    if (tree->isLeaf())
    {
        result.push_back(tree->getChar());
    }
    else
    {
        Bit temp = messageBits.dequeue();
        if (temp == 0)
        {
            result += (decodeTextRec(tree->zero, messageBits));
        }
        else if (temp == 1)
        {
            result += (decodeTextRec(tree->one, messageBits));
        }
    }
    return result;
}

string decodeText(EncodingTreeNode *tree, Queue<Bit> &messageBits)
{
    /* TODO: Implement this function. */
    string result;
    EncodingTreeNode *treeTemp = tree;

    while (!messageBits.isEmpty())
    {
        result += decodeTextRec(tree, messageBits);
    }
    return result;
}

STUDENT_TEST("decodeText, small example encoding tree")
{
    EncodingTreeNode *tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = {1, 1}; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = {1, 0, 1, 1, 1, 0}; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = {1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

STUDENT_TEST("decodeText, some others tests")
{
    EncodingTreeNode *tree = nullptr; // see diagram above
    Queue<Bit> messageBits = {};      //
    EXPECT_EQUAL(decodeText(tree, messageBits), "");
    EXPECT(tree == nullptr);
    deallocateTree(tree);

    tree = createExampleTree();
    messageBits = {1, 1}; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = {1, 0, 1, 1, 1, 0}; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = {1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */

EncodingTreeNode *unflattenTreeRec(Queue<Bit> &treeShape, Queue<char> &treeLeaves)
{
    auto tempShape = treeShape.dequeue();
    if (tempShape == 0)
    {
        auto tempLeaf = treeLeaves.dequeue();
        return new EncodingTreeNode(tempLeaf);
    }
    else
    {
        EncodingTreeNode *temp0 = unflattenTreeRec(treeShape, treeLeaves);
        EncodingTreeNode *temp1 = unflattenTreeRec(treeShape, treeLeaves);
        return new EncodingTreeNode(temp0, temp1);
    }
}

EncodingTreeNode *unflattenTree(Queue<Bit> &treeShape, Queue<char> &treeLeaves)
{
    /* TODO: Implement this function. */

    return unflattenTreeRec(treeShape, treeLeaves);
}

STUDENT_TEST("unflattenTree, small example encoding tree")
{
    EncodingTreeNode *reference = createExampleTree(); // see diagram above
    Queue<Bit> treeShape = {1, 0, 1, 1, 0, 0, 0};
    Queue<char> treeLeaves = {'T', 'R', 'S', 'E'};
    EncodingTreeNode *tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData &data)
{
    /* TODO: Implement this function. */
    EncodingTreeNode *tree = unflattenTree(data.treeShape, data.treeLeaves);
    string result = decodeText(tree, data.messageBits);
    deallocateTree(tree);
    return result;
}

STUDENT_TEST("Student, decompress, small example input")
{
    EncodedData data = {
        {1, 0, 1, 1, 0, 0, 0},               // treeShape
        {'T', 'R', 'S', 'E'},                // treeLeaves
        {0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1} // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */

EncodingTreeNode *buildHuffmanTreeIter(PriorityQueue<EncodingTreeNode *> pQueue )
{
    if (pQueue.isEmpty())
    {
        return nullptr;
    }
    else
    {
        while (pQueue.size()>1)
        {
            int por1 = pQueue.peekPriority();
            EncodingTreeNode *temp1 = pQueue.dequeue();
            int por2 = pQueue.peekPriority();
            EncodingTreeNode *temp2 = pQueue.dequeue();
            pQueue.enqueue(new EncodingTreeNode(temp1, temp2), por1+por2);
        }
    }
    return pQueue.dequeue();
}

EncodingTreeNode *buildHuffmanTree(string text)
{
    /* TODO: Implement this function. */
    Map<char, int> mapRes;
    for (int i = 0; i < text.size(); ++i)
    {
        auto tempChar = text[i];
        if (mapRes.containsKey(tempChar))
        {
            int tempInt = mapRes.get(tempChar) + 1;
            mapRes.remove(tempChar);
            mapRes.put(tempChar, tempInt);
        }
        else
        {
            mapRes.put(tempChar, 1);
        }
    }
    PriorityQueue<EncodingTreeNode *> pQueue;
    Vector<char> keys = mapRes.keys();
    for (auto item : keys)
    {
        pQueue.enqueue(new EncodingTreeNode(item), mapRes.get(item));
    }
    EncodingTreeNode *tree = buildHuffmanTreeIter(pQueue);
    return tree;
}

STUDENT_TEST("buildHuffmanTree, small example encoding tree")
{
    EncodingTreeNode *reference = createExampleTree(); // see diagram above
    EncodingTreeNode *tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);

    reference = new EncodingTreeNode('S');
    tree = buildHuffmanTree("S");
    EXPECT(areEqual(tree, reference));
    deallocateTree(reference);
    deallocateTree(tree);

    reference = nullptr;
    tree = buildHuffmanTree("");
    EXPECT(areEqual(tree, reference));
    deallocateTree(reference);
    deallocateTree(tree);
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */

void encodeText03(EncodingTreeNode *tree,
                  Map<string, Vector<Bit>> &mapRes,
                  Vector<Bit> result)
{
    if (tree->isLeaf())
    {
        string strLeaf;
        strLeaf.push_back(tree->getChar());
        mapRes.put(strLeaf, result);
    }
    else
    {
        result.add(0);
        encodeText03(tree->zero,  mapRes,result);
        result.remove(result.size()-1);

        result.add(1);
        encodeText03(tree->one,  mapRes,result);
        result.remove(result.size()-1);
    }
}


Queue<Bit> encodeText(EncodingTreeNode *tree, string text)
{
    /* TODO: Implement this function. */

    Map<string, Vector<Bit>> mapRes;
    Vector<Bit> result;
    encodeText03(tree,  mapRes, result);
    Queue<Bit> resultQueue;
    cout << "\n";
    while(text.size()!=0){
        string temp;
        temp.push_back(text[0]);
        text=text.substr(1);
        Vector<Bit> tempBits = mapRes.get(temp) ;
        for(int i=0;i<tempBits.size();++i){
            resultQueue.enqueue(tempBits[i]);
        }
    }

    return resultQueue;
}

STUDENT_TEST("Student encodeText, small example encoding tree")
{
    EncodingTreeNode *reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits;

    messageBits = {1, 0, 1}; // S
    EXPECT_EQUAL(encodeText(reference, "S"), messageBits);

    messageBits = {1, 0, 0}; // R
    EXPECT_EQUAL(encodeText(reference, "R"), messageBits);

    messageBits = {0}; // T
    EXPECT_EQUAL(encodeText(reference, "T"), messageBits);

    messageBits = {1, 0, 1, 1, 1, 0}; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = {1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode *tree, Queue<Bit> &treeShape, Queue<char> &treeLeaves)
{
    /* TODO: Implement this function. */
    if (tree->isLeaf())
    {
        treeShape.enqueue(0);
        treeLeaves.enqueue(tree->getChar());
    }else{
        treeShape.enqueue(1);
        flattenTree(tree->zero, treeShape, treeLeaves);
        flattenTree(tree->one, treeShape, treeLeaves);
    }
}

STUDENT_TEST("flattenTree, small example encoding tree")
{
    EncodingTreeNode *reference = createExampleTree(); // see diagram above
    Queue<Bit> expectedShape = {1, 0, 1, 1, 0, 0, 0};
    Queue<char> expectedLeaves = {'T', 'R', 'S', 'E'};
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape, expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    EncodingTreeNode *tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText)
{
    /* TODO: Implement this function. */
   EncodingTreeNode* tree = buildHuffmanTree(messageText);
    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    Queue<Bit>  messageBits;
    flattenTree(tree, treeShape, treeLeaves);

    messageBits=encodeText(tree, messageText);
    deallocateTree(tree);
    return {treeShape, treeLeaves, messageBits};
}

STUDENT_TEST("Student compress, small example input")
{
    EncodedData data = compress("STREETTEST");
    Queue<Bit> treeShape = {1, 0, 1, 1, 0, 0, 0};
    Queue<char> treeChars = {'T', 'R', 'S', 'E'};
    Queue<Bit> messageBits = {1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0};

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}


/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode *createExampleTree()
{
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */

    EncodingTreeNode *nodeR3 = new EncodingTreeNode('R');
    EncodingTreeNode *nodeS3 = new EncodingTreeNode('S');
    EncodingTreeNode *nodeE2 = new EncodingTreeNode('E');
    EncodingTreeNode *nodeT1 = new EncodingTreeNode('T');

    EncodingTreeNode *nodeInRS2 = new EncodingTreeNode(nodeR3, nodeS3);
    EncodingTreeNode *nodeInERS1 = new EncodingTreeNode(nodeInRS2, nodeE2);
    EncodingTreeNode *nodeIn0 = new EncodingTreeNode(nodeT1, nodeInERS1);

    return nodeIn0;
}

void deallocateTree(EncodingTreeNode *t)
{
    /* TODO: Implement this utility function needed for testing. */
    if (t == nullptr)
    {
        return;
    }
    else if (!t->isLeaf())
    {
        deallocateTree(t->zero);
        deallocateTree(t->one);
        delete t;
    }
    else
    {
        delete t;
    }
}

bool areEqual(EncodingTreeNode *a, EncodingTreeNode *b)
{
    /* TODO: Implement this utility function needed for testing. */
    if ((a == nullptr && b != nullptr) || (a != nullptr && b == nullptr))
    {
        return false;
    }
    else if (a == nullptr && b == nullptr)
    {
        ;
    }
    else
    {
        if (a->isLeaf() && b->isLeaf())
        {
            if (a->getChar() == b->getChar())
            {
                ;
            }
            else
            {
                return false;
            }
        }
        else if ((a->isLeaf() && !b->isLeaf()) || (!a->isLeaf() && b->isLeaf()))
        {
            return false;
        }
        else
        {
            return (areEqual(a->zero, b->zero) && areEqual(a->one, b->one));
        }
    }
    return true;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */

STUDENT_TEST("create and deallocate tree")
{
    EncodingTreeNode *tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);
    deallocateTree(tree);
}

STUDENT_TEST("are equal ?1")
{
    EncodingTreeNode *treeA = new EncodingTreeNode('R');
    EncodingTreeNode *treeB = nullptr;

    EXPECT(!areEqual(treeA, treeB));
    EXPECT(areEqual(treeA, treeA));
    EXPECT(areEqual(treeB, treeB));
    deallocateTree(treeA);
    deallocateTree(treeB);
}

STUDENT_TEST("are equal ?2")
{
    EncodingTreeNode *treeA = new EncodingTreeNode('R');
    EncodingTreeNode *treeB = new EncodingTreeNode('R');
    EXPECT(areEqual(treeA, treeB));
    deallocateTree(treeA);
    deallocateTree(treeB);
}

STUDENT_TEST("are equal ?3")
{
    EncodingTreeNode *treeA = createExampleTree();
    EncodingTreeNode *treeB = new EncodingTreeNode('R');
    EXPECT(!areEqual(treeA, treeB));
    deallocateTree(treeA);
    deallocateTree(treeB);

    treeA = createExampleTree();
    treeB = createExampleTree();
    EXPECT(areEqual(treeA, treeB));
    EXPECT(!areEqual(treeA, treeB->one));
    deallocateTree(treeA);
    deallocateTree(treeB);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree")
{
    EncodingTreeNode *tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = {1, 1}; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = {1, 0, 1, 1, 1, 0}; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = {1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree")
{
    EncodingTreeNode *reference = createExampleTree(); // see diagram above
    Queue<Bit> treeShape = {1, 0, 1, 1, 0, 0, 0};
    Queue<char> treeLeaves = {'T', 'R', 'S', 'E'};
    EncodingTreeNode *tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input")
{
    EncodedData data = {
        {1, 0, 1, 1, 0, 0, 0},               // treeShape
        {'T', 'R', 'S', 'E'},                // treeLeaves
        {0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1} // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree")
{
    EncodingTreeNode *reference = createExampleTree(); // see diagram above
    EncodingTreeNode *tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree")
{
    EncodingTreeNode *reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = {1, 1}; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = {1, 0, 1, 1, 1, 0}; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = {1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree")
{
    EncodingTreeNode *reference = createExampleTree(); // see diagram above
    Queue<Bit> expectedShape = {1, 0, 1, 1, 0, 0, 0};
    Queue<char> expectedLeaves = {'T', 'R', 'S', 'E'};

    Queue<Bit> treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape, expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input")
{
    EncodedData data = compress("STREETTEST");
    Queue<Bit> treeShape = {1, 0, 1, 1, 0, 0, 0};
    Queue<char> treeChars = {'T', 'R', 'S', 'E'};
    Queue<Bit> messageBits = {1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0};

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress")
{
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input : inputs)
    {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
