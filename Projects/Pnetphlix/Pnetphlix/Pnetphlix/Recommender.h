#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include "Movie.h"
#include "User.h"

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
    const MovieDatabase& m_MovieDatabase;
    const UserDatabase& m_userDatabase;
    
    struct movieInfo
    {
        int m_score;
        float m_movieRating;
        std::string m_movieTitle;
    };
    
    static bool compare(const std::pair<std::string, movieInfo>& a, const std::pair<std::string, movieInfo>& b);
};

#endif // RECOMMENDER_INCLUDED
