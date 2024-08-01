#include <iostream>
#include <cstring>
#include <cctype>
#include <exception>
#include <algorithm>
#include<stdexcept>	//for dev c

using namespace std;

void add_symbol(string& arr, char ch);
void FIND_FIRST(string& arr, char ch);
void FIND_FOLLOW(string& arr, char ch);
void FIRST_SHOW();
void FOLLOW_SHOW();
int CREATE_LL1_TABLE();
void PARSING_TABLE_SHOW(int flag);
//void LL1_PARSER(char* STR);

// Global variables
int top = 0;
int t = 0, nt = 0, cr = 0, count = 0;
char FIRST[100][100], FOLLOW[100][100];
char T[100], NT[100], G[100][100], STACK[100];
int LL1[100][100] = {{0}};

int main()
{
    int choice;
    bool continueLoop = true;
    char STR[100];

    cout << "Enter production rules of grammar in the form A->B, epsilon is !\n";

    while (continueLoop)
    {
        cout << "\n1) Insert Production Rules\n2) Show Grammar\n3) Show LL1 Parsing Table\nEnter your choice: ";
top:
        try
        {
            cin >> choice;
            if(cin.fail())
            {
                throw invalid_argument("Invalid input");
            }
        }
        catch (const invalid_argument &e)
        {
            cout << "Invalid entry: " << e.what() << endl;
            cin.clear(); // Clear the error flag on cin
            cin.ignore(); // To handle any newline character left in the buffer
            goto top;
        }
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            cout << "Enter production rule in the form A->B: ";
            cin.getline(G[cr], 100);

            // Find non-terminals
            //if (strchr(NT, G[cr][0]) == nullptr) {	//for code blocks
            if (strchr(NT, G[cr][0]) == NULL)
            {
                NT[nt++] = G[cr][0];
            }

            // Find terminals
            for (int i = 3; G[cr][i] != '\0'; i++)
            {
                if (!isupper(G[cr][i]) && strchr(T, G[cr][i]) == NULL && G[cr][i] != '!')
                {
                    T[t++] = G[cr][i];
                }
            }

            cr++;			//supposedly number of production rules
            break;
        }
        case 2:
        {
            if (cr > 0)
            {
                cout << "\nGrammar";
                cout << "\nStarting symbol is: " << G[0][0];
                cout << "\nNon-terminal symbols: ";
                for (int i = 0; i < nt; i++) cout << NT[i] << "  ";
                cout << "\nTerminal symbols: ";
                for (int i = 0; i < t; i++) cout << T[i] << "  ";
                cout << "\nProduction rules: ";
                for (int i = 0; i < cr; i++) cout << G[i] << "  ";
                cout << "\n";
            }
            else
            {
                cout << "!Enter at least one production rule\n";
            }
            break;
        }
        case 3:
            continueLoop = false;
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    if (cr > 0)
    {
        FIRST_SHOW();
        FOLLOW_SHOW();

        // Add end-of-input symbol to the terminal set
        T[t++] = '$';
        T[t] = '\0';

        int flag = CREATE_LL1_TABLE();
        PARSING_TABLE_SHOW(flag);
//Comment this part
/*
                if (flag == 0) {
                    cout << "Enter string for parsing: ";
                    cin >> STR;
                    LL1_PARSER(STR);
                }
*/
    }

    return 0;
}

void add_symbol(string& result, char ch)
{

	if (result.find(ch) == string::npos)			//npos means  a special constant representing the highest possible value for the size type, which typically means "not found."
        result += ch;
/*    if (strchr(arr, ch) == NULL)
    {
        arr[count++] = ch;
        arr[count] = '\0';
    }
*/
}

void FIND_FIRST(string& arr, char ch)
{
    if (!isupper(ch))
    {
        add_symbol(arr, ch);
    }
    else
    {
        for (int i = 0; i < cr; i++)
        {
            if (ch == G[i][0])
            {
                if (G[i][3] == '!')
                {
                    add_symbol(arr, G[i][3]);
                    FIND_FIRST(arr, G[i][4]);
                }
                else
                {
                    FIND_FIRST(arr, G[i][3]);
                }
            }
        }
    }
}

void FIND_FOLLOW(string& follow, char f)
{
    int i,j,k;
    string subResult="";
    bool foundEpsilon;

    follow="";	//follow
    if (f == G[0][0])
    {
        add_symbol(follow, '$');
    }

    //production set is G
    //
    for (i = 0; i < cr; i++)
    {
        for (j = 3; G[i][j] != '\0'; j++)		//if non-terminal in right hand, continue finding first of the non terminal
        {
            if(G[i][j]==f)
            {
                if (G[i][j + 1] == '\0')
                {
                    if (G[i][0] != f) // Avoid infinite loop
                    {
                        string tempFollow= "";
                        FIND_FOLLOW(tempFollow, G[i][0]);
                        for (char ch : tempFollow)		//for each loop iteratingover tempFOllow array
                        {
                          add_symbol(follow, ch);
                        }
                        //for (int index = 0; tempFollow[index] != '\0'; index++)
                        //{
                         //   add_symbol(follow, tempFollow[index]);
                        //}
                    }
                }
                // If the next symbol is a terminal
                else if (!isupper(G[i][j + 1]))
                {
                    add_symbol(follow, G[i][j + 1]);
                }
                // If the next symbol is a non-terminal
                else if (isupper(G[i][j + 1]))
                {
                    subResult = ""; // Reset subResult for fresh FIRST calculation
                    FIND_FIRST(subResult, G[i][j + 1]);

                    // Add all symbols in FIRST(next) except epsilon
                    for (char ch : subResult)
                    //for(int index=0; subResult[index]!='\0'; index++)
                    {
                        if (ch != '!')
                        {
                            add_symbol(follow, ch);
                        }
                    }

                    // If FIRST(next) contains epsilon, add FOLLOW(current non-terminal) to FOLLOW(f)
                    if (subResult.find('!') != string::npos)
                    //if (strchr(subResult, '!') != NULL)
                    {
                        if (G[i][j + 1] != '\0' && G[i][0] != f) // Avoid self-dependence
                        {
                            //char* tempFollow;
                            string tempFollow= "";

                            FIND_FOLLOW(tempFollow, G[i][0]);
                            //for (int index = 0; tempFollow[index] != '\0'; index++)
                            //{
                              //  add_symbol(follow, tempFollow[index]);
                            //}
                             for (char ch : tempFollow)
                            {
                                 add_symbol(follow, ch);
                             }
                        }
                    }
                }
            }
        }
    }
    /*
    int flag = 1;
    if (ch == G[0][0])
    {
        add_symbol(arr, '$');
    }
    for (int i = 0; i < cr; i++)
    {
        for (int j = 3; G[i][j] != '\0' && flag == 1; j++)
        {
            if (ch == G[i][j])
            {
                flag = 0;
                if (G[i][j + 1] != '\0' && isupper(G[i][j + 1]))
                {
                    for (int k = 0; k < nt; k++)
                    {
                        if (NT[k] == G[i][j + 1])
                        {
                            for (int l = 0; FIRST[k][l] != '\0'; l++)
                            {
                                if (FIRST[k][l] != '!')
                                {
                                    add_symbol(arr, FIRST[k][l]);
                                }
                            }
                            break;
                        }
                    }
                }
                else if (G[i][j + 1] != '\0' && !isupper(G[i][j + 1]))
                {
                    add_symbol(arr, G[i][j + 1]);
                }
                if ((G[i][j + 1] == '\0') && G[i][0] != ch)
                {
                    FIND_FOLLOW(arr, G[i][0]);
                }
            }
        }
    }
    */
}

void FIRST_SHOW()
{
    for (int i = 0; i < nt; i++)
    {
        //char arr[100] = "";
        string arr;
        FIND_FIRST(arr, NT[i]);
        //FIRST[i]=arr;
        strcpy(FIRST[i], arr.c_str());
//        count = 0;
    }

    cout << "\nFIRST:\n\n";
    for (int i = 0; i < nt; i++)
    {
        cout << "FIRST( " << NT[i] << " ): { ";
        for (int j = 0; FIRST[i][j] != '\0'; j++)
        {
            cout << FIRST[i][j];
            if (FIRST[i][j + 1] != '\0') cout << ", ";
        }
        cout << " }\n";
    }
}

void FOLLOW_SHOW()
{
    for (int i = 0; i < nt; i++)
    {
        //char arr[100] = "";
        string arr;
        FIND_FOLLOW(arr, NT[i]);
        strcpy(FOLLOW[i], arr.c_str());
//        count = 0;
    }

    cout << "\nFOLLOW:\n\n";
    for (int i = 0; i < nt; i++)
    {
        cout << "FOLLOW( " << NT[i] << " ): { ";
        for (int j = 0; FOLLOW[i][j] != '\0'; j++)
        {
            cout << FOLLOW[i][j];
            if (FOLLOW[i][j + 1] != '\0') cout << ", ";
        }
        cout << " }\n";
    }
}
/*
int CREATE_LL1_TABLE()
{
    int flag = 0;
    for (int i = 0; i < cr; i++)
    {
        //char arr[100] = "";
        string arr;
        count = 0;
        FIND_FIRST(arr, G[i][3]);
        for (int j = 0; j < count; j++)
        {
            if (arr[j] == '!')
            {
                FIND_FOLLOW(arr, G[i][0]);
                break;
            }
        }

        int pos = -1;
        for (int k = 0; k < nt; k++)
        {
            if (NT[k] == G[i][0])
            {
                pos = k;
                break;
            }
        }

        for (int j = 0; j < count; j++)
        {
            if (arr[j] != '!')
            {
                for (int k = 0; k < t; k++)
                {
                    if (arr[j] == T[k])
                    {
                        if (LL1[pos][k] != 0)
                        {
                            cout << "\n\nConflict occurs between " << G[LL1[pos][k] - 1] << " and " << G[i] << " rules!";
                            cout << "\nGiven grammar is not LL(1) grammar!\n";
                            return 1;
                        }
                        else
                        {
                            LL1[pos][k] = i + 1;
                        }
                        break;
                    }
                }
            }
        }
    }
    return flag;
}
*/
int CREATE_LL1_TABLE()
{
    int flag = 0;

    for (int i = 0; i < cr; i++)  // For each production rule
    {
        string arr;
        //count=0;
        FIND_FIRST(arr, G[i][3]);  // Find FIRST set for the right-hand side of the production

        // Check if epsilon is in FIRST set
        if (arr.find('!') != string::npos)
        {
            // Remove epsilon and find FOLLOW set of the left-hand side
            arr.erase(remove(arr.begin(), arr.end(), '!'), arr.end());
            string follow;
            FIND_FOLLOW(follow, G[i][0]);
            arr += follow;  // Append FOLLOW set to arr
        }

        // Get position of the left-hand side non-terminal in NT
        int pos = -1;
        for (int k = 0; k < nt; k++)
        {
            if (NT[k] == G[i][0])
            {
                pos = k;
                break;
            }
        }

        // For each symbol in the combined set of FIRST and FOLLOW
        for (char ch : arr)
        {
            if (ch != '!')
            {
                // Find the position of the terminal in T
                for (int k = 0; k < t; k++)
                {
                    if (T[k] == ch)
                    {
                        if (LL1[pos][k] != 0)
                        {
                            cout << "\n\nConflict occurs between " << G[LL1[pos][k] - 1] << " and " << G[i] << " rules!";
                            cout << "\nGiven grammar is not LL(1) grammar!\n";
                            return 1;
                        }
                        else
                        {
                            LL1[pos][k] = i + 1;  // Store the production rule number
                        }
                        break;
                    }
                }
            }
        }
    }
    return flag;
}

void PARSING_TABLE_SHOW(int flag)
{
    if (flag == 0)
    {
        cout << "\n\nLL1 Parsing Table:\n\n\t";
        for (int j = 0; j < t; j++)
        {
            cout << "\t" << T[j] << "\t";
        }
        cout << "\n---------------------------------------------------------------------------------------------------------------";
        cout << "\n\n";
        for (int i = 0; i < nt; i++)
        {
            cout << NT[i] << "\t|\t";
            for (int j = 0; j < t; j++)
            {
                if (LL1[i][j] != 0)
                    cout << G[LL1[i][j] - 1] << "\t\t";
                else
                    cout << "_\t\t";
            }
            cout << "\n\n";
        }
    }
}
//and here

void LL1_PARSER(char* STR) {
    strcat(STR, "$");
    STACK[top++] = '$';
    STACK[top++] = G[0][0];

    cout << "\nParsing sequence and actions\n\n";
    cout << "STACK\t\t\tINPUT\t\t\tACTION";
    cout << "\n------------------------------------------------------------------------------------\n";

    int i = 0;
    while (STACK[top - 1] != '$') {
        // Print stack
        for (int j = 0; j < top; j++) {
            cout << STACK[j] << "  ";
        }
        cout << "\t\t";

        // Print input
        for (int j = i; STR[j] != '\0'; j++) {
            cout << STR[j] << "  ";
        }

        if (STR[i] == STACK[top - 1]) {
            cout << "\t\tReduced: " << STACK[top - 1];
            top--;
            i++;
        } else {
            int pos = -1, pos1 = -1;
            for (int j = 0; j < nt; j++) {
                if (STACK[top - 1] == NT[j]) {
                    pos = j;
                    break;
                }
            }
            for (int j = 0; j < t; j++) {
                if (STR[i] == T[j]) {
                    pos1 = j;
                    break;
                }
            }
            int n = LL1[pos][pos1];
            if (n == 0) {
                cout << "\nError in parsing\n";
                return;
            }
            if (G[n - 1][3] == '!') {
                top--;
            } else {
                int len = strlen(G[n - 1]);
                top--; // Pop the non-terminal
                for (int j = len - 1; j >= 3; j--) {
                    STACK[top++] = G[n - 1][j];
                }
            }
            cout << "\t\tShift: " << G[n - 1];
        }
        cout << "\n";
    }

    // Print final stack and input
    for (int j = 0; j < top; j++) cout << STACK[j] << "  ";
    cout << "\t\t";
    for (int j = i; STR[j] != '\0'; j++) cout << STR[j] << "  ";

    cout << "\n";
    if (STACK[top - 1] == '$' && STR[i] == '$') {
        cout << "\nParsing successfully\n";
    }
}


