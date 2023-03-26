let data = JSON.parse(`[{"id":3,"ts":1678798038,"data":{"temperature":29.11,"humidity":62.53,"soil_moisture":55.4}},
{"id":1,"ts":1678799838,"data":{"temperature":25.12,"humidity":72.97,"soil_moisture":32.66}},
{"id":2,"ts":1678801638,"data":{"temperature":18.71,"humidity":74.77,"soil_moisture":55.17}},
{"id":1,"ts":1678803438,"data":{"temperature":29.62,"humidity":58.89,"soil_moisture":23.95}},
{"id":0,"ts":1678805238,"data":{"temperature":27.21,"humidity":63.2,"soil_moisture":41.42}},
{"id":2,"ts":1678807038,"data":{"temperature":26.6,"humidity":48.08,"soil_moisture":49.48}},
{"id":0,"ts":1678808838,"data":{"temperature":15.48,"humidity":45.97,"soil_moisture":42.76}},
{"id":3,"ts":1678810638,"data":{"temperature":24.91,"humidity":46.62,"soil_moisture":22.69}},
{"id":3,"ts":1678812438,"data":{"temperature":22.63,"humidity":70.8,"soil_moisture":34.97}},
{"id":1,"ts":1678814238,"data":{"temperature":15.82,"humidity":67.84,"soil_moisture":31.93}},
{"id":3,"ts":1678816038,"data":{"temperature":22.32,"humidity":53.04,"soil_moisture":42.72}},
{"id":2,"ts":1678817838,"data":{"temperature":17.46,"humidity":40.67,"soil_moisture":24.03}},
{"id":2,"ts":1678819638,"data":{"temperature":19.01,"humidity":71.34,"soil_moisture":53.51}},
{"id":3,"ts":1678821438,"data":{"temperature":18.74,"humidity":48.49,"soil_moisture":50.97}},
{"id":0,"ts":1678823238,"data":{"temperature":19.97,"humidity":42.11,"soil_moisture":29.87}},
{"id":3,"ts":1678825038,"data":{"temperature":18.42,"humidity":66.4,"soil_moisture":45.89}},
{"id":2,"ts":1678826838,"data":{"temperature":18.37,"humidity":51.59,"soil_moisture":34.62}},
{"id":2,"ts":1678828638,"data":{"temperature":28.6,"humidity":46.11,"soil_moisture":46.26}},
{"id":3,"ts":1678830438,"data":{"temperature":18.74,"humidity":46.17,"soil_moisture":39.01}}]`);

const tempArr = [];
const humidityArr = [];
const moistArr = [];
let n=0;
data.forEach(datapoint => {
    n++;
    tempArr.push(datapoint.data.temperature);
    humidityArr.push(datapoint.data.humidity);
    moistArr.push(datapoint.data.soil_moisture);
  });

//Get mean value
function findMean(arr,n) {
    let sum = 0;
    for (let i = 0; i < n; i++)
        sum += arr[i];
    return sum/n;
};

//Get median value
function findMedian(arr,n) {
    const middle = (n + 1) / 2;
        const sorted = [...arr].sort((a, b) => a - b);
        const isEven = n % 2 === 0;
        return isEven ? (sorted[middle - 1.5] + sorted[middle - 0.5]) / 2 : sorted[middle - 1];
};

//Get min & max values
function findMin(arr,n) {
    let min=arr[0];
    for (let i = 1; i < n; i++) { 
        if (arr[i] < min) 
           min = arr[i]; 
      } 
      return min;
}
function findMax(arr,n) {
    let  max=arr[0];
    for (let i = 1; i < n; i++) { 
        if (arr[i] > max) 
           max = arr[i]; 
      } 
      return max;
}

//Get standard deviation value
function findSD (arr,n) {
    const mean = arr.reduce((a, b) => a + b) / n
    return Math.sqrt(arr.map(x => Math.pow(x - mean, 2)).reduce((a, b) => a + b) / n)
  }