#include <bits/stdc++.h>
#include<fstream>
using namespace std;
long long totalchars;
long double compressedFileSize;

int freq[256], size=0;      //actual number of elements stored in the freq array
char arr[256];
string codes;
int num = 254;
char chh = char(num);	// 'â– ' is a special value for internal nodes not used

map<char,string> Dictionary;

// A Huffman tree node(from GFG)
struct MinHeapNode {

	// One of the input characters
	char data;

	// Frequency of the character
	unsigned freq;

	// Left and right child
	MinHeapNode *left, *right;

	MinHeapNode(char data, unsigned freq)

	{

		left = right = NULL;
		this->data = data;
		this->freq = freq;
	}
};

// For comparison of
// two heap nodes (needed in min heap)
struct compare {

	bool operator()(MinHeapNode* l, MinHeapNode* r)

	{
		return (l->freq > r->freq);
	}
};

// Prints huffman codes from
// the root of Huffman Tree.
void FillDictionary(struct MinHeapNode* root, string str)
{

	if (!root){
		return;
    }
	if (root->data != chh )
	{
        // cout << root->data << ": " << str << "\n";
        Dictionary[root->data]=str;
		if(root->data == ' ')
		{
			codes = codes + "_" + " " + str + "\n";
		}
		else
			codes = codes + (root->data) + " " + str + "\n";
    }

	FillDictionary(root->left, str + "0");
	FillDictionary(root->right, str + "1");
}

// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
void BuildHuffmanCodes(char data[], int freq[], int size)
{
	struct MinHeapNode *left, *right, *top;

	// Create a min heap & inserts all characters of data[]
	priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

	for (int i = 0; i < size; ++i){
		minHeap.push(new MinHeapNode(data[i], freq[i]));
    }
	// Iterate while size of heap doesn't become 1
	while (minHeap.size() != 1) {

		// Extract the two minimum freq items from min heap
		left = minHeap.top();
		minHeap.pop();

		right = minHeap.top();
		minHeap.pop();

		// Create a new internal node with
		// frequency equal to the sum of the
		// two nodes frequencies. Make the
		// two extracted node as left and right children
		// of this new node. Add this node
		// to the min heap chh is a special value
		// for internal nodes, not used
		top = new MinHeapNode(chh, left->freq + right->freq);

		top->left = left;
		top->right = right;

		minHeap.push(top);
	}

	// Print Huffman codes using the Huffman tree built above
	FillDictionary(minHeap.top(), "");
}


void fillFreqArray(ifstream & fin){
    char ch;
    map<char,int> m;
    while(fin.get(ch)){
        // fin>>ch;
		totalchars++;
        // cout<<ch;
        m[ch]++;
    }
	totalchars++;
	m[(char)129]++;	// For adding pseudo-EOF in actual_frequency & ASCII_values arrays
    // int i=0;
    for(auto it: m){
        arr[size]=it.first;
        freq[size]=it.second;
        size++;        
    }
	
}

class BitwiseWrite
{
	private:
	  char buff;
	  int totbits;
	  ostream& out;
	public:


	BitwiseWrite(ostream & os) : out(os),buff(0),totbits(0)
	{
								//clear buffer and bit counter
	}

	
	void flush();   	 // write buffer into output file and clear it 
	void writeBit(char i);
};



void BitwiseWrite::flush() {
  out.put(buff);
  out.flush();
  buff = totbits = 0;
}

void BitwiseWrite::writeBit(char i) // Write the least significant bit of the argument to the bit buffer
{
	if( i == char(129))     //for last bit if buffer is not full still flush buffer
	{
		flush();
	}
	if(totbits == 8) //if buffer full(8 bit) clear buffer
	{
		flush();
	}
	
	//if the int is 1, we must put the bit 1 into the buffer
	unsigned char mask = 1;
	if(i == '1') 
	{
		mask = mask << (7 - totbits);
		buff = buff | mask;
	}
	//if the int is 0, we must put the bit 0 into the buffer
	if(i == '0') 
	{
		mask = mask << (7 - totbits);
		mask = ~mask;
		buff = buff & mask;
	} 
	totbits++;                   //increment totbits++
}

void writeIntoFile(ifstream &fin, ofstream &fout){
    fin.clear();
	fin.seekg(0);
	char ch,bit;
	string code;
	BitwiseWrite s(fout);
	long long charactersdone=0;
	long double progress;
        int barWidth = 100;
 
	cout<<"\n\t\tFile compression initiated!\n\n";

	while(fin.get(ch)){
		// cout<<ch;
		charactersdone++;
		code=Dictionary[ch];
		compressedFileSize+=code.length();
		stringstream str(code);
		while(str>>bit){
			s.writeBit(bit);
			// cout<<bit;
		}
        cout << "[";

		int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) cout << "*";
            else if (i == pos) cout << ">";
            // else std::cout << " ";
			else break;
        }
        cout << "] " << (progress * 100.0) << " %\r";
        cout.flush();
		progress=(long double)charactersdone/totalchars;
 
	}
	ch=(char)129;
	code=Dictionary[ch];
	compressedFileSize+=code.length();
		stringstream str(code);
		while(str>>bit){
			s.writeBit(bit);
			// cout<<bit;
		}
	// s.flush();
	s.writeBit((char)129);
	cout<<"Compression Successful!";
	for (int i = 0; i < barWidth; ++i) {
        cout << " ";
	}
	cout.flush();

}

void writeToDictionary()     //creates codes file
{
	ofstream out;
	out.open("DictionaryUsed.txt");
	for(int i=0; i < codes.size();i++){
		if(codes[i] != 'Î')
		{
			out<<codes[i];
			
		}
		else
			out<<endl;
	}
	out.close();
} 

void cal_compressionRatio()
{
	totalchars-=1;//eof was extra
	compressedFileSize/=8;

	long double compressratio;

	cout<<"\nSize of original File  = "<<totalchars<<" Bytes";
	cout<<"\nSize of Compressed File = "<<compressedFileSize<<" Bytes";
	compressratio = ((long double)compressedFileSize/totalchars)*100;
	cout<<"\nCompression Ratio : "<<compressratio<<"% !\n\n";
}

int main()
{    
    string fname;   //string fname="sample.txt";
	cout << "\nEnter the Name of Text file to be compressed (with extention): ";
    cin>>fname;
    ifstream fin;
    fin.open(fname);

    fillFreqArray(fin);

	BuildHuffmanCodes(arr, freq, size);

	writeToDictionary();

    fname.replace(fname.size()-4,4,".cmp");
    ofstream fout;
  	fout.open(fname,ios::binary);

	writeIntoFile(fin,fout);

    cout<<"\n\nCompressed file saved as "<<fname<<" \nIts dictionary is saved as DictionaryUsed.txt in the same folder\n";
	
	cal_compressionRatio();

	fin.close();
	fout.close();

	return 0;
}
