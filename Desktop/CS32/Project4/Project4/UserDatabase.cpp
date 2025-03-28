#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

UserDatabase::UserDatabase()
{}

UserDatabase::~UserDatabase() {
   // TreeMultimap<string, User*>::Iterator it = m_users;
    for(int i = 0; i < v_users.size(); i++) {
        delete v_users.at(i);
    }
}

bool UserDatabase::load(const string& filename)
{
    ifstream infile(filename);
    if(!infile) {
        return false;
    }
    string name;
    string email;
    string movie;
        
    //read in the name
    while(getline(infile, name)) {
        //read the email
        getline(infile, email);
        //read the number of movies watched
        int numWatched;
        infile >> numWatched;
        infile.ignore(10000, '\n');
        //while number of movies watched does not equal zero read in the movie id and add it to the vector of movies
        vector<string> movie_list;
        while(numWatched > 0)  {
            getline(infile, movie);
            movie_list.push_back(movie);
//            cout << movie << endl;
            numWatched--;
        }
        //add it to the treemultimap of users
        User* x = new User(name, email, movie_list);
        v_users.push_back(x);
        m_users.insert(x->get_email(), x);
        getline(infile, name);
    }
    return true;  // Replace this line with correct code.
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<string, User*>::Iterator it = m_users.find(email);
    if(it.is_valid()) {
        //return the value if it is valid
        return it.get_value();
    }
    return nullptr;
}
