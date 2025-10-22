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

// Sign a message using private key (d, n)
long long sign_message(long long message, long long d, long long n) {
    return mod_exp(message, d, n);  // Signature = message^d mod n
}

// Verify a signature using public key (e, n)
bool verify_signature(long long message, long long signature, long long e, long long n) {
    long long calculated_message = mod_exp(signature, e, n);  // message = signature^e mod n
    return calculated_message == message;
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
    // Key generation
    long long p = 61;  // First prime number
    long long q = 53;  // Second prime number
    long long n = p * q;  // Public modulus
    long long phi = (p - 1) * (q - 1);  // Euler's totient function
    
    // Generate public and private keys
    long long e = public_key(phi);  // Public exponent
    long long d = modInverse(e, phi);  // Private exponent

    cout << "Public key (e, n): (" << e << ", " << n << ")" << endl;
    cout << "Private key (d, n): (" << d << ", " << n << ")" << endl;

    // Original message
    long long message = 42;
    cout << "\nOriginal Message: " << message << endl;

    // Sign the message
    long long signature = sign_message(message, d, n);
    cout << "Signature: " << signature << endl;

    // Verify the signature
    bool is_valid = verify_signature(message, signature, e, n);
    cout << "Signature verification: " << (is_valid ? "Valid" : "Invalid") << endl;

    // Try to verify with a tampered message
    long long tampered_message = message + 1;
    bool is_tampered_valid = verify_signature(tampered_message, signature, e, n);
    cout << "\nVerification with tampered message: " << (is_tampered_valid ? "Valid" : "Invalid") << endl;

    return 0;
}
