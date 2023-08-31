#include <iostream>
#include <bitset>
#include <cstring>
#include <ctime>
using namespace std;

char exor(char a, char b) {
    if (a == b) return '0';
    else return '1';
}

void crc(char data[], char key[]) {
    int datalen = strlen(data);
    int keylen = strlen(key);

    for (int i = 0; i < keylen - 1; i++)
        data[datalen + i] = '0';

    data[datalen + keylen - 1] = '\0';

    int codelen = datalen + keylen - 1;

    char temp[50], rem[50];

    for (int i = 0; i < keylen; i++)
        rem[i] = data[i];

    for (int j = keylen; j <= codelen; j++) {
        for (int i = 0; i < keylen; i++)
            temp[i] = rem[i];

        if (rem[0] == '0') {
            for (int i = 0; i < keylen - 1; i++)
                rem[i] = temp[i + 1];
        } else {
            for (int i = 0; i < keylen - 1; i++)
                rem[i] = exor(temp[i + 1], key[i + 1]);
        }

        if (j != codelen)
            rem[keylen - 1] = data[j];
        else
            rem[keylen - 1] = '\0';
    }

    for (int i = 0; i < keylen - 1; i++)
        data[datalen + i] = rem[i];

    data[codelen] = '\0';
    cout << "\nCRC=" << rem << "\nDataword=" << data << endl;
}

int main() {
    char key[50];
    string input;  // Using string instead of a character array
    char x;

    cout << "Enter the input: ";
    getline(cin, input);

    cout << "Enter the generator: ";
    cin >> key;

    cout << "Number of Redundant bits to be added: 3" << endl;
    cout << "Dataword Sent" << endl;

    int inputLen = input.length();  // Use the length() function of string
    char data[8 * inputLen + 1];

    // Convert input characters to binary
    string binaryData = "";
    for (int i = 0; i < inputLen; i++) {
        bitset<8> binaryChar(input[i]);
        binaryData += binaryChar.to_string();
    }

    strcpy(data, binaryData.c_str());

    cout << "Input in ASCII: ";
    for (int i = 0; i < inputLen; i++) {
        cout << static_cast<int>(input[i]) << " ";
    }
    cout << endl;

    cout << "Dataword in binary: " << data << "000" << endl;

    cout << "Do you want to generate an error? (y/n)" << endl;
    cin >> x;

    if (x == 'n' || x == 'N') {
        cout << "Received Codeword" << endl;
        cout << "Syndrome is Zero, No Error" << endl;
        crc(data, key);

        // Display the ASCII string of the dataword
        cout << "Dataword in ASCII: ";
        for (int i = 0; i < strlen(data) / 8; i++) {
            string binary = binaryData.substr(i * 8, 8);
            bitset<8> binaryBitset(binary);
            char asciiChar = static_cast<char>(binaryBitset.to_ulong());
            cout << asciiChar;
        }
        cout << endl;
    } else {
        srand(time(0));
        int position = rand() % strlen(data);

        data[position] = (data[position] == '0') ? '1' : '0';
        cout << "Codeword Received" << endl;
        cout << "Syndrome is not Zero, Error Detected!" << endl;
        cout << "CRC Fail\n" << endl;

        cout << "Codeword Discarded" << endl;
    }

    return 0;
}