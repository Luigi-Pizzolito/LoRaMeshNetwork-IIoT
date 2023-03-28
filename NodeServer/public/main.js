//Websocekt variables
const url = "ws://localhost:9876/myWebsocket"
const mywsServer = new WebSocket(url)

function unixTimeToString(unixTime) {
    const date = new Date(unixTime * 1000);
    const year = date.getFullYear();
    const month = ('0' + (date.getMonth() + 1)).slice(-2);
    const day = ('0' + date.getDate()).slice(-2);
    const hours = ('0' + date.getHours()).slice(-2);
    const minutes = ('0' + date.getMinutes()).slice(-2);
    const seconds = ('0' + date.getSeconds()).slice(-2);
    return `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
  }

var graphQueue = []

//enabling send message when connection is open
mywsServer.onopen = function () {
}

//handling message event
mywsServer.onmessage = function (event) {
    const { data } = event
    receivemsg(data)
}

function receivemsg(event){
    let graphDiv = graphQueue[0].graphDiv
    let field = graphQueue[0].field
    let container = graphQueue[0].container
    console.log(graphQueue)
    graphQueue.shift()
    console.log(graphQueue)
    setGraph(graphDiv, event, field, container)
}

function fetch(time_start, time_end){
    mywsServer.send('{"time_range":[' + time_start + ',' + time_end + ']}')
    
}


String.prototype.toProperCase = function () {
    return this.replace(/\w\S*/g, function(txt){return txt.charAt(0).toUpperCase() + txt.substr(1).toLowerCase();});
};


function loadJSONtoGraph(graph, json, field) {
    // transform data
    let obj = JSON.parse(json);

    let x = [];
    let y = [];
    obj.forEach(point => {
        x.push(unixTimeToString(point.ts));
        y.push(point.data[field]);
    });
    console.log(x, y);

    // load data to graph
    Plotly.react(graph, [{
        x: x,
        y: y
    }], {
        margin: { t: 50 },
        width: 1000,
        height: 400,
        title: {
            text: field.toProperCase()
        }
    });

}
function setGraph(graphDiv, jsondata, field, container){
    // add graph data
    loadJSONtoGraph(graphDiv, jsondata, field)
    //3.push div into container
    container.appendChild(graphDiv);
}

let fields = ["temperature", "humidity", "soil_moisture"]

function addGraphs(fields) {
    let container = document.getElementById("lineGraphContainer")
    fields.forEach(field => {
        // add graph to page
        //1.make new graph div element
        let graphDiv = document.createElement('div');
        //2.make new Plotly.newgraph
        Plotly.newPlot(graphDiv, [{}]);
        // push
        graphQueue.push({graphDiv:graphDiv, field:field, container:container})
        // fetch data 
        fetch(1679201350,1679403456)
        
    })
}

 

setTimeout(() => {
    addGraphs(fields);
}, 10);

setInterval(() => {
location.reload()
}, 10000);