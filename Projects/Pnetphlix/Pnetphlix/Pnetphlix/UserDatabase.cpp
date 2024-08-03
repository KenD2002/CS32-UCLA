#include "UserDatabase.h"
#include "User.h"
#include "treemm.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

UserDatabase::UserDatabase()
:m_hasLoaded(false)
{}

bool UserDatabase::load(const string& filename)
{
    std::ifstream infile(filename);    // infile is a name of our choosing
    if (!infile || m_hasLoaded)                // Did opening the file fail or has already loaded before?
    {
        return false;
    }
    
    std::string s;
    std::string fullName;
    std::string email;
    std::vector<std::string> watch_history;
    while(std::getline(infile, s))
    {
        watch_history.clear();
        fullName = s; // get names
        std::getline(infile, s);
        email = s; // get emails
        std::getline(infile, s);
        while(std::getline(infile, s)) //get watch_history
        {
            if(s.size() != 0)
            {
                watch_history.push_back(s);
            }
            else // reached new blank line
            {
                User user(fullName, email, watch_history);
                m_UserDatabase.insert(email, user);
                break;
            }
        }
    }
    
    User user(fullName, email, watch_history);
    m_UserDatabase.insert(email, user);
    m_hasLoaded = true;
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    if(!m_UserDatabase.find(email).is_valid())
    {
        return nullptr;
    }
    
    return &(m_UserDatabase.find(email).get_value());
}
