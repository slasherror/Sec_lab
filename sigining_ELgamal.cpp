#include <iostream>
#include <cmath>
#include <set>
#include <string>
#include <cstdlib>
using namespace std;

// Function to calculate power modulo p (g^k % p)
long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}


long long find_generator(long long p) {
    set<long long> num;
    long long g;
    long long check = 0;

    for (long long j = 2; j < p; j++) {
        num.clear();
        long long siz = 0;
        for (long long i = 1; i < p; i++) {
            siz++;
            long long x = mod_exp(j, i, p);
            num.insert(x);
            if (siz != num.size()) break;
            if (siz == p - 1) {
                g = j;
                check = 1;
                break;
            }
        }
        if (check) break;
    }
    return check ? g : -1;
}

// Modular inverse using Extended Euclidean Algorithm
long long mod_inverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m; a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}


pair<long long, long long> elgamal_encrypt(long long m, long long p, long long g, long long y, long long k) {
    long long c1 = mod_exp(g, k, p);
    long long c2 = (m * mod_exp(y, k, p)) % p;
    return make_pair(c1, c2);
}

long long elgamal_decrypt(long long c1, long long c2, long long p, long long x) {
    long long s = mod_exp(c1, x, p);
    long long s_inv = mod_exp(s, p - 2, p); // Fermat's inverse since p is prime
    return (c2 * s_inv) % p;
}


pair<long long, long long> elgamal_sign(long long m, long long p, long long g, long long x, long long k) {
    long long r = mod_exp(g, k, p);                 // r = g^k mod p
    long long k_inv = mod_inverse(k, p - 1);        // inverse of k mod (p-1)
    long long s = ((m - (x * r) % (p - 1) + (p - 1)) * k_inv) % (p - 1);
    return make_pair(r, s);
}


bool elgamal_verify(long long r, long long s, long long m, long long p, long long g, long long y) {
    long long v1 = mod_exp(g, m, p);
    long long v2 = (mod_exp(y, r, p) * mod_exp(r, s, p)) % p;
    return v1 == v2;
}

int main() {
    long long p = 17;  
    long long g = find_generator(p);

    if (g == -1) {
        cout << "No primitive root found.\n";
        return 0;
    }

    cout << "Primitive Root (Generator) g: " << g << endl;

    long long x = 15;  
    long long y = mod_exp(g, x, p);  

    cout << "Public Key: (p = " << p << ", g = " << g << ", y = " << y << ")\n";
    cout << "Private Key: " << x << endl;

    long long m = 9;  
    long long k = 6; 

    // Encryption
    pair<long long, long long> ciphertext = elgamal_encrypt(m, p, g, y, k);
    cout << "Ciphertext: (c1 = " << ciphertext.first << ", c2 = " << ciphertext.second << ")\n";

    // Decryption
    long long decrypted_message = elgamal_decrypt(ciphertext.first, ciphertext.second, p, x);
    cout << "Decrypted Message: " << decrypted_message << endl;

    // Digital Signature
    pair<long long, long long> signature = elgamal_sign(m, p, g, x, k);
    cout << "Digital Signature: (r = " << signature.first << ", s = " << signature.second << ")\n";

    // Verification
    bool is_valid = elgamal_verify(signature.first, signature.second, m, p, g, y);
    cout << "Signature is " << (is_valid ? "Valid" : "Invalid") << endl;

    return 0;
}
