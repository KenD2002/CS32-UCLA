#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"

#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
:m_MovieDatabase(movie_database), m_userDatabase(user_database)
{}


vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    // find user by email
    const User* user = m_userDatabase.get_user_from_email(user_email);
    if(user == nullptr || movie_count <= 0)
    {
        return std::vector<MovieAndRank>();
    }
    
    std::unordered_map <std::string, movieInfo> moviesAndScore; // first is Id, movieInfo is the information about the movie
    std::unordered_map <std::string, movieInfo>::iterator iter;
    
    std::vector<std::string> watchHistory = user->get_watch_history(); // IDs of movies watched
    
    // Directors correlated with watching history
    for(int i = 0; i < watchHistory.size(); i ++)
    {
        // Create a vector containing the directors of the current movie
        std::vector<std::string> watchedDirectors = m_MovieDatabase.get_movie_from_id(watchHistory[i])->get_directors();
        for(int j = 0; j < watchedDirectors.size(); j ++)
        {
            std::vector<Movie*> dirToMovieptrs = m_MovieDatabase.get_movies_with_director(watchedDirectors[j]);
            for(int k = 0; k < dirToMovieptrs.size(); k ++)
            {
                iter = moviesAndScore.find(dirToMovieptrs[k]->get_id());
                std::string movieID = dirToMovieptrs[k]->get_id();
                if (iter == moviesAndScore.end()) // haven't created an element for this movie
                {
                    moviesAndScore[movieID] = movieInfo();
                    moviesAndScore[movieID].m_score = 20;
                    moviesAndScore[movieID].m_movieRating = m_MovieDatabase.get_movie_from_id(movieID)->get_rating();
                    moviesAndScore[movieID].m_movieTitle = m_MovieDatabase.get_movie_from_id(movieID)->get_title();
                }
                else // this movie already exists, +20 points for it
                {
                    moviesAndScore[movieID].m_score += 20;
                }
            }
        }
        
        // Create a vector containing the actors of the current movie
        std::vector<std::string> watchedActors = m_MovieDatabase.get_movie_from_id(watchHistory[i])->get_actors();
        for(int j = 0; j < watchedActors.size(); j ++)
        {
            std::vector<Movie*> actToMovieptrs = m_MovieDatabase.get_movies_with_actor(watchedActors[j]);
            for(int k = 0; k < actToMovieptrs.size(); k ++)
            {
                iter = moviesAndScore.find(actToMovieptrs[k]->get_id());
                std::string movieID = actToMovieptrs[k]->get_id();
                if (iter == moviesAndScore.end()) // haven't created an element for this movie
                {
                    moviesAndScore[movieID] = movieInfo();
                    moviesAndScore[movieID].m_score = 30;
                    moviesAndScore[movieID].m_movieRating = m_MovieDatabase.get_movie_from_id(movieID)->get_rating();
                    moviesAndScore[movieID].m_movieTitle = m_MovieDatabase.get_movie_from_id(movieID)->get_title();
                }
                else // this movie already exists, +20 points for it
                {
                    moviesAndScore[movieID].m_score += 30;
                }
            }
        }
        
        // Create a vector containing the directors of the current movie
        std::vector<std::string> watchedGenres = m_MovieDatabase.get_movie_from_id(watchHistory[i])->get_genres();
        for(int j = 0; j < watchedGenres.size(); j ++)
        {
            std::vector<Movie*> genToMovieptrs = m_MovieDatabase.get_movies_with_genre(watchedGenres[j]);
            for(int k = 0; k < genToMovieptrs.size(); k ++)
            {
                iter = moviesAndScore.find(genToMovieptrs[k]->get_id());
                std::string movieID = genToMovieptrs[k]->get_id();
                if (iter == moviesAndScore.end()) // haven't created an element for this movie
                {
                    moviesAndScore[movieID] = movieInfo();
                    moviesAndScore[movieID].m_score = 1;
                    moviesAndScore[movieID].m_movieRating = m_MovieDatabase.get_movie_from_id(movieID)->get_rating();
                    moviesAndScore[movieID].m_movieTitle = m_MovieDatabase.get_movie_from_id(movieID)->get_title();
                }
                else // this movie already exists, +20 points for it
                {
                    moviesAndScore[movieID].m_score += 1;
                }
            }
        }
    }
    
    // Now remove the watched movies from the unordered map
    for(int i = 0; i < watchHistory.size(); i ++)
    {
        moviesAndScore.erase(watchHistory[i]);
    }

    if(moviesAndScore.size() == 0)
    {
        return std::vector<MovieAndRank>();
    }
    
    std::vector<pair<std::string, movieInfo>> vec(moviesAndScore.begin(), moviesAndScore.end());
    std::sort(vec.begin(), vec.end(), compare);


    vector<MovieAndRank> movieAndRank;
    int count = 0;
    for(const auto& pair : vec)
    {
        if(count >= movie_count)
        {
            break;
        }
        
        movieAndRank.push_back(MovieAndRank(pair.first, pair.second.m_score));
        count ++;
    }
    
    return movieAndRank;
}


        
bool Recommender::compare(const std::pair<std::string, movieInfo>& a, const std::pair<std::string, movieInfo>& b)
{
    if(a.second.m_score != b.second.m_score)
    {
        return a.second.m_score > b.second.m_score;
    }
    else if(a.second.m_movieRating != b.second.m_movieRating) //if scores are equal we compare their ratings
    {
        return a.second.m_movieRating > b.second.m_movieRating;
    }
    else //scores and ratings are both equal
    {
        return a.second.m_movieTitle < b.second.m_movieTitle;
    }
}
