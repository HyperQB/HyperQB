#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;


//Recreating genqbf using C++

int main() {

	//step1: unrolling the model

	//Unrolling the inital condition
	int k = 3;
	int bit = 3;
	string ad_I = "";
	string I = "";

	fstream  mini_I;
	mini_I.open("mini_I.bool", ios::in);

	for (int i = 0; i < bit; i++) {

		string s = to_string(i);

		if (i == 0) {

			ad_I += "(";

		}

		ad_I += "~n_";
		ad_I += s;

		if (i != bit - 1) {

			ad_I += "/\\";

		}


		if (i == bit - 1) {

			ad_I += ")";

		}

	}

	mini_I >> I;


	char* I_str = &ad_I[0];
	char added_str[6] = { '_','A','_','[','0',']' };
	// Tzu-Han: try not to have added_str as a static char array,
	// since you might have for exameple "_A_[11] as suffix which will be in length 7, not 6.
	// and all the code below will be in errors.
	//Ali: In terms of R, I totally agree with you. However, about I, since it is the time step number zero, We should not be worry about it ;)



	int len = sizeof(I_str) / sizeof(I_str[0]);

	for (int j = 0; j < len; j++) {

		if (I_str[j] == '/') {

			if (I_str[j + 1] == '\\') {

				if (I_str[j - 1] != ')') {

					for (int i = j - 6; i < len; i++) {

						I_str[i + 6] = I_str[i];

					}



					for (int i = j - 6; i < j; i++) {

						I_str[i] = added_str[6 - j - i];

					}

				}
			}
		}

		else if (I_str[j] == '\\') {

			if (I_str[j + 1] == '/') {

				if (I_str[j - 1] != ')') {

					for (int i = j - 6; i < len; i++) {

						I_str[i + 6] = I_str[i];

					}



					for (int i = j - 6; i < j; i++) {

						I_str[i] = added_str[6 - j - i];

					}

				}
			}
		}

		else if (I_str[j] == ')') {

			if (I_str[j - 1] != ')') {

				for (int i = j - 6; i < len; i++) {

					I_str[i + 6] = I_str[i];

				}



				for (int i = j - 6; i < j; i++) {

					I_str[i] = added_str[6 - j - i];

				}

			}
		}
	}




	ad_I += I;

	//fstream  mini_J;
	//mini_J.open("mini_J.bool", ios::in );


	//fstream  mini_P;
	//mini_P.open("mini_P.bool", ios::in );


	//Unrolling the transition relation
	fstream  mini_R;
	mini_R.open("mini_R.bool", ios::in);

	string R = "";

	mini_R >> R;

	char* R_str = &R[0];
	// Tzu-Han: same problem here, don't make this static as length 6
	// since you will have suffixes such as "_A_[11]" which is in length 7
	//Ali: You are right. I'm trying to change the code a little bit (considering c as a char* instead of char) to resolve this issue ;)


	int l = sizeof(R_str) / sizeof(R_str[0]);

	for (int t = 0; t < k; t++) {
        
        
        //char* c = &R[0];
		char c = t;
		char added_str[6] = { '_','A','_','[',c,']' };

		for (int j = 0; j < l; j++) {

			if (R_str[j] == '/') {

				if (R_str[j + 1] == '\\') {

					if (R_str[j - 1] != ')') {

						for (int i = j - 6; i < l; i++) {

							R_str[i + 6] = R_str[i];

						}



						for (int i = j - 6; i < j; i++) {

							R_str[i] = added_str[6 - j - i];

						}

					}
				}
			}

			else if (R_str[j] == '\\') {

				if (R_str[j + 1] == '/') {

					if (R_str[j - 1] != ')') {

						for (int i = j - 6; i < l; i++) {

							R_str[i + 6] = R_str[i];

						}



						for (int i = j - 6; i < j; i++) {

							R_str[i] = added_str[6 - j - i];

						}

					}
				}
			}

			else if (R_str[j] == ')') {

				if (R_str[j - 1] != ')') {

					for (int i = j - 6; i < l; i++) {

						R_str[i + 6] = R_str[i];

					}



					for (int i = j - 6; i < j; i++) {

						R_str[i] = added_str[6 - j - i];

					}

				}
			}
		}
	}

	int l2 = sizeof(R_str) / sizeof(R_str[0]);

	R_str[l2] = '/';
	R_str[l2] = '\\';




	//fstream  mini_S;
	//mini_S.open("mini_S.bool", ios::in );



	return 0;

}
