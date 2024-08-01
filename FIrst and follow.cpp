#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

void add_symbol(char* arr, char ch);
void FIND_FIRST(char* arr, char ch);
void FIND_FOLLOW(char* arr, char ch);
void FIRST_SHOW();
void FOLLOW_SHOW();

// Global variables
int top = 0;
int t = 0, nt = 0, cr = 0;
char FIRST[100][100], FOLLOW[100][100];
char T[100], NT[100], G[100][100];

int nt_index(char ch)
{
    for (int i = 0; i < nt; i++)
    {
        if (NT[i] == ch) return i;
    }
    return -1;
}

int main()
{
    cout << "Enter production rules of grammar in the form A->B, epsilon is !\n";
    cout << "Number of production rules: ";
    cin >> cr;
    cin.ignore(); // to consume the newline character left in the buffer
  /* strcpy(G[0],"S->[C]S");
   strcpy(G[1],"S->!");
   strcpy(G[2],"C->{A}C");
   strcpy(G[3],"C->!");
   strcpy(G[4],"A->!Q");
   strcpy(G[5],"Q->()Q");
   strcpy(G[6],"Q->!");
    */
    strcpy(G[0],"E->TQ");
   strcpy(G[1],"Q->+TQ");
   strcpy(G[2],"Q->!");
   strcpy(G[3],"T->FW");
   strcpy(G[4],"W->*FW");
   strcpy(G[5],"W->!");
   strcpy(G[6],"F->(E)");
strcpy(G[7],"F->i");

    for(int i = 0; i < cr; i++)
    {
        //cout << "Enter production rule " << i + 1 << ": ";
        //cin.getline(G[i], 100);
        // Find non-terminals
        if (strchr(NT, G[i][0]) == NULL)
        {
            NT[nt++] = G[i][0];
        }

        // Find terminals
        for (int j = 3; G[i][j] != '\0'; j++)
        {
            if (!isupper(G[i][j]) && strchr(T, G[i][j]) == NULL && G[i][j] != '!')
            {
                T[t] = G[i][j];
                t++;
            }
        }
    }
    FIRST_SHOW();
    FOLLOW_SHOW();
    return 0;
}

void add_symbol(char* arr, char ch)
{
    if (strchr(arr, ch) == NULL)
    {
        int len = strlen(arr);
        arr[len] = ch;
        arr[len + 1] = '\0';
    }
}

void FIND_FIRST(char* arr, char ch)
{
    if (!isupper(ch))
    {
        add_symbol(arr, ch); // If 'ch' is a terminal, add it to the FIRST set
    }
    else
    {
        for (int i = 0; i < cr; i++)   // Loop through all the production rules
        {
            if (ch == G[i][0])   // If the left-hand side of the rule matches 'ch'
            {
                if (G[i][3] == '!')   // If the right-hand side is epsilon
                {
                    add_symbol(arr, G[i][3]); // Add epsilon to the FIRST set
                	FIND_FIRST(arr, G[i][4]);
                }
                else
                {

                    int j = 3;
                    while (G[i][j] != '\0')
                    {
                        char subarr[100] = "";
                        bool foundEpsilon = false;
                        FIND_FIRST(subarr, G[i][j]);
                        for (int k = 0; subarr[k] != '\0'; k++)
                            add_symbol(arr, subarr[k]);
                        for (int k = 0; subarr[k] != '\0'; k++)
                        {
                            if (subarr[k] == '!')
                            {
                                foundEpsilon = true;
                                break;
                            }
                        }
                        if (!foundEpsilon)
                            break;
                        j++;


                    }
                    //FIND_FIRST(arr, G[i][3]);
                }
            }
        }
    }
}

void FIND_FOLLOW(char* arr, char ch)
{
    if (ch == G[0][0])
    {
        add_symbol(arr, '$'); // Add '$' to the FOLLOW set of the start symbol
    }
    for (int i = 0; i < cr; i++)   // Loop through all the production rules
    {
        for (int j = 3; G[i][j] != '\0'; j++)
        {
            if (G[i][j] == ch)   // If 'ch' is found in the right-hand side of a production
            {
                if (G[i][j + 1] != '\0')
                {
                    // If the next symbol is a non-terminal
                    if (isupper(G[i][j + 1]))
                    {
                        int nt_index_next = nt_index(G[i][j + 1]);
                        bool hasEpsilon = false;
                        for (int k = j + 1; G[i][k] != '\0'; k++)
                        {
                            char subarr[100] = "";
                            FIND_FIRST(subarr, G[i][k]);
                            bool foundEpsilon = false;
                            // Add FIRST set (excluding epsilon) to FOLLOW(ch)
                            for (int l = 0; subarr[l] != '\0'; l++)
                            {
                                if (subarr[l] != '!')
                                {
                                    add_symbol(arr, subarr[l]);
                                }
                                else
                                {
                                    foundEpsilon = true;
                                }
                            }
                            if (!foundEpsilon)
                            {
                                hasEpsilon = false;
                                break;
                            }
                            else
                            {
                                hasEpsilon = true;
                            }
                        }
                        if (hasEpsilon && G[i][0] != ch)
                        {
                            FIND_FOLLOW(arr, G[i][0]);
                        }
                    }
                    else
                    {
                        // If the next symbol is a terminal
                        add_symbol(arr, G[i][j + 1]);
                    }
                }
                else if (G[i][j + 1] == '\0' && G[i][0] != ch)
                {
                    // If 'ch' is at the end of the production
                    FIND_FOLLOW(arr, G[i][0]);
                }
            }
        }
    }
}

void FIRST_SHOW()
{
    for (int i = 0; i < nt; i++)
    {
        char arr[100] = "";
        FIND_FIRST(arr, NT[i]);
        strcpy(FIRST[i], arr);
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
        char arr[100] = "";
        FIND_FOLLOW(arr, NT[i]);
        strcpy(FOLLOW[i], arr);
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

