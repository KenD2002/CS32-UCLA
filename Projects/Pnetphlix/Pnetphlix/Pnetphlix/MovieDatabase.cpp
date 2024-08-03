#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
using namespace std;

MovieDatabase::MovieDatabase()
:m_hasLoaded(false)
{}

MovieDatabase::~MovieDatabase()
{
    for(int i = 0; i < m_moviePointers.size(); i ++)
    {
        delete m_moviePointers[i];
    }
}

bool MovieDatabase::load(const string& filename)
{
    std::ifstream infile(filename);    // infile is a name of our choosing
    if (!infile || m_hasLoaded)                // Did opening the file fail or has already loaded before?
    {
        return false;
    }
    
    std::string s;
    std::string token;
    
    std::string id;
    std::string title;
    std::string releaseYear;
    std::vector<std::string> directors;
    std::vector<std::string> actors;
    std::vector<std::string> genres;
    float rating;
    
    while(std::getline(infile, s))
    {
        directors.clear();
        actors.clear();
        genres.clear();
        
        id = s; // get names
        std::getline(infile, s);
        title = s; // get emails
        std::getline(infile, s);
        releaseYear = s; // get release year
        
        std::getline(infile, s); // get the string line of directors, separated by comma
        std::stringstream ss1(s);
        while(std::getline(ss1, token, ','))
        {
            // Remove leading and trailing whitespace
            token.erase(0, token.find_first_not_of(" "));
            token.erase(token.find_last_not_of(" ") + 1);
            directors.push_back(token);
        }
        
        std::getline(infile, s); // get the string line of actors, separated by comma
        std::stringstream ss2(s);
        while (std::getline(ss2, token, ','))
        {
            // Remove leading and trailing whitespace
            token.erase(0, token.find_first_not_of(" "));
            token.erase(token.find_last_not_of(" ") + 1);
            actors.push_back(token);
        }
        
        std::getline(infile, s); // get the string line of genres, separated by comma
        std::stringstream ss3(s);
        while (std::getline(ss3, token, ','))
        {
            // Remove leading and trailing whitespace
            token.erase(0, token.find_first_not_of(" "));
            token.erase(token.find_last_not_of(" ") + 1);
            genres.push_back(token);
        }
        
        std::getline(infile, s); // get rating
        rating = std::stof(s);
        
        Movie *moviePtr = new Movie(id, title, releaseYear, directors, actors, genres, rating);
        
        std::string idLower(id); // insert with key lowered id
        for(auto& ch : idLower)
        {
            ch = std::tolower(ch);
        }
        m_idTree.insert(idLower, moviePtr);
        
        for(int i = 0; i < directors.size(); i ++) // insert with key lowered director
        {
            std::string directorLower(directors[i]);
            for(auto& ch : directorLower)
            {
                ch = std::tolower(ch);
            }
            m_directorsTree.insert(directorLower, moviePtr);
        }
        
        for(int j = 0; j < actors.size(); j ++) // insert with key lowered actor
        {
            std::string actorLower(actors[j]);
            for(auto& ch : actorLower)
            {
                ch = std::tolower(ch);
            }
            m_actorsTree.insert(actorLower, moviePtr);
        }
        
        for(int k = 0; k < genres.size(); k ++) // insert with key lowered genre
        {
            std::string genreLower(genres[k]);
            for(auto& ch : genreLower)
            {
                ch = std::tolower(ch);
            }
            m_genresTree.insert(genreLower, moviePtr);
        }
        
        m_moviePointers.push_back(moviePtr);
        if(!std::getline(infile, s)) // get the blank line
        {
            break;
        }
    }

    m_hasLoaded = true;
    return true;
}


Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    std::string idLower(id);
    for(auto& ch : idLower)
    {
        ch = std::tolower(ch);
    }
    
    if(!m_idTree.find(idLower).is_valid())
    {
        return nullptr;
    }
    
    return m_idTree.find(idLower).get_value();
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    std::string directorLower(director);
    for(auto& ch : directorLower)
    {
        ch = std::tolower(ch);
    }
    
    std::vector<Movie*> directorVec;
    if(!m_directorsTree.find(directorLower).is_valid())
    {
        return directorVec;
    }
    
    TreeMultimap<std::string, Movie*>::Iterator it = m_directorsTree.find(directorLower);
    while(it.is_valid())
    {
        directorVec.push_back(it.get_value());
        it.advance();
    }
    return directorVec;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    std::string actorLower(actor);
    for(auto& ch : actorLower)
    {
        ch = std::tolower(ch);
    }
    
    std::vector<Movie*> actorVec;
    if(!m_actorsTree.find(actorLower).is_valid())
    {
        return actorVec;
    }
    
    TreeMultimap<std::string, Movie*>::Iterator it = m_actorsTree.find(actorLower);
    while(it.is_valid())
    {
        actorVec.push_back(it.get_value());
        it.advance();
    }
    return actorVec;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    std::string genreLower(genre);
    for(auto& ch : genreLower)
    {
        ch = std::tolower(ch);
    }
    
    std::vector<Movie*> genreVec;
    if(!m_genresTree.find(genreLower).is_valid())
    {
        return genreVec;
    }
    
    TreeMultimap<std::string, Movie*>::Iterator it = m_genresTree.find(genreLower);
    while(it.is_valid())
    {
        genreVec.push_back(it.get_value());
        it.advance();
    }
    return genreVec;
}

