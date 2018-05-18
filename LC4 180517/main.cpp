#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <tuple>

using namespace std;

int getRightVector(int N)
{
    return N%6;
}

int getBotVector(int N)
{
    return N/6;
}

template<typename V>
V mod(const V& a, const V& b)
{
    return (a % b + b) % b;
}

int main()
{
   string key;
   string message;

   //Get Key and Check its a valid length
   cout << "Enter Key: ";
   getline(cin, key);
   while(key.length() != 36){
    cout << "Invalid Key! Try again." << endl;
    getline(cin, key);
   }

   //Get Message to Decrypt
   cout << "Enter Message: ";
   getline(cin, message);

   //Set up LC4 alphabet
   string alpha = "#_23456789abcdefghijklmnopqrstuvwxyz";

   //tuple<N, col, row>
   map<char, tuple<int, int, int> > lookup;
   for(int i = 0; i < 36; i++)
    lookup[alpha[i]] = make_tuple(i, 0, 0);

   //Set up 6x6 matrix according to given key
   vector<vector<char> > matrix;
   matrix.resize(6, vector<char>(6, 0));
   for(int j = 0; j < 6; j++){
    for(int i = 0; i < 6; i++){
        matrix[j][i] = key[6*j+i];
        get<1>(lookup[key[6*j+i]]) = i;
        get<2>(lookup[key[6*j+i]]) = j;
    }
   }

   //Iterators to keep track of where we are
   auto msg_iter = message.begin();
   int curr_r = get<2>(lookup[*msg_iter]);
   int curr_c = get<1>(lookup[*msg_iter]);

   //Setup starting marker
   int r_marker = 0;
   int c_marker = 0;
   char marker = matrix[r_marker][c_marker];

   string result;
   while(msg_iter != message.end()){
    //Update row and col index to current character
    curr_r = get<2>(lookup[*msg_iter]);
    curr_c = get<1>(lookup[*msg_iter]);

    //How much to move by
    int h_mov = getRightVector(get<0>(lookup[marker]));
    int v_mov = getBotVector(get<0>(lookup[marker]));

    //Get Plaintext char indexes and append to result
    h_mov = mod(curr_c - h_mov, 6);
    v_mov = mod(curr_r - v_mov, 6);
    result.push_back(matrix[v_mov][h_mov]);

    //Right rotate row w/ plaintext char
    char temp = matrix[v_mov][5];
    for(int i = 5; i > 0; i--){
        matrix[v_mov][i] = matrix[v_mov][i-1];
        get<1>(lookup[matrix[v_mov][i]]) = i;
    }
    matrix[v_mov][0] = temp;
    get<1>(lookup[matrix[v_mov][0]]) = 0;

    //If marker moved, update position
    curr_r = get<2>(lookup[*msg_iter]);
    curr_c = get<1>(lookup[*msg_iter]);

    //Down rotate row w/ ciphertext char
    temp = matrix[5][curr_c];
    for(int i = 5; i > 0; i--){
        matrix[i][curr_c] = matrix[i-1][curr_c];
        get<2>(lookup[matrix[i][curr_c]]) = i;
    }
    matrix[0][curr_c] = temp;
    get<2>(lookup[matrix[0][curr_c]]) = 0;

    //If marker moved, update position
    c_marker = get<1>(lookup[marker]);
    r_marker = get<2>(lookup[marker]);

    //Update Marker by ciphertext tile vectors
    h_mov = getRightVector(get<0>(lookup[*msg_iter]));
    v_mov = getBotVector(get<0>(lookup[*msg_iter]));
    c_marker = mod(c_marker + h_mov, 6);
    r_marker = mod(r_marker + v_mov, 6);
    marker = matrix[r_marker][c_marker];

    msg_iter++;
   }
    cout << "Message: " << result << endl;
}
