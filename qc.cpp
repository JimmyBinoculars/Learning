#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
using namespace std;


vector<string> decoder(string contents, list<string> keywords, int currentPlace)
{
    char currentLetter;
    string currentKeyword;
    string targetKeyword;

    int a=0;
    for(int i=currentPlace; i<(contents.length()); i++)
    {
        currentLetter = contents[i];

        if(a != 0)
        {
            if(currentLetter == targetKeyword[currentKeyword.length()])
            {
                currentKeyword += currentLetter;
                if(currentKeyword.length()>=targetKeyword.length())
                {
                    return {targetKeyword , to_string(currentPlace),to_string(currentKeyword.length()+currentPlace)};
                }
            }else
            {
                return {"Could not find keyword on first attempt"};
            }
        }
        
        for(string keyword : keywords)
        {
            if(a == 0)
            {
                if(currentLetter == keyword[currentKeyword.length()])
                {
                    currentKeyword += currentLetter;
                    targetKeyword = keyword;
                    a++;
                }
            }
        }
        
    }
    return {};
};

int main()
{
    string path;
    list<string> keywords = {"print", "if"};
    vector<vector<string>> foundKeywords;
    vector<string> statements;
    vector<string> substatements;
    cout << "What is the full path of your code: ";
    cin >> path;

    ifstream file(path);
    stringstream buffer;
    buffer << file.rdbuf();
    string contents = buffer.str();

    cout << "File contents:\n" << contents << endl << "length: " << contents.length() << endl;

    int end = 0;
    int cursor = 0;//This cursor is relative
    string currentChar;
    bool stateEnd = false;
    string statement;
    while (end <= (contents.length() - 1))
    {
        foundKeywords.insert(foundKeywords.begin(), decoder(contents, keywords, end));
        end = stoi(foundKeywords[0][2]);
        statement = foundKeywords[0][0];
        cout << "keyword: " << foundKeywords[0][0] << " length: " << foundKeywords.size() << endl;
        cout << "end: " << end << endl;
        stateEnd = false;
        cursor = end;

        while(stateEnd == false)
        {
            currentChar = contents[cursor];
            if(cursor > (contents.length()))
            {
                cout << "Expected \";\" at: " << foundKeywords[0][2] << endl << "(You need to end the statment)" << endl;
                return -1;
            }
            if(currentChar == ";")
            {
                statement += currentChar;
                end = cursor;
                stateEnd = true;
                statements.insert(statements.begin(), statement);
            }else
            {
                statement += currentChar;
            }
            cursor++;
        }
    }

    bool substateEnd = false;
    int substateCursor = 0;
    //Look for brackets
    for(int i = 0; i < statements.size();)
    {
        end = stoi(foundKeywords[i][2]);
        cout << endl << "Current statement: " << statements[i] << endl;
        substateEnd = false;
        stateEnd = false;
        while (stateEnd == false)
        {
            if(statements[i][end] == '(')
            {
                //Opening of a "substatement"
                cout << "Found opening of a substatement" << endl;
                substateCursor = end;
                while(substateEnd == false)
                {
                    substateCursor++;
                    if(statements[i][substateCursor] == ')')
                    {
                        cout << "Found closing of a substatement at: " << statements[i][substateCursor] << endl;
                        substatements[i] += statements[i][substateCursor];
                        substateEnd = true;
                    }else
                    {
                        substatements[i] += statements[i][substateCursor];
                        cout << "Could not find closing substatement char at: " << substateCursor << " (" << statements[i][substateCursor] << ")" << endl;
                    }
                }
            }
        }
        i++;
    }

    for (int i = 0; i < statements.size(); i++)
    {
        cout << "Statement " << i+1 << " in statements is: " << statements[i] << endl;
        cout << "Substatement " << i+1 << " in substatments is: " << substatements[i] << endl;
    }
    
    return 0;
}