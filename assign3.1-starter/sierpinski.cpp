/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream> // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow &window, GPoint one, GPoint two, GPoint three)
{
    window.setColor("black");
    window.fillPolygon({one, two, three});
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int drawSierpinskiTriangle(GWindow &window, GPoint one, GPoint two, GPoint three, int order)
{
    /* TODO: Implement this function. */
    int result = 0;
    if (order == 0)
    {
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    else
    {
        GPoint twoHalf((one.x + two.x) / 2, (one.y + two.y) / 2);
        GPoint threeHalf((one.x + three.x) / 2, (one.y + three.y) / 2);
        GPoint oneHalf((three.x + two.x) / 2, (three.y + two.y) / 2);
        result += drawSierpinskiTriangle(window, one, twoHalf, threeHalf, order - 1);
        result += drawSierpinskiTriangle(window, two, oneHalf, twoHalf, order - 1);
        result += drawSierpinskiTriangle(window, three, oneHalf, threeHalf, order - 1);
    }
    return result;
}

/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo")
{
    runInteractiveDemo();
}
