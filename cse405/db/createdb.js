const pg = require('pg');

process.env.PGDATABASE = 'db';
process.env.PGPASSWORD = 'password';

const pool = new pg.Pool();

pool.on('error', (err, client) => {
	console.log(err.stack);
	process.exit(-1);
});

pool.connect((err, client, done) => {
	if (err) throw err;
	createUsers(client, done);
});

function createUsers(client, done) {
	const q = 'create table users (			' +
		  '  username varchar(255) primary key, ' +
		  '  password varchar(255) not null,	' +
		  '  color char(6),			' +
		  '  paragraph text			' +
		  ')					';
	client.query(q, (err) => {
		if (err) throw err;
		insertUser(client, done);
	});
}

function insertUser(client, done) {
	const q = "insert into users values ('fred', '1234', '0000FF', 'This is your textbox.')";
	client.query(q, (err) => {
		if (err) throw err;
	 	done();
		pool.end();
	});
}
