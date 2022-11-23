#include "labyrinth.h"
#include "demo/MazeGenerator.h"
#include "testing/SimpleTest.h"
using namespace std;

/* Change kYourName constant to your full name.
 *
 * WARNING: Once you've set set this constant and started exploring your labyrinth,
 * do NOT edit the value of kYourName. Changing kYourName will change which
 * maze you get back, which might invalidate all your hard work!
 */
// const string kYourName = "TODO";
const string kYourName = "ZY";

/* Change this constant to the path out of your labyrinth. */
// const string kPathOutOfNormalMaze = "WSSSENNSESEWNNENW";
const string kPathOutOfNormalMaze = "WSSSENESEWNNENWESWSWN";
// const string kPathOutOfNormalMaze = "WSSSENENENWESWSSEWNWN";
// const string kPathOutOfNormalMaze = "TODO";

/**
 * Given a location in a maze, returns whether the given sequence of
 * steps will let you escape the maze.
 *
 * To escape the maze, you need to collect all items in the `needs` set.
 * You can only take steps in the four cardinal directions,
 * and you can't move in directions that don't exist in the maze.
 */

#define OldTest
#ifdef OldTest
bool isPathToFreedom(MazeCell *start, string moves, Set<string> needs)
{
    /* TODO: Your code here */
    const int movesSize = moves.size();
    MazeCell *tempLocation = start;
    int num = 0;

    if (tempLocation == nullptr)
    {
        //        error("!!!!! The start is nullptr\n");
    }
    else
    {
        string thisStr = tempLocation->contents;
        if (needs.contains(thisStr))
        {
            needs.remove(thisStr);
        }
    }
    // if (needs.isEmpty())
    // {
    //     return true;
    // }

    for (int i = 0; i < movesSize; ++i)
    {
        char thisChar = (moves.at(i));
//        char thisChar = toupper(moves.at(i));
        if (thisChar != 'S' && thisChar != 'N' && thisChar != 'E' && thisChar != 'W')
        {
            error("!!!!! Illegal characters\n");
        }

        if (thisChar == 'S')
        {
            tempLocation = tempLocation->south;
        }
        else if (thisChar == 'W')
        {
            tempLocation = tempLocation->west;
        }
        else if (thisChar == 'N')
        {
            tempLocation = tempLocation->north;
        }
        else if (thisChar == 'E')
        {
            tempLocation = tempLocation->east;
        }

        if (i!= movesSize-1 && tempLocation == nullptr)
        {
        //    error("!!!!! The location is nullptr\n");
            return false;
        }

        string thisStr = tempLocation->contents;
        if (needs.contains(thisStr))
        {
            needs.remove(thisStr);
        }
        // if (needs.isEmpty())
        // {
        //     return true;
        // }
    }
    if (needs.isEmpty())
    {
        return true;
    }
    return false;
}

STUDENT_TEST("Works when start on an item")
{
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

STUDENT_TEST("Reports errors if given illegal characters")
{
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Q", {"Wand"}));
    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Ee", {"Wand"}));
//    EXPECT(isPathToFreedom(maze[0][0], "ESWNEES", {"Wand"}));
}

#endif

/* * * * * * Test Cases Below This Point * * * * * */

PROVIDED_TEST("Check paths in the sample from writeup")
{
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES", allThree));

    /* This path works to pick up just potion */
    EXPECT(isPathToFreedom(maze[2][2], "ESNW", {"Potion"}));

    /* This path doesn't work, since doesn't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW", allThree));

    /* This path doesn't work, cannot walk through walls. */
    EXPECT(!isPathToFreedom(maze[2][2], "NNWWSSSEEE", allThree));
}

PROVIDED_TEST("Works when start on an item")
{
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

PROVIDED_TEST("Reports errors if given illegal characters")
{
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Q", {"Wand"}));
    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Ee", {"Wand"}));
}

PROVIDED_TEST("Escape from your personal labyrinth!")
{
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    /* A maze for you to escape from. This maze will be personalized
     * based on the constant kYourName.
     */
    EXPECT(kYourName != "TODO");
    MazeCell *startLocation = mazeFor(kYourName);

    /* Set a breakpoint here. As a reminder, the labyrinth you
     * get will be personalized to you, so don't start exploring the labyrinth
     * unless you've edited the constant kYourName to include your name(s)!
     * Otherwise, you'll be escaping the wrong labyrinth.
     */
    EXPECT(kPathOutOfNormalMaze != "TODO");
    EXPECT(isPathToFreedom(startLocation, kPathOutOfNormalMaze, allThree));
}
