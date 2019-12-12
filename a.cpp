#include<bits/stdc++.h>
using namespace std;

int main(){
	string s;
	int cont = 0;
	int modulo = 8;
	while(getline(cin,s)){
		cont++;
		if(cont%modulo == 0){
			if(modulo == 8) modulo++;
			cont=0;
			stringstream ss;

			ss << s;

			string s2;

			ss >> s2;

			cout << s2 << endl;
		}
	}
}