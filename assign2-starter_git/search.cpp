// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"

#include "testing/SimpleTest.h"
using namespace std;

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    string result;
    // remove the no letter at the begin
    int i = 0;
    while (i < s.length())
    {
        // if (!ispunct(s.at(i)) && s.at(i) != ' ' ){
        if (isalpha(s.at(i)))
        {
            result += s.at(i);
            break;
        }
        i++;
    }
    if (result == "")
        return result;
    result.append(s.substr(i + 1));

    // remove the no letter at the end
    i = result.length() - 1;
    while (i >= 0)
    {

        //        if (ispunct(s.at(i)) || s.at(i) == ' ' ){
        if (!isalpha(result.at(i)))
        {
            result.erase(i, i);
            i--;
        }
        else
        {
            break;
        }
    }
    // letter to lower
    for (int i = 0; i < result.length(); i++)
    {
        if (isalpha(result.at(i)))
        {
            char a = result.at(i);
            result.at(i) = tolower(a);
        }
    }
    return result;
}

STUDENT_TEST("the test of cleanToken")
{
    EXPECT_EQUAL(cleanToken("<<section>>"), "section");
    EXPECT_EQUAL(cleanToken("section"), "section");
    EXPECT_EQUAL(cleanToken("secTion"), "section");
    EXPECT_EQUAL(cleanToken(".secTion,"), "section");
    EXPECT_EQUAL(cleanToken("<<seCtion>>"), "section");
    EXPECT_EQUAL(cleanToken("<<section's>>"), "section's");
    EXPECT_EQUAL(cleanToken("<<sECtion's>>"), "section's");
    EXPECT_EQUAL(cleanToken("<<''''sECtion's>>"), "section's");
    EXPECT_EQUAL(cleanToken("<<''''sECtiOn's>>"), "section's");
    EXPECT_EQUAL(cleanToken(" "), "");
    EXPECT_EQUAL(cleanToken("   ,,,,,,     "), "");
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;

    Vector<string> allTextString = stringSplit(text, " ");
    foreach (string strText, allTextString)
    {
        string strClean = cleanToken(strText);
        if (strClean != "")
        {
            tokens.add(strClean);
        }
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>> &index)
{

    // open the file named "dbfile"
    ifstream in;
    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);
    // read all the lines of the file into a Vector named "liens"
    Vector<string> lines;
    readEntireFile(in, lines);
    // get the rows number of the vector
    long numRows = lines.size(); // rows is count of lines
    // so the website number is the half of the numRows
    long numWeb = numRows / 2;
    // iterate through the Vector, acutally, we iterate two element of the Vector each time
    for (int i = 0; i < numWeb; i++)
    {
        // the web's inedx is i*2
        string webThis = lines.get(2 * i);
        // the string set is the element in the index of i*2+1
        string strSetThis = lines.get(2 * i + 1);
        // get a set of a pure string and remove the empty ones
        Set<string> gatherStr = gatherTokens(strSetThis);
        // iterate through the set to construct the inverse search index
        foreach (string strThis, gatherStr)
        {
            // assume the map does not contain the str index
            // then add this couple of (string, web) into the map
            if (index.containsKey(strThis))
            {
                index[strThis].add(webThis);
                // map.get() is a const member, so we can not use it
                // index.get(strThis).add(webThis);
            }
            // else, just add the web to the value of the key of string which has been in the map
            else
            {
                Set<string> strThisValue = {webThis};
                index.put(strThis, strThisValue);
            }
        }
    }
    //    cout << index.values() << endl;

    //    return index.size();
    return numWeb;
    //    return 0;
}

STUDENT_TEST("buildIndex test of Mine")
{
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);

    EXPECT(index.get("fish").contains("www.shoppinglist.com"));
    EXPECT(index.get("fish").contains("www.dr.seuss.net"));
    EXPECT(index.get("fish").contains("www.bigbadwolf.com"));
    EXPECT(index.get("blue").contains("www.rainbow.org"));
    EXPECT(index.get("blue").contains("www.dr.seuss.net"));
}
STUDENT_TEST("buildIndex test of website.txt")
{
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 32);

    EXPECT(index.get("practice").contains("http://cs106b.stanford.edu/class/cs106b/about_assessments"));
    EXPECT(index.get("policy").contains("http://cs106b.stanford.edu/class/cs106b/about_assignments"));
    EXPECT(index.get("video").contains("http://cs106b.stanford.edu/class/cs106b/about_lectures"));
    EXPECT(index.get("better").contains("http://cs106b.stanford.edu/class/cs106b/assignments/2-adt/warmup.html"));
    EXPECT(index.get("point").contains("http://cs106b.stanford.edu/class/cs106b/resources/testing_guide.html"));
    TIME_OPERATION(1, buildIndex("res/website.txt", index));
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>> &index, string query)
{
    Set<string> result;
    // TODO: your code here
    // split the string of query
    Vector<string> queryVector = stringSplit(query, " ");
    // temporary set of the result
    Set<string> querySet = {};
    // the temporary set of the next finding string, we should use this to compute with the querySet
    Set<string> querySetNext = {};
    querySet.unionWith(index.get(cleanToken(queryVector.get(0))));

// temporayt string of the next searching
    string thisStr;

    for (int i = 1; i < queryVector.size(); i++)
    {
        // get the current string with operation sign
        thisStr = queryVector.get(i);
        // clear all the element of the set querySetNext
        querySetNext.clear();
        // if the operation sign is '+'
        if (queryVector.get(i).at(0) == '+')
        {
            thisStr = thisStr.substr(1);
            // the union set of an empty set and the current result
            querySetNext.unionWith(index.get(cleanToken(thisStr)));
            // then, make the intersect
            querySet.intersect(querySetNext);
        }
        // if the operation sign is '-'
        else if (queryVector.get(i).at(0) == '-')
        {
            thisStr = thisStr.substr(1);
            querySetNext.unionWith(index.get(cleanToken(thisStr)));
            querySet.difference(querySetNext);
        }
        else
        {
            //            thisStr = thisStr;
            querySetNext.unionWith(index.get(cleanToken(thisStr)));
            querySet.unionWith(querySetNext);
        }
    }
    // assing the querySet to the result
    result = querySet;
    return result;
}

STUDENT_TEST("findQueryMatches from tiny.txt, compound queries")
{
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish +fish +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish -fish -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    // Build a reverse index
    Map<string, Set<string>> index;
    int webNum = buildIndex(dbfile, index);

    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << webNum << " pages containing " << index.size() << " unique terms" << endl;

    string word;
    word = getLine("Enter query sentence (RETURN/ENTER to quit):");

    while (word != "")
    {
        //    string word = "expect_error +testing";
        Set<string> result = findQueryMatches(index, word);
        cout << "Found " << result.size() << " matching pages" << endl;
        cout << result.toString() << endl;
        word = getLine("Enter query sentence (RETURN/ENTER to quit):");
    }
    cout << "All done!" << endl;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end")
{
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end")
{
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)")
{
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string")
{
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens")
{
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation")
{
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens")
{
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query")
{
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries")
{
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

// TODO: add your test cases here
