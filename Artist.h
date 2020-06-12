

#ifndef INC_1W_ARTIST_H
#define INC_1W_ARTIST_H
#include "DS_AVL.h"
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
    int GetPerformer(){return artistID;}
    int GetSongID(){return songID;}
    int GetNumOfStreams(){return numOfStreams;}
    void IncNumOfStreams(){ numOfStreams++;}
    void SetPointerToCharts(void * address){pointerToCharts= address;}
    void * GetPointerToCharts(){return pointerToCharts;}
};
class Artist {
private:
    int artistID;
    int numOfSongs;
    AVLTree<Song*>* songTree;
    Song* songArray;

public:
    Artist() = default;
    Artist(int performer, int numOfSongs = 0):
            artistID(performer),numOfSongs(numOfSongs){
        songTree = new AVLTree<Song*>();
        if (numOfSongs != 0){
            songArray = new Song[numOfSongs]();
        }
        if (numOfSongs == 0 ) {
            songArray = nullptr;
        }
        for (int i = 0; i < numOfSongs; ++i) {
            Song* songIncharts = new Song(performer,i);
            Song* songInArray = new Song(performer,i);
            songTree->Insert(i,songIncharts);
            songArray[i] = *songInArray;
        }
    };
    ~Artist()=default;
    int GetArtistID(){return artistID;}
    int GetArtistNumOfSongs(){return numOfSongs;}
    AVLTree<Song*>* GetSongTree(){return songTree;}
    Song* GetSongArray(){return songArray;}


};


#endif //INC_1W_ARTIST_H
