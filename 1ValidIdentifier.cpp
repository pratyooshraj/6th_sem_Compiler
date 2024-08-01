#include <iostream>
#include <string>
#include<cctype>
#include<algorithm>
using namespace std;

bool isValid(string str, int n)
{

    // If first character is invalid
    if (!((toupper(str[0]) >= 'A' && toupper(str[0]) <= 'Z')|| str[0] == '_'))
    {
        cout<<"First character should be alphabet or underscore"<<endl;
        return false;
    }

    // Traverse the string for the rest of the characters
    for (int i = 1; i < str.length(); i++)
    {
        if (!((toupper(str[i]) >= 'A' && toupper(str[i]) <= 'Z')|| (str[i] >= '0' && str[i] <= '9')|| str[i] == '_'))
        {
            cout<<"Character should be alphabet, letter or underscore"<<endl;
            return false;
        }
    }

    string s=str;
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    if(s=="auto"||s=="break"||s=="case"||s=="char"||s=="const"||s=="continue"||s=="default"||s=="do"
            ||s=="double"||s=="else"||s=="enum"||s=="extern"||s=="float"||s=="for"||s=="goto"||s=="if"
            ||s=="int"||s=="long"||s=="register"||s=="return"||s=="short"||s=="signed"||s=="sizeof"
            ||s=="static"||s=="struct"||s=="switch"||s=="typedef"||s=="union"||s=="unsigned"||s=="void"||s=="volatile"||s=="while")
    {
        cout<<"Identifiers cannot be keyword"<<endl;
        return false;
    }
    // String is a valid identifier
    return true;
}

int main()
{
    string str;
    cout<<"enter string\t";
    cin>>str;
    int n = str.length();
    if(n>31)
    {
        cout<<"string length cannot be greater than 31 characters"<<endl;
        cout<<"Invalid identifier";
        exit(1);
    }
    if (isValid(str, n))
        cout << "Valid identifier";
    else
        cout << "Invalid identifier";
    return 0;
}
