/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
//自己添加
#include "soundex.h"

using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s)
{
    string result = charToString(s[0]);
    for (int i = 1; i < s.length(); i++)
    {
        if (isalpha(s[i]))
        {
            result += s[i];
        }
    }
    if (result.at(0) == ' ')
    {
        result = result.substr(1, string::npos);
    }
    return result;
}

string inputName(string input)
{
    //    cout << "please input : " << endl;
    string line = getLine("please input : ");
    return line;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s)
{
    string inputSurname = s;
    inputSurname = removeNonLetters(s);
    inputSurname = getOriginalDigits(inputSurname);
    inputSurname = removeDuplicationAndAdjacent(inputSurname);
    inputSurname = substituteFirstDigitToUpperLetter(s, inputSurname);
    inputSurname = deleteZeroInDigits(inputSurname);
    /* TODO: Fill in this function. */
    return getLength4(inputSurname);
    // return "";
}
//// TODO: add your test cases here

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath)
{
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath))
    {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
    string mySurname = getLine("Enter a surname (RETURN to quit): ");
    while (mySurname != "")
    {

        string mySoundexCode = soundex(mySurname);
        cout << "Soundex code is " << mySoundexCode << endl;

        Vector<string> result;

        for (int i = 0; i < databaseNames.size(); i++)
        {
            string singleSurname = databaseNames[i];
            //        cout << "surname is " << singleSurname;
            string soundexName = soundex(singleSurname);
            //        cout << ", his/her soundex surname is " << soundexName << endl;
            if (soundexName == mySoundexCode)
            {
                result.add(singleSurname);
            }
        }
        result.sort();
        cout << "Matches from database: " << result << endl;
        cout << endl;
        mySurname = getLine("Enter a surname (RETURN to quit): ");
    }
    cout << "All done!" << endl;
}

// 3. 对每个字母寻找相应编码数字
// enum latterToDigit{'A'=1,'E'=1,'I'=1,'O'=1,'U'=1,'H'=1,'W'=1,'Y'=1};

int getDigit(char inputSingleChar)
{
    string str0 = "AEIOUHWY";
    string str1 = "BFPV";
    string str2 = "CGJKQSXZ";
    string str3 = "DT";
    string str4 = "L";
    string str5 = "MN";
    string str6 = "R";
    string strArray[] = {str0, str1, str2, str3, str4, str5, str6};
    int res = 0;

    for (int i = 0; i < 7; i++)
    {
        if (strArray[i].find(toupper(inputSingleChar)) != strArray[i].npos)
        {
            res = i;
            break;
        }
    }
    return res;
}
// 3. 对每个字母寻找相应编码数字
string getOriginalDigits(string inputSurname)
{

    int sizeInput = inputSurname.size();
    string outSurname = "";
    for (int i = 0; i < sizeInput; i++)
    {
        string a = std::to_string(getDigit(inputSurname[i]));
        outSurname.push_back(a.at(0));
    }
    //    outSurname.at(0) = toupper(inputSurname[0]);
    //    cout << outSurname << endl;
    return outSurname;
}

// 去重复
string removeDuplicationAndAdjacent(string inputSurname)
{
    int sizeInput = inputSurname.size();
    string outSurname = inputSurname.substr(0, 1);
    for (int i = 0; i < sizeInput - 1; i++)
    {
        if (inputSurname[i + 1] != inputSurname[i])
        {
            outSurname.push_back(inputSurname[i + 1]);
        }
        else
        {
        }
    }
    return outSurname;
}

//把第一个数字替换为姓氏大写字母
string substituteFirstDigitToUpperLetter(string inputSurname, string inputDigits)
{
    string outSurname = inputDigits;
    outSurname.at(0) = toupper(inputSurname.at(0));
    return outSurname;
}
// 删除0或者补零
string deleteZeroInDigits(string inputSurname)
{
    int sizeInput = inputSurname.size();
    string outSurname = inputSurname.substr(0, 1);
    for (int i = 0; i < sizeInput - 1; i++)
    {
        if (inputSurname[i + 1] != '0')
        {
            outSurname.push_back(inputSurname[i + 1]);
        }
        else
        {
        }
    }
    return outSurname;
}

string getLength4(string inputSurname)
{
    int sizeInput = inputSurname.size();
    string outResult;
    if (sizeInput >= 4)
    {
        outResult = inputSurname.substr(0, 4);
    }
    else
    {
        outResult = inputSurname += ("0000");
        outResult = outResult.substr(0, 4);
    }
    return outResult;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test removing puntuation, digits, and spaces")
{
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout")
{
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase")
{
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase")
{
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros")
{
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen")
{
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels")
{
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel")
{
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes")
{
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes")
{
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's")
{
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh")
{
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case")
{
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("Test removing puntuation, digits, and spaces")
{
    string s = " zhangsan";
    string result = removeNonLetters(s);
    //    EXPECT_EQUAL(result, "zhangsan");
    s = "Wang, Wu";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "WangWu");
    s = "Zhao-Si ";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "ZhaoSi");
    s = "   Zhang  San,,'Feng";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "ZhangSanFeng");
}

STUDENT_TEST("from letters and numbers")
{

    EXPECT_EQUAL(getDigit('a'), 0);
    EXPECT_EQUAL(getDigit('G'), 2);
    EXPECT_EQUAL(getDigit('t'), 3);
    EXPECT_EQUAL(getDigit('N'), 5);
    EXPECT_EQUAL(getDigit('M'), 5);
    EXPECT_EQUAL(getDigit('m'), 5);
    EXPECT_EQUAL(getDigit('n'), 5);
    EXPECT_EQUAL(getDigit('R'), 6);
    EXPECT_EQUAL(getDigit('P'), 1);
    EXPECT_EQUAL(getDigit('j'), 2);
    EXPECT_EQUAL(getDigit('l'), 4);
}

STUDENT_TEST("from letters and numbers, nEqu")
{

    EXPECT(!(getDigit('a') == 1 + 0));
    EXPECT(!(getDigit('G') == 1 + 2));
    EXPECT(!(getDigit('t') == 1 + 3));
    EXPECT(!(getDigit('N') == 1 + 5));
    EXPECT(!(getDigit('M') == 1 + 5));
    EXPECT(!(getDigit('m') == 1 + 5));
    EXPECT(!(getDigit('n') == 1 + 5));
    EXPECT(!(getDigit('R') == 1 + 6));
    EXPECT(!(getDigit('P') == 1 + 1));
    EXPECT(!(getDigit('j') == 1 + 2));
    EXPECT(!(getDigit('l') == 1 + 4));
}

// STUDENT_TEST("from letters and numbers")
//{

//    //    EXPECT_EQUAL(getOriginalDigits("ag"), "02");
//    //    EXPECT_EQUAL(getOriginalDigits("Curie"), "20600");
//    //    EXPECT_EQUAL(getOriginalDigits("OConner"), "0205506");
//    //    EXPECT_EQUAL(getOriginalDigits("angelou"), "0520400");
//    EXPECT_EQUAL(getOriginalDigits("ag"), "A2");
//    EXPECT_EQUAL(getOriginalDigits("Curie"), "C0600");
//    EXPECT_EQUAL(getOriginalDigits("OConner"), "O205506");
//    EXPECT_EQUAL(getOriginalDigits("angelou"), "A520400");
//}

STUDENT_TEST("remove duplication and adjacent digits")
{
    EXPECT_EQUAL(removeDuplicationAndAdjacent("222323432"), "2323432");
    EXPECT_EQUAL(removeDuplicationAndAdjacent("1232344456"), "12323456");
    EXPECT_EQUAL(removeDuplicationAndAdjacent("32002032"), "3202032");
    EXPECT_EQUAL(removeDuplicationAndAdjacent("66686777111222232344456"), "686712323456");
}
STUDENT_TEST("remove duplication and adjacent digits 002")
{
    EXPECT(removeDuplicationAndAdjacent("222323432") == "2323432");
    EXPECT(removeDuplicationAndAdjacent("1232344456") == "12323456");
    EXPECT(removeDuplicationAndAdjacent("32002032") == "3202032");
    EXPECT(removeDuplicationAndAdjacent("66686777111222232344456") == "686712323456");
}
STUDENT_TEST("remove duplication and adjacent digits 003")
{
    EXPECT(!(removeDuplicationAndAdjacent("222323432") != "2323432"));
    EXPECT(!(removeDuplicationAndAdjacent("1232344456") != "12323456"));
    EXPECT(!(removeDuplicationAndAdjacent("32002032") != "3202032"));
    EXPECT(!(removeDuplicationAndAdjacent("66686777111222232344456") != "686712323456"));
}
STUDENT_TEST("substitute First Digit To Upper Letter")
{
    EXPECT_EQUAL(substituteFirstDigitToUpperLetter("ag", "02"), "A2");
    EXPECT_EQUAL(substituteFirstDigitToUpperLetter("Curie", "20600"), "C0600");
    EXPECT_EQUAL(substituteFirstDigitToUpperLetter("OConner", "0205506"), "O205506");
    EXPECT_EQUAL(substituteFirstDigitToUpperLetter("angelou", "0520400"), "A520400");
}

STUDENT_TEST("delete zero")
{
    EXPECT_EQUAL(deleteZeroInDigits("A2"), "A2");
    EXPECT_EQUAL(deleteZeroInDigits("C0600"), "C6");
    EXPECT_EQUAL(deleteZeroInDigits("O205506"), "O2556");
    EXPECT_EQUAL(deleteZeroInDigits("A520400"), "A524");
}

STUDENT_TEST("get Length value 4")
{
    EXPECT_EQUAL(getLength4("A2"), "A200");
    EXPECT_EQUAL(getLength4("C0600"), "C060");
    EXPECT_EQUAL(getLength4("O205506"), "O205");
    EXPECT_EQUAL(getLength4("A520400"), "A520");
    EXPECT_EQUAL(getLength4(""), "0000");
}
