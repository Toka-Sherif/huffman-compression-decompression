// C++ program for Huffman Coding
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
#include <unordered_map>
#include <bits/stdc++.h>
using namespace std;

using std::ofstream;
using std::ifstream;

unordered_map<char, string> umap;
unordered_map<char, string> umap2;
// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 256

// A Huffman tree node
struct MinHeapNode
{
    // One of the input characters
    char data;

    // Frequency of the character
    unsigned freq;

    // Left and right child of this node
    struct MinHeapNode *left, *right;
};

// A Min Heap: Collection of min-heap (or Huffman tree) nodes
struct MinHeap
{
    // Current size of min heap
    unsigned size;

    // capacity of min heapC:\Users\tokay\Documents\term(7)\Algorithms\huff_cplus\main.cpp
    unsigned capacity;

    // Attay of minheap node pointers
    struct MinHeapNode** array;
};

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(char data, unsigned freq)
{
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

// A utility function to create a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity)
{
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));

    // current size is 0
    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a,struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx)
    {
        swapMinHeapNode(&minHeap->array[smallest],&minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}

// A standard function to extract minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

// A utility function to insert a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode)
{
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap)
{
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// A utility function to print an array of size n
void printArr(char arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        cout<< arr[i];

    cout<<"\n";
}

// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)
{
    return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size)
{
    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size)
{
    struct MinHeapNode *left, *right, *top;

    // Step 1: Create a min heap of capacity equal to size. Initially, there are modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap))
    {

        // Step 2: Extract the two minimum
        // freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3: Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the root node and the tree is complete.
    return extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, char arr[], int top)
{
    // Assign 0 to left edge and recur
    if (root->left)
    {
        arr[top] = '0';
        printCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right)
    {
        arr[top] = '1';
        printCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (isLeaf(root))
    {
        cout<<root->data <<"\t";
        cout<<(int)root->data <<"\t";
        cout<< bitset<8> (root->data)<<"\t";
        string s = "";
        for (int i = 0; i < top; i++)
        {
            s = s + arr[i];
        }
        umap[root->data] = s;
        printArr(arr, top);
    }
}

// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size)
{
    // Construct Huffman Tree
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    // Print Huffman codes using the Huffman tree built above
    char arr[MAX_TREE_HT];
    int top = 0;
    printCodes(root, arr, top);
}

int convertBinaryToDecimal(bitset<8> bit)
{
    bitset<8> new_bit;
    for(int i=0,j=7; i<8&&j>=0; i++,j--)
    {
        new_bit[i]=bit[j];
    }

    return new_bit.to_ulong();
}

void string_to_binary(char c_aa)
{
    bitset<8> b(c_aa);
    cout << b << endl;
}

int SplitFilename (const std::string& str)
{
    int flag = 0 ;
    //std::cout << "Splitting: " << str << '\n';
    std::size_t found = str.find_last_of(".");
    //std::cout << " path: " << str.substr(0,found) << '\n';

    if( str.substr(found+1) == "txt")
    {
        //std::cout << " file: " << str.substr(found+1) << '\n';
    }
    else if(str.substr(found+1) == "bin")
    {
        //std::cout << " file: " << str.substr(found+1) << '\n';
        flag = 1 ;
    }
    return flag ;

}

int main()
{
    char choice,ch,arr[256];
    string filename,encode,txt;
    int freq[256]= {0},i,j,n=0,arr_size=0,frequency[256]= {0};
    char counter = 0;

    clock_t start, end;

    cout << "Please enter the filename (ex: name.txt)\n ";
    getline( cin, filename );
    int extension ;

    extension = SplitFilename(filename);
//    cout << extension;

    cout << "Choose (a/b)?\na-compression\nb-decompression\n ";
    choice = getchar();

    if(choice=='a' && extension == 0 )
    {

        start = clock();
        std::ifstream file( filename.c_str(), std::ios::binary );
        if(!file)
        {
            cout << "Error! Incorrect file."<< endl;
            exit( EXIT_FAILURE );
        }
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
        file.close();

        for(char ch : buffer)
        {
            frequency[ch]++;
            n++;
        }
        cout << "\n";

        int uncompressed_size=n;

        for(i=0; i<256; i++)
        {
            if(frequency[i] !=0)
            {
                arr[counter]=i;
                freq[counter]=frequency[i];
                counter++;
            }
        }
       // cout<<"counter= "<<counter<<"\n";
       // cout <<"Number of char in file "<< n<<"\n";

        cout<<"Char\tByte\tCode\t\tNew Code\n";
        HuffmanCodes(arr, freq, counter);

        string encoded = "";

        int ascii,enc_int[8],out_byte;
        string bin,enc;
        //ofstream output ("output2.txt"); //open file in write mood
        char str_to_char[8];
        int padding_size;
        bitset<8> Byte;
        //print array size in file
        cout<<"-------------------------------------------------------------";
        encoded = encoded + counter;


        //print huffman code in file
        for (auto i = umap.begin(); i != umap.end(); i++)
        {
            ascii=i->first;
            bin=i->second;
            encoded = encoded + (char)ascii;
            encoded = encoded +  bin + char(16);

        }
        //cout<<"Map size is "<<umap.size();

        //encoding content of the file
        int compressed_size=0;
        for(char ch : buffer)
        {
            auto value = umap.find(ch);
            bin = value->second;
            encode+=bin;
            int length= encode.length();
            if(length>=8)
            {
                compressed_size++;
                if(length==8)
                {
                    enc=encode;
                    encode.clear();
                }
                else if(length>8)
                {
                    enc=encode.substr(0,8);
                    encode=encode.substr(8,length-8);
                }
                strcpy(str_to_char, enc.c_str());
                for(int i=0; i<8; i++)
                {
                    enc_int[i] = int(str_to_char[i])-'0';
                    //cout<<encoded_int_arr[i];
                    Byte[i]=enc_int[i];
                }
                out_byte=convertBinaryToDecimal(Byte);
                //cout<<char(compressed_arr[i]);
                encoded+=char(out_byte);
            }

        }

        if(encode.length()!=0)
        {
            padding_size=8-encode.length();
            compressed_size++;

            strcpy(str_to_char, encode.c_str());
            for(int i=0; i<encode.length(); i++)
            {
                enc_int[i] = int(str_to_char[i])-'0';
                //cout<<encoded_int_arr[i];
                Byte[i]=enc_int[i];
            }
            for(i=0; i<8-encode.length(); i++)
            {
                Byte[i+encode.length()]=0;
            }
            out_byte=convertBinaryToDecimal(Byte);
            encoded+=char(out_byte);
        }



        encoded = encoded + (char)padding_size;

        int l = encoded.length();
        char encoded_bytes[l];
        for(int i =0; i<l; i++)
        {
            encoded_bytes[i] = (char)encoded[i];
        }

        ofstream file2("test_huffman.bin",ios::out|ios::trunc|ios::binary);

        if( remove( "test_huffman.txt" ) != 0 )
            perror( "Error deleting file" );


        double compression_ratio=double(uncompressed_size)/double(compressed_size);

        file2.write(encoded_bytes,l);
        file2.close();

        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "\nTime taken by program is : " << fixed << time_taken << setprecision(5);
        cout << " sec " << endl;

        cout << "Uncompressed size / Compressed size  = " << uncompressed_size << "/" << compressed_size << "=" << compression_ratio <<"\n" ;
        cout<<"Compression ratio = "<< compression_ratio*100 << "%\n" ;
    }
    else if (choice=='b' && extension == 1)
    {
        start = clock();

        string tx,text;
        string count1;
        string dc,ch,p;
        char tp;
        char counter;


        //read file and loop over the file until get the last char (padding size)
        ifstream file( filename, std::ios::binary );
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
        file.close();

        if(!file)
        {
            cout << "Error! Incorrect file."<< endl;
            exit( EXIT_FAILURE );
        }

        cout << "\n";
        int padding_size = (int)buffer[buffer.size()-1];
        //cout<<"padding :"<<padding_size+1<<endl;


        buffer.pop_back();

        string file_content;
        for(unsigned char ch : buffer)
            file_content += ch;



        stringstream f(file_content);

        f.get(counter);

        //cout<<counter<<"is counter\n";
        for(int i=0; i<counter; i++)
        {
            f.get(tp);
            getline(f,tx,char(16));
            umap2[tp] = tx;
            cout<<tp<<" "<<tx<<"\n";
        }

        i=0;
        char a;
        string huff= {},part= {};
        string decoded;


        int compressed_size = 0;
        while(f.get(a))
        {
            compressed_size++;
            bitset<8>bit(a);
            //cout<<"a is "<<bit<<"\n";
            huff+=bit.to_string();
//            cout<<huff<<"\n";
        }
        int length = huff.length() -padding_size;
        while(i<length)
        {
            part= part+huff[i];
            for(auto  &p : umap2 )
            {
                 //cout<<p.second<<endl;

                if(part == p.second )
                {
                    decoded+=p.first;
                    part.clear();
                }
            }
            i++;
        }

        // write to file
        int l = decoded.length();
        char encoded_bytes[l];
        for(int i =0; i<l; i++)
        {
            encoded_bytes[i] = (char)decoded[i];
        }

        ofstream out("test_huffman.txt",ios::out|ios::trunc|ios::binary);
        out.write(encoded_bytes,l);
        out.close();

        if( remove( "test_huffman.bin" ) != 0 )
            perror( "Error deleting file" );

        int uncompressed_size=l;
        double compression_ratio=double(uncompressed_size)/double(compressed_size);


        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "\nTime taken by program is : " << fixed<< time_taken << setprecision(5);
        cout << " sec " << endl;

        cout << "Uncompressed size / Compressed size  = " << uncompressed_size << "/" << compressed_size << "=" << compression_ratio <<"\n" ;
        cout<< "Compression ratio = "<< compression_ratio*100 << "%\n" ;

    }
    else
    {
        cout << "Error! Incorrect choice. or Incorrect extention"<< endl;
        exit( EXIT_FAILURE );
    }
    return 0;
}
