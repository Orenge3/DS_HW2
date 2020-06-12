/****************************************************************************/
/*                                                                          */
/* This file contains the implementation of functions                       */
/* you should use for the wet ex 1                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library1.cpp                                                 */
/*                                                                          */
/****************************************************************************/


#include "library1.h"
#include "MusicManager.h"

void *Init() {
    MusicManager *DS = new MusicManager();
    return (void*)DS;
}

StatusType AddArtist(void *DS, int artistID, int numOfSongs) {
    if (artistID <= 0 || DS == NULL || numOfSongs<=0) {//parameter check
        return INVALID_INPUT;
    }
    return ((MusicManager*)DS)->AddArtist(artistID,numOfSongs);

}

StatusType RemoveArtist(void *DS, int artistID) {
    if (DS == nullptr || artistID <= 0) return INVALID_INPUT;
    return ((MusicManager *) DS)->RemoveArtist(artistID);
}

StatusType AddToSongCount(void *DS, int artistID, int songID) {
    if (DS == NULL || songID < 0 || artistID <= 0) return INVALID_INPUT;
    return ((MusicManager *) DS)->AddToSongCount(artistID, songID);
}

StatusType NumberOfStreams(void *DS, int artistID, int songID, int *streams) {
    if (songID < 0 || DS == NULL || artistID <= 0 || streams == NULL) return INVALID_INPUT;
    return ((MusicManager *) DS)->NumberOfStreams(artistID, songID, streams);

}

StatusType
GetRecommendedSongs(void *DS, int numOfSongs, int *artists, int *songs) {
    if (DS == nullptr || numOfSongs <= 0) return INVALID_INPUT;
    return ((MusicManager*)DS)->GetRecommendedSongs(numOfSongs, artists, songs);
}

void Quit(void **DS) {//implement for real
    *DS = nullptr;
}

