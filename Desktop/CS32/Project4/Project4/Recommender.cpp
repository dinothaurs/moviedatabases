#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <utility>
using namespace std;

//to use to help sort
struct MoviePointer
{
    Movie* movie;
    MovieAndRank m;

    MoviePointer(Movie* m, MovieAndRank movierank)
    : movie(m), m(movierank) {}
};

//use this to sort
bool cmp(const MoviePointer& m1, const MoviePointer& m2) {
    //if m1 is greater, return true
    if(m1.m.compatibility_score > m2.m.compatibility_score)
        return true;
    //if m1 is less, return false
    if(m1.m.compatibility_score < m2.m.compatibility_score)
        return false;
    //if m1 is equal to m2's score then check if the rating is greater
    if(m1.m.compatibility_score == m2.m.compatibility_score && m1.movie->get_rating() > m2.movie->get_rating())
        return true;
    if(m1.m.compatibility_score == m2.m.compatibility_score && m1.movie->get_rating() < m2.movie->get_rating())
        return false;
    //if m1's score and rating equals m2's then check based on the order of the title, if the title comes first alphabetically wise
    if(m1.m.compatibility_score == m2.m.compatibility_score && m1.movie->get_rating() == m2.movie->get_rating() && m1.movie->get_title() < m2.movie->get_title())
        return true;
    if(m1.m.compatibility_score == m2.m.compatibility_score && m1.movie->get_rating() == m2.movie->get_rating() && m1.movie->get_title() > m2.movie->get_title())
        return false;
    else
        return false;
}


Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
: user_db(user_database), movie_db(movie_database) {}

//recommender function
vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    map<string, int> director_map;
    map<string, int> actor_map;
    map<string, int> genre_map;
    set<MovieAndRank*> scored;
    set<MoviePointer, decltype(&cmp)> sortList(&cmp);
    //get the user
    User* get_user = user_db.get_user_from_email(user_email);
    unordered_set<string> watched_movies;
    //get a set of the movies the user watched
    for(int i = 0; i < get_user->get_watch_history().size(); i++) {
        watched_movies.insert(get_user->get_watch_history().at(i));
    }
    
    for(int j = 0; j < get_user->get_watch_history().size(); j++) {
        //get the movie id in the vector of movies
        string movie = get_user->get_watch_history().at(j);
        //get the movie info
        Movie* get_movie = movie_db.get_movie_from_id(movie);
        //get the directors
        vector<string> director = get_movie->get_directors();
        //get the actors
        vector<string> actor = get_movie->get_actors();
        //get the genres
        vector<string> genre = get_movie->get_genres();
        
        //insert the directors into the director map
        for(int i = 0; i < director.size(); i++) {
            //if the director has already been inserted, increase the counter
            if(director_map.find(director[i]) != director_map.end()) {
                director_map[director[i]] += 1;
//                cout << director[i] << " " << director_map[director[i]] << endl;
            }
            else {
                //insert the director into the map if there isnt one already
                director_map.insert({director[i], 1});
            }
        }
        //insert the actor data into the actor map
        for(int i = 0; i < actor.size(); i++) {
            //if the actor has already been inserted, increase counter by 1
            if(actor_map.find(actor[i]) != actor_map.end())
                actor_map[actor[i]] += 1;
            else
                //insert the actor into the actor map if there isnt one already
                actor_map.insert({actor[i], 1});
        }
        //insert the genre data into the genre map
        for(int i = 0; i < genre.size(); i++) {
            //if it has already been inserted increase +1
            if(genre_map.find(genre[i]) != genre_map.end()) {
                genre_map[genre[i]] += 1;
            }
            else
                //insert the genre into the genre map if there isnt one arleady
                genre_map.insert({genre[i], 1});
        }
    }
    
    map<string, int>::iterator it;
    //create a map to hold the ranks to check for if the movieandrank has already been created for that movie
    map<string, MovieAndRank> map_movierank;
    //loop through the director map, and for each director get all the movies with that director, then for each movie with it change the MovieAndRank struct's score to have the appropriate score based on how often the director occured
    for(it = director_map.begin(); it != director_map.end(); it++) {
        //the vector of the movies with the director
        vector<Movie*> movies_with_director = movie_db.get_movies_with_director(it->first);
        for(int i = 0; i < movies_with_director.size(); i++) {
            //get the movie id that has the director
            string movie_id = movies_with_director.at(i)->get_id();
            if(map_movierank.find(movie_id) != map_movierank.end()) {
                //itMR is the iterator for the movie and rank with the appropriate movie id
                map<string, MovieAndRank>::iterator itMR = map_movierank.find(movie_id);
                //use itMR to find the exact MovieAndRank object and change its score accordingly based on many times the director appeared(the multiplication of it->second or the umber of times the director appeared
                itMR->second.compatibility_score += 20*(it->second);
            }
            else {
                //create a struct for movieandrank with that movie id and set it to score 0
                MovieAndRank rank(movie_id, 20*(it->second));
                //insert it into the movierank map to store
                map_movierank.insert({movie_id, rank});
            }
        }
    }
    //same application as the director one but for actor
    map<string, int>::iterator a;
    for(a = actor_map.begin(); a != actor_map.end(); a++) {
        //get the movies with the actor
        vector<Movie*> movies_with_actor = movie_db.get_movies_with_actor(a->first);
        //for each movie with the actor
        for(int i = 0; i < movies_with_actor.size(); i++) {
            //get the movie id that has the actor
            string movie_id = movies_with_actor.at(i)->get_id();
            //if rank for movie has already been created, then just add on top
            if(map_movierank.find(movie_id) != map_movierank.end()) {
                //itMR is the iterator for the movie and rank with the appropriate movie id
                map<string, MovieAndRank>::iterator itMR = map_movierank.find(movie_id);
                //use itMR to find the exact MovieAndRank object to get the compatibility score to add to it
                itMR->second.compatibility_score += 30*(a->second);
            }
            else {
                //create a struct for movieandrank with that movie id and set it to score 0
                MovieAndRank rank(movie_id, 30*(a->second));
                map_movierank.insert({movie_id, rank});
               // scored.insert(rank);
                //rank->compatibility_score += 30*(a->second);
            }
        }
    }
    //same application but for genre
    map<string, int>::iterator g;
    for(g = genre_map.begin(); g != genre_map.end(); g++) {
        vector<Movie*> movies_with_genre = movie_db.get_movies_with_genre(g->first);
        for(int i = 0; i < movies_with_genre.size(); i++) {
            //get the movie id that has the genre
            string movie_id = movies_with_genre.at(i)->get_id();
            //if it is already added to the movie rank map, just add on top
            if(map_movierank.find(movie_id) != map_movierank.end()) {
                //itMR is the iterator for the movie and rank with the appropriate movie id
                map<string, MovieAndRank>::iterator itMR = map_movierank.find(movie_id);
                //use itMR to find the exact MovieAndRank object
                itMR->second.compatibility_score += 1*(g->second);
            }
            else {
                //create a struct for movieandrank with that movie id and set it to score 0
                MovieAndRank rank(movie_id, 1*(g->second));
                map_movierank.insert({movie_id, rank});
                //scored.insert(rank);
                //rank->compatibility_score += 1*(g->second);
            }
        }
    }
    map<string, MovieAndRank>::iterator itMR;
        //add the movies and ranks stored in the movierank map into a set with moviePointers to solve the sort issue
    for(itMR = map_movierank.begin(); itMR!= map_movierank.end(); itMR++) {
        MovieAndRank temp = itMR->second;
        string id_temp = temp.movie_id;
        sortList.insert(MoviePointer({movie_db.get_movie_from_id(id_temp), temp}));
    }
    
    //add all the ranked movies into a vector of the movies ranked
    vector<MovieAndRank> rankedMovies;
    for(auto it2 = sortList.begin(); it2 != sortList.end(); it2++) {
        if((*it2).m.compatibility_score > 0 && watched_movies.find((*it2).m.movie_id) == watched_movies.end())
            rankedMovies.push_back((*it2).m);
    }
    //add into the final vector the amount required
    vector<MovieAndRank> finalMovies;
    for(int i = 0; i < movie_count; i++) {
        finalMovies.push_back(rankedMovies.at(i));
    }
    return finalMovies;  // Replace this line with correct code.
}
