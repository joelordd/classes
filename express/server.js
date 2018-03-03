const http = require('http');
const express = require('express')
const qs = require('querystring');
const db = require('./db');
const auth = require('./auth');
const engine = require('./engine');
const sessions = require('./sessions');

const app = express();
const static = express.static('public');
const urlencodedParser = express.urlencoded({ extended: false });

app.use(static);
app.use(auth);
app.engine('html', engine);
app.set('views', './views');

app.get('/', function(req, res) { 
	const username = req.session.username;
	
	db.getUserColor(username, (color) => {
		db.getUserParagraph(username, (paragraph) => {
			const params =  { color: color, paragraph: paragraph };
			res.render('home.html', { params: params });
		});
	});
});

app.get('/color.html', function(req, res) { 
	const username = req.session.username;
	
	db.getUserColor(username, (color) => {
		const params =  { color: color, red: '', green: '', blue: '' };
        	
		switch (color) {
            		case 'FF0000':
				params.red = 'checked';
                		break;
            		case '00FF00':
				params.green = 'checked';
				break;
            		case '0000FF':
				params.blue = 'checked';
                		break;
		}

		res.render('color.html', { params: params });
	});
});

app.post('/color', urlencodedParser, (req, res) => {
	const username = req.session.username;
	const color = req.body.color;
	
	db.setUserColor(username, color, () => {
		res.redirect('/');
	}); 
});

app.post('/peragraph', urlencodedParser, (req, res) => {
	const username = req.session.username;
	const paragraph = req.body.paragraph;
	
	db.setUserParagraph(username, paragraph, () => {
		res.redirect('/');
	}); 
});



const port = 8000;
app.listen(port, () => {
        console.log(`Server running at http:localhost:${port}`);
});

