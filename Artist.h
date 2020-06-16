

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
    void AddNumOfStreams(int count){
        this->numOfStreams+=count;
    };
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
    Song* BestSong;
    AVLTree<Song,int>* songIDTree; //by ID
    AVLTree<Song,Song>* songStreamTree; //by numOfStreams

public:
    Artist() = default;
    explicit Artist(int performer):
            artistID(performer){
        BestSong = new Song();
        songIDTree = new AVLTree<Song,int>();
        songStreamTree = new AVLTree<Song,Song>();
    };
    ~Artist(){
        delete BestSong;
        delete songIDTree;
        delete songStreamTree;
    }
    void SetBestSong(Song* songToCheck){

        if(*songToCheck>*(this->BestSong)) {
            BestSong = songToCheck;
        }
    };
    Song* GetBestSong(){
        return this->BestSong;
    };
    AVLTree<Song,int>* GetSongIdTree(){return songIDTree;}
    AVLTree<Song,Song>* GetSongStreamTree(){return songStreamTree;}
    int operator*() const {return artistID;}

    void FindNewBestSong(){
        this->BestSong=this->GetSongStreamTree()->FindBiggestObject();
    };
    bool operator==(const Artist& otherArtist) const{
        return this->artistID==otherArtist.artistID;
    }
    bool operator!=(const Artist& otherArtist) const{
        return this->artistID != otherArtist.artistID;
    }
};



#endif //INC_1W_ARTIST_H
