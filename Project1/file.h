#include <string>
#include <sstream>
#include <iostream>

using namespace std;


class PCB;
ostream& operator<<(ostream& ost, const PCB& rhs);

class file
{
public:
	file();
	file(string &name, char &new_op, int &size, int &loc);
	file(const file& inp);
	file& operator=(const file& inp);
	void change_parameters(string &new_name, char &new_file_op, int &new_size, int &new_loc);
	friend ostream& operator<<(ostream& ost, const file& rhs);

private:
	string name;
	char file_op;
	int size;
	int loc;

};
