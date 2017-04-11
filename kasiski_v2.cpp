#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <math.h>
#include <fstream>
#include <ctime>

using namespace std;
int l=0,raw,keyL;
string block[100],oriblock[100];
string digit[50] = {"1","2","3","4","5","6","7","8","9","10",
	  				"11","12","13","14","15","16","17","18","19","20",
					"21","22","23","24","25","26","27","28","29","30","31","32","33"};
string result;
struct list{
	string kata;
	int count=1;
	int dist[50];
}lisber[50];

struct freq{
	char huruf;
	char plain='-';
	char key='-';
	int count;
}fremax[50];

char toUp(char ch)
{
	if(ch>=97&&ch<=122){
		ch-=32;
	}
	return ch;
}

string removeSpaces(string str)
{
    int i,count = 0;
    string str2;
    for (i = 0; i<str.length(); i++){
    	if (str[i] != ' '){
    		str[i] = toUp(str[i]);
            str[count++] = str[i];	
		}	
	}
    str[count] = '\0';
    str2=str.substr(0,count);
    return str2;
}

void caBer(string str,int wLength, int sLength)
{
	string temp;
	int flag=0,i,j,k,d,m,counter,idx[50];
	for(i=0;i<=sLength-wLength;i++){
		temp = str.substr(i,wLength);
		idx[0]=i;
		counter=1;
		for(j=0;j<l;j++){
			if(temp!=lisber[j].kata)	
				flag=0;
			else {
				flag=1;
				break;	
			}
		}
		if(flag==0){
			for(k=i+wLength;k<sLength;k++){
				if(temp==str.substr(k,wLength)){
					idx[counter]=k;
					counter++;
					//cout<< k << "=";
				}
			}
		}
		if(counter>1){
			lisber[l].kata = temp;
			lisber[l].count = counter;
			for(m=0;m<counter;m++){
				lisber[l].dist[m] = idx[m];
				//cout << lisber[l].dist[m] << " ";
			}	
			l++;	
		}
	}
}

int gcd(int a,int b) {
    int temp;
    while(b > 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int countGCD(){
	int fpb,j,i,idx_max,max=0;
	for(j=0;j<l;j++){
		if(lisber[j].count>=max){
			max=lisber[j].count;
			idx_max = j;
		}
	}
	fpb = lisber[idx_max].dist[0];
    for(i=0; i<lisber[idx_max].count-1; i++) {
        fpb = gcd(fpb, lisber[idx_max].dist[i+1]-lisber[idx_max].dist[i]);
        //cout << lisber[idx_max].dist[i] << lisber[idx_max].kata << "-";
    }
    cout << "GCD(";
    for(i=0; i<lisber[idx_max].count-1; i++) {
        cout << lisber[idx_max].dist[i+1]-lisber[idx_max].dist[i];
        if(i!=lisber[idx_max].count-2) cout << ",";
    }
	cout << ") = " << fpb << "\n";
    return fpb;
}

int buildBlock(string str, int keyL)
{
	int i,j,k=0,f,idx,max,huruf[30];
	raw = ceil(str.length()/keyL);
	//build tabel
	for(i=0;i<raw;i++){
		string tmp = "";
		for(j=0;j<keyL;j++){
			tmp += str[k];
			k++;
		}
		block[i] = tmp;
		oriblock[i] = tmp;
	}
	//cari max per column
	int maxi=0,maxidx;
	for(j=0;j<keyL;j++){
		max=0; idx=0;
		for(i=0;i<26;i++){
			huruf[i]=0;
		}
		for(i=0;i<raw;i++){
			huruf[block[i][j]-65]++;
		}
		for(i=0;i<26;i++){
			if(max<=huruf[i]){
				max=huruf[i];
				idx=i;
			}
		}
		fremax[j].huruf=idx+65;
		fremax[j].count=max;
		
		if(maxi<=fremax[j].count){
			maxi=fremax[j].count;
			maxidx=j;
		}
	}
	return maxidx;
	//cout << "ini fungsi buildBlock\n";
}

string decrypt(string ciphertext, string key)
{
	string plaintext,plain="";
	//cout << key <<"-";
	//cout << ciphertext <<"-";
	int j=0,k,length;
	length = ciphertext.length();
	for(int i=0;i<length;i++){
		k=(ciphertext[i]-65)-(key[j]-65);
		if(k>=0)
			plaintext[i]=(k%26)+65;
		else
			plaintext[i]=(k+26)+65;
		//cout << plaintext[i];
		plain+=plaintext[i];
		j++;	
		if(j==keyL) j=0;
	}
	//cout << "ya\n";
	//cout << plain <<"\n";
	return plain;
}

int isExist(string word)
{
	int len = word.length();
	//cout << "ex" << word << "\n";
	string filename = digit[len-1] + ".txt";
	string found,line;
	int flag=0;
	ifstream myfile(filename.c_str());
	if (myfile)
    {
    	while (getline( myfile, line ))
      	{	
		  	line = removeSpaces(line);
       		if(line==word)
       		{
       			//cout << line<< "-" << word <<"\n";
       			flag=1;
       			found=line;
       			break;
			}
      	}
    	myfile.close();
    }
    return flag;
}

int isEnglish(string plaintext)
{
	int i=0,j,idx,len = plaintext.length(), flag=0,flag2=0;
	//cout << plaintext <<len<< "hhha\n";
	string engword="";
	while(i<len)
	{
		//cout<<i;
		flag=0;
		for(j=1;j<=31&&i+j<=len;j++)
		{
			//cout << plaintext.substr(i,j)<< "\n";
			if(isExist(plaintext.substr(i,j))==1)
			{
				engword = plaintext.substr(i,j);
				//cout << engword << endl;
				idx=i+j;
				flag=1;
			}
		}
		//cout << engword <<"ha\n";
		if(flag)
		{
			result+=engword;
			result+=" ";
			i=idx;
			//break;		
		}
		else
		{
			//cout << "LOL\n";
			//cout << plaintext.substr(i,j) << endl;
			result="";
			break;
		}
	}
	return flag;
}

string toKey(char hrf)
{
	char e = 'E';
	string ch;
	int c =(hrf-65)-(e-65);
	if(c>=0)
		ch =(c%26)+65;
	else
		ch =(c+26)+65;
	return ch;
}

void findkey(int keyL, string cipher, int idxe)
{
	//cout << "halooo";
	string subk = toKey(fremax[idxe].huruf);\
	//cout << subk;
	string filename = digit[keyL-1] + ".txt";
	string line;
	ifstream myfile( filename.c_str() );
	if (myfile)
    {
    	//cout << "halooo";
    	while (getline( myfile, line ))
      	{	
      		line = removeSpaces(line);
			if(line.substr(idxe,1)== subk)
		  	{
       			//cout << line <<"-";
       			string plain = decrypt(cipher,line);
       			//cout << plain << "ini\n";
       			if(isEnglish(plain))
       			{
       				cout << "KEY: " << line << "\n";
       				cout << result << "\n";
					break;
				}
      	  	}
       		
      	}
    	myfile.close();
    }
}

int main()
{
	clock_t t1,t2;
	t1=clock();
	int idxe,i;
	string plain,cipher;
	cout << "MASUKKAN CIPHERTEXT:\n ";
	getline(cin,cipher);
	cipher = removeSpaces(cipher);
	for(int i=3;i<=6;i++){
		caBer(cipher,i,cipher.length());
	}
	cout << "KATA BERULANG\n";
	for(int j=0;j<l;j++){
		cout << "\t" << lisber[j].kata << " => " << lisber[j].count << " KALI MUNCUL. PADA POSISI ";
		for(i=0; i<lisber[j].count; i++) {
        	cout << lisber[j].dist[i];
        	if(i!=lisber[j].count-1) cout << ",";
        	else cout << "\n";
    	}		
	}
	keyL=countGCD();
	idxe = buildBlock(cipher,keyL);	
	findkey(keyL,cipher,idxe);
	t2=clock();
	float diff = (float)t2-(float)t1;
	cout <<"exec time: "<< diff/CLOCKS_PER_SEC << " detik\n";
	return 0;
}
