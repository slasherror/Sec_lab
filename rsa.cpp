#include <iostream>
// #include <cmath> // For the pow function
#include<bits/stdc++.h>
using namespace std;


long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;  // Ensure base is less than mod
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}

long long modInverse(long long e, long long phi) {
    for (long long d = 2; d < phi; d++) {
        if ((e * d) % phi == 1)
            return d;
    }
    return -1;
}

long long encrypt(long long mes, long long e, long long n) {
    return mod_exp(mes, e, n);
}
long long decrypt(long long cipher, long long d, long long n) {
    return mod_exp(cipher, d, n);
}


int product(long long a, long long b) {
    return a * b;
}

// 1 < e < Φ(n), and
// gcd(e, Φ(n)) = 1
long long public_key(long long phi) {
    for (long long e = 2; e < phi; e++) {
        if (__gcd(e, phi) == 1)
            return e;
    }
    return -1;
}


int main() {
    long long p = 61;  // Example prime number
    long long q = 53;  // Example prime number
    long long n = p * q;
    long long e = 17;  // Example public exponent
    long long d = 2753;  // Example private exponent

    long long message = 42;  // Example message
    long long message2 = 10;  // Example message
    long long encrypted = encrypt(message, e, n);
    long long encrypted2 = encrypt(message2, e, n);
    long long decrypted = decrypt(encrypted, d, n);
    long long product_result = product(encrypted, encrypted2);
    long long decrypted_product = decrypt(product_result, d, n);

    cout << "Original message: " << message << endl;
    cout << "Encrypted message: " << encrypted << endl;
    cout << "Decrypted message: " << decrypted << endl;
    cout<<"Original message2: " << message2 << endl;
    cout << "Encrypted message2: " << encrypted2 << endl;
    cout << "Decrypted product message: " << decrypted_product << endl;

    return 0;
}
