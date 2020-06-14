/****************************************************************************/
/*                                                                          */
/* This file contains the implementation of functions                       */
/* you should use for the wet ex 2                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library2.cpp                                                 */
/*                                                                          */
/****************************************************************************/


#include <clocale>
#include <new>
#include "library2.h"
#include "MusicManager.h"
using namespace std;

void *Init() {
    MusicManager *DS;
    try {
        DS = new MusicManager();
    }
    catch (const std::bad_alloc& e) {
        return NULL;
    }
    return (void*)DS;

}

StatusType AddArtist(void *DS, int artistID) {
    if (artistID <= 0 || DS == NULL) return INVALID_INPUT;
    try {
        return ((MusicManager *) DS)->AddArtist(artistID);
    }
    catch (const std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }

}

StatusType RemoveArtist(void *DS, int artistID) {
    if (DS == NULL || artistID <= 0) return INVALID_INPUT;
    try {
        return ((MusicManager *) DS)->RemoveArtist(artistID);
    }
    catch (const std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
}

StatusType AddSong(void *DS, int artistID, int songID) {
    if (DS == NULL || artistID <= 0 || songID <= 0) return INVALID_INPUT;
    try {
        return ((MusicManager*)DS)->AddSong(artistID, songID);
    }
    catch (const std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
}

StatusType RemoveSong(void *DS, int artistID, int songID) {
    if (DS == NULL || artistID <= 0 || songID <= 0) return INVALID_INPUT;
    try {
        return ((MusicManager*)DS)->RemoveSong(artistID, songID);
    }
    catch (const std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
}

StatusType AddToSongCount(void *DS, int artistID, int songID, int count) {
    if (DS == NULL || songID < 0 || artistID <= 0 || count <= 0) return INVALID_INPUT;
    try {
        return ((MusicManager *) DS)->AddToSongCount(artistID, songID, count);
    }
    catch (const std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
}


StatusType GetArtistBestSong(void *DS, int artistID, int *songId) {
    if (DS == NULL || artistID <= 0 || songId == NULL) return INVALID_INPUT;
    try {
        return ((MusicManager *) DS)->GetArtistBestSong(artistID, songId);
    }
    catch (const std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
}

StatusType
GetRecommendedSongInPlace(void *DS, int rank, int *artistId, int *songId) {
    if (DS == NULL || rank <= 0 || artistId == NULL || songId == NULL) return INVALID_INPUT;
    try {
        return ((MusicManager *) DS)->GetRecommendedSongInPlace(rank ,artistId, songId);
    }
    catch (const std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
}
void Quit(void **DS) {//implement for real

    *DS = nullptr;
}

