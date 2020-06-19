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
    Artist *newArtist =new Artist(artistID);
    HASH_RESULT res =this->artistTable.Insert(newArtist);
    if (res == HASH_FAILURE){
        delete newArtist;
    }
    return res==HASH_SUCCESS ? SUCCESS : FAILURE;
}

StatusType MusicManager::RemoveArtist(int artistID) {

    Artist *uArtist;
    uArtist=this->artistTable.find(artistID);

    if(uArtist== nullptr)
        return FAILURE;
    if(!uArtist->GetSongIdTree()->IsEmpty())
        return FAILURE;

    this->artistTable.Delete(uArtist);
    delete uArtist;

    return SUCCESS;
}

StatusType MusicManager::AddSong(int artistID, int songID) {
    Song *newSong=new Song(artistID,songID);

    Artist *uArtist;
    uArtist=this->artistTable.find(artistID);
    if(uArtist== nullptr){
        delete(newSong);
        return FAILURE;
    }

    if(uArtist->GetSongIdTree()->Insert(songID,*newSong)!=AVL_SUCCESS) {
        delete(newSong);
        return FAILURE;
    }
    uArtist->GetSongStreamTree()->Insert(*newSong,*newSong);
    this->allSongsTree->Insert(*newSong,newSong);
    uArtist->SetBestSong(newSong);
    delete newSong;
    return SUCCESS;
}

StatusType MusicManager::RemoveSong(int artistID, int songID) {
    Song *songToDelete=new Song(artistID,songID);
    Artist *uArtist;
    uArtist=this->artistTable.find(artistID);
    if(uArtist== nullptr){
        delete(songToDelete);
        return FAILURE;
    }
    if(uArtist->GetSongIdTree()->Find(songID,*songToDelete)!=AVL_SUCCESS) {
        delete(songToDelete);
        return FAILURE;
    }
    uArtist->GetSongStreamTree()->Delete(*songToDelete);
    uArtist->GetSongIdTree()->Delete(songID);
    this->allSongsTree->Delete(*songToDelete);
    //TODO find solution for deleting best song cause it's not working
    if(uArtist->GetBestSong()!= NULL){
        if ( *songToDelete==*uArtist->GetBestSong()){
            uArtist->FindNewBestSong();
        }
    } else{
        throw;
    }
    delete(songToDelete);
    return SUCCESS;
}

StatusType MusicManager::AddToSongCount(int artistID, int songID, int count) {
    Song *songToImprove=new Song(artistID,songID);
    Artist *uArtist;
    uArtist=this->artistTable.find(artistID);
    if(uArtist== nullptr){
        delete(songToImprove);
        return FAILURE;
    }
    if(uArtist->GetSongIdTree()->Find(songID,*songToImprove)!=AVL_SUCCESS) {
        delete(songToImprove);
        return FAILURE;
    }

    uArtist->GetSongStreamTree()->Delete(*songToImprove);
    uArtist->GetSongIdTree()->Delete(songID); ///** added by oren, need to change value in id tree too.
    allSongsTree->Delete(*songToImprove);

    songToImprove->AddNumOfStreams(count);

    uArtist->GetSongStreamTree()->Insert(*songToImprove,*songToImprove);
    uArtist->GetSongIdTree()->Insert(songID,*songToImprove); ///** added by oren, need to change value in id tree too
    allSongsTree->Insert(*songToImprove,songToImprove);
//    Song* currentBestSong = uArtist->GetBestSong();
    uArtist->SetBestSong(songToImprove);
//    if (uArtist->GetBestSong() == currentBestSong){
//        delete (songToImprove);
//    } else{
//        delete(currentBestSong);
//    }
    delete songToImprove;
    return SUCCESS;
}

StatusType
MusicManager::GetArtistBestSong(int artistID, int *songID) {
    Artist *uArtist;
    uArtist=this->artistTable.find(artistID);
    if(uArtist== nullptr){
        return FAILURE;
    }
    if(uArtist->GetBestSong()==NULL) return FAILURE;
    *songID=uArtist->GetBestSong()->GetSongID();
    return SUCCESS;
}

StatusType
MusicManager::GetRecommendedSongInPlace(int rank, int *artistId, int *songId) {
    Song rankedSong ;
//    if (rank==9){ ////////////**************** PRINT DS******/////
//        allSongsTree->ObserveTree();
//        artistTable.displayHash();
//    }
    try{
        rankedSong = allSongsTree->GetRankedObject(rank);
    }catch (AVLTree<Song*,Song>::bad_rank& e){
        return FAILURE;
    }

    *artistId=rankedSong.GetPerformer();
    *songId=rankedSong.GetSongID();
    return SUCCESS;
}

void MusicManager::DeleteMusicDatabase() {
    this->allSongsTree->DeleteTree();
    delete(allSongsTree);
    HASH_FOREACH(Artist*,toDelete,artistTable){
        toDelete->GetSongIdTree()->DeleteTree();
        toDelete->GetSongStreamTree()->DeleteTree();
        delete toDelete;
    }
   //delete(artistTable)

}
