from models.artist_model import ArtistModel


class ArtistService:
    def __init__(self):
        self.model = ArtistModel()

    def create(self, params):
        return self.model.create(params["name"], params["surname"])

    def list(self):
        return self.model.list()

    def list_by_popularity(self):
        return self.model.list_by_popularity()

    def list_coartist(self, name, surname):
        return self.model.list_coartist(name, surname)

    def delete(self):
        return self.model.delete()

    def login(self, params):
        return self.model.login(params["name"], params["surname"])

    def get_by_id(self, _id):
        return self.model.get_by_id(_id)
