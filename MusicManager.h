/****************************************************************************/
/*                                                                          */
/* This file contains the declarations of the music manager DS              */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : MusicManager.h                                               */
/*                                                                          */
/****************************************************************************/

#ifndef INC_1W_DS_DC_H
#define INC_1W_DS_DC_H
#include "DS_AVL.h"
#include "library2.h"
#include "Artist.h"
#include "HashTable.h"

class MusicManager{
private:
//TODO add structures and implement functions
    // add Hash table of artists
    AVLTree<Song*,Song>* allSongsTree; //by numOfStreams
    HashTable<Artist*> artistTable;

public:
    MusicManager() {
        allSongsTree = new AVLTree<Song*, Song>;
    };
    ~MusicManager() = default;
    StatusType AddArtist(int artistID);
    StatusType RemoveArtist(int artistID);
    StatusType AddSong(int artistID ,int songID);
    StatusType RemoveSong(int artistID ,int songID);
    StatusType AddToSongCount(int artistID, int songID, int count);
    StatusType GetArtistBestSong(int artistID, int *songID);
    StatusType GetRecommendedSongInPlace(int rank, int *artistId, int *songId);


};

#endif //INC_1W_DS_DC_H
