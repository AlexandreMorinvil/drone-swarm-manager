FROM python:3.8-buster

WORKDIR /usr/src/server

RUN apt-get update && \
    apt-get install pipenv -y && \
    export SHELL=/bin/bash


#     apt install python3-pip -y && \
#     python3 -m pip install --no-cache-dir --upgrade pip setuptools && \
#     pip install --user --no-cache-dir pipenv && \
#     apt-get install pipenv -y

# RUN pipenv --rm && \
#     pipenv --three install && \
#     pipenv update --clear



COPY Pipfile ./
# RUN pipenv install

# COPY . .
# EXPOSE 4200
# CMD pipenv shell && \
#     cd src/ \
#     python3 ./server.py