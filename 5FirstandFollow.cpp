#include <iostream>
#include <cctype>
using namespace std;

void computeFirst(string&, char);
void addToResultSet(string&, char);
void computeFollow(string&, char);
void addToFollowSet(string&, char);

int numOfProductions;
string productionSet[10];

int main()
{
    int i;
    char choice,c,f;
    string result, follow;

    cout << "How many productions? : ";
    cin >> numOfProductions;
    cout<<"Epsilon is $"<<endl;
    cout<<"production rule in the form of A-$"<<endl;
    cout<<"$ is epsilon"<<endl;
/*
productionSet[0]="E-TQ";
productionSet[1]="Q-+TQ";
productionSet[2]="Q-$";
productionSet[3]="T-FW";
productionSet[4]="W-*FW";
productionSet[5]="W-$";
productionSet[6]="F-(E)";
productionSet[7]="F-i";
*/
productionSet[0]="S-aBDh";
productionSet[1]="B-cC";
productionSet[2]="C-bC";
productionSet[3]="C-$";
productionSet[4]="D-EF";
productionSet[5]="E-g";
productionSet[6]="E-$";
productionSet[7]="F-f";
productionSet[8]="F-$";
	/*
    for (i = 0; i < numOfProductions; i++)
    {
        cout << "Enter production number " << i + 1 << ": ";
        cin >> productionSet[i];
    }
    */

    do
    {
        cout << "\nFind the FIRST of: ";
        cin >> c;
        computeFirst(result, c);
        cout << "\nFIRST(" << c << ") = { " << result << " }\n";
        cout << "Press 'y' to continue: ";
        cin >> choice;
    }
    while (choice == 'y' || choice == 'Y');

    do
    {
        cout << "\nFind the Follow of: ";
        cin >> f;
        computeFollow(follow, f);
        cout << "\nFOLLOW(" << f << ") = { " << follow << " }\n";
        cout << "Press 'y' to continue: ";
        cin >> choice;
    }
    while (choice == 'y' || choice == 'Y');

    return 0;
}

void computeFirst(string& result, char c)
{
    int i, j, k;
    string subResult="";
    bool foundEpsilon;

    result = "";

    if (!(isupper(c)))
    {
        addToResultSet(result, c);
        return;
    }

    for (i = 0; i < numOfProductions; i++)
    {
        if (productionSet[i][0] == c)
        {
            if (productionSet[i][2] == '$')
            {
                addToResultSet(result, '$');
            }
            else
            {
                j = 2;
                while (productionSet[i][j] != '\0')		//if non-terminal in right hand, continue finding first of the non terminal
                {
                    foundEpsilon = false;
                    computeFirst(subResult, productionSet[i][j]);
                    for (k = 0; k < subResult.length(); k++)
                        addToResultSet(result, subResult[k]);
                    for (k = 0; k < subResult.length(); k++)
                    {
                        if (subResult[k] == '$')
                        {
                            foundEpsilon = true;
                            break;
                        }
                    }
                    if (!foundEpsilon)
                        break;
                    j++;
                }
            }
        }
    }
}

void computeFollow(string& follow, char f)
{
    int i, j, k;
    string subResult="";
    bool foundEpsilon;

    follow = "";
    if (f == productionSet[0][0])
{
    addToResultSet(follow, '$');
}
    for (i = 0; i < numOfProductions; i++)
    {
        for (j = 2; productionSet[i][j] != '\0'; j++)		//if non-terminal in right hand, continue finding first of the non terminal
        {
            if(productionSet[i][j]==f)
            {
                if (productionSet[i][j + 1] == '\0')
                {
                    if (productionSet[i][0] != f) // Avoid infinite loop
                    {
                        string tempFollow = "";
                        computeFollow(tempFollow, productionSet[i][0]);
                        for (char ch : tempFollow)
                        {
                            addToResultSet(follow, ch);
                        }
                    }
                }
                // If the next symbol is a terminal
                else if (!isupper(productionSet[i][j + 1]))
                {
                    addToResultSet(follow, productionSet[i][j + 1]);
                }
                // If the next symbol is a non-terminal
                else if (isupper(productionSet[i][j + 1]))
                {
                    subResult = ""; // Reset subResult for fresh FIRST calculation
                    computeFirst(subResult, productionSet[i][j + 1]);

                    // Add all symbols in FIRST(next) except epsilon
                    for (char ch : subResult)
                    {
                        if (ch != '$')
                        {
                            addToResultSet(follow, ch);
                        }
                    }

                    // If FIRST(next) contains epsilon, add FOLLOW(current non-terminal) to FOLLOW(f)
                    if (subResult.find('$') != string::npos)
                    {
                        if (productionSet[i][j + 1] != '\0' && productionSet[i][0] != f) // Avoid self-dependence
                        {
                            string tempFollow = "";
                            computeFollow(tempFollow, productionSet[i][0]);
                            for (char ch : tempFollow)
                            {
                                addToResultSet(follow, ch);
                            }
                        }
                    }
                }
            }
        }
    }
}

void addToResultSet(string& result, char val)
{
    if (result.find(val) == string::npos)			//npos means  a special constant representing the highest possible value for the size type, which typically means "not found."
        result += val;
}
