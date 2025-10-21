#include<bits/stdc++.h>
using namespace std;


int encry(int mes ,int key){
    int result=mes^key;
    return result;

}
int decry(int mes ,int key){
    int result=mes^key;
    return result;
}
int main(){
    int mess,key;
    cout<<"Enter message to encrypt (as integer): ";
    cin>>mess;
    cout<<"Enter key (as integer): ";
    cin>>key;
    int encrypted=encry(mess,key);
    cout<<"Encrypted message: "<<encrypted<<endl;
    int decrypted=decry(encrypted,key);
    cout<<"Decrypted message: "<<decrypted<<endl;
    return 0;
    

    
}