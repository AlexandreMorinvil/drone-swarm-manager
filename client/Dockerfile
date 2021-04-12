FROM node:15-alpine3.10

RUN mkdir -p /usr/src/client
WORKDIR /usr/src/client

COPY package*.json ./
RUN npm install
COPY . .
EXPOSE 4200
CMD [ "npm", "start" ]