from flask import session
from models.album_model import AlbumModel


class AlbumService:
    def __init__(self):
        self.model = AlbumModel()

    def create(self, params):
        return self.model.create(params["album_title"], params["album_genre"], params["artist_id"])

    def update(self, params):
        return self.model.update(params["album_id"], params["album_title"], params["album_genre"])

    def list(self):
        return self.model.list(session["id"])

    def list_by_popularity(self):
        return self.model.list_by_popularity(session["id"])

    def list_by_artist_id(self, artist_id):
        return self.model.list_by_artist_id(artist_id, session["id"])

    def list_by_genre(self, genre):
        return self.model.list_by_genre(genre, session["id"])

    def list_liked(self):
        return self.model.list_liked(session["id"])

    def get_by_id(self, _id):
        return self.model.get_by_id(_id, session["id"])

    def delete(self, _id):
        return self.model.delete(_id)

    def list_genre(self):
        return self.model.list_genre()

    def like(self, params):
        return self.model.like(params["album_id"], params["listener_id"])

    def unlike(self, params):
        return self.model.unlike(params["album_id"], params["listener_id"])
