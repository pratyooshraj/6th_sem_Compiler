#include <iostream>
#include <string>
using namespace std;
void printstate(int i)
{
    if (i == 0)
        cout << "Accepted by a*" << endl;
    else if (i == 1)
        cout << "Accepted by a*b+" << endl;
    else if (i == 2)
        cout << "Not accepted" << endl;
}

int main()
{
    string str;
    int i, f = 0, flag = 0;
    cout << "Enter the string to be checked: ";
    cin >> str;
    cout << "Q" << f << "--";
    for (i = 0; str[i] != '\0'; i++)
    {
        cout << str[i] << "-->";
        if (str[i] != 'a' && str[i] != 'b')
        {
            cout << "Invalid string" << endl;
            exit(1);
        }
        switch (f)
        {
        case 0:
            if (str[i] == 'a')
                f = 0;
            else if (str[i] == 'b')
                f = 1;
            cout << "  Q" << f << "--";
            break;
        case 1:
            if (str[i] == 'a')
                f = 2;
            else if (str[i] == 'b')
                f = 1;
            cout << "  Q" << f << "--";
            break;
        case 2:
            if (str[i] == 'a')
                f = 2;
            else if (str[i] == 'a')
                f = 2;
            cout << "  Q" << f << "--";
            exit(0);
            break;
        }
    }

    printstate(f);
    if (str == "abb")
        cout << "accepted by abb" << endl;
}
