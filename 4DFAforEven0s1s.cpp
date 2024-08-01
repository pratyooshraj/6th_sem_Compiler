#include <iostream>
#include <string>
using namespace std;
void printstate(int i)
{
    if (i == 0)
        cout << "Accepted" << endl
             << "String has even number of 0s and 1s" << endl;
    else
        cout << "Not Accepted" << endl
             << "String doesn't have even number of 0s and 1s" << endl;
}

int main()
{
    string str;
    while (true)
    {
        int i, f = 0, flag = 0;
        cout << "Enter the string to be checked: ";
        cin >> str;
        cout << "Q" << f << "--";
        for (i = 0; str[i] != '\0'; i++)
        {
            cout << str[i] << "-->";
            if (str[i] != '0' && str[i] != '1')
            {
                cout << "Invalid string" << endl;
                exit(1);
            }
            switch (f)
            {
            case 0:
                if (str[i] == '0')
                    f = 1;
                else if (str[i] == '1')
                    f = 3;
                cout << "  Q" << f << "--";
                break;
            case 1:
                if (str[i] == '0')
                    f = 0;
                else if (str[i] == '1')
                    f = 2;
                cout << "  Q" << f << "--";
                break;
            case 2:
                if (str[i] == '0')
                    f = 3;
                else if (str[i] == '1')
                    f = 1;
                cout << "  Q" << f << "--";
                break;
            case 3:
                if (str[i] == '0')
                    f = 2;
                else if (str[i] == '1')
                    f = 0;
                cout << "  Q" << f << "--";
                break;
            }
        }
        printstate(f);
        cout << "press y/n to continue/exit" << endl;
        char n;
        cin >> n;
        if (n == 'n')
            exit(0);
    }
}
