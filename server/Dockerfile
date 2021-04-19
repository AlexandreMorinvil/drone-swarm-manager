FROM python:3.8-buster

WORKDIR /usr/src/server

RUN apt-get update && \
    apt-get install pipenv -y && \
    export SHELL=/bin/bash

COPY Pipfile ./
COPY Pipfile.lock ./
RUN pipenv install --system --deploy --ignore-pipfile

COPY . .
EXPOSE 5000

CMD pipenv shell && \
    cd src/ && \
    python3 ./server.py simulation