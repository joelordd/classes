const pg = require('pg');

process.env.PGDATABASE = 'db';

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
		  '  color char(6)			' +
		  ');					';
	client.query(q, (err) => {
		if (err) throw err; else insertA(client, done);
	});
}

function insertA(client, done) {
	insertRow(client, 'user', 123, 'FF0000', () => {
		//insertB(client, done);
		pool.end();
	});
}

/*function insertB(client, done) {
	inserRow(client, 'user2', 1234, '00FF00', () => {
		pool.end();
	});
}*/

function insertRow(client, username, password, color, cb) {
	client.query("insert into users values ($1::text, $2, $3::text)", [username, password, color], (err) => {
		if (err) throw err; else cb();
	});
]
