#include <iostream>
#include <vector>
#include <fstream>
#include "Node.h"
#define table_size 1000000
#define FILE_NAME_1 "combined.txt"
#define FILE_NAME_2 "unigram_freq.csv"

//cout
#define ll long long
using namespace std;


#define LLMAX 9223372036852

struct heap_entry{
    int book_no, page_no, para_no;
    long double score;
    bool operator>(heap_entry const& other){
        return this->score > other.score;
    }
    heap_entry(int book_code , int page_code , int para_code, long double score_in) : book_no(book_code) , page_no(page_code) , para_no(para_code) , score(score_in){

    }
    heap_entry(){
        this->book_no = -1;
        this->page_no = -1;
        this->para_no = -1;
        this->score = -1;
    }
};


class minheap{
vector<heap_entry> arr;
int size;
public:
    minheap(){
        this->arr.resize(0);
        this->size = 0;
    }
    int get_parent(int idx){
        if(!idx){
            return 0;
        }
        return (int)((idx-1)/2);
    }
    int get_left(int idx){
        int left = idx*2 + 1;
        if(left >= this->arr.size()){
            return -1;
        }else{
            return left;
        }
    }
    int get_right(int idx){
        int right = 2*idx + 2;
        if(right >= this->arr.size()){
            return -1;
        }else{
            return right;
        }
    }
    void heapify_up(int idx){
        int par_idx = get_parent(idx);
        if(par_idx == idx){
            return;
        }
        if(this->arr[par_idx] > this->arr[idx]){
            iter_swap(this->arr.begin() + idx , this->arr.begin() + par_idx);
            heapify_up(par_idx);
        }
        return;
    }
    int get_min_idx(int idx){
        int left = get_left(idx);
        int right = get_right(idx);
        if(right == -1){
            if(left == -1){
                return idx;
            }else{
                if(arr[idx] > arr[left]){
                    return left;
                }else{
                    return idx;
                }
            }
        }else{
            if(left == -1){
                if(arr[idx] > arr[right]){
                    return right;
                }else{
                    return idx;
                }
            }else{
                if(arr[left] > arr[idx]){
                    if(arr[right] > arr[idx]){
                        return idx;
                    }else{
                        return right;
                    }
                }else{
                    if(arr[left] > arr[right]){
                        return right;
                    }else{
                        return left;
                    }
                }
            }
        }
        // heap_entry left_child;
        // heap_entry right_child;
        // left_child.score = 222222222;
        // right_child.score = 222222222;
        // if(left != -1 && left < this->arr.size()){
        //     left_child = arr[left];
        // }
        // if(right != -1 && right < this->arr.size()){
        //     right_child = arr[right];
        // }
        // int ans = idx;
        // if(arr[ans] > left_child){
        //     ans = left;
        // }
        // if(arr[ans] > right_child){
        //     ans = right;
        // }
        // return ans;
    }
    void heapify_down(int idx){
        int min_idx = get_min_idx(idx);
        //cout<<" "<<min_idx<<"min_idx ";
        if(min_idx != idx){
            iter_swap(arr.begin() + idx , arr.begin() + min_idx);
            heapify_down(min_idx);
        }
        return;
    }
    void push_heap(int book_code , int page_code , int para_code ,long double score_inc){
        heap_entry new_entry(book_code , page_code , para_code ,score_inc);
        arr.push_back(new_entry);
        heapify_up(this->size);
        this->size++;
    }
    heap_entry get_min(){
        if(this->size){
            return arr[0];
        }
        heap_entry ans;
        ans.score = 555555;
        ans.book_no = 5555555;
        ans.para_no = 5555555;
        return ans;
    }
    void delete_min(){
        if(!this->size){
            return;
        }
        this->size--;
        if(!this->size){
            arr.erase(arr.begin());
            return;
        }
        iter_swap(arr.begin() , arr.begin() + arr.size() - 1);
        int k = arr.size() - 1;
        arr.erase(arr.begin() + k);
        // this->print_heap();
        // cout<<endl;
        heapify_down(0);
    }
    void replace(int book_no , int page_code , int para , long double score){
        arr[0].book_no = book_no;
        arr[0].page_no = page_code;
        arr[0].para_no = para;
        arr[0].score = score;
        heapify_down(0);
    }
    int get_size(){
        return this->size;
    }
    void print_heap(){
        for(int i = 0; i < arr.size(); i++){
            cout<<arr[i].score<<" ";
        }
        cout<<endl;
    }
};
class rbnode {
public:
    string key;
    ll count;
    bool color;
    int bdepth;
    rbnode* left;
    rbnode* right;
    rbnode* par;
    bool is_external;
    rbnode(string k , ll cou, int bdep,bool b , bool exter){
        this->left = NULL;
        this->right = NULL;
        this->par = NULL;
        this->key = k;
        this->count = cou;
        this->color = b;
        this->bdepth = bdep;
        this->is_external = exter;
    }
    ~rbnode(){
        if(left){
            delete left;
        }
        if(right){
            delete right;
        }
    }
    void update_bdepth(){
        rbnode* temp = this->par;
        int height = 0;
        while(temp){
            if(temp->color){
                height++;
            }
            temp = temp->par;
        }
        this->bdepth = height;
    }
    void update_bdepth_value(int v){
        if(this){
            if(this->is_external){
                this->bdepth = v;
                return;
            }
            if(this->left){
                this->left->update_bdepth_value(v);
            }
            if(this->right){
                this->right->update_bdepth_value(v);
            }
        }
        return;
    }
    char get_parent_char(){
        if(this->par == NULL){
            return 'f';
        }
        if(this->par->left == this){
            return 'l';
        }else{
            return 'r';
        }
    }
    // void set_externals(){
    //     this->left = new rbnode("!" , 1 , 1 , 1);
    //     this->right = new rbnode("!" , 1 , 1 , 1);
    //     this->left->update_bdepth();
    //     this->right->update_bdepth();
    // }
    rbnode* case_one(char l){
        rbnode* new_root = NULL;
        rbnode* shared = NULL;
        if(l == 'l'){
            if(this->left->right->color == 0){
                new_root = this->left->right;

                this->left->right = new_root->left;
                new_root->left->par = this->left;

                new_root->left = this->left;
                this->left->par = new_root;

                this->left = new_root->right;
                new_root->right->par = this;

                new_root->right = this;
                this->par = new_root;
                new_root->par = NULL;

                new_root->color = 1;
                new_root->left->color = 0;
                new_root->right->color = 0;
                return new_root;
            }
            new_root = this->left;
            shared = new_root->right;
            this->left = shared;
            new_root->right = this;
        }else{
            if(this->right->left->color == 0){
                new_root = this->right->left;

                this->right->left = new_root->right;
                new_root->right->par = this->right;

                new_root->right = this->right;
                this->right->par = new_root;

                this->right = new_root->left;
                new_root->left->par = this;

                new_root->left = this;
                this->par = new_root;
                new_root->par = NULL;

                new_root->color = 1;
                new_root->left->color = 0;
                new_root->right->color = 0;
                return new_root;
            }
            new_root = this->right;
            shared = new_root->left;
            this->right = shared;
            new_root->left = this;
        }
        if(shared){
            shared->par = this;
        }
        this->color = 0;
        new_root->color = 1;
        new_root->par = NULL;
        this->par = new_root;
        // //cout<<"case 1"<<new_root->key<<endl;
        return new_root;
    }
    
};

class rbtree {
public:
    rbnode* root;
    int size;
    vector<rbnode*> externals;
    rbtree(){
        this->root = NULL;
        this->size = 0;
        externals.resize(0);
    }
    ~rbtree(){
        delete this->root;
    }
    void insert(string k , ll count_inc = 1){
        rbnode* temp = this->root;
        this->size++;
        if(this->root == NULL){
            this->root = new rbnode(k , count_inc ,1, 1 , 0);
            this->root->left = new rbnode("!" ,1 , 1 , 1 , 1);
            this->root->right = new rbnode("!" , 1 , 1 , 1 , 1);
            externals.push_back(root->left);
            externals.push_back(root->right);
            return;
        }
        rbnode* inserted_node = new rbnode(k , count_inc, 1 , 0 , 0);
        while(!temp->is_external){
            if(temp->key == k){
                temp->count += count_inc;
                delete inserted_node;
                this->size--;
                return;
            }else if(temp->key > k){
                if(temp->left->is_external){
                    inserted_node->par = temp;
                    inserted_node->left = temp->left;
                    temp->left = inserted_node;
                    inserted_node->left->par = inserted_node;
                    inserted_node->right = new rbnode("!" ,1 ,inserted_node->left->bdepth , 1 , 1);
                    externals.push_back(inserted_node->right);
                    if(temp->color){
                        return;
                    }
                    break;
                }
                temp = temp->left;
            }else{
                if(temp->right->is_external){
                    inserted_node->par = temp;
                    inserted_node->right = temp->right;
                    temp->right = inserted_node;
                    inserted_node->right->par = inserted_node;
                    inserted_node->left = new rbnode("!" ,1 ,inserted_node->right->bdepth , 1 , 1);
                    externals.push_back(inserted_node->left);
                    if(temp->color){
                        return;
                    }
                    break;
                }
                temp = temp->right;
            }
        }
        bool b_changed = false;
        while(temp){
            rbnode* temp_par = temp->par;
            bool rc = 1, lc = 1;
            if(temp->left){
                lc = temp->left->color;
            }
            if(temp->right){
                rc = temp->right->color;
            }
            if((rc == 0 || lc == 0) && temp->color == 0){
                if((temp_par->left->color)^(temp_par->right->color)){
                    char l = temp->get_parent_char();
                    rbnode* temp_gp = temp_par->par;
                    char gl = temp_par->get_parent_char();
                    rbnode* new_root_sub = temp_par->case_one(l);
                    new_root_sub->par = temp_gp;
                    if(temp_gp){
                        if(gl == 'l'){
                            temp_gp->left = new_root_sub;
                        }else{
                            temp_gp->right = new_root_sub;
                        }
                    }else{
                        this->root = new_root_sub;
                    }
                    return;
                }else{
                    b_changed = true;
                    if(temp_par == this->root){
                        temp_par->left->color = 1;
                        temp_par->right->color = 1;
                        for(int i = 0; i < externals.size(); i++){
                            externals[i]->bdepth++;
                        }
                        return;
                    }
                    temp_par->left->color = 1;
                    temp_par->right->color = 1;
                    temp_par->color = 0;
                }
            }
            temp = temp_par;
        }
        // if(b_changed){
        //     for( int i = 0; i < externals.size(); i++){
        //         externals[i]->bdepth++;
        //     }
        // }
    }
    
    ll search(string &k){
        rbnode* temp = this->root;
        while(temp){
            if(temp->is_external){
                return -1;
            }
            if(temp->key == k){
                return temp->count;
            }else if(temp->key > k){
                temp = temp->left;
            }else{
                temp = temp->right;
            }
        }
        return -1;
    }
    
    // void inorder_entry(vector<dict_entry>* ans , rbnode* node){
    //     if(node == NULL){
    //         return;
    //     }
    //     if(node->is_external){
    //         // //cout<<" <external "+to_string(node->bdepth) + " >";
    //         // ans->push_back(dict_entry{"  external " , node->bdepth});
    //         return;
    //     }
    //     inorder_entry(ans , node->left);
    //     // dict_entry entry = {node->key , node->count};
    //     ans->push_back(dict_entry{node->key , node->count});
    //     // string ansstring = " < extr-" + to_string((int)(node->is_external)) +" "+node->key +" is_black"+to_string((int)(node->color)) +" bdep-"+to_string(node->bdepth)+ " > ";
    //     // //cout<<ansstring;
    //     inorder_entry(ans , node->right);
    // }
    // vector<dict_entry>* get_entries(){
    //     vector<dict_entry>* ans = new vector<dict_entry>;
    //     inorder_entry(ans , this->root);
    //     return ans;
    // }
    rbnode* get_root(){
        return this->root;
    }
    
};


class Dict_par{
rbtree* rb;
public:
    Dict_par(){
        this->rb = new rbtree();
    }
    ~Dict_par(){
        delete this->rb;
    }
    void insert(string s , int count = 1){
        this->rb->insert(s , count);
    }
    ll get_count_dict(string &s){
        return this->rb->search(s);
    }
};

class freq_table{
vector<rbtree*> table;
public:
    freq_table(){
        this->table.resize(table_size);
    }

    ~freq_table(){
        for(int i = 0; i < table_size; i++){
            if(this->table[i]){
                delete this->table[i];
                this->table[i] = NULL;
            }
        }
    }

    int customhash(const string &word){
        long long hash = 0;
        for (char c : word){
            hash = (1ll * hash * 31)%table_size+ static_cast<int>(c);
        }
        return hash % table_size;
    }

    void insert_freq(string&sentence , bool is_token = false){
        ll num = 1;
        if(is_token){
            int hash_code = customhash(sentence);
            if(!table[hash_code]){
                this->table[hash_code] = new rbtree();
            }
            ////cout<<sentence<<"="<<num<<">";
            this->table[hash_code]->insert(sentence , num);
            return;
        }
        int i = 0;
        for(; i <= sentence.size(); i++){
            if(sentence[i] == ','){
                break;
            }
        }
        string key = sentence.substr(0 , i);
        string number = sentence.substr(i + 1);
        ////cout<<number<<endl;
        int hash_code = customhash(key);
        if(!table[hash_code]){
            this->table[hash_code] = new rbtree();
        }
        num = stoll(number , NULL, 10);
        // if(key == "for" || key == "the" || key == "is" || key == "on"){
        //     //cout<< key<<" " <<number <<" "<<num<<endl;
        // }
        this->table[hash_code]->insert(key , num);
    }

    ll get_freq(string &s){
        int hash_code = customhash(s);
        if(this->table[hash_code]){
            ll ans = this->table[hash_code]->search(s);
            return (ans == -1)?0:ans; 
        }
        return 0;
    }
    void max_collision(){
        int max_ans = 0;
        for(int i = 0; i < table_size; i++){
            if(table[i]){
                max_ans = max(max_ans , table[i]->size);
            }
        }
        //cout<<"max_collisions_in_freq_table " <<max_ans<<endl;
    }
};


class Para_node{
Dict_par dict;
public:
    Para_node(){
    }
    ~Para_node(){

    }
    void insert(string k , ll count_inc){
        this->dict.insert(k , count_inc);
    }
    ll get_count_para(string &s){
        return this->dict.get_count_dict(s);
    }
};

class Page_node{
vector<Para_node*> paras;
int last_page;
public:
    Page_node(){
        paras.resize(1);
        last_page = 0;
    }
    ~Page_node(){
        for(int i = 0; i < paras.size(); i++){
            if(paras[i]){
                delete paras[i];
                paras[i] = NULL;
            }
        }
    }
    void insert(string k , int para_no , ll count_inc = 1){
        if(para_no >= paras.size()){
            paras.resize(2*para_no + 1);
        }
        if(!paras[para_no]){
            paras[para_no] = new Para_node;
        }
        last_page = max(last_page, para_no);
        paras[para_no]->insert(k , count_inc);
    }
    ll get_count_page(string& s , int para_no = 0){
        if(this->paras[para_no]){
            return this->paras[para_no]->get_count_para(s);
        }
        return -1;
    }
    Para_node* get_para(int idx){
        if(idx < paras.size()){
            return this->paras[idx];
        }
        return NULL;
    }
    int get_no_paras(){
        return this->paras.size();
    }
};

class Book_node{
vector<Page_node*> pages;
int last_page;
public:
    Book_node(){
        pages.resize(1);
        this->last_page = 0;
    }
    ~Book_node(){
        for(int i = 0; i < pages.size(); i++){
            if(pages[i]){
                delete this->pages[i];
                this->pages[i] = NULL; 
            }
        }
    }
    void insert(string k , int page_no , int para_no, ll count_inc = 1){
        this->last_page = max(this->last_page, page_no);
        if(page_no >= pages.size()){
            pages.resize(2*page_no + 1);
        }
        if(!pages[page_no]){
            pages[page_no] = new Page_node();
        }
        this->pages[page_no]->insert(k , para_no , count_inc);
    }
    ll get_count_book(string& s , int page_no = 0, int para_no = 0){
        if(this->pages[page_no]){
            return this->pages[page_no]->get_count_page(s , para_no);
        }
        return -1;
    }
    Page_node* get_page(int idx){
        if(idx < pages.size()){
            return this->pages[idx];
        }
        return NULL;
    }
    int get_no_pages(){
        return this->pages.size();
    }
};

class Doc_handler{
vector<Book_node*> books;
freq_table corpus_freq;
int last_book;
vector<bool> is_separator;
vector<char> chars;
public:
    Doc_handler(){
        string separators = " .,-:!\"\'()?[];@";
        this->books.resize(1);
        this->last_book = 0;
        is_separator.resize(256);
        chars.resize(256);
        for(int i = 0; i < 256; i++){
            is_separator[i] = false;
            if(i >= (int)('A') && i <= (int)('Z')){
                chars[i] = (char)(i - 'A' + 'a');
            }else{
                chars[i] = (char)(i);
            }
        }
        for(char& c : separators){
            is_separator[(int)(c)] = true;
        }
        last_book = 0;
    }

    ~Doc_handler(){
        for(int i = 0; i < books.size(); i++){
            if(books[i]){
                delete this->books[i];
                this->books[i] = NULL;
            }
        }
    }

    freq_table& get_freq_table(){
        return this->corpus_freq;
    }
    void seperator(string & sentence , vector<string> &tokens){
        for(int i = 0; i < sentence.size(); i++){
            string token = "";
            for(int k = i; k < sentence.size(); k++ , i++){
                if(is_separator[(int)(sentence[k])]){
                    break;
                }else{
                    token.push_back(chars[(int)(sentence[k])]);
                }
            }
            if(token.size()){
                tokens.push_back(token);
            }
        }
    }

    void insert_sentence(string& sentence ,int book_code = 0, int page_no = 0, int para_no = 0, int sentence_no = 0 ,ll count_inc = 1 ){
        this->last_book = max(this->last_book , book_code);
        if(book_code >= this->books.size()){
            this->books.resize(2*book_code + 1);
        }
        if(!books[book_code]){
            this->books[book_code] = new Book_node();
        }
        vector<string> tokens;
        ////cout<<sentence<<endl;
        seperator(sentence , tokens);
        for(string &token: tokens){
            ////cout<<"<"<<token<<">";
            this->books[book_code]->insert(token , page_no , para_no, count_inc);
            this->corpus_freq.insert_freq(token , true);
        }
    }
    
    ll get_count(string &s ,int book_no = 0, int page_no = 0,int para_no = 0){
        if(this->books[book_no]){
            return this->books[book_no]->get_count_book(s , page_no , para_no);
        }
        return -1;
    }

    ll get_corpus_count(string&s){
        return this->corpus_freq.get_freq(s);
    }

    void get_top_k(minheap &mhp , vector<long double> &weights , vector<string> &words ,int k){
        ofstream output_file("out1.txt" , std::ios::app);
        for(int i = 0; i < this->books.size(); i++){
            if(books[i]){
                for(int j = 0; j < books[i]->get_no_pages(); j++){
                    Page_node* temp = books[i]->get_page(j);
                    if(temp){
                        for(int m = 0; m < temp->get_no_paras(); m++){
                            Para_node* temp_para = temp->get_para(m);
                            if(temp_para){
                                long double score = 0;
                                int f = 0;
                                for(string &word : words){
                                    ll count_temp = temp_para->get_count_para(word);
                                    if(count_temp <= 0){
                                        count_temp = 0;
                                    }
                                    output_file<<"("<<to_string(count_temp)<<"*"<<to_string(weights[f])<<")";
                                    score += weights[f]*count_temp;
                                    f++;
                                }
                                if(mhp.get_size() < k){
                                    mhp.push_heap(i , j , m , score);
                                }else{
                                    heap_entry temp_he = mhp.get_min();
                                    output_file<<"|"<<temp_he.score<<"|";
                                    if(temp_he.score < score){
                                        output_file<<"here";
                                        //mhp.delete_min();
                                        mhp.replace(i , j , m ,score);
                                    }
                                }
                                output_file<<" <"<<m<<"="<<score<<"> \n";
                            }
                        }
                    }
                }
            }
        }
    output_file.close();
    }

};




class Docuement{
Doc_handler corpus;
freq_table gen_freqs;

public:
    Docuement(){
    }

    ~Docuement(){
    }
    ////cout
    void insert_sent(string &s , int book_code , int page_no , int para_no , int sentence_no){
        this->corpus.insert_sentence(s , book_code , page_no , para_no , sentence_no , 1LL);
    }
    void insert_corpus(string filename = FILE_NAME_1){
        std::ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open the input file." << std::endl;
            return;
        }

        std::string tuple;
        std::string sentence;
        while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
            // Get a line in the sentence
            tuple += ')';

            std::vector<int> metadata;    
            std::istringstream iss(tuple);

            // Temporary variables for parsing
            std::string token;

            // Ignore the first character (the opening parenthesis)
            iss.ignore(1);

            // Parse and convert the elements to integers
            while (std::getline(iss, token, ',')) {
                // Trim leading and trailing white spaces
                size_t start = token.find_first_not_of(" ");
                size_t end = token.find_last_not_of(" ");
                if (start != std::string::npos && end != std::string::npos) {
                    token = token.substr(start, end - start + 1);
                }
                
                // Check if the element is a number or a string
                if (token[0] == '\'') {
                    // Remove the single quotes and convert to integer
                    int num = std::stoi(token.substr(1, token.length() - 2));
                    metadata.push_back(num);
                } else {
                    // Convert the element to integer
                    int num = std::stoi(token);
                    metadata.push_back(num);
                }
            }
            this->corpus.insert_sentence(sentence ,metadata[0] , metadata[1] , metadata[2] , metadata[3] , 1LL);
            //this->gen_freqs.insert_freq(token , true);
        }
    }

    void insert_freq_data(string filename = FILE_NAME_2){
        std::ifstream inputfile(filename);
        if(!inputfile.is_open()){
            cerr <<"Error: Unable to open the input file." << endl;
            return;
        }
        string line;
        long long i = 0;
        while(getline(inputfile , line)){
            if(!i){
                i++;
                continue;
            }
            this->gen_freqs.insert_freq(line , false);
        }
        //this->gen_freqs.max_collision();
    }

    void set_weights(vector<long double> &weights , vector<string> &tokens){
        for(string &token : tokens){
            ll corpus_freq = this->corpus.get_corpus_count(token); 
            ll gen_freq = this->gen_freqs.get_freq(token);
            ////cout<<token<<"-" <<corpus_freq<<"<<"<<gen_freq<<"=";
            if(gen_freq <= 0){
                gen_freq = 9223372036850;
            }
            if(corpus_freq <= 0){
                corpus_freq = 0;
            }
            corpus_freq++;
            gen_freq++;
            // if(corpus_freq == -1){
            //     corpus_freq = 0;
            // }
            long double temp_ld = 1.0;
            long double weight = temp_ld*corpus_freq/gen_freq;
            weights.push_back(weight);
            // cout<< token<<" "<<weight<<endl;
        }
    }

    Node* top_k(string &sentence , int k){
        vector<string> words;
        this->corpus.seperator(sentence , words);
        vector<long double> weights;
        // for(string& word : words){
        //     //cout<<word<<" < ";
        // }
        // //cout<<endl;
        ////cout<<"tokenised question"<<endl;
        set_weights(weights , words);
        ////cout<<"weights done"<<endl;
        minheap mhp;
        this->corpus.get_top_k(mhp ,weights ,words , k);
        ////cout<<"got top k"<<endl;
        Node* tail = NULL;
        if(k){
            if(mhp.get_size()){
                heap_entry hp = mhp.get_min();
                mhp.delete_min();
                tail = new Node();
                tail->book_code = hp.book_no;
                tail->page = hp.page_no;
                tail->paragraph = hp.para_no;
                tail->left = NULL;
                tail->right = NULL;
            }
        }
        if(!tail){
            return NULL;
        }
        int i = 0;
        Node* temp_head = tail;
        while(i < k - 1 && mhp.get_size() > 0){
            ////cout<<hp.score<<"  ";
            heap_entry hp = mhp.get_min();
            mhp.delete_min();
            Node* temp = new Node();
            temp->book_code = hp.book_no;
            temp->page = hp.page_no;
            temp->paragraph = hp.para_no;
            temp->left = NULL;
            temp->right = temp_head;
            temp_head->left = temp;
            temp_head = temp;
            i++;
        }
        ////cout<<endl;
        return temp_head;
    }
};


class QNA_tool {

private:
    Docuement doc;
    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);

    // You can add attributes/helper functions here

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};