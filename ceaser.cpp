#include<bits/stdc++.h>
using namespace std;
string encrypt(string mes, int x){

string result="";
for(int i=0;i<mes.length();i++){
    if(isupper(mes[i])){
        char a=char((((int(mes[i])+x)-65)%26+65));
        result+=a;

    }
    else{
        char a=char((((int(mes[i])+x)-97)%26+97));
        result+=a;

    }
}
return result;
}

int main()
{
    string text;
    int s;
    cout<<"Enter text to encrypt: ";
    getline(cin, text);
    cout<<"Enter shift: ";
    cin>>s;
    cout << "Encrypted text: " << encrypt(text, s) << endl;
    return 0;
}
