#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

//look-up
unordered_map<string,char> morse2c={
    {".",'E'},
    {"-",'T'},
    {".-",'A'},
    {"..",'I'},
    {"--",'M'},
    {"-.",'N'},
    {"-..",'D'},
    {"--.",'G'},
    {"-.-",'K'},
    {"---",'O'},
    {".-.",'R'},
    {"...",'S'},
    {"..-",'U'},
    {".--",'W'},
    {"-...",'B'},
    {"-.-.",'C'},
    {"..-.",'F'},
    {"....",'H'},
    {".---",'J'},
    {".-..",'L'},
    {".--.",'P'},
    {"--.-",'Q'},
    {"...-",'V'},
    {"-..-",'X'},
    {"-.--",'Y'},
    {"--..",'Z'}
    };

//list of all possible letters following the current one
struct tree_node{
    map<char,tree_node*> children;
};

void dic_insert(tree_node* start,const char* str,int size){
    tree_node* pos=start;

    for(int i=0;i<size;i++){
        auto it=pos->children.find(str[i]);
        if(it!=pos->children.end()){
            // found current letter
            pos=it->second;
        }else{
            //insert remaining letters
            for(int j=i;j<size;j++){
                tree_node* nl=new tree_node;
                pos->children[str[j]]=nl;
                pos =nl;
            }
            pos->children['.']=nullptr;

            return;
        }
    }
    pos->children['.']=nullptr;
    // '.' represent a possible end of a word in the dictionary
}

void dic_print(tree_node* dic){
    for( auto& c : dic->children){        
        cerr<< c.first;
        if(c.second)dic_print(c.second);
        else cerr<<endl;
    }
}

void dic_free(tree_node* dic){
    for( auto& c : dic->children){   
        if(c.second){
            dic_free(c.second);
        }
    }
    free(dic);
}

 void extract_words(string& s,int s_pos,tree_node* beg,map<int,long>& nb_ways){
    for(int i=1;i<5;i++){// try using 1,2,3,4 characters from the morse code
        if(s.size()<s_pos+i){
            break;
        }
        
        char cc=morse2c[s.substr(s_pos,i)];
        //invalid morse code will add an entry to the map and return a null character
        if(cc){           
            auto it=beg->children.find(cc);
            if(it!=beg->children.end()){// found current letter
                
                //if it's possible to end a word at this position increment the number of ways
                if(it->second->children.count('.'))nb_ways[s_pos+i]++;
                
                //try at the folowing position
                extract_words(s,s_pos+i,it->second,nb_ways);
            }
            // else word doesn't exist in the dictionary so we ignore it
        }
    }
 }


int main()
{
    tree_node* dictionary=new tree_node;
    string L;
    cin >> L; cin.ignore();
    int N;
    cin >> N; cin.ignore();
    for (int i = 0; i < N; i++) {
        string W;
        cin >> W; cin.ignore();
        dic_insert(dictionary, W.c_str(), W.size());
    }
    //dic_print(dictionary);
    
    map<int,long> cur_way={{0,1}};
    // cur_way.second is the number of ways possible to form a sentence using cur_way.first morse characters


    auto itt=cur_way.begin();
    while(itt->first<L.size()){
        map<int,long> new_ways;
        extract_words(L,itt->first,dictionary,new_ways);
        for(auto & pp:new_ways){
            //the number of ways to get to a new position B
            //is (the number of ways to get to A) times (the number of ways from A to B)
            cur_way[pp.first]+=pp.second*itt->second;
        }
        cur_way.erase(itt);
        itt=cur_way.begin();
        if(itt==cur_way.end()){
            //impossible to make a sentence with this dictionary
            cout << 0 << endl;
            return 0;
        }
    }
    cout<<itt->second<< endl;
    //dic_free(dictionary);
    return 0;
}