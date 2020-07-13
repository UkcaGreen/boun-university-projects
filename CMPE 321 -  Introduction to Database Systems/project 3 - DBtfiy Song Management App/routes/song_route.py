from flask import Blueprint, request, jsonify, redirect, session
from services.song_service import SongService

song_bp = Blueprint("song_bp", __name__)


@song_bp.route('/create', methods=["POST"])
def create():
    context = request.form.to_dict()
    context["other_artists"] = request.form.getlist("other_artists")
    SongService().create(context)
    return redirect(request.referrer)


@song_bp.route('/update', methods=["POST"])
def update():
    context = request.form.to_dict()
    context["other_artists"] = request.form.getlist("other_artists")
    SongService().update(context)
    return redirect(request.referrer)


@song_bp.route('/delete/<_id>', methods=["GET"])
def delete(_id):
    SongService().delete(_id)
    return redirect(request.referrer)


@song_bp.route('/like/<song_id>', methods=["GET"])
def like(song_id):
    form = request.form.to_dict()
    form["song_id"] = song_id
    form["listener_id"] = session["id"]
    SongService().like(form)
    return redirect(request.referrer)


@song_bp.route('/unlike/<song_id>', methods=["GET"])
def unlike(song_id):
    form = request.form.to_dict()
    form["song_id"] = song_id
    form["listener_id"] = session["id"]
    SongService().unlike(form)
    return redirect(request.referrer)
