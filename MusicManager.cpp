/****************************************************************************/
/*                                                                          */
/* This file contains the implementation of the music manager DS              */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : MusicManager.cpp                                               */
/*                                                                          */
/****************************************************************************/
#include "MusicManager.h"
//assume invalid input handled for all functions
StatusType MusicManager::AddArtist(int artistID) {
    return FAILURE;
}

StatusType MusicManager::RemoveArtist(int artistID) {
    return FAILURE;
}

StatusType MusicManager::AddSong(int artistID, int songID) {
    return FAILURE;
}

StatusType MusicManager::RemoveSong(int artistID, int songID) {
    return FAILURE;
}

StatusType MusicManager::AddToSongCount(int artistID, int songID, int count) {
    return FAILURE;
}

StatusType
MusicManager::GetArtistBestSong(int artistID, int *songID) {
    return FAILURE;
}

StatusType
MusicManager::GetRecommendedSongInPlace(int rank, int *artistId, int *songId) {
    return FAILURE;
}
