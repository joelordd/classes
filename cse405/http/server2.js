const http = require('http');

const hostname = 'localhost';
const port = 8000;
const html =
  "<html>"                           +
  "<head><title>405</title></head>"  +
  "<body><h1>hi</h1></body>"         +
  "</html>";

const server = http.createServer((req, res) => {
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/html');
    res.end(html);
});

server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});