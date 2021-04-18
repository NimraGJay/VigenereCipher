#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <map>
#include <array>
#include <algorithm>
#include <string>

using namespace std;

//DEFINITIONS

//== == == == == == == ==VARIABLES DEFINITION= == == == == == == == =//
typedef vector<char> charVect;
struct sort_pred
{
    bool operator()(const std::pair<int, double> &left, const std::pair<int, double> &right)
    {
        return left.second > right.second;
    }
};
struct sort_pred_rev
{
    bool operator()(const std::pair<int, double> &left, const std::pair<int, double> &right)
    {
        return left.second < right.second;
    }
};
vector<pair<int, double>> keyWithIC;
vector<pair<int, double>> keyWithICSorted;
charVect key;
int keyLength;
charVect decryptedText;
charVect decryptedProcessed;
vector<vector<pair<int, double>>> chiTable;
array<double, 26> letterFrequency = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228,
    0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025,
    0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987,
    0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
    0.01974, 0.00074};

//== == == == == == == FUNCTIONS DEFINITION == == == == == == == == =//
charVect readFile(string);
int findKeyLength(charVect cipher);
vector<charVect> findSequences(int key, charVect cphr);
int counter(int ch, charVect vect);
charVect processVect(charVect fileInput);
void findKey(int keyLength, charVect processedInput);
void displayCharVect(charVect c);
void displayCharVectInt(charVect c);
double calculateChiSquare(charVect sequence);
charVect decryptCaesar(int shift, charVect sqnc);
int getInt(char ch);
charVect getKey(vector<vector<pair<int, double>>> chiTable);
charVect decryptVigenere(charVect cipherText, charVect key);
void reEvaluateKeyUsingString(int index, string str, charVect cphrtxt, charVect &key);
void reEvaluateUsingIndex(int index, charVect &key);
int searchWord(string word, charVect vect);
//== == == == == == == == == == == MAIN == == == == == == == == == == =//
int main()
{
    //Initialisation
    vector<char> fileAscii;
    vector<char> processedFileInput;

    //Reading File datasets 2
    fileAscii = readFile("dataset3.txt");
    processedFileInput = processVect(fileAscii);

    // keyLength = findKeyLength(processedFileInput);
    // cout << "\nKL: " << keyLength << "\n";
    keyLength = 486;
    //keyLength = 80;

    findKey(keyLength, processedFileInput);
    key = getKey(chiTable);
    decryptedText = (decryptVigenere(fileAscii, key));
    decryptedProcessed = decryptVigenere(processedFileInput, key);

    // int h = searchWord("comm", decryptedProcessed);
    // cout << "H: " << h << endl;

    int userOption;
    string word;
    int startingIndex;
    string dummy;
    string userCorrection;
    do
    {
        cout << "\nTHE CURRENT KEY IS :";
        displayCharVect(key);
        cout << "\nTHE DECRYPTED TEXT USING KEY IS\n ";
        decryptedText = decryptVigenere(fileAscii, key);
        displayCharVect(decryptedText);
        cout << "\nPress 0 to confirm the key\nPress 1 to evaluate the key:";
        cin >> userOption;
        switch (userOption)
        {
        case 0:
            cout << "\nPROGRAM EXITED\n";
            break;
        case 1:
            cout << "\nEnter the word you want to start making changes from:";
            cin >> word;
            cout << "\nWord: " << word;
            startingIndex = searchWord(word, decryptedProcessed);
            cout << "\nStaring index: " << startingIndex << endl;
            if (startingIndex == -1)
            {
                cout << "\nBAD sequence\n";
                break;
            }
            cout << "\nIndex will be: " << startingIndex;
            getline(cin, dummy);
            cout << "\nEnter the correct sequence of plain text beginning from the index:";
            getline(cin, userCorrection);
            cout << "\nEvaluation using index (" << startingIndex << ") and data as : " << userCorrection << "\n";
            reEvaluateKeyUsingString(startingIndex, userCorrection, processedFileInput, key);
            break;
        default:
            break;
        }
    } while (userOption != 0);

    return 0;
}

//== == == == == == == == = FUNCTIONS == == == == == == == == =//

int searchWord(string word, vector<char> vect)
{
    int correctInRow = 0;
    int wordIndex = 0;
    int index;

    for (int i = 0; i < vect.size(); i++)
    {
        if (vect.at(i) == word[wordIndex])
        {
            correctInRow++;
            wordIndex++;
            if (correctInRow == word.length())
            {
                index = i - word.length() + 1;
                return index;
            }
        }
        else
        {
            wordIndex = 0;
            correctInRow = 0;
            if (vect.at(i) == word[wordIndex])
            {
                wordIndex++;
                correctInRow++;
            }
        }
    }
    return -1;
}
//Takes the user string and replaces the ciphertext with it.
void reEvaluateKeyUsingString(int index, string str, charVect cphrtxt, charVect &key)
{
    cout << "\nRE EVALUATING KEY USING STRING\n";
    char c;
    char k;
    int ki;
    for (int si = 0; si < str.length(); si++)
    {
        c = str[si];
        if (c == 32)
        {
            // cout << "str[si] is a spc char. spaces are ignored";
            continue;
        }
        c = tolower(c);
        k = (cphrtxt.at(index) - 97) - (c - 97);
        if (k < 0)
        {
            k += 26;
        }
        ki = index % key.size();
        key.at(ki) = (k + 97);
        index++;
    }
}
//getting initial key by taking in the shifts with minimum chi score
charVect getKey(vector<vector<pair<int, double>>> chiTable)
{
    charVect key;
    for (int i = 0; i < chiTable.size(); i++)
    {
        // cout << " num: " << chiTable.at(i).at(0).first << " char: " << (char)(chiTable.at(i).at(0).first + 97) << " chi : " << chiTable.at(i).at(0).second << endl;
        // cout << "ALTERNATIVE num: " << chiTable.at(i).at(1).first << " char: " << (char)(chiTable.at(i).at(1).first + 97) << " chi : " << chiTable.at(i).at(1).second << endl;
        key.push_back((char)chiTable.at(i).at(0).first + 97);
    }
    return key;
}
//displaying a vector of char
void displayCharVect(charVect c)
{

    for (int i = 0; i < c.size(); i++)
    {
        cout << c.at(i);
    }
    cout << endl;
}
//displaying a vector of char in int form
void displayCharVectInt(charVect c)
{

    for (int i = 0; i < c.size(); i++)
    {
        cout << (int)c.at(i) << " ";
    }
    cout << endl;
}
//READ DATASET FILE and store in VECTOR of characters
charVect readFile(string fileName)
{
    ifstream input;
    charVect fileAscii;
    input.open(fileName, ios::binary | ios::in);

    if (!input)
    {
        cerr << "There was an error with the file\n";
        terminate;
    }
    char c;
    //   int i = 0;
    while (input >> noskipws >> c)
    {
        fileAscii.push_back(c);
    }

    input.close();
    return fileAscii;
}

//Processes file by removing spaces, punctuations and numbers as they are not encrypted and turning the char to lowercase
charVect processVect(charVect fileInput)
{
    charVect vect;

    for (int i = 0; i < fileInput.size(); i++)
    {
        char ch;
        ch = tolower(fileInput.at(i));

        if (ch < 65 || ch > 122 || (ch > 90 && ch < 97))
        {
            continue;
        }

        vect.push_back((char)ch);
    }

    return vect;
}

int findKeyLength(charVect cipher)
{
    vector<charVect> sqnces;
    double sum = 0.0,
           sumSqnces = 0.0,
           freq,
           freqSquare,
           IC;
    int count = 0;
    // For each possible key, it finds the sequences, calculates each sequence IC, add the ICs together and finds Average.
    // to calculate IC, it needs a sequence it calculates the IC for
    // formula for IC: sum from i=0 to i = sequence.size() > pow(frequency of char at i, 2);

    for (int possibleKey = 2; possibleKey < cipher.size() / 2; possibleKey++)
    {
        //cout << "KEY: " << possibleKey << "\n";
        sqnces = findSequences(possibleKey, cipher);

        sumSqnces = 0.0;

        for (int i = 0; i < sqnces.size(); i++)
        {
            //cout << "FOR SEQUENCE " << i << endl;
            sum = 0.0;
            freq = 0.0;
            freqSquare = 0.0;

            for (int j = 0; j < 256; j++)
            {
                count = counter(j, sqnces.at(i));
                freq = ((double)count * (count - 1));
                freqSquare = (double)freq / (((sqnces.at(i).size()) * (sqnces.at(i).size() - 1)));
                sum += (double)freqSquare;
            }
            sumSqnces += sum;
        }
        IC = (double)sumSqnces / sqnces.size();
        //cout << " with IC of: " << IC << endl;
        keyWithIC.push_back(make_pair(possibleKey, IC));
        sqnces.clear();
    }
    // for (int i = 0; i < keyWithIC.size(); i++)
    // {
    //     cout << "KEY: " << keyWithIC.at(i).first << " IC: " << keyWithIC.at(i).second << endl;
    // }
    sort(keyWithIC.begin(), keyWithIC.end(), sort_pred());

    for (int i = 0; i < 5; i++)
    {
        keyWithICSorted.push_back(keyWithIC.at(i));
    }
    sort(keyWithICSorted.begin(), keyWithICSorted.end());

    for (int i = 0; i < keyWithICSorted.size(); i++)
    {
        cout << "KEY: " << keyWithICSorted.at(i).first << " IC: " << keyWithICSorted.at(i).second << endl;
    }
    return keyWithICSorted.at(0).first;
}
charVect getSequence(int startingPoint, int key, charVect text)
{

    charVect sqnc;
    for (int i = startingPoint; i < text.size(); i = i + key)
    {
        sqnc.push_back(text.at(i));
    }
    return sqnc;
}
vector<charVect> findSequences(int key, charVect cphr)
{
    // cout << "\nFIND SEQUENCES FUNCTION:\n";
    // cout << "KEY: " << key << endl;
    vector<charVect> sequences;
    charVect sqnc;
    vector<charVect>::iterator it;

    for (int startingPoint = 0; startingPoint < key; startingPoint++)
    {
        //  cout << "\nstartingPoint: " << startingPoint << endl;
        sqnc = getSequence(startingPoint, key, cphr);
        sequences.push_back(sqnc);
        //displayCharVect(sqnc);
        sqnc.clear();
    }
    return sequences;
}

//count the occurent of a chat in a character vector
int counter(int ch, charVect vect)
{
    int count = 0;
    for (int i = 0; i < vect.size(); i++)
    {
        if ((vect.at(i)) == ch)
        {
            count++;
        }
    }
    return count;
}

/*

key length = known(i.e 486)

1) get all the possible Caesar ciphers (i.e 486)
2) for each sequence
    2.1) for each key length (0-25)
        2.1.1) calculate chi squared 
        2.1.2) choose shift as the key with the min chi squared value


*/

double calculateChiSquare(charVect sequence)
{
    double chi, chiSum = 0.0, expected, minus, minusSqr;
    int letter, count;

    for (int i = 0; i < 26; i++)
    {
        chi = 0.0;
        //cout << "alphabet no :" << i << endl;
        count = counter((i + 97), sequence);
        if (count > 0)
        {
            // cout << "count: " << count << endl;
            // cout << "letter frequency " << letterFrequency.at(i) << endl;

            expected = (double)letterFrequency.at(i) * sequence.size();
            //cout << "Expected: " << expected << endl;
            minus = count - expected;
            //cout << "Minus: " << minus;
            minusSqr = minus * minus;
            //cout << "MinusSqr: " << minusSqr << endl;

            chi = minusSqr / expected;
            // cout << "Chi: " << chi;
            chiSum += chi;
        }
        else
        {
            continue;
        }
    }
    return chiSum;
}
charVect decryptCaesar(int shift, charVect sqnc)
{
    charVect pt;
    int a;
    //cout << "SEQUENCE w/ size " << sqnc.size() << " ";
    //cout << endl;
    //displayCharVect(sqnc);
    for (int i = 0; i < sqnc.size(); i++)
    {
        a = ((sqnc.at(i) - 97) - shift) % 26;
        if (a < 0)
        {
            a += 26;
        }
        a += 97;

        pt.push_back((char)a);
    }
    return pt;
}
void findKey(int keyLength, charVect processedInput)
{
    charVect sqnc;
    double chiSqr;
    vector<pair<int, double>> chis;
    charVect plainText;
    for (int startingPoint = 0; startingPoint < keyLength; startingPoint++)
    {
        // cout << "\n";
        //cout << "STARTING POINT OF SEQUENCE: " << startingPoint << endl;

        sqnc = getSequence(startingPoint, keyLength, processedInput);
        // displayCharVect(sqnc);
        // displayCharVectInt(sqnc);
        //Calculate chiSquare for all shifts in each sequence

        for (int shift = 0; shift < 26; shift++)
        {
            //cout << "Shift " << shift;
            plainText = decryptCaesar(shift, sqnc);
            //displayCharVect(plainText);
            // displayCharVectInt(plainText);

            chiSqr = calculateChiSquare(plainText);
            //cout << "CHI: " << chiSqr << endl;
            //chiTable.at(startingPoint).at(shift).push_back(chiSqr);
            chis.push_back(make_pair(shift, chiSqr));
            if (shift == 25)
            {
                sort(chis.begin(), chis.end(), sort_pred_rev());
            }
        }
        chis.push_back(make_pair(0.00, 0.00));
        chiTable.push_back(chis);
        chis.clear();
        sqnc.clear();
        plainText.clear();
    }
}
//Retrieval of plaintext
charVect decryptVigenere(charVect cipherText, charVect key)
{
    charVect decryptedText;
    int character, plainTextChar, shiftValue, positionInKey = 0;

    //to decrypt need to go over the whole cipher text and decrypt each char
    for (int i = 0; i < cipherText.size(); i++)
    {
        if (positionInKey == key.size())
        {
            positionInKey = 0;
        }

        //shift value is the index of the key (its minus 97 because it presentented in lowercase readable form)
        shiftValue = key.at(positionInKey) - 97;
        //charUnderExamination is cipherText.at(i) we are going to be looking ata int vals of char
        character = cipherText.at(i);
        //if the character is a num or punctuation, its added as it is
        if (character < 65 || (character > 90 && character < 97) || character > 122)
        {
            decryptedText.push_back(character);
        }
        else if (character > 64 && character < 91) //now if its a capital letter
        {
            plainTextChar = (((int)character - 65) - shiftValue) % 26;
            if (plainTextChar < 0)
            {
                plainTextChar += 26;
            }
            decryptedText.push_back((char)(plainTextChar + 65));
            positionInKey++;
        }
        else if (character > 96 && character < 123) // if the character is a small letter
        {
            plainTextChar = (((int)character - 97) - shiftValue) % 26;
            if (plainTextChar < 0)
            {
                plainTextChar += 26;
            }
            decryptedText.push_back((char)(plainTextChar + 97));
            positionInKey++;
        }
    }

    return decryptedText;
}