import pymysql

TABLENAME = "album_table"


class AlbumModel:

    def __init__(self):
        self.connection = self.conn = pymysql.connect(
            "localhost", "root", "", "dbtify")
        self.cursor = self.connection.cursor()

    def __del__(self):
        self.connection.commit()
        self.connection.close()

    def create(self, title, genre, artist_id):

        try:
            query = f"""
            INSERT INTO {TABLENAME}
            (title, genre, artist_id)
            VALUES ("{title}","{genre}","{artist_id}");
            """
            self.cursor.execute(query)

            query = f"""
            SELECT id 
            FROM {TABLENAME}
            WHERE title="{title}" AND genre="{genre}" AND artist_id="{artist_id}";
            """
            self.cursor.execute(query)

            album_id = self.cursor.fetchone()[0]
        except:
            album_id = None

        return album_id

    def update(self, _id, title, genre):

        try:
            query = f"""
            UPDATE {TABLENAME}
            SET title="{title}", genre="{genre}"
            WHERE id="{_id}"; 
            """
            self.cursor.execute(query)
        except:
            pass

    def list(self, current_user):

        query = f"""
        SELECT 
        album_table.id AS album_id,
        album_table.title AS album_title,
        album_table.genre AS album_genre,
        CONCAT(artist_table.name, ' ',artist_table.surname) AS artist,
        EXISTS(SELECT *
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                AND album_like_table.listener_id = "{current_user}") AS liked,
        (SELECT COUNT(*)
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                ) AS total_likes
        FROM album_table
        INNER JOIN artist_table ON album_table.artist_id = artist_table.id
        LEFT JOIN album_like_table ON album_like_table.album_id = album_table.id;
        """

        self.cursor.execute(query)

        albums = self.cursor.fetchall()

        result = [{
            "album_id": album[0],
            "album_title": album[1],
            "album_genre": album[2],
            "artist": album[3],
            "is_liked": album[4],
            "total_likes": album[5]
        } for album in albums]

        return result

    def list_by_popularity(self, current_user):

        query = f"""
        SELECT 
        album_table.id AS album_id,
        album_table.title AS album_title,
        album_table.genre AS album_genre,
        CONCAT(artist_table.name, ' ',artist_table.surname) AS artist,
        EXISTS(SELECT *
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                AND album_like_table.listener_id = "{current_user}") AS liked,
        (SELECT COUNT(*)
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                ) AS total_likes
        FROM album_table
        INNER JOIN artist_table ON album_table.artist_id = artist_table.id
        LEFT JOIN album_like_table ON album_like_table.album_id = album_table.id
        ORDER BY total_likes DESC;
        """

        self.cursor.execute(query)

        albums = self.cursor.fetchall()

        result = [{
            "album_id": album[0],
            "album_title": album[1],
            "album_genre": album[2],
            "artist": album[3],
            "is_liked": album[4],
            "total_likes": album[5]
        } for album in albums]

        return result

    def list_by_artist_id(self, artist_id, current_user):

        query = f"""
        SELECT 
        album_table.id AS album_id,
        album_table.title AS album_title,
        album_table.genre AS album_genre,
        CONCAT(artist_table.name, ' ',artist_table.surname) AS artist,
        EXISTS(SELECT *
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                AND album_like_table.listener_id = "{current_user}") AS liked,
        (SELECT COUNT(*)
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                ) AS total_likes
        FROM album_table
        INNER JOIN artist_table ON album_table.artist_id = artist_table.id
        LEFT JOIN album_like_table ON album_like_table.album_id = album_table.id
        WHERE artist_table.id = "{artist_id}";
        """

        self.cursor.execute(query)

        albums = self.cursor.fetchall()

        result = [{
            "album_id": album[0],
            "album_title": album[1],
            "album_genre": album[2],
            "artist": album[3],
            "is_liked": album[4],
            "total_likes": album[5]
        } for album in albums]

        return result

    def list_liked(self, current_user):

        query = f"""
        SELECT 
        album_table.id AS album_id,
        album_table.title AS album_title,
        album_table.genre AS album_genre,
        CONCAT(artist_table.name, ' ',artist_table.surname) AS artist,
        EXISTS(SELECT *
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                AND album_like_table.listener_id = "{current_user}") AS liked,
        (SELECT COUNT(*)
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                ) AS total_likes
        FROM album_table
        INNER JOIN artist_table ON album_table.artist_id = artist_table.id
        LEFT JOIN album_like_table ON album_like_table.album_id = album_table.id
        WHERE album_like_table.listener_id = "{current_user}";
        """

        self.cursor.execute(query)

        albums = self.cursor.fetchall()

        result = [{
            "album_id": album[0],
            "album_title": album[1],
            "album_genre": album[2],
            "artist": album[3],
            "is_liked": album[4],
            "total_likes": album[5]
        } for album in albums]

        return result

    def get_by_id(self, album_id, current_user):

        query = f"""
        SELECT 
        album_table.id AS album_id,
        album_table.title AS album_title,
        album_table.genre AS album_genre,
        CONCAT(artist_table.name, ' ',artist_table.surname) AS artist,
        EXISTS(SELECT *
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                AND album_like_table.listener_id = "{current_user}") AS liked,
        (SELECT COUNT(*)
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                ) AS total_likes
        FROM album_table
        INNER JOIN artist_table ON album_table.artist_id = artist_table.id
        LEFT JOIN album_like_table ON album_like_table.album_id = album_table.id
        WHERE album_table.id={album_id}
        LIMIT 1;
        """

        self.cursor.execute(query)

        album = self.cursor.fetchall()[0]

        result = {
            "album_id": album[0],
            "album_title": album[1],
            "album_genre": album[2],
            "artist": album[3],
            "is_liked": album[4]
        }

        return result

    def list_by_genre(self, genre, current_user):

        query = f"""
        SELECT 
        album_table.id AS album_id,
        album_table.title AS album_title,
        album_table.genre AS album_genre,
        CONCAT(artist_table.name, ' ',artist_table.surname) AS artist,
        EXISTS(SELECT *
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                AND album_like_table.listener_id = "{current_user}") AS liked,
        (SELECT COUNT(*)
                FROM album_like_table
                WHERE album_like_table.album_id = album_table.id
                ) AS total_likes
        FROM album_table
        INNER JOIN artist_table ON album_table.artist_id = artist_table.id
        LEFT JOIN album_like_table ON album_like_table.album_id = album_table.id
        WHERE album_table.genre="{genre}";
        """

        self.cursor.execute(query)

        albums = self.cursor.fetchall()

        result = [{
            "album_id": album[0],
            "album_title": album[1],
            "album_genre": album[2],
            "artist": album[3],
            "is_liked": album[4]
        } for album in albums]

        return result

    def list_genre(self):
        query = f"""
        SELECT
        album_table.genre
        FROM album_table
        GROUP BY album_table.genre;
        """

        self.cursor.execute(query)

        genres = self.cursor.fetchall()

        genres = [genre[0] for genre in genres]

        return genres

    def delete(self, _id):

        query = f"""
        DELETE
        FROM {TABLENAME}
        WHERE id={_id};
        """

        self.cursor.execute(query)

        return "OK"

    def like(self, album_id, listener_id):

        query = f"""    
        INSERT INTO album_like_table
        (album_id, listener_id) 
        VALUES ("{album_id}","{listener_id}");
        """
        self.cursor.execute(query)

        return "OK"

    def unlike(self, album_id, listener_id):

        query = f"""    
        DELETE 
        FROM album_like_table
        WHERE album_id='{album_id}' AND listener_id='{listener_id}';
        """
        self.cursor.execute(query)

        return "OK"
