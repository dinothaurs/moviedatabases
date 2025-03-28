#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <fstream>
#include <ostream>
#include <sstream>
#include <cctype>
using namespace std;

MovieDatabase::MovieDatabase()
{}

MovieDatabase::~MovieDatabase() {
    for(int i = 0; i < v_movie.size(); i++) {
        delete v_movie.at(i);
    }
}

bool MovieDatabase::load(const string& filename)
{
    ifstream infile(filename);
    if(!infile) {
        return false;
    }
    string id;
    string name;
    string year;
    string genre;
    string director;
    string actor;
    float rating;
    
    while(getline(infile, id)) {
        //create vectors to hold directors, actors, and genres per movie
        vector<string> director_list;
        vector<string> actor_list;
        vector<string> genre_list;
        
        //get the information
        getline(infile, name);
        getline(infile, year);
        //read each director separated by commas and add into the prospective vectors
        if(getline(infile, director) && director.find(',')) {
            stringstream sd(director);
            string d;
            while(getline(sd, d, ',')) {
                director_list.push_back(d);
            }
        }
        else {
            director_list.push_back(director);
        }
        //read each actor
        if(getline(infile, actor) && actor.find(',')) {
            stringstream sa(actor);
            string a;
            while(getline(sa, a, ',')) {
                actor_list.push_back(a);
            }
        }
        else {
            actor_list.push_back(actor);
        }
        //read each genre
        if(getline(infile, genre) && genre.find(',')) {
            stringstream sg(genre);
            string g;
            while(getline(sg, g, ',')) {
                genre_list.push_back(g);
            }
        }
        else {
            genre_list.push_back(genre);
        }
        //read the rating
        infile >> rating;
        infile.ignore(10000, '\n');
        
        //create the movie
        Movie* x = new Movie(id, name, year, director_list, actor_list, genre_list, rating);
        //add it into the vector for easy deletion
        v_movie.push_back(x);
        //insert it into the ID map
        m_ID.insert(toLower(id), x);
        //directors can be multiple per movie, so add all directors into the map using a for loop
        //it is also case insensitive so set everything to lower case
        //same applies for actor director and genre
        for(int i = 0; i < director_list.size(); i++) {
            m_director.insert(toLower(director_list[i]), x);
        }
        for(int i = 0; i < actor_list.size(); i++) {
//            cout << actor_list[i] << endl;
            m_actor.insert(toLower(actor_list[i]), x);
        }
        for(int i = 0; i < genre_list.size(); i++) {
            m_genre.insert(toLower(genre_list[i]), x);
        }
        
        //skip a line
        getline(infile, id);

    }
    return true;  // Replace this line with correct code.
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    //lowercase the inserted id because it is case insensitive
    string lowercaseID = toLower(id);
    //get the iterator for where the id is located
    TreeMultimap<string, Movie*>::Iterator it = m_ID.find(lowercaseID);
    //if it is valid, return the value it contains
    if(it.is_valid()) {
        return it.get_value();
    }
    //else it cannot be found and return nullptr
    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    string lowercaseDirector = toLower(director);
    TreeMultimap<string, Movie*>::Iterator it = m_director.find(lowercaseDirector);
    vector<Movie*> movies;
    //since there are multiple, use a while loop
    while(it.is_valid()) {
        Movie* x = it.get_value();
        movies.push_back(x);
        //iterate through by advancing until it isn't valid
        it.advance();
    }
    return movies;
}

//like the get_movies_with_director
vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    string lowercaseActor = toLower(actor);
    TreeMultimap<string, Movie*>::Iterator it = m_actor.find(lowercaseActor);
    vector<Movie*> actors;
    while(it.is_valid()) {
        Movie* x = it.get_value();
        actors.push_back(x);
        it.advance();
    }
    return actors;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    string lowercaseGenre = toLower(genre);
    TreeMultimap<string, Movie*>::Iterator it = m_genre.find(lowercaseGenre);
    vector<Movie*> genres;
    while(it.is_valid()) {
        Movie* x = it.get_value();
        genres.push_back(x);
        it.advance();
    }
    return genres;
}
