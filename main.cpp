#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


bool checkСompleteness(vector<bool> &BinaryNotation) {
    for(auto &&currCharacter : BinaryNotation)
        if(currCharacter != BinaryNotation[0]) return false;
    return true;
}

struct Triangle {
    bool сompleteness = false;
    vector<bool> BinaryNotation;
    vector<int> Pos;
    Triangle(bool currCompleteness, vector<bool> &CurrBinaryNotation, vector<int> &currPos) :
        сompleteness(currCompleteness), BinaryNotation(CurrBinaryNotation), Pos(currPos) {};
};

vector< vector<bool> > TransformFrom {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0},
                                      {0, 0, 1, 0}, {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0},
                                      {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
                                      {0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}
};
vector< vector<bool> > TransformTo   {{0, 0, 0, 0}, {0, 0, 0, 1}, {1, 0, 0, 0}, {0, 1, 0, 0},
                                      {0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 1}, {1, 1, 0, 1},
                                      {0, 0, 1, 0}, {1, 0, 1, 0}, {1, 1, 1, 0}, {1, 1, 1, 1},
                                      {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 0}, {1, 1, 1, 1}
};


void transform(vector<bool> &BinaryNotation) {
    for(int i = 0; i < 16; i++)
        if (BinaryNotation == TransformFrom[i]) {
            BinaryNotation = TransformTo[i];
            return;
        }
}

vector<Triangle> getTriangles(vector<bool> &InputBinaryNotation) {
    vector<Triangle> Triangles;
    int currPos = 0, currLevel = 0, currItem = 0;
    while(currPos < InputBinaryNotation.size()) {
        vector<bool> BinaryNotation;
        vector<int> Pos;

        BinaryNotation.push_back(InputBinaryNotation[currPos]);
        Pos.push_back(currPos);
        if(currItem % 2 == 0) currPos += 4 * currLevel + 1;
        else currPos += -4 * (currLevel + 1) + 1;;
        for(int i = 0; i < 3; i++) {
            BinaryNotation.push_back(InputBinaryNotation[currPos]);
            Pos.push_back(currPos);
            currPos++;
        }

        Triangles.push_back(Triangle(checkСompleteness(BinaryNotation), BinaryNotation, Pos));

        if(int(sqrt(currPos)) * int(sqrt(currPos)) == currPos) {
            currItem = 0;
            currLevel++;
        }
        else currItem++;
    }
    return Triangles;
}

void printAllTransformations(vector<Triangle> &Triangles, vector<bool> &BinaryNotation) {
    bool continuationСondition = true;
    vector<bool> Transformation(Triangles.size() * 4);
    while(continuationСondition) {
        continuationСondition = false;
        for(auto &&currTriangle : Triangles) {
            if(!currTriangle.сompleteness)
                transform(currTriangle.BinaryNotation);
            currTriangle.сompleteness = checkСompleteness(currTriangle.BinaryNotation);
            if(!currTriangle.сompleteness) continuationСondition = true;
            for(int i = 0; i < 4; i++)
                Transformation[currTriangle.Pos[i]] = currTriangle.BinaryNotation[i];
        }
        if(Transformation != BinaryNotation) {
            cout << '\n';
            for (int i = Transformation.size() - 1; i >= 0; i--)
                cout << int(Transformation[i]);
        }
    }
}

vector<bool> GetAndPrintNextReduction(vector<Triangle> &Triangles) {
    vector<bool> NextReduction;
    cout << '\n';
    for(int i = 0; i < Triangles.size(); i++)
        NextReduction.push_back(Triangles[i].BinaryNotation[0]);
    if(checkСompleteness(NextReduction)) {
        bool character = NextReduction[0];
        NextReduction.clear();
        NextReduction.push_back(character);
    }
    for(int i = NextReduction.size() - 1; i >= 0; i--)
        cout << int(NextReduction[i]);
    return NextReduction;
}

void getSolution(vector<bool> &BinaryNotation) {
    if(BinaryNotation.size() == 1) return;

    vector<Triangle> T = getTriangles(BinaryNotation);

    printAllTransformations(T, BinaryNotation);

    vector<bool> NextReduction = GetAndPrintNextReduction(T);

    getSolution(NextReduction);
}

bool checkValidity(string &inputString) {
    int size = inputString.length();
    while(size > 1) {
        if(size % 4 != 0) {
            cout << "invalid string length";
            return 0;
        }
        size /= 4;
    }
    for(auto &&currCharacter : inputString) {
        if(currCharacter != '1' && currCharacter != '0') {
            cout << "there are invalid characters";
            return 0;
        }
    }
    cout << inputString;
    return 1;
}

int main(int argc, char *argv[]) {
    std::stringstream convert{ argv[1] };

    string inputString;
    convert >> inputString;

    bool validity = checkValidity(inputString);
    if(!validity) return 0;

    vector<bool> BinaryNotation;
    for(auto &&c : inputString)
        BinaryNotation.push_back(c - '0');
    reverse(BinaryNotation.begin(), BinaryNotation.end());

    getSolution(BinaryNotation);
    return 0;
}