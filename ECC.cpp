#include <iostream>
#include <cmath>
#include <utility>

using namespace std;

long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}


pair<long long, long long> elliptic_add(long long x1, long long y1, long long x2, long long y2, long long p, long long a) {
    long long m = 0;
    if (x1 != x2) {
        m = ((y2 - y1) * mod_exp(x2 - x1, p - 2, p)) % p;  // Point addition formula
    } else {
        m = ((3 * x1 * x1 + a) * mod_exp(2 * y1, p - 2, p)) % p;  // Point doubling formula
    }

    long long x3 = (m * m - x1 - x2) % p;
    long long y3 = (m * (x1 - x3) - y1) % p;

    return make_pair((x3 + p) % p, (y3 + p) % p);  
}


pair<long long, long long> elliptic_multiply(long long k, long long x, long long y, long long p, long long a) {
    pair<long long, long long> result = make_pair(0, 0);  // The identity point (at infinity)
    pair<long long, long long> temp = make_pair(x, y);    // The point to multiply

    while (k > 0) {
        if (k % 2 == 1) {
            result = elliptic_add(result.first, result.second, temp.first, temp.second, p, a);
        }
        temp = elliptic_add(temp.first, temp.second, temp.first, temp.second, p, a);  // Point doubling
        k /= 2;
    }

    return result;
}


pair<long long, long long> ecc_keygen(long long p, long long a, long long b, long long Gx, long long Gy, long long d) {
    pair<long long, long long> public_key = elliptic_multiply(d, Gx, Gy, p, a);
    return public_key;
}

// ECC Encryption (Message M is a point, encrypt with public key)
pair<long long, long long> ecc_encrypt(long long Mx, long long My, long long p, long long a, long long Gx, long long Gy, long long k, long long public_key_x, long long public_key_y) {
    // Compute C1 = kG and C2 = M + k * P
    pair<long long, long long> C1 = elliptic_multiply(k, Gx, Gy, p, a);
    pair<long long, long long> kP = elliptic_multiply(k, public_key_x, public_key_y, p, a);
    pair<long long, long long> C2 = elliptic_add(Mx, My, kP.first, kP.second, p, a);

    return make_pair(C1.first, C2.first);  // Return ciphertext (C1, C2)
}

// ECC Decryption (Decrypt using private key)
pair<long long, long long> ecc_decrypt(pair<long long, long long> ciphertext, long long p, long long a, long long d) {
    long long C1x = ciphertext.first;
    long long C2x = ciphertext.second;

    // Compute kP = d * C1
    pair<long long, long long> kP = elliptic_multiply(d, C1x, C2x, p, a);

    // Recover the message point
    long long Mx = (C2x - kP.first + p) % p;

    return make_pair(Mx, 0);  // Return the decrypted message (Mx, My)
}

int main() {
    // Example of ECC with small prime p
    long long p = 23;  // A small prime number for modulus
    long long a = 1;   // Coefficient a for the curve equation
    long long b = 1;   // Coefficient b for the curve equation

    // Generator point (Gx, Gy)
    long long Gx = 3;
    long long Gy = 10;

    // Private key (random integer)
    long long d = 6;

    // Generate public key (dG)
    pair<long long, long long> public_key = ecc_keygen(p, a, b, Gx, Gy, d);
    cout << "Public Key: (" << public_key.first << ", " << public_key.second << ")\n";
    
    // Message to be encrypted (Mx, My)
    long long Mx = 7;
    long long My = 12;

    // Random integer k for encryption
    long long k = 8;

    // Encrypt the message using the public key
    pair<long long, long long> ciphertext = ecc_encrypt(Mx, My, p, a, Gx, Gy, k, public_key.first, public_key.second);
    cout << "Ciphertext: (C1 = " << ciphertext.first << ", C2 = " << ciphertext.second << ")\n";

    // Decrypt the ciphertext using the private key
    pair<long long, long long> decrypted_message = ecc_decrypt(ciphertext, p, a, d);
    cout << "Decrypted Message: (" << decrypted_message.first << ", " << decrypted_message.second << ")\n";

    return 0;
}











///


#include <iostream>
 
using namespace std;
 
struct Point { int x, y; bool infinity; };
 
// Simple modulo operation
int mod(int a, int p) {
    int r = a % p;
    if (r < 0) r += p;
    return r;
}
 
// Returns gcd(a,b) and sets x such that a*x ≡ 1 mod p
int gcdExtended(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int g = gcdExtended(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
 
// Modular inverse using recursive EEA
int modInverse(int a, int p) {
    int x, y;
    int g = gcdExtended(a, p, x, y);
    if (g != 1) throw runtime_error("No inverse exists");
    return (x % p + p) % p; // ensure positive
}
 
// Add two points on curve: y^2 = x^3 + ax + b (mod p)
Point addPoints(Point P, Point Q, int a, int p) {
 
    if (P.infinity) return Q;
 
    if (Q.infinity) return P;
 
    if (P.x == Q.x && mod(P.y + Q.y, p) == 0) return {0, 0, true}; // P == -Q
 
    int s;
 
    if (P.x == Q.x && P.y == Q.y) {
        // point doubling
        int num = mod(3 * P.x * P.x + a, p);
        int den = modInverse(2 * P.y, p);
        s = mod(num * den, p);
    } else {
        // point addition
        int num = mod(Q.y - P.y, p);
        int den = modInverse(mod(Q.x - P.x, p), p);
        s = mod(num * den, p);
    }
 
    int xr = mod(s*s - P.x - Q.x, p);
    int yr = mod(s*(P.x - xr) - P.y, p);
 
    return {xr, yr, false};
}
 
 
 
// Negate a point
Point negatePoints(Point P, int p) {
    if (P.infinity) return P;
    return {P.x, mod(-P.y, p), false};
}
 
// Subtract points
Point subtract(Point P, Point Q, int a, int p) {
    return addPoints(P, negatePoints(Q, p), a, p);
}
 
// Scalar multiplication k*P (very basic)
Point scalarMultiply(int k, Point P, int a, int p) {
    if(k==0) return {0,0,true};
    if(k==1) return P;
    Point result={0,0,true};
    Point addend=P;
    while(k>0){
        if(k%2==1)
        {
 
            +++
            result=addPoints(result,addend,a,p);
        }
        addend=addPoints(addend,addend,a,p);
        k/=2;
    }
 
 
    return result;
}// ----------- ECC Encryption/Decryption ----------
struct Ciphertext { Point C1; Point C2; };
 
Ciphertext encrypt(Point M, Point Q, Point G, int k, int a, int p) {
    // ECC encryption: C1 = kG, C2 = M + kQ
 
    Point C1=scalarMultiply(k,G,a,p);
    Point kQ=scalarMultiply(k,Q,a,p);
    Point C2=addPoints(M,kQ,a,p);
 
    return {C1, C2};
}Point decrypt(Ciphertext ct, int d, int a, int p) {
    // ECC decryption: M = C2 - dC1
    Point dC1=scalarMultiply(d,ct.C1,a,p);
    return subtract(ct.C2, dC1, a, p);
}
 
int main() {
    // Curve: y^2 = x^3 + ax + b (mod p)
    int p = 97, a = 2, b = 2;
    Point G = {5, 1, false};  // base point
 
    cout << "Curve: y^2 = x^3 + 2x + 2 (mod 97)\n";
    cout << "Base point G = (" << G.x << "," << G.y << ")\n";
 
    // Bob's key
    int d = 6; // private key
 
    Point Q = scalarMultiply(d,G,a,p); // public key Q = dG
 
    cout << "Bob's public key PB = (" << Q.x << "," << Q.y << ")\n";
 
    // Message as a point
    Point M = {6, 3, false};
    cout << "Message point M = (" << M.x << "," << M.y << ")\n";
 
    // Alice encrypts
    int k = 5; // ephemeral key
 
    Ciphertext ct = encrypt(M,Q,G,k,a,p);
 
    cout << "Ciphertext:" << endl;
    cout << "C1 = (" << ct.C1.x << "," << ct.C1.y << ")\n";
    cout << "C2 = (" << ct.C2.x << "," << ct.C2.y << ")\n";
 
    // Bob decrypts
    Point decrypted = decrypt(ct, d, a, p);
    cout << "Decrypted M' = (" << decrypted.x << "," << decrypted.y << ")\n";
 
    if (decrypted.x == M.x && decrypted.y == M.y)
        cout << "Decryption successful\n";
    else
        cout << "Decryption failed\n";
 
    return 0;
}