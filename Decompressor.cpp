#include <iostream>
#include <fstream>
#include<stdlib.h>
#include <cstring>
#include <iomanip>

using namespace std;
string output;
int count = 0;

class BitwiseRead {
private:
  char buf;
  //int first;
  int nbits;        //number of bits have been read from the buffer
  istream& in;
public:

  /* Initialize a BitwiseRead that will use the given istream for input */
  
  BitwiseRead(istream & is);

  /* Fill the buffer from the input */
  void fill();
  
  /* Read the next bit from the bit buffer. Fill the buffer form the input
 * stream first if needed.
 * Return 1 if the bit read is 1, 0 if bit read is 0
 * */
  int readBit();
};

BitwiseRead::BitwiseRead(istream & is) : in(is) {
    buf = 0;
    nbits = 8;
}

void BitwiseRead::fill() 
{
	buf = in.get();
	nbits = 0;
}

int BitwiseRead::readBit() 
{
  if(nbits == 8) {
    fill();
 }
  
  //uses a mask to extract the nbits'th bit
  unsigned char mask = 1;
  mask = mask << (7-nbits); 
  mask = mask & buf;
  nbits++;
  if(mask == 0) {
    return 0;
  }
  else {
    return 1;
  } 
}

class HuffmanTree
{
	private:
	/*** Node structure ***/
	class node
	{
		public:
		char data;
		node * left,* right;
	
		node(char item)   // node constructor
		{
			data = item;
			left = right = 0;
		}
	};
	typedef node * nodePointer;
	public:
		HuffmanTree();
		void BuildDecodTree(ifstream & codeIn);
		void insert(char ch, string code);
		char decode(BitwiseRead &in);
		void printTree(ostream & out, nodePointer root, int indent);
		void displayDecodingTree(ostream & out);
		private:
		nodePointer root;
};

inline HuffmanTree::HuffmanTree()
{
	root = new node('*');
}

void HuffmanTree::BuildDecodTree(ifstream & codeIn)
{
	char ch;
	string code;
	for (;;)
	{
		if ( codeIn.eof() ) return;
		codeIn >> ch >> code;
			// cout<<"\n"<<ch<<" = "<<code;
			if(ch == '_')
			{
				ch = ' ';
			}
			insert(ch, code);
	}
}

void HuffmanTree::insert(char ch, string code)          //traverses each character from file from its HuffmanTree code      
{
	HuffmanTree::nodePointer p = root;
	for(int i = 0; i < code.length(); i++)
	{
		switch (code[i])
		{
		case '0' :
				if (p->left == 0) // create node along path
				p->left = new HuffmanTree::node('*');
				p = p->left;
				break;
		case '1' :
				if (p->right == 0) // create node along path
				p->right = new HuffmanTree::node('*');
				p = p->right;
				break;
		default:
			cerr << "*** Illegal character in code ***\n";
			exit(1);
		}
	}
	p->data = ch;
}

char HuffmanTree::decode(BitwiseRead &in)             //For Decoding process from the HuffmanTree  chart
{
	HuffmanTree::nodePointer p;
	p = root;
	int bit;
	while(true)
	{
		bit = in.readBit();
		//count++;
		if(bit == 1)
		{
			p = p -> right;
		}
		if(bit == 0)
		{
			p = p -> left;
		}
		if(p->right == NULL || p->left == NULL)
		{
			break;
		}
	}
	return (p->data);
}


int main()
{
	int total_bits_written;
	int current_bits = 0;
	char data;

	string filename;

	cout << "\nEnter name of DictionaryUsed file (default : DictionaryUsed.txt) ";
	cin >> filename;
	// filename="DictionaryUsed.txt";
	ifstream codestream(filename);
	if (!codestream.is_open())
	{
		cout <<"Error !!! Cannot open DictionaryUsed file.\n";
		exit(1);
	}
	HuffmanTree h;
	h.BuildDecodTree(codestream);
	cout << "\nEnter Name of Compressed file (with extention): ";
	cin >> filename;
	// filename="sample.cmp";
	ifstream in;
	ofstream fout;
	fout.open("Decompressed.txt");
	in.open(filename,ios::binary);
	BitwiseRead os(in);
	if (!in.is_open())
	{
		cout << "Error !!! Cannot open Compressed file.\n";
		exit(1);
	}
	cout<<"\nProcessing..... Plz Wait";
	char ch3;
	ch3=char(129);
	while(!in.eof())
	{
		
		data = h.decode(os);
		// cout<<data;
		if( data == ch3)
		{
			break;
		}
		current_bits++;
		if(!in.eof())
		fout << data;
	}
	//cout<<"\nCount = "<<count;
	cout<<"\n\nDecompressed file saved as Decompressed.txt!\n";
	
	return(0);
}