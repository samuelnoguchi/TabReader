#include <fstream>
#include <cstdlib>
#include <iostream>

using namespace std;

const int MAX_SIZE = 10000;

bool getTab(string &e, string &B, string &G, string &D,
		string &A, string &E, string (&tuning)[6], string fileName, ifstream & fin);

void array_init(char e[],char B[], char G[], char D [], char A[],
		 char E[]);
		 
void string2array(char are[],char arB[], char arG[], char arD [], char arA[], 
		char arE[], string e, string B, string G, string D, string A, string E);

void encodeArray(char ar[]);	

char encodeNums(int value);	

string array2string(char ar[]);

void string_init(string &e, string &B, string &G, string &D,string &A, 
		string &E, string rawe, string rawB, string rawG, string rawD, 
		string rawA, string rawE, char are[],char arB[], char arG[], 
		char arD [], char arA[], char arE[]);
		
string markBarLines(string line);	

void findBlanks(char are[],char arB[], char arG[], 
		char arD [], char arA[], char arE[]);
	
string removeBlanks(string line);		

void mergeStrings(string topString, string midString, string lowString, 
		string & final, bool top);	
		
char stringConversion(char note, char string);

int decodeNote(char note);		
				 
int main() //program assumes tablature is written correctly
{
		cout << "Enter name of file:" << endl;
		string fileName;
		cin >> fileName;
		
		ifstream fin(fileName.c_str());	
		
		if(!fin)
	{
		cout << "File not Found";
		return EXIT_FAILURE;
	}

	string rawe = "|", rawB = "|", rawG= "|", rawD= "|", rawA= "|", rawE= "|";
	string tuning [6];

	getTab(rawe, rawB, rawG, rawD, rawA, rawE, tuning, fileName, fin);
	
	char are[MAX_SIZE], arB[MAX_SIZE], arG[MAX_SIZE], arD[MAX_SIZE], arA[MAX_SIZE], arE[MAX_SIZE];
	
	string e, B, G, D, A, E;
	
	string_init(e,B,G,D,A,E,rawe,rawB,rawG,rawD,rawA,rawE,are, arB, arG,arD,arA,arE);
	
	//cout << e<<endl<<B<<endl<<G<<endl<<D<<endl<<A<<endl<<E <<endl;
	
	string outA, outB;
	
	mergeStrings(e, B, G, outB, true);
	mergeStrings(D, A, E, outA, false);
	
	//cout << endl << outB << endl << outA;
	ofstream foutA("Play_me_A.txt");
	ofstream foutB("Play_me_B.txt");
	
	foutA << outA;
	foutB << outB;	
	
	cout << endl<<  "Tab conversion successfull";		
	return EXIT_SUCCESS;
}

bool getTab(string &e, string &B, string &G, string &D,
		string &A, string &E, string (&tuning)[6],string fileName, ifstream & fin)
	{
	char note;
	int stringnum = 0;
	int stringPitch =0;
	
	while(fin>>note)
	{	
		if(note == '[')
		{
			while(note!=']') //ignore [intro] completely
			{
				fin>>note;
			}
			fin>>note;
		}
	
		if(stringPitch < 6) //get string pitch
		{
			while(note!='|')
			{
				tuning[stringPitch] += note;
				fin>>note;
			}
			stringPitch++;			
		}
		else
		{		
			fin>>note;
		}
		
		if(note=='|')
		{
			string str = "";
			getline(fin,str);
		
			switch(stringnum+1) 
			{
				case 1 : e += str;
				break;
				case 2 : B += str;
			    break;
			    case 3 : G += str;
			    break;
			    case 4 : D +=  str;
			    break;
			    case 5 : A +=  str;
			    break;
			    case 6 : E += str;
			    break;
			}
				
			if(stringnum!=5) //next string
			{	
				stringnum++;				
			}
			else
			{
				stringnum = 0;
			}				
		}	
	}
	return true;				
}

void array_init(char e[],char B[], char G[], char D [], char A[], char E[])
{
	for(int place=0; place< MAX_SIZE; place++)
	{
		e[place]=B[place]=G[place]=D[place]=A[place]=E[place]='#';
	}
}

void string2array(char are[],char arB[], char arG[], char arD [], char arA[], 
		char arE[], string e, string B, string G, string D, string A, string E)
{
	for(int place=0; place<e.length(); place++)
	{
	int position =place; //compiler issue
	are[position]=e[position];
	arB[position]=B[position];
	arG[position]=G[position];
	arD[position]=D[position];
	arA[position]=A[position];
	arE[position]=E[position];	
	}
}	

void encodeArray(char ar[])
{
	//0 is 48, 9 is 57
	int position=0;
	while(ar[position]!='#')
	{
		if(ar[position]>=48&&ar[position]<=57)//if position is number
		{
			string note="";
			note=ar[position];
			
			if(ar[position+1]>=48&&ar[position+1]<=57) 
			{
				note += ar[position+1];
				ar[position+1] ='%';	
			}
		
		int noteValue = atoi(note.c_str());	
		ar[position] = encodeNums(noteValue);
		
		}
		position++;
	}
}

char encodeNums(int value)
{
	char note;
	// A is 65, B is 66 ...
	if(value>0&&value<=12)
		note = (char)value + 64;
	else if (value>12&&value<=24)
		note = (char)value +52;
	else if (value == 0)
		note = 'L';		
	else
		note = '-';
	return note;	
}

string array2string(char ar[])
{
	int count = 0;
	string notes ="";
	while(ar[count]!='#')
	{
		notes +=ar[count];
		count++;
	}
	
	return notes;
}

void string_init(string &e, string &B, string &G, string &D,string &A, 
		string &E, string rawe, string rawB, string rawG, string rawD, 
		string rawA, string rawE, char are[],char arB[], char arG[], 
		char arD [], char arA[], char arE[])
{
	array_init(are, arB, arG, arD, arA, arE);
	string2array(are, arB, arG, arD, arA, arE, rawe, rawB, rawG, rawD, rawA, 
	rawE);

	encodeArray(are);
	encodeArray(arB);
	encodeArray(arG);
	encodeArray(arD);
	encodeArray(arA);
	encodeArray(arE);
	
	findBlanks(are,arB,arG,arD,arA,arE);
			
	e = removeBlanks(markBarLines(array2string(are)));
	B = removeBlanks(markBarLines(array2string(arB)));
	G = removeBlanks(markBarLines(array2string(arG)));
	D = removeBlanks(markBarLines(array2string(arD)));
	A = removeBlanks(markBarLines(array2string(arA)));
	E = removeBlanks(markBarLines(array2string(arE)));
}

string markBarLines(string line)
{
	for(int count =0; count <line.length()-1;count++)
	{
		if(line[count]=='|')
			line[count] ='%';
	}
	return line;
}

void findBlanks(char are[],char arB[], char arG[], 
		char arD [], char arA[], char arE[])
{
	int count=0;
	while(are[count]!='#')
	{
		if(are[count]=='%'||arB[count]=='%'||arG[count]=='%'||arD[count]=='%'||
				arA[count]=='%'||arE[count]=='%')
		{
			are[count]=arB[count]=arG[count]=arD[count]=arA[count]=arE[count]='%';
		}	
	count++;		
	}
}
string removeBlanks(string line)
{
	string lineRemoved="";
	for(int count =0; count<line.length();count++)
	{
		if(line[count]!='%')
		{
			lineRemoved+=line[count];
		}
	}
	return lineRemoved;
}

// --The following code pertains only to the current 2 string prototype-------
//--Its role is to convert notes on other strings to the two we are using

void mergeStrings(string topString, string midString, string lowString, 
		string & final, bool top)
{
	for(int place = 0; place < midString.length(); place++)
	{
		char stringName;
		if(midString[place]=='-')      
		{
			if(lowString[place]!='-')
			{
				if(top ==true)
					stringName = 'G';	
				else
					stringName = 'E';	
						
				final+=stringConversion(lowString[place], stringName);	
			}
			else if(topString[place]!='-')
			{
				if(top ==true)
					stringName = 'e';	
				else
					stringName = 'D';	
				
				final+=stringConversion(topString[place], stringName);
			}
			else
			{
				final+='-';
			}
		}
		else
		{
			final+=midString[place];
		}	
	}
}

char stringConversion(char note, char string)
{
	//this function deals primarly with the nuances in guitar standard tuning
	if(string == 'e' || string == 'D')
	{
		int noteValue = decodeNote(note)+5;
		return encodeNums(noteValue);
	}
	else if(string == 'G')
	{
		int noteValue = decodeNote(note)+8;
		return encodeNums(noteValue);
	}
	else
	{
		int noteValue = decodeNote(note)+7;
		return encodeNums(noteValue);
	}
}

int decodeNote(char note) //turns letteres A - L into frets 1- 12
{
	//A is 65, B is 66..
	return (int)note -64;	
}


