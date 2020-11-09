import pymysql

TABLENAME = "artist_table"


class ArtistModel:

    def __init__(self):
        self.connection = self.conn = pymysql.connect(
            "localhost", "root", "", "dbtify")
        self.cursor = self.connection.cursor()

    def __del__(self):
        self.connection.commit()
        self.connection.close()

    def create(self, name, surname):

        query = f"""
        INSERT IGNORE INTO {TABLENAME} 
        (name, surname) 
        VALUES ("{name}","{surname}");
        """

        result = self.cursor.execute(query)

        return "OK"

    def list(self):

        query = f"""
        SELECT 
        artist_table.*,
        SUM(
        (SELECT COUNT(*)
                FROM song_like_table
                WHERE song_like_table.song_id = song_artist_table.song_id
                )) AS total_likes
        FROM artist_table
        LEFT JOIN song_artist_table ON artist_table.id = song_artist_table.artist_id
        GROUP BY artist_table.id;
        """

        self.cursor.execute(query)

        artists = self.cursor.fetchall()

        result = [{
            "id": artist[0],
            "name": artist[1],
            "surname": artist[2],
            "total_likes": artist[3]
        }for artist in artists]

        return result

    def get_by_id(self, _id):

        query = f"""
        SELECT 
        artist_table.*,
        SUM(
        (SELECT COUNT(*)
                FROM song_like_table
                WHERE song_like_table.song_id = song_artist_table.song_id
                )) AS total_likes
        FROM artist_table
        LEFT JOIN song_artist_table ON artist_table.id = song_artist_table.artist_id
        WHERE artist_table.id = {_id} 
        GROUP BY artist_table.id;
        """

        self.cursor.execute(query)

        artist = self.cursor.fetchall()[0]

        result = {
            "id": artist[0],
            "name": artist[1],
            "surname": artist[2],
            "total_likes": artist[3]
        }

        return result

    def list_coartist(self, name, surname):

        query = f"""
        call dbtify.procedure_coartist("{name}", "{surname}");
        """

        self.cursor.execute(query)

        artists = self.cursor.fetchall()

        result = [{
            "id": artist[0],
            "name": artist[1],
            "surname": artist[2],
            "total_likes": artist[3]
        }for artist in artists]

        return result

    def list_by_popularity(self):

        query = f"""
        SELECT 
        artist_table.*,
        SUM(
        (SELECT COUNT(*)
                FROM song_like_table
                WHERE song_like_table.song_id = song_artist_table.song_id
                )) AS total_likes
        FROM artist_table
        LEFT JOIN song_artist_table ON artist_table.id = song_artist_table.artist_id
        GROUP BY artist_table.id
        ORDER BY total_likes DESC;
        """

        self.cursor.execute(query)

        artists = self.cursor.fetchall()

        result = [{
            "id": artist[0],
            "name": artist[1],
            "surname": artist[2],
            "total_likes": artist[3]
        }for artist in artists]

        return result

    def delete(self):

        query = f"""
        DELETE
        FROM {TABLENAME};
        """

        self.cursor.execute(query)

        return "OK"

    def login(self, name, surname):

        self.create(name, surname)

        query = f"""    
        SELECT *
        FROM {TABLENAME}
        WHERE name="{name}" AND surname="{surname}"
        """
        self.cursor.execute(query)

        artist = self.cursor.fetchall()

        if len(artist) == 0:
            return None
        else:
            artist = artist[0]

        result = {"id": artist[0],
                  "name": artist[1],
                  "surname": artist[2]}

        return result
