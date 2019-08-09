 /*huffman.h */
#ifndef _huffman_h
#define _huffman_h

#include<bits/stdc++.h>

using namespace std;

#define MAX_SIZE 260
#define stop 256

struct Huffman_node
{
    int id; 
    unsigned int calculate;
    string code;
    Huffman_node  *left, 
                  *right,
                  *parent; 
};

typedef Huffman_node* Noder;

class Huffman
{
private:
    Noder node_array[MAX_SIZE]; // Ҷ�ӽڵ�����
    Noder root;  // ���ڵ� 
    int size=0;  // Ҷ�ӽڵ������
    fstream in_file, out_file;  
    map<int, string> map_1;  // �ַ��ͱ��Ӧ�ı���
	int calculate[MAX_SIZE] = {0}; // ͳ�Ƹ���  

    class Compare
    {
        public:
            bool operator()(const Noder& c1, const Noder& c2) const
            {
                return (*c1).calculate > (*c2).calculate;
            }
    };

    priority_queue< Noder, vector<Noder>, Compare > pq;
    void create_node_array();
    void create_map_1(const Noder node, bool left); 
    void create_pq(); 
    void create_huffman_tree(); 
    void calculate_huffman_codes(); 
    void do_compress(); 
    void rebuid_huffman_tree(); 
    void decode_huffman();
    void DestroyBiTree(Noder &T);

public: 
    Huffman(string in_file_name, string out_file_name); 
    ~Huffman(); 
    void compress(); 
    void decompress();
};

#endif
