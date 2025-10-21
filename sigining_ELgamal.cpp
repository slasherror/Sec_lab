#include <iostream>
#include <cmath>
#include <set>
#include <string>
#include <cstdlib>
using namespace std;

// Function to calculate power modulo p (g^k % p)
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

// Function to calculate the generator (primitive root) modulo p
long long find_generator(long long p) {
    set<long long> num;
    long long g;
    long long check = 0;

    for(long long j = 2; j < p; j++) {  // Try each number from 2 to p-1
        num.clear();
        long long siz = 0;
        for(long long i = 1; i < p; i++) {  // Check powers of j mod p
            siz++;
            long long x = mod_exp(j, i, p);
            num.insert(x);
            if(siz != num.size()) {
                break;
            }
            if(siz == p - 1) {  // If set size reaches p-1, it's a primitive root
                g = j;
                check = 1;
                break;
            }
        }
        if(check) {
            break;
        }
    }

    return check ? g : -1;  // Return the generator if found, else return -1
}

// ElGamal Encryption Function
pair<long long, long long> elgamal_encrypt(long long m, long long p, long long g, long long y, long long k) {
    long long c1 = mod_exp(g, k, p);  // Compute c1 = g^k % p
    long long c2 = (m * mod_exp(y, k, p)) % p;  // Compute c2 = m * y^k % p
    return make_pair(c1, c2);  // Return ciphertext (c1, c2)
}

// ElGamal Decryption Function
long long elgamal_decrypt(long long c1, long long c2, long long p, long long x) {
    // Compute s = c1^x % p
    long long s = mod_exp(c1, x, p);
    
    // Find modular inverse of s mod p
    long long s_inv = mod_exp(s, p - 2, p);  // Using Fermat's Little Theorem: s^(p-2) is the inverse of s mod p
    
    // Decrypt the message: m = (c2 * s_inv) % p
    long long m = (c2 * s_inv) % p;
    
    return m;  // Return the decrypted message
}

// ElGamal Digital Signature Function
pair<long long, long long> elgamal_sign(long long m, long long p, long long g, long long x, long long k) {
    long long r = mod_exp(g, k, p);  // Compute r = g^k % p
    long long k_inv = mod_exp(k, p - 2, p);  // Compute modular inverse of k modulo p-1
    long long s = (k_inv * (m - x * r) % (p - 1)) % (p - 1);  // Compute s = (k^-1 * (m - x * r)) % (p-1)
    return make_pair(r, s);  // Return signature (r, s)
}

// ElGamal Digital Signature Verification Function
bool elgamal_verify(long long r, long long s, long long m, long long p, long long g, long long y) {
    // Check the validity of the signature
    long long v1 = mod_exp(g, m, p);
    long long v2 = (mod_exp(y, r, p) * mod_exp(r, s, p)) % p;
    
    return v1 == v2;  // Return true if signature is valid, false otherwise
}

int main() {
    long long p = 17;  // Prime modulus
    long long g = find_generator(p);  // Find the generator (primitive root)
    
    if (g == -1) {
        cout << "No primitive root found.\n";
        return 0;
    }
    
    cout << "Primitive Root (Generator) g: " << g << endl;
    
    long long x = 15;  // Private key
    long long y = mod_exp(g, x, p);  // Public key (y = g^x % p)

    cout << "Public Key: (p = " << p << ", g = " << g << ", y = " << y << ")\n";
    cout << "Private Key: " << x << endl;
    
    long long m = 9;  // The message to encrypt and sign
    long long k = 6;  // Random integer for encryption and signing
    
    // **ElGamal Encryption**:
    pair<long long, long long> ciphertext = elgamal_encrypt(m, p, g, y, k);
    cout << "Ciphertext: (c1 = " << ciphertext.first << ", c2 = " << ciphertext.second << ")\n";
    
    // **ElGamal Decryption**:
    long long decrypted_message = elgamal_decrypt(ciphertext.first, ciphertext.second, p, x);
    cout << "Decrypted Message: " << decrypted_message << endl;
    
    // **Digital Signature**:
    pair<long long, long long> signature = elgamal_sign(m, p, g, x, k);
    cout << "Digital Signature: (r = " << signature.first << ", s = " << signature.second << ")\n";
    
    // **Signature Verification**:
    bool is_valid = elgamal_verify(signature.first, signature.second, m, p, g, y);
    cout << "Signature is " << (is_valid ? "Valid" : "Invalid") << endl;

    return 0;
}
