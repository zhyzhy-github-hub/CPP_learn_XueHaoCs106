// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Set<GridLocation> generateValidMoves(Grid<bool> &maze, GridLocation cur)
{
    Set<GridLocation> neighbors;

    /* TODO: Fill in the remainder of this function. */

    if (maze.isEmpty())
    {
        return neighbors;
    }

    GridLocation neighborsEast = {cur.row + 1, cur.col + 0};
    GridLocation neighborsWest = {cur.row - 1, cur.col + 0};
    GridLocation neighborsSorth = {cur.row + 0, cur.col - 1};
    GridLocation neighborsNorth = {cur.row + 0, cur.col + 1};
    if (maze.inBounds(neighborsEast) && maze.get(neighborsEast) == 1)
    {
        neighbors.add(neighborsEast);
    }
    if (maze.inBounds(neighborsWest) && maze.get(neighborsWest) == 1)
    {
        neighbors.add(neighborsWest);
    }
    if (maze.inBounds(neighborsSorth) && maze.get(neighborsSorth) == 1)
    {
        neighbors.add(neighborsSorth);
    }
    if (maze.inBounds(neighborsNorth) && maze.get(neighborsNorth) == 1)
    {
        neighbors.add(neighborsNorth);
    }

    return neighbors;
}
STUDENT_TEST("generateValidMoves on location on the side of 0x0 grid with no walls")
{
    Grid<bool> maze = {};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {};
    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("generateValidMoves on location on the side of 3x3 grid with walls")
{
    Grid<bool> maze = {{false, false, true},
                       {false, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}
STUDENT_TEST("generateValidMoves on location on the side of 3x3 grid with walls")
{
    Grid<bool> maze = {{false, false, true},
                       {false, true, true},
                       {true, true, true}};
    GridLocation side = {5, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool> &maze, Stack<GridLocation> path)
{
    GridLocation mazeExit = {maze.numRows() - 1, maze.numCols() - 1};
    if (path.peek() != mazeExit)
    {
        error("Path does not end at maze exit");
    }

    /* TODO: Fill in the remainder of this function. */

    GridLocation mazeEnter = {0, 0};
    Set<GridLocation> pathRecord = {};
    GridLocation thisLocation = {};
    Set<GridLocation> thisNeighbors = {};

    long lengthPath = path.size();

    for (long i = lengthPath - 1; i > -1; i--)
    {
        // pop this location
        thisLocation = path.pop();
        // get the neighbors
        thisNeighbors = generateValidMoves(maze, thisLocation);
        // assume this location is not in the positions which have been record,
        // and is not in the walls
        if (!pathRecord.contains(thisLocation) && maze.get(thisLocation) == 1)
        {
            // assume this location is not the last one, and the next one is in the neighbours
            if (i != 0 && thisNeighbors.contains(path.peek()))
                pathRecord.add(thisLocation);
            else if (i == 0 && thisLocation.toString() == mazeEnter.toString())
            // else if (i==0 && thisLocation == mazeEnter)
            {
                pathRecord.add(thisLocation);
            }
            else
            {
                error("Here is my message about what has gone wrong");
            }
        }
        else
        {
            error("Here is my message about what has gone wrong");
        }
    }
    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
}

STUDENT_TEST("validatePath on EMPTY path should raise error")
{
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> empty_path = {};

    EXPECT_ERROR(validatePath(maze, empty_path));
}
STUDENT_TEST("validatePath on 5x7 path but 25x33 solution should raise error")
{
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_ERROR(validatePath(maze, soln));
}
STUDENT_TEST("validatePath on correct solution loaded from file for 19x35 path")
{
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/19x35.maze", maze);
    readSolutionFile("res/19x35.soln", soln);
    EXPECT_NO_ERROR(validatePath(maze, soln));
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool> &maze)
{
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    /* TODO: Fill in the remainder of this function. */

    // add a empty queue to contain path
    Queue<Stack<GridLocation>> recordPath;
    // a stack with the only one location, entry
    GridLocation entry={0,0};
    Stack<GridLocation> pathOnlyEntry = {entry};
    // a stack with the only one location, exit.
    // and then the exit is used to judge the break(return result) of the subprogram
    GridLocation exit={maze.numRows()-1,maze.numCols()-1};
    // add the path only with entry to the queue
    recordPath.enqueue(pathOnlyEntry);
    // create a set to contain the locations that have been passed
    Set<GridLocation> locationsHaveBeenPassed;
    locationsHaveBeenPassed.add(entry);

    // set search radius
    long level = 1;

    // assume the recordPath is not empty, then search
    while (!recordPath.isEmpty())
    {
        // get the length of the recordPath queue
        int sizeQueue = recordPath.size();

        // iterate through the queue and add the possible next location to the new path stack
        for (int i = 0; i < sizeQueue; i++)
        {
            // take out the first element of the path queue
            Stack<GridLocation> thisPath = recordPath.dequeue();
            
            // if the last element of the current stack is the exit, then return and baeak the subprogram.
            if ((thisPath.peek()).toString() == exit.toString())
            {
                // assign value to the path that needs to be returneds
                path = thisPath;
                MazeGraphics::highlightPath(path, "green", 200);
                cout << "the length of the result path is " << ++level << endl;
                return path;
            }
            // get the neighbors of the last element of the current path stack
            Set<GridLocation> thisNeighbors = generateValidMoves(maze, thisPath.peek());


            // int sizeNei = thisNeighbors.size();
            // iterate through the neighbors and add each element to the possible path
            foreach (GridLocation nei, thisNeighbors)
            {
                // judge whether the path which has been passed contained this neighbor
                if (!locationsHaveBeenPassed.contains(nei))
                {
                    // if not, add the neighbor to the path that has been passed
                    locationsHaveBeenPassed += (nei);
                    // add this path to the queue of the possible paths for the next search
                    Stack<GridLocation> nextPath{thisPath};
                    nextPath.push(nei);
                    MazeGraphics::highlightPath(nextPath, "blue", 40);
                    recordPath.enqueue(nextPath);
                }
            }
        }
        level += 1;
    }
    // if we have done all over the while loop and do not return a path, then we conclude there is no result path
    cout << "the length of the result path is 0 after " << level << "'s searching." << endl;
    MazeGraphics::highlightPath(path, "red", 200);
    return path;
}

STUDENT_TEST("solveMaze on 2x2 correct solution")
{
    Grid<bool> maze = {{true, false},
                       {true, true}};
    // Stack<GridLocation> soln = {{0, 0}, {1, 0}, {1, 1}};

    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on 19x35 correct solution")
{
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on 25x33 correct solution")
{
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}



/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool> &maze)
{
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();      // rows is count of lines
    int numCols = lines[0].length(); // cols is length of line
    maze.resize(numRows, numCols);   // resize grid dimensions

    for (int r = 0; r < numRows; r++)
    {
        if (lines[r].length() != numCols)
        {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++)
        {
            char ch = lines[r][c];
            if (ch == '@')
            { // wall
                maze[r][c] = false;
            }
            else if (ch == '-')
            { // corridor
                maze[r][c] = true;
            }
            else
            {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation> &soln)
{
    ifstream in;

    if (!openFile(in, filename))
    {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln))
    { // if not successfully read
        error("Maze solution did not have the correct format.");
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls")
{
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls")
{
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls")
{
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution")
{
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = {{0, 0}, {1, 0}, {1, 1}};

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze")
{
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze")
{
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error")
{
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = {{1, 0}, {0, 0}};
    Stack<GridLocation> not_begin_at_entry = {{1, 0}, {1, 1}};
    Stack<GridLocation> go_through_wall = {{0, 0}, {0, 1}, {1, 1}};
    Stack<GridLocation> teleport = {{0, 0}, {1, 1}};
    Stack<GridLocation> revisit = {{0, 0}, {1, 0}, {0, 0}, {1, 0}, {1, 1}};

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7")
{
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23")
{
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
