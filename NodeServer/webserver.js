const express = require('express');

// Class for implementing web server that sends our PWA to the browser.
class StaticWebServer {
    // Constructor runs when a WebServer class is initialised to setup variables and start
    constructor(host, port, dir) {
        // Initilise variables in the constructor
        this.host = host;
        this.port = port;
        this.dir  = dir;
        // Initialise server variables
        this.app = express();
        this.app.use(express.static(dir));
    }

    // Method to start server
    start() {
        // Start the web server
        this.app.listen(this.port, this.host, this.serverStarted(this));
    }

    // Method to log when server starts
    serverStarted() {
        console.log(`Web Server listening on http://${this.host}:${this.port}`);
    }
    //todo: add method to log server requests
}

module.exports = {
    StaticWebServer: StaticWebServer
};