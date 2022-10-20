/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream> // for cout, endl
#include <string>   // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"

// ---------------------------
#include <cctype>
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int points(string str)
{
    /* TODO: Implement this function. */
    //    int lengthStr = str.size();
    return (str.size() - 3);
}
STUDENT_TEST("Test point scoring, my test")
{
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

Set<GridLocation> neiborBoard(Grid<char> &board, GridLocation thisLoc)
{
    Set<GridLocation> neighbors;
    /* TODO: Fill in the remainder of this function. */
    int thisRow = thisLoc.row;
    int thisCol = thisLoc.col;

    GridLocation north(thisRow + 0, thisCol + 1);
    GridLocation south(thisRow + 0, thisCol - 1);
    GridLocation east(thisRow + 1, thisCol + 0);
    GridLocation west(thisRow - 1, thisCol + 0);
    GridLocation northEast(thisRow + 1, thisCol + 1);
    GridLocation northWest(thisRow - 1, thisCol + 1);
    GridLocation southEast(thisRow + 1, thisCol - 1);
    GridLocation southWest(thisRow - 1, thisCol - 1);

    if (board.inBounds(north) && isalpha(board[north])) // 是否在board内？是否是字符
    {
        neighbors.add(north);
    }
    if (board.inBounds(south) && isalpha(board[south])) // 是否在board内？是否是字符
    {
        neighbors.add(south);
    }
    if (board.inBounds(east) && isalpha(board[east])) // 是否在board内？是否是字符
    {
        neighbors.add(east);
    }
    if (board.inBounds(west) && isalpha(board[west])) // 是否在board内？是否是字符
    {
        neighbors.add(west);
    }
    if (board.inBounds(northEast) && isalpha(board[northEast])) // 是否在board内？是否是字符
    {
        neighbors.add(northEast);
    }
    if (board.inBounds(northWest) && isalpha(board[northWest])) // 是否在board内？是否是字符
    {
        neighbors.add(northWest);
    }
    if (board.inBounds(southEast) && isalpha(board[southEast])) // 是否在board内？是否是字符
    {
        neighbors.add(southEast);
    }
    if (board.inBounds(southWest) && isalpha(board[southWest])) // 是否在board内？是否是字符
    {
        neighbors.add(southWest);
    }
    return neighbors;
}
STUDENT_TEST("Test neiborBoard")
{
    Grid<char> board = {{'L', 'I', '_', '_'},
                        {'M', 'E', '_', '_'},
                        {'_', 'S', '_', '_'},
                        {'_', '_', '_', '_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};
    GridLocation thisLoc(0, 0);
    Set<GridLocation> neighbors = {{0, 1},
                                   {1, 0},
                                   {1, 1}};
    EXPECT_EQUAL(neiborBoard(board, thisLoc), neighbors);
    thisLoc = {2, 2};
    neighbors = {{1, 1},
                 {2, 1}};
    EXPECT_EQUAL(neiborBoard(board, thisLoc), neighbors);
    thisLoc = {2, 1};
    neighbors = {{1, 0},
                 {1, 1}};
    EXPECT_EQUAL(neiborBoard(board, thisLoc), neighbors);
    board = {{'E', 'Z', 'R', 'R'},
             {'O', 'H', 'I', 'O'},
             {'N', 'J', 'I', 'H'},
             {'Y', 'A', 'H', 'O'}};
    thisLoc = {2, 1};
    neighbors = {{1, 0},
                 {1, 1},
                 {1, 2},
                 {2, 0},
                 {2, 2},
                 {3, 0},
                 {3, 1},
                 {3, 2}};
    EXPECT_EQUAL(neiborBoard(board, thisLoc), neighbors);
}

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
// 为了方便测试函数，把这个函数移到前面
static Lexicon &sharedLexicon()
{
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}
// 判断单词是否包含在字典里面
bool wordInLexicon(const string &thisWord, Lexicon &lex)
{
    return lex.contains(thisWord);
}
STUDENT_TEST("lex contain word")
{
    string word = "word";
    EXPECT_EQUAL(wordInLexicon(word, sharedLexicon()), 1);
    word = "hello";
    EXPECT_EQUAL(wordInLexicon(word, sharedLexicon()), 1);
    word = "words";
    EXPECT_EQUAL(wordInLexicon(word, sharedLexicon()), 1);
    word = "WORDS";
    EXPECT_EQUAL(wordInLexicon(word, sharedLexicon()), 1);
}
void scoreBoardOneRoad(Grid<char> &board,
                       Lexicon &lex,
                       Set<GridLocation> &neighbors,
                       Set<string> &words,
                       int &socres,
                       Set<GridLocation> &passLoc,
                       string &thisWord)
{

    // 如果邻域为空
    if (neighbors.isEmpty())
    {
        if (wordInLexicon(thisWord, lex) && thisWord.size() > 3)
        {
            if (!words.contains(thisWord))
            {
                socres += points(thisWord);
                words.add(thisWord);
            }
        }
    }
    else
    {
        Set<GridLocation> neighborsNext;
        Set<GridLocation> neighborsEmpty = {};
        for (const auto &item : neighbors)
        {
            if (!passLoc.contains(item))
            {
                passLoc.add(item);
                // 挨个检查邻域
                neighborsNext = neiborBoard(board, item);

                if (lex.containsPrefix(thisWord))
                {
                    // 包含这个邻居的情况，继续向下搜索
                    thisWord.push_back(board[item]);
                    scoreBoardOneRoad(board, lex, neighborsNext, words, socres, passLoc, thisWord);
                    // 包含这个邻居且搜索到此为止的情况
                    scoreBoardOneRoad(board, lex, neighborsEmpty, words, socres, passLoc, thisWord);
                    thisWord.pop_back();
                    // 不包含这个邻居且搜索到此为止的情况,
                    // 这句可以不要，因为是set，回去重
                    // scoreBoardOneRoad(board, lex, neighborsEmpty, words, socres, passLoc, thisWord);
                }
                passLoc.remove(item);
            }
        }
    }
}

// STUDENT_TEST("Test scoreBoard, full board, small number of words")
//{
//     // Grid<char> board = {{'R', 'R'},
//     //                     {'I', 'O'}};
//     // Grid<char> board = {{'E', 'Z', 'R', 'R'},
//     //                     {'O', 'H', 'I', 'O'},
//     //                     {'N', 'J', 'I', 'H'},
//     //                     {'Y', 'A', 'H', 'O'}};
//     Grid<char> board = {{'E', 'A', 'A', 'R'},
//                         {'L', 'V', 'T', 'S'},
//                         {'R', 'A', 'A', 'N'},
//                         {'O', 'I', 'S', 'E'}};
//     Set<string> words = {"HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};
//     GridLocation input = {1, 0};
//     Set<GridLocation> neighbors = neiborBoard(board, input);
//     Set<string> words0;
//     Set<GridLocation> passLoc;
//     passLoc.add(input);
//     string word;
//     word.push_back(board[input]);
//     int socres = 0;
//     scoreBoardOneRoad(board, sharedLexicon(), neighbors, words0, socres, passLoc, word);
//     EXPECT_ERROR(scoreBoardOneRoad(board, sharedLexicon(), neighbors, words0, socres,passLoc, word));
// }

int scoreBoardIter(Grid<char> &board, Lexicon &lex)
{
    Set<string> words0;
    int socres = 0;
    for (int i = 0; i < board.size(); ++i)
    {
        // 位置索引
        int row = int(i / board.numRows());
        int col = int(i % board.numRows());
        GridLocation input = {row, col};
        // 设置起点
        Set<GridLocation> neighbors = neiborBoard(board, input);
        // 每一次都从零开始记录经过路径
        Set<GridLocation> passLoc;
        passLoc.add(input);
        // 单词记录

        string word;
        word.push_back(board[input]);
        scoreBoardOneRoad(board, sharedLexicon(), neighbors, words0, socres, passLoc, word);
    }
    return socres;
}

STUDENT_TEST("Test scoreBoardIter, small number of words in corner of board 001")
{
    Grid<char> board = {{'B', 'C', 'D', 'F'}, // no vowels, no words
                        {'G', 'H', 'J', 'K'},
                        {'L', 'M', 'N', 'P'},
                        {'Q', 'R', 'S', 'T'}};
    EXPECT_EQUAL(scoreBoardIter(board, sharedLexicon()), 0);
}
STUDENT_TEST("Test scoreBoardIter, small number of words in corner of board 002")
{
    Grid<char> board = {{'C', '_', '_', '_'},
                        {'Z', '_', '_', '_'},
                        {'_', 'A', '_', '_'},
                        {'_', '_', 'R', '_'}};
    EXPECT_EQUAL(scoreBoardIter(board, sharedLexicon()), 1);
}
STUDENT_TEST("Test scoreBoardIter, small number of words in corner of board 003")
{
    Grid<char> board = {{'E', 'Z', 'R', 'R'},
                        {'O', 'H', 'I', 'O'},
                        {'N', 'J', 'I', 'H'},
                        {'Y', 'A', 'H', 'O'}};
    Set<string> words = {"HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoardIter(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}
STUDENT_TEST("Test scoreBoardIter, small number of words in corner of board 004")
{
    Grid<char> board = {{'O', 'T', 'H', 'X'},
                        {'T', 'H', 'T', 'P'},
                        {'S', 'S', 'F', 'E'},
                        {'N', 'A', 'L', 'T'}};
    EXPECT_EQUAL(scoreBoardIter(board, sharedLexicon()), 76);
}
STUDENT_TEST("Test scoreBoardIter, small number of words in corner of board 005")
{
    Grid<char> board = {{'E', 'A', 'A', 'R'},
                        {'L', 'V', 'T', 'S'},
                        {'R', 'A', 'A', 'N'},
                        {'O', 'I', 'S', 'E'}};
    EXPECT_EQUAL(scoreBoardIter(board, sharedLexicon()), 234);
}
STUDENT_TEST("Test scoreBoardIter, small number of words in corner of board 006")
{
    Grid<char> board = {{'I', 'L', 'T', 'A'},
                        {'F', 'I', 'I', 'S'},
                        {'U', 'H', 'A', 'A'},
                        {'Y', 'W', 'E', 'O'}};
    EXPECT_EQUAL(scoreBoardIter(board, sharedLexicon()), 39);
}
STUDENT_TEST("Test scoreBoardIter, small number of words in corner of board 007")
{
    Grid<char> board = {{'O', 'E', 'W', 'E'},
                        {'I', 'G', 'E', 'N'},
                        {'S', 'N', 'O', 'F'},
                        {'B', 'T', 'G', 'L'}};
    EXPECT_EQUAL(scoreBoardIter(board, sharedLexicon()), 112);
}
STUDENT_TEST("Test scoreBoardIter, small number of words in corner of board 008 5x5")
{
    Grid<char> board = {{'S', 'I', 'O', 'I', 'Z'},
                        {'O', 'I', 'D', 'F', 'N'},
                        {'M', 'S', 'A', 'A', 'E'},
                        {'V', 'A', 'E', 'D', 'T'},
                        {'O', 'R', 'I', 'P', 'M'}};
    EXPECT_EQUAL(scoreBoardIter(board, sharedLexicon()), 351);
}
/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int scoreBoard(Grid<char> &board, Lexicon &lex)
{
    /* TODO: Implement this function. */
    Set<string> words;

    return scoreBoardIter(board, sharedLexicon());
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Load shared Lexicon, confirm number of words")
{
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring")
{
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero")
{
    Grid<char> board = {{'B', 'C', 'D', 'F'}, // no vowels, no words
                        {'G', 'H', 'J', 'K'},
                        {'L', 'M', 'N', 'P'},
                        {'Q', 'R', 'S', 'T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1")
{
    Grid<char> board = {{'C', '_', '_', '_'},
                        {'Z', '_', '_', '_'},
                        {'_', 'A', '_', '_'},
                        {'_', '_', 'R', '_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1")
{
    Grid<char> board = {{'C', 'C', '_', '_'},
                        {'C', 'Z', 'C', '_'},
                        {'_', 'A', '_', '_'},
                        {'R', '_', 'R', '_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board")
{
    Grid<char> board = {{'L', 'I', '_', '_'},
                        {'M', 'E', '_', '_'},
                        {'_', 'S', '_', '_'},
                        {'_', '_', '_', '_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words")
{
    Grid<char> board = {{'E', 'Z', 'R', 'R'},
                        {'O', 'H', 'I', 'O'},
                        {'N', 'J', 'I', 'H'},
                        {'Y', 'A', 'H', 'O'}};
    Set<string> words = {"HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words")
{
    Grid<char> board = {{'O', 'T', 'H', 'X'},
                        {'T', 'H', 'T', 'P'},
                        {'S', 'S', 'F', 'E'},
                        {'N', 'A', 'L', 'T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words")
{
    Grid<char> board = {{'E', 'A', 'A', 'R'},
                        {'L', 'V', 'T', 'S'},
                        {'R', 'A', 'A', 'N'},
                        {'O', 'I', 'S', 'E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
