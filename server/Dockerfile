FROM python:3.8-buster

WORKDIR /usr/src/server

RUN pip3 install requests flask python-socketio flask-socketio flask-restful cflib
COPY . .
EXPOSE 5000

CMD cd src/ && \
    python3 ./server.py simulation
    