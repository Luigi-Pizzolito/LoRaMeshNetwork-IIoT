const NoSQL = require('nosql');

class DBBridge {
    //Contructor to initialise database
    constructor(dbpath) {
        this.db = NoSQL.load(dbpath);
        console.log("DB " + dbpath + " initialised.");
    }

    // Methods for accepting queries
    requestQuery(query, callback) {
        // parse query request into object
        let queryObj = {}
        try {
            queryObj = JSON.parse(query);
        } catch (error) {
            // console.warn("Invalid JSON WebSock Request: ", error);
            callback(error, {});
            return;
        }


        let start_time = queryObj.time_range[0];
        let end_time = queryObj.time_range[1];
        // let sensors = queryObj.sensors; //!TODO: make this iterative filter later if we need it

        if (queryObj.hasOwnProperty('id')) {
            let id = queryObj.id;
            this.db.find().make(function (filter) {
                filter.where('id', '=', id)
                filter.where('ts', '>', start_time)
                filter.where('ts', '<', end_time)
                filter.callback(callback);
            });
        } else {
            this.db.find().make(function (filter) {
                filter.where('ts', '>', start_time)
                filter.where('ts', '<', end_time)
                filter.callback(callback);
            });
        }
    }

    // Method for adding DB entries
    addEntry(entry) {
        this.db.insert(entry);
    }
};

module.exports = {
    DBBridge: DBBridge
};