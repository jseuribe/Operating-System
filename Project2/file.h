#ifndef FILE_H
#define FILE_H
#include <string>
#include <sstream>
#include <iostream>

using namespace std;


class PCB;
ostream& operator<<(ostream& ost, const PCB& rhs);

template <class T>
string number_to_string(const T& input);

class file
{
public:
	file();
	file(string &name, char &new_op, int &size, int &loc);
	file(const file& inp);
	file& operator=(const file& inp);
	int return_loc() const;
	void change_parameters(string &new_name, char &new_file_op, int &new_size, int &new_loc);

	friend ostream& operator<<(ostream& ost, const file& rhs);

	template <class T>
    friend string number_to_string(const T& input);

private:
	string name;
	char file_op;
	int size;
	int loc;



};

#endif
