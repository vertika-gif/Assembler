#include <iostream>
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

// convert decimals to binary strings
string decimal_to_binary(int num){

	string dec;
	string temp;

	while(num != 0){
			int digit = num % 2;
			if(digit == 1){
				dec += '1';
			}else{
				dec += '0';
			}
		num /= 2;
	}

	reverse(dec.begin(), dec.end());
	int n = dec.length();

	for(int i = 0; i < (16 - n); i++){
		temp += '0';
	}

	temp += dec;
	return temp;
}


/*void First_Pass(unordered_map<string,string> Variables,vector<string*> v){
int lc=0,i=0;
int n=v.size();
while(n!=0){
    string temp=*(v[i]);
    if(temp[0] == ' '){
        if(temp.substr(1,3)=="ORG"){
            int len=temp.length();
            string num=temp.substr(5,len-5);
            lc=string_to_decimal(num);
        }
        else if(temp.substr(1,3)=="END"){
            return;
        }
    }
    else{
        string label;
        for(int j=0;j<4;j++){
            if(temp[j]==','){
                break;
            }
            else{
                label+=temp[j];
            }
        }
        Variables[label]=decimal_to_binary(lc);
    }
i++;
n--;
lc++;
}
}*/


int main()
{
    //hashmaps
    unordered_map<string,string> opcode;
    unordered_map<string,bool> pseudo;
    unordered_map<string,string> Register;
    unordered_map<string,string> addressing_mode;

//     IM -> IMMEDIATE
//     D -> DIRECT
//     # -> INDIRECT
//     IO -> INPUT/OUTPUT
//     R -> REGISTER

    pseudo["ORG"]=true;
    pseudo["HLT"]=true;
    pseudo["END"]=true;
    pseudo["HEX"]=true;
    pseudo["DEC"]=true;

    addressing_mode["IM"]="0000";
    addressing_mode["D"]="0011";
    addressing_mode["#"]="0010";
    addressing_mode["IO"]="0111";
    addressing_mode["R"]="1111";


    Register["R1"]="00100001";
    Register["R2"]="01011000";


    /*
	pseudo["END"]="0000000000000000";
    pseudo["ORG"]="0000000000000000";
    pseudo["HALT"]="0000000000000000";
    pseudo["DEC"]="0000000000000000";
    pseudo["HEX"]="0000000000000000";
	*/

    opcode["LD"]="000000000001";
    opcode["AND"]="000000010000";
    opcode["ADD"]="000000000010";
    opcode["XOR"]="000000000100";
    opcode["ADC"]="000000001111";
    opcode["XCHG"]="000000010001";
    opcode["STA"]="000000000101";
    opcode["BUN"]="000000000111";
    opcode["BSA"]="000000001010";
    opcode["NOT"]="000000100000";
    opcode["IN"]="000000100001";
    opcode["DEC"]="000000100101";
    opcode["SZ"]="111100000000";
    opcode["SP"]="111100000001";
    opcode["SN"]="111100010000";
    opcode["CLA"]="111100000010";
    opcode["CLE"]="111100000100";
    opcode["CLC"]="111100100000";
    opcode["CLZ"]="111100110000";
    opcode["CLR"]="111100100001";
    opcode["HLT"]="111111111111";
    opcode["CIR"]="111100110001";
    opcode["CIL"]="111101000000";
    opcode["CMA"]="111101000011";
    opcode["CME"]="111101001000";
    opcode["BLCFILL"]="111101001010";
    opcode["BLCI"]="111101001100";
    opcode["BLCIC"]="111101010011";
    opcode["BLCMSK"]="111101100000";
    opcode["BLCS"]="111110000111";
    opcode["INP"]="1101000000010000000000000000";
    opcode["OUT"]="1101000100110000000000000000";
    opcode["SKI"]="1101010000010000000000000000";
    opcode["SKO"]="1101010100100000000000000000";
    opcode["ION"]="1101011000110000000000000000";
    opcode["IOF"]="1101011101000000000000000000";

    //locally store symbolic program in vector of strings
    vector<string*> v;
    string s;
	// file inout
    ifstream input("cao_input.txt");
    if(input.is_open())
    {
        while(getline(input,s))
        {
            string* temp = new string;
            *temp=s;
            v.push_back(temp);
        }
        input.close();
    }

	/*ofstream output("cao_output.txt");
    output.open("cao_output.txt");
    for(int m=0;m<v.size();m++)
    {
        string temp;
        temp = *(v[m]);
        cout<<temp<<endl;
        output<<temp<<'/n';
    }*/
    unordered_map<string,string> Variables;
    //call for first pass

	// location counter
    int LC = 0;
	// counter for number of lines
	int i = 0;
	// number of lines in input
	int n = v.size();

	while ((n--) != 0){
		string temp = *(v[i++]);

		//TODO: change to check if there is no comma
		// check for pseudo instruction
		if(temp[0] == ' ') {
			if(temp.substr(1,3) == "ORG"){
				int len = temp.length();
				string num = temp.substr(5,len-5);
				LC = stoi(num);
			}
			else if(temp.substr(1,3) == "END"){
				break;
			}
		// case if label found
		} else {
			string label;

			for(int j = 0; j < 4; j++){
				if(temp[j] == ','){
					break;
				}
				else{
					label += temp[j];
				}
			}
			Variables[label] = decimal_to_binary(LC);
		}

		// go to next line
		LC++;
	}


    //second_pass

	// start scanning from the start again
	LC=0;

	for (int m = 0; m < v.size(); m++) {

        string current_instruction = *(v[m]);;
        string machine_code = "";

		//points to character in current_instruction
        int i = 0;


        //make i point to first character of opcode
		// if no label is found
		if(current_instruction[0] == ' ')
        {
            i = 1;
        }
        // if label found
		else
        {
           while(current_instruction[i] != ',')
           {
               i++;
           }
           i += 2;
		}

		// string that stores the opcode of the
        string op_code = "";

        // extract opcode and make i point to character after opcode
		// note what if this is does not contain any operand?
		while(current_instruction[i] != ' ')
        {
            op_code = op_code + current_instruction[i];
            i++;
        }

		// check if it is a pseudoinstruction
		if(pseudo.count(op_code) > 0)
        {
			// if ORG instruction update LC
            if(op_code == "ORG")
            {
			    int len = current_instruction.length();
			    string num = current_instruction.substr(5, len - 5);
			    LC = stoi(num);
            }

			// if END instruction break out and generate output
            else if(op_code=="END")
            {
                break;
            }
        }

		// otherwise check for non pseudo instruction
		else if(opcode.count(op_code) > 0)
        {
			// add bits of opcode to binary code
            machine_code = machine_code + opcode[op_code];

            i++;
            if(current_instruction[i] != '/'&&i<current_instruction.length())
            {
			   // store the name of the label or register in string var_reg
               string var_reg = "";
               while(current_instruction[i] != ' ')
               {
                  var_reg = var_reg + current_instruction[i];
                  i++;
               }

			   // if it is a label
               if(Variables.count(var_reg) > 0)
               {
                   machine_code = machine_code + Variables[var_reg];
               }
			   // if a register
               else if(Register.count(var_reg) > 0)
               {
                 machine_code = machine_code + Register[var_reg];
               }
			   // if an addressing mode
               else if(addressing_mode.count(var_reg) > 0)
               {
                   machine_code = addressing_mode[var_reg] + machine_code;
               }

               else
               {
                   bool flag=true;
                   // to check whether the operand ia an integer or not
                   for(int c=0;c<var_reg.length();c++)
                   {
                       if(isdigit(var_reg[c])==false)
                       {
                           flag=false;
                           break;
                       }
                   }
                   //if operand is an integer
                   if(flag==true)
                   {
                       int decimal_operand=stoi(var_reg);
                       string binary_operand = decimal_to_binary(decimal_operand);
                       machine_code=machine_code+binary_operand;
                   }
                   // nothing found output error
                   else
                   {
                       cout<<var_reg<<" : label not declared"<<endl;
                       exit(1);
                   }

               }
               i++;
            }



			if(current_instruction[i] != '/'&&i<current_instruction.length())
            {
               string var_reg = "";
               while (current_instruction[i] != ' ')
               {
                  var_reg = var_reg + current_instruction[i];
                  i++;
               }

               if (Variables.count(var_reg) > 0)
               {
                   machine_code=machine_code + Variables[var_reg];
               }
               else if(Register.count(var_reg) > 0)
               {
                 machine_code = machine_code + Register[var_reg];
               }
               else if(addressing_mode.count(var_reg) > 0)
               {
                   machine_code = addressing_mode[var_reg] + machine_code;
               }
               else
               {
                   bool flag=true;
                   // to check whether the operand ia an integer or not
                   for(int c=0;c<var_reg.length();c++)
                   {
                       if(isdigit(var_reg[c])==false)
                       {
                           flag=false;
                           break;
                       }
                   }
                   //if operand is an integer
                   if(flag==true)
                   {
                       int decimal_operand=stoi(var_reg);
                       string binary_operand = decimal_to_binary(decimal_operand);
                       machine_code=machine_code+binary_operand;
                   }
                   // nothing found output error
                   else
                   {
                       cout<<var_reg<<" : label not declared"<<endl;
                       exit(1);
                   }
               }
               i++;
            }

            if(current_instruction[i] != '/'&&i<current_instruction.length())
            {
               string var_reg = "";
               while(current_instruction[i] != ' ')
               {
                  var_reg=var_reg+current_instruction[i];
                  i++;
               }
               if(Variables.count(var_reg)>0)
               {
                   machine_code = machine_code + Variables[var_reg];
               }
               else if(Register.count(var_reg) > 0)
               {
                 machine_code = machine_code + Register[var_reg];
               }
               else if(addressing_mode.count(var_reg) > 0)
               {
                   machine_code = addressing_mode[var_reg] + machine_code;
               }
               else
               {
                   cout<<var_reg<<" : label not declared"<<endl;
                   exit(3);
               }
            }
            while(machine_code.length()<32)
                machine_code += "0";
            cout<<machine_code<<"\n";
        }

		else
        {
            cout<<op_code<<" : not an instruction"<<endl;
        }
        LC++;
    }

}
