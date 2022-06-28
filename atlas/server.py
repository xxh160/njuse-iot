#! /bin/env/python

from flask import Flask, request, jsonify
from flask_restful import reqparse
import os

app = Flask(__name__)
app.debug = True


@app.route('/register', methods=['post'])
def register():
    request.get_json(force=True)
    parser = reqparse.RequestParser()
    parser.add_argument('id', required=True)
    args = parser.parse_args()
    id = args['id']

    print("registering device:", id)

    returnData = "device registered"
    return returnData, 201


@app.route('/api/<id>/led', methods=['put'])
def led(id):
    print("requesting device:", id)

    idx = request.json.get("idx")

    os.system("bash led.sh " + str(idx))
    res = {'idx': idx}
    return jsonify(res), 200


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
