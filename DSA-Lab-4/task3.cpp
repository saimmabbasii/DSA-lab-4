#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

struct BitNode {
    int bit;
    BitNode* next;
    BitNode* prev;

    BitNode(int val) : bit(val), next(nullptr), prev(nullptr) {}
};

class BinaryNumber {
private:
    BitNode* head;
    BitNode* tail;

    void clear() {
        BitNode* current = head;
        while (current != nullptr) {
            BitNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = tail = nullptr;
    }

public:
    BinaryNumber() : head(nullptr), tail(nullptr) {}

    BinaryNumber(const BinaryNumber& other) : head(nullptr), tail(nullptr) {
        BitNode* curr = other.head;
        while (curr != nullptr) {
            appendBit(curr->bit);
            curr = curr->next;
        }
    }

    ~BinaryNumber() {
        clear();
    }

    BinaryNumber& operator=(const BinaryNumber& other) {
        if (this != &other) {
            clear();
            BitNode* curr = other.head;
            while (curr != nullptr) {
                appendBit(curr->bit);
                curr = curr->next;
            }
        }
        return *this;
    }

    void appendBit(int bit) {
        BitNode* newNode = new BitNode(bit);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void prependBit(int bit) {
        BitNode* newNode = new BitNode(bit);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    // Store input string into multiple 8-bit grouped DLL format
    void storeBinary(string binStr) {
        clear();
        // Remove spaces if present
        binStr.erase(remove(binStr.begin(), binStr.end(), ' '), binStr.end());

        // Pad to nearest multiple of 8 bits
        int rem = binStr.length() % 8;
        if (rem != 0) {
            binStr = string(8 - rem, '0') + binStr;
        }

        for (char ch : binStr) {
            appendBit(ch - '0');
        }
    }

    void display() const {
        if (head == nullptr) {
            cout << "0";
            return;
        }
        BitNode* curr = head;
        int count = 0;
        while (curr != nullptr) {
            cout << curr->bit;
            count++;
            if (count % 8 == 0 && curr->next != nullptr) {
                cout << " "; // Group by 8 bits
            }
            curr = curr->next;
        }
        cout << "\n";
    }

    // 1's Complement
    BinaryNumber onesComplement() const {
        BinaryNumber result;
        BitNode* curr = head;
        while (curr != nullptr) {
            result.appendBit(1 - curr->bit);
            curr = curr->next;
        }
        return result;
    }

    // 2's Complement
    BinaryNumber twosComplement() const {
        BinaryNumber ones = onesComplement();
        BinaryNumber one;
        one.appendBit(1);
        return add(ones, one);
    }

    // Binary Addition
    static BinaryNumber add(const BinaryNumber& num1, const BinaryNumber& num2) {
        BinaryNumber result;
        BitNode* p1 = num1.tail;
        BitNode* p2 = num2.tail;
        int carry = 0;

        while (p1 != nullptr || p2 != nullptr || carry != 0) {
            int sum = carry;
            if (p1 != nullptr) {
                sum += p1->bit;
                p1 = p1->prev;
            }
            if (p2 != nullptr) {
                sum += p2->bit;
                p2 = p2->prev;
            }

            result.prependBit(sum % 2);
            carry = sum / 2;
        }

        // Pad to 8-bit alignment
        while (result.length() % 8 != 0) {
            result.prependBit(0);
        }

        return result;
    }

    // Shift left by n positions (equivalent to multiplying by 2^n)
    void shiftLeft(int positions) {
        for (int i = 0; i < positions; ++i) {
            appendBit(0);
        }
    }

    // Binary Multiplication using repeated addition + shifting
    static BinaryNumber multiply(const BinaryNumber& num1, const BinaryNumber& num2) {
        BinaryNumber product;
        product.appendBit(0);

        BitNode* p2 = num2.tail;
        int shiftCount = 0;

        while (p2 != nullptr) {
            if (p2->bit == 1) {
                BinaryNumber shiftedNum1 = num1;
                shiftedNum1.shiftLeft(shiftCount);
                product = add(product, shiftedNum1);
            }
            shiftCount++;
            p2 = p2->prev;
        }

        return product;
    }

    // Conversion to Decimal
    long long toDecimal() const {
        long long decimal = 0;
        BitNode* curr = tail;
        long long power = 1;

        while (curr != nullptr) {
            decimal += curr->bit * power;
            power *= 2;
            curr = curr->prev;
        }
        return decimal;
    }

    int length() const {
        int len = 0;
        BitNode* curr = head;
        while (curr != nullptr) {
            len++;
            curr = curr->next;
        }
        return len;
    }
};

int main() {
    BinaryNumber bin1, bin2;

    string b1 = "1011"; // 11 in decimal
    string b2 = "0101"; // 5 in decimal

    bin1.storeBinary(b1);
    bin2.storeBinary(b2);

    cout << "Binary 1 (8-bit block): ";
    bin1.display();
    cout << "Decimal 1: " << bin1.toDecimal() << "\n\n";

    cout << "Binary 2 (8-bit block): ";
    bin2.display();
    cout << "Decimal 2: " << bin2.toDecimal() << "\n\n";

    // 1's Complement
    BinaryNumber ones = bin1.onesComplement();
    cout << "1's Complement of Binary 1: ";
    ones.display();

    // 2's Complement
    BinaryNumber twos = bin1.twosComplement();
    cout << "2's Complement of Binary 1: ";
    twos.display();

    // Addition
    BinaryNumber sum = BinaryNumber::add(bin1, bin2);
    cout << "\nBinary Addition (Binary 1 + Binary 2): ";
    sum.display();
    cout << "Sum in Decimal: " << sum.toDecimal() << "\n";

    // Multiplication
    BinaryNumber product = BinaryNumber::multiply(bin1, bin2);
    cout << "\nBinary Multiplication (Binary 1 * Binary 2): ";
    product.display();
    cout << "Product in Decimal: " << product.toDecimal() << "\n";

    return 0;
}