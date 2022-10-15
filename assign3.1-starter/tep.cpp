{
    string operatorsFrom(string str){
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
    EXPECT_EQUAL(operatorsFrom("   423212(asfaf)   {3213fadfa}32131{222}213{}vec[3]"), "(){}{}{}   []");
    EXPECT_EQUAL(operatorsFrom("vec({[3]"), "({[]   ");
    EXPECT_EQUAL(operatorsFrom("int main() { int    x = 2 * (vec[2] + 3); x = (1 + random()); }   "), "(){([])(())}");
}
}

{
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

    {
        int drawSierpinskiTriangle(GWindow & window, GPoint one, GPoint two, GPoint three, int order)
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
    }

    ACKNOWLEDGEMENTS
    REFERENCES

    Grossmann, Siegfried and Lohse, Detlef