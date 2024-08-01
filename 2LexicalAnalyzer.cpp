#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>

using namespace std;

int isKeyword(const char *buffer)
{
    const char *keywords[] =
    {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union", "unsigned", "void",
        "volatile", "while"
    };

    int totalKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < totalKeywords; ++i)
    {
        if (strcmp(keywords[i], buffer) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int main()
{
    char ch;
    char buffer[15];
    const char operators[] = "+-*/%=";
    cout<<"Press ` or CTRL+C to exit"<<endl;

    string text;
    getline(cin,text,'`');
    int j = 0,i=0;
    ch=text[i];
    cout<<endl;
    while(ch!='\0')
    {
        if (strchr(operators, ch) != nullptr)
        {
            cout << ch << " is an operator" << endl;
        }
        else if (isalnum(ch))
        {
            buffer[j++] = ch;
        }
        else if ((ch == ' ' || ch == '\n') && (j != 0))
        {
            buffer[j] = '\0';
            j = 0;
            if (isKeyword(buffer))
            {
                cout << buffer << " is a keyword" << endl;
            }
            else
            {
                cout << buffer << " is an identifier" << endl;
            }
        }
        ch=text[++i];
    }
    return 0;
}
