from flask import Blueprint, request, jsonify, redirect, session
from services.album_service import AlbumService
from services.song_service import SongService

album_bp = Blueprint("album_bp", __name__)


@album_bp.route('/list', methods=["GET"])
def list():
    return jsonify(AlbumService().list())


@album_bp.route('/create', methods=["POST"])
def create():
    context = request.form.to_dict()
    context["other_artists"] = request.form.getlist("other_artists")
    context["album_id"] = AlbumService().create(context)
    context["song_id"] = SongService().create(context)
    return redirect(request.referrer)


@album_bp.route('/update', methods=["POST"])
def update():
    context = request.form.to_dict()
    print(context)
    AlbumService().update(context)
    return redirect(request.referrer)


@album_bp.route('/delete/<_id>', methods=["GET"])
def delete(_id):
    AlbumService().delete(_id)
    return redirect(request.referrer)


@album_bp.route('/like/<album_id>', methods=["GET"])
def like(album_id):
    form = request.form.to_dict()
    form["album_id"] = album_id
    form["listener_id"] = session["id"]
    AlbumService().like(form)
    return redirect(request.referrer)


@album_bp.route('/unlike/<album_id>', methods=["GET"])
def unlike(album_id):
    form = request.form.to_dict()
    form["album_id"] = album_id
    form["listener_id"] = session["id"]
    AlbumService().unlike(form)
    return redirect(request.referrer)
