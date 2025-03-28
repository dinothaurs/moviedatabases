#include "User.h"

#include <string>
#include <vector>
using namespace std;

User::User(const string& full_name, const string& email,
           const vector<string>& watch_history)
{
    name = full_name;
    this->email = email;
    history = watch_history;
}

string User::get_full_name() const
{
    return name;
}

string User::get_email() const
{
    return email;
}

vector<string> User::get_watch_history() const
{
    return history;
}
