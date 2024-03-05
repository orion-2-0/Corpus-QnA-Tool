#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

QNA_tool::QNA_tool(){
    // Implement your function here  
    doc.insert_freq_data();
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    // Implement your function here
    Node* ans =  doc.top_k(question , k);
    return ans;
}

void QNA_tool::insert_sentence(int book_code , int page, int paragraph, int sentence_no, string sentence){
    this->doc.insert_sent(sentence , book_code , page , paragraph , sentence_no);
    //cout<<sentence<<endl;
}

void QNA_tool::query(string question, string filename){
    // Implement your function here  
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: " << "Studying COL106 :)" << std::endl;
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
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
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}


// int main(){

//     QNA_tool qna_tool;

//     int num_books = 98;

//     for(int i = 1; i <= num_books; i++){

//         std::cout << "Inserting book " << i << std::endl;

//         std::string filename = "txtfiles/mahatma-gandhi-collected-works-volume-";
//         filename += to_string(i);
//         filename += ".txt";

//         std::ifstream inputFile(filename);

//         if (!inputFile.is_open()) {
//             std::cerr << "Error: Unable to open the input file mahatma-gandhi." << std::endl;
//             return 1;
//         }

//         std::string tuple;
//         std::string sentence;


//         while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
//             // Get a line in the sentence
//             tuple += ')';

//             std::vector<int> metadata;    
//             std::istringstream iss(tuple);

//             // Temporary variables for parsing
//             std::string token;

//             // Ignore the first character (the opening parenthesis)
//             iss.ignore(1);

//             // Parse and convert the elements to integers
//             while (std::getline(iss, token, ',')) {
//                 // Trim leading and trailing white spaces
//                 size_t start = token.find_first_not_of(" ");
//                 size_t end = token.find_last_not_of(" ");
//                 if (start != std::string::npos && end != std::string::npos) {
//                     token = token.substr(start, end - start + 1);
//                 }
                
//                 // Check if the element is a number or a string
//                 if (token[0] == '\'') {
//                     // Remove the single quotes and convert to integer
//                     int num = std::stoi(token.substr(1, token.length() - 2));
//                     metadata.push_back(num);
//                 } else {
//                     // Convert the element to integer
//                     int num = std::stoi(token);
//                     metadata.push_back(num);
//                 }
//             }

//             // Now we have the string in sentence
//             // And the other info in metadata
//             // Add to the dictionary

//             // Insert in the qna_tool
//             qna_tool.insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
//         }

//         inputFile.close();

//     }

//     string question = "What were the views of Mahatma on Partition?";

//     /*
//         Part-1 : Get the top-k paragraphs
//         Checking Correctness
//     */

//     // Let's try to ask a simple question to the qna_tool
//     Node* head = qna_tool.get_top_k_para(question, 5);

//     vector<string> paras;
//     while(head != nullptr){
//         cout<<head->book_code<<" " << head->page<<" "<< head->paragraph<<endl;
//         // string res = qna_tool.get_paragraph(head->book_code, head->page, head->paragraph);
//         // paras.push_back(res);
//         head = head->right;
//     }

//     // for(int i = 0; i < (int)paras.size(); i++){
//     //     cout << paras[i] << endl << endl << endl;
//     // }

//     /*
//         Part-2 : Query the LLM
//     */
//     //qna_tool.query(question, "api_call.py");
    
//     return 0;
// }



// int main(){
//     QNA_tool qna;
//     std::ifstream inputFile("txtfiles/combined.txt");

//     if (!inputFile.is_open()) {
//         std::cerr << "Error: Unable to open the input file." << std::endl;
//         return 1;
//     }

//     std::string tuple;
//     std::string sentence;

//     while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
//         // Get a line in the sentence
//         tuple += ')';

//         std::vector<int> metadata;    
//         std::istringstream iss(tuple);

//         // Temporary variables for parsing
//         std::string token;

//         // Ignore the first character (the opening parenthesis)
//         iss.ignore(1);

//         // Parse and convert the elements to integers
//         while (std::getline(iss, token, ',')) {
//             // Trim leading and trailing white spaces
//             size_t start = token.find_first_not_of(" ");
//             size_t end = token.find_last_not_of(" ");
//             if (start != std::string::npos && end != std::string::npos) {
//                 token = token.substr(start, end - start + 1);
//             }
            
//             // Check if the element is a number or a string
//             if (token[0] == '\'') {
//                 // Remove the single quotes and convert to integer
//                 int num = std::stoi(token.substr(1, token.length() - 2));
//                 metadata.push_back(num);
//             } else {
//                 // Convert the element to integer
//                 int num = std::stoi(token);
//                 metadata.push_back(num);
//             }
//         }

//         // Now we have the string in sentence
//         // And the other info in metadata
//         // Add to the dictionary

//         // Insert in the dictionary
//         qna.insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
//     }

//     inputFile.close();
//     cout<<"test-1 \n";
//     Node* ans1 = qna.get_top_k_para("What were the views of Mahatma Gandhi on the Partition of India?" , 3);
//     while(ans1){
//         cout<< ans1->book_code<<" "<<ans1->page<<" "<<ans1->paragraph << "\n";
//         ans1 = ans1->right;
//     }
//     cout<<"test-2 \n";
//     Node* ans2 = qna.get_top_k_para("Who was Mahatma Gandhi?" , 3);
//     while(ans2){
//         cout<< ans2->book_code<<" "<<ans2->page<<" "<<ans2->paragraph << "\n";
//         ans2 = ans2->right;
//     }
//     cout<<"test-3 \n";
//     Node* ans3 = qna.get_top_k_para("What were Gandhi's views on the manner of how one should eat?" , 3);
//     while(ans3){
//         cout<< ans3->book_code<<" "<<ans3->page<<" "<<ans3->paragraph << "\n";
//         ans3 = ans3->right;
//     }
//     cout<<"test-4 \n";
//     Node* ans4 = qna.get_top_k_para("What is the purpose of life?" , 3);
//     while(ans4){
//         cout<< ans4->book_code<<" "<<ans4->page<<" "<<ans4->paragraph << "\n";
//         ans4 = ans4->right;
//     }
//     cout<<"test-5 \n";
//     Node* ans5 = qna.get_top_k_para("What was the effect of tea and coffee according to Mahatma?" , 3);
//     while(ans5){
//         cout<< ans5->book_code<<" "<<ans5->page<<" "<<ans5->paragraph << "\n";
//         ans5 = ans5->right;
//     }
// }