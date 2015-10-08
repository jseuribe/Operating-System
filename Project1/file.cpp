#ifndef FILE_H
#define FILE_H

#include "file.h"
#include <iomanip>
    
ostream& operator<<(ostream& ost, const file& rhs)
{
    ost << setw(16) << left << rhs.name
        << setw(16) << left << rhs.loc
        << setw(16) << left << rhs.size
        << setw(16) << left << rhs.file_op;

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

#endif
