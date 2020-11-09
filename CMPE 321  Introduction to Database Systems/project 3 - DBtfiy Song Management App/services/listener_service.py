from models.listener_model import ListenerModel


class ListenerService:
    def __init__(self):
        self.model = ListenerModel()

    def create(self, params):
        return self.model.create(params["username"], params["email"])

    def list(self):
        return self.model.list()

    def delete(self):
        return self.model.delete()

    def login(self, params):
        return self.model.login(params["username"], params["email"])

    def get_by_id(self, _id):
        return self.model.get_by_id(_id)
