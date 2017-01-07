#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;
string sec1, sec2, sec3, output, input;
char alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
    'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
char alphabetLower[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 
    'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
    't', 'u', 'v', 'w', 'x', 'y', 'z'};
void *pinnacol(void*);
void *hill(void*);
void *substitute(void*);
void *status;

int fixedMod(int x, int y){
    return x%y >= 0? x%y : y + (x%y);
}

void *sift(void*){
   int attempts = 0;
   string inputMsg = "Input a string to be decoded, or type EXIT to close the program: ";
   string error = "One of the cipher threads couldn't be created";
   string error2 = "That input was invalid. Please try again";
   string error3 = "Sorry, you have exceeded maximum attempts at entering a valid input.  The program will now close.";
   pthread_t subThread, hillThread, pinThread;
   int t1Status, t2Status, t3Status;
   size_t index1, index2, index3, top1, top2, top3;
   string temp1, temp2, temp3;
    while(1){
        cout << inputMsg;
        cout << endl;
        temp1 = "";
        temp2 = "";
        temp3 = "";
        getline(cin, input);
        if(input == "EXIT")
            exit(0);
        index3 = input.find("***");
        if(index3 != std::string::npos){
            temp3 = input.substr(index3 + 3);
            top3 = temp3.find('*');
            if(top3 != std::string::npos){
                if(index3 == 0){
                input = input.substr(top3);                
                } else {
                    input = input.substr(0, index3) + input.substr(top3 + index3 + 3);
                }
            } else {
                input = input.substr(0, index3);
            }
            temp3 = temp3.substr(0, top3);
        }
        index2 = input.find("**");
        if(index2 != std::string::npos){
            temp2 = input.substr(index2 + 2);
            top2 = temp2.find('*');
            if(top2 != std::string::npos){
                if(index2 == 0){
                input = input.substr(top2 + index2 + 2);               
                } else {
                    input = input.substr(0, index2);
                }
                temp2 = temp2.substr(0, top2);
            } else {
                input = input.substr(0, index2);
            }
            }
        index1 = input.find('*');
        if(index1 != std::string::npos){
            temp1 = input.substr(index1);
            temp1 = temp1.substr(1);
        }

        if(temp1 == "" || temp2 == "" || temp3 == ""){
            attempts++;
            if(attempts < 3){
                cout << error2 << endl;
        } else {
                cout << error3 << endl;
                exit(0);
        }
        } else {
        
        sec1 = temp1; 
        sec2 = temp2; 
        sec3 = temp3;

            t1Status = pthread_create(&subThread, NULL, substitute, status);
            t2Status = pthread_create(&hillThread, NULL, hill, status);
            t3Status = pthread_create(&pinThread, NULL, pinnacol, status);
            
            if(t1Status || t2Status || t3Status){
                cout << error;
            } else {
            pthread_join(subThread, &status);
            pthread_join(hillThread, &status);
            pthread_join(pinThread, &status);
            cout << endl << sec1 << endl;
            cout << sec2 << endl;
            cout << sec3 << endl;
            cout << endl;
            }     
            sec1 = "";
            sec2 = "";
            sec3 = "";
            input = "";
            attempts = 0;
}
    }
   return status;
}

void *substitute(void*){
    string temp = "";
    string errorMsg = "Sorry, not all of the characters in segment 1 of this input were alphabet characters.";
    string arg = sec1;
    while(arg[0] == ' '){
        arg = arg.substr(1);
    }
    if(isalpha(arg[0])){
        int n = 0;
        for(int i = 0; i < sizeof(alphabet); i++){
            if(arg[0] == alphabet[i]){
                n = i % 10 + 2;
                break;
            }
            if(arg[0] == alphabetLower[i]){
                n = i % 10 + 2;
                break;
            }
        }
        for(int i = 1; i < arg.length(); i++){
            if(isalpha(arg[i]) || arg[i] == ' '){
            if(isupper(arg[i])){
                temp += (char)(((arg[i] - 'A' - n + 26) % 26) + 'A');
            }else if(islower(arg[i])){
                temp += (char)(((arg[i] - 'a' - n + 26) % 26) + 'a');
            }
            }else{
                temp = errorMsg;
                break;
            }
        }
        
    } else {
        temp = errorMsg;
    }
    sec1 = "Section 1 is: " + temp;
}

void *hill(void*){
    string token1;
    string token2;
    string errorMsg1 = "Sorry, there was an odd number of characters in the first token of the second segment input.";
    string errorMsg2 = "Sorry, the second token of the second segment was invalid";
    string errorMsg3 = "There were not enough numbers in the second token of the second segment input.";
    string temp;
    string arg = sec2;
    bool nextNeg = false;
    int matrixS[] = {0,0,0,0}; 
    int matrixC[] = {0,0};
    int index = 0;
    int t = 0, q = 0;
    
    while(arg[0] == ' '){
     arg = arg.substr(1);
    }
    
    for(int i = 0; i < arg.length(); i++){
        if(isalpha(arg[i])){
            token1 += arg[i];
            continue;
        } else if(arg[i] == ' '){
            continue;
        }else{
            token2 = arg.substr(i);
            break;
        }
    }
    
    while(token2[0] == ' '){
        token2 = token2.substr(1);
    }
    
    if((token1.length() % 2) == 0){
        for(int i = 0; i < token2.length(); i++){
            if(isdigit(token2[i]) || token2[i] == '-'){
                if(i > 0){
                if(token2[i - 1] == '-'){
                nextNeg = true;
                }}
                if(i + 1 != token2.length()){
                temp = temp + token2[i];
                }else{
                    temp = temp + token2[i];
                    stringstream ss(temp);
                    ss >> matrixS[index];
                    if(nextNeg){
                        matrixS[index] *= -1;
                    }
                    index++;
                    temp = "";
                    nextNeg = false;
                }
            }else if((token2[i] == ' ' && index < 3) || i+1 == token2.length()){
                if(temp.length() > 0){
                    stringstream ss(temp);
                    ss >> matrixS[index];
                    if(nextNeg){
                        matrixS[index] *= -1;
                    }
                    index++;
                    temp = "";
                    nextNeg = false;                    
                }
            }else{
                sec2 = errorMsg2;
                return status;
            }
        }
    }else{
        sec2 = errorMsg1;
        return status;
    }
//    if(index != 3){
//        sec2 = errorMsg2;
//        return status;
//    }
    sec2 = "";
    int rows = token1.length() / 2;
    for(int i = 0; i < rows; i++){
        for(int c = 0; c < 2; c++){
        for(int j = 0; j < sizeof(alphabet); j++){
            if(token1[i * 2 + c] == alphabet[j]){
                matrixC[c] = j;
            }
        }
    }
        t = matrixC[0];
        q = matrixC[1];
        matrixC[0] = fixedMod((t * matrixS[0] + q * matrixS[1]),26);
        matrixC[1] = fixedMod((t * matrixS[2] + q * matrixS[3]),26);
        
        sec2 += alphabet[matrixC[0]];
        sec2 += alphabet[matrixC[1]]; 
    }
    sec2 = "Section 2 is: " + sec2;
}

void *pinnacol(void*){
    string token1;
    string token2;
    string errorMsg1 = "Sorry, there were not exactly 9 numbers in the second token of section 3.";
    string errorMsg2 = "Sorry, only numbers are allowed in the second token of section 3.";
    string errorMsg3 = "Sorry, the first token of section 3 has an invalid number of characters.";
    string temp;
    string arg = sec3;
    bool nextNeg = false;
    int matrixS[] = {0,0,0,0,0,0,0,0,0}; 
    int matrixC[] = {0,0,0};
    int index = 0;
    int t1 = 0, t2 = 0, t3 = 0;
    
    while(arg[0] == ' '){
    arg  = arg.substr(1);
    }
    
for(int i = 0; i < arg.length(); i++){
        if(isalpha(arg[i])){
            token1 += arg[i];
            continue;
        } else if(arg[i] == ' '){
            continue;
        }else{
            token2 = arg.substr(i);
            break;
        }
    }
    if((token1.length() % 3) == 0){
        for(int i = 0; i < token2.length(); i++){
            if(isdigit(token2[i]) || token2[i] == '-'){
                if(i > 0){
                if(token2[i - 1] == '-'){
                nextNeg = true;
                }}
                if(i + 1 != token2.length()){
                temp = temp + token2[i];
                }else{
                    temp = temp + token2[i];
                    stringstream ss(temp);
                    ss >> matrixS[index];
                    index++;
                    temp = "";
                    if(nextNeg){
                        matrixS[index - 1] *= -1;
                    }
                    nextNeg = false;
                }
            }else if((token2[i] == ' ' && index < 9) || i == token2.length()){
                if(temp.length() > 0){
                    stringstream ss(temp);
                    ss >> matrixS[index];
                    index++;
                    temp = "";
                    if(nextNeg){
                        matrixS[index - 1] *= -1;
                    }
                    nextNeg = false;                    
                }
            }else{
                if(!isdigit(token2[i]) && token2[i] != ' '){
                sec3 = errorMsg2;
                return status;
                }
            }
        }
    }else{
        sec3 = errorMsg3;
        return status;
    }
    if(index != 9){
        sec3 = errorMsg1;
        return status;
    }
    sec3 = "";
    int rows = token1.length() / 3;
    for(int i = 0; i < rows; i++){
        for(int c = 0; c < 3; c++){
        for(int j = 0; j < sizeof(alphabet); j++){
            if(token1[i * 3 + c] == alphabet[j]){
                matrixC[c] = j;
            }
        }
    }
        t1 = matrixC[0];
        t2 = matrixC[1];
        t3 = matrixC[2];
        matrixC[0] = fixedMod((t1 * matrixS[0] + t2 * matrixS[1] + t3 * matrixS[2]),26);
        matrixC[1] = fixedMod((t1 * matrixS[3] + t2 * matrixS[4] + t3 * matrixS[5]),26);
        matrixC[2] = fixedMod((t1 * matrixS[6] + t2 * matrixS[7] + t3 * matrixS[8]),26);

        
        sec3 += alphabet[matrixC[0]];
        sec3 += alphabet[matrixC[1]]; 
        sec3 += alphabet[matrixC[2]];
    }
    
    sec3 = "Section 3 is: " + sec3;
}

int main (){
    pthread_t sifter;
    int siftStatus;
    siftStatus = pthread_create(&sifter, NULL, *sift, status);
    
    if(siftStatus){
        cout << "The sifter thread was not created properly. "<< endl;
    } else{
        pthread_join(sifter, &status);
    }
    
}
//g++ -D_REENTRANT main.cpp -o main -lpthread