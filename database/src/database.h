#ifndef DATABASE_H
#define DATABASE_H

// Needed for std::string 's
#include <string>

// Needed for parsing and for date/time structures
#include <boost/regex.hpp>
#include <boost/date_time.hpp>

/**
 * @brief The Database class
 * Somewhere inside of this class you should have a container storing different types of database entries.
 * Use an object oriented hierarchy to simplify the entry of different types of entries into the database.
 */
class Database
{
public:
    /// A constructor which takes an input filename
    Database(const std::string &_filename);

    /// A destructor, which wipes clean the database
    ~Database();

    /// Dump the contents of the database to the screen
    void print();

private:
    /// Parse an input file given the filename
    void parse(const std::string &_filename);

    /// Delete the database
    void clear();

    /// Regular expression used to match staff. Note use of constexpr - see http://en.cppreference.com/w/cpp/language/constexpr
    // Staff regular expression has code, firstname, lastname, birthdate
    static const char* staff_expr;
};



#endif // DATABASE_H
