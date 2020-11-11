#include <bits/stdc++.h>
using namespace std;
struct Tree
{
    int frequency;
    unsigned char character;
    Tree *left = NULL;
    Tree *right = NULL;
};
//to compare the frequencies 
class TreeComparator
{
  public:
    bool operator()(Tree *a, Tree *b)
    {
        return a->frequency > b->frequency;
    }
};
//return the max sum root by adding the min 2 freq. 
Tree *buildHuffmanTree(vector<pair<unsigned char, int> > freqtable)
{

    priority_queue<Tree *, vector<Tree *>, TreeComparator> huffqueue; // min priority queue
    //priority_queue<Tree *, vector<Tree *>, TreeComparator> huff;
    
    for (int i = 0; i < freqtable.size(); i++)
    {
        Tree *node = new Tree();
        node->frequency = freqtable[i].second;
        node->character = freqtable[i].first;
       //huff.push(node);
        huffqueue.push(node);
    }
    /*
	while(huff.size()!=0)
	{
		cout<<huff.top()->frequency<<" "<<huff.top()->character<<endl;
		huff.pop();
	}
    */

    while (huffqueue.size() > 1)
    {
        Tree *a, *b;
        a = huffqueue.top();
        huffqueue.pop();
        
        b = huffqueue.top();
        huffqueue.pop();
        Tree *c = new Tree();
        c->frequency = a->frequency + b->frequency;
        c->left = a;
        c->right = b;
        huffqueue.push(c);
    }

    return huffqueue.top();
}

string toBinary(unsigned  char a)
{
    string output  = "";
    while(a!=0)
    {
        string bit = a%2==0?"0":"1";
        output+=bit;
        a/=2;
    }

    if(output.size()<8)
    {
        int deficit = 8 - output.size();
        for(int i=0; i<deficit; i++)
        {
            output+="0";
        }
    }

    reverse(output.begin(), output.end());
    return output;
    
}


/*
	assigning 1 to right node and 0 to left node and saving path in prev and printing it when we reach character.
	And every char has a unique path so it wont have any path leading to it 
*/
void traverseHuffmanTree(Tree *root, string prev, string toAppend, map<unsigned char, string> &codemap)
{

    prev+=toAppend;
    
    if (root->right == NULL && root->left == NULL)
    {
        cout<<root->character<<" "<<prev<<endl;   
        codemap[root->character] = prev;
    }
    if (root->right != NULL)
    {
        traverseHuffmanTree(root->right, prev, "1", codemap);
    }

    if (root->left != NULL)
    {
        traverseHuffmanTree(root->left, prev, "0", codemap);
    }
}

unsigned char *readFileIntoBuffer(char *path, int &sz)
{
    FILE *fp = fopen(path, "r");
    sz = 0;
    //pointer moves to EOF
    fseek(fp, 0, SEEK_END);
    //stores the last position
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char *buffer = (unsigned char *)malloc(sz);
    //used for storing a file
    fread(buffer, 1, sz, fp);
    return buffer;
}

void writeFileFromBuffer(char *path, unsigned char *buffer, int sz, int flag)
{
    FILE *fp;
    if(flag==0)
    {
        fp = fopen(path, "wb");
    }

    else{
        fp = fopen(path, "ab");
    }
   
    fwrite(buffer, 1, sz, fp);

    fclose(fp);
}


vector<pair<unsigned char, int> > convertToVector(map<unsigned char, int> codes)
{
    vector<pair<unsigned char, int> > codesV;

    for (map<unsigned char, int>::iterator i = codes.begin(); i != codes.end(); i++)
    {
        codesV.push_back(make_pair(i->first, i->second));
    }

    return codesV;
}

string getHuffmanBitstring(unsigned char *buffer, map<unsigned char, string> codes, int sz, int& paddedBits)
{
    string outputBuffer="";
    for(int i=0; i<sz; i++)
    {
        outputBuffer=outputBuffer+codes[buffer[i]];
    }
	//CONVERT EACH CHAR IN BUFFER TO BINARY BITSTRINGS

    if(outputBuffer.size()%8!=0)
    {
    	
        int deficit = 8*((outputBuffer.size()/8)+1)-outputBuffer.size();
        cout<<"outputBuffer.size()  "<<outputBuffer.size()<<endl;
        cout<<"deficit  "<<deficit<<endl;
        paddedBits = deficit;
        for(int i=0; i<deficit; i++)
        {
            outputBuffer+="0";
        }
    }

    //Instead of adding zeroes to the end, add zero to the begining of the last byte

    return outputBuffer;
    
}
/*
 file sizes are always measured in bytes and one byte is equal to 8 bits so arring bits to form one byte.

*/
unsigned char* getBufferFromString(string bitstring, vector<unsigned char>&outputBuffer, int& sz)
{
    int interval = 0;
   	unsigned char bit = 0;
    //cout<<"bit string 0 :::::"<<bitstring<<endl;
	//bit = (bit<<1)|(bitstring[0]-'0');
	//cout<<"heeeeeelllllllxxxxx"<<(int)bit<<endl;
    for(int i=0; i<sz; i++)
    {	//left shifting and or of each bit
         bit = (bit<<1)|(bitstring[i]-'0');
         
        interval++;
        if(interval==8)
        {
            interval = 0;
            outputBuffer.push_back(bit);
            bit = 0;
        
        }   
    }
    sz = outputBuffer.size();
    //.data(); its a pointer to the first element of the output buffer
    return outputBuffer.data();
}
//converting unsigned char buffer to  string(in binary)
string getStringFromBuffer(unsigned char* buffer, int sz)
{
    string bitstring = ""; 
    for(int i=0; i<sz; i++)
    {
        bitstring+=toBinary(buffer[i]);
     
    }

    return bitstring;
}

/*
	mapping of binary code to char(reverse)
*/
unsigned char* getDecodedBuffer(string bitstring, vector<unsigned char>&buffer, map<unsigned char, string> codes, int &sz, int paddedBits)
{
    string bit = "";
    map<string, unsigned char> reversecodes;
    
    for(map<unsigned char, string>::iterator i = codes.begin(); i!=codes.end(); i++)
    {
        reversecodes[i->second] = i->first;
    }
	//converting binary bit code to char
    for(int i=0; i<bitstring.size()-paddedBits; i++) //searching each binary bit code in map for its corresponding char if found then pushing it into the buffer
    {
        bit+=string(1, bitstring[i]);
        if(reversecodes.find(bit)!=reversecodes.end())
        {
           buffer.push_back(reversecodes[bit]);
           bit = "";
        }
    }

    sz = buffer.size();
    return buffer.data();
}


void writeHeader(char* path,map<unsigned char, string> codes,  int paddedBits){
    
    int size = codes.size();
    //cout<<"paddedBits  "<<paddedBits<<endl;
    writeFileFromBuffer(path, (unsigned char*)&paddedBits, sizeof(int), 0); //adding number of padded bits and size of the file. 
    writeFileFromBuffer(path, (unsigned char*)&size, sizeof(int), 1);
    char nullBit = '\0';
    for(map<unsigned char, string>::iterator i = codes.begin(); i!=codes.end(); i++)
    {
        writeFileFromBuffer(path, (unsigned char*)&i->first, 1, 1); //adding particular char to the file
        int len = i->second.size();
        writeFileFromBuffer(path, (unsigned char*)&len, sizeof(int), 1); //adding the length of the code
        writeFileFromBuffer(path, (unsigned char*)i->second.c_str(), i->second.size(), 1); //adding the code
    }
}

/*
	used to read number of padded bits, size of coded file, characters and their binary codes
*/
unsigned char* readHeader(unsigned char* buffer, map<unsigned char, string> &codes, int& paddedBits, int &sz)
{
   paddedBits = *((int*)buffer);
   cout<<paddedBits<<"PADDED"<<endl;
   cout<<"BUFFER1  "<<*((int*)buffer)<<endl;
   buffer = buffer+4; //moving pointer position by 4 bits to point it 
   cout<<"BUFFER2  "<<*((int*)buffer)<<endl;
   sz-=4;
   int size = *((int*)buffer);
   buffer = buffer+4;
   sz-=4;
   for(int i=0; i<size; i++)
   {    
       unsigned char key = buffer[0];
       buffer++;
       sz--;
       int len = *((int*)buffer);
       buffer+=4;
       sz-=4;
       char* value = (char*)malloc(len+1);

       for(int j = 0; j<len; j++)
       {
           value[j]=buffer[j];
       }
    //    value = (char*)buffer;
       buffer+=len;
       sz-=len;
       value[len]='\0';
       codes[key] = value;
       cout<<key<<" "<<value<<endl;
   }

   return buffer;
}



//add amount padded 
void compressFile(char *path, char *output_path, map<unsigned char, string> &codes)
{
    int sz = 0;
    int paddedBits = 0;// bits needed to make the file divisible by 8
    map<unsigned char, int> freqtable;
    unsigned char *buffer = readFileIntoBuffer(path, sz);
    for (int i = 0; i < sz; i++)
    {
            freqtable[buffer[i]]++;
    }
    Tree *root = buildHuffmanTree(convertToVector(freqtable));
    cout<<root<<endl;
    traverseHuffmanTree(root, "", "", codes);
    //---------------------------------------------------------------
    string outputString = getHuffmanBitstring(buffer, codes, sz, paddedBits);
    //---------------------------------------------------------------
    sz  = outputString.size();
    vector<unsigned char> outputBufferV;
    unsigned char* outputBuffer=getBufferFromString(outputString, outputBufferV, sz);
    //unsigned char* outputBuffer = outputBufferV.data();
    writeHeader(output_path, codes, paddedBits); //adding padded bits to the file.
    writeFileFromBuffer(output_path, outputBuffer, sz, 1); //adding all the binary bits to the file.
}

void decompressFile( char* inputPath,  char* outputPath)
{
    int sz = 0;
    map<unsigned char, string> codes;
    int paddedBits = 0; // bits needed to make the file divisible by 8
    unsigned char* fileBuffer = readFileIntoBuffer(inputPath, sz);
    fileBuffer = readHeader(fileBuffer, codes, paddedBits, sz);
    string fileBitString = getStringFromBuffer(fileBuffer, sz);
    // cout<<fileBitString<<endl;
    vector<unsigned char> outputBufferV;
    unsigned char* outputBuffer;
    getDecodedBuffer(fileBitString,outputBufferV, codes, sz, paddedBits);
    outputBuffer = outputBufferV.data();
    writeFileFromBuffer(outputPath, outputBuffer,sz, 0);
   
}

int main()
{   
    char* dEI = "test.txt";
    char* dEO = "test1.txt";
    char *dDO = "decoded.txt";
    map<unsigned char, string> codes;
    compressFile(dEI, dEO, codes);
    decompressFile(dEO, dDO);
}

