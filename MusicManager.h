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
#include "library1.h"
#include "DLLIST.h"
#include "Artist.h"


class MusicManager{
private:
    DLList<AVLTree<Artist*>> charts;
    AVLTree<Artist*> artist_tree;
    int totalNumOfSongs;
public:
    MusicManager() = default;
    ~MusicManager() = default;
    StatusType AddArtist(int artistID, int numOfSongs);
    StatusType RemoveArtist(int artistID);
    StatusType AddToSongCount(int artistID, int songID);
    StatusType NumberOfStreams(int artistID, int songID, int *streams);
    StatusType GetRecommendedSongs(int numOfSongs, int* artist, int* songs);

};

#endif //INC_1W_DS_DC_H
