#include "file.h"
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

ostream& operator<<(ostream& ost, const file& rhs)
{
    string s_name, s_loc, s_size;

    s_name = number_to_string(rhs.name);

    s_loc = number_to_string(rhs.loc); s_size = number_to_string(rhs.size);

    ost << left << setw(11) << s_name
        << left << setw(11) << s_loc
        << left << setw(11) << s_size
        << left << setw(11) << rhs.file_op;

    return ost;
}

file::file()
{
    name = 'x';
    file_op = 'x';
    size = 0;
    loc = 0;
}

file::file(string &name, char &new_file_op, int &size, int &loc)
{
    this->name = name;
    file_op = new_file_op;
    this->size = size;
    this->loc = loc;

}

file::file(const file& inp)
{
    name = inp.name;
    file_op = inp.file_op;
    size = inp.size;
    loc = inp.loc;
}

file& file::operator=(const file& inp)
{
    name = inp.name;
    file_op = inp.file_op;
    size = inp.size;
    loc = inp.loc;
}

void file::change_parameters(string &new_name, char &new_file_op, int &new_size, int &new_loc)
{
    name = new_name;
    file_op = new_file_op;
    size = new_size;
    loc = new_loc;
}

int file::return_loc() const
{
    return loc;
}

template<class T>
string number_to_string(const T& input)
{
    ostringstream buffer;
    buffer << input;

    string output = buffer.str();

    if(output.length() > 10) return output.substr(0, 9);
    else return output;
}
