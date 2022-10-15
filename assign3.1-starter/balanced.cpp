/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream> // for cout, endl
#include <string>   // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
string operatorsFrom(string str)
{
    /* TODO: Implement this function. */
    string setParenthesis = "{}[]()";

    string result;
    if (str.empty())
    {
        return result += "";
    }
    else
    {
        if (setParenthesis.find(str.at(0)) != std::string::npos)
        {
            result += str.at(0);
        }

        return result += operatorsFrom(str.substr(1));
    }
}

STUDENT_TEST("operatorsFrom on simple example")
{
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
    EXPECT_EQUAL(operatorsFrom("   423212(asfaf){3213fadfa}32131{222}213{}vec[3]"), "(){}{}{}[]");
    EXPECT_EQUAL(operatorsFrom("vec({[3]"), "({[]");
    EXPECT_EQUAL(operatorsFrom("int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }"), "(){([])(())}");
}



/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
//#define OpeMarch01
// 错误，没考虑完全
#ifdef OpeMarch01
bool operatorsAreMatched(string ops)
{
    /* TODO: Implement this function. */
    bool result;
    if (ops.empty())
    {
        return true;
    }
    else
    {
        char firPare = ops.at(0);
        string restPares = ops.substr(1);
        if (firPare == '(')
        {
            if (restPares.find(')') != std::string::npos)
            {
                restPares.erase(restPares.find(')'), 1);
            }
            else
            {
                return false;
            }
        }
        else if (firPare == '[')
        {
            if (restPares.find(']') != std::string::npos)
            {
                restPares.erase(restPares.find(']'), 1);
            }
            else
            {
                return false;
            }
        }
        else if (firPare == '{')
        {
            if (restPares.find('}') != std::string::npos)
            {
                restPares.erase(restPares.find('}'), 1);
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
        return operatorsAreMatched(restPares);
    }
}
#endif

//#define OpeMarch02
// 错误，没考虑完全
#ifdef OpeMarch02
bool operatorsAreMatched(string ops)
{
    /* TODO: Implement this function. */
    bool result;
    if (ops.empty())
    {
        return true;
    }
    else
    {
        int opsSize = ops.size();
        char firstCh = ops.at(0);
        char lastCh = ops.at(opsSize-1);
        if ((firstCh == '(' && lastCh == ')' )
            || (firstCh == '{' && lastCh == '}')
            || (firstCh == '[' && lastCh == ']') )
        {
            return operatorsAreMatched(ops.substr(1,opsSize-2));
        }
        else
        {
            return false;
        }
    }
}
#endif


#define OpeMarch03
#ifdef OpeMarch03

void operatorsAreMatchedRec(string &opsRec, string ops){
    if(ops.empty()){
        ;
    }
    // 如果有满足配对的括号，那么在寻找过程中，
    // 总能找到两个相连的成对括号，
    // 消去相连的一对括号，再寻找
    else{
        opsRec += ops.at(0);
        int opsRecSize = opsRec.size();
        if(opsRecSize>1){
            string opsRecSizeEnd2 = opsRec.substr(opsRecSize-2);
            // 如果满足配对就消去
            if(opsRecSizeEnd2 == "{}"
                || opsRecSizeEnd2 == "[]"
                || opsRecSizeEnd2 == "()")
            {
                // 找到相连的一对就消去
                opsRec = opsRec.substr(0,opsRecSize-2);
            }
        }
        // 再递归寻找 ops 排除第一个元素的子集
        operatorsAreMatchedRec(opsRec, ops.substr(1));
    }
}

bool operatorsAreMatched(string ops)
{
    /* TODO: Implement this function. */
    bool result;
    string opsRec="";
    operatorsAreMatchedRec(opsRec,  ops);

    if (opsRec.empty()){
        result = true;
    }else{
        result = false;
    }
    return result;
}
#endif

STUDENT_TEST("operatorsAreMatched on simple example")
{
    EXPECT(operatorsAreMatched(""));
    EXPECT(operatorsAreMatched("()"));
    EXPECT(operatorsAreMatched("{[()]}"));
    EXPECT(!operatorsAreMatched("([)"));
    EXPECT(!operatorsAreMatched("(212)"));
    EXPECT(!operatorsAreMatched("(])"));
    EXPECT(!operatorsAreMatched("("));
    EXPECT(!operatorsAreMatched("{[()]}{}}{"));
    EXPECT(!operatorsAreMatched("{[()]}]"));
    EXPECT(!operatorsAreMatched("{[()]hello}"));
    EXPECT(!operatorsAreMatched("{}[()]}"));
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str)
{
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example")
{
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example")
{
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup")
{
    string example = "int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup")
{
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
