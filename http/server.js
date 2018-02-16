/*
This program listens to TCP socket connections on port 8000.
For each connecting client, the program prints the data it receives
and then returns an HTTP response message that contains the word
'hi' in an H1 element.

Assuming this file is named 'server.js', run the following 
from the command line to start the server.

    node server.js

To test, go to http://localhost:8000/ in a browser.
*/

const net = require('net');

const server = net.createServer();

server.listen(8000, () => {
  console.log('Open the following URL in browser:');
  console.log('http://localhost:8000/');
  console.log('');
});

/* 
   The following string is the HTTP response message returned
   to connecting clients (browsers).  The structure of an HTTP response
   message is as follows:
       status line
       general headers
       blank line
       message data
   In an HTTP response message, a blank line ends 
   the msg header and starts the msg body.
*/
const response = 
  'HTTP/1.1 200 OK'          + '\r\n' +  // status line
  'Content-length: 11'       + '\r\n' +  // general header
  'Content-type: text/html'  + '\r\n' +  // general header
                               '\r\n' +  // blank line
  '<h1>hi</h1>';                         // message body (11 bytes)

server.on('connection', (socket) => {
  console.log('Something connected to me.');
  socket.on('data', (data) => {
    console.log('Received from client:\n' + data);
    socket.end(response); // Send response and close connection.
  });
});