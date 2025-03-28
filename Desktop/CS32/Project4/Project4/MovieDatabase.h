#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include <cctype>
#include "treemm.h"

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
    TreeMultimap<std::string, Movie*> m_ID;
    TreeMultimap<std::string, Movie*> m_director;
    TreeMultimap<std::string, Movie*> m_actor;
    TreeMultimap<std::string, Movie*> m_genre;
    std::vector<Movie*> v_movie;
    
    std::string toLower(std::string word) const{
        std::string lowered;
        for(int i = 0; i < word.size(); i++) {
            lowered += tolower(word[i]);
        }
        return lowered;
    }
};

#endif // MOVIEDATABASE_INCLUDED
