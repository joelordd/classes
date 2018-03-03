const http = require('http');
const fs = require('fs');
const qs = require('querystring');
const db = require('./db');
const sessions = require('./sessions');

const server = http.createServer();

port = 8000;

server.listen(port, () => {
        console.log(`Server running at http:localhost${port}`);
});

var message = '';

const homeTemplate = fs.readFileSync('views/home.html', 'utf8');
const colorTemplate = fs.readFileSync('views/color.html', 'utf8');
const loginTemplate = fs.readFileSync('views/login.html', 'utf8');
const paragraphTemplate = fs.readFileSync('views/peragraph.html', 'utf8');

//peragraph.html and handleParagraphPage are for my benefit, they have nothing to do with this lab.
server.on('request', (req, res) => {
    sessions.filter(req, res);
    
    if (req.url === '/login.html') {
        handleLoginPage(req, res);
    } else if (req.url === '/login') {
        handleLogin(req, res);
    } else if(!req.session.hasOwnProperty('username')) {
        redirectLoginPage(res);
    } else if(req.url === '/logout') {
        handleLogout(req, res);
    } else if (req.url === '/color') {
        handleFormColor(req, res);
    } else if (req.url === '/color.html') {
        handleColorPage(req, res);
    } else if (req.url === '/peragraph') {
	handleFormParagraph(req, res);
    } else if (req.url === '/peragraph.html') {
	handleParagraphPage(req, res);
    } else if (req.url === "/") {
        handleHomePage(req, res);
    } else {
        res.writeHead(404);
        res.end('Not Found');
    }
});

function handleHomePage(req, res) {
    var html;
    res.setHeader('Content-type', 'text/html');

    db.getUserColor(req.session.username, (color) => {
        var html;
        res.setHeader('Content-type', 'text/html');

	html = homeTemplate.replace('#color#', color);
	db.getUserParagraph(req.session.username, (paragraph) => {
	    html = html.replace('#paragraph#', paragraph);
	    res.end(html);
	});
    });
}

function handleColorPage(req, res) {
    db.getUserColor(req.session.username, (color) => {
        res.setHeader('Content-type', 'text/html');
        var html;
        switch (color) {
            case 'FF0000':
                html = colorTemplate.replace('#red#'   , 'checked');
                html = html         .replace('#green#' , '       ');
                html = html         .replace('#blue#'  , '       ');
                break;
            case '00FF00':
                html = colorTemplate.replace('#red#'   , '       ');
                html = html         .replace('#green#' , 'checked');
                html = html         .replace('#blue#'  , '       ');
                break;
            case '0000FF':
                html = colorTemplate.replace('#red#'   , '       ');
                html = html         .replace('#green#' , '       ');
                html = html         .replace('#blue#'  , 'checked');
                break;
            default:
                html = colorTemplate.replace('#red#'   , '       ');
                html = html         .replace('#green#' , '       ');
                html = html         .replace('#blue#'  , '       ');
                break;
        }
        res.end(html);
    });
}

function handleLoginPage(req, res) {
    res.setHeader('Content-type', 'text/html');
    const html = loginTemplate.replace('#message#', message);
    res.end(html);
}

function handleParagraphPage(req, res) {
     db.getUserParagraph(req.session.username, (paragraph) => {
        res.setHeader('Content-type', 'text/html');
        const html = paragraphTemplate.replace('#paragraph#', paragraph);
        res.end(html);
 });
}

function redirectHomePage(res) {
    res.writeHead(302, { 'Location': '/' });
    res.end();
}

function redirectLoginPage(res) {
    res.writeHead(301, { 'Location': '/login.html'} );
    res.end();
}

function handleFormColor(req, res) {
    console.log('\nForm data received:\n');
    console.log(req.headers);
    console.log('\n');
    let body = '';
    req.on('data', (chunk) => {
        body += chunk;
    });
    req.on('end', () => {
        const form = qs.parse(body);
        if (form.color !== 'FF0000' && form.color !== '00FF00' && form.color !== '0000FF') {
            res.writeHead(400);
            res.end('Bad request');
        }
        db.setUserColor(req.session.username, form.color, () => {
            res.writeHead(302, { 'Location': '/' });
            res.end();
        });
    });
}

function handleLogin(req, res) {
    console.log('\nForm data received:\n');
    console.log(req.headers);
    console.log('\n');
    let body = '';
    req.on('data', (chunk) => {
        body += chunk;
    });
    req.on('end', () => {
        const form = qs.parse(body);
        if (form.username !== 'fred') {
            message = 'Bad Username'
            redirectLoginPage(res);
        } else if (form.password !== '1234'){
            message = 'Bad Password';
            redirectLoginPage(res);
        } else {
            req.session.username = form.username;
            redirectHomePage(res);
        }
    });
}

function handleFormParagraph(req, res) {
    console.log('\nForm data received:\n');
    console.log(req.headers);
    console.log('\n');
    let body = '';
    req.on('data', (chunk) => {
        body += chunk;
    });
    req.on('end', () => {
        const form = qs.parse(body);
        if (!form.paragraph) {
            res.writeHead(400);
            res.end('Bad request');
        }
        db.setUserParagraph(req.session.username, form.paragraph, () => {
            res.writeHead(302, { 'Location': '/' });
            res.end();
        });
    });

}

function handleLogout(req, res) {
    delete req.session.username;
    message = '';
    redirectLoginPage(res);
}

