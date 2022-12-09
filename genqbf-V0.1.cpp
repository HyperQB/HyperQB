#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>




using namespace std;

//The function for finding indexes of a text
vector<int> substrPosition(string str, string sub_str) {
   bool flag = false;
   
   vector< int > arr;
   
   for (int i = 0; i < str.length(); i++) {
      if (str.substr(i, sub_str.length()) == sub_str) {
                        
         
         arr.push_back(i);
         
         
         
         flag = true;
      }
   }

   return arr;
}








//The function for until unroller

string until_unroller(string modelA, string modelB, int k, vector<string> added_strs_A, vector<string> added_strs_B){
       
       
       
       string final_return;
       
       string new_A = modelA;
       string new_B = modelB;
       string paranthesis;
       
       for (int i = 0; i < k+1; i++){
           
           
           
           new_B.insert(new_B.length(), added_strs_B[i]);
           new_A.insert(new_A.length(), added_strs_A[i]);
           
           
           if (i<k-1){
           
           final_return += new_B;
           final_return += "\\/";
           final_return += "(";
           final_return += new_A;
           final_return += "/\\";
           final_return += "(";
           
           paranthesis += "))";
           
           
           
           
           
           }
           
           else if (i==k-1){
                
                final_return += new_B;
                final_return += "\\/";
                
                final_return += new_A;
                final_return += "/\\";
                
                
                }
           
           else if (i==k){

                
                
                final_return += new_B;
                final_return += paranthesis;
                
                }
                
           new_A = modelA;
           new_B = modelB;
             
           
           
           }
           
       
       return final_return;
       
       
       }










//The function for release unroller

string release_unroller(string modelA, string modelB, int k, vector<string> added_strs_A, vector<string> added_strs_B){
       
       
       
       string final_return;
       
       string new_A = modelA;
       string new_B = modelB;
       string paranthesis;
       
       for (int i = 0; i < k+1; i++){
           
           
           
           new_B.insert(new_B.length(), added_strs_B[i]);
           new_A.insert(new_A.length(), added_strs_A[i]);
           
           
           if (i<k-1){
           
           final_return += new_B;
           final_return += "/\\";
           final_return += "(";
           final_return += new_A;
           final_return += "\\/";
           final_return += "(";
           
           paranthesis += "))";
           
           
           
           
           
           }
           
           else if (i==k-1){
                
                
                
           final_return += new_B;
           final_return += "/\\";
           
           final_return += new_A;
           final_return += "\\/";
           
                
                }
           
           else if (i==k){
                
                
                final_return += new_B;
                final_return += paranthesis;
                
                }
                
           new_A = modelA;
           new_B = modelB;
             
           
           
           }
           
       
       return final_return;
       
       
       }






//The function for unrolling initial conditions

string I_unroller(int k, string I_file) {


	

//	string I_file = I_file;
//	int k = k;
	int bit = 5;
	string ad_I = "";
	string I = "";


	fstream  mini_I;
	mini_I.open(I_file, ios::in);
	
	while(!mini_I.eof()){
                     
             string added;
                     
             mini_I >> added;
             I += added;
              
                     
                     }
	
	
	
	
	
	
	

	for (int i = 0; i < bit; i++) {

		string s = to_string(i);

		if (i == 0) {

			ad_I += "(";

		}

		ad_I += "~n_";
		ad_I += s;

		if (i != bit-1) {

			ad_I += "/\\";

		}


		if (i == bit-1) {

			ad_I += ")/\\";

		}

	}

//	mini_I >> I;


	char* I_str = &I[0];
	char added_str[6] = { '_','A','_','[','0',']' };
	
	int added_l = end(added_str)-begin(added_str);
	
	string new_I = I_str;
	

	
//	int len = new_I.length();
	
 
   string sub_str = "/\\";
   
   vector<int> positions = substrPosition(new_I, sub_str);

 
   int counter = 0;
 
   for (int i = 0; i<positions.size(); i++){
        
         //cout<<positions[i];
         
         if (new_I[positions[i]+counter-1] != ')'){
            new_I.insert(positions[i]+counter, added_str);
            
            counter += added_l;
            }
            
         
        
        }
        
   
   
   sub_str = "\\/";
   
   positions = substrPosition(new_I, sub_str);

 
   counter = 0;
 
   for (int i = 0; i<positions.size(); i++){
        
         //cout<<positions[i];
         
         if (new_I[positions[i]+counter-1] != ')'){
            new_I.insert(positions[i]+counter, added_str);
            
            counter += added_l;
            }
            
         
        
        }

 
        
   sub_str = ")";
   
   positions = substrPosition(new_I, sub_str);

 
   counter = 0;
 
   for (int i = 0; i<positions.size(); i++){
        
         //cout<<positions[i];
         if (new_I[positions[i]+counter-1] != ')'){
            new_I.insert(positions[i]+counter, added_str);
            
            counter += added_l;
            }
         
        
        }
        
        
        
        
   sub_str = "<->";
   
   positions = substrPosition(new_I, sub_str);

 
   counter = 0;
 
   for (int i = 0; i<positions.size(); i++){
        
         //cout<<positions[i];
         if (new_I[positions[i]+counter-1] != ')'){
            new_I.insert(positions[i]+counter, added_str);
            
            counter += added_l;
            }
         
        
        }
        


	
	
 //   cout<<"This is Unrolled I:\n"<<new_I;	





	return new_I;

}















//The function for unrolling translation relations

string R_unroller(int k, string R_file) {

	//step1: unrolling the model

	
   
	
//	int k = k;
//	string R_file = R_file;
	string ad_R = "";
	string R = "";


	fstream  mini_R;
	mini_R.open(R_file);
	
	

//	mini_R >> R;


while(!mini_R.eof()){
                     
             string added;
                     
             mini_R >> added;
             R += added;
          
                     
                     }


char* R_str = &R[0];
string new_R = R_str;


//	char* R_str = &R[0];
	char added_str[6] = { '_','A','_','[','0',']' };
	
	vector<string> added_strs;
	
	for (int i = 0; i<k; i++){
        
        string s = to_string(i);
        char const *pchar = s.c_str();
        
        added_str[4] = *pchar;
        
    //    cout<<added_str;
          
        
        added_strs.push_back(added_str);
        
        }
        
        
        
        
	
	int added_l = end(added_str)-begin(added_str);
	
//	string new_R = R_str;

    	
	string final_R;
	
	

	
//	int len = new_I.length();

   string sub_str;
   vector<int> positions;
   int counter;
   int i;
   
   
   
   sub_str = ")";
               
               positions = substrPosition(new_R, sub_str);
            
             
               counter = 0;
               
               
  
    
    for (int t = 0; t<k-1; t++){ 
           

     
               sub_str = "/\\";
               
               positions = substrPosition(new_R, sub_str);
            
             
               counter = 0;
             
               for (int i = 0; i<positions.size(); i++){
                    
                     //cout<<positions[i];
                     
                     if (new_R[positions[i]+counter-1] != ')'){
                         if (new_R[positions[i]+counter-1] == '\''){
                        
                        new_R.insert(positions[i]+counter, added_strs[t+1]);
                        }
                        else{
                             new_R.insert(positions[i]+counter, added_strs[t]);
                             }
                             counter += added_l;
                        }
                        
                     
                    
                    }
                    
       //        cout<<positions.size()<<"\n";
               
               sub_str = "\\/";
               
               positions = substrPosition(new_R, sub_str);
            
             
               counter = 0;
             
               for (int i = 0; i<positions.size(); i++){
                    
                     //cout<<positions[i];
                     
                     if (new_R[positions[i]+counter-1] != ')'){
                         if (new_R[positions[i]+counter-1] == '\''){
                        
                        new_R.insert(positions[i]+counter, added_strs[t+1]);
                        }
                        else{
                             new_R.insert(positions[i]+counter, added_strs[t]);
                             }
                             counter += added_l;
                             }
                             
                        
                        }
            
      //       cout<<positions.size()<<"\n";
                    
               sub_str = ")";
               
               positions = substrPosition(new_R, sub_str);
            
             
               counter = 0;
               
               
                  
           
               for (int i = 0; i<positions.size(); i++){
                    

                     if (new_R[positions[i]+counter-1] != ')'){
                         if (new_R[positions[i]+counter-1] == '\''){
                        
                        new_R.insert(positions[i]+counter, added_strs[t+1]);
                        }
    
                        else{
                             
                        
                                 new_R.insert(positions[i]+counter, added_strs[t]);
                             }
                             counter += added_l;
                            }
                        
                        }
                        
                        
                        
                        
                        
                        
             sub_str = "<->";
               
               positions = substrPosition(new_R, sub_str);
            
             
               counter = 0;
               
               
                  
           
               for (int i = 0; i<positions.size(); i++){
                    

                     if (new_R[positions[i]+counter-1] != ')'){
                         if (new_R[positions[i]+counter-1] == '\''){
                        
                        new_R.insert(positions[i]+counter, added_strs[t+1]);
                        }
    
                        else{
                             
                        
                                 new_R.insert(positions[i]+counter, added_strs[t]);
                             }
                             counter += added_l;
                            }
                        
                        }
                        
                        

                        
                
                        
       final_R += new_R; 
       new_R = R_str;
                    
       if (t != k-2){
         final_R += "/\\";
         }
 
 
     
     }
        
        
        
        

    
//	ad_R += new_R;

    final_R.erase(remove(final_R.begin(), final_R.end(), '\''), final_R.end());

	
	
 //   cout<<"This is Unrolled R:\n"<<final_R;	





	return final_R;

}










































//This is the function for unrolling our formula

string formula_unroller(int k, string P_file) {



	
	string ad_P = "";
	string P = "";


	fstream  mini_P;
	mini_P.open(P_file);


while(!mini_P.eof()){
                     
             string added;
                     
             mini_P >> added;
             P += added;
        //     P += " ";  
                     
                     }



char* P_str = &P[0];
string new_P = P_str;


//	char* R_str = &R[0];
	
	
	
	
	char A[6] = { '_','A','_','[','0',']' };
	
	vector<string> added_strs_A;
	vector<string> added_strs_B;
	
	string B = "_B_[0]";
	
//	char B[6] = { '_','B','_','[','0',']' };
	
	for (int i = 0; i<k+1; i++){
        
        string s = to_string(i);
        char const *pchar = s.c_str();
        
        A[4] = *pchar;
        B[4] = *pchar;
        

          
        
        added_strs_A.push_back(A);
        
        
        
        added_strs_B.push_back(B);

        
        
        
        }
        






        







//Detecting G, F signs:
            
          vector<int> positions_G;
            
          string sub_str = "G(";
               
          positions_G = substrPosition(new_P, sub_str);
          
          string P_nextgen = "";
          string end_sign = "";
          string P_reference;
          
          int added_l = end(A)-begin(A);
          
          string final_P;
          
          vector<int> positions;
          int counter;
          
          
          
          if (positions_G.size()>0){
          
          
          
             
               for (int i = positions_G[0]+1; i<new_P.length(); i++){
                   
                   if (new_P[i] == 'F' || new_P[i] == 'G'){
                                if (new_P[i+1] == '(' || new_P[i+1] == '~'){
                                
                            
                                
                                end_sign = P_nextgen[P_nextgen.length()-2];
                                end_sign += P_nextgen[P_nextgen.length()-1];
                                
                                P_nextgen = P_nextgen.substr(0, P_nextgen.size()-1);
                                P_nextgen = P_nextgen.substr(0, P_nextgen.size()-1);
                                
                                 break;
                                 }
                                
                                }
                   
                   P_nextgen += new_P[i];
                     
                    
                    } 
                    
                    P_reference = P_nextgen; 
                     
   
    
        for (int t = 0; t<k; t++){ 
           

     
                sub_str = "/\\";
             
                positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
             
               for (int i = 0; i<positions.size(); i++){
                    
                    
                     
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                                                           
                             if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }               
                             
                        }
                        
                     
                    
                    }
                    
              
               
               sub_str = "\\/";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
             
               for (int i = 0; i<positions.size(); i++){
                    
                    
                     
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                         
                              if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }   
                             }
                             
                        
                        }
            
      
                    
              sub_str = ")";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
               
               
                  
            
               for (int i = 0; i<positions.size(); i++){
                    
             //        cout<<positions[i]<<endl;
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                        
                                 if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }   
                            }
                        
                        }
                        
                        
                        
                      
                      
                      
                      
                        
                        
                        
               sub_str = "<->";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
               
               
                  
            
               for (int i = 0; i<positions.size(); i++){
                    
             //        cout<<positions[i]<<endl;
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                        
                                 if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }   
                            }
                        
                        }
                        
                        
                
                        
       final_P += P_nextgen; 
       P_nextgen = P_reference;
                    
       if (t != k-1){
         final_P += "/\\";
         }
 
 
     
     }
     
     final_P += end_sign;
    
//    cout<<final_P<<"\n";
    
    
    

   }
   
   
   
   
   
         
         
         
         
         
         
         
         
         
   
         
            
         sub_str = "G~(";
               
         positions_G = substrPosition(new_P, sub_str);
          
          P_nextgen = "";
          end_sign = "";
          
          if (positions_G.size()>0){
          
          
          
             
               for (int i = positions_G[0]+1; i<new_P.length(); i++){
                   
                   if (new_P[i] == 'F' || new_P[i] == 'G'){
                                if (new_P[i+1] == '(' || new_P[i+1] == '~'){
                                
                            
                                
                                end_sign = P_nextgen[P_nextgen.length()-2];
                                end_sign += P_nextgen[P_nextgen.length()-1];
                                
                                P_nextgen = P_nextgen.substr(0, P_nextgen.size()-1);
                                P_nextgen = P_nextgen.substr(0, P_nextgen.size()-1);
                                
                                 break;
                                 }
                                
                                }
                   
                   P_nextgen += new_P[i];
                     
                    
                    } 
                    
                    P_reference = P_nextgen; 
   
   
    
        for (int t = 0; t<k; t++){ 
           

     
                sub_str = "/\\";
             
                positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
             
               for (int i = 0; i<positions.size(); i++){
                    
                    
                     
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                         
                             if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }      
                        }
                        
                     
                    
                    }
                    
              
               
               sub_str = "\\/";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
             
               for (int i = 0; i<positions.size(); i++){
                    
                    
                     
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                         
                             if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }      
                             }
                             
                        
                        }
            
      
                    
              sub_str = ")";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
               
               
                  
            
               for (int i = 0; i<positions.size(); i++){
                    
                //     cout<<positions[i]<<endl;
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                        
                                  if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }      
                            }
                        
                        }
                        
                        
                        
               
               
               
               sub_str = "<->";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
               
               
                  
            
               for (int i = 0; i<positions.size(); i++){
                    
             //        cout<<positions[i]<<endl;
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                        
                                 if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }   
                            }
                        
                        }
                        
                        
                
                        
       final_P += P_nextgen; 
       P_nextgen = P_reference;
                    
       if (t != k-1){
         final_P += "/\\";
         }
 
 
     
     }
     
     final_P += end_sign;
    
//    cout<<final_P<<"\n";
    
    
    

   }
    
    
         
          
          
          
          
          
          
          
          
          
          
          vector<int> positions_F;
          sub_str = "F(";
               
          positions_F = substrPosition(new_P, sub_str);
          
          P_nextgen = "";
          end_sign = "";
          
          if (positions_F.size()>0){
                                    
          
          
             
               for (int i = positions_F[0]+1; i<new_P.length(); i++){
                   
                   if (new_P[i] == 'F' || new_P[i] == 'G'){
                                if (new_P[i+1] == '(' || new_P[i+1] == '~'){
                                
                            
                                
                                end_sign = P_nextgen[P_nextgen.length()-2];
                                end_sign += P_nextgen[P_nextgen.length()-1];
                                
                                P_nextgen = P_nextgen.substr(0, P_nextgen.size()-1);
                                P_nextgen = P_nextgen.substr(0, P_nextgen.size()-1);
                                
                                 break;
                                 }
                                
                                }
                   
                   P_nextgen += new_P[i];
                   
                     
                    
                    } 
                    
                    P_reference = P_nextgen; 
                    
                    

   
    
        for (int t = 0; t<k; t++){ 
           

     
                sub_str = "/\\";
             
                positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
             
               for (int i = 0; i<positions.size(); i++){
                    
                    
                     
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                         
                              if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }      
                        }
                        
                     
                    
                    }
                    
              
               
               sub_str = "\\/";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
             
               for (int i = 0; i<positions.size(); i++){
                    
                    
                     
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                         
                              if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }      
                             }
                             
                        
                        }
            
      
                    
              sub_str = ")";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
               
               
                  
            
               for (int i = 0; i<positions.size(); i++){
                    
                 //    cout<<positions[i]<<endl;
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                        
                                 if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }      
                            }
                        
                        }
                        
                        
                        
                        
                        
                        
               sub_str = "<->";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
               
               
                  
            
               for (int i = 0; i<positions.size(); i++){
                    
             //        cout<<positions[i]<<endl;
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                        
                                 if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }   
                            }
                        
                        }
                        
                        
                
                        
       final_P += P_nextgen; 
       P_nextgen = P_reference;
                    
       if (t != k-1){
         final_P += "\\/";
         }
 
 
     
     }
     
     final_P += end_sign;
    
 
    
    
    

   }
   
   
   
   
   
   
   
   
   
   
   
   
   
   
        positions_F;
          sub_str = "F~(";
               
          positions_F = substrPosition(new_P, sub_str);
          
          P_nextgen = "";
          end_sign = "";
          
          if (positions_F.size()>0){
          
          
          
             
               for (int i = positions_F[0]+1; i<new_P.length(); i++){
                   
                   if (new_P[i] == 'F' || new_P[i] == 'G'){
                                if (new_P[i+1] == '(' || new_P[i+1] == '~'){
                                
                            
                                
                                end_sign = P_nextgen[P_nextgen.length()-2];
                                end_sign += P_nextgen[P_nextgen.length()-1];
                                
                                P_nextgen = P_nextgen.substr(0, P_nextgen.size()-1);
                                P_nextgen = P_nextgen.substr(0, P_nextgen.size()-1);
                                
                                 break;
                                 }
                                
                                }
                   
                   P_nextgen += new_P[i];
                     
                    
                    } 
                    
                    P_reference = P_nextgen; 
                    

   
    
        for (int t = 0; t<k; t++){ 
           

     
                sub_str = "/\\";
             
                positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
             
               for (int i = 0; i<positions.size(); i++){
                    
                    
                     
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                                                           
                          if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }      
                        }
                        
                     
                    
                    }
                    
              
               
               sub_str = "\\/";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
             
               for (int i = 0; i<positions.size(); i++){
                    
                    
                     
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                         
                              if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }      
                             }
                             
                        
                        }
            
      
                    
              sub_str = ")";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
               
               
                  
            
               for (int i = 0; i<positions.size(); i++){
                    
                  //   cout<<positions[i]<<endl;
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                        
                                  if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }      
                            }
                        
                        }
                        
                        
                        
                        
                        
                        
                        
               sub_str = "<->";
               
               positions = substrPosition(P_nextgen, sub_str);
            
             
               counter = 0;
               
               
                  
            
               for (int i = 0; i<positions.size(); i++){
                    
             //        cout<<positions[i]<<endl;
                     if (P_nextgen[positions[i]+counter-1] != ')'){
                        
                                 if (P_nextgen[positions[i]+counter-2] == 'A'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_A[t]);
                                
                                counter += added_l;
                                
                                }
                             else if (P_nextgen[positions[i]+counter-2] == 'B'){
                         
                                P_nextgen.insert(positions[i]+counter, added_strs_B[t]);
                                
                                counter += added_l;
                                
                                }   
                            }
                        
                        }
                        
                        
             
                        
       final_P += P_nextgen; 
       P_nextgen = P_reference;
                    
       if (t != k-1){
         final_P += "\\/";
         }
 
 
     
     }
     
     
     
     final_P += end_sign;
    
   
    
    
    

   }
   
   
   
   
   
   
        vector<int> positions_U;
          sub_str = "]U";
               
          positions_U = substrPosition(new_P, sub_str);
          
          
          
          P_nextgen = "";
          end_sign = "";
          
          if (positions_U.size()>0){
                                    
                                    
               sub_str = "(";
               
               positions = substrPosition(new_P, sub_str);
               
               
               
               int minimum = new_P.length();
               int positions_1;
               
               for (int i=0; i<positions.size(); i++){
                   
                   
                  if ((positions_U[0] - positions[i])>0 && (positions_U[0] - positions[i])<minimum){
                                      
                                      
                                      
                                      minimum = positions_U[0] - positions[i];
                                      
                                      positions_1 = positions[i];
                                      
                                      
                                      }
             
               
               
               }
               
               
               
               
               string atom_A;
               string atom_B;
               
               for (int i=positions_1+1; i<positions_U[0]+1; i++){
                   
                   
                   atom_A += new_P[i];
                   
                   
                   
                   }
            
               
                    
              sub_str = ")";
               
               positions = substrPosition(new_P, sub_str);
               
               
               
               
               
               minimum = new_P.length();
               int positions_2;
               
               for (int i=0; i<positions.size(); i++){
                   
                   
                   
                   
                  if ((positions[i]-positions_U[0])>0 && (positions[i]-positions_U[0])<minimum){
                                      
                                      minimum = positions[i]-positions_U[0];
                                      
                                      
                                      
                                      positions_2 = positions[i];
                                      
                                      
                                      }
             
               
               
               }
               
               
               
              
               
               for (int i=positions_U[0]+2; i<positions_2; i++){
                   
                   
                   atom_B += new_P[i];
                   
                   
                   
                  }
                   
                   
                   
           //   cout<<atom_A<<endl;
          //    cout<<atom_B<<endl;
               
               
              string output_U =  until_unroller(atom_A, atom_B, k, added_strs_A, added_strs_B); 
              
          //    cout<<output_U<<endl;
              
              final_P += output_U;
                  
            
   
            }
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
      vector<int> positions_R;
          sub_str = "]R";
               
          positions_R = substrPosition(new_P, sub_str);
          
          
          
          P_nextgen = "";
          end_sign = "";
          
          if (positions_R.size()>0){
                                    
                                    
               sub_str = "(";
               
               positions = substrPosition(new_P, sub_str);
               
               
               
               int minimum = new_P.length();
               int positions_1;
               
               for (int i=0; i<positions.size(); i++){
                   
                   
                  if ((positions_R[0] - positions[i])>0 && (positions_R[0] - positions[i])<minimum){
                                      
                                      
                                      
                                      minimum = positions_R[0] - positions[i];
                                      
                                      positions_1 = positions[i];
                                      
                                      
                                      }
             
               
               
               }
               
               
               
               
               string atom_A;
               string atom_B;
               
               for (int i=positions_1+1; i<positions_R[0]+1; i++){
                   
                   
                   atom_A += new_P[i];
                   
                   
                   
                   }
            
               
                    
              sub_str = ")";
               
               positions = substrPosition(new_P, sub_str);
               
               minimum = new_P.length();
               int positions_2;
               
               for (int i=0; i<positions.size(); i++){
                   
                   
                  if ((-positions_R[0]+positions[i])>0 && (-positions_R[0]+positions[i])<minimum){
                                      
                                      minimum = -positions_R[0]+positions[i];
                                      
                                      positions_2 = positions[i];
                                      
                                      
                                      }
             
               
               
               }
               
               
               
               
               
               for (int i=positions_R[0]+2; i<positions_2; i++){
                   
                   
                   atom_B += new_P[i];
                   
                   
                   
                  }
                   
                   
                   
          //    cout<<atom_A<<endl;
          //    cout<<atom_B<<endl;
               
               
              string output_R =  release_unroller(atom_A, atom_B, k, added_strs_A, added_strs_B); 
              
              final_P += output_R;
                  
            
   
            }
            
            
            
            
   
   
   

     
     
        int final_len =  final_P.length();  
              
            
 
 
 
     	for (int i = 0; i<final_len; i++){
            
            string s = "[A]";

            string::size_type x = final_P.find(s);
 
            if (x != string::npos)
              final_P.erase(x, s.length());
              
            s = "[B]";

            x = final_P.find(s);
 
            if (x != string::npos)
              final_P.erase(x, s.length());
              
              
              
             
        }

       



	return final_P;

}
























//Our main function to run

int main(int argc, char** argv) {
    
        
    
//Defining and arranging the variables for taking the corresponding inputs
   
	
	int k;
	string I_file;
	string R_file;
	string J_file;
	string S_file;
	string formula_file;
	
	
	
	char* a = argv[1];	
	k = atoi(a);	

	I_file = argv[2];
	R_file = argv[3];
	J_file = argv[4];
	S_file = argv[5];
	formula_file = argv[6];

	
	
//calling the I_unroller function to unroll both the initial conditions	
	
    
	string unrolled_I = I_unroller(k, I_file);
	string unrolled_J = I_unroller(k, J_file);
	
	

//calling the R_unroller function to unroll both the translation relations		
	
	string unrolled_R = R_unroller(k, R_file);
	string unrolled_S = R_unroller(k, S_file);
	
	
//calling the formula_unroller function to unroll our formula
	
	string unrolled_formula = formula_unroller(k, formula_file);
	
	

	
	
    //Writing the output into an integrated file
       
       ofstream outdata;
       outdata.open("output.txt"); // opens the file
       if( !outdata ) { // file couldn't be opened
          cerr << "Error: file could not be opened" << endl;
          exit(1);
       }
    
       
       outdata<<"This is unrolled I: "<<unrolled_I<<endl<<endl<<endl;
	   outdata<<"This is unrolled J: "<<unrolled_J<<endl<<endl<<endl;
	   outdata<<"This is unrolled R: "<<unrolled_R<<endl<<endl<<endl;
	   outdata<<"This is unrolled S: "<<unrolled_S<<endl<<endl<<endl;
	   outdata<<"This is unrolled formula: "<<unrolled_formula<<endl;
       outdata.close();




return 0;	
	
	
}










