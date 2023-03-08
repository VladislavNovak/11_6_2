// Задание 2. Проверка корректности email-адреса
// Интересно: Использован search_n для поиска рядом расположенных символов
// Интересно: Реализован split для string
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm> // search_n

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

// changes the string 'troubles' adding information
void changeWarningString(int troublesCount, string &troubles, const string &warning = "Issue found!") {
    troubles += troublesCount != 1 ? "\n - " : " - ";
    troubles += "Error! ";
    // troubles += (std::to_string(troublesCount) + ": ");
    troubles += warning;
}

// checks the original string and returns a warning string
string verifyOriginalString(string &text) {
    string troubles;
    int troublesCount = 0;

    if (text.length() < 3) {
        string warning = "Original text must contain at least three characters";
        changeWarningString(++troublesCount, troubles, warning);
    }

    if (text[0] == '@' || text[text.length() - 1] == '@') {
        string warning = "Original text cannot contain symbol @ as the first or last character";
        changeWarningString(++troublesCount, troubles, warning);
    }

    return troubles;
}

// checks the substrings and returns a warning string
string verifySubstringStrings(const vector<string> &substrings) {
    string troubles;
    int troublesCount = 0;

    if (substrings.size() != 2) {
        int delimCount = (substrings.size() - 1) <= 0 ? 0 : (int) substrings.size() - 1;
        string warning = "The string contains the wrong number of delimiters (" + std::to_string(delimCount) + ")";
        changeWarningString(++troublesCount, troubles, warning);
    }

    for (int i = 0; i < substrings.size(); ++i) {
        const string &text = substrings[i];
        int allowableLength = (i == 0) ? 64 : 63;
        int currentLength = (int) text.length();

        if (currentLength > allowableLength) {
            string warning = ((i == 0) ? "First " : "Second ") +
                             ("part must be less than " +
                              std::to_string(allowableLength) +
                              " characters. Now contains (" +
                              std::to_string(currentLength) + ")");
            changeWarningString(++troublesCount, troubles, warning);
        }

        int consecutiveCount = 2;
        auto result = std::search_n(begin(text), end(text), consecutiveCount, '.');
        if (result != end(text)) {
            const auto position = std::distance(begin(text), result);
            string warning = "String contain multiple consecutive characters dot. Starting from position " + std::to_string(position);
            changeWarningString(++troublesCount, troubles, warning);
        }
    }

    return troubles;
}

// returns a vector with strings separated by delim
vector<string> getSubstrings(const string &text, const char &delim) {
    vector<string> substrings;
    string temp;

    for (char c: text) {
        if (c == delim && temp.length()) {
            substrings.push_back(temp);
            temp = "";
        }

        if (c != delim) temp += c;
    }

    if (temp.length()) substrings.push_back(temp);

    return substrings;
}

int main() {
    vector<string> list = {
            // валидные:
            "simple@example.com",
            "very.common@example.com",
            "disposable.style.email.with+symbol@example.com",
            "other.email-with-hyphen@example.com",
            "fully-qualified-domain@example.com",
            "user.name+tag+sorting@example.com",
            "x@example.com",
            "example-indeed@strange-example.com",
            "admin@mailserver1",
            "example@s.example",
            "mailhost!username@example.org",
            "user%example.com@example.org",
            // невалидные:
            "",
            "@",
            "@@",
            "@bla",
            "bla@",
            "@bla@",
            "@b@la@",
            "John..Doe@example.com",
            "Abc.example.com",
            "A@b@c@example.com",
            "a\"b(c)d,e:f;g<h>i[j\\k]l@example.com",
            "1234567890123456789012345678901234567890123456789012345678901234+x@example.com",
            "i_like_underscore@but_its_not_allow_in _this_part.example.com",
    };

    for (string &item: list) {
        cout << "------" << endl;
        cout << "test: " << item << endl;

        if (item.empty()) {
            cout << " - Error! String is empty" << endl;
            continue;
        }

        string preCheck = verifyOriginalString(item);

        if (!preCheck.empty()) {
            cout << preCheck << endl;
            continue;
        }

        vector<string> substrings = getSubstrings(item, '@');

        preCheck = verifySubstringStrings(substrings);

        if (!preCheck.empty()) {
            cout << preCheck << endl;
            continue;
        }

        const string VALID_CHARS_FOR_FIRST_PART = "!#$%&'*+-/=?^_`{|}~";
        bool isTroublesFound = false;

        // spelling part ---------------------
        for (int i = 0; i < substrings.size(); ++i) {
            string substring = substrings[i];

            if ((substring[0] == '.') || (substring[substring.length() - 1] == '.')) {
                cout << " - Error! " << item << " cannot contain dot as first or last character" << endl;
                isTroublesFound = true;
                break;
            }

            for (int j = 0; j < substring.length(); ++j) {
                char c = substring[j];

                int index = (int) VALID_CHARS_FOR_FIRST_PART.find(c);
                if (!(std::isalnum(c) || c == '-' || c == '.' || (i == 0 && index != std::string::npos))) {
                    cout << " - Error! Substring (" << substring << ") contains a forbidden character: " << c << endl;
                    isTroublesFound = true;
                    break;
                }
            }
        }

        if (isTroublesFound) continue;

        cout << " - OK! String (" << item << ") validated successfully!" << endl;
    }
}