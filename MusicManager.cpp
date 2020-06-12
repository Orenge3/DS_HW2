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

StatusType MusicManager::AddArtist(int artistID, int numOfSongs) { //invalid input handled
    Artist* artist_to_add = new Artist(artistID, numOfSongs);
    //add to artist_tree
    AVL_RESULT res = artist_tree.Insert(artistID,artist_to_add);

    //now add to charts
    Artist* artist_in_charts = new Artist(artistID, numOfSongs);
    switch (res) {
        case AVL_FAILURE: //artist already exists
            return FAILURE;
        case AVL_SUCCESS: //artist added to artist tree.
            totalNumOfSongs += numOfSongs;
            // charts is not empty and has a "no streams" tree.
            if (!charts.empty() && charts.GETHeadID() == ZERO_STREAMS){
                //add artist and initialized songs to artist tree with no streams in start of list
                charts.GetHead()->_data->Insert(artistID,artist_in_charts);
                for (int i = 0; i < numOfSongs; ++i) { //put pointers to node in song array.
                    artist_to_add->GetSongArray()[i]
                            .SetPointerToCharts(charts.GetHead());
                }
                return SUCCESS;
            } else{ //charts is empty, or first item is not "no streams" tree
                AVLTree<Artist*>* no_streams = new AVLTree<Artist*>();
                //add artist and initialized songs to artist tree with no streams
                no_streams->Insert(artistID,artist_in_charts);
                Node<AVLTree<Artist*>>* no_streams_node;
                no_streams_node = charts.Insert(NULL,ZERO_STREAMS,*no_streams);
                for (int i = 0; i < numOfSongs; ++i) { //put pointers to node in song array.
                    artist_to_add
                            ->GetSongArray()[i].SetPointerToCharts(no_streams_node);
                }
                return (StatusType) SUCCESS;
            }
        case AVL_ALLOCATION_ERROR: return ALLOCATION_ERROR;
        case AVL_INVALID_INPUT: return INVALID_INPUT;
        case AVL_NODE_NOT_FOUND: return INVALID_INPUT;
        case AVL_EMPTY: return FAILURE;
    }
    return (StatusType) FAILURE;
}

StatusType MusicManager::RemoveArtist(int artistID) {
    Artist* tempArtistInTree;
    Artist* tempArtistInCharts;
    AVL_RESULT  res = artist_tree.Find(artistID,tempArtistInTree);
    switch (res) {
        case AVL_FAILURE: //artist does not exist
            return FAILURE;
        case AVL_SUCCESS: {
            int numOfSongs = tempArtistInTree->GetArtistNumOfSongs();
            totalNumOfSongs -= numOfSongs;
            for (int i = 0; i < numOfSongs; ++i) {
                void *place = tempArtistInTree->GetSongArray()[i].GetPointerToCharts();
                AVLTree<Artist *> *tempTree =
                        charts.GetData((Node<AVLTree<Artist *>> *) place);
                AVL_RESULT res2 = tempTree->Find(artistID, tempArtistInCharts);
                switch (res2) {
                    case AVL_FAILURE:
                        break;
                    case AVL_SUCCESS: {
                        tempArtistInCharts->GetSongTree()->Delete(i);
                        if (tempArtistInCharts->GetSongTree()->IsEmpty()) {
                            tempTree->Delete(artistID);
                        }
                        if (tempTree->IsEmpty()) {
                            charts.Delete((Node<AVLTree<Artist *>> *) place);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            artist_tree.Delete(artistID);
            return SUCCESS;
        }
        default:
            break;
    }
    return SUCCESS;
}

StatusType MusicManager::AddToSongCount(int artistID, int songID) {//songid<0,DS==NULL,artistID<=0 handled
    //handle num of streams in artist tree
    Artist* artistInTree;
    Artist* tempArtist;
    //tempArtist = artist in artistTree

    AVL_RESULT artistRes= artist_tree.Find(artistID,artistInTree);
    if (artistRes == AVL_FAILURE || artistRes == AVL_NODE_NOT_FOUND
        || artistRes == AVL_EMPTY) return FAILURE;
    int numOfSongs = artistInTree->GetArtistNumOfSongs();
    if (numOfSongs <= songID) return  INVALID_INPUT;
    artistInTree->GetSongArray()[songID].IncNumOfStreams();

//create copy of incremented song since its about to be deleted
    Song* incSong = new Song();
    *incSong = artistInTree->GetSongArray()[songID];

//handle charts list
    //get position in charts (avoid looking for it, costing time)
    void* place = artistInTree->GetSongArray()[songID].GetPointerToCharts();
    Node<AVLTree<Artist*>>* deleteHere = (Node<AVLTree<Artist*>>*) place;

    //insert to new place
    Node<AVLTree<Artist*>>* next_node =
            charts.GetNext((Node<AVLTree<Artist*>>*)place);
    //if next position in charts exists and equal to incremented song (otherwise needs tree allocation)
    if (next_node != NULL &&
        charts.GetID(next_node) == incSong->GetNumOfStreams()){
        //if artist already has song with same amount of streams
        //otherwise need to allocate new artist
        //artist = some_stream+1_Tree
        if (next_node->_data->Find(artistID,tempArtist) ==AVL_SUCCESS){
            //put song in song tree
            StatusType song_res =(StatusType)tempArtist->GetSongTree()->Insert(songID,incSong);
            artistInTree->GetSongArray()[songID].SetPointerToCharts((void *)next_node);
            //temp artist = artist in some_streamsTree
            deleteHere->_data->Find(artistID,tempArtist);
            tempArtist->GetSongTree()->Delete(songID);
//    if artist songtree is empty delete it, and the artist from the tree.
            if (tempArtist->GetSongTree()->IsEmpty()){
                deleteHere->_data->Delete(artistID);
                //if some_streams is empty, delete it.
                if (deleteHere->_data->IsEmpty()){
                    charts.Delete(deleteHere);
                }
            }
            return song_res;
        }
        //allocate new artist, put song in its songtree and put in artists tree.
        Artist* new_Artist = new Artist(artistID);
        StatusType song_res2 = (StatusType) new_Artist->GetSongTree()->Insert(songID,incSong);
        StatusType artist_res =(StatusType) next_node->_data->Insert(artistID,new_Artist);
        //temp artist = artist in some_streamsTree
        deleteHere->_data->Find(artistID,tempArtist);
        tempArtist->GetSongTree()->Delete(songID);
//    if artist songtree is empty delete it, and the artist from the tree.
        if (tempArtist->GetSongTree()->IsEmpty()){
            deleteHere->_data->Delete(artistID);
            //if some_streams is empty, delete it.
            if (deleteHere->_data->IsEmpty()){
                charts.Delete(deleteHere);
            }
        }
        artistInTree->GetSongArray()[songID].SetPointerToCharts((void *)next_node);
        return artist_res<song_res2?artist_res:song_res2;
    }
    //allocate new tree.
    AVLTree<Artist*>* some_streams = new AVLTree<Artist*>();
    //then allocate new artist, put song in its songtree and put in artists tree.
    Artist* new_artist = new Artist(artistID);
    new_artist->GetSongTree()->Insert(songID,incSong);
    some_streams->Insert(artistID,new_artist);
    //put tree in charts using addtoSongCount interface(Node indicator)
    Node<AVLTree<Artist*>>* new_place = charts.Insert(deleteHere,
                                                      incSong->GetNumOfStreams(),*some_streams);
    //temp artist = artist in some_streamsTree
    deleteHere->_data->Find(artistID,tempArtist);
    tempArtist->GetSongTree()->Delete(songID);
//    if artist songtree is empty delete it, and the artist from the tree.
    if (tempArtist->GetSongTree()->IsEmpty()){
        deleteHere->_data->Delete(artistID);
        //if some_streams is empty, delete it.
        if (deleteHere->_data->IsEmpty()){
            charts.Delete(deleteHere);
        }
    }
    //handle charts pointer in artist tree
    artist_tree.Find(artistID,tempArtist);
    tempArtist->GetSongArray()[songID].SetPointerToCharts(new_place);

    return SUCCESS;

}

StatusType MusicManager::NumberOfStreams(int artistID, int songID, int *streams) { //songid<0,DS==NULL,artistID<=0 handled
    //get from artist tree
    Artist* tempArtist;
    AVL_RESULT artistRes= artist_tree.Find(artistID,tempArtist);
    if (artistRes == AVL_FAILURE || artistRes == AVL_EMPTY
        ||artistRes == AVL_NODE_NOT_FOUND) return FAILURE;
    if (tempArtist->GetArtistNumOfSongs() <= songID) return  INVALID_INPUT;
    *streams = tempArtist->GetSongArray()[songID].GetNumOfStreams();
    return SUCCESS;
}

StatusType MusicManager::GetRecommendedSongs(int numOfSongs, int *artist, int *songs) {
    if (numOfSongs <= 0) return ALLOCATION_ERROR;
    if (totalNumOfSongs < numOfSongs) return FAILURE;
    int counter = 0;
    AVLTree<Artist*>* temp_artist_tree = charts.GetLast()->_data; //get leading songs
    Artist* temp_artist;
    temp_artist_tree->treeGetSmallest(temp_artist); //get smallest artist in leading songs
    Song* temp_song;
    AVL_RESULT res1 = AVL_SUCCESS;
    Node<AVLTree<Artist*>>* current = charts.GetLast();
    temp_artist->GetSongTree()->treeGetSmallest(temp_song); //get smallest song in smallest artist in leading songs
    while (counter < numOfSongs-1){
        artist[counter] = temp_song->GetPerformer();
        songs[counter] = temp_song->GetSongID();
        res1 = temp_artist->GetSongTree()->treeGetNextInOrder(temp_song);
        if (res1 == AVL_EMPTY){
            int res2 = temp_artist_tree->treeGetNextInOrder(temp_artist);
            if (res2 == AVL_EMPTY){
                current = charts.GetPrev(current);
                temp_artist_tree = current->_data;
                temp_artist_tree->treeGetSmallest(temp_artist);
            }
            temp_artist->GetSongTree()->treeGetSmallest(temp_song);
        }
        counter++;
    }
    artist[counter] = temp_song->GetPerformer();
    songs[counter] = temp_song->GetSongID();
    return SUCCESS;
}
