from flask import Blueprint, request, jsonify, redirect, url_for, session
from services.listener_service import ListenerService

listener_bp = Blueprint("listener_bp", __name__)


@listener_bp.route('/list', methods=["GET"])
def list():
    return jsonify(ListenerService().list())


@listener_bp.route('/create', methods=["GET"])
def create():
    context = request.args
    return ListenerService().create(context)


@listener_bp.route('/delete', methods=["GET"])
def delete():
    return ListenerService().delete()


@listener_bp.route('/login', methods=["POST"])
def login():
    form = request.form
    listener_info = ListenerService().login(form)

    if listener_info is not None:
        session["id"] = listener_info["id"]
        session["username"] = listener_info["username"]
        session["email"] = listener_info["email"]
        session["type"] = "listener"
        session["state"] = True

        return redirect(url_for('index'))
    else:
        return redirect(url_for('login'))
