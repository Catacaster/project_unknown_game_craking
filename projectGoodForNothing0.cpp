#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <ctype.h>

int num_row(0), num_col(0); 								//number of rows/cols
std::vector<std::vector<std::vector<int>>> space_row;		//white space of row
std::vector<int> col_space;									//num of white space of col
std::vector<std::vector<std::pair<int, int>>> row_vector;	//first int is color, second one is num---row
std::vector<std::vector<std::pair<int, int>>> col_vector;	//first int is color, second one is num---col
void get_white_space(int value, int box, std::vector<int> &pool, std::vector<std::vector<int>> &res); //create white space
void initial_white_space(void);		//initial white space, and update space_row
void print_input(void);
void print_matrix(const std::vector<std::vector<int>> &m);
bool process_input(std::string s, std::vector<std::pair<int, int>> &res);
bool check_matrix(const std::vector<std::vector<int>> &m);
bool check(std::vector<std::vector<int>> &res, int row);	//row from 0 to num_row-1
int main(){
	std::cout << "********ProjectGoodForNothing********\n";
	//enter row
	while(num_row ==0){
		std::cout << "Enter input here for rows \n(number+character sets and enter, "
		 	  	  << "OR e for an empty row, OR q to stop entering): \n"
		 	  	  << "EX: 16o14b\n";
		std::string input_s;
		std::cin >> input_s;
		while(!(input_s.size()==1 && (input_s[0]=='q'))){
			std::vector<std::pair<int, int>> res;
			if(input_s.size() == 1 && (input_s[0]=='e'))
				row_vector.push_back(res);
			else if(!process_input(input_s, res))
				std::cout << "Bad data, not taken!\n";
			else
				row_vector.push_back(res);
			std::cin >> input_s;
		}
		num_row = row_vector.size();
	}
	//enter col
	while(num_col ==0){
		std::cout << "Enter input here for cols \n(number+character sets and enter, "
		 	  	  << "OR e for an empty row, OR q to stop entering): \n"
		 	  	  << "EX: 16o14b\n";
		std::string input_s;
		std::cin >> input_s;
		while(!(input_s.size()==1 && (input_s[0]=='q'))){
			std::vector<std::pair<int, int>> res;
			if(input_s.size() == 1 && (input_s[0]=='e'))
				col_vector.push_back(res);
			else if(!process_input(input_s, res))
				std::cout << "Bad data, not taken!\n";
			else
				col_vector.push_back(res);
			std::cin >> input_s;
		}
		num_col = col_vector.size();
	}
	print_input();
	//check for bad data, and update white space data structure
	for(int i=0; i<row_vector.size(); i++){
		int sum(0);
		for(int j=0; j<row_vector[i].size(); j++)
			sum += row_vector[i][j].second;
		if(sum > num_col){
			std::cout << "Error occur at row " << i+1
				 << "\nSum of color > number of cols\n";
			return(1);
		}
		else{
			std::vector<int> pool;
			std::vector<std::vector<int>> res;
			get_white_space(num_col - sum, row_vector[i].size()+1, pool, res);
			space_row.push_back(res);	//update
		}
	}
	for(int i=0; i<col_vector.size(); i++){
		int sum(0);
		for(int j=0; j<col_vector[i].size(); j++)
			sum += col_vector[i][j].second;
		if(sum > num_row){
			std::cout << "Error occur at col "<< i+1
					  << "\nSum of color > number of rows\n";
			return(1);
		}
		else{
			col_space.push_back(num_row - sum);
		}
	}
	
	std::vector<std::vector<int>> res;
	check(res, 0);
	return 0;
}

void get_white_space(int value, int box, std::vector<int> &pool, std::vector<std::vector<int>> &res){
	if(box == 0) return;
	else if(box == 1){
		pool.push_back(value);
		res.push_back(pool);
		pool.pop_back();
	}
	else{
		for(int i=0; i<=value; i++){
			pool.push_back(i);
			get_white_space(value-i, box-1, pool, res);
			pool.pop_back();
		}
	}	
}
//initial data structure space_row
void initial_white_space(void){
	for(int i=0; i<row_vector.size(); i++){
		int num_space = num_col;
		for(int j=0; j<row_vector[i].size(); j++){
			num_space -= row_vector[i][j].second;	//subtract num of colors
		}
		std::vector<int> pool;
		std::vector<std::vector<int>> res;
		get_white_space(num_space, row_vector[i].size()+1, pool, res);
		space_row.push_back(res);		//update data structure
	}
}

void print_input(void){
	for(int i=0; i<row_vector.size(); i++){
		//if has nothing print empty
		if(row_vector[i].size()==0) std::cout << "EMPTY";
		//else print all elements
		for(int j=0; j<row_vector[i].size(); j++)
			std::cout << "[" << (char)(row_vector[i][j].first-1+'a')<< ","
					  << row_vector[i][j].second << "]";
		std::cout << "\n";
	}
	std::cout <<"       ";
	for(int i=0; i<col_vector.size(); i++){
		std::cout << " {";
		if(col_vector[i].size()==0) std::cout << "EMPTY";
		for(int j=0; j<col_vector[i].size(); j++)
			std::cout << "[" <<(char)(col_vector[i][j].first-1+'a')<< ","
					  << col_vector[i][j].second << "]";
		std::cout << "} ";
	}
	std::cout << "\n";
}

bool process_input(std::string s, std::vector<std::pair<int, int>> &res){
	char c;
	int num, i(0);
	while(i < s.size()){
		num = 0;					//reset each time
		while(isspace(s[i])) i++;	//get rid of space
		//CHECK BAD DATA
		if(!isdigit(s[i])) return false;
		//input number
		while(isdigit(s[i])){
			num = num*10 + (s[i++] - '0');
		}
		while(isspace(s[i])) i++;	//get rid of space
		//input color
		if(isalpha(s[i]))
			c = tolower(s[i++]);
		else
			return false;	//BAD DATA
		//now insert
		res.push_back(std::make_pair(c-'a'+1, num));
	}
	return true;
}

void print_matrix(const std::vector<std::vector<int>> &m){
	for(int i=0; i<m.size(); i++){
		for(int j=0; j<m[i].size(); j++){
			int temp(m[i][j]);
			if(temp == 0)
				std::cout << " W_S ";
			else
				std::cout << "  " <<(char)('a'+temp-1) << "  ";
		}
		std::cout << "\n";
	}
}

bool check_matrix(const std::vector<std::vector<int>> &m){
	//check matrix against col vector
	for(int j=0; j<num_col; j++){
		int next(0), white_space(0), index(0);
		if(col_vector[j].size()!=0) next = col_vector[j][index].first;	//get first color
		int row(0);
		while(row < m.size()){
			if(m[row][j] == 0){
				white_space++;
				row++;
			}
			else{
				if(m[row][j] == next){
					//if match
					for(int i=0; row<m.size() && (i < col_vector[j][index].second); i++){
						if(m[row][j] != next) return false;	//does not mach
						else row++;							//row now in the next row
					}
					index++;
					if(index < col_vector[j].size())
						next = col_vector[j][index].first;
					else
						next = 0;	//rest can only be space
				}
				else return false;
			}
		}
		if(white_space > col_space[j]) return false;	//white space is greater than expacted 
	}
	return true;
}

bool check(std::vector<std::vector<int>> &res, int row){
	for(int i=0; i<space_row[row].size(); i++){
		std::vector<int> temp(space_row[row][i]);
		std::vector<int> try_row;
		int col(0), color(0), index_space(0), index_color(0);
		bool istSpace = true;
		while(col < num_col){
			if(istSpace){
				color=0;
				for(int i=0; i<temp[index_space]; i++){
					try_row.push_back(0);
					col++;
				}
				index_space++;
				istSpace = false;
			}
			else{
				color = row_vector[row][index_color].first;
				for(int i=0; i<row_vector[row][index_color].second; i++){
					try_row.push_back(color);
					col++;
				}
				index_color++;
				istSpace = true;
			}
		}
		res.push_back(try_row);	//push
		std::cout << "Now checking: \n";
		print_matrix(res);
		if(check_matrix(res)){
			if(row == num_row-1){
				std::cout << "FOUND!!!\n";
				return true;
			}
			else if(check(res, row+1)) return true;;
		}
		else 
			std::cout << "NOT MATCHING!\n";
		res.pop_back();	//pop
	}
	return false;
}
