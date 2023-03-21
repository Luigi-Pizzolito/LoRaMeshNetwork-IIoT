var NoSQL = require('nosql');
var db = NoSQL.load('database.nosql');

var fs = require('fs');
 
// db === Database instance <https://docs.totaljs.com/latest/en.html#api~Database>

// insert
/*
db.insert({"age": 18}, true);
db.insert({"age": 22}, true);
db.insert({"age": 28}, true);
db.insert({"age": 37}, true);
*/

// filter and return array
function timeConverter(UNIX_timestamp){
    var a = new Date(UNIX_timestamp * 1000);
    var months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
    var year = a.getFullYear();
    var month = months[a.getMonth()];
    var date = a.getDate();
    var hour = a.getHours();
    var min = a.getMinutes();
    var sec = a.getSeconds();
    var time = date + ' ' + month + ' ' + year + ' ' + hour + ':' + min + ':' + sec ;
    return time;
  }

// db.find().make(function(filter) {
//     filter.where('ts', '>', 1679201350);
//     filter.callback(function(err, response) {
//         if (err == null) {
//             // console.log(response);
//             response.forEach(entry => {
//                 console.log(timeConverter(entry.ts));
//                 // console.log(entry);
//             });
//         }
//     });
// });


// read fakedata and copy to test db
const fakedatastring = fs.readFileSync("../DBfakedataGen/data.json");
const fakedata = JSON.parse(fakedatastring.toString());
fakedata.forEach(entry => {
    db.insert(entry);
});