from flask import session
from models.song_model import SongModel


class SongService:
    def __init__(self):
        self.model = SongModel()

    def create(self, params):
        return self.model.create(params["song_title"], params["album_id"], session["id"], params["other_artists"])

    def update(self, params):
        return self.model.update(params["song_id"], params["song_title"], session["id"], params["other_artists"])

    def list(self):
        return self.model.list(session["id"])

    def list_by_popularity(self):
        return self.model.list_by_popularity(session["id"])

    def list_liked(self):
        return self.model.list_liked(session["id"])

    def list_by_artist_id(self, artist_id):
        return self.model.list_by_artist_id(artist_id, session["id"])

    def list_by_artist_id_and_popularity(self, artist_id):
        return self.model.list_by_artist_id_and_popularity(artist_id, session["id"])

    def list_by_album_id(self, album_id):
        return self.model.list_by_album_id(album_id, session["id"])

    def list_by_listener_id(self, listener_id):
        return self.model.list_by_listener_id(listener_id, session["id"])

    def list_by_genre(self, genre):
        return self.model.list_by_genre(genre, session["id"])

    def search(self, params):
        return self.model.search(params["search_text"], session["id"])

    def delete(self, _id):
        return self.model.delete(_id)

    def like(self, params):
        return self.model.like(params["song_id"], params["listener_id"])

    def unlike(self, params):
        return self.model.unlike(params["song_id"], params["listener_id"])
