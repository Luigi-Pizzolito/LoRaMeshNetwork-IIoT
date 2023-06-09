const url = "ws://localhost:9876/myWebsocket"
const mywsServer = new WebSocket(url)

mywsServer.onopen = function () {
}

//handling message event
mywsServer.onmessage = function (event) {
    const { data } = event
    receiveMsg(data)
}

let pdata = {};
function receiveMsg(data) {
    let obj = JSON.parse(data);
    let fields = ["temperature", "humidity", "soil_moisture"];
    
    fields.forEach(field => {
        pdata[field] = [];
        obj.forEach(point => {
            pdata[field].push(point.data[field]);
        })
    })
    console.log(pdata);

    let table = "<table class=\"table\"><tr><th>Stats</th>";
    fields.forEach(field => {
        table +="<th scope=\"col\">"+field+"</th>";
    })
    table+="</tr>"
    table+="<tr><td>Mean</th>";
    fields.forEach(field => {
        table+="<td>"+findMean(pdata[field]).toFixed(2)+"</td>";
    })
    table+="</tr>"
    table+="<tr><td>Median</td>";
    fields.forEach(field => {
        table+="<td>"+findMedian(pdata[field]).toFixed(2)+"</td>";
    })
    table+="</tr>"
    table+="<tr><td>Max</td>";
    fields.forEach(field => {
        table+="<td>"+findMax(pdata[field]).toFixed(2)+"</td>";
    })
    table+="</tr>"
    table+="<tr><td>Min</td>";
    fields.forEach(field => {
        table+="<td>"+findMin(pdata[field]).toFixed(2)+"</td>";
    })
    table+="</tr>"
    table+="<tr><td>St. Dev.</td>";
    fields.forEach(field => {
        table+="<td>"+findSD(pdata[field]).toFixed(2)+"</td>";
    })
    table+="</tr>"
    table+="</table>"

    var y0 = pdata["temperature"];
    var y1 = pdata["humidity"];
    var y2 = pdata["soil_moisture"];
    var trace0 = {
        y: y0,
        type: 'box',
        name: "Temperature"
    };
    var trace1 = {
        y: y1,
        type: 'box',
        name: "Humidity"
    };
    var trace2 = {
        y: y2,
        type: 'box',
        name: "Soil Moisture"
    };
    data = [trace0, trace1, trace2];
    Plotly.newPlot('myDiv', data);

////////////////////////////////
    //fields.forEach(field => {
        // let graphDiv = document.createElement('div');
        // y0= pdata["humidity"];
        // console.log(y0)
        // var data = [{
        //     y: y0,
        //     type: 'box'
        // }];

        // var layout = {
        //     //name: JSON.stringify(pdata.field)
        // };
        // Plotly.newPlot(graphDiv, data, layout);
    //});
///////////////////////////////


    function createElementFromHTML(htmlString) {
        var div = document.createElement('div');
        div.innerHTML = htmlString.trim();
        // Change this to div.childNodes to support multiple top-level nodes.
        return div.firstChild;
      }
    document.body.appendChild(createElementFromHTML(table)); 
}

setTimeout(() => {
    mywsServer.send('{"time_range":[1679201350,1679403456]}')
}, 1000);
