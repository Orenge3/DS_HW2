

#ifndef INC_1W_ARTIST_H
#define INC_1W_ARTIST_H
#include "DS_AVL.h"
#include <ostream>

using std::ostream;
using std::endl;

const int ZERO_STREAMS = 0;
class Song{
    int artistID;
    int songID;
    int numOfStreams;
    void* pointerToCharts;
public:
    Song() = default;
    Song(int performer,int songID):artistID(performer),songID(songID)
            ,numOfStreams(ZERO_STREAMS),pointerToCharts(NULL){};
    ~Song()=default;
    int GetPerformer() const {return artistID;}
    int GetSongID() const {return songID;}
    int GetNumOfStreams() const {return numOfStreams;}
    void IncNumOfStreams(){ numOfStreams++;}
    void SetPointerToCharts(void * address){pointerToCharts= address;}
    void * GetPointerToCharts(){return pointerToCharts;}
    bool operator==(const Song& song1) const {
        return numOfStreams == song1.numOfStreams && songID == song1.songID
               && artistID == song1.artistID;
    }
    bool operator!=(const Song& song1) const{
        return !(*this == song1);
    }
    bool operator>(const Song& song1) const{ //by num of streams.
        if (*this == song1){
            return false;
        }
        if (numOfStreams > song1.numOfStreams){
            return true;
        }
        if (numOfStreams == song1.numOfStreams){
            if (artistID < song1.artistID){
                return true;
            }
            if (artistID == song1.artistID){
                return songID < song1.songID;
            }
            return false;
        }
        return false;
    }
    bool operator<(const Song& song1) const{
        return !(*this > song1) && !(*this == song1);
    }
    Song& operator=(const Song& song1){
        if (this != &song1){
            songID = song1.songID;
            artistID = song1.artistID;
            numOfStreams = song1.numOfStreams;
        }
        return *this;
    }
    friend ostream& operator<<(ostream& os, const Song& toPrint){
        return os << "printing Song" << endl
        << "performer:" << toPrint.GetPerformer()<< endl
        << "songID:" << toPrint.GetSongID() << endl
        << "numberOfStream:" << toPrint.GetNumOfStreams() << endl;
    }
};
class Artist {
private:
    int artistID;
    int numOfSongs;
    Song* BestSong;
    AVLTree<Song*,int>* songIDTree; //by ID
    AVLTree<Song*,Song>* songStreamTree; //by numOfStreams

public:
    Artist() = default;
    Artist(int performer, int numOfSongs = 0):
            artistID(performer),numOfSongs(numOfSongs){
        songIDTree = new AVLTree<Song*,int>();
        if (numOfSongs != 0){
            songStreamTree = new Song[numOfSongs]();
        }
        if (numOfSongs == 0 ) {
            songStreamTree = nullptr;
        }
        for (int i = 0; i < numOfSongs; ++i) {
            Song* songIncharts = new Song(performer,i);
            Song* songInArray = new Song(performer,i);
            songIDTree->Insert(i, songIncharts);
            songStreamTree[i] = *songInArray;
        }
    };
    ~Artist()=default;
    int GetArtistID(){return artistID;}
    int GetArtistNumOfSongs(){return numOfSongs;}
    AVLTree<Song*>* GetSongTree(){return songIDTree;}
    Song* GetSongArray(){return songStreamTree;}
    int operator*() const {return artistID;}


};


#endif //INC_1W_ARTIST_H
