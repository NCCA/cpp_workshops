#include "database.h"

// Needed for cout and cerr
#include <iostream>

// Needed for reading from the file with ifstream (why not include these in the header?)
#include <fstream>


const char * Database::staff_expr = "([apr]) ([A-Za-z]+) ([A-Za-z]+) ([0-9/]+)";

/**
 * @brief Database::Database
 * @param _filename
 */
Database::Database(const std::string &_filename) {
    parse(_filename);
}

/**
 * @brief Database::~Database
 */
Database::~Database() {
    clear();
}

/**
 * @brief Database::clear
 * Erase the contents of the database
 */
void Database::clear() {
    // Implement a data structure specific method to erase the data in your database
}

/**
 * @brief Database::print dumps the database contents to screen
 */
void Database::print() {
    // How will you do this? The smart way might be to have a virtual print function for each database
    // entry. You then need to iterate over your storage structure and call the print function for
    // each entry. Other idea's exist!
}

/**
 * @brief Database::parse Parses the input file and inserts the entries into the DB.
 * I've gone with an approach which attempts to parse each line with a set of prebaked regular expressions.
 * This isn't the most efficient method, but is pretty flexible, and demonstrates several nifty concepts from
 * boost which might prove useful.
 * Note that this function should really do some exception handling - for example if the DOB isn't in the
 * right format, you could try some alternative formats instead.
 * @param _filename A filename to parse
 */
void Database::parse(const std::string &_filename) {
    // This will store our regular expression matching results. Plenty of reading matter here:
    // http://www.boost.org/doc/libs/1_57_0/libs/regex/doc/html/index.html
    boost::match_results<std::string::const_iterator> results;

    // This will store each line from the file
    std::string line;

    // The input file
    std::ifstream infile (_filename.c_str());

    // Sanity check - if not open something went wrong
    if (infile.is_open()) {
        while ( getline (infile,line) ) {
            // Perform regular expression matching - returns false if it isn't matched
            if (boost::regex_match(line, results, boost::regex(staff_expr))) {
                // Format should be:
                // results[1] code of role (a, p or r)
                std::string code_str = results[1];
                char code = code_str.c_str()[0];

                // results[2] first name
                std::string firstname = results[2];

                // results[3] last name
                std::string lastname = results[3];

                // results[4] date of birth (as a string)
                std::string dob_str = results[4];

                // Store the dob as a boost date (will be useful for exception handling)
                boost::gregorian::date dob(boost::gregorian::from_simple_string(dob_str));

                std::cout << "Parsed the following: code="<<code<<", firstname="<<firstname.c_str()<<
                             ", lastname="<<lastname.c_str()<<", dob="<<dob.day()<<" "<<dob.month()<<" "<<dob.year()<<"\n";

                // Now you have all the information you need to create a database entry.
            } else {
                // The input line did not match the regular expressions staff_regex or resource_regex
                std::cout << "Line "<<line.c_str()<<" could not be parsed!\n";
            }
        }
        // Be good and close up the opened file
        infile.close();
    }
}
