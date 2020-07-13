import pymysql

databaseServerIP = "127.0.0.1"
databaseUserName = "root"
databaseUserPassword = ""
databaseName = "dbtify"


class Schema:
    def __init__(self):
        self.conn = pymysql.connect(databaseServerIP,
                                    databaseUserName,
                                    databaseUserPassword)
        self.cursor = self.conn.cursor()
        self.create_database()
        self.create_listener_table()
        self.create_artist_table()
        self.create_album_table()
        self.create_song_table()
        self.create_song_artist_table()
        self.create_song_like_table()
        self.create_album_like_table()
        self.create_trigger_delete_song()
        self.create_trigger_delete_song_artist_relation()
        self.create_trigger_remove_deleted_song_from_likes()
        self.create_trigger_like_songs_of_album()
        self.create_trigger_remove_deleted_album_from_likes()
        self.create_procedure_coartist()

    def __del__(self):
        self.conn.commit()
        self.conn.close()

    def create_database(self):
        try:
            query = f"""
            CREATE DATABASE {databaseName}
            """
            self.cursor.execute(query)
        except Exception as e:
            print("Exeception occured:{}".format(e))
        self.conn.close()
        self.conn = pymysql.connect(databaseServerIP,
                                    databaseUserName,
                                    databaseUserPassword,
                                    databaseName)
        self.cursor = self.conn.cursor()

    def create_trigger_delete_song(self):
        try:
            query = """
            CREATE TRIGGER delete_song BEFORE DELETE ON album_table
            FOR EACH ROW BEGIN 
              DELETE FROM song_table WHERE song_table.album_id = OLD.id; 
            END
            """
            self.cursor.execute(query)
        except:
            pass

    def create_trigger_remove_deleted_song_from_likes(self):
        try:
            query = """
            CREATE TRIGGER remove_deleted_song_from_likes BEFORE DELETE ON song_table
            FOR EACH ROW BEGIN
              DELETE FROM song_like_table WHERE song_like_table.song_id = OLD.id;
            END
            """
            self.cursor.execute(query)
        except:
            pass

    def create_trigger_remove_deleted_album_from_likes(self):
        try:
            query = """
            CREATE TRIGGER remove_deleted_album_from_likes BEFORE DELETE ON album_table
            FOR EACH ROW BEGIN
              DELETE FROM album_like_table WHERE album_like_table.album_id = OLD.id;
            END
            """
            self.cursor.execute(query)
        except:
            pass

    def create_trigger_delete_song_artist_relation(self):
        try:
            query = """
            CREATE TRIGGER delete_song_artist BEFORE DELETE ON song_table
            FOR EACH ROW BEGIN 
              DELETE FROM song_artist_table WHERE song_artist_table.song_id = OLD.id; 
            END
            """
            self.cursor.execute(query)
        except:
            pass

    def create_trigger_like_songs_of_album(self):
        try:
            query = """
            CREATE TRIGGER like_songs_of_album AFTER INSERT ON album_like_table
            FOR EACH ROW BEGIN 
                INSERT IGNORE INTO song_like_table (song_id, listener_id)
                SELECT song_table.id, NEW.listener_id
                FROM song_table
                WHERE song_table.album_id = NEW.album_id; 
            END
            """
            self.cursor.execute(query)
        except:
            pass

    def create_song_table(self):
        try:
            query = """
            CREATE TABLE IF NOT EXISTS song_table (
              id INTEGER NOT NULL AUTO_INCREMENT,
              title varchar(64),
              album_id INTEGER,
              FOREIGN KEY(album_id) REFERENCES album_table(id),
              PRIMARY KEY(id)
            );
            """
            self.cursor.execute(query)
        except:
            pass

    def create_album_table(self):
        try:
            query = """
            CREATE TABLE IF NOT EXISTS album_table (
              id INTEGER NOT NULL AUTO_INCREMENT,
              title varchar(64),
              genre varchar(64),
              artist_id INTEGER,
              FOREIGN KEY(artist_id) REFERENCES artist_table(id),
              PRIMARY KEY(id)
            );
            """
            self.cursor.execute(query)
        except:
            pass

    def create_listener_table(self):
        try:
            query = """
            CREATE TABLE IF NOT EXISTS listener_table (
              id INTEGER NOT NULL AUTO_INCREMENT,
              username varchar(64),
              email varchar(64),
              PRIMARY KEY(id),
              UNIQUE(username),
              UNIQUE(email)
            );
            """
            self.cursor.execute(query)
        except:
            pass

    def create_artist_table(self):
        try:
            query = """
            CREATE TABLE IF NOT EXISTS artist_table (
              id INTEGER NOT NULL AUTO_INCREMENT,
              name varchar(64),
              surname varchar(64),
              PRIMARY KEY(id),
              UNIQUE full_name (name, surname)
            );
            """
            self.cursor.execute(query)
        except:
            pass

    def create_song_artist_table(self):
        try:
            query = """
            CREATE TABLE IF NOT EXISTS song_artist_table (
              id INTEGER NOT NULL AUTO_INCREMENT,
              song_id INTEGER,
              artist_id INTEGER,
              FOREIGN KEY(song_id) REFERENCES song_table(id),
              FOREIGN KEY(artist_id) REFERENCES artist_table(id),
              PRIMARY KEY(id),
              UNIQUE (artist_id, song_id)
            );
            """
            self.cursor.execute(query)
        except:
            pass

    def create_song_like_table(self):
        try:
            query = """
            CREATE TABLE IF NOT EXISTS song_like_table (
              id INTEGER NOT NULL AUTO_INCREMENT,
              listener_id INTEGER,
              song_id INTEGER,
              FOREIGN KEY(listener_id) REFERENCES listener_table(id),
              FOREIGN KEY(song_id) REFERENCES song_table(id),
              PRIMARY KEY(id),
              UNIQUE unique_values (listener_id, song_id)
            );
            """
            self.cursor.execute(query)
        except:
            pass

    def create_album_like_table(self):
        try:
            query = """
            CREATE TABLE IF NOT EXISTS album_like_table (
              id INTEGER NOT NULL AUTO_INCREMENT,
              listener_id INTEGER,
              album_id INTEGER,
              FOREIGN KEY(listener_id) REFERENCES listener_table(id),
              FOREIGN KEY(album_id) REFERENCES album_table(id),
              PRIMARY KEY(id),
              UNIQUE unique_values (listener_id, album_id)
            );
            """
            self.cursor.execute(query)
        except:
            pass

    def create_procedure_coartist(self):
        try:
            query = """
            CREATE PROCEDURE procedure_coartist(
            name_param varchar(64),
            surname_param varchar(64)
            ) 
            BEGIN
                SELECT
                at2.*,
                (
                SELECT 
                SUM(
                (SELECT COUNT(*)
                        FROM song_like_table
                        WHERE song_like_table.song_id = song_artist_table.song_id
                        )) AS total_likes
                FROM artist_table at0
                LEFT JOIN song_artist_table ON at0.id = song_artist_table.artist_id
                WHERE at0.id = sat2.artist_id
                GROUP BY at0.id
                ) AS total_likes
                FROM
                artist_table at1
                LEFT JOIN song_artist_table sat1 ON sat1.artist_id = at1.id
                LEFT JOIN song_artist_table sat2 ON sat1.song_id = sat2.song_id
                INNER JOIN artist_table at2 ON sat2.artist_id = at2.id
                WHERE 
                at1.name = name_param AND
                at1.surname = surname_param AND
                NOT at2.name = name_param AND
                NOT at2.surname = surname_param
                GROUP BY sat2.artist_id;
            END
            """
            self.cursor.execute(query)
        except:
            pass
