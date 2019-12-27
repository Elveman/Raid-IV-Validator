#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <regex>

using namespace std;

main()
{
    while(true)
    {
        regex re(R"((\d+) (\d+) (\d+) (\d+) (\d+) (\d+))");
        string currentIVs;
        vector<int> currentIVsMod8 = {-1, -1, -1, -1, -1, -1};
        vector<int> currentPerfectIVs = {-1, -1, -1, -1, -1, -1};
        vector<string> statNames = {"HP", "Atk", "Def", "SpA", "SpD", "Spe"};
        int perfectIVcount = 0;
        int skipAmount = 0;
        int lastReroll = -1;
        bool errorFlag = false;
        cout << "This program allows you to check if you're able to find your seed with xoroshiro-inverse." << endl;
        cout << "Assuming you're on frame 1 right now, move 3 frames forward and catch the Pokemon at frame 4 so it has exactly 3 stats with 31 IVs (don't save the game) and calculate the rest of its IVs." << endl;
        cout << "Enter the exact IVs of the Pokemon (format: X X X X X X) to validate or \"exit\" to exit: ";
        getline(cin, currentIVs);
        if (currentIVs == "exit") exit(0);
        if (!regex_match(currentIVs, re))
        {
            cout << "Input doesn't match the format!" << endl;
            cout << "press ENTER to continue" << endl;
            while(getchar() != '\n');
            continue;
        }
        istringstream iss(currentIVs);
        vector<int> currentIVnums(istream_iterator<int>{iss}, istream_iterator<int>());
        for (int i = 0; i < 6; i++) // check for errors
        {
            if ((currentIVnums[i] < 0) || (currentIVnums[i] > 31))
            {
                cout << "Invalid IV number in " << statNames[i] << endl;
                errorFlag = true;
                break;
            }
            if (currentIVnums[i] == 31)
            {
                perfectIVcount++;
                currentPerfectIVs[i] = 1;
            }
            currentIVsMod8[i] = currentIVnums[i] % 8;
        }
        if (perfectIVcount != 3)
        {
            cout << "Invalid perfect IV amount!" << endl;
            errorFlag = true;
        }
        if (errorFlag) 
        {
            cout << "press ENTER to continue" << endl;
            while(getchar() != '\n');
            continue;
        }
        for (int i = 0; i < 6; i++) // check validity
        {
            if (currentPerfectIVs[i] == 1)
            {
                continue;
            }
            if ((currentIVsMod8[i] >= 6) || (currentPerfectIVs[currentIVsMod8[i]] == 1))
            {
                skipAmount++;
            } else
            {
                lastReroll = i;
                break;
            }
        }
        if (skipAmount == 2)
        {
            currentPerfectIVs[currentIVsMod8[lastReroll]] = 1;
            cout << "IVs are valid! Expected 4x31 IV Pokemon IVs on the same frame:";
            for (int i = 0; i < 6; i++)
            {
                cout << " " << ((currentPerfectIVs[i] == 1) ? "31" : "X");
            }
            cout << endl;
        } else if (skipAmount < 2)
        {
            cout << "Invalid IVs: rerolls end before reaching last non-perfect IV stat. Last roll happens on " << statNames[lastReroll] << endl;
        } else
        {
            cout << "Invalid IVs: can't determine 4th perfect IV from the current IVs" << endl;
        }
        cout << "press ENTER to continue" << endl;
        while(getchar() != '\n');
    }
}

