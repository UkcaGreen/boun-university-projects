import pymysql
TABLENAME = "listener_table"


class ListenerModel:

    def __init__(self):
        self.connection = self.conn = pymysql.connect(
            "localhost", "root", "", "dbtify")
        self.cursor = self.connection.cursor()

    def __del__(self):
        self.connection.commit()
        self.connection.close()

    def create(self, username, email):

        query = f"""
        INSERT IGNORE INTO {TABLENAME} 
        (username, email) 
        VALUES ("{username}","{email}");
        """
        self.cursor.execute(query)

        return "OK"

    def list(self):

        query = f"""
        SELECT *
        FROM {TABLENAME};
        """

        self.cursor.execute(query)

        listeners = self.cursor.fetchall()

        result = [{
            "id": listener[0],
            "username": listener[1],
            "email": listener[2]
        } for listener in listeners]

        return result

    def get_by_id(self, _id):

        query = f"""
        SELECT *
        FROM {TABLENAME}
        WHERE id = {_id}
        LIMIT 1;
        """

        self.cursor.execute(query)

        listener = self.cursor.fetchall()[0]

        result = {
            "id": listener[0],
            "username": listener[1],
            "email": listener[2]
        }

        return result

    def delete(self):

        query = f"""
        DELETE
        FROM {TABLENAME};
        """

        self.cursor.execute(query)

        return "OK"

    def login(self, username, email):

        self.create(username, email)

        query = f"""    
        SELECT *
        FROM {TABLENAME}
        WHERE username="{username}" AND email="{email}"
        """
        self.cursor.execute(query)

        listener = self.cursor.fetchall()

        if len(listener) == 0:
            return None
        else:
            listener = listener[0]

        result = {"id": listener[0],
                  "username": listener[1],
                  "email": listener[2]}

        return result
